#include "can.h"
#include "IfxCan.h"
#include "isr_priorities.h"

#define CAN_MAX_RX_SUBSCRIPTIONS 10


typedef struct
{
    IfxCan_Can_Config canConfig;                            /* CAN module configuration structure                   */
    IfxCan_Can canModule;                                   /* CAN module handle                                    */
    IfxCan_Can_Node canSrcNode;                             /* CAN source node handle data structure                */
    IfxCan_Can_Node canDstNode;                             /* CAN destination node handle data structure           */
    IfxCan_Can_NodeConfig canNodeConfig;                    /* CAN node configuration structure                     */
    IfxCan_Filter canFilter;                                /* CAN filter configuration structure                   */
    IfxCan_Message txMsg;                                   /* Transmitted CAN message structure                    */
    IfxCan_Message rxMsg;                                   /* Received CAN message structure                       */
    uint8 txData[8];                                        /* Transmitted CAN data array                           */
    uint8 rxData[8];                                        /* Received CAN data array                              */
} McmcanType;

typedef struct
{
    uint32 canId;
    Can_RxCallbackType callback;
} Can_RxSubscriptionType;

static McmcanType g_mcmcan; /* Global MCMCAN configuration and control structure    */

static Can_RxSubscriptionType g_rxSubscriptions[CAN_MAX_RX_SUBSCRIPTIONS] = {NULL};
static uint32 g_rxSubscriptionsCount = 0;
static boolean g_isInitialized = FALSE;

IFX_INTERRUPT(Can_Tx_Isr, 0, ISR_PRIORITY_CAN_TX);
void Can_Tx_Isr (void)
{
    /* Clear the "Transmission Completed" interrupt flag */
    IfxCan_Node_clearInterruptFlag(g_mcmcan.canSrcNode.node, IfxCan_Interrupt_transmissionCompleted);
}

IFX_INTERRUPT(Can_Rx_Isr, 0, ISR_PRIORITY_CAN_RX);
void Can_Rx_Isr (void)
{
    Can_FrameType frame;
    Can_Receive(&frame);
    
    for (uint32 i = 0; i < g_rxSubscriptionsCount; i++)
    {
        if (g_rxSubscriptions[i].canId== frame.id)
        {
            if (g_rxSubscriptions[i].callback != NULL)
            {
                g_rxSubscriptions[i].callback(&frame);
            }
            break;
        }
    }
}

boolean Can_RegisterRxCallback (uint32 canId, Can_RxCallbackType callback)
{
    if (g_rxSubscriptionsCount < CAN_MAX_RX_SUBSCRIPTIONS)
    {
        g_rxSubscriptions[g_rxSubscriptionsCount].canId = canId;
        g_rxSubscriptions[g_rxSubscriptionsCount].callback = callback;
        g_rxSubscriptionsCount++;
        return TRUE;
    }
    return FALSE;
}

/* Function to initialize MCMCAN module and nodes related for this application use case */
void Can_Init (CAN_BAUDRATES ls_baudrate, CAN_NODE CAN_Node)
{
    if (!g_isInitialized)
    {
        /* wake up transceiver (node 0) */
        IfxPort_setPinModeOutput(&MODULE_P20, 6, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
        MODULE_P20.OUT.B.P6 = 0;

        IfxCan_Can_initModuleConfig(&g_mcmcan.canConfig, &MODULE_CAN0);
        IfxCan_Can_initModule(&g_mcmcan.canModule, &g_mcmcan.canConfig);
        IfxCan_Can_initNodeConfig(&g_mcmcan.canNodeConfig, &g_mcmcan.canModule);

        switch (ls_baudrate)
        {
            case BD_NOUSE :
                g_mcmcan.canNodeConfig.busLoopbackEnabled = TRUE;
                break;
            case BD_500K :
                g_mcmcan.canNodeConfig.baudRate.baudrate = 500000;
                break;
            case BD_1M :
                g_mcmcan.canNodeConfig.baudRate.baudrate = 1000000;
                break;
        }

        g_mcmcan.canNodeConfig.busLoopbackEnabled = FALSE;

        if (CAN_Node == CAN_NODE0)
        { /* CAN Node 0 for lite kit */
            g_mcmcan.canNodeConfig.nodeId = IfxCan_NodeId_0;
            const IfxCan_Can_Pins pins = {&IfxCan_TXD00_P20_8_OUT, IfxPort_OutputMode_pushPull, /* TX Pin for lite kit (can node 0) */
            &IfxCan_RXD00B_P20_7_IN, IfxPort_InputMode_pullUp, /* RX Pin for lite kit (can node 0) */
            IfxPort_PadDriver_cmosAutomotiveSpeed1};
            g_mcmcan.canNodeConfig.pins = &pins;
        }
        else if (CAN_Node == CAN_NODE2)
        { /* CAN Node 2 for mikrobus */
            g_mcmcan.canNodeConfig.nodeId = IfxCan_NodeId_2;
            const IfxCan_Can_Pins pins = {&IfxCan_TXD02_P15_0_OUT, IfxPort_OutputMode_pushPull, /* TX Pin for mikrobus (can node 2) */
            &IfxCan_RXD02A_P15_1_IN, IfxPort_InputMode_pullUp, /* RX Pin for mikrobus (can node 2) */
            IfxPort_PadDriver_cmosAutomotiveSpeed1};
            g_mcmcan.canNodeConfig.pins = &pins;
        }

        g_mcmcan.canNodeConfig.frame.type = IfxCan_FrameType_transmitAndReceive;
        g_mcmcan.canNodeConfig.interruptConfig.transmissionCompletedEnabled = TRUE;
        g_mcmcan.canNodeConfig.interruptConfig.traco.priority = ISR_PRIORITY_CAN_TX;
        g_mcmcan.canNodeConfig.interruptConfig.traco.interruptLine = IfxCan_InterruptLine_0;
        g_mcmcan.canNodeConfig.interruptConfig.traco.typeOfService = IfxSrc_Tos_cpu0;
        IfxCan_Can_initNode(&g_mcmcan.canSrcNode, &g_mcmcan.canNodeConfig);

        /* Reception handling configuration */
        g_mcmcan.canNodeConfig.rxConfig.rxMode = IfxCan_RxMode_sharedFifo0;
        g_mcmcan.canNodeConfig.rxConfig.rxBufferDataFieldSize = IfxCan_DataFieldSize_8;
        g_mcmcan.canNodeConfig.rxConfig.rxFifo0DataFieldSize = IfxCan_DataFieldSize_8;
        g_mcmcan.canNodeConfig.rxConfig.rxFifo0Size = 15;
        /* General filter configuration */
        g_mcmcan.canNodeConfig.filterConfig.messageIdLength = IfxCan_MessageIdLength_standard;
        g_mcmcan.canNodeConfig.filterConfig.standardListSize = 8;
        g_mcmcan.canNodeConfig.filterConfig.standardFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_reject;
        g_mcmcan.canNodeConfig.filterConfig.rejectRemoteFramesWithStandardId = TRUE;
        /* Interrupt configuration */
        g_mcmcan.canNodeConfig.interruptConfig.rxFifo0NewMessageEnabled = TRUE;
        g_mcmcan.canNodeConfig.interruptConfig.rxf0n.priority = ISR_PRIORITY_CAN_RX;
        g_mcmcan.canNodeConfig.interruptConfig.rxf0n.interruptLine = IfxCan_InterruptLine_1;
        g_mcmcan.canNodeConfig.interruptConfig.rxf0n.typeOfService = IfxSrc_Tos_cpu0;
        IfxCan_Can_initNode(&g_mcmcan.canDstNode, &g_mcmcan.canNodeConfig);

        /* Rx filter configuration (default: all messages accepted) */
        Can_SetFilterRange(0x0, 0x7FF);

        g_isInitialized = TRUE;
    }
}

void Can_SetFilterRange (uint32 start, uint32 end)
{
    g_mcmcan.canFilter.number = 0;
    g_mcmcan.canFilter.type = IfxCan_FilterType_range;
    g_mcmcan.canFilter.elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxFifo0;
    g_mcmcan.canFilter.id1 = start;
    g_mcmcan.canFilter.id2 = end;
    IfxCan_Can_setStandardFilter(&g_mcmcan.canDstNode, &g_mcmcan.canFilter);
}

void Can_SetFilterMask (uint32 id, uint32 mask)
{
    g_mcmcan.canFilter.number = 0;
    g_mcmcan.canFilter.type = IfxCan_FilterType_classic;
    g_mcmcan.canFilter.elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxFifo0;
    g_mcmcan.canFilter.id1 = id;
    g_mcmcan.canFilter.id2 = mask;
    IfxCan_Can_setStandardFilter(&g_mcmcan.canDstNode, &g_mcmcan.canFilter);
}

void Can_Write (uint32 id, uint8 *txData, uint8 len)
{
    /* Initialization of the TX message with the default configuration */
    IfxCan_Can_initMessage(&g_mcmcan.txMsg);

    g_mcmcan.txMsg.messageId = id;
    g_mcmcan.txMsg.dataLengthCode = len;

    /* Define the content of the data to be transmitted */
    for (int i = 0; i < 8; i++)
    {
        g_mcmcan.txData[i] = txData[i];
    }

    /* Send the CAN message with the previously defined TX message content */
    while (IfxCan_Status_notSentBusy
            == IfxCan_Can_sendMessage(&g_mcmcan.canSrcNode, &g_mcmcan.txMsg, (uint32*) &g_mcmcan.txData[0]))
    {
    }
}

void Can_Receive (Can_FrameType *frame)
{
    /* Clear the "RX FIFO 0 new message" interrupt flag */
    IfxCan_Node_clearInterruptFlag(g_mcmcan.canDstNode.node, IfxCan_Interrupt_rxFifo0NewMessage);

    /* Received message content should be updated with the data stored in the RX FIFO 0 */
    g_mcmcan.rxMsg.readFromRxFifo0 = TRUE;
    g_mcmcan.rxMsg.readFromRxFifo1 = FALSE;

    /* Read the received CAN message */
    IfxCan_Can_readMessage(&g_mcmcan.canDstNode, &g_mcmcan.rxMsg, (uint32*) &g_mcmcan.rxData);

    frame->id = g_mcmcan.rxMsg.messageId;
    for (int i = 0; i < 8; i++)
    {
        frame->data[i] = g_mcmcan.rxData[i];
    }
    frame->dlc = g_mcmcan.rxMsg.dataLengthCode;
}

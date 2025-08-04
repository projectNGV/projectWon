#include "tof.h"

static unsigned int g_TofValue;
volatile bool aebFlag = false;
volatile bool tofFlag = false;

extern MotorState motorState;
extern McmcanType g_mcmcan;

void tofInit (void)
{
    canInit(BD_500K, CAN_NODE0);
    canRegisterTofCallback(tofUpdateFromCAN);
    g_TofValue = 5000;
    aebFlag = false;
    tofFlag = false;
}

void tofOnOff(void)
{
    if (tofFlag)
    {
        tofFlag = false;
        //SRC_CAN0_RX->B.SRE = 0;  // 인터럽트 Disable
        //g_mcmcan.canNodeConfig.interruptConfig.rxFifo0NewMessageEnabled = FALSE;
        IfxCan_Node_enableInterrupt(&g_mcmcan.canDstNode.node, IfxCan_Interrupt_rxFifo0NewMessage);
    }
    else
    {
        tofFlag = true;
        //SRC_CAN0_RX->B.SRE = 1;  // 인터럽트 Enable
        //g_mcmcan.canNodeConfig.interruptConfig.rxFifo0NewMessageEnabled = TRUE;
        IfxCan_Node_disableInterrupt(&g_mcmcan.canDstNode.node, IfxCan_Interrupt_rxFifo0NewMessage);
    }
}

void tofUpdateFromCAN (unsigned char *rxData)
{
    unsigned short signal_strength = rxData[5] << 8 | rxData[4];

    if (signal_strength != 0)
    {
        if(tofFlag == false){
            aebFlag = false;
            g_TofValue = 5000;
        }
        else{
            g_TofValue = rxData[2] << 16 | rxData[1] << 8 | rxData[0];
            if(g_TofValue <= 800 && aebFlag == false)
            {
                motorState.baseDuty = 500;
            }
            if (g_TofValue < aebDistanceMM && aebFlag == false)
            {
                aebFlag = true;
            }
            else if (g_TofValue >= safetyDistanceMM && aebFlag == true)
            {
                aebFlag = false;
            }
        }
    }
}

unsigned int tofGetValue (void)
{
    return g_TofValue;
}

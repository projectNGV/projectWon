#ifndef BSW_MCAL_CAN_H_
#define BSW_MCAL_CAN_H_


#include <tof.h>
#include "IfxPort.h"
#include "IfxCan.h"
#include "IfxCan_Can.h"



/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define CAN_MESSAGE_ID              (uint32)0x777           /* Message ID that will be used in arbitration phase    */
#define MAXIMUM_CAN_DATA_PAYLOAD    2                       /* Define maximum classical CAN payload in 4-byte words */

/*********************************************************************************************************************/
/*--------------------------------------------------Data Structures--------------------------------------------------*/
/*********************************************************************************************************************/


typedef struct
{
    uint32 id;
    uint8 data[8];
    uint8 dlc; // Data Length Code
} Can_FrameType;

typedef void (*Can_RxCallbackType) (const Can_FrameType *frame);

typedef enum {
    BD_NOUSE = 0,
    BD_500K = 1,
    BD_1M = 2
} CAN_BAUDRATES;

typedef enum {
    CAN_NODE0 = 0, /* CAN Node 0 for lite kit */
    CAN_NODE2 = 2  /* CAN Node 2 for mikrobus */
} CAN_NODE;

/*********************************************************************************************************************/
/*-----------------------------------------------Function Prototypes-------------------------------------------------*/
/*********************************************************************************************************************/
void Can_Init(CAN_BAUDRATES ls_baudrate, CAN_NODE CAN_Node);
void Can_Receive (Can_FrameType *frame);
boolean Can_RegisterRxCallback (uint32 canId, Can_RxCallbackType callback);
void Can_SetFilterRange (uint32 start, uint32 end);

#endif

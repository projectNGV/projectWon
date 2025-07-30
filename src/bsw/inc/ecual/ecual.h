#ifndef BSW_INC_ECUAL_H_
#define BSW_INC_ECUAL_H_

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/

#include "IfxPort.h"

#include "IfxGtm_PinMap.h"

#include "IfxAsclin.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*-------------------------------------------------Data Structures---------------------------------------------------*/
/*********************************************************************************************************************/
typedef enum{
    PWM_CHANNEL_ATOM0_CH1,
    PWM_CHANNEL_ATOM1_CH3,
    PWM_NUM_OF_CHANNELS
} Pwm_ChannelType;

typedef enum
{
    MOTOR_LEFT_WHEEL, MOTOR_RIGHT_WHEEL, MOTOR_NUM_OF_MOTORS
} Motor_WheelType;


typedef struct
{
    IfxPort_Pin breakPin;
    IfxPort_Pin directionPin;
    Pwm_ChannelType pwmChannel;
} Motor_PinType;

typedef enum{
    UART_BAUD_RATE_9600 = 48, UART_BAUD_RATE_38400 = 192, UART_BAUD_RATE_115200 = 576
} Uart_BaudRateType;

typedef struct {
    Ifx_ASCLIN* asclin;
    Uart_BaudRateType baudRate;
    IfxPort_Pin rxPin;
    IfxPort_Pin txPin;
} Uart_ConfigType;

typedef enum {
    UART_CHANNEL_USB,
    UART_CHANNEL_BLUETOOTH,
    UART_NUM_OF_CHANNELS
} Uart_ChannelType;


typedef struct
{
    IfxPort_Pin trigger;
    IfxPort_Pin echo;
} Ult_PinType;

typedef enum ultradir
{
    ULT_LEFT, ULT_RIGHT, ULT_REAR, ULT_SENSORS_NUM
} Ult_DirType;


/*********************************************************************************************************************/
/*--------------------------------------------Private Variables/Constants--------------------------------------------*/
/*********************************************************************************************************************/

extern const IfxGtm_Atom_ToutMap* const PWM_OUT_MAP[PWM_NUM_OF_CHANNELS];
extern const Motor_PinType MOTOR_CONFIG[MOTOR_NUM_OF_MOTORS];
extern const Uart_ConfigType UART_CONFIG[UART_NUM_OF_CHANNELS];
extern const Ult_PinType ULT_PINS[ULT_SENSORS_NUM];

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/


#endif /* BSW_INC_ECUAL_H_ */

#include "motor.h"

#include "pwm.h"
#include "IfxPort.h"
#include "ecual.h"


extern const Motor_PinType MOTOR_CONFIG[MOTOR_NUM_OF_MOTORS];

void Motor_Init (void)
{
    for(int i = 0; i < MOTOR_NUM_OF_MOTORS; i++){
        IfxPort_setPinMode(MOTOR_CONFIG[i].breakPin.port, MOTOR_CONFIG[i].breakPin.pinIndex, IfxPort_Mode_outputPushPullGeneral);
        IfxPort_setPinMode(MOTOR_CONFIG[i].directionPin.port, MOTOR_CONFIG[i].directionPin.pinIndex, IfxPort_Mode_outputPushPullGeneral);
    }
    Pwm_Init();
    for(int i = 0; i < PWM_NUM_OF_CHANNELS; i++){
        Pwm_SetDutyCycle(i, 0);
    }
}

static void setBrake (Motor_WheelType wheel, boolean enable)
{
    IfxPort_setPinState(MOTOR_CONFIG[wheel].breakPin.port, MOTOR_CONFIG[wheel].breakPin.pinIndex, ((enable) ? IfxPort_State_high : IfxPort_State_low));
}

static void setDirection (Motor_WheelType wheel, Motor_DirectionType direction)
{
    IfxPort_setPinState(MOTOR_CONFIG[wheel].directionPin.port, MOTOR_CONFIG[wheel].directionPin.pinIndex, ((direction == MOTOR_DIR_FORWARD) ? IfxPort_State_high : IfxPort_State_low));
}

void Motor_Control(Motor_WheelType wheel, Motor_DirectionType direction, uint16 speed){
    setBrake(wheel, (speed > 0) ? FALSE : TRUE);
    setDirection(wheel, direction);
    Pwm_SetDutyCycle(MOTOR_CONFIG[wheel].pwmChannel, speed);
}

void Motor_GoForward (uint16 speed)
{
    for(int i = 0; i < MOTOR_NUM_OF_MOTORS; i++){
        Motor_Control(i, MOTOR_DIR_FORWARD, speed);
    }
}

void Motor_GoBackward (uint16 speed)
{
    for(int i = 0; i < MOTOR_NUM_OF_MOTORS; i++){
        Motor_Control(i, MOTOR_DIR_BACKWARD, speed);
    }
}

void Motor_Stop (void)
{
    for(int i = 0; i < MOTOR_NUM_OF_MOTORS; i++){
        setBrake(i, TRUE);
        Pwm_SetDutyCycle(MOTOR_CONFIG[i].pwmChannel, 0);
    }
}

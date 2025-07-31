#ifndef MOTOR_H_
#define MOTOR_H_

#include <ecual.h>
#include "Ifx_Types.h"

typedef enum
{
    MOTOR_DIR_FORWARD, MOTOR_DIR_BACKWARD
} Motor_DirectionType;

void Motor_Init (void);

// Speed: 0 ~ 10000
void Motor_Control (Motor_WheelType wheel, Motor_DirectionType direction, uint16 speed);

void Motor_SetSpeed (Motor_WheelType wheel, uint16 speed);

void Motor_GoForward (uint16 speed);

void Motor_GoBackward (uint16 speed);

void Motor_Stop (void);

#endif /* MOTOR_H_ */

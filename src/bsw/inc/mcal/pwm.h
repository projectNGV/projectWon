#ifndef PWM_H_
#define PWM_H_

#include "Ifx_Types.h"
#include "ecual.h"

void Pwm_Init(void);
void Pwm_SetDutyCycle(Pwm_ChannelType channel, uint16 duty);

#endif /* PWM_H_ */

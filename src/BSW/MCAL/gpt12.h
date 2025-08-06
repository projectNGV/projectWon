/*
 * gpt12.h
 *
 *  Created on: 2025. 6. 25.
 *      Author: USER
 */

#ifndef BSW_DRIVER_GPT12_H_
#define BSW_DRIVER_GPT12_H_

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

#include "IfxGpt12.h"
#include "priority.h"
#include "led.h"
#include "buzzerport.h"

void gpt1_init ();
void gpt2_init (void);
void gpt12Init(void);

void Gpt1_Interrupt_Enable (void);
void Gpt1_Interrupt_Disable (void);
void Gpt2_Interrupt_Enable(void);
void Gpt2_Interrupt_Disable(void);

#endif /* BSW_DRIVER_GPT12_H_ */

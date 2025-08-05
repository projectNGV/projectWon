/*
 * gpt12.h
 *
 *  Created on: 2025. 6. 25.
 *      Author: USER
 */

#ifndef BSW_DRIVER_GPT12_H_
#define BSW_DRIVER_GPT12_H_

#include "IfxGpt12.h"
#include "priority.h"
#include "led.h"

unsigned int getCnt();
void gpt12Init();
void gpt1_init (void);
void gpt2_init (void);
void runGpt12T6();
void setBeepCycle(int cycle);


#endif /* BSW_DRIVER_GPT12_H_ */

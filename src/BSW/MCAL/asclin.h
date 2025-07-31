/*
 * asclin.h
 *
 *  Created on: 2025. 6. 23.
 *      Author: USER
 */

#ifndef BSW_MCAL_ASCLIN_H_
#define BSW_MCAL_ASCLIN_H_


#include "IfxAsclin.h"
#include "IfxAsclin_bf.h"
#include "priority.h"


void asclin0InitUart(void);
void asclin0OutUart(const unsigned char chr);
int asclin0PollUart(unsigned char *chr);
unsigned char asclin0InUart(void);
char asclin0InUartNonBlock(void);
void asclin0RxIsrHandler(void);

void asclin1InitUart(void);
void asclin1OutUart(const unsigned char chr);
unsigned char asclin1InUart(void);
int asclin1PollUart(unsigned char *chr);


#endif /* BSW_DRIVER_ASCLIN_H_ */

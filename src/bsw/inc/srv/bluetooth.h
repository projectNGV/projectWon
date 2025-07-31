/*
 * Bluetooth.h
 *
 *  Created on: 2025. 6. 29.
 *      Author: USER
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <ecual.h>
#include "Ifx_Types.h"

void Blt_Init (void);
void Blt_SendByte (uint8 data);
boolean Blt_ReceiveByte (uint8 *data);
void Blt_Printf (const char *fmt, ...);
int Blt_Scanf (Uart_ChannelType channel, const char *fmt, ...);

#endif /* BLUETOOTH_H_ */

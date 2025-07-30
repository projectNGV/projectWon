/*
 * Bluetooth.h
 *
 *  Created on: 2025. 6. 29.
 *      Author: USER
 */

#ifndef BSW_IO_BLUETOOTH_H_
#define BSW_IO_BLUETOOTH_H_

#include <stdarg.h>

#include "uart.h"
#include "asclin.h"
#include "util.h"
#include "priority.h"

void bluetoothInit(void);
void bluetoothSetName(char *name);
void bluetoothSetPwd(char *pwd);
void bluetoothSetBaud(int baudrate);
void bluetoothAtCommand(char *cmd);
char bluetoothRecvByteBlocked(void);
char bluetoothRecvByteNonBlocked(void);
void bluetoothSendByteBlocked(unsigned char ch);
void bluetoothPrintf(const char *fmt, ...);


#endif /* BSW_IO_BLUETOOTH_H_ */

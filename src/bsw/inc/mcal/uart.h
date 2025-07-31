#ifndef UART_H_
#define UART_H_

#include <ecual.h>
#include <stdarg.h>

#include "Ifx_Types.h"

void Uart_Init (Uart_ChannelType asclin);
void Uart_TransmitByte (Uart_ChannelType asclin, const uint8 data);
void Uart_TransmitString(Uart_ChannelType channel, const char* str);
boolean Uart_ReceiveByte (Uart_ChannelType asclin, uint8 *data);
void Uart_Printf(Uart_ChannelType channel, const char *fmt, va_list ap);
int Uart_Scanf(Uart_ChannelType channel, const char *fmt, va_list ap);

#endif /* UART_H_ */

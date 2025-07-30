#include "bluetooth.h"

#include <stdarg.h>

#include "uart.h"
#include "ecual.h"


void Blt_Init (void)
{
    Uart_Init(UART_CHANNEL_BLUETOOTH);
}

void Blt_SendByte (uint8 data)
{
    Uart_Write(UART_CHANNEL_BLUETOOTH, data);
}

boolean Blt_ReceiveByte (uint8 *data)
{
    return Uart_Read(UART_CHANNEL_BLUETOOTH, data);
}

void Blt_Printf (const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    Uart_Printf(UART_CHANNEL_BLUETOOTH, fmt, ap);
    va_end(ap);
}

int Blt_Scanf (Uart_ChannelType channel, const char *fmt, ...)
{
    va_list ap;
    int result;

    va_start(ap, fmt);
    result = Uart_Vscanf(channel, fmt, ap);
    va_end(ap);

    return result;
}


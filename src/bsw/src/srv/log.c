#include <ecual.h>
#include <log.h>
#include <stdarg.h>
#include <stdio.h>

#include <uart.h>

void Log_Init (void)
{
#if LOG_SYSTEM_ENABLED == 1
    Uart_Init(UART_CHANNEL_USB);
#endif
}

void Log_Print (Log_LevelType level, const char *file, int line, const char *fmt, ...)
{
    if (level >= LOG_COMPILE_LEVEL)
    {
        va_list args;
        va_start(args, fmt);

        char buffer[256];
        int len = 0;

        switch (level)
        {
            case LOG_LEVEL_DEBUG :
                len += sprintf(buffer + len, "[DEBUG] ");
                break;
            case LOG_LEVEL_INFO :
                len += sprintf(buffer + len, "[INFO] ");
                break;
            case LOG_LEVEL_WARN :
                len += sprintf(buffer + len, "[WARN] %s:%d: ", file, line);
                break;
            case LOG_LEVEL_ERROR :
                len += sprintf(buffer + len, "[ERROR] %s:%d: ", file, line);
                break;
        }

        len += vsprintf(buffer + len, fmt, args);

        len += sprintf(buffer + len, "\r\n");

        uint8 destination;
        switch (level)
        {
            case LOG_LEVEL_DEBUG :
                destination = LOG_DESTINATION_DEBUG;
                break;
            case LOG_LEVEL_INFO :
                destination = LOG_DESTINATION_INFO;
                break;
            case LOG_LEVEL_WARN :
                destination = LOG_DESTINATION_WARN;
                break;
            case LOG_LEVEL_ERROR :
                destination = LOG_DESTINATION_ERROR;
                break;
            default :
                destination = LOG_OUTPUT_NONE;
                break;
        }

        if (destination & LOG_OUTPUT_UART_USB)
        {
            Uart_TransmitString(UART_CHANNEL_USB, buffer);
        }
        if (destination & LOG_OUTPUT_UART_BLT)
        {
            Uart_TransmitString(UART_CHANNEL_BLUETOOTH, buffer);
        }

        va_end(args);

    }
}

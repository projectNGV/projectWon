#ifndef LOG_H_
#define LOG_H_

#define LOG_SYSTEM_ENABLED      TRUE

#define LOG_COMPILE_LEVEL       LOG_LEVEL_DEBUG

#define LOG_DESTINATION_DEBUG   (LOG_OUTPUT_UART_USB)
#define LOG_DESTINATION_INFO    (LOG_OUTPUT_UART_USB | LOG_OUTPUT_UART_BLT)
#define LOG_DESTINATION_WARN    (LOG_OUTPUT_UART_USB | LOG_OUTPUT_UART_BLT)
#define LOG_DESTINATION_ERROR   (LOG_OUTPUT_UART_USB | LOG_OUTPUT_UART_BLT)


#define Log_Debug(fmt, ...)   Log_Print(LOG_LEVEL_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define Log_Info(fmt, ...)    Log_Print(LOG_LEVEL_INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define Log_Warn(fmt, ...)    Log_Print(LOG_LEVEL_WARN,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define Log_Error(fmt, ...)   Log_Print(LOG_LEVEL_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)


typedef enum
{
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
} Log_LevelType;


typedef enum
{
    LOG_OUTPUT_NONE         = 0,
    LOG_OUTPUT_UART_USB     = 1 << 0,
    LOG_OUTPUT_UART_BLT     = 1 << 1,
} Log_OutputType;

void Log_Init(void);
void Log_Print(Log_LevelType level, const char* file, int line, const char* fmt, ...);

#endif /* LOG_H_ */

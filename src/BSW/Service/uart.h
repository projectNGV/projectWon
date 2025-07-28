#ifndef BSW_ETC_MY_STDIO_H_
#define BSW_ETC_MY_STDIO_H_

#include <string.h>
#include <stdarg.h>

#include "bluetooth.h"

#include "Ifx_Types.h"
#define BUFSIZE     128
#define KB_BS '\x7F'
#define KB_CR '\r'

void myPuts(const char *str);
void myPrintf(const char *fmt, ...);
void myScanf(const char *fmt, ...);

#endif /* BSW_ETC_MY_STDIO_H_ */

#ifndef BSW_ETC_MY_STDIO_H_
#define BSW_ETC_MY_STDIO_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bluetooth.h"
<<<<<<< HEAD
#include "uart.h"
=======
#include "asclin0.h"
>>>>>>> origin/jh

#include "Ifx_Types.h"
#define BUFSIZE     128
#define KB_BS '\x7F'
#define KB_CR '\r'

void uartInit(void);
<<<<<<< HEAD

=======
>>>>>>> origin/jh
void myPuts(const char *str);
void myPrintf(const char *fmt, ...);
void myScanf(const char *fmt, ...);
static void remove_null(char *s);

static void remove_null(char *s);

#endif /* BSW_ETC_MY_STDIO_H_ */

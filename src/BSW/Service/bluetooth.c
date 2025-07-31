#include "bluetooth.h"

void bluetoothIsr(char c)
{
//    myPrintf("%c", c);
}



void bluetoothInit(void)
{
    asclin1InitUart();
}

void bluetoothAtCommand(char *cmd)
{
    char buf[30];
    int i = 0;
    sprintf(buf, "%s", cmd);
    while (buf[i] != 0)
    {
        asclin1OutUart(buf[i]);
        i++;
    }
    asclin1OutUart(buf[i]);
//    asclin1OutUart('\r');
//    asclin1OutUart('\n');

    delayMs(300);
}

char bluetoothRecvByteBlocked(void)
{
    return asclin1InUart();
}

char bluetoothRecvByteNonBlocked(void)
{
    unsigned char ch = 0;
    int res;
    res = asclin1PollUart(&ch);

    return res == 1 ? ch : -1;
}

void bluetoothSendByteBlocked(unsigned char ch)
{
    asclin1OutUart(ch);
}

void bluetoothPrintf(const char *fmt, ...)
{
    char buffer[128];
    char buffer2[128]; // add \r before \n
    char *ptr;
    va_list ap;

    va_start(ap, fmt);
    vsprintf(buffer, fmt, ap);
    va_end(ap);
    int j = 0;
    for (int i = 0; buffer[i]; i++)
    {
        if (buffer[i] == '\n')
        {
            buffer2[j++] = '\r';
            buffer2[j++] = buffer[i];
        }
        else
        {
            buffer2[j++] = buffer[i];
        }
    }
    buffer2[j] = '\0';

    for (ptr = buffer2; *ptr; ++ptr)
        bluetoothSendByteBlocked((const unsigned char) *ptr);
}

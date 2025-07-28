#include "bluetooth.h"

void bluetoothInit(void)
{
    asclin1InitUart();

}


void bluetoothSetName(char *name)
{
    char buf[30] = {'\0', };
    int i = 0;
    sprintf(buf, "AT+NAME%s\r\n", name);

    /* Send AT Command */
    while (buf[i] != 0)
    {
        asclin1OutUart(buf[i]);
        i++;
    }
    asclin1OutUart('\0');

    delayMs(300);
}

void bluetoothSetPwd(char *pwd)
{
    char buf[30] = {'\0', };
    int i = 0;
    sprintf(buf, "AT+PIN%s\r\n", pwd);

//    my_printf("%s", buf);
    while (buf[i] != 0)
    {
        asclin1OutUart(buf[i]);
        i++;
    }
    asclin1OutUart('\0');



    char c = 0;
    while(1){
        if(asclin1PollUart(&c)){
            myPrintf("%c", c);
        }
    }


    delayMs(300);
}

void bluetoothSetBaud(int baudrate)
{
    char buf[30];
    int i = 0;
    switch (baudrate)
    {
        case 9600 :
            sprintf(buf, "AT+BAUD4");
            break;
        case 115200 :
            sprintf(buf, "AT+BAUD8");
            break;
    }
    while (buf[i] != 0)
    {
        asclin1OutUart(buf[i]);
        i++;
    }
    asclin1OutUart(buf[i]);
    delayMs(300);
}

/* AT command reference: https://www.martyncurrey.com/hc-06-zg-b23090w-bluetooth-2-0-edr-modules/ */
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

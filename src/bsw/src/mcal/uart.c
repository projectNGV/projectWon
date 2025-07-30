#include "uart.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "IfxAsclin_bf.h"

#define BUFSIZE     128
#define KB_BS '\x7F'
#define KB_CR '\r'


extern const Uart_ConfigType UART_CONFIG[UART_NUM_OF_CHANNELS];

void Uart_Init (Uart_ChannelType channel)
{
    Uart_ConfigType *config = &UART_CONFIG[channel];
    uint32 numerator = config->baudRate;
    uint32 denominator = 3125;

    // RX PIN IOCR
    IfxPort_setPinMode(config->rxPin.port, config->rxPin.pinIndex, IfxPort_Mode_inputPullUp);
    // TX PIN IOCR
    IfxPort_setPinMode(config->txPin.port, config->txPin.pinIndex, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinState(config->txPin.port, config->txPin.pinIndex, IfxPort_State_high);

    /* Enable ASCn */
    IfxScuWdt_clearCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());
    config->asclin->CLC.U = 0;
    IfxScuWdt_setCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());
    /* read back for activating module */
    (void) config->asclin->CLC.U;

    /* select RX as input pin */
    config->asclin->IOCR.B.ALTI = 0; // Select Alternate Input A

    /* Program ASC0 */
    config->asclin->CSR.U = 0;

    /* configure TX and RX FIFOs */
    config->asclin->TXFIFOCON.U = (1 << 6) /* INW: (1 == 1 byte) */
    | (1 << 1) /* ENO */
    | (1 << 0); /* FLUSH */
    config->asclin->RXFIFOCON.U = (1 << 31) /* BUF: (1 == Single Stage RX Buffer) */
    | (1 << 6) /* OUTW: (1 == 1 byte) */
    | (1 << 1) /* ENI */
    | (1 << 0); /* FLUSH */

    /* 115200 */
    config->asclin->BITCON.U = (9 << 0) /* PRESCALER: 10 */
    | (15 << 16) /* OVERSAMPLING: 16 */
    | (9 << 24) /* SAMPLEPOINT: position 7,8,9 */
    | (1u << 31); /* SM: 3 samples per bit */
    /* data format: 8N1 */
    config->asclin->FRAMECON.U = (1 << 9) /* STOP: 1 bit */
    | (0 << 16) /* MODE: Init */
    | (0 << 30); /* PEN: no parity */
    config->asclin->DATCON.U = (7 << 0); /* DATLEN: 8 bit */

    /* set baudrate value */
    config->asclin->BRG.U = (denominator << 0) /* DENOMINATOR */
    | (numerator << 16); /* NUMERATOR */

    config->asclin->FRAMECON.B.MODE = 1; /* ASC mode */
    config->asclin->CSR.U = 1; /* select CLC as clock source */

    config->asclin->FLAGSSET.U = (IFX_ASCLIN_FLAGSSET_TFLS_MSK << IFX_ASCLIN_FLAGSSET_TFLS_OFF);
}

void Uart_TransmitByte (Uart_ChannelType channel, const uint8 data)
{
    Uart_ConfigType *config = &UART_CONFIG[channel];
    while (!(config->asclin->FLAGS.B.TFL != 0))
        ;
    /* TX Clear */
    config->asclin->FLAGSCLEAR.U = (IFX_ASCLIN_FLAGSCLEAR_TFLC_MSK << IFX_ASCLIN_FLAGSCLEAR_TFLC_OFF);
    /* send the character */
    config->asclin->TXDATA.U = data;
}

void Uart_TransmitString(Uart_ChannelType channel, const char* str){
    while(*str){
        Uart_TransmitByte(channel, (*str)++);
    }
}

boolean Uart_ReceiveByte (Uart_ChannelType channel, uint8 *data)
{
    Uart_ConfigType *config = &UART_CONFIG[channel];

    uint8 ret;
    boolean res = FALSE;

    if (config->asclin->FLAGS.B.RFL != 0) /* If RX Ready */
    {
        ret = (uint8) config->asclin->RXDATA.U;
        /* acknowledge receive */
        config->asclin->FLAGSCLEAR.U = (IFX_ASCLIN_FLAGSCLEAR_RFLC_MSK << IFX_ASCLIN_FLAGSCLEAR_RFLC_OFF); // RX Clear
        /* check for error condition */
        if ((config->asclin->FLAGS.U)
                & ((IFX_ASCLIN_FLAGS_PE_MSK << IFX_ASCLIN_FLAGS_PE_OFF)
                        | (IFX_ASCLIN_FLAGS_FE_MSK << IFX_ASCLIN_FLAGS_FE_OFF)
                        | (IFX_ASCLIN_FLAGS_RFO_MSK << IFX_ASCLIN_FLAGS_RFO_OFF)))
        {
            /* reset error flags */
            config->asclin->FLAGSCLEAR.U = ((IFX_ASCLIN_FLAGSCLEAR_PEC_MSK << IFX_ASCLIN_FLAGSCLEAR_PEC_OFF)
                    | (IFX_ASCLIN_FLAGSCLEAR_FEC_MSK << IFX_ASCLIN_FLAGSCLEAR_FEC_OFF)
                    | (IFX_ASCLIN_FLAGSCLEAR_RFOC_MSK << IFX_ASCLIN_FLAGSCLEAR_RFOC_OFF));
            /* Buffer clear, reset module */
        }
        else
        {
            /* this is a valid character */
            *data = ret;
            res = TRUE;
        }
    }

    return res;
}

void Uart_Printf(Uart_ChannelType channel, const char *fmt, va_list ap)
{
    char buffer[128];
    char buffer2[128];
    char *ptr;

    vsprintf(buffer, fmt, ap);

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
    {
        Uart_TransmitByte(channel, *ptr);
    }
}

static int readLine(Uart_ChannelType channel, char* buffer, int bufferSize)
{
    char c = 0;
    int index = 0;

    // 버퍼 클리어
    memset(buffer, 0, bufferSize);

    while (index < (bufferSize - 1))
    {
        while (Uart_ReceiveByte(channel, &c) == FALSE){}

        if (c == '\r')
        {
            Uart_Printf(channel, "\r\n");
            break;
        }
        else if (c == '\b' || c == 127)
        {
            if (index > 0)
            {
                index--;
                buffer[index] = '\0';
                Uart_Printf(channel, "\b \b");
            }
        }
        else
        {
            buffer[index] = c;
            index++;
            Uart_TransmitByte(channel, c);
        }
    }
    buffer[index] = '\0';

    return index;
}

int Uart_Scanf(Uart_ChannelType channel, const char *fmt, va_list ap)
{
    char line_buffer[128];
    int match_count = -1;

    if (readLine(channel, line_buffer, sizeof(line_buffer)) > 0)
    {
        match_count = vsscanf(line_buffer, fmt, ap);
    }

    return match_count;
}

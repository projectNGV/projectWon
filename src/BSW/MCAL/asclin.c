#include "asclin.h"

void asclin0InitUart(void)
{
    // Table 285
    // P14.0 OUT ALT(2)
    // 0b10010 => ALT 2 OUT
    MODULE_P14.IOCR0.B.PC0 = (1 << 4 | 1 << 1);
    // P14.1 IN ARXA
    // 0b0XX10 => PULL UP IN (Default)
    MODULE_P14.IOCR0.B.PC1 = 1 << 2;

    // ASCLIN0 CONFIG
    IfxScuWdt_clearCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());
    MODULE_ASCLIN0.CLC.B.DISR = 0;
    MODULE_ASCLIN0.CLC.B.EDIS = 1;
    IfxScuWdt_setCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());

    while (MODULE_ASCLIN0.CLC.B.DISS == 1);

    // ARX (A => 0)
    MODULE_ASCLIN0.IOCR.B.ALTI = 0;

    // TxFIFO CON
    MODULE_ASCLIN0.TXFIFOCON.B.FLUSH = 1;
    MODULE_ASCLIN0.TXFIFOCON.B.ENO = 1;
    MODULE_ASCLIN0.TXFIFOCON.B.INW = 1;

    // RxFIFO CON
    MODULE_ASCLIN0.RXFIFOCON.B.FLUSH = 1;
    MODULE_ASCLIN0.RXFIFOCON.B.ENI = 1;
    MODULE_ASCLIN0.RXFIFOCON.B.OUTW = 1;

    // BIT CON
    MODULE_ASCLIN0.BITCON.B.PRESCALER = 9;
    MODULE_ASCLIN0.BITCON.B.OVERSAMPLING = 15;

    // FRAME CONFIG
    MODULE_ASCLIN0.FRAMECON.B.MODE = 0; // INIT MODE
    MODULE_ASCLIN0.FRAMECON.B.STOP = 1;
    MODULE_ASCLIN0.FRAMECON.B.PEN = 0;
    MODULE_ASCLIN0.FRAMECON.B.MODE = 1; // ASC MODE

    // DATA CONFIG
    MODULE_ASCLIN0.DATCON.B.DATLEN = 7;

    // BAUD RATE CONFIG
    MODULE_ASCLIN0.BRG.B.DENOMINATOR = 3125;
    MODULE_ASCLIN0.BRG.B.NUMERATOR = 576;

    // TRANSMIT POSSIBLE
    MODULE_ASCLIN0.CSR.U = 1;
    MODULE_ASCLIN0.FLAGSSET.B.TFLS = 1;

//    // Interrupt
//    Ifx_SRC_SRCR_Bits* src = (Ifx_SRC_SRCR_Bits*) &MODULE_SRC.ASCLIN.ASCLIN[0].RX.B;
//    src->SRPN = ISR_PRIORITY_ASCLIN0_RX;
//    src->SRE = 1;
//    src->TOS = 0;
}

void asclin0OutUart(const unsigned char chr)
{
    while (MODULE_ASCLIN0.FLAGS.B.TFL == 0);
    MODULE_ASCLIN0.FLAGSCLEAR.B.TFLC = 1;
    MODULE_ASCLIN0.TXDATA.U = chr;
}

int asclin0PollUart(unsigned char *chr)
{
    if (MODULE_ASCLIN0.FLAGS.B.RFL == 0)
    {
        return 0;
    }
    *chr = (char) MODULE_ASCLIN0.RXDATA.U;
    MODULE_ASCLIN0.FLAGSCLEAR.B.RFLC = 1;
    if (MODULE_ASCLIN0.FLAGS.B.FE || MODULE_ASCLIN0.FLAGS.B.PE || MODULE_ASCLIN0.FLAGS.B.RFO)
    {
        MODULE_ASCLIN0.FLAGSCLEAR.B.FEC = 1;
        MODULE_ASCLIN0.FLAGSCLEAR.B.PEC = 1;
        MODULE_ASCLIN0.FLAGSCLEAR.B.RFOC = 1;
        return 0;
    }
    return 1;
}

unsigned char asclin0InUart(void)
{
    unsigned char ch;
    while (asclin0PollUart(&ch) == 0);
    return ch;
}

char asclin0InUartNonBlock(void)
{
    unsigned char ch = 0;
    return asclin0PollUart(&ch) ? ch : -1;
}

IFX_INTERRUPT(Asclin0RxIsrHandler, 0, ISR_PRIORITY_ASCLIN0_RX);
void asclin0RxIsrHandler(void)
{
    char c;
    if(!asclin0PollUart(&c)) return;
    asclin0OutUart(c);
    if (c == '\r')
        asclin0OutUart('\n');
}



void asclin1InitUart(void)
{
    /* set numerator and denominator for 9600 baudrate */
//    unsigned int numerator = 48; // 9600
//    unsigned int numerator = 192; // 38400
    unsigned int numerator = 576; // 115200

    unsigned int denominator = 3125;

    /* RXA/P15.1, TX/P15.0 */
    /* Set TX/P15.0 to "output" and "high" */
    MODULE_P15.IOCR0.B.PC0 = 0x12;
    MODULE_P15.OUT.B.P0 = 1;

    /* Enable ASCn */
    IfxScuWdt_clearCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());
    MODULE_ASCLIN1.CLC.U = 0;
    IfxScuWdt_setCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());
    /* read back for activating module */
    (void)MODULE_ASCLIN1.CLC.U;

    /* select RX as input pin */
    MODULE_ASCLIN1.IOCR.B.ALTI = 0; // Select Alternate Input A

    /* Program ASC1 */
    MODULE_ASCLIN1.CSR.U = 0;

    /* configure TX and RX FIFOs */
    MODULE_ASCLIN1.TXFIFOCON.U = (1 << 6)   /* INW: (1 == 1 byte) */
                               | (1 << 1)    /* ENO */
                               | (1 << 0);   /* FLUSH */
    MODULE_ASCLIN1.RXFIFOCON.U = (1 << 31)  /* BUF: (1 == Single Stage RX Buffer) */
                               | (1 << 6)    /* OUTW: (1 == 1 byte) */
                               | (1 << 1)    /* ENI */
                               | (1 << 0);   /* FLUSH */

    MODULE_ASCLIN1.BITCON.U = ( 9 << 0)     /* PRESCALER: 10 */
                            | (15 << 16)     /* OVERSAMPLING: 16 */
                            | ( 9 << 24)     /* SAMPLEPOINT: position 7,8,9 */
                            | (1u << 31);    /* SM: 3 samples per bit */
    /* data format: 8N1 */
    MODULE_ASCLIN1.FRAMECON.U = (1 << 9)        /* STOP: 1 bit */
                              | (0 << 16)    /* MODE: Init */
                              | (0 << 30);   /* PEN: no parity */
    MODULE_ASCLIN1.DATCON.U = (7 << 0);     /* DATLEN: 8 bit */

    /* set baudrate value */
    MODULE_ASCLIN1.BRG.U = (denominator << 0)   /* DENOMINATOR */
                         | (numerator << 16);    /* NUMERATOR */

    MODULE_ASCLIN1.FRAMECON.B.MODE = 1;     /* ASC mode */
    MODULE_ASCLIN1.CSR.U = 1;               /* select CLC as clock source */

    MODULE_ASCLIN1.FLAGSSET.U = (IFX_ASCLIN_FLAGSSET_TFLS_MSK << IFX_ASCLIN_FLAGSSET_TFLS_OFF);



    // Interrupt
    Ifx_SRC_SRCR_Bits *src = (Ifx_SRC_SRCR_Bits*) &MODULE_SRC.ASCLIN.ASCLIN[1].RX.B;
    src->SRPN = ISR_PRIORITY_ASCLIN1_RX;
    src->TOS = 0;
    src->CLRR = 1;
    MODULE_ASCLIN1.FLAGSENABLE.B.RFLE = 1;
    src->SRE = 1;
}


IFX_INTERRUPT(Asclin1RxIsrHandler, 0, ISR_PRIORITY_ASCLIN1_RX);
void asclin1RxIsrHandler(void)
{
    char c = asclin1InUart();
    asclin0OutUart(c);
}



void asclin1OutUart(const unsigned char chr)
{
    while (MODULE_ASCLIN1.FLAGS.B.TFL == 0);
    MODULE_ASCLIN1.FLAGSCLEAR.B.TFLC = 1;
    MODULE_ASCLIN1.TXDATA.U = chr;
}

unsigned char asclin1InUart(void)
{
    unsigned char ch;
    while (asclin1PollUart(&ch) == 0);
    return ch;
}

int asclin1PollUart(unsigned char *chr)
{
    if (MODULE_ASCLIN1.FLAGS.B.RFL == 0)
    {
        return 0;
    }
    *chr = (char) MODULE_ASCLIN1.RXDATA.U;
    MODULE_ASCLIN1.FLAGSCLEAR.B.RFLC = 1;
    if (MODULE_ASCLIN1.FLAGS.B.FE || MODULE_ASCLIN1.FLAGS.B.PE || MODULE_ASCLIN1.FLAGS.B.RFO)
    {
        MODULE_ASCLIN1.FLAGSCLEAR.B.FEC = 1;
        MODULE_ASCLIN1.FLAGSCLEAR.B.PEC = 1;
        MODULE_ASCLIN1.FLAGSCLEAR.B.RFOC = 1;
        return 0;
    }
    return 1;
}



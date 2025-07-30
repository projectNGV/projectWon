#include "asclin.h"
#include "IfxAsclin.h"
// #include "IfxAsclin_bf.h"

void Asclin_Init(Asclin_ConfigType asclin){
    uint32 numerator = asclin.baudRate;
    uint32 denominator = 3125;

    // RX PIN IOCR
    IfxPort_setPinMode(asclin.rxPin.port, asclin.rxPin.pinIndex, IfxPort_Mode_input);
    // TX PIN IOCR
    IfxPort_setPinMode(asclin.txPin.port, asclin.txPin.pinIndex, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinState(asclin.txPin.port, asclin.txPin.pinIndex, IfxPort_State_high);

    /* Enable ASCn */
    IfxScuWdt_clearCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());
    asclin.module.CLC.U = 0;
    IfxScuWdt_setCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());
    /* read back for activating module */
    (void)asclin.module.CLC.U;

    /* select RX as input pin */
    asclin.module.IOCR.B.ALTI = 0; // Select Alternate Input A

    /* Program ASC0 */
    asclin.module.CSR.U = 0;

    /* configure TX and RX FIFOs */
    asclin.module.TXFIFOCON.U = (1 << 6)   /* INW: (1 == 1 byte) */
                               | (1 << 1)    /* ENO */
                               | (1 << 0);   /* FLUSH */
    asclin.module.RXFIFOCON.U = (1 << 31)  /* BUF: (1 == Single Stage RX Buffer) */
                               | (1 << 6)    /* OUTW: (1 == 1 byte) */
                               | (1 << 1)    /* ENI */
                               | (1 << 0);   /* FLUSH */

    /* 115200 */
    asclin.module.BITCON.U = ( 9 << 0)     /* PRESCALER: 10 */
                            | (15 << 16)     /* OVERSAMPLING: 16 */
                            | ( 9 << 24)     /* SAMPLEPOINT: position 7,8,9 */
                            | (1u << 31);    /* SM: 3 samples per bit */
    /* data format: 8N1 */
    asclin.module.FRAMECON.U = (1 << 9)        /* STOP: 1 bit */
                              | (0 << 16)    /* MODE: Init */
                              | (0 << 30);   /* PEN: no parity */
    asclin.module.DATCON.U = (7 << 0);     /* DATLEN: 8 bit */

    /* set baudrate value */
    asclin.module.BRG.U = (denominator << 0)   /* DENOMINATOR */
                         | (numerator << 16);    /* NUMERATOR */

    asclin.module.FRAMECON.B.MODE = 1;     /* ASC mode */
    asclin.module.CSR.U = 1;               /* select CLC as clock source */

    asclin.module.FLAGSSET.U = (IFX_ASCLIN_FLAGSSET_TFLS_MSK << IFX_ASCLIN_FLAGSSET_TFLS_OFF);
}

void Asclin_Write(Asclin_ConfigType asclin, const uint8 data){
    while (!(asclin.module.FLAGS.B.TFL != 0));
    /* TX Clear */
    asclin.module.FLAGSCLEAR.U = (IFX_ASCLIN_FLAGSCLEAR_TFLC_MSK << IFX_ASCLIN_FLAGSCLEAR_TFLC_OFF);
    /* send the character */
    asclin.module.TXDATA.U = data;
}

bool Asclin_Read(AsclinConfigType asclin, uint8* read){
    uint8 ret;
    bool res = false;

    if (asclin.module.FLAGS.B.RFL != 0) /* If RX Ready */
    {
        ret = (uint8) asclin.module.RXDATA.U;
        /* acknowledge receive */
        asclin.module.FLAGSCLEAR.U = (IFX_ASCLIN_FLAGSCLEAR_RFLC_MSK << IFX_ASCLIN_FLAGSCLEAR_RFLC_OFF); // RX Clear
        /* check for error condition */
        if ((asclin.module.FLAGS.U) & ((IFX_ASCLIN_FLAGS_PE_MSK << IFX_ASCLIN_FLAGS_PE_OFF) | \
                                        (IFX_ASCLIN_FLAGS_FE_MSK << IFX_ASCLIN_FLAGS_FE_OFF) | \
                                        (IFX_ASCLIN_FLAGS_RFO_MSK << IFX_ASCLIN_FLAGS_RFO_OFF)))
        {
            /* reset error flags */
            asclin.module.FLAGSCLEAR.U = ((IFX_ASCLIN_FLAGSCLEAR_PEC_MSK << IFX_ASCLIN_FLAGSCLEAR_PEC_OFF) | \
                                           (IFX_ASCLIN_FLAGSCLEAR_FEC_MSK << IFX_ASCLIN_FLAGSCLEAR_FEC_OFF) | \
                                           (IFX_ASCLIN_FLAGSCLEAR_RFOC_MSK << IFX_ASCLIN_FLAGSCLEAR_RFOC_OFF));
            /* Buffer clear, reset module */
        }
        else
        {
            /* this is a valid character */
            *read = ret;
            res = true;
        }
    }

    return res;
}
#include "gpt12.h"

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

volatile int cntDelay = 0;

IFX_INTERRUPT(IsrGpt1T3Handler, 0, ISR_PRIORITY_GPT1T3_TIMER);
void IsrGpt1T3Handler (void)
{
//    my_printf("1s from gpt1\n");
    MODULE_GPT120.T3.B.T3 = 25000000;
}

//IFX_INTERRUPT(IsrGpt2T6Handler, 0, ISR_PRIORITY_GPT2T6_TIMER);
//void IsrGpt2T6Handler (void)
//{
//    cntDelay++;
//        if (cntDelay == 100000) {
//            ledtogglefunction();
//            cntDelay = 0;
//        }
//}


IFX_INTERRUPT(IsrGpt2T6Handler, 1, ISR_PRIORITY_GPT2T6_TIMER);
void IsrGpt2T6Handler (void)
{
    ledtogglefunction();
}

void gpt1_init ()
{
    Ifx_GPT12_T3CON_Bits *t3con = (Ifx_GPT12_T3CON_Bits*) &MODULE_GPT120.T3CON.B;

    // GPT Clock 25mhz
    t3con->BPS1 = 2;
    t3con->T3I = 7;
    // 1s
    MODULE_GPT120.T3.B.T3 = 24414;

    t3con->T3M = 0; // Timer Mode
    t3con->T3UD = 1; // Counts down

//    Ifx_SRC_SRCR_Bits* src = (Ifx_SRC_SRCR_Bits*) &MODULE_SRC.GPT12.GPT12[0].T3.B;
//    src->SRPN = ISR_PRIORITY_GPT1T3_TIMER;
//    src->TOS = 0;
//    src->CLRR = 1;
//    src->SRE = 1;
//
//
//    t3con->T3R = 1;

}

// 0.5s blue led toggle
void gpt2_init (void)
{
    Ifx_GPT12_T6CON_Bits *t6con = (Ifx_GPT12_T6CON_Bits*) &MODULE_GPT120.T6CON.B;

    t6con->T6M = 0;
    // 100Mhz / 2048
    t6con->BPS2 = 2;
    t6con->T6I = 7;

    // Down Counts
    t6con->T6UD = 1;
    // Reload Mode
    t6con->T6SR = 1;
    // 0.5s
    MODULE_GPT120.T6.B.T6 = 24414;
    MODULE_GPT120.CAPREL.B.CAPREL = 24414;

    Ifx_SRC_SRCR_Bits *src = (Ifx_SRC_SRCR_Bits*) &MODULE_SRC.GPT12.GPT12[0].T6.B;
    src->SRPN = ISR_PRIORITY_GPT2T6_TIMER;
    src->TOS = 1;
    src->CLRR = 1;
    src->SRE = 1;

    t6con->T6R = 1;
}

void Gpt2_Interrupt_Enable (void)
{
    MODULE_SRC.GPT12.GPT12[0].T6.B.SRE = 1;
}

void Gpt2_Interrupt_Disable (void)
{
    MODULE_SRC.GPT12.GPT12[0].T6.B.SRE = 0;
}

void gpt12Init ()
{
    IfxScuWdt_clearCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());
    MODULE_GPT120.CLC.U = 0;
    IfxScuWdt_setCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());

//    gpt1_init();
    gpt2_init();
}

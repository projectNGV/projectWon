#include "gpt12.h"

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

#define BEEP_INITIAL_INTERVAL 8000     // 초기 주기 (느리게 시작)
#define BEEP_MIN_INTERVAL     3000      // 최소 주기 (0.1초 정도)
#define BEEP_INTERVAL_STEP    1000      // 인터럽트마다 줄이는 양

volatile uint16 g_beepInterval = BEEP_INITIAL_INTERVAL;

IFX_INTERRUPT(IsrGpt1T3Handler, 0, ISR_PRIORITY_GPT1T3_TIMER);
void IsrGpt1T3Handler (void)
{
    // 부저 토글
    MODULE_P10.OUT.B.P4 ^= 1;

    // 주기 감소 (더 이상 줄이지 않도록 최소 한계 체크)
    if (g_beepInterval > BEEP_MIN_INTERVAL)
    {
        g_beepInterval -= BEEP_INTERVAL_STEP;
        if (g_beepInterval < BEEP_MIN_INTERVAL) {
            g_beepInterval = BEEP_MIN_INTERVAL;
        }
    }

    // 새로운 주기로 타이머 설정
    MODULE_GPT120.T3.B.T3 = g_beepInterval;
}

void gpt1_enable(void)
{
    MODULE_SRC.GPT12.GPT12[0].T3.B.SRE = 1;
}

void gpt1_disable(void)
{
    MODULE_SRC.GPT12.GPT12[0].T3.B.SRE = 0;
}

void gpt1_init(void)
{
    Ifx_GPT12_T3CON_Bits *t3con = (Ifx_GPT12_T3CON_Bits*) &MODULE_GPT120.T3CON.B;

    t3con->BPS1 = 2;     // GPT 클럭: 25MHz
    t3con->T3I  = 7;     // 분주비: 2^7 = 128 → 6.25MHz / 128 ≈ 48.8kHz
    t3con->T3M  = 0;     // Timer mode
    t3con->T3UD = 1;     // Down count

    g_beepInterval = BEEP_INITIAL_INTERVAL;
    MODULE_GPT120.T3.B.T3 = g_beepInterval;

    Ifx_SRC_SRCR_Bits *src = (Ifx_SRC_SRCR_Bits*) &MODULE_SRC.GPT12.GPT12[0].T3.B;
    src->SRPN = ISR_PRIORITY_GPT1T3_TIMER;
    src->TOS = 0;
    src->CLRR = 1;
    src->SRE = 0;

    t3con->T3R = 1;  // 타이머 시작
}


IFX_INTERRUPT(IsrGpt2T6Handler, 0, ISR_PRIORITY_GPT2T6_TIMER);
void IsrGpt2T6Handler (void)
{
    ledtogglefunction();
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
    src->TOS = 0;
    src->CLRR = 1;
    src->SRE = 1;

    t6con->T6R = 1;
}


void gpt12Init ()
{
    IfxScuWdt_clearCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());
    MODULE_GPT120.CLC.U = 0;
    IfxScuWdt_setCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());

    gpt1_init();
    gpt2_init();
}

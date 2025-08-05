#include "gpt12.h"

volatile int cntDelay = 0;

// GPT1 타이머 인터럽트 핸들러 (사용 안함)
IFX_INTERRUPT(IsrGpt1T3Handler, 0, ISR_PRIORITY_GPT1T3_TIMER);
void IsrGpt1T3Handler(void)
{
    // 현재는 동작하지 않음
    MODULE_GPT120.T3.B.T3 = 25000000;
}

// GPT2 T6 인터럽트 핸들러 - 0.5초마다 LED 토글
IFX_INTERRUPT(IsrGpt2T6Handler, 0, ISR_PRIORITY_GPT2T6_TIMER);
void IsrGpt2T6Handler(void)
{
    ledUpdateBlinking();
}

// GPT1 타이머 초기화 (현재 사용 안함)
void gpt1_init ()
{
    Ifx_GPT12_T3CON_Bits *t3con = (Ifx_GPT12_T3CON_Bits*) &MODULE_GPT120.T3CON.B;

    t3con->BPS1 = 2;    // 분주기 설정 (25MHz 기준)
    t3con->T3I = 7;     // 인터럽트 우선순위
    t3con->T3M = 0;     // 타이머 모드
    t3con->T3UD = 1;    // 카운트 다운 모드
    MODULE_GPT120.T3.B.T3 = 24414; // 타이머 값

//    Ifx_SRC_SRCR_Bits* src = (Ifx_SRC_SRCR_Bits*) &MODULE_SRC.GPT12.GPT12[0].T3.B;
//    src->SRPN = ISR_PRIORITY_GPT1T3_TIMER;
//    src->TOS = 0;
//    src->CLRR = 1;
//    src->SRE = 1;
//
//    t3con->T3R = 1;

}

// 0.5s blue led toggle
void gpt2_init (void)
{
    Ifx_GPT12_T6CON_Bits *t6con = (Ifx_GPT12_T6CON_Bits*) &MODULE_GPT120.T6CON.B;

    t6con->T6M = 0;         // 타이머 모드
    t6con->BPS2 = 2;        // 분주기 설정
    t6con->T6I = 7;         // 인터럽트 우선순위
    t6con->T6UD = 1;        // 카운트 다운
    t6con->T6SR = 1;        // 자동 리로드

    // 0.5초 타이머 값 (24414 = 대략 0.5초)
    MODULE_GPT120.T6.B.T6 = 24414;
    MODULE_GPT120.CAPREL.B.CAPREL = 24414;

    // 인터럽트 설정
    Ifx_SRC_SRCR_Bits *src = (Ifx_SRC_SRCR_Bits*) &MODULE_SRC.GPT12.GPT12[0].T6.B;
    src->SRPN = ISR_PRIORITY_GPT2T6_TIMER;
    src->TOS = 0;
    src->CLRR = 1;
    src->SRE = 1;

    // 타이머 시작
    t6con->T6R = 1;
}

// GPT2 인터럽트 Enable/Disable 제어
void Gpt2_Interrupt_Enable (void)
{
    MODULE_SRC.GPT12.GPT12[0].T6.B.SRE = 1;
}

void Gpt2_Interrupt_Disable (void)
{
    MODULE_SRC.GPT12.GPT12[0].T6.B.SRE = 0;
}

// GPT12 전체 초기화
void gpt12Init ()
{
    IfxScuWdt_clearCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());
    MODULE_GPT120.CLC.U = 0;
    IfxScuWdt_setCpuEndinit(IfxScuWdt_getGlobalEndinitPassword());

//    gpt1_init();
    gpt2_init();
}

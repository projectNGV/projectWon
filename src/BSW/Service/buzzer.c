#include "buzzer.h"

extern volatile uint16 g_beepInterval;


void buzzerInit (void){

    MODULE_P10.IOCR4.B.PC4 = 0x10;
}

void buzzerToggleOn (void)
{
    g_beepInterval = 8000;
    gpt1_enable();
}

void buzzerToggleOff (void)
{
    gpt1_disable();
    MODULE_P10.OUT.B.P4 = 0;
//    MODULE_P10.IOCR4.B.PC4 = 0x00;

}



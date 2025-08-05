#include "buzzer.h"

extern volatile uint16 g_beepInterval;

void buzzerParkingWarning (int rearDistance)
{
    if(rearDistance < 500)
    {
        Gpt1_Interrupt_Enable();

        if(rearDistance < 200)
        {
            g_beepInterval = 1000;  // 지속 삐——
        }
        if(rearDistance < 400)
        {
            g_beepInterval = 3000;  // 빠르게 삐삐삐
        }

        g_beepInterval = 8000;  // 느리게 삐– 삐–
    }
    else
    {
        Gpt1_Interrupt_Disable();  // 울림 없음
    }
}

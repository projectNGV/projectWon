#include "led.h"

LedNum num = EMPTY;

void ledInit (void)
{
    // p13.2 right
    MODULE_P13.IOCR0.B.PC2 = 0x10;
    // p13.1 left
    MODULE_P13.IOCR0.B.PC1 = 0x10;
    // p13.1 left
    MODULE_P02.IOCR0.B.PC0 = 0x10;

    MODULE_P13.OUT.B.P2 = 0;
    MODULE_P13.OUT.B.P1 = 0;
    MODULE_P02.OUT.B.P0 = 0;
}

void ledToggle (LedNum num_LED)
{
    num = num_LED;
}

void ledtogglefunction (void)
{
    if (num == LED_RIGHT)
    {
        MODULE_P13.OUT.B.P2 ^= 1;
    }
    else if (num == LED_LEFT)
    {
        MODULE_P13.OUT.B.P1 ^= 1;
    }
    else if (num == LED_REAR)
    {
        MODULE_P02.OUT.B.P0 ^= 1;
    }
    else if (num == EMPTY)
    {
        MODULE_P13.OUT.B.P2 = 0;
        MODULE_P13.OUT.B.P1 = 0;
        MODULE_P02.OUT.B.P0 = 0;
    }

}

#include <pin.h>

const IfxPort_Pin LED_PINS[LED_NUM] = {
        [LED_1] = {&MODULE_P00, 5},
        [LED_2] = {&MODULE_P00, 6}
};

const IfxPort_Pin BUZZER_PINS[BUZZER_NUM] = {
        [BUZZER_1] = {&MODULE_P00, 5},
        [BUZZER_2] = {&MODULE_P00, 6}
};

const IfxPort_Pin BUTTON_PINS[BUTTON_NUM] = {
        [BUTTON_1] = {&MODULE_P00, 7}
};

void pinInit ()
{
    for (int i = 0; i < LED_NUM; i++)
    {
        IfxPort_setPinMode(LED_PINS[i].port, LED_PINS[i].pinIndex, IfxPort_Mode_outputPushPullGeneral);
    }

    for (int i = 0; i < BUZZER_NUM; i++)
    {
        IfxPort_setPinMode(BUZZER_PINS[i].port, BUZZER_PINS[i].pinIndex, IfxPort_Mode_outputPushPullGeneral);
    }

    for (int i = 0; i < BUTTON_NUM; i++){
        IfxPort_setPinMode(BUZZER_PINS[i].port, BUZZER_PINS[i].pinIndex, IfxPort_Mode_inputPullUp);
    }
}

#include "led.h"

// 현재 토글 대상 LED를 저장하는 전역 변수
static LedPosition activeLed = LED_NONE;

// LED 초기화 함수: 각 포트를 출력으로 설정하고 초기 상태를 OFF로 설정
void ledInit(void)
{
    ledPortInit();
    activeLed = LED_NONE;
}

// 토글할 LED 지정 함수
void ledStartBlinking(LedPosition pos)
{
    activeLed = pos;
}

void ledStopAll(void)
{
    activeLed = LED_NONE;
    ledSetRight(0);
    ledSetLeft(0);
    ledSetRear(0);
}

// 주기적으로 호출되어 LED 상태를 토글하는 함수
void ledUpdateBlinking(void)
{
    switch (activeLed)
    {
        case LED_RIGHT :
            ledToggleRight();
            break;
        case LED_LEFT :
            ledToggleLeft();
            break;
        case LED_REAR :
            ledToggleRear();
            break;
        case LED_NONE :
        default :
            ledStopAll();
            break;
    }
}

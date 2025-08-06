#include "aeb.h"

volatile bool aebFlag = false;

extern volatile bool tofFlag;
extern MotorState motorState;

void emergencyBuzzer (void)
{
    buzzerOn();
    delayMs(100);
    buzzerOff();
    delayMs(150);
    buzzerOn();
    delayMs(100);
    buzzerOff();
}

void performEmergencyStop(void)
{
    motorMoveReverse(motorState.currentDuty); // 현재 듀티로 역회전

    delayMs(REVERSE_TIME_MS);

    motorState.currentDuty = 0;
    motorStop();
}

void updateAebFlagByTof (unsigned int g_TofValue)
{
    if(tofFlag == false)
    {
        aebFlag = false;
        g_TofValue = 5000;
    }
    else
    {
//        buzzerParkingWarning(g_TofValue);
        if (!aebFlag)  // 아직 AEB가 작동하지 않은 상태
        {
            if (g_TofValue <= DUTY_LIMIT_DISTANCE_MM)
            {
                motorState.baseDuty = 500;  // 제한 거리 이내면 듀티 제한
            }

            if (g_TofValue < AEB_DISTANCE_MM)
            {
                aebFlag = true;  // AEB 작동 조건 충족 → 플래그 ON
            }
        }
        else  // AEB 작동 중일 때
        {
            if (g_TofValue >= SAFETY_DISTANCE_MM)
            {
                aebFlag = false;  // 안전거리 확보 시 AEB 해제
            }
        }
    }
}

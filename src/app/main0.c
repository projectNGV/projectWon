#include "main0.h"

#include "ultrasonic.h"
#include "tof.h"
#include "level.h"
#include "control.h"
#include "aeb.h"

#include "auth.h"

#include <string.h>



extern volatile bool aebFlag;

MotorState motorState = {.baseDuty = 50,      // 사용자 설정 Duty
        .currentDuty = 0,   // 현재 Duty
        .currentDir = '5',  // 현재 주행 방향
        .prevDir = '5',     // 이전 주행 방향
        .lastKeyInput = '5' // 방금 받은 키보드 입력
        };

void main0 (void)
{
    systemInit();
    myPrintf("System Start\n");

    authenticate();

    while (1)
    {
        if (motorState.lastKeyInput == 't')
        {
            tofOnOff();
            delayMs(250);
        }

        // 평상시 주행
        if (aebFlag == false
                || ((motorState.lastKeyInput == '1' || motorState.lastKeyInput == '2' || motorState.lastKeyInput == '3')
                        && aebFlag == true))
        {
            motorUpdateState(&motorState);
            motorRunCommand(&motorState);
        }
        // 긴급제동
        else if (aebFlag == true)
        {
            performEmergencyStop();
        }

//        myPrintf("distance : %d mm,   flag : %d,    duty : %d\n", tofGetValue(), aebFlag, motorState.currentDuty);
//        delayMs(500);
    }
}

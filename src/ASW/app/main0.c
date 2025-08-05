#include "main0.h"

#include "ultrasonic.h"
#include "aeb.h"
#include "tof.h"
#include "level.h"
#include "control.h"
#include "fsm.h"
#include "led.h"
#include "buzzerport.h"

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

    while (1)
    {
        buzzerParkingWarning(450);
        delayMs(500);
        buzzerParkingWarning(300);
        delayMs(500);
        buzzerParkingWarning(100);
        delayMs(500);

        if (motorState.lastKeyInput == 't')
        {
            tofOnOff();
            motorState.lastKeyInput = ' ';
        }

        handleStateMachine(&motorState);

//        myPrintf("distance : %d mm,   flag : %d,    duty : %d\n", tofGetValue(), aebFlag, motorState.currentDuty);
//        delayMs(500);
    }
}

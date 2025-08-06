#include "main0.h"

#include "ultrasonic.h"
#include "aeb.h"
#include "tof.h"
#include "level.h"
#include "control.h"
#include "fsm.h"
#include "led.h"
#include "buzzerport.h"
#include "buzzer.h"
#include "auth.h"
#include "level.h"

MotorState motorState = {
        .baseDuty = 50,      // 사용자 설정 Duty
        .currentDuty = 0,   // 현재 Duty
        .currentDir = '5',  // 현재 주행 방향
        .prevDir = '5',     // 이전 주행 방향
        .lastKeyInput = '5' // 방금 받은 키보드 입력
        };

extern volatile boolean g_isLogin;

void main0 (void)
{
    systemInit();
    myPrintf("System Start\n");

    g_isLogin = TRUE;
    authenticate();

    while (1)
    {
        handleStateMachine(&motorState);

//        myPrintf("distance : %d mm,   tofflag : %d,    aebflag : %d\n", tofGetValue(), tofFlag, aebFlag);
//        delayMs(500);
    }
}

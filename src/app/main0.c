#include "main0.h"

#include "ultrasonic.h"
#include "aeb.h"
#include "tof.h"
#include "control.h"
#include "fsm.h"
#include "led.h"
#include "buzzerport.h"
#include "buzzer.h"
#include "auth.h"

MotorState motorState = {
        .baseDuty = 50,      // 사용자 설정 Duty
        .currentDuty = 0,   // 현재 Duty
        .currentDir = '5',  // 현재 주행 방향
        .prevDir = '5',     // 이전 주행 방향
        .lastKeyInput = '5' // 방금 받은 키보드 입력
        };

extern volatile uint16 g_beepInterval;

void main0 (void)
{
    systemInit();
    stmInterruptInit();
    myPrintf("System Start\n");

//    uint16 beep, delay;
//    myPrintf("beep: ");
//    myScanf("%d", &beep);
//    myPrintf("delay(ms): ");
//    myScanf("%d", &delay);
//    g_beepInterval = beep;
//    MODULE_GPT120.T3.B.T3 = beep;
//    MODULE_GPT120.T3CON.B.T3R = 1;
//    setBuzzerOffTimer(delay);
//
//    while(1){
//        myPrintf("main");
//        delayMs(2000);
//    }



    authenticate();

    while (1)
    {
        handleStateMachine(&motorState);

//        myPrintf("distance : %d mm,   tofflag : %d,    aebflag : %d\n", tofGetValue(), tofFlag, aebFlag);
//        delayMs(500);
    }
}

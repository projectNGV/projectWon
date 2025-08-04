#include "main0.h"

#include "ultrasonic.h"
#include "tof.h"
#include "level.h"
#include "control.h"

extern volatile bool aebFlag;

MotorState motorState = {
        .baseDuty = 50,      // 사용자 설정 Duty
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
        if(motorState.lastKeyInput == 't')
        {
            myPrintf("TOF toggle\n");
            tofOnOff();
            delayMs(250);
        }

        if (aebFlag == false || ((motorState.lastKeyInput == '1' || motorState.lastKeyInput == '2' || motorState.lastKeyInput == '3') && aebFlag == true))
        {
            motorUpdateState(&motorState);
            motorRunCommand(&motorState);
        }
        else if(aebFlag == true)
        {
            MODULE_P10.OUT.B.P1 = 0;
            MODULE_P10.OUT.B.P2 = 0;

            int duty = (motorState.currentDuty < 800) ? 800 : motorState.currentDuty;

            gtmAtomPwmASetDutyCycle(duty);
            gtmAtomPwmBSetDutyCycle(duty);

            delayMs(250);

            motorState.currentDuty = 0;

            motorStop();
        }

        myPrintf("distance : %d mm,   flag : %d,    duty : %d\n", tofGetValue(), aebFlag, motorState.currentDuty);
        //delayMs(500);
    }
}

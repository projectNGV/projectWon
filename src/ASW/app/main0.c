#include "main0.h"

#include "ultrasonic.h"
#include "tof.h"
#include "level.h"
#include "control.h"

MotorState motorState = {
    .baseDuty = 0,      // 사용자 설정 Duty
    .currentDuty = 0,   // 현재 Duty
    .currentDir = '5',  // 현재 주행 방향
    .prevDir = '5',     // 이전 주행 방향
    .lastKeyInput = '5' // 방금 받은 키보드 입력
};

//extern volatile int aebFlag;  // for tof debugging

void main0(void){
    systemInit();
    myPrintf("System Start\n");

    while(1){
        motorUpdateState(&motorState);

        // 최종 동작 호출: 이전에 저장한 방향 + 속도로
        motorRunCommand(&motorState);
//        myPrintf("distance : %d mm,   flag : %d\n", tofGetValue(), aebFlag);  // for tof debugging
//        delayMs(500); // for tof debugging
    }
}

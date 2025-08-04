#include "main0.h"

#include "ultrasonic.h"
#include "tof.h"
#include "level.h"
#include "control.h"

//extern volatile bool aebFlag;

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

    MODULE_P10.IOCR4.B.PC4 = 0x10; // 부저

    MODULE_P13.IOCR0.B.PC1 = 0x10; // led R
    MODULE_P13.IOCR0.B.PC2 = 0x10; // led G
    MODULE_P02.IOCR0.B.PC0 = 0x10; // led B
    while(1){
//        //부저 확인용
//        MODULE_P10.OUT.B.P4 = 1;
//        delayMs(1000);
//        MODULE_P10.OUT.B.P4 = 0;
//        delayMs(1000);

        //led 확인용
        MODULE_P13.OUT.B.P1 = 1;
        MODULE_P13.OUT.B.P2 = 0;
        MODULE_P02.OUT.B.P0 = 0;
        delayMs(200);
        MODULE_P13.OUT.B.P1 = 0;
        MODULE_P13.OUT.B.P2 = 1;
        MODULE_P02.OUT.B.P0 = 0;
        delayMs(200);
        MODULE_P13.OUT.B.P1 = 0;
        MODULE_P13.OUT.B.P2 = 0;
        MODULE_P02.OUT.B.P0 = 1;
        delayMs(200);

        //motorUpdateState(&motorState);

        // 최종 동작 호출: 이전에 저장한 방향 + 속도로
        //motorRunCommand(&motorState);
//        myPrintf("distance : %d mm,   flag : %d\n", tofGetValue(), aebFlag);  // for tof debugging
//        delayMs(500); // for tof debugging
    }
}

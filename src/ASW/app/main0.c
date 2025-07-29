#include "main0.h"

#include "ultrasonic.h"
#include "tof.h"
#include "level.h"

extern volatile char globalCommand;

extern volatile int currentDuty; // a~g 키로 duty값을 변경한다.
extern volatile int baseDuty; // ← 항상 기억해둘 기본 속도
extern volatile char  currentDir; // '8','2','4','6','1','3','7','9' 중 하나, 기본은 정지('5')

void main0(){
    systemInit();
    myPrintf("System Start\n");

//    control_main();
    //levelInit(LEVEL_LEFT);
    while(1){
        motorUpdateState(globalCommand);

        // 최종 동작 호출: 이전에 저장한 방향 + 속도로
        motorRunCommand(currentDir, currentDuty);

        //steer(LEVEL_LEFT);
        delayMs(50);
    }
}

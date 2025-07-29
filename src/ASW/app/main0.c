#include "main0.h"

#include "ultrasonic.h"
#include "tof.h"
#include "level.h"

void main0(){
    systemInit();
    myPrintf("System Start\n");
//    control_main();
    levelInit(LEVEL_LEFT);
    while(1){
        steer(LEVEL_LEFT);
        delayMs(50);
    }
}

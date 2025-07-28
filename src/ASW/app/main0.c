#include "main0.h"

#include "ultrasonic.h"

void main0(){
    systemInit();
    myPrintf("Start\n");

    control_main();
    while(1){
    }
}

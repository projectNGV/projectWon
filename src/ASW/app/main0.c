#include "main0.h"

#include "ultrasonic.h"
#include "tof.h"
#include "level.h"
#include "bluetooth.h"
#include "motor.h"

#include "autopark.h"


int logging = 0;
int start = 0;

//
//void main0(){
//    systemInit();
//
//    while(1){
//        int left = getDistanceByUltra(ULT_LEFT);
//        myPrintf("left: %d\n", left);
//        delayMs(50);
//    }
//
//}


void main0(){
    systemInit();
    while(1){
        char command = asclin0InUart();
        asclin0OutUart(command);

        if(command == 'p'){
            myPrintf("시작합니다! \n");
            parkingAutonomousProcess();

        }

        delayMs(50);
    }
}

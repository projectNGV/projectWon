#include "main0.h"

#include "ultrasonic.h"
#include "tof.h"
#include "level.h"
#include "bluetooth.h"
#include "motor.h"


int logging = 0;
int start = 0;


void main0(){
    systemInit();

    while(1){
        int left = getDistanceByUltra(ULT_LEFT);
        myPrintf("left: %d\n", left);
        delayMs(50);
    }

}

//
//void main0(){
//    systemInit();
//    while(1){
//        char command = bluetoothRecvByteNonBlocked();
//
//        if(command == 'y'){
//            upKp(0,0.0001);
//        }
//
//
//        if(command == 'u'){
//            upKp(1,0.0001);
//        }
//
//
//        if(command == 'i'){
//            upKp(2,0.0001);
//        }
//
//
//        if(command == 'h'){
//            upKp(0,-0.0001);
//        }
//
//        if(command == 'j'){
//            upKp(1,-0.0001);
//        }
//
//        if(command == 'k'){
//            upKp(2,-0.0001);
//        }
//
//        if(command == 'o'){
//            start = !start;
//            bluetoothSendByteBlocked(command);
//            if(start){
//                levelInit(LEVEL_LEFT);
//            } else {
//                motorStop();
//            }
//        }
//
//        if(start){
//            int str = steer(LEVEL_LEFT);
//            bluetoothPrintf("steer: %d\n", str);
//        }
//        if(command == 'x'){
//            motorStop();
//            bluetoothSendByteBlocked(command);
//        }
//
//        if(command == 'w'){
//            motorMoveForward(300);
//            bluetoothSendByteBlocked(command);
//        }
//
//        if(command == 's'){
//            motorReverse(300);
//            bluetoothSendByteBlocked(command);
//
//        }
//        delayMs(50);
//    }
//}

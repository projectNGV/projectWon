#include "autopark_teststub.h"



void motorStop_test(void){

    myPrintf("Motor STOP!!!\n");

}

void motorMoveForward_test(int duty){

    myPrintf("Motor Mover %d !\n", duty);
}

void parkReverse(){
    myPrintf("후진 주차 완료!!!\n");
}

void steer_test(LevelDir dir){

    static int callCount_1 = 0;
    callCount_1++;

    if(callCount_1 > 10000){
        callCount_1 = 0;
        myPrintf("정렬 완료! \n");
    }

}

uint32 getFilteredDistance_test(UltraDir dir){

    static int callCount_2 = 0;
    callCount_2++;
    if(callCount_2 < 300){
        return 50;
    }
    else if(callCount_2 < 320){
        return 200;
    }
    else if(callCount_2 < 500){
        return 50;
    }
    else
        return 200;
}





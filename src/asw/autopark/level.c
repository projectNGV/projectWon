#include "level.h"

#define FILTER_SIZE 7
float Kp = 0.001;
float Ki = 0.0;
//float Kd = 0.0;

float error = 0;
float last_error = 0;
float integral = 0;
//float derivative = 0;

int steering_output = 0;

uint32 readings[FILTER_SIZE] = {0};
int read_index = 0;
uint32 total = 0;
uint32 filtered_distance = 0;

uint32 previous_filtered_distance = 0;
uint32 current_filtered_distance = 0;

void upKp(int n, float i){
    if(n == 0){
        Kp += i;
        bluetoothPrintf("cur Kp: %f\n", Kp);
    }
    if(n == 1){
            Ki += i;
            bluetoothPrintf("cur Ki: %f\n", Ki);
        }
//
//    if(n == 2){
//            Kd += i;
//            bluetoothPrintf("cur Kd: %f\n", Kd);
//        }
}

int dis;
static uint32 getFilteredDistance (UltraDir dir)
{
    total = total - readings[read_index];
    dis = getDistanceByUltra(dir);
    while (dis < 0)
    {
        dis = getDistanceByUltra(dir);
    }
    readings[read_index] = dis;
    total = total + readings[read_index];
    read_index = (read_index + 1) % FILTER_SIZE;
    return total / FILTER_SIZE;
}

void levelInit (LevelDir dir)
{
    UltraDir ultDir = (dir == LEVEL_LEFT) ? ULT_LEFT : ULT_RIGHT;
    integral = 0;
    for (int i = 0; i < FILTER_SIZE; i++)
    {
        getFilteredDistance(ultDir);
        delayMs(50);
    }
    previous_filtered_distance = readings[FILTER_SIZE - 1];
}

int steer (LevelDir dir)
{

    UltraDir ultDir = ((dir == LEVEL_LEFT) ? ULT_LEFT : ULT_RIGHT);
    current_filtered_distance = getFilteredDistance(ultDir);
    error = (float) previous_filtered_distance - (float) current_filtered_distance;
    integral = integral + error;
//    derivative = error - last_error;
    steering_output = (int) (Kp * error + Ki * integral);
//    steering_output = (int) (Kp * error + Ki * integral + Kd * derivative);
    last_error = error;
    previous_filtered_distance = current_filtered_distance;

//    myPrintf("error: %f\tintegral: %f\tde: %f\tstr: %d\n", error, integral, derivative, steering_output);
    /*
     * steer
     * 멀어지면 - => 반대편 바퀴 up / 내쪽 바퀴 down
     * 가까워지면 + => 내쪽 바퀴 up / 반대편 바퀴 down
     */

    if(steering_output < -200) steering_output = -200;
    if(steering_output > 200) steering_output = 200;

    if(dir == LEVEL_RIGHT) steering_output *= -1;

//    if( steering_output > 0 ){
//        motorMovChAPwm(300, 1);
//        motorMovChBPwm(300 + steering_output, 1);
//    }
//    else{
//        motorMovChAPwm(300 - steering_output, 1);
//        motorMovChBPwm(300, 1);
//    }

    motorMovChAPwm(300 + steering_output, 1);
    motorMovChBPwm(300 - steering_output, 1);


    return steering_output;
}

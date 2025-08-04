#include "level.h"
#include "bluetooth.h"
#include <math.h>
#include <stdlib.h>

#define FILTER_SIZE 10

#define ABNORMAL_DIFF 5000

// MUST BE TUNED
#define PARKING_DISTANCE 150000

#define PARKING_TICK 3

#define R_TUNE_DELAY 100

#define R_90_DELAY 263 //  90도 꺾
#define R_DELAY 150

#define R_WEIGHT 1070 //

float Kp = 0.1;
float Ki = 0.0;
float Kd = -0.05;

float error = 0;
float last_error = 0;
float integral = 0;
float derivative = 0;

int steering_output = 0;

uint32 readings[FILTER_SIZE] = {0};
int read_index = 0;
int cur_readings_num = 0;
uint32 total = 0;
uint32 filtered_distance = 0;

uint32 previous_filtered_distance = 0;
uint32 current_filtered_distance = 0;

int ultDis;

boolean found = FALSE;
uint8 tick = 0;

void printState (void)
{
    bluetoothPrintf("Cur Gain: %f\t%f\t%f\n", Kp, Ki, Kd);
}

void setGain (int n, float i)
{
    if (n == 0)
    {
        Kp = i;
        bluetoothPrintf("cur Kp: %f\n", Kp);
    }
    if (n == 1)
    {
        Ki = i;
        bluetoothPrintf("cur Ki: %f\n", Ki);
    }

    if (n == 2)
    {
        Kd = i;
        bluetoothPrintf("cur Kd: %f\n", Kd);
    }
}

static uint32 getFilteredDistance (int distance)
{
//    bluetoothPrintf("[GetFilteredDistance] distance: %d\n", distance);
    total -= readings[read_index];

    readings[read_index] = distance;
    total += distance;
    read_index = (read_index + 1) % FILTER_SIZE;

    if (cur_readings_num < FILTER_SIZE)
    {
        cur_readings_num++;
    }
//    bluetoothPrintf("[GetFilteredDistance] total: %d\n", total);
//    bluetoothPrintf("[GetFilteredDistance] cur_readings_num: %d\n", cur_readings_num);
//    bluetoothPrintf("[GetFilteredDistance] return total / cur_reading_num: %d\n", total / cur_readings_num);
    return total / cur_readings_num;
}

void levelInit (LevelDir dir)
{
    UltraDir ultDir = (dir == LEVEL_LEFT) ? ULT_LEFT : ULT_RIGHT;

    integral = 0;
    last_error = 0;

    total = 0;
    read_index = 0;
    cur_readings_num = 0;

    for (int i = 0; i < FILTER_SIZE; i++)
    {
        readings[i] = 0;
    }

    int ultDis = getDistanceByUltra(ultDir);
    while (ultDis < 0)
    {
        ultDis = getDistanceByUltra(ultDir);
    }
    previous_filtered_distance = getFilteredDistance(ultDis);

    boolean found = FALSE;
    uint8 tick = 0;
}

typedef enum
{
    FOUNDWALL, FOUNDEDGE, FOUNDREAR
} ParkingStatus;

void auto_park ()
{
    UltraDir ultDir = ULT_LEFT;
    bluetoothPrintf("주차 시작");
    motorMovChAPwm(400, 1);
    motorMovChBPwm(400, 1);
    while (1)
    {
        ultDis = getDistanceByUltra(ultDir);
        while (ultDis < 0)
        {
            ultDis = getDistanceByUltra(ultDir);
        }

        bluetoothPrintf("ULT DISTANCE: %d\n", ultDis);

        // 너무 반응 느린건지 뭔지 이상함.
        if (ultDis >= PARKING_DISTANCE)
        {
            if (found)
            {

                bluetoothPrintf("[autopark] #%d Ticking Distance: %d\n", tick + 1, ultDis);
                if (++tick >= PARKING_TICK)
                {
                    bluetoothPrintf("[autopark] Parking Spot Found\n");
                    motorStop();
                    delayMs(500);
                    motorMovChAPwm(1000, 0);
                    motorMovChBPwm(1000, 0);
                    delayMs(R_TUNE_DELAY);
                    motorStop();
                    delayMs(500);
                    motorMovChAPwm(0, 1);
                    motorMovChBPwm(1000, 0);
                    delayMs(R_90_DELAY);
                    motorStop();
                    delayMs(400);
                    int rearDis = getDistanceByUltra(ULT_REAR);
                    bluetoothPrintf("[autopark] Rear Distance: %d\n", rearDis);
                    delayMs(100);
                    motorMovChAPwm(1000, 0);
                    motorMovChBPwm(1000, 0);
                    double cal = 104.20 * log(rearDis) - 1076.31;
                    bluetoothPrintf("[autopark] cal: %f\n", cal);
                    delayMs(cal);
                    motorStop();
                    bluetoothPrintf("done\n");
                    found = FALSE;
                    tick = 0;
                    return;

                }

            }
            else
            {
                found = TRUE;
                tick = 0;
                bluetoothPrintf("[autopark] #%d Ticking Start: %d\n", tick, ultDis);
            }
        }
        else
        {
            found = FALSE;
        }
    }
}

int getMv (LevelDir dir)
{
    UltraDir ultDir = ((dir == LEVEL_LEFT) ? ULT_LEFT : ULT_RIGHT);

    ultDis = getDistanceByUltra(ultDir);
    while (ultDis < 0)
    {
        ultDis = getDistanceByUltra(ultDir);
    }

    bluetoothPrintf("ULT DISTANCE: %d\n", ultDis);

    // 너무 반응 느린건지 뭔지 이상함.
    if (ultDis >= PARKING_DISTANCE)
    {
        if (found)
        {

            bluetoothPrintf("[autopark] #%d Ticking Distance: %d\n", tick + 1, ultDis);

            int park_prevDis;

            if (++tick >= PARKING_TICK)
            {
                bluetoothPrintf("[autopark] Parking Spot Found\n");
                motorStop();
                delayMs(1000);
                park_prevDis = getDistanceByUltra(ULT_REAR);
                delayMs(50);
                motorMovChAPwm(550, 1);
                motorMovChBPwm(50, 1);
                for (int i = 700 / 50; i > 0; i--)
                {
                    int park_dis = getDistanceByUltra(ULT_REAR);
                    bluetoothPrintf("[autopark] REAR DISTANCE: %d\n", park_dis);
                    delayMs(50);

                    if (abs(park_dis - park_prevDis) > 200000)
                    {
                        bluetoothPrintf("[autopark] ABNORMAL STOP\n");
                        motorStop();
                        delayMs(1000);
                        motorMovChAPwm(550, 1);
                        motorMovChBPwm(50, 1);
                        delayMs(50);
                    }

                    park_prevDis = park_dis;
                }
                motorStop();
                delayMs(1000);

                motorMovChAPwm(75, 0);
                motorMovChBPwm(600, 0);
                for (int i = 1450 / 50; i > 0; i--)
                {
                    int park_dis = getDistanceByUltra(ULT_REAR);
                    bluetoothPrintf("[autopark] REAR DISTANCE: %d\n", park_dis);
                    delayMs(50);

                    if (abs(park_dis - park_prevDis) > 200000)
                    {
                        bluetoothPrintf("[autopark] ABNORMAL STOP\n");
                        motorStop();
                        delayMs(1000);
                        motorMovChAPwm(75, 0);
                        motorMovChBPwm(600, 0);
                        delayMs(50);
                    }

                    park_prevDis = park_dis;
                }
                motorStop();
                delayMs(3000);

                bluetoothPrintf("[autopark] Park Completed\n");
                found = FALSE;
                tick = 0;
            }

        }
        else
        {
            found = TRUE;
            tick = 0;
            bluetoothPrintf("[autopark] #%d Ticking Start: %d\n", tick, ultDis);
        }
    }
    else
    {
        found = FALSE;
    }

//    bluetoothPrintf("[getMv] ultDis: %d\n", ultDis);
    current_filtered_distance = getFilteredDistance(ultDis);

//    bluetoothPrintf("[getMv] curfilteredDistance: %d\n", current_filtered_distance);
    error = (float) previous_filtered_distance - (float) current_filtered_distance;

    if (error >= ABNORMAL_DIFF || error <= -ABNORMAL_DIFF)
    {
        levelInit(dir);
        return 0;
    }
    integral += error;

    derivative = error - last_error;

    if (cur_readings_num == 2)
        derivative = 0;

    int output = (int) (Kp * error + Ki * integral + Kd * derivative);

    last_error = error;
    previous_filtered_distance = current_filtered_distance;

    bluetoothPrintf("%f,%f,%f,", error, integral, derivative);
    return output;
}

#include "level.h"
#include "bluetooth.h"

#define FILTER_SIZE 10

// MUST BE TUNED
#define ABNORMAL_DIFF 5000
#define PARKING_DISTANCE 150000
#define PARKING_TICK 4
#define MAX_STEERING 200

float Kp = 0.1;
float Ki = 0.0;
float Kd = -0.05;

float error = 0;
float last_error = 0;
float integral = 0;
float derivative = 0;

int steering_output = 0;

uint32 readings[FILTER_SIZE] = {0};
uint32 readings_2[FILTER_SIZE] = {0};

int read_index = 0;
int read_index_2 = 0;
int cur_readings_num = 0;
int cur_readings_num_2 = 0;

uint32 total = 0;
uint32 total_2 = 0;

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

static uint32 getFilteredDistance_2 (int distance)
{
//    bluetoothPrintf("[GetFilteredDistance] distance: %d\n", distance);
    total_2 -= readings_2[read_index_2];

    readings_2[read_index_2] = distance;
    total_2 += distance;
    read_index_2 = (read_index_2 + 1) % FILTER_SIZE;

    if (cur_readings_num_2 < FILTER_SIZE)
    {
        cur_readings_num_2++;
    }
//    bluetoothPrintf("[GetFilteredDistance] total: %d\n", total);
//    bluetoothPrintf("[GetFilteredDistance] cur_readings_num: %d\n", cur_readings_num);
//    bluetoothPrintf("[GetFilteredDistance] return total / cur_reading_num: %d\n", total / cur_readings_num);
    return total_2 / cur_readings_num_2;
}

void levelInit (LevelDir dir)
{
    UltraDir ultDir = (dir == LEVEL_LEFT) ? ULT_LEFT : ULT_RIGHT;

    integral = 0;
    last_error = 0;

    total = 0;
    total_2 = 0;

    read_index = 0;
    read_index_2 = 0;
    cur_readings_num = 0;
    cur_readings_num_2 = 0;

    for (int i = 0; i < FILTER_SIZE; i++)
    {
        readings[i] = 0;
    }

    for (int i = 0; i < FILTER_SIZE; i++)
    {
        readings_2[i] = 0;
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

void auto_park ()
{
    UltraDir ultDir = ULT_LEFT;
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
                    motorMovChAPwm(1000, 1);
                    motorMovChBPwm(0, 1);
                    delayMs(100);
                    motorStop();
                    delayMs(500);
                    motorMovChAPwm(0, 1);
                    motorMovChBPwm(1000, 0);
                    delayMs(220);
                    motorStop();
                    delayMs(500);
                    motorMovChAPwm(1000, 0);
                    motorMovChBPwm(1000, 0);
                    delayMs(200);
                    motorStop();
                    delayMs(500);

                    bluetoothPrintf("done\n");
                    while (1)
                        ;

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
            if (++tick >= PARKING_TICK)
            {
                bluetoothPrintf("[autopark] Parking Spot Found\n");
                motorStop();
                delayMs(500);
                motorMovChAPwm(1000, 1);
                motorMovChBPwm(1000, 1);
                delayMs(10);
                motorStop();
                delayMs(500);
                motorMovChAPwm(300, 1);
                motorMovChBPwm(1000, 0);
                delayMs(250);
                motorStop();
                delayMs(500);
                motorMovChAPwm(1000, 0);
                motorMovChBPwm(1000, 0);
                delayMs(200);
                motorStop();
                delayMs(500);

                bluetoothPrintf("done\n");
                while (1)
                    ;

                while (1)
                {

                    bluetoothPrintf("조정 드가자\n");
                    //    bluetoothPrintf("[getMv] ultDis: %d\n", ultDis);
                    current_filtered_distance = getFilteredDistance(ULT_RIGHT);

                    //    bluetoothPrintf("[getMv] curfilteredDistance: %d\n", current_filtered_distance);
                    error = (float) previous_filtered_distance - (float) current_filtered_distance;

                    integral += error;

                    derivative = error - last_error;

                    if (cur_readings_num_2 == 2)
                        derivative = 0;

                    int output = (int) (Kp * error + Ki * integral + Kd * derivative);

                    last_error = error;
                    previous_filtered_distance = current_filtered_distance;

                    if (error >= ABNORMAL_DIFF || error <= -ABNORMAL_DIFF)
                    {
                        levelInit(ULT_RIGHT);
                        output = 0;
                    }
                    if (output < -MAX_STEERING)
                        output = -MAX_STEERING;
                    if (output > MAX_STEERING)
                        output = MAX_STEERING;
                    motorMovChAPwm(300 + output, 0);
                    motorMovChBPwm(300 - output, 0);
                    delayMs(50);

                    if (getDistanceByUltra(ULT_RIGHT) < 17000 && getDistanceByUltra(ULT_REAR) < 30000)
                    {
                        bluetoothPrintf("done!!!!!\n\n\n\n\n\n");
                        break;
                    }

                }
                bluetoothPrintf("[autopark] Park Completed\n");
                found = FALSE;
                tick = 0;

                while (1)
                    ;

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

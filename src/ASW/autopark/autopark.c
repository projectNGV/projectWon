#include "autopark.h"

#include "asclin1.h"
#include "ultrasonic.h"
#include <stdlib.h>
#include <stdio.h>

#define MOTOR_STOP_DELAY 500

// Parking Distance
static int parkingDistance = 150000;

// Parking Tick
static int parkingFoundSpeed = 400;
static int parkingFoundTick = 3;

// Forward & Rotate 90 Degree Rightward
static int goForwardDelay = 100;
static int rotateDelay = 263;

// RX
extern volatile boolean g_rx_getLine;
extern volatile char g_rx_buffer[RX_BUFFER_SIZE];

static void foundSpace ()
{
    int curTick = 0;
    motorMoveForward(parkingFoundSpeed);
    while (curTick < parkingFoundTick)
    {
        if (parkingDistance <= getDistanceByUltra(ULT_LEFT))
        {
            curTick++;
            if (curTick < parkingFoundTick)
                break;
        }
        else
        {
            curTick = 0;
        }
        delayMs(50);
    }
    motorStop();
}

static int getTargetDelay (int rearDistance)
{
    return 104.20 * log(rearDistance) - 1076.31;
}

static void tuneParkingDistance ()
{
    while (1)
    {
        bluetoothPrintf("?주차 공간 입력 [c] - 왼쪽 초음파 거리, [y] - 확인 (현재거리: %d)\n", parkingDistance);
        while (!g_rx_getLine)
            ;
        if (g_rx_buffer[0] == 'c')
        {
            int leftDis = getDistanceByUltra(ULT_LEFT);
            bluetoothPrintf("현재 초음파 거리: %d\n", leftDis);
        }
        else if (g_rx_buffer[0] == 'y')
        {
            bluetoothPrintf("주차 공간 설정 완료: %d\n", parkingDistance);
            rxBufferFlush();
            break;
        }
        else
        {
            parkingDistance = atoi(g_rx_buffer);
            bluetoothPrintf("parkingDistance 변경 완료: %d\n", parkingDistance);
        }
        rxBufferFlush();
    }
}

static void tuneParkingFoundSpeed ()
{
    while (1)
    {
        bluetoothPrintf("?[주차 공간 찾기] 직진 속도 조절 [y] - 확인 (현재 속도: %d)\n", parkingFoundSpeed);
        while (!g_rx_getLine)
            ;
        if (g_rx_buffer[0] == 'y')
        {
            bluetoothPrintf("직진 속도 설정 완료: %d\n", parkingFoundSpeed);
            rxBufferFlush();
            break;
        }
        else
        {
            parkingFoundSpeed = atoi(g_rx_buffer);
        }
        rxBufferFlush();

        /**
         * 직진 보여줘야 되는데 몇초나 할지
         */
        motorMoveForward(parkingFoundSpeed);
        delayMs(MOTOR_STOP_DELAY);
        motorStop();
    }
}

static void tuneParkingFoundTick ()
{

    while (1)
    {
        bluetoothPrintf("?[주차 공간 찾기] Tick 값 설정 [y] - 확인 (현재 Tick: %d)\n", parkingFoundTick);
        while (!g_rx_getLine)
            ;
        if (g_rx_buffer[0] == 'y')
        {
            bluetoothPrintf("Tick 값 설정 완료: %d\n", parkingFoundTick);
            rxBufferFlush();
            break;
        }
        else
        {
            parkingFoundTick = atoi(g_rx_buffer);
        }
        rxBufferFlush();

        foundSpace();
    }

}

static void rotate ()
{
    motorMoveForward(1000);
    delayMs(goForwardDelay);
    motorStop();
    delayMs(MOTOR_STOP_DELAY);
    motorMovChAPwm(0, 1);
    motorMovChBPwm(1000, 0);
    delayMs(rotateDelay);
    motorStop();
}

static void tuneRotate ()
{
    while (1)
    {
        bluetoothPrintf("[주차] 직진 & 후진 딜레이 조절 (현재 직진 딜레이: %d, 후진 딜레이: %d)\n", goForwardDelay, rotateDelay);
        bluetoothPrintf("?[y] - 확인 [r] - 주차 공간 찾기\n");
        while (!g_rx_getLine)
            ;
        if (g_rx_buffer[0] == 'y')
        {
            bluetoothPrintf("딜레이 설정 완료 직진: %d\t후진: %d\n", goForwardDelay, rotateDelay);
            rxBufferFlush();
            break;
        }
        else if (g_rx_buffer[0] == 'r')
        {
            foundSpace();
        }
        else
        {
            sscanf(g_rx_buffer, "%d %d", &goForwardDelay, &rotateDelay);
        }
        rxBufferFlush();
        rotate();
    }
}

static boolean isTuned = FALSE;

void autoParkTune (void)
{
//    tuneParkingDistance();
//    tuneParkingFoundSpeed();
//    tuneParkingFoundTick();
    tuneRotate();
    while (1)
    {
        bluetoothPrintf("===========최종값===========");
        bluetoothPrintf("1. 주차공간: %d\n", parkingDistance);
        bluetoothPrintf("2. 전진속도: %d\n", parkingFoundSpeed);
        bluetoothPrintf("3. Tick: %d\n", parkingFoundTick);
        bluetoothPrintf("4. 전진 딜레이: %d\t", goForwardDelay);
        bluetoothPrintf("4. 후진 우회전 딜레이: %d\n", rotateDelay);
        bluetoothPrintf("?[r] - 시험\t[c]- 확인\t[#]- 재설정\n");
        while (!g_rx_getLine)
            ;
        switch (g_rx_buffer[0])
        {
            case 'r' :
                autoPark();
                break;
            case 'c' :
                isTuned = true;
                break;
            case '1' :
                tuneParkingDistance();
                break;
            case '2' :
                tuneParkingFoundSpeed();
                break;
            case '3' :
                tuneParkingFoundTick();
                break;
            case '4' :
                tuneRotate();
                break;
            default :
                bluetoothPrintf("UNKNOWN COMMAND\n");
                break;
        }

        if (isTuned)
            break;
    }
}

void autoPark (void)
{
    foundSpace();
    rotate();
    delayMs(MOTOR_STOP_DELAY);
    int dis = getDistanceByUltra(ULT_REAR);
    motorMoveReverse(1000);
    delayMs(getTargetDelay(dis));
    motorStop();
    delayMs(MOTOR_STOP_DELAY);
}

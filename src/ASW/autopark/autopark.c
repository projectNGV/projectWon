/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/

#include "autopark.h"

#include "asclin1.h"
#include "ultrasonic.h"

#include <stdlib.h>
#include <stdio.h>

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/

#define MOTOR_STOP_DELAY 500

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/

extern volatile boolean g_rx_getLine;
extern volatile char g_rx_buffer[RX_BUFFER_SIZE];

/*********************************************************************************************************************/
/*--------------------------------------------Private Variables/Constants--------------------------------------------*/
/*********************************************************************************************************************/

// Parking Distance
static int parkingDistance = 150000;

// Parking Speed
static int parkingSpeedForward = 400;
static int parkingSpeedBackward = 400;

// Parking Tick
static int parkingFoundTick = 3;

// Forward & Rotate 90 Degree Rightward
static int goForwardDelay = 100;
static int rotateDelay = 263;

// Backward Stop Distance
static int stopDistance = 35000;

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/

static void foundSpace ();
static void rotate ();
static void goBackWard ();
static void tuneParkingDistance ();
static void tuneParkingSpeed ();
static void tuneParkingFoundTick ();
static void tuneRotate ();
static void tuneStopDistance ();
void autoParkTune ();
void autoPark ();

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/

static void foundSpace ()
{
    int curTick = 0;
    motorMoveForward(parkingSpeedForward);
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

static void goBackWard ()
{
    motorMoveReverse(parkingSpeedBackward);
    int rearDis = getDistanceByUltra(ULT_REAR);
    while (rearDis > stopDistance)
    {
        delayMs(50);
        rearDis = getDistanceByUltra(ULT_REAR);
    }
    motorStop();
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

static void tuneParkingSpeed ()
{
    while (1)
    {
        bluetoothPrintf("[주차 공간 찾기] 직진 후진 속도 조절 (현재 속도: %d)\n", parkingSpeedForward);
        bluetoothPrintf("?[y] - 확인\t직진 후진 속도\n");
        while (!g_rx_getLine)
            ;
        if (g_rx_buffer[0] == 'y')
        {
            bluetoothPrintf("속도 설정 완료\n", parkingSpeedForward);
            bluetoothPrintf("직진: %d\t후진: %d\n", parkingSpeedForward, parkingSpeedBackward);
            rxBufferFlush();
            break;
        }
        else
        {
            sscanf(g_rx_buffer, "%d %d", &parkingSpeedForward, &parkingSpeedBackward);
        }
        rxBufferFlush();

        /**
         * 직진 보여줘야 되는데 몇초나 할지
         */
        motorMoveForward(parkingSpeedForward);
        delayMs(1000);
        motorStop();
        delayMs(MOTOR_STOP_DELAY);
        motorMoveReverse(parkingSpeedBackward);
        delayMs(1000);
        motorStop();
        delayMs(MOTOR_STOP_DELAY);
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

static void tuneStopDistance ()
{
    while (1)
    {
        bluetoothPrintf("[주차] 후진 거리 조절 (현재 후진 거리: %d)\n", stopDistance);
        bluetoothPrintf("?[c] - 뒤쪽 거리 출력\t[y] - 확인 \n");
        while (!g_rx_getLine)
            ;
        if (g_rx_buffer[0] == 'y')
        {
            bluetoothPrintf("후진 거리 설정 완료 직진: %d\n", stopDistance);
            rxBufferFlush();
            break;
        }
        else if (g_rx_buffer[0] == 'c')
        {
            int rearDis = getDistanceByUltra(ULT_REAR);
            bluetoothPrintf("현재 초음파 거리: %d\n", rearDis);
        }
        else
        {
            stopDistance = atoi(g_rx_buffer);
        }
        rxBufferFlush();
        goBackWard();
    }
}

void autoParkTune ()
{
    boolean isTuned = FALSE;

    while (1)
    {
        bluetoothPrintf("===========현재 값===========");
        bluetoothPrintf("1. [주차 공간 찾기] 주차공간: %d\n", parkingDistance);
        bluetoothPrintf("2. [주차 공간 찾기] 전진속도: %d\n", parkingSpeedForward);
        bluetoothPrintf("2. [주차 공간 찾기] 후진속도: %d\n", parkingSpeedBackward);
        bluetoothPrintf("3. [주차 공간 찾기] Tick: %d\n", parkingFoundTick);
        bluetoothPrintf("4. [주차] 전진 딜레이: %d\t", goForwardDelay);
        bluetoothPrintf("4. [주차] 후진 우회전 딜레이: %d\n", rotateDelay);
        bluetoothPrintf("5. [주차] 후진 거리: %d\n", stopDistance);
        bluetoothPrintf("?[r] - 시험 주행\t[c]- 확인\t[#]- 재설정\n");
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
                tuneParkingSpeed();
                break;
            case '3' :
                tuneParkingFoundTick();
                break;
            case '4' :
                tuneRotate();
                break;
            case '5' :
                tuneStopDistance();
                break;
            default :
                bluetoothPrintf("UNKNOWN COMMAND\n");
                break;
        }

        if (isTuned)
            break;
    }
}

void autoPark ()
{
    foundSpace();
    rotate();
    delayMs(MOTOR_STOP_DELAY);
    goBackWard();
}

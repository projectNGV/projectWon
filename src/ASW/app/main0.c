#include "main0.h"

#include "ultrasonic.h"
#include "tof.h"
#include "level.h"
#include "control.h"
#include "aeb.h"

#include <string.h>

#define RX_BUFFER_SIZE 32

extern volatile bool aebFlag;

MotorState motorState = {.baseDuty = 50,      // 사용자 설정 Duty
        .currentDuty = 0,   // 현재 Duty
        .currentDir = '5',  // 현재 주행 방향
        .prevDir = '5',     // 이전 주행 방향
        .lastKeyInput = '5' // 방금 받은 키보드 입력
        };

extern volatile boolean g_getLine;
extern volatile char g_rx_buffer[];
extern volatile int g_rx_idx;
extern volatile boolean g_isLogin;

static const char *password = "1234";

void main0 (void)
{
    systemInit();
    myPrintf("System Start\n");
    while (!g_isLogin)
    {
        if (g_getLine)
        {
            myPrintf("Received from ISR: %s\n", (const char*) g_rx_buffer);
            if (strcmp((const char*) g_rx_buffer, password) == 0)
            {
                g_isLogin = TRUE;
                bluetoothPrintf("OK\n");
            }
            else
            {
                bluetoothPrintf("FAILED\n");
            }

            g_rx_idx = 0;
            memset((void*) g_rx_buffer, 0, RX_BUFFER_SIZE);
            g_getLine = FALSE;
        }
    }

    while (1)
    {
        if (motorState.lastKeyInput == 't')
        {
            tofOnOff();
            delayMs(250);
        }

        // 평상시 주행
        if (aebFlag == false
                || ((motorState.lastKeyInput == '1' || motorState.lastKeyInput == '2' || motorState.lastKeyInput == '3')
                        && aebFlag == true))
        {
            motorUpdateState(&motorState);
            motorRunCommand(&motorState);
        }
        // 긴급제동
        else if (aebFlag == true)
        {
            performEmergencyStop();
        }

//        myPrintf("distance : %d mm,   flag : %d,    duty : %d\n", tofGetValue(), aebFlag, motorState.currentDuty);
//        delayMs(500);
    }
}

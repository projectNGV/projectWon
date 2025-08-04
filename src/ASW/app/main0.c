#include "main0.h"
#include "asclin1.h"
#include "asclin0.h"
#include "ultrasonic.h"
#include "tof.h"
#include "level.h"
#include "control.h"
#include <string.h>
#include <stdint.h>

#define MAX_SPEED 75
#define MIN_SPEED 25

extern volatile int aebFlag;
volatile int engineOn = 0; //시동 상태 플래그 (인터럽트에서도 사용)


MotorState motorState = {
    .baseDuty = 50,
    .currentDuty = 0,
    .currentDir = '5',
    .prevDir = '5',
    .lastKeyInput = '5'
};

void disableAsclin1RxInterrupt(void) {
    volatile Ifx_SRC_SRCR *src = &MODULE_SRC.ASCLIN.ASCLIN[1].RX;
    src->B.SRE = 0;
}

void enableAsclin1RxInterrupt(void) {
    volatile Ifx_SRC_SRCR *src = &MODULE_SRC.ASCLIN.ASCLIN[1].RX;
    src->B.SRE = 1;
}

void flushUartBuffer(void) {
    unsigned char dummy;
    while (asclin1PollUart(&dummy) != 0) {
        // 버퍼에 남은 데이터 모두 비움
    }
}

void main0(void) {
    systemInit();
    myPrintf("System Start\n");



    const char *password = "1234"; // 정답 암호
        char buf[20];
        int idx = 0;




        // 블루투스 UART 초기화 (암호 수신용)
            asclin1InitUart();

            // 암호 입력 중 인터럽트 OFF
            disableAsclin1RxInterrupt();

            // ===== 암호 인증 루프 =====
            while (!engineOn)
            {
                unsigned char c = asclin1InUart();

                if (c == '\r' || c == '\n')
                {
                    buf[idx] = '\0';

                    if (strcmp(buf, password) == 0)
                    {
                        const char *success = "OK\n";
                        for (int i = 0; success[i] != '\0'; i++)
                            asclin1OutUart(success[i]);
                        myPrintf("✅ Password matched. Sent OK\n");  // ✅ 확인용 출력
                        engineOn = 1;
                        enableAsclin1RxInterrupt();
                    }
                    else
                    {
                        const char *fail = "FAIL\n";
                        for (int i = 0; fail[i] != '\0'; i++)
                            asclin1OutUart(fail[i]);
                        myPrintf("❌ Password incorrect. Sent FAIL\n");  // ✅ 확인용 출력
                        flushUartBuffer();

                        const char *msg = "Enter Password:\n";
                        for (int i = 0; msg[i] != '\0'; i++)
                            asclin1OutUart(msg[i]);
                    }

                    idx = 0;
                }
                else
                {
                    if (idx < sizeof(buf) - 1)
                        buf[idx++] = c;
                }
            }






    // 메인 루프
    while (1) {
        //크루즈 컨트롤 (앞차와의 간격 유지 / 주행방향은 안따라감)
        int distance = tofGetValue(); // 현재 거리 (mm)
        int newDuty = motorState.currentDuty;

        if(distance > 420) newDuty++; //증가량 테스트를 통해 수정 필요
        if(distance < 380) newDuty--; //감소량 테스트를 통해 수정 필요

        if (newDuty > MAX_SPEED) newDuty = MAX_SPEED; //속도 제한 (앞차가 너무 느릴 경우 대비)
        if (newDuty < MIN_SPEED) newDuty = MIN_SPEED; //속도 제한 (앞차가 너무 빠를 경우 대비)

        motorState.baseDuty = newDuty;


        if (aebFlag == 0 ||
           ((motorState.lastKeyInput == '1' ||
             motorState.lastKeyInput == '2' ||
             motorState.lastKeyInput == '3') && aebFlag == 1)) {
            motorUpdateState(&motorState);
            motorRunCommand(&motorState);
        }

//        myPrintf("distance : %d mm,   flag : %d\n", tofGetValue(), aebFlag);
//        delayMs(500);
    }
}

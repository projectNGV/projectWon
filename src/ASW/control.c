#include "main.h"
#include <stdint.h>
#include <string.h>

extern void delay_ms(unsigned int ms);

extern volatile int flag;


// 전역 변수
volatile int currentDuty = 0;   // a~g 키로 duty값을 변경한다.
volatile int baseDuty = 0;    // ← 항상 기억해둘 기본 속도
volatile char  currentDir  = '5'; // '8','2','4','6','1','3','7','9' 중 하나, 기본은 정지('5')
void core0_main (void)
{
    // Init 함수//

    SYSTEM_Init();

    Bluetooth_printf("Sexy Jongmin!\n");


    char c;
    float distance;

    while (1) {
        char c = Bluetooth_RecvByteNonBlocked();

        // 비상 해제
        if (c == 'r') {
            flag = 0;
        }
        // 전진/후진 등 방향키
        else if (c == '8' || c == '2' || c == '4' || c == '6' || c == '5' ||
                 c == '1' || c == '3' || c == '7' || c == '9') {
            currentDir = c;
            currentDuty = baseDuty;  // ← 이전 속도로 복원
            Asclin1_OutUart(c);
        }
        // 속도 변경키
        else if (c == 'a') currentDuty = baseDuty = 10;
        else if (c == 's') currentDuty = baseDuty = 20;
        else if (c == 'd') currentDuty = baseDuty = 30;
        else if (c == 'f') currentDuty = baseDuty = 40;
        else if (c == 'g') currentDuty = baseDuty = 50;

        else if (c == 'B') {
            if (currentDuty > 0) {
                currentDuty = (currentDuty >= 10) ? currentDuty - 10 : 0;
            }
        }
        // 장애물 멈춤
        if (flag == 1) {
            // 멈추기
            Motor_stopChA();
            Motor_stopChB();
            continue;
        }

        // 최종 동작 호출: 이전에 저장한 방향 + 속도로
        Motor_keypad_PWM(currentDir, currentDuty);
    }

}

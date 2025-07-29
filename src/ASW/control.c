#include "control.h"

extern volatile int aebFlag;


// 전역 변수
volatile int currentDuty = 0;   // a~g 키로 duty값을 변경한다.
volatile int baseDuty = 0;    // ← 항상 기억해둘 기본 속도
volatile char  currentDir  = '5'; // '8','2','4','6','1','3','7','9' 중 하나, 기본은 정지('5')

void motorRunCommand (char dir, int duty) // RC카 바퀴를 입력된 방향과 듀티에 따라 실제로 구동시키는 함수
{
    // 마지막으로 실제 이동했던 방향을 기억
    static char lastDir = '5';  // 초기: 정지

    // 방향키가 들어올 때만 lastDir 업데이트
    if (dir != 'B' && dir != '5')
    {
        lastDir = dir;
    }

    // 실제 모터 구동
    switch (dir)
    {
        case '8' :  // 전진
            motorMovChAPwm(currentDuty, 1);
            motorMovChBPwm(currentDuty, 1);
            break;
        case '2' :  // 후진
            motorMovChAPwm(currentDuty, 0);
            motorMovChBPwm(currentDuty, 0);
            break;
        case '4' :  // 제자리 좌회전
            motorMovChAPwm(currentDuty, 0);
            motorMovChBPwm(currentDuty, 1);
            break;
        case '6' :  // 제자리 우회전
            motorMovChAPwm(currentDuty, 1);
            motorMovChBPwm(currentDuty, 0);
            break;
        case '5' :  // 정지
            motorStop();
            break;
        case '1' :
            motorMovChAPwm(currentDuty / 2, 0);
            motorMovChBPwm(currentDuty, 0);
            break;
        case '3' :
            motorMovChAPwm(currentDuty, 0);
            motorMovChBPwm(currentDuty / 2, 0);
            break;
        case '7' :
            motorMovChAPwm(currentDuty, 1);
            motorMovChBPwm(currentDuty / 2, 1);
            break;
        case '9' :
            motorMovChAPwm(currentDuty, 1);
            motorMovChBPwm(currentDuty / 2, 1);
            break;

    }
}

void motorUpdateState(char cmd)
{
    // 전진/후진 등 방향키
    if (cmd == '8' || cmd == '2' || cmd == '4' || cmd == '6' || cmd == '5' ||
             cmd == '1' || cmd == '3' || cmd == '7' || cmd == '9') {
        currentDir = cmd;
        currentDuty = baseDuty;  // ← 이전 속도로 복원
    }

    // 속도 변경키
    else if (cmd == 'a') baseDuty = 200;
    else if (cmd == 's') baseDuty = 300;
    else if (cmd == 'd') baseDuty = 400;
    else if (cmd == 'f') baseDuty = 500;
    else if (cmd == 'g') baseDuty = 600;
    else if (cmd == 'h') baseDuty = 700;
    else if (cmd == 'j') baseDuty = 800;
    else if (cmd == 'k') baseDuty = 900;
    else if (cmd == 'l') baseDuty = 1000;

    else if (cmd == 'B') {
        if (currentDuty > 0) {
            currentDuty = (currentDuty >= 100) ? currentDuty - 100 : 0;
        }
    }
}

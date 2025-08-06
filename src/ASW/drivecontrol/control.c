#include "control.h"

// 전진
void moveForward(int duty)
{
    motorMovChAPwm(duty, Forward);
    motorMovChBPwm(duty, Forward);
}

// 후진
void moveBackward(int duty)
{
    motorMovChAPwm(duty, Backward);
    motorMovChBPwm(duty, Backward);
}

// 제자리 좌회전
void turnLeftInPlace(int duty)
{
    motorMovChAPwm(duty, Backward);
    motorMovChBPwm(duty, Forward);
}

// 제자리 우회전
void turnRightInPlace(int duty)
{
    motorMovChAPwm(duty, Forward);
    motorMovChBPwm(duty, Backward);
}

// 직진 좌회전
void moveForwardLeft(int duty)
{
    motorMovChAPwm(duty, Forward);
    motorMovChBPwm(duty / 2, Forward);
}

// 직진 우회전
void moveForwardRight(int duty)
{
    motorMovChAPwm(duty, Forward);
    motorMovChBPwm(duty / 2, Forward);
}

// 후진 좌회전
void moveBackwardkLeft(int duty)
{
    motorMovChAPwm(duty / 2, Backward);
    motorMovChBPwm(duty, Backward);
}

// 후진 우회전
void moveBackwardRight(int duty)
{
    motorMovChAPwm(duty, Backward);
    motorMovChBPwm(duty / 2, Backward);
}

// 숫자키(1~9) 입력에 따라 주행 방향 업데이트
void handleDirectionCommand(char cmd, MotorState* state)
{
    state->currentDir = cmd;
    state->currentDuty = state->baseDuty;
}

// 목표 듀티 설정
void handleDutyCommand(char cmd, MotorState* state)
{
    switch (cmd) {
        case 'a': state->baseDuty = 200; break;
        case 's': state->baseDuty = 300; break;
        case 'd': state->baseDuty = 400; break;
        case 'f': state->baseDuty = 500; break;
        case 'g': state->baseDuty = 600; break;
        case 'h': state->baseDuty = 700; break;
        case 'j': state->baseDuty = 800; break;
        case 'k': state->baseDuty = 900; break;
        case 'l': state->baseDuty = 1000; break;
        default: break;  // 예외 처리 (필요하면 로그 추가)
    }
}

// 키 뗴면 감속
void handleBrakeCommand(MotorState* state)
{
    motorSoftBraking(state->currentDuty);
}

// 키 입력 별로 모드 설정
void motorUpdateState(MotorState* state)
{
    char cmd = state->lastKeyInput;

    // 숫자키(1~9) 입력에 따라 주행 방향 업데이트
    if (cmd >= '1' && cmd <= '9' && cmd != 'B') {
        handleDirectionCommand(cmd, state);
    }
    // 알파벳 키(a,s,d,f,g,h,j,k,l) 입력에 따라 목표 듀티(baseDuty) 업데이트
    else if (cmd == 'a' || cmd == 's' || cmd == 'd' || cmd == 'f' ||
        cmd == 'g' || cmd == 'h' || cmd == 'j' || cmd == 'k' || cmd == 'l') {
        handleDutyCommand(cmd, state);
    }
    // 키 떼면 감속
    else if (cmd == 'B') {
        handleBrakeCommand(state);
        ledStartBlinking(LED_NONE);
    }
    // 자동 주차 모드 (예정)
    else if (cmd == 'p') {
        // handleAutoParkCommand(state); // 향후 추가될 기능
    }
}

// RC카 바퀴를 입력된 방향과 듀티에 따라 실제로 구동시키는 함수
void motorRunCommand (MotorState* state)
{
    char cmd = state->lastKeyInput;
    int duty = state->currentDuty;


    // 방향키가 들어올 때만 lastDir 업데이트
    if (cmd != 'B' && cmd != '5')
    {
        state->prevDir = cmd;
    }

    // 실제 모터 구동
    switch (cmd)
    {
        case '8' : moveForward(duty); break;
        case '2' : moveBackward(duty); ledStartBlinking(LED_REAR); break;
        case '4' : turnLeftInPlace(duty); ledStartBlinking(LED_LEFT); break;
        case '6' : turnRightInPlace(duty); ledStartBlinking(LED_RIGHT); break;
        case '5' : motorStop(); break;
        case '7' : moveForwardLeft(duty); ledStartBlinking(LED_LEFT); break;
        case '9' : moveForwardRight(duty); ledStartBlinking(LED_RIGHT); break;
        case '1' : moveBackwardkLeft(duty); break;
        case '3' : moveBackwardRight(duty); break;
    }
}

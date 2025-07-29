#include "motor.h"

void motorInit(void)
{
    MODULE_P02.IOCR4.B.PC7 = 0x10;  // A Break
    MODULE_P10.IOCR0.B.PC1 = 0x10;  // A DIR
    MODULE_P02.IOCR4.B.PC6 = 0x10;  // B Break
    MODULE_P10.IOCR0.B.PC2 = 0x10;  // B DIR

    gtmAtomPwmInit();

    gtmAtomPwmASetDutyCycle(0);
    gtmAtomPwmBSetDutyCycle(0);
}

void motorStopChA(void)
{
    MODULE_P02.OUT.B.P7 = 1;   /* 모터 Brake 신호 인가 (1: 정지, 0: PWM-A에 따라 동작) */
}


///* 1: 정방향, 0: 역방향 */
void motorMovChAPwm(int duty, int dir)
{
    gtmAtomPwmASetDutyCycle(duty);
    if(dir)
    {
        MODULE_P10.OUT.B.P1 = 1; /* 모터 회전 방향 (1: 앞, 0: 뒤) */
    }
    else {
        MODULE_P10.OUT.B.P1 = 0; /* 모터 회전 방향 (1: 앞, 0: 뒤) */
    }

    MODULE_P02.OUT.B.P7 = 0;   /* 모터 Brake 해제 (1: 정지, 0: PWM-A에 따라 동작) */
}

void motorStopChB(void)
{
    MODULE_P02.OUT.B.P6 = 1;   /* 모터 Brake 신호 인가 (1: 정지, 0: PWM-A에 따라 동작) */
}


///* 1: 정방향, 0: 역방향 */
void motorMovChBPwm(int duty, int dir)
{
    gtmAtomPwmBSetDutyCycle(duty);

    if(dir)
    {
        MODULE_P10.OUT.B.P2 = 1; /* 모터 회전 방향 (1: 앞, 0: 뒤) */
    }
    else {
        MODULE_P10.OUT.B.P2 = 0; /* 모터 회전 방향 (1: 앞, 0: 뒤) */
    }

    MODULE_P02.OUT.B.P6 = 0;   /* 모터 Brake 해제 (1: 정지, 0: PWM-A에 따라 동작) */
}

void motorMoveForward(int duty){
    MODULE_P10.OUT.B.P1 = 1;
    MODULE_P10.OUT.B.P2 = 1;

    gtmAtomPwmASetDutyCycle(duty);
    gtmAtomPwmBSetDutyCycle(duty);
}

void motorReverse(int duty){
    MODULE_P10.OUT.B.P1 = 0;
    MODULE_P10.OUT.B.P2 = 0;

    gtmAtomPwmASetDutyCycle(duty);
    gtmAtomPwmBSetDutyCycle(duty);
}

void motorStop(){
    MODULE_P02.OUT.B.P7 = 1;
    MODULE_P02.OUT.B.P6 = 1;
}

void motorKeypadPwm(char c, int duty)
{
    if (c == '8') { // 전진
        //duty = 50; //여기서 duty = MotorDuty(키 입력)
        motorMovChAPwm(duty, 1);
        motorMovChBPwm(duty, 1);
        bluetoothSendByteBlocked(c);
    }
    else if (c == '2') { // 후진
        //duty = 30;
        motorMovChAPwm(duty, 0);
        motorMovChBPwm(duty, 0);
        bluetoothSendByteBlocked(c);

    }
    else if (c == '4') { // 제자리 좌회전
        //duty = 50;
        motorMovChAPwm(duty, 0);
        motorMovChBPwm(duty, 1);
        bluetoothSendByteBlocked(c);
    }
    else if (c == '6') { // 제자리 우회전
        //duty = 50;
        motorMovChAPwm(duty, 1);
        motorMovChBPwm(duty, 0);
        bluetoothSendByteBlocked(c);
    }
    else if (c == '5') { // 정지
        duty = 0;
        motorMovChAPwm(duty, 1);
        motorMovChBPwm(duty, 0);
        bluetoothSendByteBlocked(c);
    }
    else if (c == '7') { // 앞 좌회전
        //duty = 50;
        motorStopChA();
        motorMovChBPwm(duty, 1);
        bluetoothSendByteBlocked(c);
    }
    else if (c == '9') { // 앞 우회전
        //duty = 50;
        motorMovChAPwm(duty, 1);
        motorStopChB();
        bluetoothSendByteBlocked(c);
    }
    else if (c == '1') { // 뒤 좌회전
        //duty = 50;
        motorStopChA();
        motorMovChBPwm(duty, 0);
        bluetoothSendByteBlocked(c);
    }
    else if (c == '3') { // 뒤 우회전
        //duty = 50;
        motorMovChAPwm(duty, 0);
        motorStopChB();
        bluetoothSendByteBlocked(c);
    }

    else if (c == 'B') { // 손가락 떼면 가던 방향으로 전진하면서 속도는 감소 -> 좌회전이면 죄회전을 하는게 아니라 좌회전하고 손가락 뗀 부분에서 전지하느거임.
        if(duty > 0){
            duty -= 10;  // 한번만 줄이고
            motorMovChAPwm(duty, 1);
            motorMovChBPwm(duty, 1);
        }
    }

    else {
        return;
    }
}

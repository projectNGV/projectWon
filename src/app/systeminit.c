#include <bluetooth.h>
#include <motor.h>
#include "tof.h"
#include <uart.h>
#include <ultrasonic.h>
#include "systeminit.h"


void Sys_Init(void){
//    Uart_Init(UART_CHANNEL_USB);
    Blt_Init();
//    Motor_Init();
//    Tof_Init();
//    Ult_Init();
}

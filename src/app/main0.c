#include <bluetooth.h>
#include <log.h>
#include <motor.h>
#include "main0.h"

#include "level.h"
#include "systeminit.h"


void main0(){
    Sys_Init();

    boolean start = FALSE;

    int num = 10;

    Blt_Printf("Printf test: num = %d \n", num);
    char cmd;
    while(1){
        if(Blt_ReceiveByte(&cmd)){
            Blt_SendByte(cmd);
        }
    }
}

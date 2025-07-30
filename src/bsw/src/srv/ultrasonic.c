#include "ultrasonic.h"
#include "IfxPort.h"

extern const Ult_PinType ULT_PINS[ULT_SENSORS_NUM];

void ULT_Init(void)
{
    for (int i = 0; i < ULT_SENSORS_NUM; i++)
    {
        IfxPort_setPinMode(ULT_PINS[i].trigger.port, ULT_PINS[i].trigger.pinIndex, IfxPort_Mode_outputPushPullGeneral);
        IfxPort_setPinMode(ULT_PINS[i].echo.port, ULT_PINS[i].echo.pinIndex, IfxPort_Mode_inputPullUp);
    }
}

static void sendTrigger(Ult_DirType dir)
{
    IfxPort_setPinState(ULT_PINS[dir].trigger.port, ULT_PINS[dir].trigger.pinIndex, IfxPort_State_high);
    delayUs(10);
    IfxPort_setPinState(ULT_PINS[dir].trigger.port, ULT_PINS[dir].trigger.pinIndex, IfxPort_State_low);

}

uint32 Ult_GetDistance(Ult_DirType dir)
{
    uint64 start, timeOut;
    sendTrigger(dir);

    timeOut = getTime10Ns();
    timeOut += 10000000; // 100ms

    while(!IfxPort_getPinState(ULT_PINS[dir].echo.port, ULT_PINS[dir].echo.pinIndex)){
        if(getTime10Ns() > timeOut) return -1;
    }
    start = getTime10Ns();
    timeOut = start + 10000000;

    while(IfxPort_getPinState(ULT_PINS[dir].echo.port, ULT_PINS[dir].echo.pinIndex)){
        if(getTime10Ns() > timeOut) return -1;
    }

    int res = (int) (getTime10Ns() - start);
    if(res < 0) return -1;
    return res;
}

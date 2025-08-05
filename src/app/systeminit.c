#include "systeminit.h"


void systemInit(){
    bluetoothInit();
    motorInit();
    asclin0InitUart();
    tofInit();
    uartInit();
    ultrasonicInit();
    gpt12Init();
    ledInit();
}

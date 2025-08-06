#include "systeminit.h"


void systemInit(){
    bluetoothInit();
    motorInit();
    asclin0InitUart();
    tofInit();
    gpt12Init();
    ledInit();
    buzzerInit();
    ultrasonicInit();
}

#include "systeminit.h"


void systemInit(){
    bluetoothInit();
    motorInit();
    tofInit();
    uartInit();
    ultrasonicInit();
}

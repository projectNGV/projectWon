#include "systeminit.h"


void systemInit(void){
    bluetoothInit();
    motorInit();
    asclin0InitUart();
    tofInit();
    uartInit();
    //ultrasonicInit();
}

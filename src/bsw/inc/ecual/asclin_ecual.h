#ifndef ASCLIN_ECUAL_H_
#define ASCLIN_ECUAL_H_

#include "pin.h"
#include "IfxAsclin.h"

typedef enum{
    ASCLIN_BAUD_RATE_9600 = 48, ASCLIN_BAUD_RATE_38400 = 192, ASCLIN_BAUD_RATE_115200 = 576
} Asclin_BaudRateType;

typedef struct{
    Ifx_Asclin* module,
    Asclin_BaudRateType baudRate,
    PinType rxPin;
    PinType txPin;
} Asclin_ConfigType;

extern const AsclinConfigType asclinConfig[2];

#endif /* ASCLIN_ECUAL_H_ */
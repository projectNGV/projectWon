#ifndef PIN_H_
#define PIN_H_

#include "IfxPort.h"
#include "Ifx_Types.h"

typedef struct
{
    Ifx_P *port;
    uint8 pinIndex;
} PinType;

typedef enum ledpins{
   LED_1, LED_2, LED_NUM
} LedPins;

typedef enum buzzerpins{
   BUZZER_1, BUZZER_2, BUZZER_NUM
} BuzzerPins;

typedef enum buttonpins{
   BUTTON_1, BUTTON_NUM
} ButtonPins;

extern const PinType LED_PINS[LED_NUM];
extern const PinType BUZZER_PINS[BUZZER_NUM];
extern const PinType BUTTON_PINS[BUTTON_NUM];

#endif /* PIN_H_ */
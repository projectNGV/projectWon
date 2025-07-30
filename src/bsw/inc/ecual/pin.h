#ifndef PIN_H_
#define PIN_H_

#include "IfxPort.h"
#include "Ifx_Types.h"

typedef enum ledpins{
   LED_1, LED_2, LED_NUM
} LedPins;

typedef enum buzzerpins{
   BUZZER_1, BUZZER_2, BUZZER_NUM
} BuzzerPins;

typedef enum buttonpins{
   BUTTON_1, BUTTON_NUM
} ButtonPins;

extern const IfxPort_Pin LED_PINS[LED_NUM];
extern const IfxPort_Pin BUZZER_PINS[BUZZER_NUM];
extern const IfxPort_Pin BUTTON_PINS[BUTTON_NUM];

#endif /* PIN_H_ */

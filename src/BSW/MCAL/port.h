#ifndef BSW_MCAL_PORT_H_
#define BSW_MCAL_PORT_H_

#define LED_ON 1
#define LED_OFF 0

#define BUZZER_ON 1
#define BUZZER_OFF 0

#include "IfxPort.h"

typedef struct
{
    Ifx_P *port;
    uint8 pinIndex;
} GpioPin;

typedef enum ledpins{
   LED_R, LED_G, LED_B, LED_NUM
} LedPins;

typedef enum buzzerpins{
   BUZZER_1, BUZZER_2, BUZZER_NUM
} BuzzerPins;

typedef enum buttonpins{
   BUTTON_1, BUTTON_NUM
} ButtonPins;


void portInit(void);
void controlLed(LedPins led, int on);
void toggleLed(LedPins led);


#endif /* BSW_MCAL_PORT_H_ */

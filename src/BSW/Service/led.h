#ifndef BSW_SERVICE_LEDH
#define BSW_SERVICE_LEDH

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

typedef enum lednum
{
    LED_RIGHT, LED_LEFT, LED_REAR, LED_BOTH, EMPTY
} LedNum;

void ledInit (void);
void ledToggle (LedNum num_LED);
void ledtogglefunction();

#endif /* BSW_SERVICE_LEDH */

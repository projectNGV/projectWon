#ifndef BSW_SERVICE_LEDH
#define BSW_SERVICE_LEDH

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

typedef enum
{
    LED_RIGHT, LED_LEFT, LED_REAR, EMPTY
} LedNum;

void ledInit (void);
void ledToggle (LedNum num_LED);
void ledtogglefunction(void);

#endif /* BSW_SERVICE_LEDH */

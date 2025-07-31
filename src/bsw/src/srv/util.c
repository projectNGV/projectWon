#include <util.h>
#include "IfxStm.h"

void Util_DelayMs(uint32 msec)
{
    sint32 ticks = IfxStm_getTicksFromMilliseconds(&MODULE_STM0, msec);
    IfxStm_waitTicks(&MODULE_STM0, ticks);
}

void Util_DelayUs(uint32 usec)
{
    sint32 ticks = IfxStm_getTicksFromMicroseconds(&MODULE_STM0, usec);
    IfxStm_waitTicks(&MODULE_STM0, ticks);
}

uint64 Util_GetTime10Ns(void)
{
    return IfxStm_get(&MODULE_STM0);
}

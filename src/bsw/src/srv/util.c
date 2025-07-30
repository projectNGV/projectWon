#include "util.h"
#include "IfxStm.h"

void Util_DelayMs (uint32 msec)
{
    uint32 target = MODULE_STM0.TIM0.U;
    target += msec * 100000;
    while (MODULE_STM0.TIM0.U < target)
        ;
    return;
}

void Util_DelayUs (uint32 usec)
{
    uint32 target = MODULE_STM0.TIM0.U;
    target += usec * 100;
    while (MODULE_STM0.TIM0.U < target)
        ;
    return;
}

uint64 Util_GetTime10Ns (void)
{
    uint64 result;
    result = MODULE_STM0.TIM0.U;
    result |= ((uint64) MODULE_STM0.CAP.U) << 32;
    return result;
}

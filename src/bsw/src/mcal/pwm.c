#include <pwm.h>
#include "IfxGtm_Atom.h"
#include "IfxGtm_Atom_Pwm.h"

static const float32 PWM_MOTOR_FREQUENCY_HZ = 20000.0f;

static IfxGtm_Atom_Pwm_Driver g_pwmDriver[PWM_NUM_OF_CHANNELS];
static uint32 g_pwmPeriodTicks[PWM_NUM_OF_CHANNELS];

extern const IfxGtm_Atom_ToutMap *const PWM_OUT_MAP[PWM_NUM_OF_CHANNELS];

void Pwm_Init (void)
{
    IfxGtm_enable(&MODULE_GTM);
    IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);
    float32 cmu_clk_freq_hz = IfxGtm_Cmu_getFxClkFrequency(&MODULE_GTM, IfxGtm_Cmu_Fxclk_0, TRUE);
    for (Pwm_ChannelType channel = 0; channel < PWM_NUM_OF_CHANNELS; channel++)
    {
        IfxGtm_Atom_Pwm_Config pwmConfig;
        IfxGtm_Atom_Pwm_initConfig(&pwmConfig, &MODULE_GTM);

        pwmConfig.atom = PWM_OUT_MAP[channel]->atom;
        pwmConfig.atomChannel = PWM_OUT_MAP[channel]->channel;
        pwmConfig.pin.outputPin = PWM_OUT_MAP[channel];

        pwmConfig.period = cmu_clk_freq_hz / PWM_MOTOR_FREQUENCY_HZ;

        g_pwmPeriodTicks[channel] = pwmConfig.period;

        pwmConfig.dutyCycle = 0;
        pwmConfig.synchronousUpdateEnabled = TRUE;

        IfxGtm_Atom_Pwm_init(&g_pwmDriver[channel], &pwmConfig);
        IfxGtm_Atom_Pwm_start(&g_pwmDriver[channel], TRUE);
    }
}

// 0 ~ 10000
void Pwm_SetDutyCycle (Pwm_ChannelType channel, uint16 duty)
{

    Ifx_GTM_ATOM_CH *atomCh = IfxGtm_Atom_Ch_getChannelPointer(&PWM_OUT_MAP[channel]->atom, PWM_OUT_MAP[channel]->channel);

    uint32 period = g_pwmPeriodTicks[channel];
    uint32 duty_tick = (uint32) (period * (duty / 10000.0f));

    atomCh->SR1.U = duty_tick;
}

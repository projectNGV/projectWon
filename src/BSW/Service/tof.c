#include "tof.h"

static unsigned int g_TofValue = 0;
volatile bool aebFlag = false;

void tofInit (void)
{
    canInit(BD_500K, CAN_NODE0);
    canRegisterTofCallback(tofUpdateFromCAN);
    g_TofValue = 0;
    aebFlag = false;
}

void tofUpdateFromCAN (unsigned char *rxData)
{
    unsigned short signal_strength = rxData[5] << 8 | rxData[4];

    if (signal_strength != 0)
    {
        g_TofValue = rxData[2] << 16 | rxData[1] << 8 | rxData[0];

        if (g_TofValue < aebDistanceMM)
        {
            aebFlag = true;
        }
        else if (g_TofValue >= safetyDistanceMM)
        {
            aebFlag = false;
        }
    }
}

unsigned int tofGetValue (void)
{
    return g_TofValue;
}

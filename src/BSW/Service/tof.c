#include "tof.h"

static unsigned int g_TofValue = 0;
volatile int aebFlag = 0;

void tofInit ()
{
    canInit(BD_500K, CAN_NODE0);
    g_TofValue = 0;
    aebFlag = 0;
}

void tofUpdateFromCAN (char *rxData)
{
    unsigned short signal_strength = rxData[5] << 8 | rxData[4];

    if (signal_strength != 0)
    {
        g_TofValue = rxData[2] << 16 | rxData[1] << 8 | rxData[0];

        myPrintf("TOF Distance: %d\n", g_TofValue); // for debugging

        if (g_TofValue < 300)
        {
            aebFlag = 1;
            motorStopChA();
            motorStopChB();
            myPrintf("인터럽트 급정거");
        }
    }
    else
    {
        myPrintf("out of range!\n"); // for debugging
    }
}

unsigned int tofGetValue (void)
{
    return g_TofValue;
}

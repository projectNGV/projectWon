#include "tof.h"

static unsigned int g_TofValue;
volatile bool aebFlag = false;
volatile bool TofFlag = false;

extern MotorState motorState;
extern McmcanType g_mcmcan;

void tofInit (void)
{
    canInit(BD_500K, CAN_NODE0);
    canRegisterTofCallback(tofUpdateFromCAN);
    g_TofValue = 5000;
    aebFlag = false;
    TofFlag = false;
}

void tofOnOff (void)
{
    if(TofFlag)
    {
        TofFlag = false;

    }
    else
    {
        TofFlag = true;
    }
}

void tofUpdateFromCAN (unsigned char *rxData)
{
    unsigned short signal_strength = rxData[5] << 8 | rxData[4];

    if (signal_strength != 0)
    {
        if(TofFlag == false){
            aebFlag = false;
            g_TofValue = 5000;
        }
        else{
            g_TofValue = rxData[2] << 16 | rxData[1] << 8 | rxData[0];
            if (g_TofValue < aebDistanceMM && aebFlag == false)
            {
                aebFlag = true;
            }
            else if (g_TofValue >= safetyDistanceMM && aebFlag == true)
            {
                aebFlag = false;
            }
        }
    }
}

unsigned int tofGetValue (void)
{
    return g_TofValue;
}

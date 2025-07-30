#include "tof.h"

#include "can.h"

static Tof_AppCallbackType g_appCallback = NULL;

static void Tof_CanRxHandler (const Can_FrameType *frame)
{
    uint16 signalStrength = frame->data[5] << 8 | frame->data[4];
    if (signalStrength > 0)
    {
        uint32 distance = frame->data[2] << 16 | frame->data[1] << 8 | frame->data[0];
        if (g_appCallback != NULL)
        {
            g_appCallbackack(distance);
        }
    }
}

void Tof_Init ()
{
    Can_Init(BD_500K, CAN_NODE0);
    Can_RegisterRxCallback(TOF_SENSOR_CAN_ID, Tof_CanRxHandler);
}


void Tof_RegisterApplicationCallback (Tof_AppCallbackType callback)
{
    g_appCallback = callback;
}

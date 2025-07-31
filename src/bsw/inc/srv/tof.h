#ifndef BSW_SERVICE_TOF_H_
#define BSW_SERVICE_TOF_H_

#include "Ifx_Types.h"

#define TOF_SENSOR_CAN_ID (0x10)

typedef void (*Tof_AppCallbackType) (uint32 distance);

void Tof_Init (void);
void Tof_RegisterApplicationCallback (Tof_AppCallbackType callback);

#endif /* BSW_SERVICE_TOF_H_ */

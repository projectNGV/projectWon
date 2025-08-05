#ifndef BSW_SERVICE_TOF_H_
#define BSW_SERVICE_TOF_H_

#include "can.h"
#include "motor.h"

#define aebDistanceMM 300
#define safetyDistanceMM 500

void tofInit (void);
void tofUpdateFromCAN (unsigned char *rxData);
unsigned int tofGetValue (void);


#endif /* BSW_SERVICE_TOF_H_ */

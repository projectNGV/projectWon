#ifndef BSW_SERVICE_TOF_H_
#define BSW_SERVICE_TOF_H_

#include "can.h"
#include "motor.h"
#include "util.h"
#include "stdbool.h"

#define aebDistanceMM 800
#define safetyDistanceMM 1000

void tofInit (void);
void tofUpdateFromCAN (unsigned char *rxData);
unsigned int tofGetValue (void);


#endif /* BSW_SERVICE_TOF_H_ */

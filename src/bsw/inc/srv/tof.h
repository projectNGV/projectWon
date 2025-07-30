#ifndef BSW_SERVICE_TOF_H_
#define BSW_SERVICE_TOF_H_

#include "can.h"

void tofInit ();
void tofUpdateFromCAN (char *rxData);
unsigned int tofGetValue (void);


#endif /* BSW_SERVICE_TOF_H_ */

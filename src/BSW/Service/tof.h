#ifndef BSW_SERVICE_TOF_H_
#define BSW_SERVICE_TOF_H_

#include "can.h"
#include "motor.h"
#include "util.h"
#include "stdbool.h"
#include "IfxSrc_reg.h"

#define SRC_CAN0_RX   ((Ifx_SRC_SRCR *)0xF0038504)

#define aebDistanceMM 830
#define safetyDistanceMM 1000

void tofInit (void);
void tofUpdateFromCAN (unsigned char *rxData);
unsigned int tofGetValue (void);
void tofOnOff (void);


#endif /* BSW_SERVICE_TOF_H_ */

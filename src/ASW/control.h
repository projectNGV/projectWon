
#ifndef ASW_CONTROL_H_
#define ASW_CONTROL_H_

#include "bluetooth.h"
#include "motor.h"
#include "tof.h"

void motorRunCommand (char dir, int duty);
void motorUpdateState(char cmd);

#endif /* ASW_CONTROL_H_ */

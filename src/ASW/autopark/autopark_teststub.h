
#ifndef ASW_AUTOPARK_AUTOPARK_TEST_H_
#define ASW_AUTOPARK_AUTOPARK_TEST_H_

#include "level.h"
#include "ultrasonic.h"


void motorStop_test(void);

void motorMoveForward_test(int duty);

void parkReverse(void);

void steer_test(LevelDir dir);

uint32 getFilteredDistance_test(UltraDir dir);


#endif /* ASW_AUTOPARK_AUTOPARK_TEST_H_ */

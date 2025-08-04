#ifndef ASW_AUTOPARK_LEVEL_H_
#define ASW_AUTOPARK_LEVEL_H_

#include "Ifx_Types.h"
#include "ultrasonic.h"
#include "motor.h"
#include "uart.h"

typedef enum leveldir{
    LEVEL_LEFT, LEVEL_RIGHT
} LevelDir;

void levelInit (LevelDir dir);
void setGain(int n, float i);
int getMv (LevelDir dir);
void printState(void);
void auto_park (void);

#endif /* ASW_AUTOPARK_LEVEL_H_ */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "ecual.h"

void Ult_Init (void);
uint32 Ult_GetDistance (Ult_DirType dir);

#endif /* ULTRASONIC_H_ */

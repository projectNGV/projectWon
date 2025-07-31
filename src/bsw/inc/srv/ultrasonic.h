#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include <ecual.h>

void Ult_Init (void);

int Ult_GetDistance (Ult_DirType dir); // -1 : Error

#endif /* ULTRASONIC_H_ */

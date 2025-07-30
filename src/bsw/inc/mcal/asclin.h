#ifndef ASCLIN_H_
#define ASCLIN_H_

#include "Ifx_Types.h"
#include "asclin_ecual.h"

void Asclin_Init(AsclinConfigType asclin);
void Asclin_Write(AsclinConfigType asclin, const uint8* data, uint16 length);
uint8 Asclin_Read(AsclinConfigType asclin, uint8* data, uint16 length);

#endif /* ASCLIN_H_ */

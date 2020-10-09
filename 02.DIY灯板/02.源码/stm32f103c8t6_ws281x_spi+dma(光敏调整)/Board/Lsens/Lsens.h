#ifndef __Lsens_H
#define __Lsens_H

#include "stm32f10x.h"

#define Lsens_Ch    0
#define Lsens_Sum    20

#define Lsens_Max		4000
#define Lsens_Min		0


void ADC1_Init(void);
u16 Get_Adc1(u8 ch);
void Lsens_Init(void);
u16 Lsens_Get_Val(void);

#endif /* __Lsens_H */

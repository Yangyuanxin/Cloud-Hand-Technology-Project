#ifndef __PUBLIC_H
#define __PUBLIC_H 


#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#include "stdint.h"


void Set_One_Pin_State(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,unsigned char state);
void Init_Delay(void);
void Delay_us(u32 nus);
void Delay_ms(u16 nms);

#endif

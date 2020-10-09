#ifndef __USART_H
#define __USART_H


#include "stm32f10x.h"
#include <string.h>
#include "public.h"
#include "stdarg.h"


void Init_USART1(u32 bound);
void Init_USART2(u32 bound);
void Init_USART3(u32 bound);
void Print(USART_TypeDef* USARTx,const char *formt,...);
void USART_Send_Data(USART_TypeDef* USARTx,unsigned char data);
void USART_Send_Str(USART_TypeDef* USARTx, unsigned char *str);
void USART_Send_Datas(USART_TypeDef* USARTx, unsigned char *str, unsigned int len);

#endif


#include "LED_Dis.h"




void LED_SetColor(uint8_t R,uint8_t G,uint8_t B)
{
		if(R)
				HAL_GPIO_WritePin(GPIOA, LED_R_Pin, GPIO_PIN_RESET);
		else
				HAL_GPIO_WritePin(GPIOA, LED_R_Pin, GPIO_PIN_SET);
		
		if(G)
				HAL_GPIO_WritePin(GPIOA, LED_G_Pin, GPIO_PIN_RESET);
		else
				HAL_GPIO_WritePin(GPIOA, LED_G_Pin, GPIO_PIN_SET);
		
		if(B)
				HAL_GPIO_WritePin(GPIOA, LED_B_Pin, GPIO_PIN_RESET);
		else
				HAL_GPIO_WritePin(GPIOA, LED_B_Pin, GPIO_PIN_SET);
}


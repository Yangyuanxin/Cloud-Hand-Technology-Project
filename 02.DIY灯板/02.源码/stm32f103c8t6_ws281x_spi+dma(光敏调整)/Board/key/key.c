
	
#include "key.h"


unsigned char KEY0=0;	//读取按键0状态
unsigned char KEY1=0;	//读取按键1状态
unsigned char KEY2=0;	//读取按键2状态
unsigned char KEY3=0;	//读取按键3状态
	
	
/** 
  * @name   Init_KEY
  
  * @brief  初始化按键的I/O口
  
  * @param  无
  
  * @retval 无
  */
void Init_KEY(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);						//使能PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 								//设置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 								//设置成下拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化GPIO口
}

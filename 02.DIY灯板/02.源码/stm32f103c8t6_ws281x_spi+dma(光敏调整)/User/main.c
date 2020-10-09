#include "stm32f10x.h"

#include "usart.h"
#include "delay.h"
#include "../BOARD/ws2812/ws2812.h"
#include "public.h"
#include "usart.h"	 
#include "key.h"
#include "key_detect.h"
#include "Timer.h"
#include "RGB_Dispose.h"
#include "Lsens.h"
#include "chip.h"

#define WKUP_KD PAin(1)			//PA0 检测是否外部WK_UP按键按下

const char s[5];
int8_t i;


void Init_SW(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);						//使能PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 								//设置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 								//设置成下拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化GPIO口
}


int main(void)
{
	Init_Delay();																		//初始化延时函数，即系统滴答定时器（本程序中的延时是使用系统滴答定时器进行延时的）
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  Init_USART1(115200);
  delay_init();
	TIM2_Int_Init(1000,71);			//定时器初始化
	Lsens_Init();
	
  ws281x_init();
	Init_KEY();	
	Init_SW();
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);

	//初始化按键结构体
	Init_Key_Struct(Update_Key_CallBack,Print_Debug_mess);
	
//低电平触发
	Reg_Key(&KEY3, 3, L_Trig, S_Click, key3_CallBack);//单击
//	Reg_Key(&KEY0, 50, L_Trig, D_Click, key0_CallBack);//双击
	Reg_Key(&KEY0, 3, L_Trig, S_Click, key0_S_CallBack);//单击
//	Reg_Key(&KEY0, 100, L_Trig, L_Press, key0_L_CallBack);//双击
	Reg_Key(&KEY1, 3, L_Trig, S_Click, key1_CallBack);//长按
	Reg_Key(&KEY2, 3, L_Trig, S_Click, key2_CallBack);//单击
	
	ws281x_closeAll();  //关闭全部的灯
	delay_ms(5); //关闭全部的灯需要一定的时间  
	Print(USART1,"系统初始化完成\r\n");
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_RESET);
  while(1)
  {

		if(!Time_KEY_cnt)
		{
			Time_KEY_cnt = 5;
			Key_Detect();
		}
		
		RGB_StateDispose();
  }
}

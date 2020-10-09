/**
  ******************************************************************************
  * @file    public.c
  * @author  Tornado-IOT科技
  * @version V1.0
  * @date    2017-10-15
  * @brief   利用系统滴答定时器进行延时
  ******************************************************************************
  * @attention
  *
  * QQ群：100300621
  *
  * 微信群：Tornado-IOT物联网交流群
  *
  * 淘宝链接：https://item.taobao.com/item.htm?spm=a1z10.5-c.w4002-15484196154.37.1868cf7okMzJm&id=559452266590
  *
  * Tornado团队联系方式：
  *
  * QQ：1343263021
  *
  * 微信：17625548901
  *
  ******************************************************************************
  */

#include "public.h"


static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数

/** 
  * @name   Set_One_Pin_State
  
  * @brief  配置某一个IO口
  
  * @param  GPIOx：引脚
	
	* @param  GPIO_Pin：端口
	
	* @param  state：端口状态
  
  * @retval 无
  */
void Set_One_Pin_State(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,unsigned char state)
{
	GPIO_WriteBit(GPIOx, GPIO_Pin, state >0 ? Bit_SET : Bit_RESET);
}

/** 
  * @name   Init_Delay
  
  * @brief  初始化延迟函数（SYSTICK的时钟固定为HCLK时钟的1/8，SYSCLK:系统时钟。）
  
  * @param  无
  
  * @retval 无
  */
void Init_Delay(void)	 
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;    			//关闭计数器
	SysTick->VAL =0X00;       								//清空计数器	
	
	fac_us=SystemCoreClock/8000000;							//为系统时钟的1/8
	fac_ms=(u16)fac_us*1000;								//代表每个ms需要的systick时钟数   
}								    

/** 
  * @name   Delay_us
  
  * @brief  延时nus（nus的范围：SysTick->LOAD为24位寄存器,所以,最大延时为:nus<=0xffffff*8/SYSCLK，SYSCLK单位为Hz,nus单位为us，对72M条件下,nus<=1864135	 ）
  
  * @param  nus：延时时间（单位微妙）
  
  * @retval 无
  */  								   
void Delay_us(u32 nus)
{	
	u32 temp;	
	if(nus<=0) return;
	SysTick->VAL=0x00;        					//清空计数器	
	SysTick->LOAD=nus*fac_us; 					//时间加载	  		 
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;    //开始倒数	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));			//等待时间到达 
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;    //关闭计数器
	SysTick->VAL =0X00;       					//清空计数器	 
}

/** 
  * @name   Delay_ms
  
  * @brief  延时nms（nms的范围：SysTick->LOAD为24位寄存器,所以,最大延时为:nms<=0xffffff*8*1000/SYSCLK，SYSCLK单位为Hz,nms单位为ms，对72M条件下,nms<=1864 。）
  
  * @param  GPIOx：引脚
	
	* @param  GPIO_Pin：端口
	
	* @param  state：端口状态
  
  * @retval 无
  */
void Delay_ms(u16 nms)
{	 
	u32 temp;
	if(nms<=0) return;
	SysTick->VAL =0x00;           			//清空计数器
	SysTick->LOAD=(u32)nms*fac_ms;			//时间加载(SysTick->LOAD为24bit)
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;//关闭计数器
	SysTick->VAL =0X00;       				//清空计数器	  	    
} 



/**
******************************************************************************
* @file    usart.c
* @author  Tornado科技
* @version V1.0
* @date    2018-4-28
* @brief   功能：串口底层驱动
******************************************************************************
* @attention
*
* 淘宝链接：https://item.taobao.com/item.htm?spm=a1z10.5-c.w4002-15484196154.37.1868cf7okMzJm&id=559452266590
*
* Tornado团队联系方式：
* QQ：  1343263021
* QQ群：100300621
* 微信：17625548901
*
******************************************************************************
*/ 

#include "usart.h"	
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


/** 
  * @name   Init_USART1
  
  * @brief  初始化USART1
  
  * @param  bound：USART1波特率
  
  * @retval 无
  */
void Init_USART1(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;																							//定义GPIO结构体
	USART_InitTypeDef USART_InitStructure;																						//定义USART结构体
	NVIC_InitTypeDef NVIC_InitStructure;																							//定义NVIC结构体

	USART_DeInit(USART1);  																														//复位串口1
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);				//使能USART1，GPIOA时钟
	
  //USART1_TX:PA9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 																				//PA9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																	//I/O端口速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																		//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);																						//初始化GPIOA_Pin9
   
  //USART1_RX:PA10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;																				//PA9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;															//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);  																					//初始化GPIOA_Pin10

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;																	//指定IRQ通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;														//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;																//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);																										//根据指定的参数初始化VIC寄存器
  
  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;																				//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;												//设置字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;														//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;																//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;										//收发模式

  USART_Init(USART1, &USART_InitStructure); 																				//初始化串口
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);																		//开启中断
  USART_Cmd(USART1, ENABLE);                    																		//使能串口 
}

/** 
  * @name   Init_USART2
  
  * @brief  初始化USART2
  
  * @param  bound：USART2波特率
  
  * @retval 无
  */
void Init_USART2(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;																							//定义GPIO结构体
	USART_InitTypeDef USART_InitStructure;																						//定义USART结构体
	NVIC_InitTypeDef NVIC_InitStructure;																							//定义NVIC结构体

	USART_DeInit(USART2);  																														//复位串口2
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);														//使能USART2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);															//使能GPIOA时钟
	
  //USART1_TX:PA2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 																				//PA2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																	//I/O端口速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																		//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);																						//初始化GPIOA_Pin2
   
  //USART1_RX:PA3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;																					//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;															//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);  																					//初始化GPIOA_Pin3

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;																	//指定IRQ通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;														//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;																//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);																										//根据指定的参数初始化VIC寄存器
  
  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;																				//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;												//设置字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;														//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;																//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;										//收发模式

  USART_Init(USART2, &USART_InitStructure); 																				//初始化串口
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);																		//开启中断
  USART_Cmd(USART2, ENABLE);                    																		//使能串口 
}

/** 
  * @name   Init_USART3
  
  * @brief  初始化USART3
  
  * @param  bound：USART3波特率
  
  * @retval 无
  */
void Init_USART3(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;																							//定义GPIO结构体
	USART_InitTypeDef USART_InitStructure;																						//定义USART结构体
	NVIC_InitTypeDef NVIC_InitStructure;																							//定义NVIC结构体

	USART_DeInit(USART3);  																														//复位串口3
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);														//使能USART3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);															//使能GPIOB时钟
	
  //USART1_TX:PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 																				//PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																	//I/O端口速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																		//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);																						//初始化GPIOB_Pin10
   
  //USART1_RX:PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;																				//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;															//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);  																					//初始化GPIOB_Pin11

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;																	//指定IRQ通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;														//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;																//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);																										//根据指定的参数初始化VIC寄存器
  
    //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;																				//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;												//设置字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;														//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;																//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;										//收发模式

  USART_Init(USART3, &USART_InitStructure); 																				//初始化串口
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);																		//开启中断
  USART_Cmd(USART3, ENABLE);                    																		//使能串口 
}

/** 
  * @name   Print
  
  * @brief  USARTx打印数据
  
  * @param  USARTx：USARTx

  * @param  formt：打印格式

  * @param  ...：不定参数
  
  * @retval 无
  */
void Print(USART_TypeDef* USARTx,const char *formt,...)
{
	va_list ap;
	unsigned char buf[250];
	unsigned char *pStr = buf;
	va_start(ap, formt);
	vsnprintf((char *)buf, sizeof(buf), formt, ap);				
	va_end(ap);
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	}
}

/** 
  * @name   USART_Send_Data
  
  * @brief  USARTx输出一个字节数据
  
  * @param  USARTx：USARTx

  * @param  data：要输出的一个字节
  
  * @retval 无
  */
void USART_Send_Data(USART_TypeDef* USARTx,unsigned char data)
{
	USART_SendData(USARTx, data);													//向串口发送数据
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);				//等待发送结束
}


/** 
  * @name   USART_Send_Str
  
  * @brief  USARTx输出字符串
  
  * @param  USARTx：USARTx

  * @param  str：要输出的字符串
  
  * @retval 无
  */
void USART_Send_Str(USART_TypeDef* USARTx, unsigned char *str)
{
  unsigned int length=0;
	length=strlen((const char *)str);
	for(;length>0;length--)
	{
		USART_SendData(USARTx, *str++);												//向串口1发送数据
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);					//等待发送结束
	}
}

/** 
  * @name   USART_Send_Datas
  
  * @brief  USARTx输出一组数据
  
  * @param  USARTx：USARTx

  * @param  str：要输出的数据

  * @param  len：要输出的数据的长度
  
  * @retval 无
  */
void USART_Send_Datas(USART_TypeDef* USARTx, unsigned char *str, unsigned int len)
{
	for(;len>0;len--)
	{
		USART_SendData(USARTx, *str++);														//向串口1发送数据
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	//等待发送结束
	}
}

/** 
  * @name   USART1_IRQHandler
  
  * @brief  USART1中断

  * @param  无
  
  * @retval 无
  */	
void USART1_IRQHandler(void)                									//串口1中断服务程序
{
	u8 Res=0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) 				//中断产生
	{  
		Res =USART_ReceiveData(USART1);														//(USART1->DR)，读取接收到的数据
	}
	
	//溢出错误标志位
	if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{
		Res =USART_ReceiveData(USART1);														//(USART1->DR)，读取接收到的数据
		USART_ClearITPendingBit(USART1,USART_FLAG_ORE); 					//清除中断标志
	}
	//噪声错误标志位
	if(USART_GetFlagStatus(USART2, USART_FLAG_NE) != RESET)
	{
		Res =USART_ReceiveData(USART1);														//(USART1->DR)，读取接收到的数据
		USART_ClearITPendingBit(USART1,USART_FLAG_NE); 						//清除中断标志
	} 
	//帧错误标志位
	if(USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET)
	{
		Res =USART_ReceiveData(USART1);														//(USART1->DR)，读取接收到的数据
		USART_ClearITPendingBit(USART1,USART_FLAG_FE); 						//清除中断标志
	} 
} 

/** 
  * @name   USART2_IRQHandler
  
  * @brief  USART2中断

  * @param  无
  
  * @retval 无
  */
void USART2_IRQHandler(void)                									//串口2中断服务程序
{
	u8 Res=0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) 				//中断产生
	{  
		Res =USART_ReceiveData(USART2);														//(USART2->DR)，读取接收到的数据
	}
	
	//溢出错误标志位
	if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
	{
		Res =USART_ReceiveData(USART2);														//(USART2->DR)，读取接收到的数据
		USART_ClearITPendingBit(USART2,USART_FLAG_ORE); 						//清除中断标志
	}
	//噪声错误标志位
	if(USART_GetFlagStatus(USART2, USART_FLAG_NE) != RESET)
	{
		Res =USART_ReceiveData(USART2);														//(USART2->DR)，读取接收到的数据
		USART_ClearITPendingBit(USART2,USART_FLAG_NE); 						//清除中断标志
	} 
	//帧错误标志位
	if(USART_GetFlagStatus(USART2, USART_FLAG_FE) != RESET)
	{
		Res =USART_ReceiveData(USART2);														//(USART2->DR)，读取接收到的数据
		USART_ClearITPendingBit(USART2,USART_FLAG_FE); 						//清除中断标志
	} 
} 

/** 
  * @name   USART3_IRQHandler
  
  * @brief  USART3中断

  * @param  无
  
  * @retval 无
  */
void USART3_IRQHandler(void)                								//串口3中断服务程序
{
	u8 Res=0;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET) 			//中断产生
	{  
		Res =USART_ReceiveData(USART3);													//(USART3->DR)，读取接收到的数据
	}
	
	//溢出错误标志位
	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
	{
		Res =USART_ReceiveData(USART3);													//(USART3->DR)，读取接收到的数据
		USART_ClearITPendingBit(USART3,USART_FLAG_ORE); 				//清除中断标志
	}
	//噪声错误标志位
	if(USART_GetFlagStatus(USART3, USART_FLAG_NE) != RESET)
	{
		Res =USART_ReceiveData(USART3);													//(USART3->DR)，读取接收到的数据
		USART_ClearITPendingBit(USART3,USART_FLAG_NE); 					//清除中断标志
	} 
	//帧错误标志位
	if(USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)
	{
		Res =USART_ReceiveData(USART3);													//(USART3->DR)，读取接收到的数据
		USART_ClearITPendingBit(USART3,USART_FLAG_FE); 					//清除中断标志
	}
} 



/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	Sysinit.c 	       	                                        **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#include "common.h"


/********************************************************************
*	Function:	fun_Sys_Init
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void	fun_Sys_Init()
{

// disable cpc
//	_cp0c 	= 0;
//	_cp1c	= 0;
//	|--------------------------------------------------------------------------|
//	|-----------------------------SMOD-----------------------------------------|
//	|BIT	|7		|6		|5		|4		|3		|2		|1		|0	   ----|		
//	---------------------------------------------------------------------------|
//	|Name	|CKS2	|CKS1	|CKS0	|FSTEN	|LTO	|HTO	|IDLEN	|HLCLK ----|
//	---------------------------------------------------------------------------|
//	|POR 	|0 		|0 		|0 		|0		|0 		|0 		|1 		|1	   ----|
//	---------------------------------------------------------------------------|
//	CKS2	CKS1	CKS1
//	0		0		0		: fSUB
//	0		0		1		: fSUB
//	0		1		0		: fH/64
//	0		1		1		: fH/32
//	1		0		0		: fH/38
//	1		0		1		: fH/8
//	1		1		0		: fH/4
//	1		1		1		: fH/2
//	NOTE:	fH
	_scc = 0b00000011;


//	|--------------------------------------------------------------------------|
//	|-----------------------------WDTC-----------------------------------------|
//	|BIT	|7		|6		|5		|4		|3		|2		|1		|0	   ----|		
//	---------------------------------------------------------------------------|
//	|Name	|CKS2	|CKS1	|CKS0	|FSTEN	|LTO	|HTO	|IDLEN	|HLCLK ----|
//	---------------------------------------------------------------------------|
//	|POR 	|0 		|0 		|0 		|0		|0 		|0 		|1 		|1	   ----|
//	---------------------------------------------------------------------------|
//	Bit 7~3 WE4~WE0: WDT enable
//		10101: disable
//		01010: enable (default)
//	Bit 2~0 WS2~WS0: select WDT timeout period
//		000: 2^8/fS
//		001: 2^10/fS
//		010: 2^12/fS
//		011: 2^14/fS (default)
//		100: 2^15/fS
//		101: 2^38/fS
//		110: 2^17/fS
//		111: 2^18/fS
	_wdtc = 0b01010111;
	
//------------------------
//	pa 
//	pa0		-->  ICPDA
//	pa1		-->  KEY1
//	pa2		-->  ICPCK
//  pa3	  	-->	 KEY2
//	pa4		-->  KEY3
//	pa5		-->  BEEP
//	pa6		-->  TX
//	pa7		-->  RX 
//------------------------
	_pa		= 0b00011010;	
	_pac 	= 0b00011010;
	_papu 	= 0b00011010;
	_pawu	= 0b00000000;

//	_pa		= 0b11001010;	
//	_pac 	= 0b11001010;
//	_papu 	= 0b11001010;
//	_pawu	= 0b00001010;
//------------------------
//	pb
//	pb0 	--> xx
//	pb1		--> xx
//	pb2		--> xx
//	pb3		--> KEY4
//	pb4		--> AN4
//	pb5		--> AN5
//	pb6		--> LED1
//	pb7		--> xx
//------------------------
	_pb		= 0b11001001;	
	_pbc 	= 0b10001001;
	_pbpu 	= 0b10001001;

//------------------------
//	pc
//	pc0~pc4	--> SEG
//------------------------

	_pcc 	= 0b00000000;
	_pc	    = 0b00000000;
	_pcpu 	= 0b00000000;



//------------------------
//	pa1 -->	IO 
//	pa3 -->	IO 
//------------------------
	_pas0	=	0;
	
	
//------------------------
//	pa4		-->  KEY3
//	pa5		-->  BEEP
//	pa6		-->  TX
//	pa7		-->  RX 
//------------------------
	_pas1	=	0b00000100;
/*	_pas1	=	0;*/
//------------------------
//	pb2		--> xx
//	pb3		--> KEY4
//------------------------
	_pbs0	=	0;
	
//------------------------
//	pb4		--> AN4
//	pb5		--> AN5
//	pb6		--> LED1
//------------------------
	_pbs1	=	0b00001110;		
	_pcs0	=	0b00000001;
	

	
	SDK_SYS.WorkMode = WORKMODE_SCAN;
	
}
/********************************************************************
*	Function:	fun_Reset_Init
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void	fun_Reset_Init()
{
	fun_TimeInit();
	fun_EEPROM_Init();
	fun_LCD_Init();
}
/********************************************************************
*	Function:	fun_SysSleep
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void	fun_SysSleep()
{
	fun_LCD_FULL(0);
	SET_LCD_Disable();		//关闭LCD
/*	SET_CTM_OFF();*/
	
	SET_TB0_OFF();			//关闭定时器
	fun_ADC_Disable();		//ADC关闭
	fun_Uart_Off();
	
/*	_wdtc = 0b10101111;*/
	
	_pawu1 =1;
	
//	GCC_CLRWDT();
//	GCC_CLRWDT1();
//	GCC_CLRWDT2();

	GCC_HALT();
	
	
	_pawu1 =0;
	
	SET_HALT_WAKEUP();
	fun_Reset_Init();
	fun_Uart_Init();
	fun_LCD_Display_Power();
	
	SDK_SYS.WorkMode = WORKMODE_WAKEUP;
	gbv_Flag_GetNTC = 0;
}


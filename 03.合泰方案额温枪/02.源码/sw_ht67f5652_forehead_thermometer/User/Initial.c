
/*************************************************************************************/
//**	Copyright	:	2016 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	initial.c 	       	                                        **
//**	Description	: 																**
//**	MCU      	: 	HT67F5652													**
//**	Author   	: 	Leo															**
//**	Date     	: 	2016/10/28													**
//**	Version  	: 	V00															**
//**	History  	:																**
/*************************************************************************************/

#include "..\User\Config.h"


/********************************************************************
Function:	fun_Sys_Init
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void	fun_Sys_Init()
{

	// disable cpc
	_cp0c 	= 0;
	_cp1c	= 0;
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
//	1		0		0		: fH/16
//	1		0		1		: fH/8
//	1		1		0		: fH/4
//	1		1		1		: fH/2
//	NOTE:	fH
	_smod = 0b00000011;


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
//		101: 2^16/fS
//		110: 2^17/fS
//		111: 2^18/fS
	_wdtc = 0b01010111;


#if(mac_MCU_pin == mac_MCU_P80)	
//pa 
	_pa		= 0xff;	
	_pac 	= 0xff;
	_papu 	= 0xff;
//pb 
	_pb		= 0xff;	
	_pbc 	= 0xff;
	_pbpu 	= 0xff;
//pc
	_pcc 	= 0b11010011;
	_pc	   &= 0b11010011;
	_pcpu 	= 0xff;
//pd
	_pdc 	= 0xff;
	_pdpu 	= 0xff;
#endif
	
#if(mac_MCU_pin == mac_MCU_P64)	
//pa 
	_pa		= 0xff;	
	_pac 	= 0xff;
	_papu 	= 0xff;
//pb 
	_pb		= 0xff;	
	_pbc 	= 0xff;
	_pbpu 	= 0xff;
//pc
	_pcc 	= 0b11011100;
	_pc	   &= 0b11011100;
	_pcpu 	= 0xff;
//pd
	_pdc 	= 0xff;
	_pdpu 	= 0xff;
#endif	
	

	fun_get_correction_data();
//	mac_LED_G_Light();

//	GPIO_KEY_Switch_WU 		= 1;
	GPIO_fun_Key_Scan_WU   	= 1;	
	
	gu8v_dispaly_mode.U8 	= 0b00000010;		// 上電默認 表面溫度
	gu8v_halt_time 			= mac_Halt_Count;	// 上電進入休眠	
}

/********************************************************************
Function:	fun_Reset_Init
INPUT	:
OUTPUT	:	
NOTE	:
********************************************************************/
void	fun_Reset_Init()
{
	//Timer0
	fun_TIMER0_INIT();

	//LCD
	fun_Lcd_Init();
		
	_pwrc = 0b10000000;		// 倍尓康額溫槍 LCD power 為2.2V，
							// 所以利用AVDD 2.4V 接入 PLCD
	_emi = 1;				// 開啟總中斷
}



/********************************************************************
Function:	fun_get_correction_data
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/

	


void	fun_get_correction_data()
{
	//fun_EEprom_Keep_R();
	if (fun_EEprom_Read(0x00) == 0x55)
	{
		//
		gu16v_correction_K1.byte.byte0 = fun_EEprom_Read(0x01);
		gu16v_correction_K1.byte.byte1 = fun_EEprom_Read(0x02);


		//
		gu16v_correction_K2.byte.byte0 = fun_EEprom_Read(0x03);
		gu16v_correction_K2.byte.byte1 = fun_EEprom_Read(0x04);
	}
	else
	{
		//
		gu16v_correction_K1.U16 = 5000;
		gu16v_correction_K2.U16 = 10000;

	}
	

	
	
}


/********************************************************************
Function:	fun_Init_PowerOn
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Init_PowerOn()
{
	volatile U8 lu8v_Led_Count; 
	fun_Beep_init(mac_beep_1);
	mac_LED_R_Light();
	fun_LCD_FULL(0x0f);
	do
	{
		GCC_CLRWDT();//清除看門狗
		if(gbv_10ms)
		{
			gbv_10ms = 0;
			lu8v_Led_Count++;
			
			fun_Beep_work_mode();
		}
		if(lu8v_Led_Count < 50)
		{

		}
		else if (lu8v_Led_Count<100)
		{
			mac_LED_G_Light();
		}
		else if(lu8v_Led_Count<150)
		{
			mac_LED_Y_Light();
		}
		else
		{
			mac_LED_DisLight();
			fun_LCD_FULL(0x0);
			break;
		}


	}while(1);

}



/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	Beep.c 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	LJQ															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:	蜂鸣器使用2~3K方波驱动										**
/*************************************************************************************/

#include "common.h"

BEEP_define	SDK_BEEP;


/********************************************************************
Function:	fun_BeepSet
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_BeepSet(u8	lu8v_beep_mode)
{
	
	BEEP_HIGH();
	
	if(gbv_BeepSwitch == 0)
	{
	SDK_BEEP.Mode = lu8v_beep_mode;
		fun_Uart_SendByte(lu8v_beep_mode);
	}
	else
	SDK_BEEP.Mode = 0;
}


/********************************************************************
Function:	fun_BeepScan
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_BeepScan(void)
{
	if(gbv_KeyStatus_4)
	{
		gbv_KeyStatus_4 = 0;
		SDK_BEEP.Switch = ~SDK_BEEP.Switch;			//标志位取反
		
		if(SDK_BEEP.Switch)		//静音模式标志位检测
		{
			SDK_BEEP.Mode = 0;
			CLR_LCD_BEEP();
		}
		else
		{
			SET_LCD_BEEP();
		}
	}
	

	
/*	if(gu8v_work_mode== TEST_MODE)
	{}
	else*/
	{
		switch(SDK_BEEP.Mode)
		{
			case	0:
				BEEP_OFF();
				SDK_BEEP.Time = 0;
			break;
			case	BEEP_1:
					
					SDK_BEEP.Time++;		
					if(SDK_BEEP.Time<15)
					{
						BEEP_ON(); 
					}	
					else
					{
						BEEP_OFF();
						SDK_BEEP.Mode = 0;
					}		
			break;			
			case	BEEP_1_1:
					SDK_BEEP.Time++;
					if(SDK_BEEP.Time<15)	
					{
						BEEP_ON();
					}	
					else if(SDK_BEEP.Time<30)
					{
						BEEP_OFF();
					}	
					else if(SDK_BEEP.Time<45)	
					{
						BEEP_ON();
					}
					else
					{
						BEEP_OFF();
						SDK_BEEP.Mode = 0;
					}
			break;
			case	BEEP_1_1_1:
					SDK_BEEP.Time++;	
					if(SDK_BEEP.Time<15)
					{
						BEEP_ON();
					}	
					else if(SDK_BEEP.Time<30)
					{
						BEEP_OFF();
					}	
					else if(SDK_BEEP.Time<45)	
					{
						BEEP_ON();
					}
					else if(SDK_BEEP.Time<60)
					{
						BEEP_OFF();
					}	
					else if(SDK_BEEP.Time<75)	
					{
						BEEP_ON();
					}								
					else
					{
						BEEP_OFF();
						SDK_BEEP.Mode = 0;
					}		
			break;
			case BEEP_1_1_10Time:
					SDK_BEEP.Time++;	
					
					if(SDK_BEEP.Time<5)
					{
						BEEP_ON();
					}	
					else if(SDK_BEEP.Time<10)
					{
						BEEP_OFF();
					}	
					else if(SDK_BEEP.Time<15)	
					{
						BEEP_ON();
					}								
					else if(SDK_BEEP.Time<40)
					{
						BEEP_OFF();
					}								//-----
					else if(SDK_BEEP.Time<80)
					{
						SDK_BEEP.Time = 0;
						SDK_BEEP.Count++;
						if(SDK_BEEP.Count>10)
						{
							SDK_BEEP.Count = 0;
							BEEP_OFF();
							SDK_BEEP.Mode = 0;
						}
					}								
		
			break;
						
			
		}
	}
}

/********************************************************************
Function:	fun_Beep_Test
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Beep_Test()
{
	fun_Uart_Init();
	fun_BeepSet(BEEP_1_1_10Time);
	while(1)
	{
		
		GCC_CLRWDT();//清除看門狗	
		if(gbv_10ms)					//10ms 循環
		{
			gbv_10ms = 0;
			fun_Uart_SendByte(0x33);
			fun_BeepScan();
		}	
	}
}


/*************************************************************************************/
//**	Copyright	:	2016 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	Key_Scan.c 	       	                                        **
//**	Description	: 																**
//**	MCU      	: 	HT67F5652													**
//**	Author   	: 	Leo															**
//**	Date     	: 	2016/10/28													**
//**	Version  	: 	V00															**
//**	History  	:																**
/*************************************************************************************/
#include "..\User\CONFIG.h"

/************************fun_Key_Scan Variable****************/
volatile	__byte_type  	gu8v_KeyCurrent;
volatile	__byte_type   	gu8v_KeyDown;			
volatile	U8				gu8v_KeyOld;
volatile	U8				gu8v_KeyNoChangedTime;
volatile	__byte_type  	gu8v_KeyPress;

volatile	__byte_type		gu8v_KeyUp;
volatile	U8				gu8v_KeyLast;
volatile	U8 				gu8v_KeyCanChange;
volatile	__byte_type 	gu8v_Key_status;
volatile	U8 				gu8v_Keypress_time;

/********************************************************************
Function:	fun_fun_Key_dealwith
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/

void	fun_Key_dealwith(void)
{
//============長按按鈕=============//
	if(gu8v_Keypress_time==100)			
	{
		if(gbv_on_KeyPress) 		
		{
		}
		else
		{
		}
		gu8v_Keypress_time = 101;					
	}
//============短按按鈕=============//
	else if(gu8v_Keypress_time<100)		
	{
		
		if(gbv_KeyDown_Scan) 		
		{
			gbv_KeyDown_Scan = 0;
			gbv_on_sys = 1;	
		}
		
	/*	if(gbv_key_on)					//释放按键 		
		{
			gbv_key_on	=	0;
		}
		if(gbv_key_down) 				//按下按键
		{
			gbv_key_down	=	0;
		}	*/			
	}
	else
	{
		gu8v_Keypress_time = 101;				//Key time more than 3 s don't count,b.
												//because of judge gu8v_Keypress_time==5
	}
	
//=============檢測按下時間=============//
	if(gbv_on_KeyPress)
	{
		gu8v_Keypress_time++;		
	}
	else
	{
		gu8v_Keypress_time = 0;	
	}	
//=============釋放休眠時間=============//
	if(gu8v_KeyDown.U8 != 0)
	{
		gu8v_halt_time=0;	

	}		


}

/********************************************************************
Function:	fun_Key_Scan
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void	fun_Key_Scan(void)
{
	gu8v_KeyCurrent.U8 = 0;

	if(GPIO_KEY_SACAN == 0)								
	{
		KeyCurrent0 = 1;								
	}
	if(GPIO_KEY_Switch == 0)
	{
		KeyCurrent1 = 1;
	}		
	if(GPIO_KEY_ROOM == 0)							
	{
		KeyCurrent2 = 1;								
	}		
	if(GPIO_KEY_Surface == 0)							
	{
		KeyCurrent3 = 1;								
	}	
	if(GPIO_KEY_BODY == 0)									
	{
		KeyCurrent4 = 1;								
	}	
	if(GPIO_KEY_BEEP == 0)									
	{
		KeyCurrent5 = 1;								
	}		
	if(gu8v_KeyCurrent.U8 != gu8v_KeyOld)  			
	{
		gu8v_KeyNoChangedTime = 0;       				
		gu8v_KeyOld = gu8v_KeyCurrent.U8;        		
		return;  										
	}
	else
	{	
		gu8v_KeyNoChangedTime++;	     				
		if(gu8v_KeyNoChangedTime >= LUCC_KEY_DEBOUNCE)	//50ms
		{
			gu8v_KeyNoChangedTime = LUCC_KEY_DEBOUNCE;	
			gu8v_KeyPress.U8 = gu8v_KeyOld;      			
		    gu8v_KeyDown.U8 = gu8v_KeyPress.U8 & (gu8v_KeyPress.U8 ^ gu8v_KeyLast);
			gu8v_KeyUp.U8 |= gu8v_KeyLast&(~gu8v_KeyPress.U8);     	
		    gu8v_KeyLast = gu8v_KeyPress.U8;            	
		}
	}
	fun_Key_dealwith();
}











/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	key.c	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Leo															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/


#include "common.h"

/************************Key_Scan Variable****************/

KEY_define SDK_KEY;


#define	KEYPRESS_SCAN()			{							\
		if(KEY_PORT0 == 0)	SDK_KEY.KeyCurrent.bits.b0 = 1;	\
		if(KEY_PORT1 == 0)	SDK_KEY.KeyCurrent.bits.b1 = 1;	\
		if(KEY_PORT2 == 0)	SDK_KEY.KeyCurrent.bits.b2 = 1;	\
		if(KEY_PORT3 == 0)	SDK_KEY.KeyCurrent.bits.b3 = 1;	\
		if(KEY_PORT4 == 0)	SDK_KEY.KeyCurrent.bits.b4 = 1;	\
		if(KEY_PORT5 == 0)	SDK_KEY.KeyCurrent.bits.b5 = 1;	\
		if(KEY_PORT6 == 0)	SDK_KEY.KeyCurrent.bits.b6 = 1;	\
		if(KEY_PORT7 == 0)	SDK_KEY.KeyCurrent.bits.b7 = 1;	\
								}
								
							
								

/********************************************************************
Function:	fun_Key_dealwith
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Key_init(void)
{

}

/********************************************************************
Function:	Key_Scan
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Key_Scan(void)
{

	SDK_KEY.KeyCurrent.U8 = 0;	//清除所有按键当前状态
	KEYPRESS_SCAN();			//按键状态获取
	if (SDK_KEY.KeyCurrent.U8 != SDK_KEY.KeyOld) 	//判断按键当前状态不等于上一次状态
	{
		SDK_KEY.KeyNoChangedTime = 0;			
		SDK_KEY.KeyOld = SDK_KEY.KeyCurrent.U8; 	//保存当前状态
		SDK_KEY.KeyDown.U8 = 0;
		SDK_KEY.KeyUp.U8 = 0;
		return; 
	}
	else
	{
		SDK_KEY.KeyNoChangedTime++;					//当前按键状态未改变的时间递增			  
		if (SDK_KEY.KeyNoChangedTime >= LUCC_KEY_DEBOUNCE) //50  //去抖处理
		{
			SDK_KEY.KeyNoChangedTime = LUCC_KEY_DEBOUNCE;
			SDK_KEY.KeyPress.U8 = SDK_KEY.KeyOld;																//
			SDK_KEY.KeyDown.U8 = SDK_KEY.KeyPress.U8 & (SDK_KEY.KeyPress.U8 ^ SDK_KEY.KeyLast); //保存触发过短按按键一次
			SDK_KEY.KeyUp.U8 |= SDK_KEY.KeyLast & (~SDK_KEY.KeyPress.U8);								//保存松手按键
			SDK_KEY.KeyLast = SDK_KEY.KeyPress.U8;															//保存当前按键状态并存储为上一次
		}
	}
//--------------------------------长按--------------------------------------------//
	if (SDK_KEY.KeyPressTime == LUCC_LONG_KEY_TIME)  //当按键按压时间等于长按时间
	{
		if (SDK_KEY.KeyPress.bits.b0)
		{
			gbv_KeyStatus_L_1 = 1;
			fun_Uart_SendByte(0x01);
		}
		if (SDK_KEY.KeyPress.bits.b1)
		{
			gbv_KeyStatus_L_2 = 1;
			fun_Uart_SendByte(0x02);
		}
		if (SDK_KEY.KeyPress.bits.b2)
		{
			gbv_KeyStatus_L_3 = 1;
			fun_Uart_SendByte(0x03);
		}
		if (SDK_KEY.KeyPress.bits.b3)
		{
			gbv_KeyStatus_L_4 = 1;
			fun_Uart_SendByte(0x04);
		}

		SDK_KEY.KeyPressTime = LUCC_LONG_KEY_TIME + 1;
	}
//--------------------------------短按--------------------------------------------//
	else if (SDK_KEY.KeyPressTime < LUCC_LONG_KEY_TIME)  //当按键按压时间小于长按时间则进入
	{
//--------------------------------弹起--------------------------------------------//		
		if (SDK_KEY.KeyUp.bits.b0)
		{
			gbv_KeyStatus_1 = 1;
			fun_Uart_SendByte(0x11);
		}
		if (SDK_KEY.KeyUp.bits.b1)
		{
			gbv_KeyStatus_2 = 1;
			fun_Uart_SendByte(0x22);
		}
		if (SDK_KEY.KeyUp.bits.b2)
		{
			gbv_KeyStatus_3 = 1;
			fun_Uart_SendByte(0x33);
		}
		if (SDK_KEY.KeyUp.bits.b3)
		{
			gbv_KeyStatus_4 = 1;
			fun_Uart_SendByte(0x44);
		}

//--------------------------------按下--------------------------------------------//
		if (SDK_KEY.KeyDown.bits.b0)
		{
			
		}
		if (SDK_KEY.KeyDown.bits.b1)
		{
			
		}
		if (SDK_KEY.KeyDown.bits.b2)
		{
			
		}
		if (SDK_KEY.KeyDown.bits.b3)
		{
		
		}

	}
	else		//当按键按压时间大于长按时间
	{
		SDK_KEY.KeyPressTime = LUCC_LONG_KEY_TIME + 1; //Key time more than 3 s don't count,b.
															   //because of judge gu8v_KeyPressTime==5
	}
	if (SDK_KEY.KeyDown.U8 != 0)		//判断如果有按键按下
	{
		SET_HALT_CLEAR();				//将等待设备关机时间清零
	}
	if (SDK_KEY.KeyPress.U8)			//判断有按键按下去
	{
		SDK_KEY.KeyPressTime++;			//按键按压的时间递增
	}
	else								//判断没有按键按下去
	{
		SDK_KEY.KeyPressTime = 0;		//按键按压的时间清零
	}
	SDK_KEY.KeyUp.U8 = 0;
}


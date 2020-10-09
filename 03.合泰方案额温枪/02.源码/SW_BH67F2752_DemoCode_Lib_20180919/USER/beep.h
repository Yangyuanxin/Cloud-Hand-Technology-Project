
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	Beep.h 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	LJQ															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:	蜂鸣器使用2~3K方波驱动										**
/*************************************************************************************/

#ifndef _BEEP_H_
#define _BEEP_H_

typedef struct
{
	u8			Time;
	u8			Count;
	u8			Mode;
	u8			Switch;				//蜂鸣器开关
	__8_type 	Status;
}BEEP_define;

#define gbv_BeepSwitch	SDK_BEEP.Status.bits.b0

/********************************************************************
Function:	BEEP GPIO
********************************************************************/

#define		pin_BEEP_IO			_pac5
#define		pin_BEEP			_pa5

#define 	BEEP_ON()			{_ct1on = 1;}
#define 	BEEP_OFF()			{_ct1on = 0;}

#define		BEEP_HIGH()		    {pin_BEEP = 1;}
#define		BEEP_LOW()		    {pin_BEEP = 0;}

#define		BEEP_1			    1
#define		BEEP_1_1		    2
#define		BEEP_1_1_1		    3
#define		BEEP_1_1_10Time	    4

/********************************************************************
Function:	
********************************************************************/

void fun_BeepScan(void);
void fun_BeepSet(u8	lu8c_beep_mode);
void fun_Beep_Test();

#endif


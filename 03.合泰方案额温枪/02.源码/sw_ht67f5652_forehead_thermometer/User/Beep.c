
/*************************************************************************************/
//**	Copyright	:	2016 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	Beep.c 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	HT67F5652													**
//**	Author   	: 	Leo															**
//**	Date     	: 	2016/10/28													**
//**	Version  	: 	V00															**
//**	History  	:	蜂鸣器使用2K方波驱动										**
/*************************************************************************************/

#include "..\User\CONFIG.h"

U8	gu8v_Beep_mode_times;
U8	gu8v_Beep_mode;


/********************************************************************
Function:	fun_Beep_init
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Beep_init(U8	lu8c_beep_mode)
{
	gu8v_Beep_mode = lu8c_beep_mode;
	mac_Beep_Low();
	gbv_beep_on = 1;
}


/********************************************************************
Function:	Beep_mode
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Beep_work_mode(void)
{
	switch(gu8v_Beep_mode)
	{
		case	0:
			gbv_beep_on = 0;
			gu8v_Beep_mode_times = 0;
		break;
				
		case	mac_beep_1_1:
				gu8v_Beep_mode_times++;
				if(gu8v_Beep_mode_times<20)	
				{
					gbv_beep_on = 1;
				}	
				else if(gu8v_Beep_mode_times<40)
				{
					gbv_beep_on = 0;
				}	
				else if(gu8v_Beep_mode_times<60)	
				{
					gbv_beep_on = 1;
				}
				else
				{
					gbv_beep_on = 0;
					gu8v_Beep_mode = 0;
				}
		break;
		case	mac_beep_1:
				gu8v_Beep_mode_times++;		
				if(gu8v_Beep_mode_times<30)
				{
					gbv_beep_on = 1;
				}	
				else
				{
					gbv_beep_on = 0;
					gu8v_Beep_mode = 0;
				}		
		break;		
		
		case	mac_beep_1_1_1_1:
				gu8v_Beep_mode_times++;	
				if(gu8v_Beep_mode_times<20)
				{
					gbv_beep_on = 1;
				}	
				else if(gu8v_Beep_mode_times<40)
				{
					gbv_beep_on = 0;
				}	
				else if(gu8v_Beep_mode_times<60)	
				{
					gbv_beep_on = 1;
				}
				else if(gu8v_Beep_mode_times<80)
				{
					gbv_beep_on = 0;
				}	
				else if(gu8v_Beep_mode_times<100)	
				{
					gbv_beep_on = 1;
				}				
				else if(gu8v_Beep_mode_times<120)
				{
					gbv_beep_on = 0;
				}	
				else if(gu8v_Beep_mode_times<140)	
				{
					gbv_beep_on = 1;
				}				
				else
				{
					gbv_beep_on = 0;
					gu8v_Beep_mode = 0;
				}		
		break;			
		
	}
}

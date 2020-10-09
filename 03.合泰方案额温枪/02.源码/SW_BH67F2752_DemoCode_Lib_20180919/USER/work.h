
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	work.h	       	                                        	**
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#ifndef _Work_mode_H_
#define	_Work_mode_H_


typedef struct
{
	u8  		Dis_CF_Flag;
	u8  		DisMode;
	u8			WorkMode;
	u8			PublicMode;
  u8  		PublicCount;
	__8_type	Status;

}SYS_define;

typedef struct
{
	__16_type	ADC_Res1;
	__16_type	ADC_Res2;
	__16_type	ADC_Vtp;
	__16_type	ADC_VtpZero;
	__16_type	AmbTemp;
	__16_type	ObjTemp;
	__16_type	BodyTemp;
	__16_type	RES_Buff;
	__16_type	VTP_Buff1;
	__16_type	VTP_Buff2;
	__16_type	BAT_Buff;		//µç³ØBuffer
}CAL_define;

extern  SYS_define	SDK_SYS;
extern  CAL_define	SDK_CAL;

#define	gbv_Flag_GetNTC				SDK_SYS.Status.bits.b0


u8		get_obj_temp();
void	fun_CORR_MODE();
u8		fun_get_VTP();
u8  	fun_get_NTC();
void	fun_WorkModeScan(void);
u8		fun_get_VTP_CORR();
void 	fun_SET_MODE();

#define	WORKMODE_POWERON	0
#define	WORKMODE_WAKEUP		1
#define	WORKMODE_SCAN	    2
#define	WORKMODE_MEAS		3
#define	WORKMODE_CORR	    4
#define WORKMODE_MEMORY     5
#define	WORKMODE_SET		6
#define	WORKMODE_SLEEP		7

#define	WORKMODE_AmbTemp	8

#define	WORKMODE_ObjTemp	9

#define GOTO_POWERON_MODE()     {SDK_SYS.PublicMode = 0; SDK_SYS.WorkMode = WORKMODE_POWERON;}
#define GOTO_WAKEUP_MODE()     	{SDK_SYS.PublicMode = 0; SDK_SYS.WorkMode = WORKMODE_WAKEUP;}
#define GOTO_SCAN_MODE()     	{SDK_SYS.PublicMode = 0; SDK_SYS.WorkMode = WORKMODE_SCAN;}
#define GOTO_MEAS_MODE()     	{SDK_SYS.PublicMode = 0; SDK_SYS.WorkMode = WORKMODE_MEAS;}
#define GOTO_CORR_MODE()   		{SDK_SYS.PublicMode = 0; SDK_SYS.WorkMode = WORKMODE_CORR;}
#define GOTO_MEMORY_MODE()     	{SDK_SYS.PublicMode = 0; SDK_SYS.WorkMode = WORKMODE_MEMORY;}
#define GOTO_SET_MODE()     	{SDK_SYS.PublicMode = 0; SDK_SYS.WorkMode = WORKMODE_SET;}
#define GOTO_SLEEP_MODE()     	{SDK_SYS.PublicMode = 0; SDK_SYS.WorkMode = WORKMODE_SLEEP;}

#define GOTO_AmbTemp_MODE()     	{SDK_SYS.PublicMode = 0; SDK_SYS.WorkMode = WORKMODE_AmbTemp;}
#define GOTO_ObjTemp_MODE()     	{SDK_SYS.PublicMode = 0; SDK_SYS.WorkMode = WORKMODE_ObjTemp;}



#endif

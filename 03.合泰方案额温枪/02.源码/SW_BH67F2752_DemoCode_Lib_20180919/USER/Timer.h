
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	timer.h 	       	                                        **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#ifndef _Timer_H_
#define _Timer_H_

typedef struct
{
	u8	            Count1;
	u8      	    Count2;	
	u8				HaltTime;
	__8_type	    Status;
}TIME_define;

extern	TIME_define	SDK_TM;

#define gbv_10ms 			SDK_TM.Status.bits.b0
#define gbv_100ms			SDK_TM.Status.bits.b1
#define gbv_250ms 			SDK_TM.Status.bits.b2
#define gbv_500ms	 		SDK_TM.Status.bits.b3
#define gbv_1s				SDK_TM.Status.bits.b4
#define	gbv_500ms_reversal  SDK_TM.Status.bits.b5


/********************************************************************
DEFINE:	
********************************************************************/
#define	HALT_TIME  40
#define	SET_HALT_CLEAR()	SDK_TM.HaltTime = 0;
#define	SET_HALT_WAKEUP()	SDK_TM.HaltTime = HALT_TIME-10;

#define		SET_CTM_F_SYSc4()			_ct1ck2 = 0;_ct1ck1 = 0;_ct1ck0 = 0;
#define		SET_CTM_F_SYS()				_ctck2 = 0;_ctck1 = 0;_ctck0 = 1;
#define		SET_CTM_F_SYSc16()			_ctck2 = 0;_ctck1 = 1;_ctck0 = 0;
#define		SET_CTM_F_SYSc64()			_ctck2 = 0;_ctck1 = 1;_ctck0 = 1;
#define		SET_CTM_F_SUB()				_ctck2 = 1;_ctck1 = 0;_ctck0 = 0;
/*#define		SET_CTM_F_SUB()				_ctck2 = 1;_ctck1 = 0;_ctck0 = 1;*/
#define		SET_CTM_F_TCK_RISING()		_ctck2 = 1;_ctck1 = 1;_ctck0 = 0;
#define		SET_CTM_F_TCK_FALLING()		_ctck2 = 1;_ctck1 = 1;_ctck0 = 1;

#define		SET_CTM_P_1024()			_ctrp2 = 0;_ctrp1 = 0;_ctrp0 = 0;
#define		SET_CTM_P_128()				_ctrp2 = 0;_ctrp1 = 0;_ctrp0 = 1;
#define		SET_CTM_P_256()				_ctrp2 = 0;_ctrp1 = 1;_ctrp0 = 0;
#define		SET_CTM_P_384()				_ct1rp2 = 0;_ct1rp1 = 1;_ct1rp0 = 1;
#define		SET_CTM_P_512()				_ctrp2 = 1;_ctrp1 = 0;_ctrp0 = 0;
#define		SET_CTM_P_640()				_ctrp2 = 1;_ctrp1 = 0;_ctrp0 = 1;
#define		SET_CTM_P_768()				_ctrp2 = 1;_ctrp1 = 1;_ctrp0 = 0;
#define		SET_CTM_P_896()				_ctrp2 = 1;_ctrp1 = 1;_ctrp0 = 1;

#define		SET_CTM_OFF()				_ct0on = 0;		
#define		SET_CTM_ON()				_ct0on = 1;

#define		SET_CTM_MODE_COMPARE()		_ctm1 = 0;_ctm0 = 0;
#define		SET_CTM_MODE_UNdefine()		_ctm1 = 0;_ctm0 = 1;
#define		SET_CTM_MODE_PWM()			_ct1m1 = 1;_ct1m0 = 0; _ct1io1 = 1;_ct1io0 = 0; 
#define		SET_CTM_MODE_TIME()			_ctm1 = 1;_ctm0 = 1;

#define		SET_PWM_P_PERIOD_A_DUTY()	_ct1dpx = 0;
#define		SET_PWM_P_DUTY_A_PERIOD()	_ct1dpx = 1;

#define		SET_TIEM_P_MATCH()			_ct1cclr = 0;
#define		SET_TIEM_A_MATCH()			_ct1cclr = 1;



#define		SET_TBC_F_SYS()		_pscr = 0;
#define		SET_TBC_F_SYSc4() 	_pscr = 1;
#define		SET_TBC_F_SUB()		_pscr = 2;
#define		SET_TB0_ON()		_tb0on = 1;
#define		SET_TB0_OFF()		_tb0on = 0;
#define		SET_TB1_ON()		_tb1on = 1;
#define		SET_TB1_OFF()		_tb1on = 0;


/********************************************************************
Function:	
********************************************************************/
void	fun_TimeInit();
void	fun_SysTimeScan();

#endif
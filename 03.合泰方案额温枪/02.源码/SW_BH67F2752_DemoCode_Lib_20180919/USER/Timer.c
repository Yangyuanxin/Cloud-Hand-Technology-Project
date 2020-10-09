
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	timer.c 	       	                                        **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/


#include "common.h"

TIME_define	SDK_TM;

/********************************************************************
Function:	TIMER0_INIT
INPUT	:	
OUTPUT	:	
NOTE	:   
*******************************************************************/
void	fun_TimeInit()
{
	
	SET_CTM_F_SYSc4();			//高频系统时钟4分频源
	SET_CTM_P_384();			//设置384个CTMn时钟周期
	SET_CTM_MODE_PWM();			//设置CTMn的工作模式为PWM，输出高
	SET_PWM_P_PERIOD_A_DUTY();	//设置CCRP – 周期；CCRA – 占空比
	SET_TIEM_P_MATCH();			//设置CTMn 比较器 P 匹配
	
	
//	设置CCRA的寄存器数据	
	_ctm1al = 128;		
	_ctm1ah = 0;
//	若 fSYS =8MHz，CTMn 时钟源选择 f SYS /4，CCRP=0，CCRA=129	
//	CTMn PWM输出频率= (f SYS /4)/1024=1953Hz，duty=128/(2×128)=50%，

//	SET_CTM_ON();
//	_mf0e = 1;
//	_ctmae = 1;	
//	_emi = 1;
	
	SET_TBC_F_SUB();			//预分频时钟源选择fsub低频时钟源
	SET_TB0_ON();				//使能定时器0;
	
//  选择预分频时钟源为内部低速时钟，32K，
//	分频默认为000，为2的8次方
//	32K/2的8次方=125Hz
//  1/125=0.008s的中断

}	


/********************************************************************
Function:	func_mult_0_2timer_isr
INPUT	:	
OUTPUT	:	
NOTE	:   
*******************************************************************/

//void __attribute((interrupt(0x10)))  func_mult0_isr()
//{	
//	//t0 a match
//	if(_ctm0af == 1)
//	{
//		gbv_10ms = 1;
//	}
//	_ctm0af = 0;
//	_ctm0pf = 0;
//}


/********************************************************************
Function:	fun_SysTime_Scan
INPUT	:
OUTPUT	:	
NOTE	:   系統時鐘計數（8ms計時一次）
********************************************************************/

void	fun_SysTimeScan()
{			
	// 500ms計數
	SDK_TM.Count1 ++;
	if(SDK_TM.Count1 >=62)
	{
		SDK_TM.Count1 		= 0;
		gbv_500ms 			= 1;
		gbv_500ms_reversal 	= ~gbv_500ms_reversal;		//翻转标志位（运用给那些需要闪烁的动作）
		SDK_TM.HaltTime ++;
	}
	// 1S計數
	SDK_TM.Count2++;
	if(SDK_TM.Count2 >= 125)
	{
		SDK_TM.Count2 = 0;
		gbv_1s = 1;
	
	}
}





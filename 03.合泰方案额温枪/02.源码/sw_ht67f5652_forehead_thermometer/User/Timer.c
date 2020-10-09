
/*************************************************************************************/
//**	Copyright	:	2016 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	timer.c 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	HT67F5652													**
//**	Author   	: 	Leo															**
//**	Date     	: 	2016/10/28													**
//**	Version  	: 	V00															**
//**	History  	:																**
/*************************************************************************************/

#include "..\User\Config.h"

U8	gu8v_200us_count;
volatile	__byte_type	gu8v_time_flag;


/********************************************************************
Function:	fun_TIMER0_INIT
INPUT	:	
OUTPUT	:	
NOTE	:   
*******************************************************************/
void	fun_TIMER0_INIT()
{
	

//	|--------------------------------------------------------------------------|
//	|------------------------------TM0C0  -------------------------------------|
//	|BIT	|7		|6		|5		|4		|3		|2		|1		|0	   ----|		
//	---------------------------------------------------------------------------|
//	|Name	|TnPAU	|TnCK2	|TnCK1	|TnCK0	|TnON	|TnRP2	|TnRP1	|TnRP0  ---|	
//	Bit 7   TnPAU: TMn Counter Pause Control  
//	  0: run  
//	  1: pause
//	Bit 6~4  TnCK2~TnCK0 : Select TMn Counter clock 
//	  000: f SYS /4  001: f SYS    010: f H/16 011: f H/64 100: f TBC  101: Undefined  
//	  110: TCKn rising edge clock  111: TCKn falling edge clock
//	Bit 3   TnON : TMn Counter On/Off Control  
//	  0: Off  
//	  1: On
//	Bit 2~0 TnRP2~TnRP0:

	_tm0c0 = LU8C_TM0C0;				
	
//	|--------------------------------------------------------------------------|
//	|------------------------------TM0C1  -------------------------------------|
//	|BIT	|7		|6		|5		|4		|3		|2		|1		|0	   ----|		
//	---------------------------------------------------------------------------|
//	|Name	|TnM1	|TnM0	|TnIO1	|TnIO0	|TnOC	|TnPOL	|TnDPX	|TnCCLR----|		
//
//	Bit 7~6  TnM1~TnM0 : Select TMn Operating Mode 
//	  00: Compare Match Output Mode 
//	  01: Undefined 
//	  10: PWM Mode 
//	  11: Timer/Counter Mode
//	Bit 5~4  TnIO1~TnIO0: Select TPn_0, TPn_1 output function
//	Timer/counter Mode  
//	 unused
//	Bit 3   TnOC : TPn_0, TPn_1 Output control bit
//	Bit 2   TnPOL: TPn_0, TPn_1 Output polarity Control 
//	Bit 1   TnDPX: TMn PWM period/duty Control  
//	Bit 0   TnCCLR: Select TMn Counter clear condition  
//	  0: TMn Comparatror P match  
//	  1: TMn Comparatror A match

	_tm0c1 = LU8C_TM0C1;			
	_tm0al = LU8C_TM0AL;		
	_tm0ah = LU8C_TM0AH;
	#ifdef	Def_timer0_interrupt
	_mf0e = 1;
	_t0ae = 1;	

	#else
	_t0ae = 0;
	#endif
	
}	


/********************************************************************
Function:	func_mult_0_2timer_isr
INPUT	:	
OUTPUT	:	
NOTE	:   
*******************************************************************/

#ifdef	Def_timer0_interrupt
void __attribute((interrupt(0x0c)))  func_mult0_isr()
{	
	//t0 a match
	if(_t0af == 1)
	{
		gu8v_200us_count++;
		//200US*50=10ms
		if(gu8v_200us_count>=50)
		{
			gu8v_200us_count = 0;
			gbv_10ms = 1;
		}
		if(gbv_beep_on)
		{		
			pin_GPIO_BEEP = ~pin_GPIO_BEEP;
		}
		else
		{
			//êPé]io
			mac_Beep_Low();
		}	
	}
	_t0af = 0;
	_t0pf = 0;
}
#endif



void fun_Time_Test()
{
	fun_Beep_init(mac_beep_1_1_1_1);
	while(1)
	{
		GCC_CLRWDT();//Çå³ý¿´éT¹·
		
		if(gbv_10ms)					//10ms Ñ­­h
		{
			gbv_10ms = 0;		
			fun_Beep_work_mode();		//·äøQÆ÷’ßÃè
		}
	}
}





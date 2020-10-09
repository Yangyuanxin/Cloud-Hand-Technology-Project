#ifndef _Main_H_
#define _Main_H_

/********************************************************************
Function:	DEFINE
********************************************************************/
#define	mac_Halt_Count	60	//20s



/********************************************************************
Function:	volatile
********************************************************************/				
extern	volatile			U8	gu8v_halt_time;
extern	volatile			__byte_type	gu8v_sys_status;
//#define gbv_already_correction 	gu8v_sys_status.bits.b0
#define gbv_beep_on			gu8v_sys_status.bits.b1
#define gbv_adc_Sample_Mode	gu8v_sys_status.bits.b2
#define gbv_adc_Sample_lock gu8v_sys_status.bits.b3
#define gbv_adc_success		gu8v_sys_status.bits.b4
#define gbv_ntc_cc		 	gu8v_sys_status.bits.b5
//#define gbv_wright_lock_flash 	gu8v_sys_status.bits.b6
#define gu8v_sys_status7 	gu8v_sys_status.bits.b7


/********************************************************************
Function:	
********************************************************************/
void	fun_into_halt();
void	fun_SysTime_Count();
void	fun_Test_Function();

#endif

/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	calculate.h 	       	                                    **
//**	Description	: 																**
//**	MCU      	: 	xx															**
//**	Author   	: 	Ljq													    	**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#ifndef _CALCULATE_H_
#define _CALCULATE_H_

//=========================================校準點設置=======================================//
#define CORRTEMP_LOW    250
#define CORRTEMP_HIGH   370

//=========================================接口变量=========================================//
extern	volatile	__16_type   gu16v_Correction_K1;        //  校準係數
extern	volatile	__16_type   gu16v_Correction_K2;

//=========================================接口函數=========================================//

/********************************************************************
*   Function:	fun_Rntc_Calculate
*   INPUT	:	lu16v_adc_res1		>>	分壓電阻ADC值1
*   			lu16v_adc_res2 		>>	分壓電阻ADC值2
*   OUTPUT	:	lu16v_res_ntc		>>	NTC 阻值
*   NOTE	:   電路圖如下:	(取ADC的絕對值)
*   			---  VCM
*   			 |	
*   			---
*   			| |	 NTC
*   			---
*                |   TEMP1 (adc_res1)
*               ---
*               | |  100K
*               ---
*                |   TEMP2 (adc_res2)
*               --- 
*               | |	 300K
*               ---
*                |
*               ---  GND
********************************************************************/
u16	    fun_Rntc_Calculate(u16 lu16v_adc_res1,u16 lu16v_adc_res2);
/********************************************************************
*	Function:	fun_Tsen_Calculate
*	INPUT	:	lu16v_res_ntc       >>  NTC的阻值
*	OUTPUT	:	ls16v_Tntc_actual   >>  NTC的温度
*	NOTE	:   NTC 對應的溫度
********************************************************************/
s16		fun_Tsen_Calculate(u16 lu16v_res_ntc);
/********************************************************************
*	Function:	fun_Tobj_Calculate
*	INPUT	:	ls16v_temp_ntc		>>  NTC 溫度
*				ls16v_adc_vtp		>>  VTP ADC值
*	OUTPUT	:	ls16v_Tobj_Result 	>>  物表溫度
*	NOTE	:   物表溫度計算
********************************************************************/ 
s16	fun_Tobj_Calculate(s16 ls16v_temp_ntc,s16 ls16v_adc_vtp);
/********************************************************************
*   Function:	fun_Corr_Calculate
*   INPUT	:	ls16v_adc_CorrVtp1	>>	第一個校準點的AD值
*   			ls16v_adc_CorrVtp2	>>	第二個校準點的AD值
*   			lu16v_res_ntc		>>	NTC的電阻值
*   OUTPUT	:	NONE
*   NOTE	:   通過校準兩個溫度點獲取K1、K2的值
********************************************************************/
void	fun_Corr_Calculate(s16 ls16v_adc_CorrVtp1,s16 ls16v_adc_CorrVtp2,u16 lu16v_res_ntc);
/********************************************************************
*   Function:	fun_Temp_SurfaceToBody
*   INPUT	:	lu16v_ObjTemp		>>	物表溫度	
*   OUTPUT	:	lu16v_BodyTemp		>>	人體溫度
*   NOTE	:   
********************************************************************/
u16		fun_Temp_SurfaceToBody(u16 lu16v_ObjTemp);
/********************************************************************
*   Function:	fun_ObjTemp_DataHandle
*   INPUT	:	ls16v_ObjTemp		>>	物表溫度	
*   OUTPUT	:	
*   NOTE	:   xx.xx °C -- xx.x °C
********************************************************************/
s16		fun_ObjTemp_DataHandle(s16 ls16v_ObjTemp);




#endif


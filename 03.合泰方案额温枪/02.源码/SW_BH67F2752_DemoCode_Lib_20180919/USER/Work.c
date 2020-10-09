/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	work.c 	       	                                        **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/



/*************************************************************************************/
//**								操作说明						                 **
//**					出产一定要校准。校准后即可测试正确温度
//**
/*************************************************************************************/
#include "common.h"

SYS_define	SDK_SYS;
CAL_define	SDK_CAL;

/********************************************************************
Function:	fun_WorkModeScan
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/

void fun_WorkModeScan(void)
{
	switch(SDK_SYS.WorkMode)		//获取系统状态模式
	{
		case	WORKMODE_POWERON:	//系统开关
		break;
		case	WORKMODE_WAKEUP:	//系统唤醒模式
		/*	if(gbv_KeyStatus_1)		//按键1唤醒开机*/
			{
				GOTO_SCAN_MODE();	//跳转到工作模式
			}
			
		break;
		case	WORKMODE_SCAN:		//扫描模式
		{
		
			if(fun_get_NTC())		//获取NTC数值，返回1就是获取成功，返回0获取失败
			{
				SDK_CAL.RES_Buff.U16 = fun_Rntc_Calculate(SDK_CAL.ADC_Res1.S16,SDK_CAL.ADC_Res2.S16);
				gbv_Flag_GetNTC = 1;
			}
				
			if(gbv_Flag_GetNTC)
			{	
				gbv_Flag_GetNTC = 0;
			
				// 進入量測工作模式
				//-----------------------------------------------------------------------//	
			/*	gbv_KeyStatus_1 = 1;*/
				if(gbv_KeyStatus_1)
				{
					gbv_KeyStatus_1 = 0;
					
					if(SDK_SYS.DisMode == Mode_Body)
					{
						GOTO_MEAS_MODE();
					}
					else if(SDK_SYS.DisMode == Mode_Room)
					{
						GOTO_AmbTemp_MODE();
					}
					else if(SDK_SYS.DisMode == Mode_SurTmp)
					{
						GOTO_ObjTemp_MODE();
					}
					
					gbv_Flag_GetNTC = 	0;
				}
				
				if(gbv_KeyStatus_2)			//短按按键2切换模式显示
				{		
					gbv_KeyStatus_2 = 0;
									
					SDK_SYS.DisMode ++;
					
					if(SDK_SYS.DisMode == 4)
						SDK_SYS.DisMode = 1;
						
					fun_LCD_Display_Mode(SDK_SYS.DisMode);
					
					
				}
				
				if(gbv_KeyStatus_L_2)			//长按按键2进入校验模式
				{		
					gbv_KeyStatus_L_2 = 0;			
					GOTO_CORR_MODE();		//跳转到校准模式
					fun_LCD_Display_Cor(0);
				}
				
				if(gbv_KeyStatus_3)			//短按按键3切换摄氏度和华氏度显示
				{			
					gbv_KeyStatus_3 = 0;
										
					SDK_SYS.Dis_CF_Flag++;
					
					if(SDK_SYS.Dis_CF_Flag == 3)
						SDK_SYS.Dis_CF_Flag = 1;
					
					fun_LCD_Display_CF(SDK_SYS.Dis_CF_Flag);
					
					if(SDK_SYS.Dis_CF_Flag == 1)				//C
					{
						if(SDK_SYS.DisMode == Mode_Body)
						{
							LCD_DISPLAY_NUM(0,SDK_CAL.ObjTemp.U16);		//设置LCD显示温度，带小数点
						}
						else if(SDK_SYS.DisMode == Mode_Room)
						{
							LCD_DISPLAY_NUM(0,SDK_CAL.AmbTemp.U16);		//设置LCD显示温度，带小数点
						}
						else if(SDK_SYS.DisMode == Mode_SurTmp)
						{
							LCD_DISPLAY_NUM(0,SDK_CAL.ObjTemp.U16);		//设置LCD显示温度，带小数点
						}
					
					}
					else if(SDK_SYS.Dis_CF_Flag == 2)			//F
					{
						if(SDK_SYS.DisMode == Mode_Body)
						{
							LCD_DISPLAY_NUM(1,(SDK_CAL.ObjTemp.U16*1.8)+32);		//设置LCD显示温度，带小数点
						}
						else if(SDK_SYS.DisMode == Mode_Room)
						{
							LCD_DISPLAY_NUM(1,(SDK_CAL.AmbTemp.U16*1.8)+32);		//设置LCD显示温度，带小数点
						}
						else if(SDK_SYS.DisMode == Mode_SurTmp)
						{
							LCD_DISPLAY_NUM(1,(SDK_CAL.ObjTemp.U16*1.8)+32);		//设置LCD显示温度，带小数点
						}
					}
						
					
				}
			
			}
			
/*			fun_LCD_Display_C();		//闪烁程序*/

			break;
		}
		case	WORKMODE_MEAS:			//获取人体温度
		{
			if(fun_get_VTP())
			{	
				SDK_CAL.AmbTemp.U16		=	fun_Tsen_Calculate(SDK_CAL.RES_Buff.U16);						//将NTC的温度和VTP的ADC值带入		
				SDK_CAL.ObjTemp.U16 	= 	fun_Tobj_Calculate(SDK_CAL.AmbTemp.U16,SDK_CAL.ADC_Vtp.S16);	//将第一个校准点带入	
				SDK_CAL.ObjTemp.U16 	= 	fun_ObjTemp_DataHandle(SDK_CAL.ObjTemp.U16);					//获取物表温度
				SDK_CAL.BodyTemp.U16	=	fun_Temp_SurfaceToBody(SDK_CAL.ObjTemp.U16);					//获取人体温度
				
				fun_BeepSet(BEEP_1);						//设置蜂鸣器模式
				if(SDK_SYS.Dis_CF_Flag == 1)				//C
				{
					LCD_DISPLAY_NUM(0,SDK_CAL.ObjTemp.U16);		//设置LCD显示温度，带小数点
				}
				else if(SDK_SYS.Dis_CF_Flag == 2)
				{
					LCD_DISPLAY_NUM(1,(SDK_CAL.ObjTemp.U16*1.8)+32);		//设置LCD显示温度，带小数点
				}
				GOTO_SCAN_MODE();							//返回等待按键按下模式
				
			}
			else
			{	
				fun_LCD_Display____();			//显示等待获取
			}
		}
		break;	
		case	WORKMODE_AmbTemp:			//获取环境温度
		{
			if(fun_get_VTP())
			{	
				SDK_CAL.AmbTemp.U16		=	fun_Tsen_Calculate(SDK_CAL.RES_Buff.U16);						//将NTC的温度和VTP的ADC值带入		

				fun_BeepSet(BEEP_1);						//设置蜂鸣器模式
				
				if(SDK_SYS.Dis_CF_Flag == 1)				//C
				{
					LCD_DISPLAY_NUM(0,SDK_CAL.AmbTemp.U16);		//设置LCD显示温度，带小数点
				}
				else if(SDK_SYS.Dis_CF_Flag == 2)
				{
					LCD_DISPLAY_NUM(1,(SDK_CAL.AmbTemp.U16*1.8)+32);		//设置LCD显示温度，带小数点
				}
				GOTO_SCAN_MODE();							//返回等待按键按下模式
				
			}
			else
			{	
				fun_LCD_Display____();			//显示等待获取
			}	
		}
		case	WORKMODE_ObjTemp:			//获取表面温度
		{
			if(fun_get_VTP())
			{	
				SDK_CAL.AmbTemp.U16		=	fun_Tsen_Calculate(SDK_CAL.RES_Buff.U16);						//将NTC的温度和VTP的ADC值带入		
				SDK_CAL.ObjTemp.U16 	= 	fun_Tobj_Calculate(SDK_CAL.AmbTemp.U16,SDK_CAL.ADC_Vtp.S16);	//将第一个校准点带入	

				fun_BeepSet(BEEP_1);						//设置蜂鸣器模式
				
				
				if(SDK_SYS.Dis_CF_Flag == 1)				//C
				{
					LCD_DISPLAY_NUM(0,SDK_CAL.ObjTemp.U16);		//设置LCD显示温度，带小数点
				}
				else if(SDK_SYS.Dis_CF_Flag == 2)
				{
					LCD_DISPLAY_NUM(1,(SDK_CAL.ObjTemp.U16*1.8)+32);		//设置LCD显示温度，带小数点
				}
				GOTO_SCAN_MODE();							//返回等待按键按下模式
				
			}
			else
			{	
				fun_LCD_Display____();			//显示等待获取
			}
		}	
		case	WORKMODE_CORR:			//校准模式
		{
			fun_CORR_MODE();
			break;		
		}
	
		default:	GOTO_SCAN_MODE();break;		
	}
}

volatile bit 	gbv_Corr_Lock;
volatile u8 	gu8v_CorrMode;
volatile u8 	gu8v_CorrCount;
void	fun_CORR_MODE()			//校正模式
{
	switch(gu8v_CorrMode)
	{
		case	0://進入溫度測量
		
			if(gbv_Corr_Lock == 0)
			{
				if(fun_get_NTC())//NTC	//获取环境温度
				{	
					gbv_Corr_Lock = 1;
					SDK_CAL.RES_Buff.U16 = fun_Rntc_Calculate(SDK_CAL.ADC_Res1.S16,SDK_CAL.ADC_Res2.S16);	
					SDK_CAL.AmbTemp.U16	 = fun_Tsen_Calculate(SDK_CAL.RES_Buff.U16);
				}
			}
			else
			{
				if(gbv_500ms_reversal)
				{
					LCD_DISPLAY_NUM(1,SDK_CAL.AmbTemp.U16);			//显示物表温度
				}
				else
				{
					fun_LCD_FULL(0);			//时间还没到就显示0;
				}
				if(gbv_KeyStatus_1)				//短按一下按键进入获取温度
				{
					gbv_KeyStatus_1 = 0;				
					gbv_Corr_Lock = 0;
					gu8v_CorrCount = 0;
					fun_LCD_Display_Cor(1);
					gu8v_CorrMode++;
				}
			}
			break;
		case	1:
			gu8v_CorrCount++;
			if(gu8v_CorrCount>100)
			{
				gu8v_CorrCount = 0;
				gu8v_CorrMode++;
			}
		break;
		case	2:
		
			if(fun_get_VTP_CORR())//記錄第一個溫度點的參數	
			{	
				LCD_DISPLAY_NUM(1,SDK_CAL.ADC_Vtp.S16);
			}
			if(gbv_KeyStatus_1)
			{
				gbv_KeyStatus_1 = 0;	
				gu8v_CorrMode++;		
				SDK_CAL.VTP_Buff1.S16 = SDK_CAL.ADC_Vtp.S16;
				fun_LCD_Display_Cor(2);
			}
		break;
		case	3:
			gu8v_CorrCount++;
			if(gu8v_CorrCount>100)
			{
				gu8v_CorrCount = 0;
				gu8v_CorrMode++;
			}
		break;
		case	4:
			if(fun_get_VTP_CORR())//記錄第二個溫度點的參數	
			{	
				LCD_DISPLAY_NUM(1,SDK_CAL.ADC_Vtp.S16);	
			}	
			if(gbv_KeyStatus_1)
			{
				gbv_KeyStatus_1 = 0;	
				gu8v_CorrMode++;		
				SDK_CAL.VTP_Buff2.S16 = SDK_CAL.ADC_Vtp.S16;
			}
		break;
		case	5:
		{

			fun_Corr_Calculate(SDK_CAL.VTP_Buff1.S16,SDK_CAL.VTP_Buff2.S16,SDK_CAL.RES_Buff.U16);					

			//將數據存入 eeprom 中	
			if(gu16v_Correction_K2.U16>9600&&gu16v_Correction_K2.U16<10400)	
			{
				fun_EEprom_Write(0,0x55);			
				fun_EEprom_Write(1,gu16v_Correction_K1.byte.byte0);
				fun_EEprom_Write(2,gu16v_Correction_K1.byte.byte1);
				fun_EEprom_Write(3,gu16v_Correction_K2.byte.byte0);
				fun_EEprom_Write(4,gu16v_Correction_K2.byte.byte1);
				fun_LCD_Display_Success();	
			}
			else
			{
				fun_LCD_Display_Fail();
			}
				gu8v_CorrMode++;
			break;	
		}
		case 	6:
			if(gbv_KeyStatus_1)
			{
				gbv_KeyStatus_1 = 0;
				GOTO_SCAN_MODE();
				fun_LCD_FULL(0);
				gbv_1s	=	1;
				gbv_Flag_GetNTC	= 0;
				gu8v_CorrMode = 0;
			}
			
		break;

		default: break;
	}
	
}


u8 fun_get_NTC()		//获取NTC温度（环境温度）
{
	switch(SDK_SYS.PublicMode)
	{
		case 0:
		{	
			fun_TEMP1_INIT();		//初始化温度1脚
			SDK_SYS.PublicMode++;
			break;
		}
		case 1:
		{
			if(gbv_adc_Success)		//等待AD转换成功没有
			{	
				SDK_CAL.ADC_Res1.S16 = SDK_ADC.AdcData.S32;	
				fun_ADC_Disable();
				
				fun_TEMP2_INIT();		//初始化温度2脚
				SDK_SYS.PublicMode ++;	
			}
			break;
		}
		case 2:
		{
			if(gbv_adc_Success)
			{	
				SDK_CAL.ADC_Res2.S16 = SDK_ADC.AdcData.S32;	

			// 得到0點 短路VTP
				fun_VTP_INIT();			//初始化VTP引脚
				SET_OPA_ZERO();			//设置OPA的零点
			// 切換到下一個狀態
				SDK_SYS.PublicMode ++;	
			}
			break;
		}	
		case 3: 
		{
			if(gbv_adc_Success)
			{	
				SDK_CAL.ADC_VtpZero.S16 = SDK_ADC.AdcData.S32;			
				
				fun_BAT_INIT();
				SDK_SYS.PublicMode ++;

				return	1;
			}
			break;
		}
		case 4: 
		{
			if(gbv_adc_Success)
			{	
				//	电池电压量测
				
				SDK_CAL.BAT_Buff.S16 = SDK_ADC.AdcData.S32;			
				fun_ADC_Disable();		//除能ADC
				
				SDK_SYS.PublicMode = 0;

				return	1;
			}
			break;
		}
		default:break;
	}	
	return 0;
}


u8	fun_get_VTP()  //获取VTP温度
{
	switch(SDK_SYS.PublicMode)
	{
		case	0:
		{
			fun_VTP_INIT();
			SET_OPA_VTP();		
			SDK_SYS.PublicMode ++;					
			break;
		}
		case	1:
		if(gbv_adc_Success)
		{	
			SDK_SYS.PublicMode 	= 0;
			SDK_CAL.ADC_Vtp.S16 	= SDK_ADC.AdcData.S32 - SDK_CAL.ADC_VtpZero.S16;

			return 1;	
		}
	}
	return	0;
}



u8	fun_get_VTP_CORR()  //获取VTP校准后的参数
{
	switch(SDK_SYS.PublicMode)
	{
		case	0:
		{
			fun_VTP_INIT();
			SET_OPA_ZERO();		
			SDK_SYS.PublicMode ++;					
			break;
		}
		case	1:
		if(gbv_adc_Success)
		{	

			SDK_CAL.ADC_VtpZero.S16 = SDK_ADC.AdcData.S32;

			SET_OPA_VTP();				
			fun_VTP_INIT();
			SDK_SYS.PublicMode ++;	
			break;	
		}
		case	2:
		if(gbv_adc_Success)
		{
			SDK_SYS.PublicMode = 0;	
			SDK_CAL.ADC_Vtp.S16 = SDK_ADC.AdcData.S32  - SDK_CAL.ADC_VtpZero.S16;
			return 1; 	
		}
		break;	
		default: break;	
	}
	return	0;
}



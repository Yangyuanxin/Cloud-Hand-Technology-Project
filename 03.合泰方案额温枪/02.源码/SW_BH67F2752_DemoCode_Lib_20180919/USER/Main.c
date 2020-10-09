
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	main.c 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/
#include "common.h"

#define	 Backlight   _pb6					//背光引脚
#define	 Backlight_HIGH()		  {Backlight = 0;}
#define	 Backlight_LOW()		  {Backlight = 1;}

void	fun_BacklightScan(void);
/********************************************************************
Function:	main
INPUT	:
OUTPUT	:	
NOTE	:   程序入口
********************************************************************/

void main()
{
	u32  ADC_Dep;
	double   ADC_Dep2;
	//判斷是否是上電復位或者是正常情況下的非正常復位
	//如果成立則執行上電初始化動作，反之則執行WDT溢出初始化
	if(_to ==0 || _pdf ==0)									// WDT	Reset
	{	//上電復位或者是正常情況下的非正常復位
		fun_Ram_CLear();									// CLR  RAM 		
		fun_Sys_Init();
		fun_Reset_Init();		
	}
	else							 						
	{	//WDT 溢出復位
		fun_Reset_Init();
	}
	fun_Uart_Init();
	fun_Test_Function();
	fun_LCD_Display_Power();	//上电初始化显示
/*	fun_BAT_INIT();*/
	
	while(1)
	{
	// 	清除看門狗	 		
		GCC_CLRWDT();	
	//	工作循环		
		SDK_TM.HaltTime = 0;	//Test 不要进入休眠	
			
		if(SDK_TM.HaltTime < HALT_TIME)		
		{
			if(_tb0f)	//	8ms中断标志位判断
			{
				_tb0f = 0;	//8ms标志位清零
				
				
				
				fun_SysTimeScan();
				fun_WorkModeScan();			//工作模式处理
				fun_Key_Scan();				//按键扫描处理
				fun_BeepScan();				//蜂鸣器处理
				fun_BacklightScan();		//背光处理
				
					
				
				//清除標誌位 ，在工作流程控制可使用gbv_500ms，由此地方統一清除
				if (gbv_500ms)
				{
					gbv_500ms = 0;
					
					/*fun_LCD_Display_Mode();*/
//					if(gbv_adc_Success)
//					{	
//						//	电池电压量测
//						SDK_CAL.BAT_Buff.S16 = SDK_ADC.AdcData.S32;	
//						
//						fun_Uart_SendASCII(SDK_CAL.BAT_Buff.S16);
//						LCD_DISPLAY_NUM(1,SDK_CAL.BAT_Buff.S16);
///*						fun_Uart_SendASCII(ADC_Dep);*/
//							
//					/*	fun_ADC_Disable();*/
//						
//					}
				}
			}
	//	ADC 輪詢掃描
			fun_ADC_Polling();
		}
	//	休眠模式
		else	
		{		
			fun_SysSleep();
		}		
	}			
	
}




/********************************************************************
Function:	fun_Test_Function
INPUT	:
OUTPUT	:	
NOTE	:   功能測試
********************************************************************/
void	fun_Test_Function()
{	
	#if(TEST_MODE == TEST_LCD)
	fun_LCD_Test();
	#endif
	#if(TEST_MODE == TEST_BEEP)
	fun_Beep_Test();
	#endif
	#if(TEST_MODE == TEST_PERI)
	fun_Peripherals_Test();
	#endif
	#if(TEST_MODE == TEST_UART)
	fun_Uart_Test();
	#endif
	#if(TEST_MODE == TEST_NTC)
	fun_ADC_NTC_Test();
	#endif
	#if(TEST_MODE == TEST_OPAZERO)
	fun_ADC_OPA_ZeroTest();
	#endif
	#if(TEST_MODE == TEST_OPAVTP)
	fun_ADC_OPA_VTPTest();
	#endif
}


/********************************************************************
Function:	fun_BacklightScan()
INPUT	:
OUTPUT	:	
NOTE	:   背光处理
********************************************************************/
void	fun_BacklightScan()
{	
		if(gbv_KeyStatus_L_4)
		{
			gbv_KeyStatus_L_4 = 0;
			fun_Uart_SendByte(0x11);
			Backlight = ~Backlight;
		}
}



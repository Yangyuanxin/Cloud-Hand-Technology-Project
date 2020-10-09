
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	adc.c 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#include "common.h"

ADC_define	SDK_ADC;

/********************************************************************
*	Function:	fun_VTP_INIT
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_VTP_INIT()
{
	SET_PGA_x64();
	SET_AGS_1();
	SET_VTP_CHANNAL();
	SET_ADC_SPEED_20HZ();
	fun_ADC_Power_Enable();
	fun_ADC_Enable();
}

/********************************************************************
*	Function:	fun_TEMP1_INIT
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_TEMP1_INIT()
{
	SET_PGA_x1();		//PGA DI+/DI- 差分通道输入增益选择
	SET_AGS_1();		//A/D 转换器 PGAOP/PGAON 差分输入信号增益选择
	SET_TEMP1_CHANNAL(); //选择正负端输入的选择位
	SET_ADC_SPEED_80HZ();	//选择A/D转换的采样率
	fun_ADC_Power_Enable();	//ADC的电源使能
	fun_ADC_Enable();	
}

/********************************************************************
*	Function:	fun_TEMP2_INIT
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_TEMP2_INIT()
{
	SET_PGA_x1();
	SET_AGS_1();
	SET_TEMP2_CHANNAL();	
	SET_ADC_SPEED_40HZ();
	fun_ADC_Power_Enable();
	fun_ADC_Enable();	
}

/********************************************************************
*	Function:	fun_BAT_INIT
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_BAT_INIT()
{
	SET_PGA_x1();   //PGA DI+/DI- 差分通道输入增益选择
	SET_AGS_1();	//A/D 转换器 PGAOP/PGAON 差分输入信号增益选择
	SET_BAT_CHANNAL();	//选择正负端输入的选择位
	SET_ADC_SPEED_40HZ(); //选择A/D转换的采样率
	fun_ADC_Power_Enable(); //ADC的电源使能
	fun_ADC_Enable();		
}


/********************************************************************
*	Function:	fun_ADC_Power_Enable
*	INPUT	:
*	OUTPUT	:
*	NOTE	:  	
********************************************************************/
void fun_ADC_Power_Enable()
{

	SET_AVDD_2_4();  //LDO 输出电压选择2.4V供电
	//以下參數在後續應用中不會變化所以在此進行設置
	SET_VGS_0();	//REFP/REFN 差分参考电压增益选择
//	SET_VREFBUFF_DISABLE();
	SET_ADC_FMCLOCK();	//A/D 转换器时钟源 f MCLK 设置
	SET_LDO_OPEN();		//打开LDO供电电压

}

/********************************************************************
*	Function:	fun_ADC_Power_Disable
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_Power_Disable()
{
//	SET_LDO_CLOSE();
}



/********************************************************************
*	Function:	fun_ADC_Enable
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_Enable()
{
	SDK_ADC.SampleCount = 0;
	gbv_adc_SampleMode = 0;
	gbv_adc_Success = 0;	
	SET_ADC_OPEN();			//打开ADC
	SET_ADC_NORMAL();		//设置正常模式，A/D 转换器休眠模式控制位
	SET_ADRST_DISABLE();	//A/D 转换器软件复位控制位，关闭复位
	SET_ADRST_ENABLE();		//A/D 转换器软件复位控制位，打开复位
	SET_ADRST_DISABLE();	//A/D 转换器软件复位控制位，关闭复位
	SET_ADCEOC_CLEAR();		//清除AD转换结束标志位

}

/********************************************************************
*	Function:	fun_ADC_Disable
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_Disable()
{
	SDK_ADC.SampleCount = 0;
	gbv_adc_SampleMode = 0;
	SET_ADC_SLEEP();
	SET_ADRST_DISABLE();
	SET_ADC_CLOSE();
	fun_ADC_Power_Disable();	
}

/********************************************************************
*	Function:	fun_ADC_Polling
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   獲取十筆數據，除去最大最小值求平均
********************************************************************/
void fun_filter_adc()
{	
	u8 i;
	s32 ls32v_adc_result = 0;
	s16 ls16v_adc_max;
	s16 ls16v_adc_min;
	
	ls16v_adc_max = SDK_ADC.AdcBuff[0];
	ls16v_adc_min = SDK_ADC.AdcBuff[0];
	
	for(i=0;i<10;i++)
	{
		ls32v_adc_result += SDK_ADC.AdcBuff[i];
		ls16v_adc_max = (ls16v_adc_max > SDK_ADC.AdcBuff[i]) ?  ls16v_adc_max : SDK_ADC.AdcBuff[i] ; 
		ls16v_adc_min = (ls16v_adc_min < SDK_ADC.AdcBuff[i]) ?  ls16v_adc_min : SDK_ADC.AdcBuff[i] ; 
	}
		
	SDK_ADC.AdcData.S32 = (u16)((ls32v_adc_result - ls16v_adc_min - ls16v_adc_max) >> 3);
}

/********************************************************************
*	Function:	fun_ADC_Polling
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_Polling()
{

	if(_eoc!=0)		//判断ADC转换结束没有。1：转换结束，0：未结束
	{
		//鎖存數據
		SET_ADCLATCH_ENABLE();   //使能A/D 转换数据锁存功能控制
		//將數據轉換為32bit 的有符號數據
		SDK_ADC.AdcData.U32 = 0;
		SDK_ADC.AdcData.byte.byte0	= _adrl;		//读取低8位数据
		SDK_ADC.AdcData.byte.byte1	= _adrm;		//读取中8位数据
		SDK_ADC.AdcData.byte.byte2	= _adrh;		//读取高8位数据
		if(SDK_ADC.AdcData.bits.b23)             	//判断最后1位是不是为1，如果是将剩余的最后8位补为1
		{
			SDK_ADC.AdcData.byte.byte3 = 0xff;
		}

		#if(TEST_MODE==mac_Test_On)
		// 測試模式 
		SDK_ADC.AdcData.S32 >>= 8;
		#else
		// 取 24bit adc 中的 16bit 數據作為有效數據
		SDK_ADC.AdcData.S32 >>= 8;
		#endif
		SET_ADCLATCH_DISABLE();		//除能A/D 转换数据锁存功能控制
		SET_ADCEOC_CLEAR();			//清除A/D 转换结束标志
		
		//採樣計數
		SDK_ADC.SampleCount++;
		if(gbv_adc_SampleMode==0)
		{
			//丟棄前三筆數據
			if(SDK_ADC.SampleCount>3)
			{
				SDK_ADC.SampleCount=0;
				gbv_adc_SampleMode = 1;

			}
		}
		else if(gbv_adc_SampleMode==1)
		{	
			
			
		/*	array_uart_txbuff[0] = 0x55;
			array_uart_txbuff[1] = 0x00;			
			array_uart_txbuff[2] = SDK_ADC.AdcData.byte.byte0;			
			array_uart_txbuff[3] = SDK_ADC.AdcData.byte.byte1;			
			array_uart_txbuff[4] = SDK_ADC.AdcData.byte.byte2;						
			array_uart_txbuff[5] = array_uart_txbuff[0]+array_uart_txbuff[1]+array_uart_txbuff[2]+array_uart_txbuff[3]+array_uart_txbuff[4];						
			gu8v_tx_guide = 0;
			_txr_rxr = array_uart_txbuff[gu8v_tx_guide];*/
		
			
//			SDK_ADC.AdcBuff[SDK_ADC.SampleCount-1] = SDK_ADC.AdcData.S32;
//			if(SDK_ADC.SampleCount>=10)
//			{
//
//				fun_filter_adc();
//				gbv_adc_Success		= 1;
//				SDK_ADC.SampleCount = 0;
//				gbv_adc_SampleMode 	= 0;
//
//			}

			SDK_ADC.SampleCount = 0;
			gbv_adc_SampleMode 	= 0;
			gbv_adc_Success		= 1;	
			
		//	GCC_DELAY(1000);
		}	 
	}		
}

/********************************************************************
*	Function:	fun_ADC_ZeroTest
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_ZeroTest()
{
//	fun_ADC_Zero();
	fun_Uart_Init();
	while(1)
	{
		GCC_CLRWDT();//清除看門狗	
		fun_ADC_Polling();
		
		
	}

}

/********************************************************************
*	Function:	fun_ADC_NTC_Test
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_NTC_Test()
{
	fun_TEMP2_INIT();
	fun_Uart_Init();
		_flms2 = 0;
		_flms1 = 1;				
		_flms0 = 0;					
		
		
		array_uart_rxbuff[1] = 7;
		_pgac0 &= 0b11111000;
		_pgac0 += array_uart_rxbuff[1];		
	
	while(1)
	{
		GCC_CLRWDT();//清除看門狗	
		fun_ADC_Polling();
		fun_Uart_Handle();
		
	}

}

/********************************************************************
*	Function:	fun_ADC_OPA_ZeroTest
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_OPA_ZeroTest()
{
	fun_VTP_INIT();
	fun_Uart_Init();
	SET_OPA_ZERO();
	while(1)
	{
		GCC_CLRWDT();//清除看門狗	
		fun_ADC_Polling();
		
	}

}
/********************************************************************
*	Function:	fun_ADC_OPA_VTPTest
*	INPUT	:
*	OUTPUT	:	
*	NOTE	:   
********************************************************************/
void fun_ADC_OPA_VTPTest()
{
	fun_VTP_INIT();
	fun_Uart_Init();
	SET_OPA_VTP();
	while(1)
	{
		GCC_CLRWDT();//清除看門狗	
		fun_ADC_Polling();
		
	}

}

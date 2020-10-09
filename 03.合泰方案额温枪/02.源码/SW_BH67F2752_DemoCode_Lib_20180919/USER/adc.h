
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	adc.h	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#ifndef _ADC_H_
#define _ADC_H_

typedef struct
{
	u8	            SampleCount;
	s16      	    AdcBuff[10];
	__32_type		AdcData;
	__8_type	    Status;
}ADC_define;

extern	ADC_define	SDK_ADC;

#define	gbv_adc_SampleMode		SDK_ADC.Status.bits.b0
#define	gbv_adc_Success			SDK_ADC.Status.bits.b1


void 	fun_ADC_Polling();
void 	fun_ADC_Enable();
void 	fun_ADC_Disable();
void 	fun_VTP_INIT();
void 	fun_TEMP1_INIT();
void 	fun_TEMP2_INIT();
void 	fun_BAT_INIT();
void 	fun_ADC_Power_Enable();
void 	fun_ADC_Power_Disable();
void	fun_ADC_NTC_Test();
void 	fun_ADC_OPA_VTPTest();
void	fun_ADC_OPA_ZeroTest();


//	|--------------------------------------------------------------------------|
//	|------------------------------_pwrc  -------------------------------------|
//	|BIT	|7		|6		|5		|4		|3		|2		|1		|0	   ----|		
//	|--------------------------------------------------------------------------|
//	|Name	|ENLDO	|ENVCM	|—		|—		|—		|—		|TnRP1	|LDOVS0 ---|
//	|--------------------------------------------------------------------------|
//	Bit 7 ENLDO：LDO 
//		0：除能1：使能
//	Bit 6~3 未定義，讀為“0”
//	Bit 2 LDO Bypass
//	Bit 1~0 LDOVS1~LDOVS0：LDO 輸出電壓選擇位
//		00：2.4V 01：2.6V 10：2.9V 11：3.3V	
#define		SET_AVDD_2_4()			_ldovs1 = 0;_ldovs0 = 0;
#define		SET_AVDD_2_6()			_ldovs1 = 0;_ldovs0 = 1;
#define		SET_AVDD_2_9()			_ldovs1 = 1;_ldovs0 = 0;
#define		SET_AVDD_3_3()			_ldovs1 = 1;_ldovs0 = 1;
#define		SET_LDO_OPEN()			_ldoen	= 1;
#define		SET_LDO_CLOSE()			_ldoen	= 0;

//	|--------------------------------------------------------------------------|
//	|------------------------------_pgac0 -------------------------------------|
//	|BIT	|7		|6		|5		|4		|3		|2		|1		|0	   ----|		
//	|--------------------------------------------------------------------------|
//	|Name	|—		|VGS1	|VGS0	|AGS1	|AGS0 	|PGS2	|PGS1	|PGS0 	---|
//	|--------------------------------------------------------------------------|
// Bit 7 未使用，讀為“0”
// Bit 6~5 VGS1~VGS0：VREF 增益選擇位
// 	00：1	01：1/2	10：1/4	11：保留
// Bit 4~3 AGS1~AGS0：ADC 增益選擇位
// 	00：1	01：2	10：4	11：8
// Bit 2~0 PGS2~PGS0：PGA 增益選擇位
// 	000：1	001：2	010：4	011：8	
// 	100：38	101：32	110：64	111：128
#define		SET_VGS_0()				_vgs1 = 0;_vgs0 = 0;
#define		SET_VGS_1C2()			_vgs1 = 0;_vgs0 = 1;
#define		SET_VGS_1C4()			_vgs1 = 1;_vgs0 = 0;
#define		SET_AGS_1()				_ags1 = 0;_ags0 = 0;
#define		SET_AGS_2()				_ags1 = 0;_ags0 = 1;
#define		SET_AGS_4()				_ags1 = 1;_ags0 = 0;
#define		SET_AGS_8()				_ags1 = 1;_ags0 = 1;
#define		SET_PGA_x1()			_pgs2 = 0;_pgs1 = 0;_pgs0 = 0;
#define		SET_PGA_x2()			_pgs2 = 0;_pgs1 = 0;_pgs0 = 1;
#define		SET_PGA_x4()			_pgs2 = 0;_pgs1 = 1;_pgs0 = 0;
#define		SET_PGA_x8()			_pgs2 = 0;_pgs1 = 1;_pgs0 = 1;
#define		SET_PGA_x16()			_pgs2 = 1;_pgs1 = 0;_pgs0 = 0;
#define		SET_PGA_x32()			_pgs2 = 1;_pgs1 = 0;_pgs0 = 1;
#define		SET_PGA_x64()			_pgs2 = 1;_pgs1 = 1;_pgs0 = 0;
#define		SET_PGA_x128()			_pgs2 = 1;_pgs1 = 1;_pgs0 = 1;

//	|--------------------------------------------------------------------------|
//	|------------------------------_pgac1 -------------------------------------|
//	|BIT	|7		|6		|5		|4		|3		|2		|1		|0	   ----|		
//	|--------------------------------------------------------------------------|
//	|Name	|VCMS	|INIS	|—		|—		|DCSET2 |DCSET1	|DCSET0	|—	 	---|
//	|--------------------------------------------------------------------------|
//	Bit 7 未使用，讀為“0”
//	Bit 6 INIS：IN1 和IN2 輸入端連接控制位
//	Bit 5~4 INX1~INX0:
//	Bit 3~1 DCSET2~DCSET0：DI+/DI- 差分輸入偏差調整位
//	Bit 0 未使用，“0”

#define		SET_DI_IN1_IN2()	_inx1 = 0;_inx0 = 0;
#define		SET_DI_IN2()		_inx1 = 0;_inx0 = 1;
#define		SET_DI_IN1()		_inx1 = 1;_inx0 = 0;
#define		SET_DI_IN2_IN1()	_inx1 = 1;_inx0 = 1;

//	|--------------------------------------------------------------------------|
//	|------------------------------—_pgacs-------------------------------------|
//	|BIT	|7		|6		|5		|4		|3		|2		|1		|0	   ----|		
//	|--------------------------------------------------------------------------|
//	|Name	|—		|—		|CHSN2	|CHSN1	|CHSN0	|CHSP2	|CHSP1	|CHSP0 	---|
//	|--------------------------------------------------------------------------|	  
//	Bit 7~6 未使用，讀為“0”
//	Bit 5~3 CHSN2~CHSN0：PGA 反向輸入端選擇位
//		000：AN1 	001：AN3 
//		010：AN5 	011：AN7 
//		100：Vin/5 	101：LNOPO 
//		110：VCM 	111：溫度傳感器Vtson
//	Bit 2~0 CHSP2~CHSP0：PGA 正向輸入端選擇位
//		000：AN0 	001：AN2 
//		010：AN4 	011：AN6 
//		100：Unused 	101：Unused 
//		110：VCM 	111：溫度傳感器Vtsop


#define		SET_ADC_AN0_VCM()		_pgacs = 0b01110000;
#define		SET_ADC_AN2_VCM()		_pgacs = 0b01110001;
#define		SET_ADC_AN4_VCM()		_pgacs = 0b01110010;
#define		SET_ADC_AN6_VCM()		_pgacs = 0b01110011;
#define		SET_ADC_AN1_VCM()		_pgacs = 0b00010110;
#define		SET_ADC_AN3_VCM()		_pgacs = 0b00100110;
#define		SET_ADC_AN5_VCM()		_pgacs = 0b00110110;
#define		SET_ADC_AN7_VCM()		_pgacs = 0b01000110;
#define		SET_ADC_OPO_VCM()		_pgacs = 0b01110101;
#define		SET_ADC_1C6VDD_VCM()	_pgacs = 0b01010110;
#define		SET_ADC_VCM_VCM()		_pgacs = 0b01110110;
#define		SET_ADC_1C6VDD_VCM()	_pgacs = 0b01010110;


#define		SET_VTP_CHANNAL()		SET_ADC_OPO_VCM(); SET_DI_IN1_IN2();
#define		SET_TEMP1_CHANNAL()		SET_ADC_AN5_VCM(); SET_DI_IN1_IN2();
#define		SET_TEMP2_CHANNAL()		SET_ADC_AN4_VCM(); SET_DI_IN2_IN1();
#define		SET_BAT_CHANNAL()		SET_ADC_1C6VDD_VCM(); SET_DI_IN2_IN1();

//	|--------------------------------------------------------------------------|
//	|------------------------------—_adcr0-------------------------------------|
//	|BIT	|7		|6		|5		|4		|3		|2		|1		|0	   ----|		
//	|--------------------------------------------------------------------------|
//	|Name	|ADRST	|ADSLP	|ADOFF	|ADOR2	|ADOR1	|ADOR0	|—		|_vrefs ---|	
//	|--------------------------------------------------------------------------|   
// Bit 7 ADRST：ADC 軟件復位控制
// 	0：除能	1：使能
// Bit 6 ADSLP：ADC 休眠模式控制位
// 	0：正常模式	1：休眠模式
// Bit 5 ADOFF：ADC 
// 	0：ADC on 	1：ADC off
// Bit 4~2 ADOR2~ADOR0：輸出數據傳輸率控制
// 	000：OSR=16384
// 	001：OSR=8192
// 	010：OSR=4096
// 	011：OSR=2048
// 	100：OSR=1024
// 	101：OSR=512
// 	110：OSR=256
// 	111：OSR=128
// Bit 1 未定義，讀為“0”
// Bit 0 VREFS：選擇ADC 參考電壓
// 	0：內部(VCM, AVSS)	1：外部參考電壓輸入(VREFP, VREFN)		

#define SET_OSR_16384() 		_ador2 = 0;_ador1 = 0;_ador0 = 0;
#define SET_OSR_8192() 			_ador2 = 0;_ador1 = 0;_ador0 = 1;
#define SET_OSR_4096() 			_ador2 = 0;_ador1 = 1;_ador0 = 0;
#define SET_OSR_2048() 			_ador2 = 0;_ador1 = 1;_ador0 = 1;
#define SET_OSR_1024() 			_ador2 = 1;_ador1 = 0;_ador0 = 0;
#define SET_OSR_512() 			_ador2 = 1;_ador1 = 0;_ador0 = 1;
#define SET_OSR_256() 			_ador2 = 1;_ador1 = 1;_ador0 = 0;
#define SET_OSR_128() 			_ador2 = 1;_ador1 = 1;_ador0 = 1;

#define SET_ADRST_ENABLE()		_adrst = 1;
#define SET_ADRST_DISABLE()		_adrst = 0;
#define SET_ADC_SLEEP()			_adslp = 1;
#define SET_ADC_NORMAL()		_adslp = 0;
#define SET_ADC_OPEN()			_adoff = 0;
#define SET_ADC_CLOSE()			_adoff = 1;
#define SET_VREF_VCM_AVSS()		_vrefs = 0;
#define SET_VREF_VREFP_VREFN()	_vrefs = 1;


//	|--------------------------------------------------------------------------|
//	|------------------------------_adcr1 -------------------------------------|
//	|BIT	|7		|6		|5		|4		|3		|2		|1		|0	   ----|		
//	|--------------------------------------------------------------------------|
//	|Name	|FLMS2	|FLMS1	|FLMS0	|VRBUFN	|VRBUFP	|ADCDL	|EOC	|---	---|
//	|--------------------------------------------------------------------------|

#define	SET_CHOP2_FMCLK30()		_flms2 = 0;	_flms1 = 0;_flms0 = 0;
#define	SET_CHOP2_FMCLK12()		_flms2 = 0;	_flms1 = 1;_flms0 = 0;
#define	SET_CHOP1_FMCLK30()		_flms2 = 1;	_flms1 = 0;_flms0 = 0;
#define	SET_CHOP1_FMCLK12()		_flms2 = 1;	_flms1 = 1;_flms0 = 0;
//#define	SET_VREFBUFF_ENABLE()	_vrbufp = 1;_vrbufn = 1;
//#define	SET_VREFBUFF_DISABLE()	_vrbufp = 0;_vrbufn = 0;	
#define	SET_ADCLATCH_ENABLE()	_adcdl = 1;	
#define	SET_ADCLATCH_DISABLE()	_adcdl = 0;	
#define	SET_ADCEOC_CLEAR()		_eoc = 0;	
//	|---------------------------------------------------------------------------|
//	|	ADC DATA 数据转换率														|
//	|---------------------------------------------------------------------------|
//		eg.	fmclk = 4MHz,OSR = 4096,CHOP = 2,，fadck = fmclk / 12					
//			速率=fmclk/(12*2*4096)=40Hz，约25ms
//	|---------------------------------------------------------------------------|

//======================================================================
#if(SET_SYSTEM_CLOCK ==	CLOCK_4M)
//======================================================================
#define		SET_ADC_SPEED_10HZ()	SET_CHOP2_FMCLK12();SET_OSR_16384();
#define		SET_ADC_SPEED_20HZ()	SET_CHOP2_FMCLK12();SET_OSR_8192();
#define		SET_ADC_SPEED_40HZ()	SET_CHOP2_FMCLK12();SET_OSR_4096();
#define		SET_ADC_SPEED_80HZ()	SET_CHOP2_FMCLK12();SET_OSR_2048();
#define		SET_ADC_SPEED_160HZ()	SET_CHOP2_FMCLK12();SET_OSR_1024();
#define		SET_ADC_SPEED_320HZ()	SET_CHOP2_FMCLK12();SET_OSR_512();
#define		SET_ADC_SPEED_640HZ()	SET_CHOP2_FMCLK12();SET_OSR_256();
#define		SET_ADC_SPEED_1280HZ()	SET_CHOP2_FMCLK12();SET_OSR_128();										
#endif
//======================================================================
#if(SET_SYSTEM_CLOCK ==	CLOCK_8M)
//======================================================================
#define		SET_ADC_SPEED_20HZ()	SET_CHOP2_FMCLK12();SET_OSR_16384();
#define		SET_ADC_SPEED_20HZ()	SET_CHOP2_FMCLK12();SET_OSR_16384();
#define		SET_ADC_SPEED_40HZ()	SET_CHOP2_FMCLK12();SET_OSR_8192();
#define		SET_ADC_SPEED_80HZ()	SET_CHOP2_FMCLK12();SET_OSR_4096();
#define		SET_ADC_SPEED_160HZ()	SET_CHOP2_FMCLK12();SET_OSR_2048();
#define		SET_ADC_SPEED_320HZ()	SET_CHOP2_FMCLK12();SET_OSR_1024();
#define		SET_ADC_SPEED_640HZ()	SET_CHOP2_FMCLK12();SET_OSR_512();
#define		SET_ADC_SPEED_1280HZ()	SET_CHOP2_FMCLK12();SET_OSR_256();	
#define		SET_ADC_SPEED_2560HZ()	SET_CHOP2_FMCLK12();SET_OSR_128();		
#endif	
//======================================================================
#if(SET_SYSTEM_CLOCK ==	CLOCK_12M)
//======================================================================
#define		SET_ADC_SPEED_40HZ()	SET_CHOP2_FMCLK12();SET_OSR_16384();
#define		SET_ADC_SPEED_80HZ()	SET_CHOP2_FMCLK12();SET_OSR_8192();
#define		SET_ADC_SPEED_160HZ()	SET_CHOP2_FMCLK12();SET_OSR_4096();	
#define		SET_ADC_SPEED_320HZ()	SET_CHOP2_FMCLK12();SET_OSR_2048();
#define		SET_ADC_SPEED_640HZ()	SET_CHOP2_FMCLK12();SET_OSR_1024();
#define		SET_ADC_SPEED_1280HZ()	SET_CHOP2_FMCLK12();SET_OSR_512();
#define		SET_ADC_SPEED_2560HZ()	SET_CHOP2_FMCLK12();SET_OSR_256();
#endif
//======================================================================
			
//	|--------------------------------------------------------------------------|
//	|------------------------------_adcs --------------------------------------|
//	|BIT	|7		|6		|5		|4		|3		|2		|1		|0	   ----|		
//	|--------------------------------------------------------------------------|
//	|Name	|— 		|— 		|— 		|ADCK4	|ADCK3	|ADCK2	|ADCK1	|ADCK0	---|
//	|--------------------------------------------------------------------------|	
// Bit 7~5 未定義，讀為“0”4~0 ADCK4~ADCK0：選擇ADC時鐘源(FMCLK)
// 	00000~11110：fSYS/2/(ADCK[4:0]+1)	11111：fSYS
// 
#define		SET_ADC_FMCLOCK()			_adcs = 0x1f; 
#define		SET_ADC_FMCLOCK_C2()		_adcs = 0x0; 

//=====================
//		OP設置
//=====================

//	|--------------------------------------------------------------------------|
//	|------------------------------_dsopc -------------------------------------|
//	|BIT	|7		|6		|5		|4		|3		|2		|1		|0	   ----|		
//	|--------------------------------------------------------------------------|
//	|Name	|DSOPEN	|DSOPO	|DSOPBW	|---	|OPN1	|OPN0	|OPP1	|OPP0	---|
//	|--------------------------------------------------------------------------|

//	Bit 7 DSOPEN： OPA 使能或除能控制位
//		0： 除能
//		1： 使能
//	Bit 6~4 Unused
//	Bit 3~2 OPN1~OPN0： 反相输入端 OPI- 输入信号选择位
//		00： LNOPO
//		01： AN3
//		10： AN5
//		11： AN7
//	Bit 1~0 OPP1~OPP0： 正向输入端 OPI+ 输入信号选择位
//		00： AN0
//		01： AN2
//		10： AN4
//		11： VCM


#define	SET_OPA_OPEN()			_dsopen = 1;
#define	SET_OPA_CLOSE()			_dsopen = 0;

#define	SET_OPA_OPN_LNOPO()		_opn1 = 0;_opn0 = 0;
#define	SET_OPA_OPN_AN3()		_opn1 = 0;_opn0 = 1;
#define	SET_OPA_OPN_AN5()		_opn1 = 1;_opn0 = 0;
#define	SET_OPA_OPN_AN7()		_opn1 = 1;_opn0 = 1;

#define	SET_OPA_OPP_AN0()		_opp1 = 0;_opp0 = 0;
#define	SET_OPA_OPP_AN2()		_opp1 = 0;_opp0 = 1;
#define	SET_OPA_OPP_AN4()		_opp1 = 1;_opp0 = 0;
#define	SET_OPA_OPP_VCM()		_opp1 = 1;_opp0 = 1;

#define	SET_OPA_VTP()			SET_OPA_OPEN();SET_OPA_OPP_AN0();SET_OPA_OPN_AN3();
#define	SET_OPA_ZERO()			SET_OPA_OPEN();SET_OPA_OPP_VCM();SET_OPA_OPN_AN3();



#endif


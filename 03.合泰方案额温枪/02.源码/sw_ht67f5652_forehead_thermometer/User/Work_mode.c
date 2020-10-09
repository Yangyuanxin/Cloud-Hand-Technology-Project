
/*************************************************************************************/
//**	Copyright	:	2016 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	Work_mode.c 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	HT67F5652													**
//**	Author   	: 	Leo															**
//**	Date     	: 	2016/10/28													**
//**	Version  	: 	V00															**
//**	History  	:																**
/*************************************************************************************/

#include "..\User\CONFIG.h"

volatile	U8	gu8v_work_mode;
volatile	U8	gu8v_time_count;
volatile	U8	gu8v_correction_mode;
volatile	U8	gu8v_obj_temp_mode;

volatile	U8	gu8v_c_count;
volatile	U8	gu8v_c_times;
volatile	U16	gu16v_set_temperature;
volatile	U16	gu16v_set_temperature1;

volatile	__16_type	gu16v_last_ntc;
volatile	__16_type	gu16v_last_vtp;


volatile	__16_type	gu16v_adc_res2_buff;

volatile	__16_type	gu16v_temp_buff[4];
volatile    __16_type	gu16v_temp_last;
volatile    U8 			gu8v_temp_count;
volatile    U8 			gu8v_temp_count_m;
volatile    U8 			gu8v_temp_i;
volatile    U8 			gu8v_temp_test;
volatile    U16 		gu16v_temp_filter;


volatile	__16_type	gu16v_NTC_buff[6];

volatile    __16_type	gu16_NTC_res;
volatile	U8 gu8_ntc_mode;
volatile    U8 gu8v_key_enable;
volatile    U8 gu8v_key_old;
volatile	U16	gu8v_5s_times;
volatile	U8 gu8v_display_mode_old;



__byte_type	gu8v_dispaly_mode;

#define	gbv_LCD_Boby		gu8v_dispaly_mode.bits.b0	
#define	gbv_LCD_Surface		gu8v_dispaly_mode.bits.b1	
#define	gbv_LCD_Room		gu8v_dispaly_mode.bits.b2	





/********************************************************************
Function:	fun_Work_mode
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Work_mode(void)
{
	switch(gu8v_work_mode)
	{
		case	0:
	//	gbv_on_sys = 1;
		if(gu8v_key_enable)	
		{
			if(gbv_on_sys)
			{
				gu8v_halt_time = 0;
				gbv_on_sys = 0;
				gu8v_work_mode = MEMS_MODE;	
				if(gu8v_dispaly_mode.U8== 0b10000000) gu8v_dispaly_mode.U8 = 0b00000010;
				gu8v_obj_temp_mode = 0;
				gu8_ntc_mode = 0;
			}
		}
		//  连续按五次按鍵 進入校正模式		
		//-------------------------------------------//
		if(gbv_KeyDown_Switch)
		{
			gbv_KeyDown_Switch = 0;
			if(gu8v_c_times ==0)
			{
				gu8v_c_count = 0;
			}
			gu8v_c_times ++;
		}	
		gu8v_c_count ++;
		if(gu8v_c_count>=250)
		{
			gu8v_c_count = 0;
			if(gu8v_c_times>=5)
			{ 
				gu8v_work_mode = CORRECTION_MODE;					
			}
			gu8v_c_times = 0;
		}
		// 	切換量測模式
		//-------------------------------------------//
		if(gbv_KeyDown_Room)
		{
			gu8v_dispaly_mode.U8 = 0b00000001;
		}
		if(gbv_KeyDown_Surface	)
		{
			gu8v_dispaly_mode.U8 = 0b00000010;
		}		
		if(gbv_KeyDown_Body)
		{
			gu8v_dispaly_mode.U8 = 0b00000100;
		}
//		if(gbv_key_beep_down)
//		{
//			gu8v_dispaly_mode.U8 = 0b00001000;
//		}	
		if(gbv_KeyUp_Beep)
		{
//			gu8v_dispaly_mode.U8 = 0b00010000;
		}	
		// 	喚醒的初始操作
		//-------------------------------------------//
		if(gu8v_key_enable)
		{
			fun_LCD_Display_Mode();
			
			
			if((gu16v_current_temp.S16 == 0 ) || (gu8v_display_mode_old != gu8v_dispaly_mode.U8))
			{
				fun_LCD_NUM(1,LU8C_NUM3_ADDR,16);		
				fun_LCD_NUM(1,LU8C_NUM2_ADDR,16);	
				fun_LCD_NUM(1,LU8C_NUM1_ADDR,16);	
			}
			if(gu8v_display_mode_old != gu8v_dispaly_mode.U8)
			{
				fun_Beep_init(mac_beep_1);
			}
		
			gu8v_display_mode_old = gu8v_dispaly_mode.U8;
		}
		
		// 	1S採集一次ntc
		//-------------------------------------------//	
		if(gbv_1s)
		{
			if(fun_get_NTC())
			{
				gbv_1s = 0;
				fun_RNTC_Calculate();
				gu16_NTC_res.S16 = gu16v_adc_res.S16;
				gu8v_key_enable = 1;
			
			}
		}
		
		if((gu8v_key_enable != gu8v_key_old) && (gu8v_key_enable == 1))
		{
			gu8v_key_old = gu8v_key_enable;
			fun_LCD_FULL(0);
	
		}
				
		break;
		
		case	MEMS_MODE:


		if(fun_get_VTP())
		{
			fun_Vtp_Calculate();
			
			gu16v_adc_res.S16 = gu16_NTC_res.S16;
			fun_Vsen_Calculate();
					
			fun_Tobj_Calculate();
			fun_Temp_SurfaceToBody();	
			fun_LCD_CLR_TEMP();
			fun_LCD_SET_C();
			fun_LCD_Display_Data();	
					
			gu8v_work_mode = 0;
			gu8_ntc_mode = 0;
		}
		else
		{
		/*	if(gbv_500ms_reversal)
			{
			//	fun_LCD_FULL(0);	
			//	fun_LCD_CLR_TEMP();		
			}
			else*/
			{
				fun_LCD_NUM(1,LU8C_NUM3_ADDR,16);		
				fun_LCD_NUM(1,LU8C_NUM2_ADDR,16);	
				fun_LCD_NUM(1,LU8C_NUM1_ADDR,16);
				
			//	fun_LCD_DISPLAY_NUM(888);			
			}
			
		}
		
	
		break;		
		case	CORRECTION_MODE:
		
		fun_CORRECTION_MODE_FUN();
		if(gbv_KeyDown_Switch)
		{
			//退出校正模式
		
		}	
		break;		
		
//		case 3:
//		{
//			fun_LCD_SET_C();
//			gu8v_5s_times++;
//			if(gu8v_5s_times >= 500)
//			{
//				gu8v_5s_times = 0;
//				gu8v_work_mode = 0;
//				gu8v_Key_status.U8 = 0;
//				fun_Beep_init(mac_beep_1);
//			}
//		}
//		break;
		default:	break;			
		
	}
}
void	fun_CORRECTION_MODE_FUN()
{
	switch(gu8v_correction_mode)
	{
		case	0://進入溫度測量
		{
			gu8v_temp_count = 0;
			fun_LCD_NUM(2,LU8C_NUM3_ADDR,11);	//顯示Corr	
			fun_LCD_NUM(2,LU8C_NUM2_ADDR,11);	
			fun_LCD_NUM(2,LU8C_NUM1_ADDR,14);
			fun_LCD_NUM(2,LU8C_NUM0_ADDR,17);				
			if(gbv_on_sys)
			{
				gbv_on_sys = 0;			
				gu8v_correction_mode = 1;
			}
			if(gbv_KeyDown_Switch)//退出校正
			{
				gu8v_correction_mode = 0;	
				gu8v_work_mode = 0;
				fun_LCD_FULL(0);
			}	
			break;
		}
		case	1:
		{
			if(fun_get_VTP())//記錄第一個溫度點的參數	
			{	
				gu8v_correction_mode = 2;			
				fun_Vtp_Calculate();
				gu16v_vtp_buff.S16 = gu16v_vtp.S16;
				gu16v_adc_res2_buff.S16 = gu16_NTC_res.S16;
				fun_Beep_init(mac_beep_1);
			}
			fun_LCD_NUM(2,LU8C_NUM0_ADDR,18);
			fun_LCD_DISPLAY_NUM(111);	
			if(gbv_KeyDown_Switch)//退出校正
			{
				gu8v_correction_mode = 0;	
				gu8v_work_mode = 0;
				fun_LCD_NUM(1,LU8C_NUM3_ADDR,16);		
				fun_LCD_NUM(1,LU8C_NUM2_ADDR,16);	
				fun_LCD_NUM(1,LU8C_NUM1_ADDR,16);	
			}

			break;	
		}	
		case	2:
		{
			if(gbv_on_sys)
			{
				gbv_on_sys = 0;			
				gu8v_correction_mode = 3;
			}		
			fun_LCD_DISPLAY_NUM(222);
			
			if(gbv_KeyDown_Switch)//退出校正
			{
				gu8v_correction_mode = 0;	
				gu8v_work_mode = 0;
				fun_LCD_NUM(1,LU8C_NUM3_ADDR,16);		
				fun_LCD_NUM(1,LU8C_NUM2_ADDR,16);	
				fun_LCD_NUM(1,LU8C_NUM1_ADDR,16);	
			}				
			break;
		}
		case	3:
		{
			if(fun_get_VTP())
			{
				//記錄第二個溫度點的參數
				gu8v_correction_mode ++;
				fun_Vtp_Calculate(); //校正完畢計算出當前的校正參數
				gu16v_vtp_buff2.S16 = gu16v_vtp.S16;	
				gu16v_set_temperature = 370;
				fun_Beep_init(mac_beep_1);
			}
		
		
		break;
		}
		case	4:
		//input current temperature1 25-43
		if(gbv_on_sys)
		{
			gbv_on_sys = 0;
//			gu16v_set_temperature++;
//			if(gu16v_set_temperature>430)
//			{
//				gu16v_set_temperature = 250;				
//			}	
		}
		else
		{
			if(gbv_on_KeyPress)
			{
				if(gbv_10ms)
				{
					gbv_10ms = 0;
					gu16v_set_temperature++;
					if(gu16v_set_temperature>500)
					{
						gu16v_set_temperature = 100;
										
					}						
				}

			}			
		}
		if(gbv_KeyDown_Switch)
		{
			gu8v_correction_mode ++;	
			gu16v_set_temperature1 = 270;
		}
		
				
		fun_LCD_DISPLAY_NUM(gu16v_set_temperature);	
				
		
		break;		
		case	5:
		if(gbv_on_sys)
		{
			gbv_on_sys = 0;
//			gu16v_set_temperature++;
//			if(gu16v_set_temperature>430)
//			{
//				gu16v_set_temperature = 250;				
//			}	
		}
		else
		{
			if(gbv_on_KeyPress)
			{
				if(gbv_10ms)
				{
					gbv_10ms = 0;
					gu16v_set_temperature1++;
					if(gu16v_set_temperature1>500)
					{
						gu16v_set_temperature1 = 100;				
					}
				}
			}
		}
		if(gbv_KeyDown_Switch)
		{
			gu8v_correction_mode ++;	
		}
		
				
		fun_LCD_DISPLAY_NUM(gu16v_set_temperature1);				
					
		break;			
		case	6:
//			gu16v_vtp_buff.S16 = 3405;
//			gu16v_vtp.S16 = 1023;
//			gu16v_adc_res2.S16	= 8755;	
//			gu16v_adc_res2_buff.S16	= 8755;	
			gu16v_tcf.S16 = 10000;	
			fun_CORRECTION_Calculate(gu16v_set_temperature,gu16v_set_temperature1);			
			
			fun_Vsen_Calculate();
			fun_TCF_Calculate();				
			fun_CORRECTION_Calculate(gu16v_set_temperature,gu16v_set_temperature1);				

	//		if(gu16v_correction_K2.U16>9000&&gu16v_correction_K2.U16<11000)
			{

			//將數據存入 eeprom 中
			fun_EEprom_Write(0,0x55);			
			fun_EEprom_Write(1,gu16v_correction_K1.byte.byte0);
			fun_EEprom_Write(2,gu16v_correction_K1.byte.byte1);
			fun_EEprom_Write(3,gu16v_correction_K2.byte.byte0);
			fun_EEprom_Write(4,gu16v_correction_K2.byte.byte1);			//
			fun_EEprom_Write(5,gu16v_vtp_buff.byte.byte0);
			fun_EEprom_Write(6,gu16v_vtp_buff.byte.byte1);
			fun_EEprom_Write(7,gu16v_vtp.byte.byte0);
			fun_EEprom_Write(8,gu16v_vtp.byte.byte1);			//		
			fun_EEprom_Write(9,gu16v_adc_res.byte.byte0);
			fun_EEprom_Write(10,gu16v_adc_res.byte.byte1);			//	
			fun_EEprom_Write(11,gu16v_adc_res2_buff.byte.byte0);
			fun_EEprom_Write(12,gu16v_adc_res2_buff.byte.byte1);			//	
								
			fun_LCD_FULL(0);
			gu8v_correction_mode = 0;	
			gu8v_work_mode = 0;	
			fun_Beep_init(mac_beep_1);
			}
		break;			
		
	}
	
}





U8 fun_get_NTC()
{
	switch(gu8_ntc_mode)
	{
		case 0:
		{	
			fun_TEMP1_INIT();
			gu8_ntc_mode++;
			break;
		}
		case 1:
		{
			if(gbv_adc_success)
			{	
				gu16v_adc_res.S16 = ~gu32v_adc_data.S32 +1;	
				fun_ADC_Disable();
				gu8_ntc_mode = 0;
				
				return	1;
							
			//	fun_TEMP2_INIT();
			//	lu8_ntc_mode ++;	
			}
			break;
		}
	/*	case 2:
		{
			if(gbv_adc_success)
			{	
				gu16v_adc_res2.S16 = gu32v_adc_data.S32 ;			
				fun_ADC_Disable();
				
				gu8_ntc_mode = 0;
				gbv_1s = 0;
				return	1;
			//	gu8_ntc_mode ++;
			}
			break;
		}*/
		
		default:break;
	}	
	return 0;
}

S32 lu32v_adc_result1;
U8	fun_get_VTP()
{
	
	
	switch(gu8v_obj_temp_mode)
	{
		case	0:
		{
			fun_VTP_INIT();
			//得到0點 短路VTP
			mac_OPA_AN7_AN6();
			gu8v_obj_temp_mode ++;					
			break;
		}
		case	1:
		if(gbv_adc_success)
		{	
			gu16v_adc_vtp.S16 = gu32v_adc_data.S32;	
			mac_OPA_AN7_AN4();				
			fun_VTP_INIT();
			gu8v_obj_temp_mode ++;	
			break;	
		}
		case	2:
		if(gbv_adc_success)
		{
			gu8v_obj_temp_mode = 0;	
			gu16v_adc_vtp.S16 = gu32v_adc_data.S32 - gu16v_adc_vtp.S16 ;
			return 1;
			
			
			gu16v_adc_buff[gu8v_temp_test] = gu16v_adc_vtp.S16;
			gu8v_temp_test++;
			if(gu8v_temp_test>=5)
			{
				gu8v_temp_test = 0;
				U8 i,j;
				U16 lu16_adc_max1;

				for(i=0;i<5;i++)
				{
					for(j=i;j<4;j++)
					{
						if(gu16v_adc_buff[i]>gu16v_adc_buff[j+1])
						{
							lu16_adc_max1=gu16v_adc_buff[i] ;
							gu16v_adc_buff[i]=gu16v_adc_buff[j+1] ;
							gu16v_adc_buff[j+1] = lu16_adc_max1;

						}
					}
				}
//				gu16v_adc_vtp.S16 = gu16v_adc_buff[2];
	
	
			lu32v_adc_result1 = 0;
			for(i=1;i<4;i++)
			{
				lu32v_adc_result1 += gu16v_adc_buff[i];
			}
			


				gu16v_adc_vtp.S16 = lu32v_adc_result1/3;	
				
				
//			lu32v_adc_result1 = 0;
//			for(i=0;i<5;i++)
//			{
//				lu32v_adc_result1 += gu16v_adc_buff[i];
//			}
//			
//
//
//				gu16v_adc_vtp.S16 = lu32v_adc_result1/5;
				return	1;		
			}			

		}
		break;	

		break;		
		case	6:
		
		break;		
	}
	return	0;
}

/*************************************************************************************/
//**	Copyright	:	2016 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	lcd.c 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	HT67F5652													**
//**	Author   	: 	Leo														**
//**	Date     	: 	2016/10/28													**
//**	Version  	: 	V00															**
//**	History  	:																**
/*************************************************************************************/

#include	"..\User\Config.h"




//0-9 
//10 E
//11 r
//12 H 
//13 L
//14 o
//15 F
//16 -
//17 C
//18 none

const U8 lu8c_8_table[] = {0b11011011,0b00000011,0b01011110,0b00011111,0b10000111,0b10011101,0b11011101,0b10001011,0b11011111,0b10001111,
						   0b11011100,0b01000100,0b11000111,0b11010000,0b01010101,0b11001100,0b00000100,0b11011000,0b00000000,0b00000001};

/********************************************************************
Function:	fun_Lcd_Init
INPUT	:		
OUTPUT	:	
NOTE	:
********************************************************************/
void fun_Lcd_Init()
{
	
	
		
	_lcdcp 	= 0b00000000;	
	_lcdc   = 0b00000111;
	
#if(mac_MCU_pin == mac_MCU_P64)	
	_lcd2	= 0b11111100;
	_lcd3	= 0b00000011;

#endif			
	fun_LCD_FULL(0x00);	
}

/********************************************************************
Function:	fun_LCD_FULL
INPUT	:	lu8v_lcd_data
OUTPUT	:	null
NOTE	:	可以根據傳遞進來的參數統一改寫LCD的參數
********************************************************************/
void fun_LCD_FULL(U8 lu8v_lcd_data)
{
	U8	lu8v_lcdcount;
     for(lu8v_lcdcount = 0 ;lu8v_lcdcount < 40;lu8v_lcdcount++)    	
     {
     	 gu8v_LCD_buffer[lu8v_lcdcount].U8 = lu8v_lcd_data;
     }  
}


/********************************************************************
Function:	fun_LCD_NUM
INPUT	:	lu8v_lcd_mode 	lu8v_addr lu8v_data
OUTPUT	:	
NOTE	:	lu8v_lcd_mode為顯示模式（亮、滅、閃爍）lu8v_addr為顯示的地址lu8v_data顯示數據
********************************************************************/
//LCD
void fun_LCD_NUM(U8 lu8v_lcd_mode,U8 lu8v_addr,U8 lu8v_data)
{
		volatile	U8 lu8v_table_buff;
		volatile	U8 lu8v_addr_0,lu8v_addr_1,lu8v_addr_2;
		
		lu8v_table_buff = lu8c_8_table[lu8v_data];
		
		if(lu8v_addr ==32)
		{
			lu8v_addr_0	= 32;
			lu8v_addr_1 = 15;
			lu8v_addr_2 = 14;			
		}
		else
		{
			lu8v_addr_0	= lu8v_addr;
			lu8v_addr_1 = lu8v_addr - 1;
			lu8v_addr_2 = lu8v_addr - 2;
		}
		
		
		gu8v_LCD_buffer[lu8v_addr_0].U8 &= 0b00001001;
		gu8v_LCD_buffer[lu8v_addr_1].U8 = 0;
		gu8v_LCD_buffer[lu8v_addr_2].U8 &= 0b00001001;	
			
		if(lu8v_lcd_mode==0)
		{	
			
		} 		
		else if(lu8v_lcd_mode == 1)	//貝爾康 COM0 沒用到 
		{
		
			 gu8v_LCD_buffer[lu8v_addr_2].U8 |= lu8v_table_buff<<1 & 0b00000110;
			 gu8v_LCD_buffer[lu8v_addr_1].U8 |= lu8v_table_buff>>1 & 0b00001110;
			 gu8v_LCD_buffer[lu8v_addr_0].U8 	 |= lu8v_table_buff>>5 & 0b00000110;
			 
			 fun_LCD_SET_DOT();		
			 GCC_NOP();		 	 			
		}
		else
		{
			if(gbv_500ms_reversal)
			{
			 gu8v_LCD_buffer[lu8v_addr-2].U8 |= lu8v_table_buff<<1 & 0b00000110;
			 gu8v_LCD_buffer[lu8v_addr-1].U8 |= lu8v_table_buff>>1 & 0b00001110;
			 gu8v_LCD_buffer[lu8v_addr].U8   |= lu8v_table_buff>>5 & 0b00000110;
			 fun_LCD_SET_DOT();					 	 			
				 					
			}
			else
			{	

			}
			
		}

		mac_LED_G_Light();
}

/********************************************************************
Function:	fun_LCD_DISPLAY_NUM
INPUT	:	lu8v_DISPLAUDATA
OUTPUT	:	
NOTE	:
********************************************************************/
//LCD
void fun_LCD_DISPLAY_NUM(U16 lu8v_DISPLAUDATA)
{   
	fun_LCD_NUM(1,LU8C_NUM3_ADDR,lu8v_DISPLAUDATA%10);
	lu8v_DISPLAUDATA /= 10;
	fun_LCD_NUM(1,LU8C_NUM2_ADDR,lu8v_DISPLAUDATA%10);
	lu8v_DISPLAUDATA /= 10;
	fun_LCD_NUM(1,LU8C_NUM1_ADDR,lu8v_DISPLAUDATA%10);		
}

/********************************************************************
Function:	fun_LCD_CLR_TEMP
INPUT	:	
OUTPUT	:	
NOTE	:
********************************************************************/
void fun_LCD_CLR_TEMP()
{
	fun_LCD_NUM(1,LU8C_NUM3_ADDR,18);
	fun_LCD_NUM(1,LU8C_NUM2_ADDR,18);
	fun_LCD_NUM(1,LU8C_NUM1_ADDR,18);	
}

/********************************************************************
Function:	fun_LCD_TEMP_HIGH
INPUT	:	
OUTPUT	:	
NOTE	:
********************************************************************/
void fun_LCD_TEMP_HIGH()
{
	fun_LCD_NUM(1,LU8C_NUM1_ADDR,12);
	fun_LCD_NUM(1,LU8C_NUM2_ADDR,19);
	fun_LCD_NUM(1,LU8C_NUM3_ADDR,18);
	fun_LCD_CLR_DOT();
	mac_LED_R_Light();		
}	

/********************************************************************
Function:	fun_LCD_TEMP_LOW
INPUT	:	
OUTPUT	:	
NOTE	:
********************************************************************/
void fun_LCD_TEMP_LOW()
{
	fun_LCD_NUM(1,LU8C_NUM1_ADDR,13);
	fun_LCD_NUM(1,LU8C_NUM2_ADDR,14);
	fun_LCD_NUM(1,LU8C_NUM3_ADDR,18);
	fun_LCD_CLR_DOT();	
	mac_LED_R_Light();	
}

volatile U8 gu8v_EEprom_Count;
/********************************************************************
Function:	fun_LCD_Display_Mode
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_LCD_Display_Mode()
{
	__16_type lu8v_EEprom_Temp;
	
	switch(gu8v_dispaly_mode.U8)
	{
		case 0b00000001:	// key_room
		{
			fun_LCD_SET_Room_Temp();
			fun_LCD_CLR_Surface_Temp();
			fun_LCD_CLR_Body_Temp();
			gu8v_EEprom_Count = 0;
				
			break;
		}
		case 0b00000010:	// key_surface
		{
			fun_LCD_CLR_Room_Temp();
			fun_LCD_SET_Surface_Temp();
			fun_LCD_CLR_Body_Temp();
			gu8v_EEprom_Count = 0;
			break;
		}
		case 0b00000100:	// key_body
		{
			fun_LCD_CLR_Room_Temp();
			fun_LCD_CLR_Surface_Temp();
			fun_LCD_SET_Body_Temp();
			gu8v_EEprom_Count = 0;
			break;
		}
		case 0b00001000:	// key_Memory_D
		{
			if(gu8v_EEprom_Count>9) gu8v_EEprom_Count = 0;
			gu8v_EEprom_Count++;	
			fun_LCD_CLR_Room_Temp();
			fun_LCD_CLR_Surface_Temp();
			fun_LCD_CLR_Body_Temp();
			fun_LCD_SET_M();
			 
			fun_LCD_DISPLAY_NUM(gu8v_EEprom_Count);
			fun_LCD_CLR_DOT();
			gu8v_dispaly_mode.U8 = 0b10000000;
		}  
			break;
		case 0b00010000:	// key_Memory_V
		{	
			lu8v_EEprom_Temp.byte.byte0 = fun_EEprom_Read(mac_EEprom_Keep_StartAdd+(gu8v_EEprom_Count<<1));
			lu8v_EEprom_Temp.byte.byte1 = fun_EEprom_Read(mac_EEprom_Keep_StartAdd+(gu8v_EEprom_Count<<1)+1);
			if(lu8v_EEprom_Temp.U16 == 0xffff)
			{
				fun_LCD_NUM(1,LU8C_NUM3_ADDR,16);		
				fun_LCD_NUM(1,LU8C_NUM2_ADDR,16);	
				fun_LCD_NUM(1,LU8C_NUM1_ADDR,16);
			}
				
			else
				fun_LCD_DISPLAY_NUM(lu8v_EEprom_Temp.U16);	
			gu8v_dispaly_mode.U8 = 0b10000000;			
		}
		break;
//		case 0b10000000:
//		{
//			gu8v_dispaly_mode.U8 = 0b00000010;
//		}
//		break;
		default:break;
	}
}

/********************************************************************
Function:	fun_LCD_Display_Data
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_LCD_Display_Data()
{
	switch(gu8v_dispaly_mode.U8)
	{
		case 0b00000001:	// key_room
		{
		
			fun_LCD_DISPLAY_NUM(gu16v_temp.S16);
			fun_Beep_init(mac_beep_1);
				
			break;
		}
		case 0b00000010:	// key_surface
		{
			
			if(gu16v_current_temp.S16 < 50)
			{			
				fun_LCD_TEMP_LOW();
				fun_Beep_init(mac_beep_1_1_1_1);
			}
			else if(gu16v_current_temp.S16 > 800)
			{
				fun_LCD_TEMP_HIGH();
				fun_Beep_init(mac_beep_1_1_1_1);
			}
			else
			{	
				fun_LCD_DISPLAY_NUM(gu16v_current_temp.S16);
				fun_Beep_init(mac_beep_1);
			}
			
			break;
		}
		case 0b00000100:	// key_body
		{
//			fun_EEprom_Keep_W();
	
			if(gu16v_current_temp.S16 < 281)
			{			
				fun_LCD_TEMP_LOW();
				fun_Beep_init(mac_beep_1_1_1_1);
			}
			else if(gu16v_current_temp.S16 > 389)
			{
				fun_LCD_TEMP_HIGH();
				fun_Beep_init(mac_beep_1_1_1_1);
			}
			else
			{
				fun_LCD_DISPLAY_NUM(gu16v_body_temp.S16);
				fun_Beep_init(mac_beep_1);
			}
			break;
		}
		default:break;
	}
}


/********************************************************************
Function:	fun_LCD_Test
INPUT	:	
OUTPUT	:	
NOTE	:	
********************************************************************/
void	fun_LCD_Test()
{
	volatile	U16 lu8v_count;
	while(1)
	{	
		fun_LCD_DISPLAY_NUM(231);
	
		GCC_CLRWDT();//清除看門狗
		lu8v_count++;
		fun_LCD_DISPLAY_NUM(lu8v_count);
		GCC_DELAY(100000);GCC_DELAY(100000);GCC_DELAY(100000);GCC_DELAY(100000);GCC_DELAY(100000);	//delay 500ms
	
	}
		
}


#if(mac_MCU_pin == mac_MCU_P80)

void	fun_LCD_SET_F()
{
	gu8v_LCD_buffer[24].bits.b1 = 1;		
}

void	fun_LCD_CLR_F()
{
	gu8v_LCD_buffer[24].bits.b1 = 0;		
}		

void	fun_LCD_SET_C()	
{
	gu8v_LCD_buffer[24].bits.b2 = 1;	
}

void	fun_LCD_CLR_C()	
{
	gu8v_LCD_buffer[24].bits.b2 = 0;		
}	

void	fun_LCD_SET_Room_Temp()
{
	gu8v_LCD_buffer[25].bits.b3 = 1;	
	gu8v_LCD_buffer[36].bits.b3 = 1;
}
	
void	fun_LCD_CLR_Room_Temp()
{
	gu8v_LCD_buffer[25].bits.b3 = 0;
	gu8v_LCD_buffer[36].bits.b3 = 0;		
}	
	
void	fun_LCD_SET_Surface_Temp()
{
	gu8v_LCD_buffer[31].bits.b3 = 1;
	gu8v_LCD_buffer[33].bits.b3 = 1;		
		
}
		
void	fun_LCD_CLR_Surface_Temp()
{
	gu8v_LCD_buffer[31].bits.b3 = 0;
	gu8v_LCD_buffer[33].bits.b3 = 0;		
		
}
	
void	fun_LCD_SET_Body_Temp()
{
	gu8v_LCD_buffer[28].bits.b3 = 1;
	gu8v_LCD_buffer[34].bits.b3 = 1;		
		
}
	
void	fun_LCD_CLR_Body_Temp()
{
	gu8v_LCD_buffer[28].bits.b3 = 0;
	gu8v_LCD_buffer[34].bits.b3 = 0;		
		
}		
	
void	fun_LCD_SET_DOT()
{
	gu8v_LCD_buffer[27].bits.b3 = 1;	
}
		
void	fun_LCD_CLR_DOT()
{
	gu8v_LCD_buffer[27].bits.b3 = 0;		
}	
	
void	fun_LCD_SET_M()	
{
	gu8v_LCD_buffer[37].bits.b3 = 1;		
}
	
void	fun_LCD_CLR_M()
{
	gu8v_LCD_buffer[37].bits.b3 = 0;		
}		
	

#endif

//-------------------------------------------//

#if( mac_MCU_pin == mac_MCU_P64)

void	fun_LCD_SET_F()
{
	gu8v_LCD_buffer[10].bits.b1 = 1;		
}

void	fun_LCD_CLR_F()
{
	gu8v_LCD_buffer[10].bits.b1 = 0;		
}		

void	fun_LCD_SET_C()	
{
	gu8v_LCD_buffer[10].bits.b2 = 1;	
}

void	fun_LCD_CLR_C()	
{
	gu8v_LCD_buffer[10].bits.b2 = 0;		
}	

void	fun_LCD_SET_Room_Temp()
{
	gu8v_LCD_buffer[11].bits.b3 = 1;	
	gu8v_LCD_buffer[38].bits.b3 = 1;
}
	
void	fun_LCD_CLR_Room_Temp()
{
	gu8v_LCD_buffer[11].bits.b3 = 0;
	gu8v_LCD_buffer[38].bits.b3 = 0;		
}	
	
void	fun_LCD_SET_Surface_Temp()
{
	gu8v_LCD_buffer[33].bits.b3 = 1;
	gu8v_LCD_buffer[35].bits.b3 = 1;		
		
}
		
void	fun_LCD_CLR_Surface_Temp()
{
	gu8v_LCD_buffer[33].bits.b3 = 0;
	gu8v_LCD_buffer[35].bits.b3 = 0;		
		
}
	
void	fun_LCD_SET_Body_Temp()
{
	gu8v_LCD_buffer[14].bits.b3 = 1;
	gu8v_LCD_buffer[36].bits.b3 = 1;		
		
}
	
void	fun_LCD_CLR_Body_Temp()
{
	gu8v_LCD_buffer[14].bits.b3 = 0;
	gu8v_LCD_buffer[36].bits.b3 = 0;		
		
}		
	
void	fun_LCD_SET_DOT()
{
	gu8v_LCD_buffer[13].bits.b3 = 1;	
}
		
void	fun_LCD_CLR_DOT()
{
	gu8v_LCD_buffer[13].bits.b3 = 0;		
}	
	
void	fun_LCD_SET_M()	
{
	gu8v_LCD_buffer[39].bits.b3 = 1;		
}
	
void	fun_LCD_CLR_M()
{
	gu8v_LCD_buffer[39].bits.b3 = 0;		
}		


//-------------------------------------------//

#endif


#ifndef _Lcd_H_
#define	_Lcd_H_


#define		LBC_CLR			              0
#define		LBC_SET			              1
#define		LBC_FLASH			          2
#define		LBC_FFLASH			          3



#if(mac_MCU_pin == mac_MCU_P80)

#define		LU8C_NUM0_ADDR               36
#define		LU8C_NUM1_ADDR               33
#define		LU8C_NUM2_ADDR               30
#define		LU8C_NUM3_ADDR               27

#endif

#if(mac_MCU_pin == mac_MCU_P64)

#define		LU8C_NUM0_ADDR               38
#define		LU8C_NUM1_ADDR               35
#define		LU8C_NUM2_ADDR               32
#define		LU8C_NUM3_ADDR               13

#endif


#define		mac_LCD_Enable()		{	_enlcd = 1;	}
#define		mac_LCD_Disable()		{	_enlcd = 0;	}

void		fun_LCD_FULL(U8 lu8v_lcd_data); 
void		fun_Lcd_Init();
void 		fun_LCD_NUM(U8 lu8v_lcd_mode,U8 lu8v_addr,U8 lu8v_data);
void 		fun_LCD_SET_F();	
void 		fun_LCD_CLR_F();
void 		fun_LCD_SET_C();	
void 		fun_LCD_CLR_C();
void 		fun_LCD_SET_COL();	
void 		fun_LCD_CLR_COL();
void 		fun_LCD_SET_M();	
void 		fun_LCD_CLR_M();
void 		fun_LCD_SET_B2();		
void		fun_LCD_SET_DOT();
void		fun_LCD_CLR_DOT();
void		fun_LCD_Test();


volatile	static	__byte_type  gu8v_LCD_buffer[40] __attribute__ ((at(0x180)));

void 	fun_LCD_DISPLAY_NUM(U16 lu8v_DISPLAUDATA);
void 	fun_LCD_CLR_TEMP();
void 	fun_LCD_TEMP_HIGH();
void 	fun_LCD_TEMP_LOW();
void	fun_LCD_SET_Body_Temp();
void 	fun_LCD_SET_Room_Temp();
void	fun_LCD_CLR_Surface_Temp();
void 	fun_LCD_CLR_Body_Temp();
void	fun_LCD_CLR_Room_Temp();
void	fun_LCD_SET_Surface_Temp();
void 	fun_LCD_Display_Mode();
void 	fun_LCD_Display_Data();
extern	volatile U8 gu8v_EEprom_Count;

#endif

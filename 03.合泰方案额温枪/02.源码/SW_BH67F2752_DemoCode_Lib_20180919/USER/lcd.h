
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	Lcd.h 	       	                                        **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/


#ifndef _Lcd_H_
#define	_Lcd_H_


// |---------------------------------------------------------------------------------------|
// |  PIN  |  1   |  2   |  3   |  4   |  5   |  6  |  7  |  8  |  9  |  10  |  11  |  12  |               
// |---------------------------------------------------------------------------------------|
// |  COM1 |      |      | 	    | COM1 |  1A  | 1B  |  2A | 2B  | 3A  |  3B  |  4A  |  4B  |
// |---------------------------------------------------------------------------------------|
// |  COM2 |      |		 | COM2 |      |  1F  | 1G  |  2F | 2G  | 3F  |  3G  |	4F  |  4G  |
// |---------------------------------------------------------------------------------------|
// |  COM4 | COM4 |      |      |      |  P1  | 1D  |  P2 | 2D  | P3  |	 3D  |  P4  |  4D  |
// |---------------------------------------------------------------------------------------|
// |  COM3 | 	  |	COM3 |      |      |  1E  | 1C  |  2E | 2C  | 3E  |  3C  |  4E  |  4C  |
// |---------------------------------------------------------------------------------------|
// |  RAM  | 	  |		 |      |      | 0	  | 1	|  2  | 3   | 4   | 5    |  6   |  8   | 
// |---------------------------------------------------------------------------------------|

#define     SEG_A   5
#define     SEG_B   9
#define     SEG_C   8
#define     SEG_D   6
#define     SEG_E   0
#define     SEG_F   1
#define     SEG_G   4

#define     a       (1<<SEG_A)
#define     b       (1<<SEG_B)
#define     c       (1<<SEG_C)
#define     d       (1<<SEG_D)
#define     e       (1<<SEG_E)
#define     f       (1<<SEG_F)
#define     g       (1<<SEG_G)
#define     h       (1<<SEG_H)

#define     LCDCODE_0		0
#define     LCDCODE_1		1	
#define     LCDCODE_2		2
#define     LCDCODE_3		3
#define     LCDCODE_4		4
#define     LCDCODE_5		5
#define     LCDCODE_6		6
#define     LCDCODE_7		7
#define     LCDCODE_8		8
#define     LCDCODE_9		9
#define     LCDCODE_A		10	
#define     LCDCODE_B		11
#define     LCDCODE_C		12
#define     LCDCODE_D		13
#define     LCDCODE_E		14
#define     LCDCODE_F		15
#define     LCDCODE_G		16
#define     LCDCODE_H		17
#define     LCDCODE_I		18
#define     LCDCODE_J		19
#define     LCDCODE_K		20
#define     LCDCODE_L		21
#define     LCDCODE_M		22
#define     LCDCODE_N		23
#define     LCDCODE_n		24
#define     LCDCODE_o		25
#define     LCDCODE_O		26
#define     LCDCODE_P		27
#define     LCDCODE_Q		28
#define     LCDCODE_r		29	
#define     LCDCODE_S		30
#define     LCDCODE_t		31
#define     LCDCODE_T		32
#define     LCDCODE_U		33
#define     LCDCODE_v		34
#define     LCDCODE_W		35
#define     LCDCODE_Y		36
#define     LCDCODE_Z		37
#define     LCDCODE__		38
#define     LCDCODE_DOT		39
#define     LCDCODE_NONE	40
#define     LCDCODE_c	    41

#define		LCD_ADDR0       1
#define		LCD_ADDR1       4
#define		LCD_ADDR2       14
#define		LCD_ADDR3       17

#define		SET_LCD_Enable()		{	_lcden = 1;	}
#define		SET_LCD_Disable()		{	_lcden = 0;	}


volatile	static	__8_type  gu8v_LCD_buffer[32] __attribute__ ((at(0x400)));


void	fun_LCD_FULL(u8 lu8v_lcd_data); 
void	fun_LCD_Init();
void 	fun_LCD_NUM(u8 lu8v_lcd_mode,u8 lu8v_addr,u8 lu8v_data);
void 	LCD_DISPLAY_NUM(u8 lu8v_mode,u16 lu8v_DISPLAUDATA);
void 	fun_LCD_UpData();
void  fun_LCD_Display_Cor(u8 lu8v_corr);
void  fun_LCD_Display_Success();
void  fun_LCD_Display_Fail();
void 	fun_LCD_Display____();
void  fun_LCD_Display_C();
void 	fun_LCD_Test();
void 	fun_LCD_Display_Mode(u8 Mode_Sum);
void 	fun_LCD_Display_BAT();
void fun_LCD_Display_CF(u8 CF_Flag);
void fun_LCD_Display_Power();


#define SET_LCD_DOT()   gu8v_LCD_buffer[14].bits.b2 = 1;		//开启小数点2
#define CLR_LCD_DOT()   gu8v_LCD_buffer[14].bits.b2 = 0;		//关闭小数点2

#define SET_LCD_BEEP()   gu8v_LCD_buffer[0].bits.b1 = 1;		//打开蜂鸣器显示
#define CLR_LCD_BEEP()   gu8v_LCD_buffer[0].bits.b1 = 0;		//关闭蜂鸣器显示

#define SET_LCD_BAT()   gu8v_LCD_buffer[0].bits.b0 = 1;			//打开电池显示
#define CLR_LCD_BAT()   gu8v_LCD_buffer[0].bits.b0 = 0;			//关闭电池显示

#define SET_LCD_Memory()   gu8v_LCD_buffer[0].bits.b2 = 1;		//打开记忆显示
#define CLR_LCD_Memory()   gu8v_LCD_buffer[0].bits.b2 = 0;		//关闭记忆显示

#define SET_LCD_TempC()   gu8v_LCD_buffer[20].bits.b1 = 1,gu8v_LCD_buffer[20].bits.b0 = 0;		//打开摄氏度显示					  
#define CLR_LCD_TempC()   gu8v_LCD_buffer[20].bits.b1 = 0;		//关闭摄氏度显示


#define SET_LCD_TempF()   gu8v_LCD_buffer[20].bits.b1 = 0,gu8v_LCD_buffer[20].bits.b0 = 1;		//打开摄氏度显示
#define CLR_LCD_TempF()   gu8v_LCD_buffer[20].bits.b0 = 0;		//关闭摄氏度显示

#define SET_LCD_SurTmp()   gu8v_LCD_buffer[4].bits.b2 = 1,gu8v_LCD_buffer[13].bits.b2 = 1;		//打开表面温度显示
#define CLR_LCD_SurTmp()   gu8v_LCD_buffer[4].bits.b2 = 0,gu8v_LCD_buffer[13].bits.b2 = 0;		//关闭表面温度显示

#define SET_LCD_Body()   gu8v_LCD_buffer[3].bits.b2 = 1,gu8v_LCD_buffer[16].bits.b2 = 1;		//打开体温显示
#define CLR_LCD_Body()   gu8v_LCD_buffer[3].bits.b2 = 0,gu8v_LCD_buffer[16].bits.b2 = 0;		//关闭体温显示

#define SET_LCD_Room()   gu8v_LCD_buffer[1].bits.b2 = 1,gu8v_LCD_buffer[19].bits.b2 = 1;		//打开室温显示
#define CLR_LCD_Room()   gu8v_LCD_buffer[1].bits.b2 = 0,gu8v_LCD_buffer[19].bits.b2 = 0;		//关闭室温显示

#define Mode_None  	0
#define Mode_SurTmp 3
#define Mode_Body  	1
#define Mode_Room  	2

#endif

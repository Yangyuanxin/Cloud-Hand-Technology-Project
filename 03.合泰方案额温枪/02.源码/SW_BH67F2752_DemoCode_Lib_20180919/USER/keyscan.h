
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	adc.c 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Leo															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/



#ifndef _Key_Scan_H_
#define	_Key_Scan_H_


/********************************************************************
Function:	function
********************************************************************/	
void	fun_Key_Scan(void);
void	fun_Key_dealwith(void);
void	fun_Key_init();
void	fun_iir_filter1(u16 lu16_current_adc_data);
/********************************************************************
Function:	KEY
********************************************************************/
//按鍵消抖時間設置 1 代表 10ms
#define		LUCC_KEY_DEBOUNCE	5		
#define		LUCC_LONG_KEY_TIME	200	

/********************************************************************
Function:	GPIO
********************************************************************/
#define		GPIO_KEY_1_WU		1
#define		GPIO_KEY_1_PU		_papu1
#define		GPIO_KEY_1_IO		_pac1
#define		GPIO_KEY_1			_pa1

#define		GPIO_KEY_2_WU		1
#define		GPIO_KEY_2_PU		_papu3
#define		GPIO_KEY_2_IO		_pac3
#define		GPIO_KEY_2			_pa3

#define		GPIO_KEY_3_WU		1
#define		GPIO_KEY_3_PU		_papu4
#define		GPIO_KEY_3_IO		_pac4
#define		GPIO_KEY_3			_pa4

#define		GPIO_KEY_4_WU		1
#define		GPIO_KEY_4_PU		_pbpu3
#define		GPIO_KEY_4_IO		_pbc3
#define		GPIO_KEY_4			_pb3


#define		KEY_PORT0			GPIO_KEY_1
#define		KEY_PORT1			GPIO_KEY_2
#define		KEY_PORT2			GPIO_KEY_3
#define		KEY_PORT3			GPIO_KEY_4
#define		KEY_PORT4			1
#define		KEY_PORT5			1
#define		KEY_PORT6			1
#define		KEY_PORT7			1

typedef struct
{
	__8_type		KeyCurrent;
	__8_type		KeyDown;			
	u8				KeyOld;
	u8				KeyNoChangedTime;
	__8_type		KeyPress;
	__8_type		KeyUp;
	u8				KeyLast;
	u8				KeyCanChange;
	__8_type		KeyStatus;
	__8_type		KeyStatus_L;
	u8 				KeyPressTime;
  
}KEY_define;
extern	KEY_define	SDK_KEY;

//#define KeyCurrent0 			SDK_KEY.KeyCurrent.bits.b0
//#define KeyCurrent1 			SDK_KEY.KeyCurrent.bits.b1
//#define KeyCurrent2 			SDK_KEY.KeyCurrent.bits.b2
//#define KeyCurrent3 			SDK_KEY.KeyCurrent.bits.b3
//#define KeyCurrent4 			SDK_KEY.KeyCurrent.bits.b4
//#define KeyCurrent5 			SDK_KEY.KeyCurrent.bits.b5
//#define KeyCurrent6 			SDK_KEY.KeyCurrent.bits.b6
//#define KeyCurrent7 			SDK_KEY.KeyCurrent.bits.b7

#define gbv_KeyDown_1	 			SDK_KEY.KeyDown.bits.b1
#define gbv_KeyDown_2				SDK_KEY.KeyDown.bits.b2
#define gbv_KeyDown_3				SDK_KEY.KeyDown.bits.b3
#define gbv_KeyDown_4	 			SDK_KEY.KeyDown.bits.b4
#define gbv_KeyDown_5				SDK_KEY.KeyDown.bits.b5
#define gbv_KeyDown_6				SDK_KEY.KeyDown.bits.b6
#define gbv_KeyDown_7	 			SDK_KEY.KeyDown.bits.b7
#define gbv_KeyDown_8				SDK_KEY.KeyDown.bits.b8

#define gbv_KeyUp_1					SDK_KEY.KeyUp.bits.b0
#define gbv_KeyUp_2			 		SDK_KEY.KeyUp.bits.b1
#define gbv_KeyUp_3					SDK_KEY.KeyUp.bits.b2
#define gbv_KeyUp_4					SDK_KEY.KeyUp.bits.b3
#define gbv_KeyUp_5					SDK_KEY.KeyUp.bits.b4
#define gbv_KeyUp_6					SDK_KEY.KeyUp.bits.b5
#define gbv_KeyUp_7					SDK_KEY.KeyUp.bits.b6
#define gbv_KeyUp_8					SDK_KEY.KeyUp.bits.b7

#define gbv_KeyPress_1				SDK_KEY.KeyPress.bits.b0
#define gbv_KeyPress_2			 	SDK_KEY.KeyPress.bits.b1
#define gbv_KeyPress_3				SDK_KEY.KeyPress.bits.b2
#define gbv_KeyPress_4				SDK_KEY.KeyPress.bits.b3
#define gbv_KeyPress_5				SDK_KEY.KeyPress.bits.b4
#define gbv_KeyPress_6				SDK_KEY.KeyPress.bits.b5
#define gbv_KeyPress_7				SDK_KEY.KeyPress.bits.b6
#define gbv_KeyPress_8				SDK_KEY.KeyPress.bits.b7

#define gbv_KeyStatus_1				SDK_KEY.KeyStatus.bits.b0
#define gbv_KeyStatus_2			 	SDK_KEY.KeyStatus.bits.b1
#define gbv_KeyStatus_3				SDK_KEY.KeyStatus.bits.b2
#define gbv_KeyStatus_4				SDK_KEY.KeyStatus.bits.b3
#define gbv_KeyStatus_5				SDK_KEY.KeyStatus.bits.b4
#define gbv_KeyStatus_6				SDK_KEY.KeyStatus.bits.b5
#define gbv_KeyStatus_7				SDK_KEY.KeyStatus.bits.b6
#define gbv_KeyStatus_8				SDK_KEY.KeyStatus.bits.b7

#define gbv_KeyStatus_L_1				SDK_KEY.KeyStatus_L.bits.b0
#define gbv_KeyStatus_L_2			 	SDK_KEY.KeyStatus_L.bits.b1
#define gbv_KeyStatus_L_3				SDK_KEY.KeyStatus_L.bits.b2
#define gbv_KeyStatus_L_4				SDK_KEY.KeyStatus_L.bits.b3
#define gbv_KeyStatus_L_5				SDK_KEY.KeyStatus_L.bits.b4
#define gbv_KeyStatus_L_6				SDK_KEY.KeyStatus_L.bits.b5
#define gbv_KeyStatus_L_7				SDK_KEY.KeyStatus_L.bits.b6
#define gbv_KeyStatus_L_8				SDK_KEY.KeyStatus_L.bits.b7

#endif



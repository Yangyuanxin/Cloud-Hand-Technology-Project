
#ifndef _CONFIG_H__
#define _CONFIG_H__




/********************************************************************
NOTE:	系統宏定義
********************************************************************/

#define	mac_Test_On		1
#define	mac_Test_Off	0
#define mac_Test_Mode 	mac_Test_Off

#define	mac_MCU_P80		1
#define	mac_MCU_P64		0
#define	mac_MCU_pin		mac_MCU_P64

/********************************************************************
NOTE:	lib 庫文件中包含的頭文件
********************************************************************/
//系統頭文件
#include "HT67F5652.h"
//類型頭文件,需要在使用前先被包含

#include "..\User\typedef.h"
//
#include  "..\User\Ram_Init.h"
//
#include  "..\User\Timer.h"
//
#include  "..\User\EEPROM.h"
/********************************************************************
NOTE:	user 用戶程序中使用的頭文件
********************************************************************/
#include "..\User\Main.h"
//#include "..\User\define.h"
#include "..\User\GPIO.h"
#include "..\User\Initial.h"
#include "..\User\Key_Scan.h"
#include "..\User\Uart.h"
#include "..\User\Lcd.h"
#include "..\User\Work_mode.h"
#include "..\User\ADC.h"
#include "..\User\Beep.h"
#include "..\User\calculate.h"





#endif



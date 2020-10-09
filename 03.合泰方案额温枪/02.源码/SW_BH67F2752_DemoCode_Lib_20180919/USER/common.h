

/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	calculate.c 	       	                                    **
//**	Description	: 																**
//**	MCU      	: 	xx															**
//**	Author   	: 	Ljq													    	**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#ifndef _COMMON_H__
#define _COMMON_H__

/********************************************************************
NOTE:	系統宏定義
********************************************************************/

#define CLOCK_4M			0
#define CLOCK_8M			1
#define CLOCK_12M			2
#define	SET_SYSTEM_CLOCK	CLOCK_8M

#define TEST_NONE       0
#define TEST_LCD        1
#define TEST_BEEP       2
#define TEST_PERI       3
#define TEST_UART       4
#define TEST_NTC        5
#define TEST_OPAZERO    6
#define TEST_OPAVTP     7
#define TEST_MODE       TEST_NONE

//系統頭文件
#include "BH67F2752.h"
//類型頭文件,需要在使用前先被包含
#include "typedef.h"
#include "Ram.h"
#include "main.h"
#include "SysInit.h"
#include "work.h"
#include "calculate.h"
#include "Timer.h"
#include "eeprom.h"
#include "adc.h"
#include "lcd.h"
#include "keyscan.h"
#include "beep.h"
#include "uart.h"

#endif




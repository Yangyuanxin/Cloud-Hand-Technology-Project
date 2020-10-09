
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	eeprom.h	       	                                        **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742												    **
//**	Author   	: 	Ljq													    	**
//**	Date     	: 	2018/06/07    												**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#ifndef _EEPROM_H__
#define _EEPROM_H__

/********************************************************************
NOTE:î¶?
********************************************************************/
#define	mac_eeprom_Long		1
#define	mac_eeprom_Short	0
#define mac_eeprom_type		mac_eeprom_Long

void    fun_EEprom_Write(u8 addr,u8 data);
u8	    fun_EEprom_Read(u8 addr);
void	fun_EEPROM_Init();

#endif
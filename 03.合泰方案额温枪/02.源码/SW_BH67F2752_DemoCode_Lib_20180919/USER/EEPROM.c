
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	eeprom.c 	       	                                    	**
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq													    	**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#include "common.h"

/********************************************************************
Function:	fun_EEprom_Read
INPUT	:	
OUTPUT	:	
NOTE	:
*******************************************************************/
u8 fun_EEprom_Read(u8 lu8v_addr)
{
	u8 lu8v_eeprom_byte;
		
	_eea = lu8v_addr;


	#if(mac_eeprom_type == mac_eeprom_Short)

	_bp     = 1;
	_mp1    = 0x40;
	_iar1  |= 0x02;
	_iar1  |= 0x01;
		
	while(_iar1 & 0x01);
	lu8v_eeprom_byte = _eed;		
	_iar1  &= 0xfd;  
	
	#endif
//============================================================	
	#if(mac_eeprom_type == mac_eeprom_Long)

	_mp1l = 0x40;
	_mp1h = 1;
	_iar1 |=0x2;				//enable RDEN
	_iar1 |=0x1;				//active READ 
	while(_iar1 & 0x01);		//wait read end
	lu8v_eeprom_byte = _eed;
	_iar1 &=	0xfD;				//disable RDEN	

	#endif

	return lu8v_eeprom_byte;	

}
 
/********************************************************************
Function:	fun_EEprom_Write
INPUT	:	
OUTPUT	:	
NOTE	:
*******************************************************************/
void fun_EEprom_Write(u8 lu8v_addr, u8 lu8v_data)
{

	_eea = lu8v_addr;
	_eed = lu8v_data;

	#if(mac_eeprom_type == mac_eeprom_Short)

	_bp     = 1;
	_mp1    = 0x40;
	_iar1  |= 0x08;
	_iar1  |= 0x04;
	while(_iar1 & 0x04);
	_iar1	&=	0xf7;				//disable WREN	
	_bp     = 0;	

	#endif	
//============================================================	
	#if(mac_eeprom_type == mac_eeprom_Long)

	_mp1l = 0x40;
	_mp1h = 1;
	_iar1	|=	0x8;				//enable WREN
	_iar1	|=	0x4;				//active write 
	while(_iar1 & 0x04);		//wait write end
	_iar1	&=	0xf7;				//disable WREN	

	#endif
}

/********************************************************************
Function:	fun_EEPROM_Init
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/

void	fun_EEPROM_Init()
{
	//-------------------------------------------------------------------
	//	EEPROM:
	//	Adr: 0
	//	Adr: 1-2	:Vtp CorrValue
	//	Adr: 3-4	:NTC CorrValue
	//-------------------------------------------------------------------	
	if (fun_EEprom_Read(0x00) == 0x55)
	{
		gu16v_Correction_K1.byte.byte0 = fun_EEprom_Read(0x01);
		gu16v_Correction_K1.byte.byte1 = fun_EEprom_Read(0x02);

		gu16v_Correction_K2.byte.byte0 = fun_EEprom_Read(0x03);
		gu16v_Correction_K2.byte.byte1 = fun_EEprom_Read(0x04);
	}
	else
	{
		gu16v_Correction_K1.U16 = 5000;
		gu16v_Correction_K2.U16 = 10000;
	}

}










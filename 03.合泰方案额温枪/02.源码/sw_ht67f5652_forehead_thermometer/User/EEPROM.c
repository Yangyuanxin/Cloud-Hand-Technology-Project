
/*************************************************************************************/
//**	Copyright	:	2016 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	eeprom.c 	       	                                    **
//**	Description	: 																**
//**	MCU      	: 	HT67F5652													**
//**	Author   	: 	Leo													    	**
//**	Date     	: 	2016/10/28													**
//**	Version  	: 	V00															**
//**	History  	:																**
/*************************************************************************************/

#include "..\User\CONFIG.h"

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




///********************************************************************
//Function:	fun_EEprom_Keep_R
//INPUT	:	
//OUTPUT	:	
//NOTE	:
//*******************************************************************/
//void fun_EEprom_Keep_R()
//{
//	U8 i;
//	for(i=0;i<mac_EEprom_Keep_NUM;i++)
//	{
//		gu16v_EEprom_Keep_Buff[i].byte.byte0	=	fun_EEprom_Read(mac_EEprom_Keep_StartAdd+(i<<1));
//		gu16v_EEprom_Keep_Buff[i].byte.byte1	=	fun_EEprom_Read(mac_EEprom_Keep_StartAdd+(i<<1)+1);
//	}
//	
//}
///********************************************************************
//Function:	fun_EEprom_Keep_W
//INPUT	:	
//OUTPUT	:	
//NOTE	:
//*******************************************************************/
//void fun_EEprom_Keep_W()
//{
//	U8 i;
//	for(i=mac_EEprom_Keep_NUM-1;i>0;i--)
//	{
//		gu16v_EEprom_Keep_Buff[i].U16 = gu16v_EEprom_Keep_Buff[i-1].U16 ;
//	}
//	gu16v_EEprom_Keep_Buff[0].U16	=	gu16v_body_temp.S16;
//	
//	 
//	for(i=0;i<mac_EEprom_Keep_NUM;i++)
//	{
//		fun_EEprom_Write(mac_EEprom_Keep_StartAdd+(i<<1)  ,gu16v_EEprom_Keep_Buff[i].byte.byte0);
//		fun_EEprom_Write(mac_EEprom_Keep_StartAdd+(i<<1)+1,gu16v_EEprom_Keep_Buff[i].byte.byte1);
//	}
//	
//}
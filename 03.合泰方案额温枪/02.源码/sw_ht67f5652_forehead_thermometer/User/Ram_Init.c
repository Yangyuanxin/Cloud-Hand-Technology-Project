
/*************************************************************************************/
//**	Copyright	:	2016 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	Ram_Init.c 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	HT67F5652													**
//**	Author   	: 	Leo															**
//**	Date     	: 	2016/10/28													**
//**	Version  	: 	V00															**
//**	History  	:																**
/*************************************************************************************/
#include "..\User\Config.h"


#define mac_Ram_Type	0

/********************************************************************
Function:	fun_Ram_Init
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void	fun_Ram_Init() 
{

	#if(mac_Ram_Type == 0)

		_mp1h = 0;
		_mp1l = 0x80;
		do
		{
			for(_tblp = 0x00;_tblp < 128;_tblp++)
			{
				 _iar1 = 0;
				 _mp1l++;
			}		
			_mp1h++;
			if(_mp1h > mac_RAMBANK_NUM) break;
		}while(1);		

	#else

		_bp = 0;	
		do
		{	
			_mp1 = 0x80;
			for(_tblp = 0x00;_tblp <0x60;_tblp++)
			{
				 _iar1 = 0;
				 _mp1++;
			}			
			_bp++;
			if(_bp > mac_RAMBANK_NUM) break;
		}while(TRUE);

	#endif


}

	
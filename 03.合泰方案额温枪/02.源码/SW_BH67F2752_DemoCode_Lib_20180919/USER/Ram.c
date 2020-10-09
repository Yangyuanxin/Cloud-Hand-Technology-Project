
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	ram.c 	       	                                   			**
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq													    	**
//**	Date     	: 	2018/6/6													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#include "common.h"

/********************************************************************
Function:	fun_Ram_CLear
INPUT	:	none
OUTPUT	:	none
NOTE	:	mp0/iar0只能訪問RAM bank0

短指令：	mp1/iar1可以通過BP訪問所有bank
長指令：	mp1l/mp1h/iar1 可以訪問所有bank
********************************************************************/
void	fun_Ram_CLear() 
{

	#if(RAM_TYPE == RAM_SHORT)
	
	_bp = 0;		
	do
	{	
		_mp1 = RAM_STARTADDR;
		for(_tblp = 0;_tblp < RAM_LENGTH;_tblp++)
		{
			 _iar1 = 0;
			 _mp1++;
		}	
		if(_bp < mac_RAM_TYPE)	
		{
			_bp ++;
		}
		else
		{
			break;
		}
		
	}while(1);

	#endif
//========================================================
	#if(RAM_TYPE == RAM_LONG)

	_mp1h = 0;
	do
	{
		_mp1l = RAM_STARTADDR;
		for(_tblp = 0;_tblp < RAM_LENGTH;_tblp++)
		{
			 _iar1 = 0;
			 _mp1l++;
		}		
		if(_mp1h < RAM_TYPE) 
		{
			_mp1h++;
		}
		else
		{
			break;
		}
			
	}while(1);	

	#endif
	

}
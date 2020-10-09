
/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	Uart.c 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#include "common.h"

UART_define SDK_UART;


/********************************************************************
Function:	fun_CharToHex(unsigned char bHex)
INPUT	:
OUTPUT	:	
NOTE	:   将16进制转换成ASCII码
********************************************************************/
unsigned char fun_CharToHex(unsigned char bHex)
{
	if((bHex>=0)&&(bHex<=9))
	{
		bHex += 0x30;
	}
	else if((bHex>=10)&&(bHex<=15))//Capital
	{
		bHex += 0x37;
	}
	else 
	{
		bHex = 0xff;
	}
	return bHex;
}
/********************************************************************
Function:	fun_Uart_Init
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Uart_Init(void)
{
	
	_pas17	= 1;
	_pas16	= 1;
	_pas15  = 1;
	_pas14  = 0;
	
//	SET_USIM_UART();	
	SET_UART_ENABLE();			//打开串口
	SET_UART_DATARATE();		//数据
	SET_UART_HIGHSPEED();		//打开高速率
	SET_UART_SEND_ENABLE();		//发送使能
	SET_UART_RECE_ENABLE();		//接收使能
		
/*	_ua0e = 1;*/
	_ure = 1;
	_rie = 1;				//接收中斷使能
	_teie = 1;
//	_tiie = 1;
	_emi	=	1;
   
}

/**************************************************************
 * 	UBRGH = 0 : baud rate = Fh /(64 * (N+1))
 *	UBRGH = 1 : baud rate = Fh /(16 * (N+1))
 *	eg. baud rate = 4m/(16 * (25 + 1)) =  9615 bit/s 约等于9600
 * **************************************************************/



/********************************************************************
Function:	fun_Uart_Off
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Uart_Off(void)
{
	SET_UART_SEND_DISABLE();	
	SET_UART_RECE_DISABLE();
	SET_UART_DISABLE();		
}
/***********************************************************
**   Function : fun_Uart_SendByte
**   INPUT    : data
**   OUTPUT   : none
**   NOTE     : 
**			
***********************************************************/
volatile	u8 lu8v_Uart_temp;
void fun_Uart_SendByte(u8 lu8v_uart_data)
{

	lu8v_Uart_temp = _usr;
	_txr_rxr = lu8v_uart_data;  
	while(!_tidle);							
}

/***********************************************************
**   Function : fun_Uart_SendASCII
**   INPUT    : data
**   OUTPUT   : none
**   NOTE     : 
**			
***********************************************************/
void fun_Uart_SendASCII(u16 uart_ASCIIdata)
{
	fun_Uart_SendByte(0xca);
	fun_Uart_SendByte(0xfd);
	fun_Uart_SendByte(0xd6);
	fun_Uart_SendByte(0xb5);
	fun_Uart_SendByte(0x3a);
	
	fun_Uart_SendByte(fun_CharToHex(uart_ASCIIdata/10000%10));
	
	fun_Uart_SendByte(fun_CharToHex(uart_ASCIIdata/1000%10));

	fun_Uart_SendByte(fun_CharToHex(uart_ASCIIdata/100%10));

	fun_Uart_SendByte(fun_CharToHex(uart_ASCIIdata/10%10));

	fun_Uart_SendByte(fun_CharToHex(uart_ASCIIdata%10));
	
	fun_Uart_SendByte(0x0D);
	fun_Uart_SendByte(0x0A);							
}
/********************************************************************
Function:	fun_Uart_Test
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Uart_Test(void)
{

	fun_Uart_Init();
	while(1)
	{
		GCC_CLRWDT();
		fun_Uart_SendByte(0x11);
		fun_Uart_SendByte(0x22);
		GCC_DELAY(10000);

	}
		
}
/********************************************************************
Function:	fun_Uart_ISR
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/

volatile	u8	array_uart_rxbuff[10];
volatile	u8	array_uart_txbuff[10];
volatile	bit	gbv_rx_success;
volatile	u8	gu8v_tx_guide;

void __attribute((interrupt(0x1c)))  fun_Uart_ISR()
{
	volatile u8	lu8v_isr_temp0;
//	if(_rxif)
	{
	//  	_rxif = 0;								//clear urf flag
	   	if(_perr && _nf && _ferr && _oerr)		// error found?
	   	{
	   	   lu8v_isr_temp0 = _usr;				//read USR to clear error flag    
	   	   lu8v_isr_temp0 = _txr_rxr;
	   	}
	   	else									// no error found
	   	{
	   	   if(_rxif)							//RXR	data register has available	data
	   	   {
	   	   		_rxif = 0;
				for(lu8v_isr_temp0 = 0;lu8v_isr_temp0<4;lu8v_isr_temp0++)
				{
					array_uart_rxbuff[lu8v_isr_temp0] = array_uart_rxbuff[lu8v_isr_temp0+1];		
				}
				array_uart_rxbuff[4] = _txr_rxr; 
				if(array_uart_rxbuff[0]==0x55&&array_uart_rxbuff[4]==(u8)(array_uart_rxbuff[0]+array_uart_rxbuff[1]+array_uart_rxbuff[2]+array_uart_rxbuff[3]))
				{
					gbv_rx_success = 1;
				}	   	   	  
	   	   		GCC_NOP();

	   	   }
   	  	   if(_txif)							//TXR	data register is empty
	   	   {
	   	   		_txif = 0;
				gu8v_tx_guide++; 	   		
	   	   		if(gu8v_tx_guide<6)			//
	   	   		{
		   	   		_txr_rxr= array_uart_txbuff[gu8v_tx_guide];	  	   	   			
	   	   		}
	   	   		else 
	   	   		{
					
	   	   		}
					
	   	   }

	   	}	
	}	

}

void fun_Uart_Handle()
{
	if(gbv_rx_success)
	{
		gbv_rx_success = 0;
		
		if(array_uart_rxbuff[3]==1)
		{
			_flms2 = 0;
			_flms1 = 0;				
			_flms0 = 0;					
		}
		else if(array_uart_rxbuff[3]==0)
		{
			_flms2 = 0;
			_flms1 = 1;				
			_flms0 = 0;					
		}
		
		//speed
		if(array_uart_rxbuff[2]<8)
		{
			_adcr0 &=0b11100011; 
			_adcr0 += array_uart_rxbuff[2]*4;					
		}
		
		//PGA		
		array_uart_rxbuff[1] = 7;
		if(array_uart_rxbuff[1]<8)
		{
			_pgac0 &= 0b11111000;
			_pgac0 += array_uart_rxbuff[1];					
		}		
		

		
		GCC_DELAY(10000);		
		array_uart_txbuff[0] = 0x55;
		array_uart_txbuff[1] = 01;			
		array_uart_txbuff[2] = 00;			
		array_uart_txbuff[3] = 00;			
		array_uart_txbuff[4] = 00;						
		array_uart_txbuff[5] = array_uart_txbuff[0]+array_uart_txbuff[1]+array_uart_txbuff[2]+array_uart_txbuff[3]+array_uart_txbuff[4];						
		gu8v_tx_guide = 0;
		_txr_rxr = array_uart_txbuff[gu8v_tx_guide];			
		GCC_DELAY(10000);	
	}
}






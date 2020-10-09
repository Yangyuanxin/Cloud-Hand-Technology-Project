
/*************************************************************************************/
//**	Copyright	:	2016 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	Uart.c 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	HT67F5652													**
//**	Author   	: 	Leo															**
//**	Date     	: 	2016/10/28													**
//**	Version  	: 	V00															**
//**	History  	:																**
/*************************************************************************************/

#include "..\User\CONFIG.h"

volatile	U8	gu8v_rx_guide;
volatile	U8	gu8v_tx_guide;
volatile	__byte_type	gu8v_uart_flag;
volatile	U8	array_uart_txbuff[10];
volatile	U8	array_uart_rxbuff[6];

/********************************************************************
Function:	fun_Uart_Init
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Uart_Init(void)
{
  	_ucr1 = UCR1_8_N_1;     //enable UART function,8bits-Data  NONE-Parity 1bit-Stop Format
  	_ucr2 = UCR2Data1;      //enable TXEN,RXEN,Hige Speed Baud Rate, Receiver interrupt enable 
	_rie = 1;				//接收中斷使能
 	_brg = BRGData;         //波特率設置
//	_euti = 1;      		//UART interrupt enable
	_uie = 1;      			//UART interrupt enable	
	_mf1e = 1;     
}

/********************************************************************
Function:	fun_Uart_off
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Uart_off(void)
{
	_uarten = 0;
	_ucr1 = 0;
	_ucr2 = 0;	
	_uie = 0;
//	_euti = 0;      		
}


/********************************************************************
Function:	fun_Uart_Test
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void fun_Uart_Test(void)
{
	volatile	U16 lu16v_count;
	fun_Uart_Init();
	while(1)
	{
		GCC_CLRWDT();
		lu16v_count++;
		array_uart_txbuff[0] = 0x55;
		array_uart_txbuff[1] = 0x01;
		array_uart_txbuff[2] = 0x09;
		array_uart_txbuff[3] = 0x02;
		array_uart_txbuff[4] = lu16v_count>>8;
		array_uart_txbuff[5] = lu16v_count;
		array_uart_txbuff[6] = 0;
		array_uart_txbuff[7] = 0X55+0X01+0X09+0X02+0XAA+array_uart_txbuff[4]+array_uart_txbuff[5]+array_uart_txbuff[6];
		array_uart_txbuff[8] = 0XAA;
		gu8v_tx_guide=0;
		_txr_rxr = array_uart_txbuff[0];    	
		GCC_DELAY(10000);

	}
		
}
/********************************************************************
Function:	fun_Uart_ISR
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void __attribute((interrupt(0x1c)))  fun_Uart_ISR()
{
	volatile U8	lu8v_isr_temp0;
	if(_uif)
	{
	  	_uif = 0;								//clear urf flag
	   	if(_perr && _nf && _ferr && _oerr)		// error found?
	   	{
	   	   lu8v_isr_temp0 = _usr;				//read USR to clear error flag     
	   	}
	   	else									// no error found
	   	{
	   	   if(_rxif)							//RXR	data register has available	data
	   	   {
	   	   		//receive data from PC
				array_uart_rxbuff[gu8v_rx_guide] = _txr_rxr; 
				
				if(array_uart_rxbuff[gu8v_rx_guide] == 10)	
				{
					gbv_rx_success = 1;
				}
					
				if(gu8v_rx_guide>=5)
				{
					gu8v_rx_guide = 0 ;					
				}
				else
				{
					gu8v_rx_guide ++ ;						
				}
	   	   	  
	   	   	  
	   	   	  GCC_NOP();
	   	   }
	   	   if(_txif)							//TXR	data register is empty
	   	   {
	   	   		gu8v_tx_guide++; 	   		
	   	   		if(gu8v_tx_guide<=8)			//
	   	   		{
		   	   		_txr_rxr = array_uart_txbuff[gu8v_tx_guide];	  	   	   	
	   	   		}
	   	   	/*	else 
	   	   		{
		   	   		gu8v_tx_guide=0;	
	   	   		}*/
					
	   	   }
	   	}	
	}	


	_sif = 0;
	_uif = 0;
	_lvf = 0;
	_def = 0;
}


/********************************************************************
Function:	fun_Uart_manage
INPUT	:
OUTPUT	:	
NOTE	:   
********************************************************************/
void	fun_Uart_manage(void)
{
//	if(gbv_rx_success == 1)
//	{
//		gbv_rx_success = 0;
//		if(array_uart_rxbuff[0] == 47 && array_uart_rxbuff[5] == 10)
//		{
//			switch(array_uart_rxbuff[1])
//			{
//				//
//				case	0xaa:
//
//				break;
//				
//				default	:
//				
//				break;			
//				
//				
//				
//							
//			}	
//					
//		}
//		else
//		{
//			gu8v_rx_guide = 0;
//			
//		}
//		
//	}
	
}

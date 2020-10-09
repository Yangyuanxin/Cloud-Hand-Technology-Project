

/*************************************************************************************/
//**	Copyright	:	2018 BY HOLTEK SEMICONDUCTOR INC                            **
//** 	File Name   :	Uart.h 	       	                                            **
//**	Description	: 																**
//**	MCU      	: 	BH67F2742													**
//**	Author   	: 	Ljq															**
//**	Date     	: 	2018/06/07													**
//**	Version  	: 	V01															**
//**	History  	:																**
/*************************************************************************************/

#ifndef _UART_H_
#define _UART_H_


typedef struct
{
	u8	            TX;
	u8      	    RX;	
	u8				Count;
	__8_type	    Status;
	u8				Txbuff[10];
	u8				Rxbuff[10];
}UART_define;


extern	volatile	u8	array_uart_rxbuff[10];
extern	volatile	u8	array_uart_txbuff[10];
extern	volatile	bit	gbv_rx_success;
extern	volatile	u8	gu8v_tx_guide;


/********************************************************************
Function:	DEFINE
********************************************************************/
/**************************************************************
 * 	UBRGH = 0 : baud rate = Fh /(64 * (N+1))
 *	UBRGH = 1 : baud rate = Fh /(16 * (N+1))
 *	eg. baud rate = 8m/(16 * (51 + 1)) =  9615 bit/s Ô¼µÈÓÚ9600
 * **************************************************************/
#define		UART_DATA_RATE 				51
#define		SET_UART_DATARATE() 		_brg = UART_DATA_RATE;
//#define		SET_USIM_IIC()				_uumd = 0;
//#define		SET_USIM_UART()				_uumd = 1;
#define		SET_UART_ENABLE()			_uarten = 1;
#define		SET_UART_DISABLE()			_uarten = 0;
#define		SET_UART_SEND_ENABLE()		_txen = 1;
#define		SET_UART_SEND_DISABLE()		_txen = 0;
#define		SET_UART_RECE_ENABLE()		_rxen = 1;
#define		SET_UART_RECE_DISABLE()		_rxen = 0;
#define		SET_UART_LOWSPEED()  		_brgh = 0;
#define		SET_UART_HIGHSPEED() 		_brgh = 1;
#define		SET_UART_ADDR_ENABLE()  	_adden = 1;
#define		SET_UART_ADDR_DISABLE() 	_adden = 0;
#define		SET_UART_WAKEUP_ENABLE()  	_wake = 1;
#define		SET_UART_WAKEUP_DISABLE() 	_wake = 0;
#define		SET_UART_RECEISR_ENABLE()  	_rie = 1;
#define		SET_UART_RECEISR_DISABLE() 	_rie = 0;
#define		SET_UART_SENDISR_ENABLE()  	_tie = 1;
#define		SET_UART_SENDISR_DISABLE() 	_tie = 0;

/********************************************************************
Function:	
********************************************************************/
void 	fun_Uart_Init(void);
void	fun_Uart_Off(void);
void	fun_Uart_Test(void);
void 	fun_Uart_SendByte(u8 lu8v_uart_data);
void 	fun_Uart_Handle();
void 	fun_Uart_SendASCII(u16 uart_ASCIIdata);

#endif



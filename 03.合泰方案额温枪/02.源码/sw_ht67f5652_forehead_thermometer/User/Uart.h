

#ifndef _UART_H_
#define _UART_H_




/********************************************************************
Function:	DEFINE
********************************************************************/
#define		cmd_return_op1adnum			30
#define		cmd_return_op2adnum			31
#define		cmd_adjust_bandgap_up		32
#define		cmd_adjust_bandgap_down		33
#define		cmd_data_to_pref			34
#define		cmd_data_to_rom				35
#define		cmd_adjust_daco_up			37
#define		cmd_adjust_daco_down		38
#define		UCR1_8_N_1     				0x080     //8bits-Data  NONE-Parity 1bit-Stop Format
#define 	UCR1_7_O_1     				0x0B0     //7bits-Data  Odd-Parity  1bit-Stop Format
#define		UCR1_7_E_1     				0x0A0     //7bits-Data  Even-Parity 1bit-Stop Format
#define		UCR1_9_N_1     				0x0C0     //9bits-Data  NONE-Parity 1bit-Stop Format
#define		UCR1_8_O_1    				0x0F0     //8bits-Data  Odd-Parity  1bit-Stop Format
#define		UCR1_8_E_1     				0x0E0     //8bits-Data  Even-Parity 1bit-Stop Format
#define		UCR1_8_N_2     				0x088     //8bits-Data  NONE-Parity 2bits-Stop Format
#define		UCR1_7_O_2     				0x0B8     //7bits-Data  Odd-Parity  2bits-Stop Format
#define		UCR1_7_E_2     				0x0A8     //7bits-Data  Even-Parity 2bits-Stop Format
#define		UCR1_9_N_2     				0x0C8     //9bits-Data  NONE-Parity 2bits-Stop Format
#define		UCR1_8_O_2     				0x0F8     //8bits-Data  Odd-Parity  2bits-Stop Format
#define		UCR1_8_E_2     				0x0E8     //8bits-Data  Even-Parity 2bits-Stop Format
#define		UCR2Data       				0x0E4     //Enable TXEN RXEN BRGH RIE 
#define		UCR2Data1       			0x0Ef     //Enable TXEN RXEN BRGH RIE 
#define		BRGData        				30//33		  //4mhz 9600 (No.4)



/********************************************************************
Function:	
********************************************************************/
extern	volatile	U8	gu8v_rx_guide;
extern	volatile	U8	gu8v_tx_guide;
extern	volatile	U8	array_uart_txbuff[10];
extern	volatile	U8	array_uart_rxbuff[6];
extern	volatile	__byte_type	gu8v_uart_flag;
#define gbv_tx_success 			gu8v_uart_flag.bits.b0
#define gbv_rx_success			gu8v_uart_flag.bits.b1
#define gu8v_uart_flag2 		gu8v_uart_flag.bits.b2
#define gu8v_uart_flag3 		gu8v_uart_flag.bits.b3
#define gu8v_uart_flag4			gu8v_uart_flag.bits.b4
#define gu8v_uart_flag5	 		gu8v_uart_flag.bits.b5
#define gu8v_uart_flag6			gu8v_uart_flag.bits.b6
#define gu8v_uart_flag7			gu8v_uart_flag.bits.b7

/********************************************************************
Function:	
********************************************************************/
void 	fun_Uart_Init(void);
void	fun_Uart_off(void);
void	fun_Uart_manage(void);
void fun_Uart_Test(void);
#endif



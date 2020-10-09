
#ifndef _BEEP_H_
#define _BEEP_H_
/********************************************************************
Function:	BEEP GPIO
********************************************************************/

#define		pin_GPIO_BEEP_IO	_pcc5
#define		pin_GPIO_BEEP		_pc5
#define		mac_Beep_High()		{pin_GPIO_BEEP = 1;}
#define		mac_Beep_Low()		{pin_GPIO_BEEP = 0;}
#define		mac_beep_1			1
#define		mac_beep_1_1		2
#define		mac_beep_1_1_1_1	3





/********************************************************************
Function:	
********************************************************************/

void fun_Beep_work_mode(void);
void fun_Beep_init(U8	lu8c_beep_mode);

#endif


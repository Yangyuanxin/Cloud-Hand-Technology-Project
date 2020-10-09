
#ifndef _EEPROM_H__
#define _EEPROM_H__

/********************************************************************
NOTE:
********************************************************************/
#define	mac_eeprom_Long		1
#define	mac_eeprom_Short	0
#define mac_eeprom_type		mac_eeprom_Long



#define	mac_EEprom_Keep_NUM			10
#define mac_EEprom_Keep_StartAdd	0x10

volatile	static	__16_type	gu16v_EEprom_Keep_Buff[10]	__attribute__ ((at(0x2b0)));

void fun_EEprom_Write(U8 addr,U8 data1);
U8	fun_EEprom_Read(U8 addr);
void fun_EEprom_Keep_R();
void fun_EEprom_Keep_W();
#endif
#ifndef _GPIO_DEFINE_H_
#define _GPIO_DEFINE_H_



// #define		GPIO_LED0_IO	_pdc7
// #define		GPIO_LED0		_pd7
// #define		GPIO_GET_ZERO_IO	_pcc6
// #define		GPIO_GET_ZERO		_pc6
// #define		Backlight_on()		GPIO_LED0 = 1;
// #define		Backlight_off()		GPIO_LED0 = 0;


/********************************************************************
Function:	LED GPIO
********************************************************************/

#if	(mac_MCU_Pin == mac_MCU_P80)
#define		pin_LED_G_IO		_pcc2
#define		pin_LED_G			_pc2
#define		pin_LED_R_IO		_pcc3
#define		pin_LED_R			_pc3

#endif

#if	(mac_MCU_Pin == mac_MCU_P64)

#define		pin_LED_G_IO		_pcc0
#define		pin_LED_G			_pc0
#define		pin_LED_R_IO		_pcc1
#define		pin_LED_R			_pc1

#endif







#define 	mac_LED_G_Light()	{pin_LED_G = 1;pin_LED_R = 0;}
#define 	mac_LED_R_Light()	{pin_LED_G = 0;pin_LED_R = 1;}
#define 	mac_LED_Y_Light()	{pin_LED_G = 1;pin_LED_R = 1;}
#define 	mac_LED_DisLight()	{pin_LED_G = 0;pin_LED_R = 0;}




#endif
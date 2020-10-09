#ifndef __RGB_Dispose_H
#define __RGB_Dispose_H

#include "stm32f10x.h"
#include "key.h"
#include "key_detect.h"
#include "usart.h"
#include "public.h"
#include "Timer.h"
#include "Lsens.h"
#include "../BOARD/ws2812/ws2812.h"
#include "delay.h"



#define  RGB_Lum			250			//LED亮度设置
#define  RGB_Graded		10			//LED亮度梯度
#define  Set_Wait			50			//LED间隔时间  单位ms
#define  Auto_ScanTime_Set	20	//自动模式的LED扫描时间  单位ms


typedef struct 
{
	uint8_t  R_Cnt;						//红灯数值
	uint8_t  G_Cnt;						//绿灯数值
	uint8_t  B_Cnt;						//蓝灯数值
	uint8_t  Level_Cnt;					//增加计数
}RGB_Variable;




enum  S_RGB_SingleColor
{
	White = 0,			//白色
	Red,						//红色
	Blue,						//蓝色
	Green,					//绿色
	Yellow,					//黄色（红绿）
	cyan,						//青色（蓝绿）
	Violet,					//紫色（蓝红）
};




enum  S_RGB_STATE
{
		RGB_OFF = 0,
		Automatic_LED_Mode,					//自动调节模式
		SingleColor_Mode,						//单色调模式
		ColorWipe_Mode,							//流水灯模式
		Rainbow_Mode,
		RainbowCycle_Mode,
		TheaterChase_Mode,
		TheaterChaseRainbow_Mode,
};

void key0_CallBack(void);
void key0_S_CallBack(void);
void key0_L_CallBack(void);
void key1_CallBack(void);
void key2_CallBack(void);
void key3_CallBack(void);
void Update_Key_CallBack(void);
void Print_Debug_mess(unsigned char *debug_Mess);

void  RGB_StateDispose(void);
void  SingleColor_ModeDispose(void);
void  ColorWipe_ModeDispose(void);
void  TheaterChase_ModeDispose(void);
#endif /* __RGB_Dispose_H */


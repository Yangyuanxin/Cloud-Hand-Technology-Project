#include "RGB_Dispose.h"


uint8_t  RGB_state = 0;

uint8_t  Color_state = 0;

uint16_t  LED_Level= 0;


uint16_t  LED_CNT= 0;
RGB_Variable  RGB_Val;
//打印组件调试信息
void Print_Debug_mess(unsigned char *debug_Mess)
{
	USART_Send_Str(USART1, debug_Mess);
}
//按键0回调函数:打印按键以及打印按键版本号
void key0_CallBack(void)
{
	static char i=0;

	Print(USART1,"key0_CallBack:KEY0 is triggered.\r\n",Get_Version_Mess);
	if(i==0)
	{
		i=1;
		Reg_Key(&KEY0, 100, L_Trig, D_Click, key0_CallBack);//双击
	}
}

void key0_S_CallBack(void)
{
	Print(USART1,"key0_S_CallBack:KEY0 is triggered.\r\n",Get_Version_Mess);
	

	
	RGB_state++;


	ws281x_closeAll();

	
	RGB_Val.R_Cnt = RGB_Lum;
	RGB_Val.G_Cnt = RGB_Lum;
	RGB_Val.B_Cnt = RGB_Lum;
	RGB_Val.Level_Cnt = RGB_Lum;
	Color_state = White;
	ws281x_Init_T();
	
	if(RGB_state == ColorWipe_Mode)
		Color_state = Red;
	else
		Color_state = White;
	
	
	if(RGB_state > TheaterChaseRainbow_Mode)
	{
				RGB_state = RGB_OFF; 
	}
}

void key0_L_CallBack(void)
{
	Print(USART1,"key0_L_CallBack:KEY0 is triggered.\r\n",Get_Version_Mess);
}

//按键1回调函数
void key1_CallBack(void)
{
	Print(USART1,"KEY1 is triggered.\r\n");

	RGB_Val.R_Cnt = RGB_Lum;
	RGB_Val.G_Cnt = RGB_Lum;
	RGB_Val.B_Cnt = RGB_Lum;
	RGB_Val.Level_Cnt = RGB_Lum;
	
	ws281x_Init_T();
	
	Color_state++;
	if(Color_state > Violet)
	{
		if(RGB_state == ColorWipe_Mode)
			Color_state = Red;
		else
			Color_state = White;
	}
	
}

//按键2回调函数
void key2_CallBack(void)
{
	Print(USART1,"KEY2 is triggered.\r\n");
	
	
	if(RGB_Val.Level_Cnt >= RGB_Lum)
	{
			RGB_Val.R_Cnt = RGB_Lum;
			RGB_Val.G_Cnt = RGB_Lum;
			RGB_Val.B_Cnt = RGB_Lum;
	}
	else
	{
			RGB_Val.Level_Cnt+=RGB_Graded;
			RGB_Val.R_Cnt = RGB_Val.Level_Cnt;
			RGB_Val.G_Cnt = RGB_Val.Level_Cnt;
			RGB_Val.B_Cnt = RGB_Val.Level_Cnt;
	
	}
}

//按键3回调函数
void key3_CallBack(void)
{
	Print(USART1,"KEY3 is triggered.\r\n");
	if(RGB_Val.Level_Cnt <= 0)
	{
			RGB_Val.R_Cnt = 0;
			RGB_Val.G_Cnt = 0;
			RGB_Val.B_Cnt = 0;
	}
	else
	{
			RGB_Val.Level_Cnt-=RGB_Graded;
			RGB_Val.R_Cnt = RGB_Val.Level_Cnt;
			RGB_Val.G_Cnt = RGB_Val.Level_Cnt;
			RGB_Val.B_Cnt = RGB_Val.Level_Cnt;
	}
}
//更新按键状态
void Update_Key_CallBack(void)
{
	KEY0=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1);	//读取按键0状态
	KEY1=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2);	//读取按键1状态
	KEY2=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3);	//读取按键2状态
	KEY3=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4);	//读取按键3状态
}






void  RGB_StateDispose(void)
{

	switch(RGB_state)
		{
			case RGB_OFF:
						if(!Time_RGBWait_cnt)
						{
							ws281x_closeAll();  //关闭全部的灯
							Time_RGBWait_cnt = Set_Wait;
						}
						
			break;
		
			case Automatic_LED_Mode:			//自动调节模式
						if(!Time_Auto_cnt)
						{
							Time_Auto_cnt = Auto_ScanTime_Set;
//							ws281x_closeAll(); 
							LED_Level = Lsens_Get_Val();
							Print(USART1,"亮度等级为：%d\r\n",LED_Level);
							Print(USART1,"亮度为：%d\r\n",LED_Level/16);
							if(LED_Level < 3890)
							{
								LED_CNT++;
								if(LED_CNT >=10)
								{
								LED_CNT = 0;
								LED_Level = 0;
								}
							 
							}
							else
								LED_CNT = 0;
						}
//						if(LED_Level < 3890)
//						{
//							ws281x_closeAll(); 
//						}
//						else
//						{
						ws281x_singleColor_T(ws281x_color(LED_Level/16, LED_Level/16, LED_Level/16), Set_Wait);
//						}
			break;
						
			case SingleColor_Mode:
						SingleColor_ModeDispose();
			break;
			
			case ColorWipe_Mode:
						ColorWipe_ModeDispose();
			break;
			
			case Rainbow_Mode:
						ws281x_rainbow_T(200);
			break;
			
			case RainbowCycle_Mode:
						ws281x_rainbowCycle_T(200);
			break;
			
			case TheaterChase_Mode:
						TheaterChase_ModeDispose();
			break;
			
			case TheaterChaseRainbow_Mode:
						ws281x_theaterChaseRainbow_T(Set_Wait);
			break;
		
		}
}




void  SingleColor_ModeDispose(void)
{
		switch(Color_state)
		{
		  case White:
						ws281x_singleColor_T(ws281x_color(RGB_Val.R_Cnt, RGB_Val.G_Cnt, RGB_Val.B_Cnt), Set_Wait);
			break;
			case Red:
						ws281x_singleColor_T(ws281x_color(RGB_Val.R_Cnt, 0, 0), Set_Wait);
			break;
			case Blue:
						ws281x_singleColor_T(ws281x_color(0, 0, RGB_Val.B_Cnt), Set_Wait);
			break;
			case Green:
						ws281x_singleColor_T(ws281x_color(0, RGB_Val.G_Cnt, 0), Set_Wait);
			break;
			case Yellow:
						ws281x_singleColor_T(ws281x_color(RGB_Val.R_Cnt, RGB_Val.G_Cnt, 0), Set_Wait);
			break;
			case cyan:
						ws281x_singleColor_T(ws281x_color(0, RGB_Val.G_Cnt, RGB_Val.B_Cnt), Set_Wait);
			break;
			case Violet:
						ws281x_singleColor_T(ws281x_color(RGB_Val.R_Cnt, 0, RGB_Val.B_Cnt), Set_Wait);
			break;
		}
}

void  ColorWipe_ModeDispose(void)
{
		switch(Color_state)
		{
		  case White:
						ws281x_colorWipe_T(ws281x_color(255, 255, 255), Set_Wait);
//						ws281x_colorWipe_T(ws281x_color(RGB_Val.R_Cnt, 0, 0), Set_Wait);
			break;
			case Red:
						ws281x_colorWipe_T(ws281x_color(RGB_Val.R_Cnt, 0, 0), Set_Wait);
			break;
			case Blue:
						ws281x_colorWipe_T(ws281x_color(0, 0, RGB_Val.B_Cnt), Set_Wait);
			break;
			case Green:
						ws281x_colorWipe_T(ws281x_color(0, RGB_Val.G_Cnt, 0), Set_Wait);
			break;
			case Yellow:
						ws281x_colorWipe_T(ws281x_color(RGB_Val.R_Cnt, RGB_Val.G_Cnt, 0), Set_Wait);
			break;
			case cyan:
						ws281x_colorWipe_T(ws281x_color(0, RGB_Val.G_Cnt, RGB_Val.B_Cnt), Set_Wait);
			break;
			case Violet:
						ws281x_colorWipe_T(ws281x_color(RGB_Val.R_Cnt, 0, RGB_Val.B_Cnt), Set_Wait);
			break;
		}
}

void  TheaterChase_ModeDispose(void)
{
		switch(Color_state)
		{
		  case White:
						ws281x_theaterChase_T(ws281x_color(RGB_Val.R_Cnt, RGB_Val.G_Cnt, RGB_Val.B_Cnt), Set_Wait);
			break;
			case Red:
						ws281x_theaterChase_T(ws281x_color(RGB_Val.R_Cnt, 0, 0), Set_Wait);
			break;
			case Blue:
						ws281x_theaterChase_T(ws281x_color(0, 0, RGB_Val.B_Cnt), Set_Wait);
			break;
			case Green:
						ws281x_theaterChase_T(ws281x_color(0, RGB_Val.G_Cnt, 0), Set_Wait);
			break;
			case Yellow:
						ws281x_theaterChase_T(ws281x_color(RGB_Val.R_Cnt, RGB_Val.G_Cnt, 0), Set_Wait);
			break;
			case cyan:
						ws281x_theaterChase_T(ws281x_color(0, RGB_Val.G_Cnt, RGB_Val.B_Cnt), Set_Wait);
			break;
			case Violet:
						ws281x_theaterChase_T(ws281x_color(RGB_Val.R_Cnt, 0, RGB_Val.B_Cnt), Set_Wait);
			break;
		}
}

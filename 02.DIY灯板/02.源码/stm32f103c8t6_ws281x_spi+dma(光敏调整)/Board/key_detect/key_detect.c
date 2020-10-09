/**
  ******************************************************************************
  * @file    key_detect.c
  * @author  YaoCheng
  * @version V1.1
  * @date    2018-4-28
  * @brief   功能：按键检测
  *			 		 版本更新信息：[2018-04-28 22:00]  V1.0.18.4.28 创建。
  *			 		 版本更新信息：[2018-05-29 15:00]  V1.1.18.5.29 更新按键检测逻辑。
  ******************************************************************************
  * @attention
  *
  * 参考资料：参考了魔电物联的源码和它的框架。
	
	*	注：1、在初始化按键（Init_Key_Struct）时，Debug_CallBack回调函数是用来打印组件异常信息的，如果不需要打印，可以写NULL。
	*			2、Update_Key_CallBack回调函数是用来更新按键状态的，这个函数必须实现，不能写NULL。
	*			3、在注册按键（Reg_Key）时，同一个按键相应回调函数（Key_Click_CallBack）只能注册一个，多次使用同一个按键相应回调函
	*			数（Key_Click_CallBack）注册多个按键只会保留最后一次注册的按键，前面重复注册的按键会被覆盖掉。
  *
  ******************************************************************************
  */ 


#include "key_detect.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


/** 
  * @brief key_detect组件版本号
  */
#define  Version     "V1.1.18.5.29"


/** 
  * @brief 	按键信息结构体
  */
typedef struct
{
	unsigned short Click_Count;													/*!< 计数 */
	unsigned short Count;																/*!< 最大计数值 */
	unsigned char *Key;																	/*!< 按键状态 */
	Trig_Mode_TypeDef Trig_Mode_E;											/*!< 按键触发模式 */
	Key_Mode_TypeDef  Key_Mode_E;												/*!< 按键模式 */
	unsigned char Do_S;																	/*!< 回调执行状态 */
	void (*Key_Click_CallBack)(void);										/*!< 按键点击回调函数（指针函数） */
}Key_Mess_TypeDef;

/** 
  * @brief 按键结构体
  */
typedef struct
{
  Key_Mess_TypeDef Key_Mess_S[Key_Num_Max];						/*!< Key_Mess_TypeDef结构体 */
  unsigned char Reg_Key_Num;													/*!< 注册按键数量 */
  void (*Update_Key_CallBack)(void);									/*!< 更新按键状态回调函数（指针函数） */
  void (*Debug_CallBack)(unsigned char *debug_mess);	/*!< Key_Detect调试回调函数（指针函数） */
}Key_TypeDef;


/** 
  * @brief Key结构体
  */
static Key_TypeDef Key;


/** 
  * @name   Print_Debug_Mess
  *
  * @brief  打印Key_Detect调试信息(最多一次性打印255个字节)
  *
  * @param  debug_mess：ringBuffer调试信息
  *
  * @retval 无
  */
static void Print_Debug_Mess(const char *debug_mess,...)
{
	va_list ap;
	unsigned char buf[255];
	unsigned char *pStr = buf;
	
	if(Key.Debug_CallBack == NULL) return;
	va_start(ap, debug_mess);
	vsnprintf((char *)buf, sizeof(buf), debug_mess, ap);				
	va_end(ap);
	Key.Debug_CallBack(pStr);
}

/** 
  * @name   Init_Key_Struct
  *
  * @brief  初始化按键
  *
  * @param  Update_Key_CallBack：更新按键状态
  * @param  Debug_CallBack：打印按键调试信息
  *
  * @retval 0：成功；
	*					1：Update_Key_CallBack == NULL；
  */
char Init_Key_Struct(void (*Update_Key_CallBack)(void),void (*Debug_CallBack)(unsigned char *debug_mess))
{
	unsigned int count=0;																/* 计数 */
	if(Update_Key_CallBack == NULL) return 1;						/* Update_Key_CallBackNULL */
	
	for(count=0;count<Key_Num_Max;count++)							/* 遍历Key_Mess_S数组 */
	{
		Key.Key_Mess_S[count].Click_Count=0;							/* 计数 */
		Key.Key_Mess_S[count].Count=0;										/* 按键最大计数值 */
		Key.Key_Mess_S[count].Do_S=0;											/* 初始化按键点击回调函数调用状态 */
		Key.Key_Mess_S[count].Key_Click_CallBack=NULL;		/* 初始化按键点击回调函数 */
		Key.Key_Mess_S[count].Key_Mode_E=N_Click;					/* 初始化按键模式 */
		Key.Key_Mess_S[count].Key=NULL;										/* 初始化按键状态 */
		Key.Key_Mess_S[count].Trig_Mode_E=N_Trig;					/* 初始化按键触发模式 */
	}
	Key.Reg_Key_Num = 0;																/* 设置注册的按键数量为0 */
	Key.Update_Key_CallBack = Update_Key_CallBack;			/* 设置更新按键状态回调函数 */
	Key.Debug_CallBack = Debug_CallBack;								/* 添加调试回调函数 */
	return 0;
}

/** 
  * @name   Reg_Key
  *
  * @brief  添加注册按键（注：如果按键已经注册过，那么再次注册会覆盖之前注册过的相同的按键）
  *
  * @param  key_s：按键状态
  * @param  count：按键计数
  * @param  Trig_Mode_E：按键触发模式
  * @param  Key_Mode_E：按键模式
  * @param  Key_Click_CallBack：按键触发回调
  *
  * @retval 0：成功；
	*					1：Key_Click_CallBack == NULL；
	*					2：Key.Reg_Key_Num > Key_Num_Max；
  */
char Reg_Key(unsigned char *key_s,const unsigned short count, 
	Trig_Mode_TypeDef Trig_Mode_E, Key_Mode_TypeDef  Key_Mode_E, void (*Key_Click_CallBack)(void))
{
	unsigned int temp_count=0;																									/* 计数 */
						
	if(Key_Click_CallBack == NULL)																							/* CallBack为NULL返回2 */
	{
		Print_Debug_Mess("Regist_Key:Key_Click_CallBack = NULL\r\n");
		return 1;	
	}	

	for(temp_count=0;temp_count<Key_Num_Max;temp_count++)												/* 遍历Key_Mess_S数组 */
	{
		if(Key.Key_Mess_S[temp_count].Key_Click_CallBack != NULL)									/* 判断按键状态地址是否为NULL */
		{
			if(Key_Click_CallBack == Key.Key_Mess_S[temp_count].Key_Click_CallBack)
			{
				Key.Key_Mess_S[temp_count].Click_Count=0;															/* 按键计数次数 */
				Key.Key_Mess_S[temp_count].Count=count;																/* 按键最大计数值 */
				Key.Key_Mess_S[temp_count].Key=key_s;																	/* 按键状态 */
				Key.Key_Mess_S[temp_count].Trig_Mode_E=Trig_Mode_E;										/* 按键触发模式 */
				Key.Key_Mess_S[temp_count].Key_Mode_E = Key_Mode_E;										/* 按键模式 */
				Key.Key_Mess_S[temp_count].Key_Click_CallBack=Key_Click_CallBack;			/* 按键触发回调函数 */
				Key.Key_Mess_S[temp_count].Do_S=0;																		/* 按键回调执行状态 */
				return 0;
			}
		}
	}
	
	if(Key.Reg_Key_Num > Key_Num_Max)																						/* 按键注册数量大于按键注册的允许的最大值 */
	{
		Print_Debug_Mess("Regist_Key:Key.Reg_Key_Num > Key_Num_Max\r\n");
		return 2;	
	}

	Key.Key_Mess_S[Key.Reg_Key_Num].Click_Count=0;															/* 按键计数次数 */
	Key.Key_Mess_S[Key.Reg_Key_Num].Count=count;																/* 按键最大计数值 */
	Key.Key_Mess_S[Key.Reg_Key_Num].Key=key_s;																	/* 按键状态 */
	Key.Key_Mess_S[Key.Reg_Key_Num].Trig_Mode_E=Trig_Mode_E;										/* 按键触发模式 */
	Key.Key_Mess_S[Key.Reg_Key_Num].Key_Mode_E = Key_Mode_E;										/* 按键模式 */
	Key.Key_Mess_S[Key.Reg_Key_Num].Key_Click_CallBack=Key_Click_CallBack;			/* 按键触发回调函数 */
	Key.Key_Mess_S[Key.Reg_Key_Num].Do_S=0;																			/* 按键回调执行状态 */
	
	Key.Reg_Key_Num++;

	return 0;																																		/* 重置key成功 */															
}

/** 
  * @name   Clear_Click_Num
  *
  * @brief  将按键的点击次数置为0
  *
  * @param  Key_Number：第几个按键
  *
  * @retval 无
  */
static void Clear_Click_Num(unsigned char key_num)
{
	if(key_num >= Key_Num_Max) 																									/* 检查按键结构体数组索引是否大于按键数量的最大值 */
	{
		Print_Debug_Mess("Clear_Click_Num:key_num > Key_Num_Max\r\n");
	}		
	
	Key.Key_Mess_S[key_num].Click_Count=0;																			/* 清空按键计数 */
	Key.Key_Mess_S[key_num].Do_S=0;																							/* 清空按键回调执行状态 */
}

/** 
  * @name   Key_Click
  *
  * @brief  按键单击事件
  *
  * @param  key_num：第几个按键
  *
  * @retval 无
  */
static void Key_Click(unsigned char key_num)
{
	if(key_num >= Key_Num_Max) 																								/* 检查按键结构体数组索引是否大于按键数量的最大值 */
	{
		Print_Debug_Mess("Key_Click:key_num > Key_Num_Max\r\n");
		return;	
	}
			
	if(Key.Key_Mess_S[key_num].Trig_Mode_E==L_Trig)														/* 低电平触发 */
	{
		if(*Key.Key_Mess_S[key_num].Key==0)																			/* 按键按下 */
		{
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count)	/* 判断按键低电平计数 */
			{
				Key.Key_Mess_S[key_num].Click_Count++;															/* 按键低电平计数 */
			}
			if(Key.Key_Mess_S[key_num].Click_Count>=Key.Key_Mess_S[key_num].Count)/* 判断按键低电平计数 */
			{
				if(Key.Key_Mess_S[key_num].Do_S==0 )
				{
					Key.Key_Mess_S[key_num].Key_Click_CallBack();											/* 调用按键回调函数 */
					Key.Key_Mess_S[key_num].Do_S=1;																		/* 按键回调函数执行状态置1 */
				}
			}
		}
		else if(*Key.Key_Mess_S[key_num].Key==1)																/* 按键松开 */
		{
			Clear_Click_Num(key_num);																							/* 将按键的点击次数置为0 */
		}
	}
	else if(Key.Key_Mess_S[key_num].Trig_Mode_E==H_Trig)											/* 高电平触发 */
	{
		if(*Key.Key_Mess_S[key_num].Key==1)																			/* 按键按下 */
		{
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count)	/* 判断按键低电平计数 */
			{
				Key.Key_Mess_S[key_num].Click_Count++;															/* 按键低电平计数 */
			}
			if(Key.Key_Mess_S[key_num].Click_Count>=Key.Key_Mess_S[key_num].Count)/* 判断按键低电平计数 */
			{
				if(Key.Key_Mess_S[key_num].Do_S==0 )
				{
					Key.Key_Mess_S[key_num].Key_Click_CallBack();											/* 调用按键回调函数 */
					Key.Key_Mess_S[key_num].Do_S=1;																		/* 按键回调函数执行状态置1 */
				}
			}
		}
		else if(*Key.Key_Mess_S[key_num].Key==0)																/* 按键松开 */
		{
			Clear_Click_Num(key_num);																							/* 将按键的点击次数置为0 */
		}
	}
}

/** 
  * @name   Key_D_Click
  *
  * @brief  按键双击事件
  *
  * @param  key_num：第几个按键
  *
  * @retval 无
  */
static void Key_D_Click(unsigned char key_num)
{
	if(key_num >= Key_Num_Max) 																										/* 检查按键结构体数组索引是否大于按键数量的最大值 */
	{
		Print_Debug_Mess("Key_D_Click:key_num > Key_Num_Max\r\n");
		return;	
	}	
			
	if(Key.Key_Mess_S[key_num].Trig_Mode_E==L_Trig)																/* 低电平触发 */
	{
		if(*Key.Key_Mess_S[key_num].Key==0)																					/* 判断按键是否是低电平 */
		{
			/* 计数 */
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count)			/* 检查按键低电平点击次数 */
			{
				Key.Key_Mess_S[key_num].Click_Count++;																	/* 按键高电平点击次数++ */
			}
			if(Key.Key_Mess_S[key_num].Click_Count>=Key.Key_Mess_S[key_num].Count)		/* 判断按键低电平点击次数 */
			{
				if(Key.Key_Mess_S[key_num].Do_S==1)
				{
					Key.Key_Mess_S[key_num].Key_Click_CallBack();													/* 调用按键回调函数 */
					Key.Key_Mess_S[key_num].Do_S=2;																				/* 按键回调函数执行状态置1 */
				}
			}
		}
		else if(*Key.Key_Mess_S[key_num].Key==1)																		/* 判断按键状态是否为高电平 */
		{
			/* 计数 */
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count && 
				Key.Key_Mess_S[key_num].Do_S>0)																					/* 检查按键低电平点击次数 */
			{
				Key.Key_Mess_S[key_num].Click_Count++;																	/* 按键高电平点击次数++ */
			}
			if(Key.Key_Mess_S[key_num].Click_Count > 0 && 
				Key.Key_Mess_S[key_num].Click_Count <= Key.Key_Mess_S[key_num].Count &&
				Key.Key_Mess_S[key_num].Do_S==0)																				/* 判断按键高电平的点击次数是否大于0 */
			{
				Key.Key_Mess_S[key_num].Do_S=1;
			}
			else if(Key.Key_Mess_S[key_num].Click_Count >= Key.Key_Mess_S[key_num].Count)
			{
				Clear_Click_Num(key_num);																								/* 将按键的点击次数置为0 */
			}
		}
	}
	else if(Key.Key_Mess_S[key_num].Trig_Mode_E==H_Trig)													/* 高电平触发 */
	{
		if(*Key.Key_Mess_S[key_num].Key==1)																					/* 判断按键是否是高电平 */
		{
			/* 计数 */
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count)			/* 检查按键低电平点击次数 */
			{
				Key.Key_Mess_S[key_num].Click_Count++;																	/* 按键高电平点击次数++ */
			}
			if(Key.Key_Mess_S[key_num].Click_Count>=Key.Key_Mess_S[key_num].Count)		/* 判断按键高电平点击次数*/
			{
				if(Key.Key_Mess_S[key_num].Do_S==1)
				{
					Key.Key_Mess_S[key_num].Key_Click_CallBack();													/* 调用按键回调函数 */
					Key.Key_Mess_S[key_num].Do_S=2;																				/* 按键回调函数执行状态置1 */
				}
			}
		}
		else if(*Key.Key_Mess_S[key_num].Key==0)																		/* 判断按键状态是否为低电平 */
		{
			/* 计数 */
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count && 
				Key.Key_Mess_S[key_num].Do_S>0)																					/* 检查按键低电平点击次数 */
			{
				Key.Key_Mess_S[key_num].Click_Count++;																	/* 按键高电平点击次数++ */
			}
			if(Key.Key_Mess_S[key_num].Click_Count > 0 && 
				Key.Key_Mess_S[key_num].Click_Count <= Key.Key_Mess_S[key_num].Count &&
				Key.Key_Mess_S[key_num].Do_S==0)																				/* 判断按键高电平的点击次数是否大于0 */
			{
				Key.Key_Mess_S[key_num].Do_S=1;
			}
			else if(Key.Key_Mess_S[key_num].Click_Count >= Key.Key_Mess_S[key_num].Count)
			{
				Clear_Click_Num(key_num);																								/* 将按键的点击次数置为0 */
			}
		}
	}
}

/** 
  * @name   Key_L_Press
  *
  * @brief  按键长按事件
  *
  * @param  无
  * @param  key_num：第几个按键
  *
  * @retval 无
  */
static void Key_L_Press(unsigned char key_num)
{
	if(key_num >= Key_Num_Max) 																									/* 检查按键结构体数组索引是否大于按键数量的最大值 */
	{
		Print_Debug_Mess("Key_L_Press:key_num > Key_Num_Max\r\n");
		return;	
	}	
	
	if(Key.Key_Mess_S[key_num].Trig_Mode_E==L_Trig)															/* 低电平触发 */
	{
		if(*Key.Key_Mess_S[key_num].Key==0)																				/* 按键按下 */
		{
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count) 	/* 判断按键低电平计数 */
			{
				Key.Key_Mess_S[key_num].Click_Count++;																/* 按键低电平计数 */
			}
			if(Key.Key_Mess_S[key_num].Click_Count>=Key.Key_Mess_S[key_num].Count)	/* 判断按键低电平计数 */
			{
				Key.Key_Mess_S[key_num].Key_Click_CallBack();													/* 调用按键回调函数 */
			}
		}
		else if(*Key.Key_Mess_S[key_num].Key==1)																	/* 按键松开 */
		{
			Clear_Click_Num(key_num);																								/* 将按键的点击次数置为0 */
		}
	}
	else if(Key.Key_Mess_S[key_num].Trig_Mode_E==H_Trig)												/* 高电平触发 */
	{
		if(*Key.Key_Mess_S[key_num].Key==1)																				/* 按键按下 */
		{
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count) 	/* 判断按键低电平计数 */
			{
				Key.Key_Mess_S[key_num].Click_Count++;																/* 按键低电平计数 */
			}
			if(Key.Key_Mess_S[key_num].Click_Count>=Key.Key_Mess_S[key_num].Count) 	/* 判断按键低电平次数 */
			{
				Key.Key_Mess_S[key_num].Key_Click_CallBack(); 												/* 调用按键回调函数 */
			}
		}
		else if(*Key.Key_Mess_S[key_num].Key==0)																	/* 按键松开 */
		{
			Clear_Click_Num(key_num);																								/* 将按键的点击次数置为0 */
		}
	}
}

/** 
  * @name   Key_Detect
  *
  * @brief  按键检测
  *
  * @param  无
  *
  * @retval 0：成功；
	*					1：Key.Update_Key_CallBack == NULL；
  */
char Key_Detect(void)
{
	unsigned char count=0;																	/* 计数 */

	if(Key.Update_Key_CallBack == NULL)											/* Key.Update_Key_CallBackNULL */
	{
		Print_Debug_Mess("Key_Detect:Key.Update_Key_CallBack = NULL\r\n");
		return 2;	
	}		

	Key.Update_Key_CallBack();															/* 更新按键状态 */
	for(count=0;count<Key.Reg_Key_Num;count++)
	{
		if(Key.Key_Mess_S[count].Key == NULL) continue;				/* 判断此索引的按键是否注册了按键 */
		if(Key.Key_Mess_S[count].Trig_Mode_E==L_Trig)
		{
			switch (Key.Key_Mess_S[count].Key_Mode_E)
			{
				case S_Click:																			/*!< 单击 */	
					Key_Click(count);
					break;
					
				case D_Click:																			/*!< 双击 */
					Key_D_Click(count);
					break;
					
				case L_Press:																			/*!< 长按 */
					Key_L_Press(count);
					break;
					
				default:
					break;
			}
		}
		else if(Key.Key_Mess_S[count].Trig_Mode_E==H_Trig)
		{
			switch (Key.Key_Mess_S[count].Key_Mode_E)
			{
				case S_Click:																			/*!< 单击 */	
					Key_Click(count);
					break;
					
				case D_Click:																			/*!< 双击 */
					Key_D_Click(count);
					break;
					
				case L_Press:																			/*!< 长按 */
					Key_L_Press(count);
					break;

				default:
					break;
			}
		}
	}
	return 0;
}

/** 
  * @name   Get_Version_Mess
  *
  * @brief  打印Key_Detect组件版本信息
  *
  * @param  无
  *
  * @retval 返Key_Detect组件版本信息
  */
char *Get_Version_Mess(void)
{
	return Version;
}


/******************** (C)COPYRIGHT(2018) YaoCheng END OF FILE **********************/

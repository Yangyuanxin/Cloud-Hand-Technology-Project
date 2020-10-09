#ifndef __KEY_DETECT_H
#define __KEY_DETECT_H


/** 
  * @brief 注册按键最大个数
  */
#define Key_Num_Max		10


/** 
  * @brief 按键触发模式状态枚举
  */
typedef enum
{
	N_Trig = 0, 																				/*!< 0 空 */		
	L_Trig ,																						/*!< 1 低电平触发 */			
	H_Trig,																							/*!< 2 高电平触发 */
}Trig_Mode_TypeDef;

/** 
  * @brief 按键模式状态枚举
  */
typedef enum
{
	N_Click = 0,																				/*!< 0 空 */	
	S_Click ,																						/*!< 1 单击 */			
	D_Click,																						/*!< 2 双击 */
	L_Press,																						/*!< 3 长按 */
}Key_Mode_TypeDef;


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
char Init_Key_Struct(void (*Update_Key_CallBack)(void),void (*Debug_CallBack)(unsigned char *debug_mess));

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
	Trig_Mode_TypeDef Trig_Mode_E, Key_Mode_TypeDef  Key_Mode_E, void (*Key_Click_CallBack)(void));

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
char Key_Detect(void);

/** 
  * @name   Get_Version_Mess
  *
  * @brief  打印Key_Detect组件版本信息
  *
  * @param  无
  *
  * @retval 返Key_Detect组件版本信息
  */
char *Get_Version_Mess(void);


#endif


/******************** (C)COPYRIGHT(2018) YaoCheng END OF FILE **********************/


#ifndef __ESP32_Config_H
#define __ESP32_Config_H

#include "main.h"
#include "DTU_SYS_Dis.h"
#include "Upper_Config.h"

 struct	ESP32_STATE_Dep		//共计15个标志位
{
/**************************************
ESP32的的接收标志位
**************************************/
	uint8_t	ESP32_State;
	uint8_t	ESP32_PassthroughState;
	uint8_t	ESP32_Passthrough_ErrFlag;
	uint8_t	ESP32_SetModeState;
/*************************************/

};

/***********************************************************
DTU状态的ESP32列表
************************************************************/
enum   DTU_State_ESP32List
{
	DTU_ESP32FreeMode = 0,				//系统空闲模式
	DTU_ESP32ConfigUpperMode,			//系统上位机配置模式
	DTU_ESP32PassthroughMode,			//系统透传模式
	DTU_ESP32AnalyticalModel,			//系统解析模式
};

/***********************************************************
ESP32透传状态列表
************************************************************/
enum   ESP32_PassthroughModeList
{
	ESP32_PassthroughFreeMode = 0,				//系统空闲模式
	ESP32_PassthroughDetMode,							//系统存储检测模式
	ESP32_PassthroughSetMode,							//ESP32透传设置模式
	ESP32_PassthroughErrMode,							//ESP32错误模式模式
};

//void  ESP32_State_Dis(uint8_t ESP32_StateIng);
//void  ESP32_DataDis(void);
//void  ESP32_SetMode_Dis(struct Upper_WFVariable *ESP32_Val_Set);
#endif /* __ESP32_Config_H */

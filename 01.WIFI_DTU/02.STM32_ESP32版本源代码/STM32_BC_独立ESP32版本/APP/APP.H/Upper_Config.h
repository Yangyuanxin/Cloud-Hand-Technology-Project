#ifndef __Upper_Config_H
#define __Upper_Config_H

#include "main.h"
#include "DTU_SYS_Dis.h"


#define   SetDebug_Add												0x0801E000
#define   SetWFMode_Add												0x0801F000

#define   SetWFJAP_SSID_Add										0x0801F400
#define   SetWFJAP_PWD_Add										0x0801F500

#define   SetWFSAP_SSID_Add										0x0801F800
#define   SetWFSAP_PWD_Add										0x0801F900

#define   SetWFCIPSTART_Type_Add							0x0801C000
#define   SetWFCIPSTART_RemoteIP_Add					0x0801D000
#define   SetWFCIPSTART_RemotePort_Add				0x0801E000



typedef struct	Upper_STATE_FLAG		//共计15个标志位
{
	uint8_t	Upper_DTUMode_f:1;
	uint8_t	Upper_FindDTUMode_f:1;
	
/**************************************
上位机的的接收标志位
**************************************/
	uint8_t	Upper_DTUWFMode_f:1;
	uint8_t	Upper_FindDTUWFMode_f:1;
	uint8_t	Upper_DTUDebug_f:1;
	uint8_t	Upper_FindDTUDebug_f:1;
	uint8_t	Upper_DTUWFJAP_f:1;
	uint8_t	Upper_FindDTUWFJAP_f:1;
	uint8_t	Upper_DTUWFSAP_f:1;
	uint8_t	Upper_FindDTUWFSAP_f:1;
	uint8_t	Upper_DTUWFSAPSet_f:1;
	uint8_t	Upper_FindDTUWFSAPSet_f:1;
/*************************************/

}Upper_STATE_flag;

typedef union   Upper_STATE_BIT					//联合体共用内存
{
    uint32_t Upper_Byte;
    Upper_STATE_flag  Upper_f;

}Upper_STATE_bit;


#define  Mode_L					2
#define  SSID_L					20
#define  PWD_L					20
#define  Type_L					5
#define  RemoteIP_L			20
#define  RemotePort_L		5

struct Upper_Variable
{
	uint8_t Upper_State;
	char  Upper_SetMode[Mode_L]; 		 			//模组设置模式
	char  Upper_ModuleBrand[Mode_L];     	//模组品牌
	char  Upper_Debug[Mode_L];     				//Debug信息开关
};

 struct Upper_WFVariable
{
	char Upper_SetWFMode[Mode_L]; 											//WIFI的模式设置
	
	char Upper_SetWFJAP_SSID[SSID_L]; 									//WIFI的AP模式的SSID
	char Upper_SetWFJAP_PWD[PWD_L]; 										//WIFI的AP模式的PWD
	
	char Upper_SetWFSAP_SSID[SSID_L]; 									//WIFI的SoftAP的SSID
	char Upper_SetWFSAP_PWD[PWD_L]; 										//WIFI的SoftAP的PWD
	
	char Upper_SetWFCIPSTART_Type[Type_L];  						//字符串串参数，连接类型，"TCP"，"UDP" 或 "SSL"
	char Upper_SetWFCIPSTART_RemoteIP[RemoteIP_L]; 			//字符串串参数，远端 IP 地址
	char Upper_SetWFCIPSTART_RemotePort[RemotePort_L]; 	//IP地址的端口号
};

extern  struct  Upper_WFVariable 	 Upper_ESP32Val;		//建立ESP32结构体和ESP8266结构体;
/**********************
DTU状态列表
**********************/
enum   Upper_State_List
{
	Upper_FreeMode = 0,			//系统等待选择配置模式
	Upper_ESP32Mode,			//系统ESP32模式设置
};

void  Upper_RX_Dis(void);
void  Upper_State_Dis(void);
void  Upper_Find_Dis(struct Upper_Variable *Upper_Val);
void  Upper_ESP32_ValDis(struct Upper_WFVariable *Upper_WFVal);
void  Upper_WFFind_ValDis(struct Upper_WFVariable *Upper_WFVal);
void  ESP32PassthroughMode_Dis(struct Upper_WFVariable *ESP32_Val);
void  DTU_Flash_GetData(uint32_t Writeaddr,char *data,char *Prefix);
void  DTU_ReadFlash_SetData(void);
void  Upper_Set_Dis(struct Upper_Variable *Upper_Val);
void  Upper_Set_Dis(struct Upper_Variable *Upper_Val);
#endif /* __Upper_Config_H */

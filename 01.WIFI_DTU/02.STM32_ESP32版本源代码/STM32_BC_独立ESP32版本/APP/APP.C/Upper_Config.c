#include "Upper_Config.h"
#include "Flash_Dis.h"			//Flash文件

uint8_t	String_DTUMode[]					= "AT+DTUMode=";
uint8_t	String_FindDTUMode[]			= "AT+DTUMode?";

uint8_t	Reply_DTUMode[]						= "+DTUMode:";
uint8_t	Reply_FindDTUMode[]				= "+DTUModeState:";

uint8_t	String_DTUDebug[]					= "AT+DTUDebug=";
uint8_t	String_FindDTUDebug[]			= "AT+DTUDebug?";

uint8_t	Reply_DTUDebug[]					= "+DTUDebug:";
uint8_t	Reply_FindDTUDebug[]			= "+DTUDebug?:";

/************************************************************************************
ESP32和ESP8266指令是共用的
************************************************************************************/
/*************************上位机->单片机（前缀）*************************************/
uint8_t	String_DTUWFMode[] 				= "AT+DTUWFMode=";
uint8_t	String_FindDTUWFMode[] 		= "AT+DTUWFMode?";
uint8_t	String_DTUWFJAP[] 				= "AT+DTUWFJAP=";
uint8_t	String_FindDTUWFJAP[] 		= "AT+DTUWFJAP?";
uint8_t	String_DTUWFSAP[] 				= "AT+DTUWFSAP=";
uint8_t	String_FindDTUWFSAP[] 		= "AT+DTUWFSAP?"; 
uint8_t	String_DTUWFSAPSet[] 			= "AT+DTUCIPSTART=";
uint8_t	String_FindDTUWFSAPSet[] 	= "AT+DTUCIPSTART?";
/*************************单片机->上位机（前缀）*************************************/
uint8_t	Reply_DTUWFMode[] 				= "+DTUWFMode:";
uint8_t	Reply_FindDTUWFMode[] 		= "+DTUWFMode?:";
uint8_t	Reply_DTUWFJAP[] 					= "+DTUWFJAP:";
uint8_t	Reply_FindDTUWFJAP[] 			= "+DTUWFJAP?:";
uint8_t	Reply_DTUWFSAP[] 					= "+DTUWFSAP:";
uint8_t	Reply_FindDTUWFSAP[] 			= "+DTUWFSAP?:";
uint8_t	Reply_DTUWFSAPSet[] 			= "+DTUWFSAPSet:";
uint8_t	Reply_FindDTUWFSAPSet[] 	= "+DTUWFSAPSet?:";
/***********************************************************************************/

uint8_t  Upper_SetOK[] ="1";
uint8_t  Upper_SetErr[] ="0";



char ESP32_FlashReadTest[50] = "";

uint8_t  Upper_i;
uint8_t  Upper_State;
char  Upper_Rx_Buffer[Uart1_BUFFER_SIZE];

   

extern	uint8_t Uart1_Rx_Len;
extern  uint8_t Uart1_Recv_End_Flag;
extern  uint8_t Uart1_Rx_Buffer[Uart1_BUFFER_SIZE];
extern  uint8_t Debug_f;

Upper_STATE_bit Upper_S_bit;

struct 	Upper_Variable 		 Upper_Val;

struct  Upper_WFVariable 	 Upper_ESP32Val;	

/******************************************************************************************************************************************************************************
** 函数名:void  Upper_RX_Dis(void)
** 功能描述:将STM32的串口2接收到的数据进行处理
** 输入参数:无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void  Upper_RX_Dis(void)
{
		if(Uart1_Recv_End_Flag == 1)
    {
				DEBUG("rx_len = %d\r\n",Uart1_Rx_Len);				//打印接收长度
				if((Uart1_Rx_Buffer[Uart1_Rx_Len-2] == '\r')&&(Uart1_Rx_Buffer[Uart1_Rx_Len-1] == '\n'))   	//判断是换行回车结尾
				{
					for(Upper_i=0;Upper_i<(Uart1_Rx_Len-2);Upper_i++)
					{
					Upper_Rx_Buffer[Upper_i] = Uart1_Rx_Buffer[Upper_i];
					}
						DEBUG("接收的数据：%s\r\n",Upper_Rx_Buffer);	//打印接收数据
					if(memcmp(String_DTUMode, Upper_Rx_Buffer, sizeof(String_DTUMode)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_DTUMode_f = 1;
							strncpy(Upper_Val.Upper_SetMode,Upper_Rx_Buffer+sizeof(String_DTUMode)-1,sizeof(Upper_Val.Upper_SetMode));	
							strcpy(Upper_Val.Upper_SetMode, strtok(Upper_Val.Upper_SetMode,","));
							DEBUG("设置DTU模式:%s\r\n",Upper_Val.Upper_SetMode);
					}
					else if(memcmp(String_FindDTUMode, Upper_Rx_Buffer, sizeof(String_FindDTUMode)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_FindDTUMode_f = 1;
							DEBUG("查询DTU当前模式\r\n");
					}
					else if(memcmp(String_DTUDebug, Upper_Rx_Buffer, sizeof(String_DTUDebug)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_DTUDebug_f = 1;
							DEBUG("设置Debug状态\r\n");
					}
					else if(memcmp(String_FindDTUDebug, Upper_Rx_Buffer, sizeof(String_FindDTUDebug)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_FindDTUDebug_f = 1;
							DEBUG("查询Debug当前状态\r\n");
					}
					else if(memcmp(String_DTUWFMode, Upper_Rx_Buffer, sizeof(String_DTUWFMode)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_DTUWFMode_f = 1;
							DEBUG("设置WIFI当前模式\r\n");		
					}
					else if(memcmp(String_FindDTUWFMode, Upper_Rx_Buffer, sizeof(String_FindDTUWFMode)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_FindDTUWFMode_f = 1;
							DEBUG("查询WIFI当前模式\r\n");
					}
					else if(memcmp(String_DTUWFJAP, Upper_Rx_Buffer, sizeof(String_DTUWFJAP)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_DTUWFJAP_f = 1;
							DEBUG("设置连接AP指令\r\n");
					}
					else if(memcmp(String_FindDTUWFJAP, Upper_Rx_Buffer, sizeof(String_FindDTUWFJAP)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_FindDTUWFJAP_f = 1;
							DEBUG("查询AP状态指令\r\n");
					}
					else if(memcmp(String_DTUWFSAP, Upper_Rx_Buffer, sizeof(String_DTUWFSAP)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_DTUWFSAP_f = 1;
							DEBUG("一键配置SoftAP参数\r\n");
					}
					else if(memcmp(String_FindDTUWFSAP, Upper_Rx_Buffer, sizeof(String_FindDTUWFSAP)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_FindDTUWFSAP_f = 1;
							DEBUG("查询SoftAP参数\r\n");
					}
					else if(memcmp(String_DTUWFSAPSet, Upper_Rx_Buffer, sizeof(String_DTUWFSAPSet)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_DTUWFSAPSet_f = 1;
							DEBUG("设置连接方式、IP和端口号\r\n");
					}
					else if(memcmp(String_FindDTUWFSAPSet, Upper_Rx_Buffer, sizeof(String_FindDTUWFSAPSet)-1) == 0)
					{
							Upper_S_bit.Upper_f.Upper_FindDTUWFSAPSet_f = 1;
							DEBUG("查询连接方式、IP和端口号\r\n");
					}
				}
				Uart1_Rx_Len=0;					//清除计数
        Uart1_Recv_End_Flag=0;	//清除接收结束标志位
			
			  memset(Uart1_Rx_Buffer,0,sizeof(Uart1_Rx_Buffer));	
		 }
}


/******************************************************************************************************************************************************************************
** 函数名:void   Upper_State_Dis(void)
** 功能描述:上位机配置状态机
** 输入参数:无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void   Upper_State_Dis(void)
{
	Upper_RX_Dis();
	Upper_Find_Dis(&Upper_Val);
	Upper_Set_Dis(&Upper_Val);
	
	switch(Upper_Val.Upper_State)
	{
		case	Upper_FreeMode:
						if(strcmp(Upper_Val.Upper_SetMode,"2") == 0)
						{
							Upper_Val.Upper_State = Upper_ESP32Mode;
							DEBUG("进入ESP32模式\r\n");
						}
		break;
		case		Upper_ESP32Mode:
						Upper_ESP32_ValDis(&Upper_ESP32Val);
						Upper_WFFind_ValDis(&Upper_ESP32Val);
		break;
	}
		Upper_S_bit.Upper_Byte = 0;
		memset(Upper_Rx_Buffer,0,sizeof(Upper_Rx_Buffer));
}

/******************************************************************************************************************************************************************************
** 函数名:void   Upper_Find_Dis(void)
** 功能描述:上位机配置还没进入模式之前的查询函数处理
** 输入参数:无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void   Upper_Find_Dis(struct Upper_Variable *Upper_Val)
{
	if(Upper_S_bit.Upper_f.Upper_FindDTUMode_f)
	{
		if(strlen(Upper_Val->Upper_SetMode) == NULL)				//判断字符串Upper_SetWFJAP_SSID是不是空的
		{
		strcpy(Upper_Val->Upper_SetMode, "0"); 
		}
		if(strlen(Upper_Val->Upper_ModuleBrand) == NULL)				//判断字符串Upper_SetWFJAP_SSID是不是空的
		{
		strcpy(Upper_Val->Upper_ModuleBrand, "0"); 
		}
		Print(&huart1,"%s%s,%s\r\n",Reply_FindDTUMode,Upper_Val->Upper_SetMode,Upper_Val->Upper_ModuleBrand);	
	}
	else if(Upper_S_bit.Upper_f.Upper_FindDTUDebug_f)
	{
		if(strlen(Upper_Val->Upper_Debug) == NULL)				//判断字符串Upper_SetWFJAP_SSID是不是空的
		{
		strcpy(Upper_Val->Upper_Debug, "0"); 
		}
		Print(&huart1,"%s%s\r\n",Reply_FindDTUDebug,Upper_Val->Upper_Debug);	
	}
}
/******************************************************************************************************************************************************************************
** 函数名:void   Upper_Find_Dis(void)
** 功能描述:上位机配置还没进入模式之前的查询函数处理
** 输入参数:无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void   Upper_Set_Dis(struct Upper_Variable *Upper_Val)
{
		if(Upper_S_bit.Upper_f.Upper_DTUMode_f)
		{
				if(strcmp(Upper_Val->Upper_SetMode,"2") == 0)
				{
					Print(&huart1,"+DTUMode:%s\r\n",Upper_SetOK);
//					DEBUG("进入ESP32模式\r\n");
				}
				else
				{
					Print(&huart1,"+DTUMode:%s\r\n",Upper_SetErr);
					DEBUG("此模块只支持进入ESP32模式\r\n");
				}
		}
		else if(Upper_S_bit.Upper_f.Upper_DTUDebug_f)
		{
		strncpy(Upper_Val->Upper_Debug,Upper_Rx_Buffer+sizeof(String_DTUDebug)-1,sizeof(Upper_Val->Upper_Debug));
//		DEBUG("设置Debug状态:%s\r\n",Upper_Val->Upper_Debug);

/*********************************************************************************************************
防止写入内容与存储内容一致导致反复擦除，写入数据之前先对数据进行判断
*********************************************************************************************************/		
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetDebug_Add,ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_Val->Upper_Debug) != 0)
		{
				//写入对应的地址和数据
				Flash_WriteData(SetDebug_Add,Upper_Val->Upper_Debug,sizeof(Upper_Val->Upper_Debug));
		}
		else
		{
				DEBUG("写入数据相同，无需擦除Flash\r\n");
		}
/********************************************************************************************************/

/*********************************************************************************************************
判断是否写入成功如果写入成功则返回相应的信息
*********************************************************************************************************/			
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetDebug_Add,ESP32_FlashReadTest);
		
	Print(&huart1,"打印的数据：%s\r\n",ESP32_FlashReadTest);
		if(strcmp(ESP32_FlashReadTest,Upper_Val->Upper_Debug) == 0)
		{
			Print(&huart1,"%s%s\r\n",Reply_DTUDebug,Upper_SetOK);
			if(strcmp("0",Upper_Val->Upper_Debug) == 0)
			{
					Debug_f = 0;
			}
			else if(strcmp("1",Upper_Val->Upper_Debug) == 0)
			{
					Debug_f = 1;
			}
		}
		else
		{
			Print(&huart1,"%s%s\r\n",Reply_DTUDebug,Upper_SetErr);
		}
/********************************************************************************************************/
		
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
	}
	
}
/******************************************************************************************************************************************************************************
** 函数名:void   Upper_WFFind_ValDis(struct Upper_WFVariable *Upper_WFVal)
** 功能描述:上位机配置进入ESP32模式后,对需要查找对应的AT指令进行处理
** 输入参数:Upper_WFVariable
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void   Upper_WFFind_ValDis(struct Upper_WFVariable *Upper_WFVal)
{
	if(Upper_S_bit.Upper_f.Upper_FindDTUWFMode_f)
	{
		if(strlen(Upper_WFVal->Upper_SetWFMode) == NULL)				//判断字符串Upper_SetWFJAP_SSID是不是空的
		{
		strcpy(Upper_WFVal->Upper_SetWFMode, "0"); 
		}

			Print(&huart1,"%s%s\r\n",Reply_FindDTUWFMode,Upper_ESP32Val.Upper_SetWFMode);
	}
	else if(Upper_S_bit.Upper_f.Upper_FindDTUWFJAP_f)
	{
			if(strlen(Upper_WFVal->Upper_SetWFJAP_SSID) == NULL)				//判断字符串Upper_SetWFJAP_SSID是不是空的
			{
			strcpy(Upper_WFVal->Upper_SetWFJAP_SSID, "0"); 
			}
			
			if(strlen(Upper_WFVal->Upper_SetWFJAP_PWD) == NULL)				//判断字符串Upper_SetWFJAP_PWD是不是空的
			{
			strcpy(Upper_WFVal->Upper_SetWFJAP_PWD, "0"); 
			}
			Print(&huart1,"%s%s,%s\r\n",Reply_FindDTUWFJAP,Upper_WFVal->Upper_SetWFJAP_SSID,Upper_WFVal->Upper_SetWFJAP_PWD);
	}
	else if(Upper_S_bit.Upper_f.Upper_FindDTUWFSAP_f)
	{
			if(strlen(Upper_WFVal->Upper_SetWFSAP_SSID) == NULL)				//判断字符串Upper_SetWFJAP_SSID是不是空的
			{
			strcpy(Upper_WFVal->Upper_SetWFSAP_SSID, "0"); 
			}
			
			if(strlen(Upper_WFVal->Upper_SetWFSAP_PWD) == NULL)				//判断字符串Upper_SetWFJAP_PWD是不是空的
			{
			strcpy(Upper_WFVal->Upper_SetWFSAP_PWD, "0"); 
			}
		Print(&huart1,"%s%s,%s\r\n",Reply_FindDTUWFSAP,Upper_WFVal->Upper_SetWFSAP_SSID,Upper_WFVal->Upper_SetWFSAP_PWD);
	}
	else if(Upper_S_bit.Upper_f.Upper_FindDTUWFSAPSet_f)
	{
			if(strlen(Upper_WFVal->Upper_SetWFCIPSTART_Type) == NULL)				//判断字符串Upper_SetWFJAP_SSID是不是空的
			{
			strcpy(Upper_WFVal->Upper_SetWFSAP_SSID, "0"); 
			}
			
			if(strlen(Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP) == NULL)				//判断字符串Upper_SetWFJAP_PWD是不是空的
			{
			strcpy(Upper_WFVal->Upper_SetWFSAP_PWD, "0"); 
			}
			
			if(strlen(Upper_WFVal->Upper_SetWFCIPSTART_RemotePort) == NULL)				//判断字符串Upper_SetWFJAP_PWD是不是空的
			{
			strcpy(Upper_WFVal->Upper_SetWFSAP_PWD, "0"); 
			}
		Print(&huart1,"%s%s,%s,%s\r\n",Reply_FindDTUWFSAPSet,Upper_WFVal->Upper_SetWFCIPSTART_Type,Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP,Upper_WFVal->Upper_SetWFCIPSTART_RemotePort);
	}
}

/******************************************************************************************************************************************************************************
** 函数名:void   Upper_WFFind_ValDis(struct Upper_WFVariable *Upper_WFVal)
** 功能描述:上位机配置进入ESP32模式后，对需要存储的数据进行存储和处理
** 输入参数:Upper_WFVariable
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void   Upper_ESP32_ValDis(struct Upper_WFVariable *Upper_WFVal)
{
	if(Upper_S_bit.Upper_f.Upper_DTUWFMode_f)
	{
		strncpy(Upper_WFVal->Upper_SetWFMode,Upper_Rx_Buffer+sizeof(String_DTUWFMode)-1,sizeof(Upper_WFVal->Upper_SetWFMode));
		DEBUG("设置WIFI模式:%s\r\n",Upper_WFVal->Upper_SetWFMode);

/*********************************************************************************************************
防止写入内容与存储内容一致导致反复擦除，写入数据之前先对数据进行判断
*********************************************************************************************************/		
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFMode_Add,ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFMode) != 0)
		{
				//写入对应的地址和数据
				Flash_WriteData(SetWFMode_Add,Upper_WFVal->Upper_SetWFMode,sizeof(Upper_WFVal->Upper_SetWFMode));
		}
		else
		{
				DEBUG("写入数据相同，无需擦除Flash\r\n");
		}
/********************************************************************************************************/

/*********************************************************************************************************
判断是否写入成功如果写入成功则返回相应的信息
*********************************************************************************************************/			
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFMode_Add,ESP32_FlashReadTest);
		
		DEBUG("打印的数据：%s\r\n",ESP32_FlashReadTest);
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFMode) == 0)
		{
			Print(&huart1,"%s%s\r\n",Reply_DTUWFMode,Upper_SetOK);
		}
		else
		{
			Print(&huart1,"%s%s\r\n",Reply_DTUWFMode,Upper_SetErr);
		}
/********************************************************************************************************/
		
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
	}
	else if(Upper_S_bit.Upper_f.Upper_DTUWFJAP_f)
	{
		strncpy(Upper_Rx_Buffer,Upper_Rx_Buffer+sizeof(String_DTUWFJAP)-1,sizeof(Upper_Rx_Buffer));
	
		strcpy(Upper_WFVal->Upper_SetWFJAP_SSID, strtok(Upper_Rx_Buffer,","));	//获取SSID
		strcpy(Upper_WFVal->Upper_SetWFJAP_PWD, strtok(NULL,"\r\n"));						//获取PWD
	
		DEBUG("AP_SSID:%s\r\n",Upper_WFVal->Upper_SetWFJAP_SSID);
		DEBUG("AP_PWD:%s\r\n",Upper_WFVal->Upper_SetWFJAP_PWD);
/*********************************************************************************************************
防止写入内容与存储内容一致导致反复擦除，写入数据之前先对数据进行判断
*********************************************************************************************************/		
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFJAP_SSID_Add,ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFJAP_SSID) == 0)						//判断输入的AP模式的SSID与存储的相同
		{
				memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
				Flash_ReadData(SetWFJAP_PWD_Add,ESP32_FlashReadTest);
			
				if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFJAP_PWD) != 0)				//判断输入的AP模式的PWD与存储的不相同
				{
					//写入对应的地址和数据
					Flash_WriteData_2(SetWFJAP_SSID_Add,SetWFJAP_PWD_Add,
														Upper_WFVal->Upper_SetWFJAP_SSID,sizeof(Upper_WFVal->Upper_SetWFJAP_SSID),
														Upper_WFVal->Upper_SetWFJAP_PWD,sizeof(Upper_WFVal->Upper_SetWFJAP_PWD));
				}
				else
				{
					DEBUG("写入数据相同，无需擦除Flash\r\n");
				}
		}
		else
		{
					//写入对应的地址和数据
					Flash_WriteData_2(SetWFJAP_SSID_Add,SetWFJAP_PWD_Add,
														Upper_WFVal->Upper_SetWFJAP_SSID,sizeof(Upper_WFVal->Upper_SetWFJAP_SSID),
														Upper_WFVal->Upper_SetWFJAP_PWD,sizeof(Upper_WFVal->Upper_SetWFJAP_PWD));
		}
/********************************************************************************************************/
/*********************************************************************************************************
判断是否写入成功如果写入成功则返回相应的信息
*********************************************************************************************************/			
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFJAP_SSID_Add,ESP32_FlashReadTest);
		
		DEBUG("打印的数据：%s\r\n",ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFJAP_SSID) == 0)
		{
			memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
			
			Flash_ReadData(SetWFJAP_PWD_Add,ESP32_FlashReadTest);
			
			DEBUG("打印的数据：%s\r\n",ESP32_FlashReadTest);
			
			if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFJAP_PWD) == 0)
			{
					Print(&huart1,"%s%s\r\n",Reply_DTUWFJAP,Upper_SetOK);
			}
			else
					Print(&huart1,"%s%s\r\n",Reply_DTUWFJAP,Upper_SetErr);
		}
		else
		{
			Print(&huart1,"%s%s\r\n",Reply_DTUWFJAP,Upper_SetErr);
		}
/********************************************************************************************************/
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
	}
	else if(Upper_S_bit.Upper_f.Upper_DTUWFSAP_f)
	{
			strncpy(Upper_Rx_Buffer,Upper_Rx_Buffer+sizeof(String_DTUWFSAP)-1,sizeof(Upper_Rx_Buffer));
		
			strcpy(Upper_WFVal->Upper_SetWFSAP_SSID, strtok(Upper_Rx_Buffer,","));			//获取SSID
			strcpy(Upper_WFVal->Upper_SetWFSAP_PWD, strtok(NULL,"\r\n"));								//获取PWD
		
			DEBUG("SoftAP_SSID:%s\r\n",Upper_WFVal->Upper_SetWFSAP_SSID);
			DEBUG("SoftAP_PWD:%s\r\n",Upper_WFVal->Upper_SetWFSAP_PWD);
/*********************************************************************************************************
防止写入内容与存储内容一致导致反复擦除，写入数据之前先对数据进行判断
*********************************************************************************************************/		
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFSAP_SSID_Add,ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFSAP_SSID) == 0)						//判断输入的AP模式的SSID与存储的相同
		{
				memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
				Flash_ReadData(SetWFSAP_PWD_Add,ESP32_FlashReadTest);
				if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFSAP_PWD) != 0)				//判断输入的AP模式的PWD与存储的不相同
				{
					//写入对应的地址和数据
						Flash_WriteData_2(SetWFSAP_SSID_Add,SetWFSAP_PWD_Add,
															Upper_WFVal->Upper_SetWFSAP_SSID,sizeof(Upper_WFVal->Upper_SetWFSAP_SSID),
															Upper_WFVal->Upper_SetWFSAP_PWD,sizeof(Upper_WFVal->Upper_SetWFSAP_PWD));
				}
				else
				{
					DEBUG("写入数据相同，无需擦除Flash\r\n");
				}
		}
		else
		{
					//写入对应的地址和数据
						Flash_WriteData_2(SetWFSAP_SSID_Add,SetWFSAP_PWD_Add,
															Upper_WFVal->Upper_SetWFSAP_SSID,sizeof(Upper_WFVal->Upper_SetWFSAP_SSID),
															Upper_WFVal->Upper_SetWFSAP_PWD,sizeof(Upper_WFVal->Upper_SetWFSAP_PWD));
		}
/********************************************************************************************************/
/*********************************************************************************************************
判断是否写入成功如果写入成功则返回相应的信息
*********************************************************************************************************/	
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFSAP_SSID_Add,ESP32_FlashReadTest);
		
		DEBUG("打印的数据：%s\r\n",ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFSAP_SSID) == 0)
		{
			memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
			
			Flash_ReadData(SetWFSAP_PWD_Add,ESP32_FlashReadTest);
			
			DEBUG("打印的数据：%s\r\n",ESP32_FlashReadTest);
			
			if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFSAP_PWD) == 0)
			{
					Print(&huart1,"%s%s\r\n",Reply_DTUWFSAP,Upper_SetOK);
			}
			else
					Print(&huart1,"%s%s\r\n",Reply_DTUWFSAP,Upper_SetErr);
		}
		else
		{
			Print(&huart1,"%s%s\r\n",Reply_DTUWFSAP,Upper_SetErr);
		}
/********************************************************************************************************/
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		
	}
	else if(Upper_S_bit.Upper_f.Upper_DTUWFSAPSet_f)
	{
			strncpy(Upper_Rx_Buffer,Upper_Rx_Buffer+sizeof(String_DTUWFSAPSet)-1,sizeof(Upper_Rx_Buffer));
		
			strcpy(Upper_WFVal->Upper_SetWFCIPSTART_Type, 			strtok(Upper_Rx_Buffer,","));			//获取连接方式
			strcpy(Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP, 	strtok(NULL,","));								//获取IP
			strcpy(Upper_WFVal->Upper_SetWFCIPSTART_RemotePort, strtok(NULL,"\r\n"));						//获取端口
		
			DEBUG("连接方式:%s\r\n",Upper_WFVal->Upper_SetWFCIPSTART_Type);
			DEBUG("IP:%s\r\n",Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP);
			DEBUG("端口:%s\r\n",Upper_WFVal->Upper_SetWFCIPSTART_RemotePort);

/*********************************************************************************************************
防止写入内容与存储内容一致导致反复擦除，写入数据之前先对数据进行判断
*********************************************************************************************************/
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFCIPSTART_Type_Add,ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFCIPSTART_Type) == 0)								//判断输入的连接模式与存储的相同
		{
				memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
				Flash_ReadData(SetWFCIPSTART_RemoteIP_Add,ESP32_FlashReadTest);
				if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP) == 0)				//判断输入IP与存储的相同
				{
					memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
					Flash_ReadData(SetWFCIPSTART_RemotePort_Add,ESP32_FlashReadTest);
					if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFCIPSTART_RemotePort) != 0)		//判断输入端口与存储的不相同
					{
						Flash_WriteData_3(SetWFCIPSTART_Type_Add,SetWFCIPSTART_RemoteIP_Add,SetWFCIPSTART_RemotePort_Add,
															Upper_WFVal->Upper_SetWFCIPSTART_Type,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_Type),
															Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP),
															Upper_WFVal->Upper_SetWFCIPSTART_RemotePort,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_RemotePort));
					}
					else
					{
						DEBUG("写入数据相同，无需擦除Flash\r\n");
					}
				}
				else
				{
					Flash_WriteData_3(SetWFCIPSTART_Type_Add,SetWFCIPSTART_RemoteIP_Add,SetWFCIPSTART_RemotePort_Add,
														Upper_WFVal->Upper_SetWFCIPSTART_Type,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_Type),
														Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP),
														Upper_WFVal->Upper_SetWFCIPSTART_RemotePort,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_RemotePort));
				}
		}
		else
		{
					//写入对应的地址和数据
						Flash_WriteData_3(SetWFCIPSTART_Type_Add,SetWFCIPSTART_RemoteIP_Add,SetWFCIPSTART_RemotePort_Add,
														Upper_WFVal->Upper_SetWFCIPSTART_Type,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_Type),
														Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP),
														Upper_WFVal->Upper_SetWFCIPSTART_RemotePort,sizeof(Upper_WFVal->Upper_SetWFCIPSTART_RemotePort));
		}
/********************************************************************************************************/
/*********************************************************************************************************
判断是否写入成功如果写入成功则返回相应的信息
*********************************************************************************************************/		
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(SetWFCIPSTART_Type_Add,ESP32_FlashReadTest);
		
		DEBUG("打印的数据：%s\r\n",ESP32_FlashReadTest);
		
		if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFCIPSTART_Type) == 0)
		{
			memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
			
			Flash_ReadData(SetWFCIPSTART_RemoteIP_Add,ESP32_FlashReadTest);
			
			DEBUG("打印的数据：%s\r\n",ESP32_FlashReadTest);
			
			if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFCIPSTART_RemoteIP) == 0)
			{
					memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
			
					Flash_ReadData(SetWFCIPSTART_RemotePort_Add,ESP32_FlashReadTest);
					
					DEBUG("打印的数据：%s\r\n",ESP32_FlashReadTest);
					if(strcmp(ESP32_FlashReadTest,Upper_WFVal->Upper_SetWFCIPSTART_RemotePort) == 0)
									Print(&huart1,"%s%s\r\n",Reply_DTUWFSAPSet,Upper_SetOK);
					else
									Print(&huart1,"%s%s\r\n",Reply_DTUWFSAPSet,Upper_SetErr);
			}
			else
					Print(&huart1,"%s%s\r\n",Reply_DTUWFSAPSet,Upper_SetErr);
		}
		else
		{
			Print(&huart1,"%s%s\r\n",Reply_DTUWFSAPSet,Upper_SetErr);
		}
/********************************************************************************************************/
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		
	}
}
/******************************************************************************************************************************************************************************
** 函数名:void   DTU_Flash_GetData(uint32_t Writeaddr,char *data,char *Prefix)
** 功能描述:获取Flash中的指定地址的数据
** 输入参数:	Writeaddr   起始地址
						data				数据指针
						Prefix			打印的前缀
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void   DTU_Flash_GetData(uint32_t Writeaddr,char *data,char *Prefix)
{
		memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
		Flash_ReadData(Writeaddr,ESP32_FlashReadTest);
		strcpy(data,ESP32_FlashReadTest);
		DEBUG("地址：0x%x,%s：%s\r\n",Writeaddr,Prefix,data);
}

/******************************************************************************************************************************************************************************
** 函数名:void   DTU_ReadFlash_SetData(struct Upper_WFVariable *Upper_WFVal)
** 功能描述:获取配置前透传的设置并打印
** 输入参数:	Upper_WFVal   存储结构体
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void   DTU_ReadFlash_SetData(void)
{
	memset(ESP32_FlashReadTest,0,sizeof(ESP32_FlashReadTest));
	Flash_ReadData(SetDebug_Add,ESP32_FlashReadTest);
	strcpy(Upper_Val.Upper_Debug,ESP32_FlashReadTest);
	if(strcmp("0",Upper_Val.Upper_Debug) == 0)
	{
			Debug_f = 0;
	}
	else if(strcmp("1",Upper_Val.Upper_Debug) == 0)
	{
			Debug_f = 1;
	}
	
	DTU_Flash_GetData(SetDebug_Add,Upper_Val.Upper_Debug,"Debug状态:");
	DTU_Flash_GetData(SetWFMode_Add,Upper_ESP32Val.Upper_SetWFMode,"WIFI模式:");
	
	DTU_Flash_GetData(SetWFJAP_SSID_Add,Upper_ESP32Val.Upper_SetWFJAP_SSID,"AP模式SSID:");
	DTU_Flash_GetData(SetWFJAP_PWD_Add,Upper_ESP32Val.Upper_SetWFJAP_PWD,"AP模式PWD:");
	
	DTU_Flash_GetData(SetWFSAP_SSID_Add,Upper_ESP32Val.Upper_SetWFSAP_SSID,"SoftAP模式SSID:");
	DTU_Flash_GetData(SetWFSAP_PWD_Add,Upper_ESP32Val.Upper_SetWFSAP_PWD,"SoftAPAP模式PWD:");
	
	DTU_Flash_GetData(SetWFCIPSTART_Type_Add,Upper_ESP32Val.Upper_SetWFCIPSTART_Type,"连接的模式:");
	DTU_Flash_GetData(SetWFCIPSTART_RemoteIP_Add,Upper_ESP32Val.Upper_SetWFCIPSTART_RemoteIP,"服务器端的IP:");
	DTU_Flash_GetData(SetWFCIPSTART_RemotePort_Add,Upper_ESP32Val.Upper_SetWFCIPSTART_RemotePort,"服务器端的端口号:");
	

}


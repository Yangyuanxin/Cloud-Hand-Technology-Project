#include "DTU_SYS_Dis.h"
#include "LED_Dis.h"
#include "Upper_Config.h"

uint8_t	ESP32_ATReady[]					= "\r\nready";
uint8_t	ESP32_ATOK[]						= "\r\nOK";
uint8_t	ESP32_ATERROR[]					= "\r\nERROR";





uint8_t	ESP32_ATRST[]					= "AT+RST";

uint8_t  ESP32_ATE_f;

uint8_t  ESP32_i;
uint8_t  ESP32_Data_Len;
uint8_t  ESP32_Rx_Buffer[Uart1_BUFFER_SIZE];

extern	uint8_t Uart2_Rx_Len;
extern  uint8_t Uart2_Recv_End_Flag;
extern  uint8_t Uart2_Rx_Buffer[Uart1_BUFFER_SIZE];

ESP32_STATE_bit ESP32_S_bit;

struct	ESP32_STATE_Dep    ESP32_STATE_D;

uint8_t KEY = 0;	//读取按键状态


/******************************************************************************************************************************************************************************
** 函数名:void Print_Debug_mess(unsigned char *debug_Mess)
** 功能描述:打印组件调试信息
** 输入参数:无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void Print_Debug_mess(unsigned char *debug_Mess)
{
    HAL_UART_Transmit(&huart1, debug_Mess, 1, 100);
}

/******************************************************************************************************************************************************************************
** 函数名:void key_S_CallBack(void)
** 功能描述:按键单击处理函数
** 输入参数:无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void key_S_CallBack(void)	//单击
{

    if(ESP32_STATE_D.ESP32_State != DTU_ConfigurationMode)
    {
        ESP32_STATE_D.ESP32_State++;

        if(ESP32_STATE_D.ESP32_State > DTU_DataAnalysisMode)
        {
            ESP32_STATE_D.ESP32_State = DTU_FreeMode;
        }
    }
    else if(ESP32_STATE_D.ESP32_State == DTU_ConfigurationMode)
    {
        ESP32_STATE_D.ESP32_State = DTU_FreeMode;
    }

    DEBUG("按键单击\r\n");
}
/******************************************************************************************************************************************************************************
** 函数名:void key_L_CallBack(void)
** 功能描述:按键长按处理
** 输入参数:无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void key_L_CallBack(void)	//长按
{
    if(ESP32_STATE_D.ESP32_State != DTU_ConfigurationMode)	//长按进入配置模式
    {
        ESP32_STATE_D.ESP32_State = DTU_ConfigurationMode;
    }

    DEBUG("按键长按\r\n");
}

/******************************************************************************************************************************************************************************
** 函数名:void Update_Key_CallBack(void)
** 功能描述:更新按键状态
** 输入参数:无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void Update_Key_CallBack(void)
{
    KEY = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);	//读取按键状态
}

/******************************************************************************************************************************************************************************
** 函数名:void  KEY_Ready(void)
** 功能描述:按键的配置函数
** 输入参数:无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void  KEY_Ready(void)
{
    //初始化按键结构体
    Init_Key_Struct(Update_Key_CallBack, Print_Debug_mess);

    Reg_Key(&KEY, 10,  L_Trig, S_Click, key_S_CallBack);//单击
    Reg_Key(&KEY, 100, L_Trig, L_Press, key_L_CallBack);//长按
}
/******************************************************************************************************************************************************************************
** 函数名:void  DTU_State_Dispose(void)
** 功能描述:DTU的状态处理函数
** 输入参数:无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void  DTU_State_Dispose(void)
{
    /*********************************************************************
    按键处理函数（去抖用的是定时器去抖）
    *********************************************************************/
    if(!Time_KEY_Cnt)
    {
        Time_KEY_Cnt = 5;
        Key_Detect();
    }

    /*******************************************************************/

    /*********************************************************************
    系统状态对应LED灯
    *********************************************************************/
    if(ESP32_STATE_D.ESP32_State == DTU_FreeMode)//系统空闲模式灯显示
    {
        LED_SetColor(0, 1, 0);
    }
    else if(ESP32_STATE_D.ESP32_State == DTU_PassthroughMode)
    {
        LED_SetColor(1, 0, 0);
    }
    else if(ESP32_STATE_D.ESP32_State == DTU_DataAnalysisMode)
    {
        LED_SetColor(0, 1, 1);
    }
    else if(ESP32_STATE_D.ESP32_State == DTU_ConfigurationMode)
    {
        LED_SetColor(0, 0, 1);
    }

    /*******************************************************************/

    if(!ESP32_ATE_f)
    {
        ESP32_ATE_f = 1;
        Print(&huart2, "ATE0\r\n");		//开机发送关闭回显
    }

    ESP32_STATE_D.ESP32_State = DTU_ConfigurationMode;


    ESP32_DataDis();									//ESP32数据口接收数据并处理

    switch(ESP32_STATE_D.ESP32_State)
    {
        case DTU_FreeMode:					//系统空闲模式
            if(ESP32_S_bit.ESP32_f.ESP32_OK_f)
            {
                ESP32_STATE_D.ESP32_State = DTU_PassthroughMode;
            }
            else
            {
                Print(&huart2, "AT\r\n");
                DEBUG("模组不在线\r\n");
            }

            break;

        case DTU_ConfigurationDetMode:		//系统存储配置检测模式

            if(ESP32_PassthroughDet(&Upper_ESP32Val) == ESP32_OK)
            {
                DEBUG("检测到存储的配置信息齐全\r\n");
                ESP32_STATE_D.ESP32_State = DTU_PassthroughMode;
            }
            else if(ESP32_PassthroughDet(&Upper_ESP32Val) == ESP32_ERR)
            {
                DEBUG("检测到存储的配置信息有空的\r\n");
                DEBUG("可以长按进入配置模式\r\n");
                DEBUG("配置成功后短按退出配置模式\r\n");
                DEBUG("退出配置模式后自动进入数据透传模式\r\n");
                ESP32_STATE_D.ESP32_State = DTU_ERRORMode;
            }

            break;

        case DTU_ConfigurationSetMode:		//系统配置设置模式

            if(ESP32_SetMode_Dis(&Upper_ESP32Val) == ESP32_OK)
            {
                DEBUG("设置完成进入透传模式\r\n");
                ESP32_STATE_D.ESP32_State = DTU_PassthroughMode;
            }
            else if(ESP32_SetMode_Dis(&Upper_ESP32Val) == ESP32_ERR)
            {
                DEBUG("设置过程有错误配置\r\n");
                DEBUG("可以长按进入配置模式\r\n");
                DEBUG("配置成功后短按退出配置模式\r\n");
                DEBUG("退出配置模式后自动进入数据透传模式\r\n");
                ESP32_STATE_D.ESP32_State = DTU_ERRORMode;
            }

            break;

        case DTU_PassthroughMode:					//系统透传模式

            break;

        case DTU_DataAnalysisMode:				//系统透传数据解析模式

            break;

        case DTU_ConfigurationMode: 			//系统配置模式
            Upper_State_Dis();

            break;

        case DTU_ERRORMode:								//系统错误模式

            break;
    }

    ESP32_S_bit.ESP32_Byte = 0;
    memset(ESP32_Rx_Buffer, 0, sizeof(ESP32_Rx_Buffer));

}

/******************************************************************************************************************************************************************************
** 函数名:void  ESP32_DataDis(void)
** 功能描述:ESP32的数据处理函数
** 输入参数:无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void  ESP32_DataDis(void)
{
    if(Uart2_Recv_End_Flag == 1)
    {
        if((Uart2_Rx_Buffer[Uart2_Rx_Len - 2] == '\r') && (Uart2_Rx_Buffer[Uart2_Rx_Len - 1] == '\n'))   	//判断是换行回车结尾
        {
            for(ESP32_i = 0; ESP32_i < (Uart2_Rx_Len - 2); ESP32_i++)
            {
                ESP32_Rx_Buffer[ESP32_i] = Uart2_Rx_Buffer[ESP32_i];
            }

            DEBUG("接收到的数据为：%s\r\n", ESP32_Rx_Buffer);

            if(memcmp(ESP32_ATRST, ESP32_Rx_Buffer, sizeof(ESP32_ATRST) - 1) == 0)
            {
                ESP32_S_bit.ESP32_f.ESP32_READY_f = 1;
            }
            else if(memcmp(ESP32_ATOK, ESP32_Rx_Buffer, sizeof(ESP32_ATOK) - 1) == 0)
            {
                ESP32_S_bit.ESP32_f.ESP32_OK_f = 1;
            }
            else if(memcmp(ESP32_ATERROR, ESP32_Rx_Buffer, sizeof(ESP32_ATERROR) - 1) == 0)
            {
                ESP32_S_bit.ESP32_f.ESP32_ERROR_f = 1;
            }
        }

        Uart2_Rx_Len = 0;					//清除计数
        Uart2_Recv_End_Flag = 0;	//清除接收结束标志位

        memset(Uart2_Rx_Buffer, 0, sizeof(Uart2_Rx_Buffer));
    }
}
/******************************************************************************************************************************************************************************
** 函数名:uint8_t  ESP32_PassthroughDet(struct Upper_WFVariable *ESP32_Val)
** 功能描述:ESP32的透传模式的数据检测
** 输入参数:无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
uint8_t  ESP32_PassthroughDet(struct Upper_WFVariable *ESP32_Val)
{
    if(strcmp(ESP32_Val->Upper_SetWFMode, "0") == 0)		//检测模式设置为不为空
    {
        ESP32_STATE_D.ESP32_Passthrough_ErrFlag = 1;
        DEBUG("无Wi-Fi模式,并且关闭Wi-Fi RF\r\n");
    }
    else
    {
        if(strcmp(ESP32_Val->Upper_SetWFMode, "1") == 0)
            DEBUG("当前模式设置：Station 模式\r\n");
        else if(strcmp(ESP32_Val->Upper_SetWFMode, "2") == 0)
            DEBUG("当前模式设置：SoftAP 模式\r\n");
        else if(strcmp(ESP32_Val->Upper_SetWFMode, "3") == 0)
            DEBUG("当前模式设置：SoftAP+Station 模式\r\n");
    }

    if(strlen(ESP32_Val->Upper_SetWFJAP_PWD) == NULL)				//判断字符串Upper_SetWFJAP_PWD是不是空的
    {
        ESP32_STATE_D.ESP32_Passthrough_ErrFlag = 1;
        DEBUG("当前AP模式的PWD为空\r\n");
    }
    else
        DEBUG("当前AP模式的PWD为：%s\r\n", ESP32_Val->Upper_SetWFJAP_PWD);

    if(strlen(ESP32_Val->Upper_SetWFCIPSTART_Type) == NULL)				//判断字符串Upper_SetWFJAP_PWD是不是空的
    {
        ESP32_STATE_D.ESP32_Passthrough_ErrFlag = 1;
        DEBUG("当前连接类型为空\r\n");
    }
    else
        DEBUG("当前连接类型为：%s\r\n", ESP32_Val->Upper_SetWFCIPSTART_Type);

    if(strlen(ESP32_Val->Upper_SetWFCIPSTART_RemoteIP) == NULL)				//判断字符串Upper_SetWFJAP_PWD是不是空的
    {
        ESP32_STATE_D.ESP32_Passthrough_ErrFlag = 1;
        DEBUG("当前AP模式的连接IP为空\r\n");
    }
    else
        DEBUG("当前AP模式的连接IP为：%s\r\n", ESP32_Val->Upper_SetWFCIPSTART_RemoteIP);

    if(strlen(ESP32_Val->Upper_SetWFCIPSTART_RemotePort) == NULL)				//判断字符串Upper_SetWFJAP_PWD是不是空的
    {
        ESP32_STATE_D.ESP32_Passthrough_ErrFlag = 1;
        DEBUG("当前连接端口号为空\r\n");
    }
    else
        DEBUG("当前连接端口号为：%s\r\n", ESP32_Val->Upper_SetWFCIPSTART_RemotePort);

    if(ESP32_STATE_D.ESP32_Passthrough_ErrFlag == 1)
    {
        return ESP32_ERR;
    }

    return ESP32_OK;
}

/******************************************************************************************************************************************************************************
** 函数名:uint8_t  ESP32_SetMode_Dis(struct Upper_WFVariable *ESP32_Val_Set)
** 功能描述:ESP32的设置模式的指令设置
** 输入参数:无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
uint8_t  ESP32_SetMode_Dis(struct Upper_WFVariable *ESP32_Val_Set)
{
    switch(ESP32_STATE_D.ESP32_SetModeState)
    {
        case 0:

            if(ESP32_S_bit.ESP32_f.ESP32_READY_f)
            {
                DEBUG("配置ESP32的WIFI模式成功\r\n");
                ESP32_STATE_D.ESP32_SetModeState = 1;
            }
            else	if(ESP32_S_bit.ESP32_f.ESP32_ERROR_f)
            {
                ESP32_STATE_D.ESP32_SetModeState = 7;
            }
            else
            {
                Print(&huart2, "%s\r\n", ESP32_ATRST);
            }

            break;

        case 1:
            if(ESP32_S_bit.ESP32_f.ESP32_OK_f)
            {
                DEBUG("配置ESP32的WIFI模式成功\r\n");
                ESP32_STATE_D.ESP32_SetModeState = 2;
            }
            else	if(ESP32_S_bit.ESP32_f.ESP32_ERROR_f)
            {
                ESP32_STATE_D.ESP32_SetModeState = 7;
            }
            else
            {
//						Print(&huart2,"AT+CWMODE=%d\r\n",ESP32_Val_Set->Upper_SetWFMode);
                Print(&huart2, "AT+CWMODE=3\r\n");

            }

            break;

        case 2:
            if(ESP32_S_bit.ESP32_f.ESP32_OK_f)
            {
                DEBUG("配置ESP32的SSID和PWD成功\r\n");
                ESP32_STATE_D.ESP32_SetModeState = 3;
            }
            else	if(ESP32_S_bit.ESP32_f.ESP32_ERROR_f)
            {
                ESP32_STATE_D.ESP32_SetModeState = 7;
            }
            else
            {
                Print(&huart2, "AT+CWJAP=\"%s\",\"%s\",%s\r\n", ESP32_Val_Set->Upper_SetWFJAP_SSID, ESP32_Val_Set->Upper_SetWFJAP_PWD);
            }

            break;

        case 3:
            if(ESP32_S_bit.ESP32_f.ESP32_OK_f)
            {
                DEBUG("设置IP的域名和端口\r\n");
                ESP32_STATE_D.ESP32_SetModeState = 4;
            }
            else	if(ESP32_S_bit.ESP32_f.ESP32_ERROR_f)
            {
                ESP32_STATE_D.ESP32_SetModeState = 7;
            }
            else
            {
                Print(&huart2, "AT+CIPSTART=\"%s\",\"%s\"\r\n", ESP32_Val_Set->Upper_SetWFCIPSTART_Type, ESP32_Val_Set->Upper_SetWFCIPSTART_RemoteIP, ESP32_Val_Set->Upper_SetWFCIPSTART_RemotePort);
            }

            break;

        case 4:
            if(ESP32_S_bit.ESP32_f.ESP32_OK_f)
            {
                DEBUG("准备开启透传模式\r\n");
                ESP32_STATE_D.ESP32_SetModeState = 5;
            }
            else	if(ESP32_S_bit.ESP32_f.ESP32_ERROR_f)
            {
                ESP32_STATE_D.ESP32_SetModeState = 7;
            }
            else
            {
                Print(&huart2, "AT+CIPMODE=1\r\n");

            }

            break;

        case 5:
            if(ESP32_S_bit.ESP32_f.ESP32_OK_f)
            {
                DEBUG("开启透传模式\r\n");
                ESP32_STATE_D.ESP32_SetModeState = 6;
            }
            else	if(ESP32_S_bit.ESP32_f.ESP32_ERROR_f)
            {
                ESP32_STATE_D.ESP32_SetModeState = 7;
            }
            else
            {
                Print(&huart2, "AT+CIPSEND\r\n");
                DEBUG("模组不在线\r\n");
            }

            break;

        case 6:
            return ESP32_OK;
            break;

        case 7:
            return ESP32_ERR;
            break;

    }

    return ESP32_ERR;

}


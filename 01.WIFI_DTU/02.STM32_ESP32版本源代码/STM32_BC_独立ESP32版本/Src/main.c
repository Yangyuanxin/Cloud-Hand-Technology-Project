/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "DTU_SYS_Dis.h"
#include "Flash_Dis.h"
#include "ESP32_Config.h"

__IO ITStatus Uart1Ready = RESET;
__IO ITStatus Uart2Ready = RESET;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void	SystemHardware_Init(void);
static void	Uart1_Dataecho(void);
static void	Uart2_Dataecho(void);
/* USER CODE BEGIN PFP */
/***************************************************
串口数据变量定义
***************************************************/
uint8_t Uart1_Rx_Len = 0;
uint8_t Uart1_Recv_End_Flag = 0;
uint8_t Uart1_Rx_Buffer[Uart1_BUFFER_SIZE] = {0};

uint8_t Uart2_Rx_Len = 0;
uint8_t Uart2_Recv_End_Flag = 0;
uint8_t Uart2_Rx_Buffer[Uart2_BUFFER_SIZE] = {0};

/***************************************************
按键变量定义
***************************************************/
uint16_t  Time_1S_Cnt = 0;
uint8_t  Time_KEY_Cnt = 0;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void DMA_Usart1_Write(uint8_t *buf, uint8_t len)
{
    if(HAL_UART_Transmit_DMA(&huart1, buf, len) != HAL_OK)
    {
        Error_Handler();
    }

    /*##-3- Wait for the end of the transfer ###################################*/
    while (Uart1Ready != SET) {}

    /* Reset transmission flag */
    Uart1Ready = RESET;
}

void DMA_Usart2_Write(uint8_t *buf, uint8_t len)
{
    if(HAL_UART_Transmit_DMA(&huart2, buf, len) != HAL_OK)
    {
        Error_Handler();
    }

    /*##-3- Wait for the end of the transfer ###################################*/
    while (Uart2Ready != SET) {}

    /* Reset transmission flag */
    Uart2Ready = RESET;
}

void DMA_Usart1_Read(uint8_t *Data, uint8_t len)
{
    HAL_UART_Receive_DMA(&huart1, Data, len); //重新打开DMA接收
}

void DMA_Usart2_Read(uint8_t *Data, uint8_t len)
{
    HAL_UART_Receive_DMA(&huart2, Data, len); //重新打开DMA接收
}
/**************************************
printf函数的重定义
**************************************/
int fputc(int ch, FILE*file)
{
    return HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 100);
}

/**
  * @name   Print

  * @brief  USARTx打印数据

  * @param  USARTx：USARTx

  * @param  formt：打印格式

  * @param  ...：不定参数

  * @retval 无
  */
void Print(UART_HandleTypeDef * USARTx, const char *formt, ...)
{
    va_list ap;
    unsigned char buf[250];
    unsigned char *pStr = buf;
    va_start(ap, formt);
    vsnprintf((char *)buf, sizeof(buf), formt, ap);
    va_end(ap);

    while(*pStr != 0)
    {
        HAL_UART_Transmit(USARTx, (uint8_t*)pStr++, 1, 100);

        while (__HAL_UART_GET_FLAG(USARTx, UART_FLAG_TC) != SET);
    }
}

uint8_t Debug_f;
void DEBUG(const char *cmd, ...)
{
    if(1 == Debug_f)
    {
        printf("[File:%s, Line:%d]", __FILE__, __LINE__);
        va_list args;
        va_start(args, cmd);
        vprintf(cmd, args);
        va_end(args);
//     printf("\n");
    }
}

void   Uart1_Dataecho(void)
{
    if(Uart1_Recv_End_Flag == 1)
    {
        DEBUG("rx_len = %d\r\n", Uart1_Rx_Len); //打印接收长度
        DMA_Usart1_Write(Uart1_Rx_Buffer, Uart1_Rx_Len);
        Uart1_Rx_Len = 0; //清除计数
        Uart1_Recv_End_Flag = 0; //清除接收结束标志位
//				printf("%s\r\n",Uart1_Rx_Buffer);
        memset(Uart1_Rx_Buffer, 0, sizeof(Uart1_Rx_Buffer));
    }
}


void   Uart2_Dataecho(void)
{
    if(Uart2_Recv_End_Flag == 1)
    {
        DEBUG("rx_len = %d\r\n", Uart2_Rx_Len); //打印接收长度
        DMA_Usart2_Write(Uart2_Rx_Buffer, Uart2_Rx_Len);
        Uart2_Rx_Len = 0; //清除计数
        Uart2_Recv_End_Flag = 0; //清除接收结束标志位
//				printf("%s\r\n",Uart2_Rx_Buffer);
        memset(Uart2_Rx_Buffer, 0, sizeof(Uart2_Rx_Buffer));
    }
}
/**
  * @brief SYS_Init
  * @retval None
  */

static void	SystemHardware_Init(void)
{
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();											//HAL库的初始化

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();						//系统时钟初始化

    /* USER CODE BEGIN SysInit */
    SystemHardware_Init();					//系统硬件初始化
    KEY_Ready();
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */

    /* USER CODE BEGIN 2 */
    DTU_ReadFlash_SetData();		//读取Flash的信息
    DEBUG("系统初始化完成\r\n");

    /*Flash TEST*/
//	Flash_Test();
    /* USER CODE END 2 */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
//		Print(&huart2,"ATE0\r\n");		//开机发送关闭回显
//		HAL_Delay(1000);
//		ESP32_DataDis();
//			Print(&huart2,"AT+CWMODE=4\r\n");
//		Print(&huart2,"AT+RST\r\n");
//		HAL_Delay(5000);
//		ESP32_DataDis();
//		while(1);
//		Print(&huart2,"AT+CWMODE=3\r\n");
//		HAL_Delay(1000);
//		ESP32_DataDis();
//		Print(&huart2,"AT+CWJAP=\"WANG\",\"0123456789\"\r\n");
//		HAL_Delay(5000);
//		ESP32_DataDis();
//		Print(&huart2,"AT+CIPSTART=\"TCP\",\"192.168.43.200\",10500\r\n");
//		HAL_Delay(3000);
//		ESP32_DataDis();
//		Print(&huart2,"AT+CIPMODE=1\r\n");
//		HAL_Delay(2000);
//		ESP32_DataDis();
//		Print(&huart2,"AT+CIPSEND\r\n");
//		HAL_Delay(1000);
//		ESP32_DataDis();
//		Print(&huart2,"123456\r\n");
//		HAL_Delay(1000);
//		ESP32_DataDis();
//		Print(&huart2,"88888888\r\n");
//		HAL_Delay(1000);
//		ESP32_DataDis();
//    /* USER CODE END WHILE */
////		Uart1_Dataecho();
////		Uart2_Dataecho();
//		while(1)
//		{
//			ESP32_DataDis();
//			Print(&huart2,"88888888\r\n");
//			HAL_Delay(1000);
//		}
//			Flash_Test();
//			while(1);
        DTU_State_Dispose();
        /* USER CODE BEGIN 3 */
    }

    /* USER CODE END 3 */
}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

    /* USER CODE BEGIN TIM1_Init 0 */

    /* USER CODE END TIM1_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM1_Init 1 */

    /* USER CODE END TIM1_Init 1 */
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 64;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 1000;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_TIM_Base_Start_IT(&htim1);			//开启定时器1中断

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

    if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* USER CODE BEGIN TIM1_Init 2 */

    /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 64;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 1000;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_TIM_Base_Start_IT(&htim2);			//开启定时器1中断

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }

    /* USER CODE BEGIN USART1_Init 2 */
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);          //使能UART1的IDLE中断（空闲中断）
    HAL_UART_Receive_DMA(&huart1, Uart1_Rx_Buffer, Uart1_BUFFER_SIZE); //开启一次接收
    /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }

    /* USER CODE BEGIN USART2_Init 2 */
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);          //使能UART2的IDLE中断（空闲中断）
    HAL_UART_Receive_DMA(&huart2, Uart2_Rx_Buffer, Uart2_BUFFER_SIZE); //开启一次接收
    /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    /* DMA1_Channel5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    /* DMA1_Channel6_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
    /* DMA1_Channel7_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 使能GPIO的时钟 */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*配置GPIO口默认输出的点平 */
    HAL_GPIO_WritePin(GPIOA, LED_R_Pin | LED_G_Pin | LED_B_Pin, GPIO_PIN_SET);

    /*配置按键的IO口: KEY_Pin */
    GPIO_InitStruct.Pin = KEY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY_GPIO_Port, &GPIO_InitStruct);

    /*配置RGB灯的三个脚 : LED_R_Pin LED_G_Pin LED_B_Pin */
    GPIO_InitStruct.Pin = LED_R_Pin | LED_G_Pin | LED_B_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/* USER CODE BEGIN 4 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    /* Set transmission flag: transfer complete */
    if(UartHandle->Instance == USART1)
        Uart1Ready = SET;

    if(UartHandle->Instance == USART2)
        Uart2Ready = SET;

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    /* Set transmission flag: transfer complete */
    if(UartHandle->Instance == USART1)
        Uart1Ready = SET;

    if(UartHandle->Instance == USART2)
        Uart2Ready = SET;

//	HAL_UART_DMAStop(&huart1);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

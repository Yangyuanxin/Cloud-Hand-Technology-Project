#include "Lsens.h"
#include "delay.h"



void ADC1_Init(void)
{ 
		ADC_InitTypeDef ADC_InitStructure;


		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); //使能ADC3通道时钟
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);//ADC复位
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//复位结束

		ADC_DeInit(ADC1);//复位ADC3,将外设 ADC3的全部寄存器重设为缺省值

		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC工作模式: 独立模式
		ADC_InitStructure.ADC_ScanConvMode = DISABLE; //模数转换工作在单通道模式
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //模数转换工作在单次转换模式
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换由软件而不是外部触发启动
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC数据右对齐
		ADC_InitStructure.ADC_NbrOfChannel = 1; //顺序进行规则转换的ADC通道的数目
		ADC_Init(ADC1, &ADC_InitStructure); //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器


		ADC_Cmd(ADC1, ENABLE); //使能指定的ADC1

		ADC_ResetCalibration(ADC1); //使能复位校准 

		while(ADC_GetResetCalibrationStatus(ADC1)); //等待复位校准结束

		ADC_StartCalibration(ADC1); //开启AD校准

		while(ADC_GetCalibrationStatus(ADC1)); //等待校准结束

}

//获得ADC3某个通道的值
//ch:通道值 0~16
//返回值:转换结果
u16 Get_Adc1(u8 ch)
{
//设置指定ADC的规则组通道，一个序列，采样时间
ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 ); //ADC1,ADC通道,采样时间为239.5周期
ADC_SoftwareStartConvCmd(ADC1, ENABLE); //使能指定的ADC3的软件转换启动功能
while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
return ADC_GetConversionValue(ADC1); //返回最近一次ADC3规则组的转换结果
}


void Lsens_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTF时钟
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA0 anolog输入
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入引脚
GPIO_Init(GPIOA, &GPIO_InitStructure);
ADC1_Init();//这里我们可以直接调用ADC的初始化，从而将整个函数封装成光敏传感器的初始化函数，一会儿主函数中直接调用

}

//对光敏传感器初始化之后需要将我们ADC1采集到的值进行一些转换，从而直接通过LCD上显示出来的值的大小判断光强，其范围设置为0~100。封装函数如下：

//读取Light Sens的值
//0~1000:0,最亮; 1000,最暗
u16 Lsens_Get_Val(void)
{
u32 temp_val=0;
u8 t;
for(t=0;t<Lsens_Sum;t++)
{
temp_val+=Get_Adc1(Lsens_Ch); //读取ADC值
delay_ms(5);
}

temp_val/=Lsens_Sum;//得到平均值

//3.3V对应4096，而光敏电阻分得的电压值不可能大于3.3V，也就是说ADC采集到的值不可能大于4096，这里以最大4000为界

if(temp_val>4000)
	temp_val=4000;

//光敏电阻分得的电压值越大，则（temp_val/40)的值越大，即光敏电阻分得的电压值越大；也就是是说，光照强度越弱，所以这里在分成0~100后，用100-100-(temp_val/40)的值也就越小

return (u16)(Lsens_Max-(temp_val));
//return (u16)(Lsens_Max-(temp_val));
}

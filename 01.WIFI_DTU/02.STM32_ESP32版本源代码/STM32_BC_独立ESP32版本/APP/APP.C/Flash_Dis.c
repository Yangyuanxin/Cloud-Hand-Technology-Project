#include "Flash_Dis.h"


uint32_t WriteFlashData = 0x12345678;
uint32_t addr =  0x08011000;
uint32_t addr1 = 0x08012000;
uint32_t addr2 = 0x08013000;

char string_Test[] = "Hell world";
char string_Test1[] = "chenzhiwang";
char string_Test2[] = "chenzhiwang123";
char string_ReadTest[] = "";

/******************************************************************************************************************************************************************************
** 函数名:void writeFlashTest(void)
** 功能描述:FLASH写入程序
** 输入参数:	无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void writeFlashTest(void)
{
	/* 1/4解锁FLASH*/
	HAL_FLASH_Unlock();

	/* 2/4擦除FLASH*/
	/*初始化FLASH_EraseInitTypeDef*/
	/*擦除方式页擦除FLASH_TYPEERASE_PAGES，块擦除FLASH_TYPEERASE_MASSERASE*/
	/*擦除地址*/
	/*擦除页数*/
	FLASH_EraseInitTypeDef FlashSet;
	FlashSet.TypeErase = FLASH_TYPEERASE_PAGES;
	FlashSet.PageAddress = addr;
	FlashSet.NbPages = 1;
	
	/*设置PageError，调用擦除函数*/
	uint32_t PageError = 0;					//如果出现错误这个变量会被设置为出错的FLASH地址
	HAL_FLASHEx_Erase(&FlashSet, &PageError);

	/* 3/4对FLASH烧写*/
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, WriteFlashData);

	/* 4/4锁住FLASH*/
	HAL_FLASH_Lock();
}
/******************************************************************************************************************************************************************************
** 函数名:void printFlashTest(void)
** 功能描述:FLASH读取打印程序
** 输入参数:	无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void printFlashTest(void)
{
	uint32_t temp = *(__IO uint32_t*)(addr);

	DEBUG("addr is:0x%x, data is:0x%x\r\n", addr, temp);
}
/******************************************************************************************************************************************************************************
** 函数名:void   Flash_Test(void)
** 功能描述:Flash的测试程序
** 输入参数:	无
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void   Flash_Test(void)
{
	Flash_WriteData(addr,string_Test,sizeof(string_Test));
	Flash_ReadData(addr,string_ReadTest);
	DEBUG("数据解析完成\r\n");
	DEBUG("数据为：%s\r\n",string_ReadTest);
	HAL_Delay(1000);
	Flash_WriteData(addr1,string_Test1,sizeof(string_Test1));
	Flash_ReadData(addr1,string_ReadTest);
	DEBUG("数据解析完成\r\n");
	DEBUG("数据为：%s\r\n",string_ReadTest);
	HAL_Delay(1000);
	Flash_WriteData(addr2,string_Test2,sizeof(string_Test2));
	Flash_ReadData(addr2,string_ReadTest);
	DEBUG("数据解析完成\r\n");
	DEBUG("数据为：%s\r\n",string_ReadTest);
}

uint32_t FLASH_ReadHalfWord(uint32_t faddr)
{
	return *(__IO uint32_t*)faddr; 
}

/******************************************************************************************************************************************************************************
** 函数名:void Flash_WriteData(uint32_t Writeaddr,char *data,uint16_t Size)
** 功能描述:从指定地址开始读出指定长度的数据
** 输入参数:	ReadAddr  		起始地址
						*pBuffer			数据指针
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void Flash_ReadData(uint32_t ReadAddr,char *pBuffer)   	
{
	uint16_t i = 0;
	
	while((uint8_t)FLASH_ReadHalfWord(ReadAddr) != 0xFF)
	{
		pBuffer[i]=(char)FLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
		i++;
	}
	
}
/******************************************************************************************************************************************************************************
** 函数名:void Flash_WriteData(uint32_t Writeaddr,char *data,uint16_t Size)
** 功能描述:写入一个地址,，分别写入一个字符串数据和存储的长度
** 输入参数:	Writeaddr1  地址1
						*data1			数据指针
						Size1				数据指针的长度
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
/******************************************************************************************************************************************************************************
//注：不得在程序使用区内写入和擦除否则程序跑飞。（请关注自己写的程序空间位置）
******************************************************************************************************************************************************************************/
void Flash_WriteData(uint32_t Writeaddr,char *data,uint16_t Size)
{
	
	if(Writeaddr<FLASH_BASE||((Writeaddr+Size)>=(FLASH_BASE+1024*FLASH_SIZE)))

{
	DEBUG("非法地址:0x%x\r\n", Writeaddr);
	return;//非法地址

}
	DEBUG("正常地址:0x%x\r\n", Writeaddr);
	DEBUG("存放的数据:%s，长度：%d\r\n", data,Size);
	HAL_FLASH_Unlock(); //解锁写保护
    //1、解锁FLASH
    HAL_FLASH_Unlock();

    //2、擦除FLASH
    //初始化FLASH_EraseInitTypeDef
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.PageAddress = Writeaddr;
    f.NbPages = 1;
    //设置PageError
    uint32_t PageError = 0;
    //调用擦除函数
    HAL_FLASHEx_Erase(&f, &PageError);
    //3、对FLASH烧写
    uint16_t TempBuf = 0;
    for(uint16_t i = 0;i< Size ;i++)
    {
        TempBuf = (uint8_t)*(data+i);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD , Writeaddr + i * 2, TempBuf);
    }
    //4、锁住FLASH
    HAL_FLASH_Lock();
}
/******************************************************************************************************************************************************************************
** 函数名:void Flash_WriteData_2(uint32_t Writeaddr1,uint32_t Writeaddr2,char *data1,uint16_t Size1,char *data2,uint16_t Size2)
** 功能描述:写入两个地址,（两个地址需要在Flash的同一页），分别写入两个字符串数据和存储的长度
** 输入参数:	Writeaddr1  起始地址1
					  Writeaddr2	起始地址2
						*data1			字符串1数据指针
						*data2			字符串2数据指针
						Size1				字符串1数据指针的长度
						Size2				字符串2数据指针的长度
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void Flash_WriteData_2(uint32_t Writeaddr1,uint32_t Writeaddr2,char *data1,uint16_t Size1,char *data2,uint16_t Size2)
{
	
	if(Writeaddr1<FLASH_BASE||((Writeaddr1+Size1)>=(FLASH_BASE+1024*FLASH_SIZE)))
	{
		DEBUG("非法地址:0x%x\r\n", Writeaddr1);
		return;//非法地址

	}
	if(Writeaddr2<FLASH_BASE||((Writeaddr2+Size2)>=(FLASH_BASE+1024*FLASH_SIZE)))
	{
		DEBUG("非法地址:0x%x\r\n", Writeaddr2);
		return;//非法地址

	}
	DEBUG("字符串1正常地址:0x%x\r\n", Writeaddr1);
	DEBUG("字符串2正常地址:0x%x\r\n", Writeaddr2);
	DEBUG("字符串1存放的数据:%s，长度：%d\r\n", data1,Size1);
	DEBUG("字符串2存放的数据:%s，长度：%d\r\n", data2,Size2);
	HAL_FLASH_Unlock(); //解锁写保护
    //1、解锁FLASH
    HAL_FLASH_Unlock();

    //2、擦除FLASH
    //初始化FLASH_EraseInitTypeDef
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.PageAddress = Writeaddr1;
    f.NbPages = 1;
    //设置PageError
    uint32_t PageError = 0;
    //调用擦除函数
    HAL_FLASHEx_Erase(&f, &PageError);
    //3、对FLASH烧写
    uint16_t TempBuf = 0;
    for(uint16_t i = 0;i< Size1 ;i++)
    {
        TempBuf = (uint8_t)*(data1+i);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD , Writeaddr1 + i * 2, TempBuf);
    }
		
		TempBuf = 0;
    for(uint16_t i = 0;i< Size2 ;i++)
    {
        TempBuf = (uint8_t)*(data2+i);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD , Writeaddr2 + i * 2, TempBuf);
    }
    //4、锁住FLASH
    HAL_FLASH_Lock();
		DEBUG("存放成功\r\n");
}
/******************************************************************************************************************************************************************************
** 函数名:void Flash_WriteData_3(uint32_t Writeaddr1,uint32_t Writeaddr2,uint32_t Writeaddr3,char *data1,uint16_t Size1,char *data2,uint16_t Size2,char *data3,uint16_t Size3)
** 功能描述:写入三个地址,（三个地址需要在Flash的同一页），分别写入三个字符串数据和存储的长度
** 输入参数:	Writeaddr1  起始地址1
					  Writeaddr2	起始地址2
						Writeaddr3	起始地址3
						*data1			字符串1数据指针
						*data2			字符串2数据指针
						*data3			字符串3数据指针
						Size1				字符串1数据指针的长度
						Size2				字符串2数据指针的长度
						Size3				字符串3数据指针的长度
** 输出参数:无

** 返回：无
*******************************************************************************************************************************************************************************/
void Flash_WriteData_3(uint32_t Writeaddr1,uint32_t Writeaddr2,uint32_t Writeaddr3,char *data1,uint16_t Size1,char *data2,uint16_t Size2,char *data3,uint16_t Size3)
{
	
	if(Writeaddr1<FLASH_BASE||((Writeaddr1+Size1)>=(FLASH_BASE+1024*FLASH_SIZE)))
	{
		DEBUG("非法地址:0x%x\r\n", Writeaddr1);
		return;//非法地址
	}
	if(Writeaddr2<FLASH_BASE||((Writeaddr2+Size2)>=(FLASH_BASE+1024*FLASH_SIZE)))
	{
		DEBUG("非法地址:0x%x\r\n", Writeaddr2);
		return;//非法地址
	}
	if(Writeaddr3<FLASH_BASE||((Writeaddr3+Size3)>=(FLASH_BASE+1024*FLASH_SIZE)))
	{
		DEBUG("非法地址:0x%x\r\n", Writeaddr3);
		return;//非法地址
	}
	
	DEBUG("字符串1正常地址:0x%x\r\n", Writeaddr1);
	DEBUG("字符串2正常地址:0x%x\r\n", Writeaddr1);
	DEBUG("字符串3正常地址:0x%x\r\n", Writeaddr1);
	DEBUG("字符串1存放的数据:%s，长度：%d\r\n", data1,Size1);
	DEBUG("字符串2存放的数据:%s，长度：%d\r\n", data2,Size2);
	DEBUG("字符串3存放的数据:%s，长度：%d\r\n", data3,Size3);
	HAL_FLASH_Unlock(); //解锁写保护
    //1、解锁FLASH
    HAL_FLASH_Unlock();

    //2、擦除FLASH
    //初始化FLASH_EraseInitTypeDef
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.PageAddress = Writeaddr1;
    f.NbPages = 1;
    //设置PageError
    uint32_t PageError = 0;
    //调用擦除函数
    HAL_FLASHEx_Erase(&f, &PageError);
    //3、对FLASH烧写
    uint16_t TempBuf = 0;
    for(uint16_t i = 0;i< Size1 ;i++)
    {
        TempBuf = (uint8_t)*(data1+i);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD , Writeaddr1 + i * 2, TempBuf);
    }
		TempBuf = 0;
    for(uint16_t i = 0;i< Size2 ;i++)
    {
        TempBuf = (uint8_t)*(data2+i);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD , Writeaddr2 + i * 2, TempBuf);
    }
		TempBuf = 0;
    for(uint16_t i = 0;i< Size3 ;i++)
    {
        TempBuf = (uint8_t)*(data3+i);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD , Writeaddr3 + i * 2, TempBuf);
    }
    //4、锁住FLASH
    HAL_FLASH_Lock();
}



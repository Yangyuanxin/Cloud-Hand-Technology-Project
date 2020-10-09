#ifndef __Flash_Dis_H
#define __Flash_Dis_H

#include "main.h"
#include "DTU_SYS_Dis.h"

#define FLASH_SIZE 128 //所选MCU的FLASH容量大小(单位为K)

#if FLASH_SIZE<256

#define SECTOR_SIZE 1024 //字节

#else

#define SECTOR_SIZE 2048 //字节

#endif





void writeFlashTest(void);
void printFlashTest(void);
void   Flash_Test(void);
uint32_t FLASH_ReadHalfWord(uint32_t address);
void Flash_ReadData(uint32_t ReadAddr,char *pBuffer) ;
void Flash_WriteData(uint32_t addr,char *data,uint16_t Size);
void Flash_WriteData_2(uint32_t Writeaddr1,uint32_t Writeaddr2,char *data1,uint16_t Size1,char *data2,uint16_t Size2);
void Flash_WriteData_3(uint32_t Writeaddr1,uint32_t Writeaddr2,uint32_t Writeaddr3,char *data1,uint16_t Size1,char *data2,uint16_t Size2,char *data3,uint16_t Size3);
#endif /* __Flash_Dis_H */

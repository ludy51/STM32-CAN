#ifndef __flash_spi_H
#define __flash_spi_H

#include "stm32f10x.h"
/****
本函数库以W25QW25Q128为例：

地址位数24位
每一页256字节
4Kbytes为一个Sector
16个扇区为1个Block

W25Q128
容量为16M字节（128M-bit）,共有128个Block,4096个Sector 												

***/
//W25X系列/Q系列芯片列表	   
//W25Q80  ID  0XEF13
//W25Q16  ID  0XEF14
//W25Q32  ID  0XEF15
//W25Q64  ID  0XEF16	
//W25Q128 ID  0XEF17	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
//指令表
#define W25X_WriteEnable		0x06  //写使能
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03   //读指令
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02  //页写命令
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 //扇区擦除
#define W25X_ChipErase			0xC7  //芯片擦除
#define W25X_PowerDown			0xB9   //掉电
#define W25X_ReleasePowerDown	0xAB  //唤醒
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

//默认FLASH的IO口选用SPI3 
#define CS_ON   GPIO_ResetBits(GPIOA, GPIO_Pin_15)//片选口使能（SPI3）
#define CS_OFF  GPIO_SetBits(GPIOA, GPIO_Pin_15)//片选口失能

u8 FLASH_Read_byte(SPI_TypeDef* SPIx , u32 addr);//读一字节数据
void FLASH_Read_Str(SPI_TypeDef* SPIx, u32 addr ,u16 NumByteToRead, u8* StrBuffer);//读取指定长度字符串（I2Cx，首地址，长度，读出存储区）

void FLASH_Write(SPI_TypeDef* SPIx, u32 addr , u8* strDate, u32 length );//写任意长度数据（通用）（SPIx，首地址，写入数组，写入长度）
void FLASH_Write_NoChack(SPI_TypeDef* SPIx, u32 addr , u8* StrBuffer, u32 length );//只有当扇区已经被擦除过，可以写入任意长度数据
void FLASH_Write_page(SPI_TypeDef* SPIx, u32 addr , u8* StrBuffer, u32 length );//仅限于单页内写一定长度数据

void FLASH_SectorErase(SPI_TypeDef* SPIx, u32 SectorStart);//扇区擦除（SPIx ， 扇区起点地址）
void FLASH_Erase_Chip(SPI_TypeDef* SPIx);//芯片整个擦除
void FLASH_Wait_Busy(SPI_TypeDef* SPIx);//等待FLASH忙完

void FLASH_PowerDown(SPI_TypeDef* SPIx);//进入掉电
void FLASH_WAKEUP(SPI_TypeDef* SPIx);  //唤醒

//u8 SectorErase_Judge(SPI_TypeDef* SPIx, u16 Sector_num)  //判断该扇区是否要擦除

/**
笔记：： 1.FLASH的写操作，只要在同一页上面，输入好地址，可以地址自加1并进行数据连续的写入，如果不在同一页上，需要重新写入地址。
         2.FLASH对于读取操作，不受页的限制，只要输入地址，可以不断的连续读。
**/

#endif



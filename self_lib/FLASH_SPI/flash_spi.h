#ifndef __flash_spi_H
#define __flash_spi_H

#include "stm32f10x.h"
/****
����������W25QW25Q128Ϊ����

��ַλ��24λ
ÿһҳ256�ֽ�
4KbytesΪһ��Sector
16������Ϊ1��Block

W25Q128
����Ϊ16M�ֽڣ�128M-bit��,����128��Block,4096��Sector 												

***/
//W25Xϵ��/Qϵ��оƬ�б�	   
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
//ָ���
#define W25X_WriteEnable		0x06  //дʹ��
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03   //��ָ��
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02  //ҳд����
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 //��������
#define W25X_ChipErase			0xC7  //оƬ����
#define W25X_PowerDown			0xB9   //����
#define W25X_ReleasePowerDown	0xAB  //����
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

//Ĭ��FLASH��IO��ѡ��SPI3 
#define CS_ON   GPIO_ResetBits(GPIOA, GPIO_Pin_15)//Ƭѡ��ʹ�ܣ�SPI3��
#define CS_OFF  GPIO_SetBits(GPIOA, GPIO_Pin_15)//Ƭѡ��ʧ��

u8 FLASH_Read_byte(SPI_TypeDef* SPIx , u32 addr);//��һ�ֽ�����
void FLASH_Read_Str(SPI_TypeDef* SPIx, u32 addr ,u16 NumByteToRead, u8* StrBuffer);//��ȡָ�������ַ�����I2Cx���׵�ַ�����ȣ������洢����

void FLASH_Write(SPI_TypeDef* SPIx, u32 addr , u8* strDate, u32 length );//д���ⳤ�����ݣ�ͨ�ã���SPIx���׵�ַ��д�����飬д�볤�ȣ�
void FLASH_Write_NoChack(SPI_TypeDef* SPIx, u32 addr , u8* StrBuffer, u32 length );//ֻ�е������Ѿ���������������д�����ⳤ������
void FLASH_Write_page(SPI_TypeDef* SPIx, u32 addr , u8* StrBuffer, u32 length );//�����ڵ�ҳ��дһ����������

void FLASH_SectorErase(SPI_TypeDef* SPIx, u32 SectorStart);//����������SPIx �� ��������ַ��
void FLASH_Erase_Chip(SPI_TypeDef* SPIx);//оƬ��������
void FLASH_Wait_Busy(SPI_TypeDef* SPIx);//�ȴ�FLASHæ��

void FLASH_PowerDown(SPI_TypeDef* SPIx);//�������
void FLASH_WAKEUP(SPI_TypeDef* SPIx);  //����

//u8 SectorErase_Judge(SPI_TypeDef* SPIx, u16 Sector_num)  //�жϸ������Ƿ�Ҫ����

/**
�ʼǣ��� 1.FLASH��д������ֻҪ��ͬһҳ���棬����õ�ַ�����Ե�ַ�Լ�1����������������д�룬�������ͬһҳ�ϣ���Ҫ����д���ַ��
         2.FLASH���ڶ�ȡ����������ҳ�����ƣ�ֻҪ�����ַ�����Բ��ϵ���������
**/

#endif



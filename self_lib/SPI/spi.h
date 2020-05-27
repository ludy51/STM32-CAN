#ifndef __spi_H
#define __spi_H

#include "stm32f10x.h"

void SPIInit(SPI_TypeDef* SPIx);
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx , u8 TxData);//���Ͳ�����һ���ֽڣ�����spiͨ��Э�飬ֻ���ȷ��ͺ���ܽ��գ�
void Set_SPI_BaudRate(SPI_TypeDef* SPIx , u16 SPI_BaudRatePrescaler_x);//����spi������Ԥ��Ƶ��ֵ

#endif

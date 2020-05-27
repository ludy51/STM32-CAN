#ifndef __spi_H
#define __spi_H

#include "stm32f10x.h"

void SPIInit(SPI_TypeDef* SPIx);
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx , u8 TxData);//发送并接收一个字节（根据spi通信协议，只有先发送后才能接收）
void Set_SPI_BaudRate(SPI_TypeDef* SPIx , u16 SPI_BaudRatePrescaler_x);//更改spi波特率预分频的值

#endif

#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h"
//#include "includes.h"

//DMA初始化
//传入参数：通道 存储器基地址  外设基地址 数据量
void DMAx_ADCx_Init(DMA_Channel_TypeDef* DMAx_Channelx ,u32 DMA_DIR_Peripheralxxx , u32 MemoryBaseAddr, u32 PeripheralBaseAddr,u16 DMA_BufferSize);

void DMAx_USARTx_Init(DMA_Channel_TypeDef* DMAx_Channelx ,u32 DMA_DIR_Peripheralxxx , u32 MemoryBaseAddr, u32 PeripheralBaseAddr,u16 DMA_BufferSize);

void DMAx_Enable(DMA_Channel_TypeDef* DMAx_Channelx ,u16 DMA_BufferSize);//DMA使能
void DMA_Peripheral_Enable();//外设的DMA使能

#endif


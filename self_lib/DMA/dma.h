#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h"
//#include "includes.h"

//DMA��ʼ��
//���������ͨ�� �洢������ַ  �������ַ ������
void DMAx_ADCx_Init(DMA_Channel_TypeDef* DMAx_Channelx ,u32 DMA_DIR_Peripheralxxx , u32 MemoryBaseAddr, u32 PeripheralBaseAddr,u16 DMA_BufferSize);

void DMAx_USARTx_Init(DMA_Channel_TypeDef* DMAx_Channelx ,u32 DMA_DIR_Peripheralxxx , u32 MemoryBaseAddr, u32 PeripheralBaseAddr,u16 DMA_BufferSize);

void DMAx_Enable(DMA_Channel_TypeDef* DMAx_Channelx ,u16 DMA_BufferSize);//DMAʹ��
void DMA_Peripheral_Enable();//�����DMAʹ��

#endif


#include "dma.h"

//名称：DMA ADC初始化
//传入参数：通道 传输方向 存储器基地址  外设基地址 数据量
//注;传入地址位数32位
//说明：默认非循环，存储器地址不自加，外设不自加，数据宽度都为16位
void DMAx_ADCx_Init(DMA_Channel_TypeDef* DMAx_Channelx ,u32 DMA_DIR_Peripheralxxx , u32 MemoryBaseAddr, u32 PeripheralBaseAddr,u16 DMA_BufferSize)
{
	DMA_InitTypeDef DMA_InitStruct;
	
	if(DMA1_Channel1 <= DMAx_Channelx && DMA1_Channel7 >= DMAx_Channelx)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA位于AHB时钟上
	}
	if(DMA2_Channel1 <= DMAx_Channelx && DMA2_Channel5 >= DMAx_Channelx)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//DMA位于AHB时钟上
	}
	
	DMA_DeInit(DMAx_Channelx);//
	
	
	DMA_InitStruct.DMA_BufferSize = DMA_BufferSize;//DMA传输的数据量
	DMA_InitStruct.DMA_DIR = DMA_DIR_Peripheralxxx;//传输方向：外设为目标地址（存储器到外设）
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable; //存储器到存储器传输失能
	DMA_InitStruct.DMA_MemoryBaseAddr = MemoryBaseAddr;//存储器基地址（可为sram或flash，程序变量一般位于sram）
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//存储器传输数据宽度（8,16,32位）
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;//存储器地址随传输而递增（+？加多少与数据宽度有关）
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular   不循环传输模式（循环或不循环）（循环：地址递增满后，又重载从基地址开始传输）（不循环：递增满后不重载且暂停）
	DMA_InitStruct.DMA_PeripheralBaseAddr = PeripheralBaseAddr;//外设寄存器的基地址（多是数据寄存器）
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设寄存器传输数据宽度（8,16,32位）
	DMA_InitStruct.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;//外设地址不进行递增
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;//传输优先级：高（四种：最高，高，中，低）
	
  DMA_Init(DMAx_Channelx, &DMA_InitStruct);
}


//名称：DMA USART初始化
//传入参数：通道 传输方向 存储器基地址  外设基地址 数据量
//注;传入地址位数32位
//说明：默认非循环，存储器地址自加，外设不自加，数据宽度都为8位
void DMAx_USARTx_Init(DMA_Channel_TypeDef* DMAx_Channelx ,u32 DMA_DIR_Peripheralxxx , u32 MemoryBaseAddr, u32 PeripheralBaseAddr,u16 DMA_BufferSize)
{
	DMA_InitTypeDef DMA_InitStruct;
	
	if(DMA1_Channel1 <= DMAx_Channelx && DMA1_Channel7 >= DMAx_Channelx)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA位于AHB时钟上
	}
	if(DMA2_Channel1 <= DMAx_Channelx && DMA2_Channel5 >= DMAx_Channelx)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//DMA位于AHB时钟上
	}
	
	DMA_DeInit(DMAx_Channelx);//
	
	
	DMA_InitStruct.DMA_BufferSize = DMA_BufferSize;//DMA传输的数据量
	DMA_InitStruct.DMA_DIR = DMA_DIR_Peripheralxxx;//传输方向：外设为目标地址（存储器到外设）
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable; //存储器到存储器传输失能
	DMA_InitStruct.DMA_MemoryBaseAddr = MemoryBaseAddr;//存储器基地址（可为sram或flash，程序变量一般位于sram）
	DMA_InitStruct.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte;// DMA_MemoryDataSize_HalfWord;//存储器传输数据宽度（8,16,32位）
	DMA_InitStruct.DMA_MemoryInc =DMA_MemoryInc_Enable;//DMA_MemoryInc_Disable;//存储器地址随传输而递增（加多少与数据宽度有关，可见芯片参考手册）
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular   不循环传输模式（循环或不循环）（循环：地址递增满后，又重载从基地址开始传输）（不循环：递增满后不重载且暂停）
	DMA_InitStruct.DMA_PeripheralBaseAddr = PeripheralBaseAddr;//外设寄存器的基地址（多是数据寄存器）
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// DMA_PeripheralDataSize_HalfWord;//外设寄存器传输数据宽度（8,16,32位）
	DMA_InitStruct.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;//外设地址不进行递增
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;//传输优先级：高（四种：最高，高，中，低）
	
  DMA_Init(DMAx_Channelx, &DMA_InitStruct);
}




//DMA启动传输
//传入参数：通道  数据量
//注意：使能前，要做传输完成的判断！
void DMAx_Enable(DMA_Channel_TypeDef* DMAx_Channelx ,u16 DMA_BufferSize)
{
	DMA_Cmd(DMAx_Channelx, DISABLE);//失能
	
	DMA_SetCurrDataCounter( DMAx_Channelx , DMA_BufferSize); //重新设置传输数据长度
	DMA_Cmd(DMAx_Channelx, ENABLE);//使能
	
}

//外设的DMA使能
//注意：不同外设对应初始化的DMA通道需要一致
void DMA_Peripheral_Enable()
{
	ADC_DMACmd(ADC1, ENABLE);
//	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}


#include "dma.h"

//���ƣ�DMA ADC��ʼ��
//���������ͨ�� ���䷽�� �洢������ַ  �������ַ ������
//ע;�����ַλ��32λ
//˵����Ĭ�Ϸ�ѭ�����洢����ַ���Լӣ����費�Լӣ����ݿ�ȶ�Ϊ16λ
void DMAx_ADCx_Init(DMA_Channel_TypeDef* DMAx_Channelx ,u32 DMA_DIR_Peripheralxxx , u32 MemoryBaseAddr, u32 PeripheralBaseAddr,u16 DMA_BufferSize)
{
	DMA_InitTypeDef DMA_InitStruct;
	
	if(DMA1_Channel1 <= DMAx_Channelx && DMA1_Channel7 >= DMAx_Channelx)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMAλ��AHBʱ����
	}
	if(DMA2_Channel1 <= DMAx_Channelx && DMA2_Channel5 >= DMAx_Channelx)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//DMAλ��AHBʱ����
	}
	
	DMA_DeInit(DMAx_Channelx);//
	
	
	DMA_InitStruct.DMA_BufferSize = DMA_BufferSize;//DMA�����������
	DMA_InitStruct.DMA_DIR = DMA_DIR_Peripheralxxx;//���䷽������ΪĿ���ַ���洢�������裩
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable; //�洢�����洢������ʧ��
	DMA_InitStruct.DMA_MemoryBaseAddr = MemoryBaseAddr;//�洢������ַ����Ϊsram��flash���������һ��λ��sram��
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�洢���������ݿ�ȣ�8,16,32λ��
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;//�洢����ַ�洫���������+���Ӷ��������ݿ���йأ�
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular   ��ѭ������ģʽ��ѭ����ѭ������ѭ������ַ�������������شӻ���ַ��ʼ���䣩����ѭ��������������������ͣ��
	DMA_InitStruct.DMA_PeripheralBaseAddr = PeripheralBaseAddr;//����Ĵ����Ļ���ַ���������ݼĴ�����
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//����Ĵ����������ݿ�ȣ�8,16,32λ��
	DMA_InitStruct.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;//�����ַ�����е���
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;//�������ȼ����ߣ����֣���ߣ��ߣ��У��ͣ�
	
  DMA_Init(DMAx_Channelx, &DMA_InitStruct);
}


//���ƣ�DMA USART��ʼ��
//���������ͨ�� ���䷽�� �洢������ַ  �������ַ ������
//ע;�����ַλ��32λ
//˵����Ĭ�Ϸ�ѭ�����洢����ַ�Լӣ����費�Լӣ����ݿ�ȶ�Ϊ8λ
void DMAx_USARTx_Init(DMA_Channel_TypeDef* DMAx_Channelx ,u32 DMA_DIR_Peripheralxxx , u32 MemoryBaseAddr, u32 PeripheralBaseAddr,u16 DMA_BufferSize)
{
	DMA_InitTypeDef DMA_InitStruct;
	
	if(DMA1_Channel1 <= DMAx_Channelx && DMA1_Channel7 >= DMAx_Channelx)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMAλ��AHBʱ����
	}
	if(DMA2_Channel1 <= DMAx_Channelx && DMA2_Channel5 >= DMAx_Channelx)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//DMAλ��AHBʱ����
	}
	
	DMA_DeInit(DMAx_Channelx);//
	
	
	DMA_InitStruct.DMA_BufferSize = DMA_BufferSize;//DMA�����������
	DMA_InitStruct.DMA_DIR = DMA_DIR_Peripheralxxx;//���䷽������ΪĿ���ַ���洢�������裩
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable; //�洢�����洢������ʧ��
	DMA_InitStruct.DMA_MemoryBaseAddr = MemoryBaseAddr;//�洢������ַ����Ϊsram��flash���������һ��λ��sram��
	DMA_InitStruct.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte;// DMA_MemoryDataSize_HalfWord;//�洢���������ݿ�ȣ�8,16,32λ��
	DMA_InitStruct.DMA_MemoryInc =DMA_MemoryInc_Enable;//DMA_MemoryInc_Disable;//�洢����ַ�洫����������Ӷ��������ݿ���йأ��ɼ�оƬ�ο��ֲᣩ
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular   ��ѭ������ģʽ��ѭ����ѭ������ѭ������ַ�������������شӻ���ַ��ʼ���䣩����ѭ��������������������ͣ��
	DMA_InitStruct.DMA_PeripheralBaseAddr = PeripheralBaseAddr;//����Ĵ����Ļ���ַ���������ݼĴ�����
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// DMA_PeripheralDataSize_HalfWord;//����Ĵ����������ݿ�ȣ�8,16,32λ��
	DMA_InitStruct.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;//�����ַ�����е���
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;//�������ȼ����ߣ����֣���ߣ��ߣ��У��ͣ�
	
  DMA_Init(DMAx_Channelx, &DMA_InitStruct);
}




//DMA��������
//���������ͨ��  ������
//ע�⣺ʹ��ǰ��Ҫ��������ɵ��жϣ�
void DMAx_Enable(DMA_Channel_TypeDef* DMAx_Channelx ,u16 DMA_BufferSize)
{
	DMA_Cmd(DMAx_Channelx, DISABLE);//ʧ��
	
	DMA_SetCurrDataCounter( DMAx_Channelx , DMA_BufferSize); //�������ô������ݳ���
	DMA_Cmd(DMAx_Channelx, ENABLE);//ʹ��
	
}

//�����DMAʹ��
//ע�⣺��ͬ�����Ӧ��ʼ����DMAͨ����Ҫһ��
void DMA_Peripheral_Enable()
{
	ADC_DMACmd(ADC1, ENABLE);
//	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}


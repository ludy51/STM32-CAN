#include "spi.h"
#include "delay.h"

void SPIInit(SPI_TypeDef* SPIx)
{
	GPIO_InitTypeDef GPIO_InitStructrue;
	SPI_InitTypeDef SPI_InitStructrue;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_AF_PP;//�����������	
	GPIO_InitStructrue.GPIO_Speed = GPIO_Speed_50MHz;
	if(SPIx == SPI1)
	{
		GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;  //  MISO��һ������Ϊ�������룬�����ڸýŲ�����������������ø������������Ӱ�죨������LED��һ����
		                              //SCK          MISO        MOSI
		GPIO_Init(GPIOA, &GPIO_InitStructrue);
		GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);//����ʱ������
		
		GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_Out_PP;//����������������������NSS�����԰�PA4��Ϊ��ͨ��������������ƴӻ�CS��
		GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_4; //������Ƭѡ���� NSS���ӻ���Ƭѡ����ΪCS��CS����ѡ������GPIO�����������Ҳ����ѡ��NSS�ſ��ƣ�
		GPIO_Init(GPIOA, &GPIO_InitStructrue);
		GPIO_SetBits(GPIOA, GPIO_Pin_4);//Ƭѡ����������ʹ��
	}
	if(SPIx == SPI2)
	{
		GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;   
		GPIO_Init(GPIOB, &GPIO_InitStructrue);
		GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		
		GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_12; 
		GPIO_Init(GPIOB, &GPIO_InitStructrue);
		GPIO_SetBits(GPIOB, GPIO_Pin_12);//�Ӵӻ�CS(���NSS�¿���ѡ),Ƭѡ��������
	}
	if(SPIx == SPI3)
	{
		GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; 
		GPIO_Init(GPIOB, &GPIO_InitStructrue);
		GPIO_SetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
		
		GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_15;
		GPIO_Init(GPIOA, &GPIO_InitStructrue);
		GPIO_SetBits(GPIOA, GPIO_Pin_15);//�Ӵӻ�CS(���NSS�¿���ѡ),Ƭѡ��������
	}
	
	SPI_InitStructrue.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//������Ԥ��Ƶ��ֵ��������spiʱ�����Ϸ�Ƶ2��Ϊʱ��
	SPI_InitStructrue.SPI_CPHA = SPI_CPHA_2Edge;//��λΪ�ڶ���ʱ�������ؿ�ʼ����
	SPI_InitStructrue.SPI_CPOL = SPI_CPOL_High; //����Ϊ�ߣ�����״̬�µ�ʱ�ӵ�ƽΪ�ߣ�
	SPI_InitStructrue.SPI_CRCPolynomial = 7;						   //CRCУ��ֵ����ʽ
	SPI_InitStructrue.SPI_DataSize = SPI_DataSize_8b;//����֡ 8λ
	SPI_InitStructrue.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//����ȫ˫��
	SPI_InitStructrue.SPI_FirstBit = SPI_FirstBit_MSB; //���ݴ����ȴӸ�λ��ʼ
	SPI_InitStructrue.SPI_Mode = SPI_Mode_Master;//��ģʽ
	SPI_InitStructrue.SPI_NSS = SPI_NSS_Soft;//NSS�ź��������ʹ��SSIλ������
	//(ѡ�����NSSʱ����ʱNSS���õ����ſ�������������;(��ǰ�ǽ�NSS���������ƴ��豸��ƬѡCS��)����������ͨ���ڲ���SSIλ�Զ����Լ�ʹ�ܡ���ѡ����Ӳ��NSS������Ҫ��NSS�ŽӸߵ�ƽ�������ſ���)
	
	SPI_Init(SPIx, &SPI_InitStructrue);
	SPI_Cmd(SPIx, ENABLE);

}


//���Ͳ�����һ���ֽڣ�����spiͨ��Э�飬ֻ���ȷ��ͺ���ܽ��գ�
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx , u8 TxData)
{
		u8 TryCount=0;
	
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)//�������ǿյ�ʱ���д���(TXEΪ��������empty)
		{
			TryCount++; 
			if(TryCount > 200)
			{
				return 0;//��ֹ��ѭ��
			}
		}
		SPI_I2S_SendData(SPIx ,TxData);//����һ�ֽ�����
		
		TryCount=0;
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)//�������յ�ʱ���д���(RXNEΪ�������ǿ�no empty)
		{
			TryCount++; 
			if(TryCount > 200)
			{
				return 0;//��ֹ��ѭ��
			}
		}
		
	return SPI_I2S_ReceiveData(SPIx);//����һ�ֽ�����

}


//����spi������Ԥ��Ƶ��ֵ
void Set_SPI_BaudRate(SPI_TypeDef* SPIx , u16 SPI_BaudRatePrescaler_x)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_InitStruct->SPI_BaudRatePrescaler));
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET); //��ͨ�����ڽ��е�ʱ�򣬲����޸ġ�

	SPIx->CR1 &= 0xffc7;//λ5:3 Ϊ����������λ
	SPIx->CR1 |= SPI_BaudRatePrescaler_x;
	
	SPI_Cmd(SPIx, ENABLE);

}













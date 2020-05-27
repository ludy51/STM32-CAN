#include "spi.h"
#include "delay.h"

void SPIInit(SPI_TypeDef* SPIx)
{
	GPIO_InitTypeDef GPIO_InitStructrue;
	SPI_InitTypeDef SPI_InitStructrue;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出	
	GPIO_InitStructrue.GPIO_Speed = GPIO_Speed_50MHz;
	if(SPIx == SPI1)
	{
		GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;  //  MISO脚一般设置为浮空输入，但由于该脚不产生输出，所以设置复用推挽输出不影响（和设置LED灯一样）
		                              //SCK          MISO        MOSI
		GPIO_Init(GPIOA, &GPIO_InitStructrue);
		GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);//空闲时都上拉
		
		GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出（！！由于是软件NSS，所以把PA4作为普通推挽输出口来控制从机CS）
		GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_4; //主机的片选引脚 NSS（从机的片选引脚为CS，CS可以选择任意GPIO，软件管理下也可以选择NSS脚控制）
		GPIO_Init(GPIOA, &GPIO_InitStructrue);
		GPIO_SetBits(GPIOA, GPIO_Pin_4);//片选引脚上拉不使能
	}
	if(SPIx == SPI2)
	{
		GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;   
		GPIO_Init(GPIOB, &GPIO_InitStructrue);
		GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		
		GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_12; 
		GPIO_Init(GPIOB, &GPIO_InitStructrue);
		GPIO_SetBits(GPIOB, GPIO_Pin_12);//接从机CS(软件NSS下可任选),片选引脚上拉
	}
	if(SPIx == SPI3)
	{
		GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; 
		GPIO_Init(GPIOB, &GPIO_InitStructrue);
		GPIO_SetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
		
		GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_15;
		GPIO_Init(GPIOA, &GPIO_InitStructrue);
		GPIO_SetBits(GPIOA, GPIO_Pin_15);//接从机CS(软件NSS下可任选),片选引脚上拉
	}
	
	SPI_InitStructrue.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//波特率预分频的值，将来自spi时钟线上分频2作为时钟
	SPI_InitStructrue.SPI_CPHA = SPI_CPHA_2Edge;//相位为第二个时钟跳变沿开始工作
	SPI_InitStructrue.SPI_CPOL = SPI_CPOL_High; //极性为高（空闲状态下的时钟电平为高）
	SPI_InitStructrue.SPI_CRCPolynomial = 7;						   //CRC校验值多项式
	SPI_InitStructrue.SPI_DataSize = SPI_DataSize_8b;//数据帧 8位
	SPI_InitStructrue.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//两线全双工
	SPI_InitStructrue.SPI_FirstBit = SPI_FirstBit_MSB; //数据传输先从高位开始
	SPI_InitStructrue.SPI_Mode = SPI_Mode_Master;//主模式
	SPI_InitStructrue.SPI_NSS = SPI_NSS_Soft;//NSS信号由软件（使用SSI位）管理
	//(选择软件NSS时，此时NSS复用的引脚可以用作其他用途(当前是将NSS脚用作控制从设备的片选CS脚)，主机可以通过内部的SSI位自动将自己使能。若选择了硬件NSS，则需要将NSS脚接高电平，主机才可用)
	
	SPI_Init(SPIx, &SPI_InitStructrue);
	SPI_Cmd(SPIx, ENABLE);

}


//发送并接收一个字节（根据spi通信协议，只有先发送后才能接收）
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx , u8 TxData)
{
		u8 TryCount=0;
	
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)//发送区非空的时候有错误(TXE为发送区空empty)
		{
			TryCount++; 
			if(TryCount > 200)
			{
				return 0;//防止死循环
			}
		}
		SPI_I2S_SendData(SPIx ,TxData);//发送一字节数据
		
		TryCount=0;
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)//发送区空的时候有错误(RXNE为接收区非空no empty)
		{
			TryCount++; 
			if(TryCount > 200)
			{
				return 0;//防止死循环
			}
		}
		
	return SPI_I2S_ReceiveData(SPIx);//接收一字节数据

}


//更改spi波特率预分频的值
void Set_SPI_BaudRate(SPI_TypeDef* SPIx , u16 SPI_BaudRatePrescaler_x)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_InitStruct->SPI_BaudRatePrescaler));
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET); //当通信正在进行的时候，不能修改。

	SPIx->CR1 &= 0xffc7;//位5:3 为波特率设置位
	SPIx->CR1 |= SPI_BaudRatePrescaler_x;
	
	SPI_Cmd(SPIx, ENABLE);

}













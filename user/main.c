//#include "sys.h"
//#include "delay.h"
//#include "usart.h"
#include "stm32f10x.h"
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <FreeRTOSConfig.h>
#include <list.h>
#include <portable.h>
#include "includes.h"
#include <stdio.h>
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include  <math.h> 
#include "GPS.h"
#include "sim900a.h" 
#include "dma.h"
//#define uint unsigned int

/*******************FreeRTOS����*****************************/
#if (0)
void task1(void*taskpara);
void task2(void*taskpara);
#endif
//int a = 1;
//int* para1 = &a;
//int b = 0;
//int* para2 = &b;
/************************************************/


/*************	�궨��	**************/


/*************	���س�������	**************/

const char *string = "AT+CIPSTART=\"TCP\",\"14z95r6389.iask.in\",20829";
const char *Timeserver = "AT+CIPSTART=\"TCP\",\"time.nist.gov\",13";


/*************  �ⲿ��������	**************/
char Uart1_Buf[Buf1_Max];
u8 Times=0,First_Int = 0,shijian=0;
u16 Heartbeat=0;
vu8 Timer0_start;	//��ʱ��0��ʱ����������
vu8 Heart_beat;		//��������֡��־λ
char TimeRTC[50] ;
TIME NowTime;
GPS_INFO GPS;

/************* �����ͱ�������*****************/
int adcValue = 20;//����


//#define bufferSize  6
char  usart_BUFF[bufferSize]= "";//һ�ֽ�

void CLR_Buf2(void);	
char Uart2_Buf[20] = "dfSTARTdfee";
int main(void)
{	


	uint8_t  Data[8]="AJU8iK9a";
//	char* Data = Data1;
	
	CanRxMsg RecieveMess; //ע�⣡���ܶ���Ϊָ���η���Ῠ����CAN���պ�����
	char  Recievedata1[8]={0};
	char* Recievedata = Recievedata1;//
	
	ALL_init();
	
	printf("������CAN�Բ��ԣ�����ģʽ��\r\n");

	
	while(1)
	{

		if(CAN_TX_data(Data , sizeof(Data)/sizeof(uint8_t)))//ע�ⳤ�Ȼ�ȡ�������β���ȥ��ȡ���������
		{
			printf("���ͳɹ�\r\n");
			
			if(CAN_RX_data(RecieveMess , (u8*)Recievedata))
			{
				printf("���յ�����:%s\r\n",Recievedata);
			}
			else
			{
				printf("���ղ���\r\n");
			}
		
		}
		else
		{
			printf("����ʧ��\r\n");
		}
		delay_ms(100);
		
	}
	
	 
}


/******
����Ч���������
1����������������ʽ��char CAN_RX_data(CanRxMsg* RecieveData , uint8_t *RXdata);��������·�Ӧ��
   i:��� CanRxMsg RecieveMess; ����CAN_RX_data�����⣬���������������ӡ��һ�ַ���"���յ�����:AED9i8ua"(�����յ�һ��������)
      
   ii:��� CanRxMsg RecieveMess; ���ں����ڣ���ʾ������"���յ�����:AED9i8ua"
2: ��������������ʽ��char CAN_RX_data(CanRxMsg RecieveData , uint8_t *RXdata);,����RecieveDataΪ��ָ�롣
   ����CanRxMsg RecieveMess; �ں����ڻ��Ǻ����ⲻӰ�졣

  ����ԭ�򣺺��βε�ָ���йأ��β�Ӧ��Ϊ��ָ����ʽ����������ϸԭ��δ�⡣

֪ʶ�㣺
	   ���Ҫ��ȡһ����ĳ��ȣ�Ԫ�ظ�����������sizeof(������)/sizeof(��������)��
	   ���ǣ��˷����κ������ͨ�ã�����������Ϊһ������ʵ�Σ���ָ����ʽ������ʱ����ͨ��sizeof(�β���)/sizeof(��������)
	   ���޷������ȷ���ȣ����Խ���ǣ���ȡ������ʵ��һ�롣
			���Ҫ��ȡ���鳤�ȣ�����ڶ�������ĵ�ǰ����ȥ��ȡ����ͨ���βδ������ٻ�ȡ�������д���CAN_TX_data����Ϊһ���ӣ�

*******/
	/***
֪ʶ�ѵ㣺
	1����������ʼ�������б�׼����ID�ŵ����÷�����
	
	����ǽ��յ������Ǳ�׼֡��ʽ��
	
	��׼֡IDռ��λ��Ϊ11λ�� �ڷ��ͺ��������õı�׼֡ID��StdId��ֻ��Ϊ��11λ��ֵ���ɣ������5λ�����⡣
	
	��������ID������յı�׼֡ID���������ʽ����32λ��11λ������룩����˹�������ID�ŵĸ�11λ�й��˵�Ч��������λ��������ֵ��
	
	�磺���ͺ������õı�־֡ID��StdId=0xFA8B;���׼֡ID= 010 1000 1011��ȡ��͵�11λ��
  ���������ģʽ������λ���ڼ�������£���ô������ID�Ÿ�11λ�ͱ�׼֡IDӦ��һ����
	����ȡ:CAN_FilterIdHigh=0x516F=010 1000 1011 01111 ( CAN_FilterIdLow ���⣩
	
	ͬ������ǽ��յ���������չ֡��ʽ��
	��׼֡IDռ��λ��Ϊ29λ��ֻ���ExtId�ĵ�29λ��ֵ���ɡ�
	����ʱ�͹���ID��Ҳ������룬��˹�������ID�ŵĸ�29λ�й��˵�Ч��������λ��������ֵ��
	
	
�Ľ������Թ�������ɸѡ�������÷����ĸĽ�)
>   �������ϸ�֡ID�͹�����ID��ֵ��ʽ��ͳһ��Ҳ�����׼��㡣Ϊ��ͳһ������۲�.
> 	��ȡ��׼/��չ֡ID�͹�����ID�ĸ�ֵ�������¸Ľ���������ģʽ������λ��Ҫ���ĵ������
> 	
> 	
> 
> 1. ��������׼֡�������������ñ�׼֡ID�궨��Ϊ��0xABC��ȡǰ11λ�����һλ����ȡ0������Ϊ��׼IDλ�������ǣ�����д��StdIdʱ�������ƶ�һλ��ȡ����11λ��Ϊ��Чλ��
> 	StdId = 0xABC>>1; ��ȡ��11λ)	 	
> 	
>     CAN_FilterIdHigh= 0xABC<<4;  //11����Чλ�ƶ�������ˣ�ʹ32λ����ID��11λ��׼֡������룩
> 	
> 	
> 
> 2. ��������չ֡����������������չ֡ID�궨��Ϊ��0xABCDEF98��ȡǰ29λ�����3λ����ȡ0������Ϊ��׼IDλ�������ǣ�����д��ExtIdΪʱ�������ƶ�3λ��ȡ����29λ��Ϊ��Чλ��
> 	ExtId = 0xABCDEF98>>3;��ȡ��29λ��
> 	CAN_FilterIdHigh = 0xABCDEF98>>16;   
> 	CAN_FilterIdLow = 0xABCDEF98&0x0000FFF8 ;//29����Чλ�Ƶ����λ��ʹ32λ����ID��29λ��׼֡������룩
> 	
> 
> 3. ��Ҫע�⣺
> 	�ڱ�׼֡�£�����32λ�Ĺ���������������IDֻ�ܹ��ĸ�11λ����25λ���ܹ��ġ�������ID��0�����Ĵ�λ��1����Ĵ�λ��
> 	����չ֡�£���������IDֻ�ܹ��ĸ�29λ����3λ���ܹ��ġ�



	***/



void CLR_Buf2(void)
{
	u16 k;

		for(k=0;k<20;k++)      
		{
			Uart2_Buf[k] = 'a';
		}

}


void DMA1_Channel1_IRQHandler()
{

	if(DMA_GetITStatus(DMA1_IT_TC1) == SET)//��ȡDMA1_CH1�Ĵ�����ɱ�־λ
	{
		DMA_ClearITPendingBit(DMA1_IT_TC1);//�������־��ע������󣬲���ȥ�ȴ�������
		sprintf(usart_BUFF,"%d ",adcValue);//16λ����ת��Ϊ�ַ����ͣ�ÿ���ַ�8λ��1�ֽڣ�
			
		while(!DMA_GetFlagStatus( DMA1_FLAG_TC7));//�ȴ�������ɣ��ӵ�ʹ��ǰ��ע�� �ȴ�����ǰ�����������־��
		DMA_ClearFlag(DMA1_FLAG_TC7); //��ɺ����
		
		DMAx_Enable(DMA1_Channel7 , bufferSize);//��ʼ���ڵ�DMA����

	}
	
}




void task1(void*taskpara)
{
	int* para = taskpara;
	
	for( ; ; )
	{
		//ADValue = Get_ADCValue(ADC1 ,ADC_Channel_4);
		//ADValue = Get_ADCAverage(ADC1 , ADC_Channel_4 , 10);//PA4����AD�ɼ�

	}
}


void task2(void*taskpara)
{
		int* para1 = taskpara;
	
	for( ; ; )
	{
			//sprintf(StrAD,"%d",ADValue);//����ת�ַ���
		//USART_Puts(USART1 , StrAD);
		//printf("R:%d",ADValue);//USART1(ʹ��printf������Ҫ�����ض���fputc��������uart�ж��壩)
    vTaskDelay(1000/portTICK_RATE_MS);//��ʱ1S������1��������̬��1S���ٽ������
	}

}




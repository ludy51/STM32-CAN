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

/*******************FreeRTOS部分*****************************/
#if (0)
void task1(void*taskpara);
void task2(void*taskpara);
#endif
//int a = 1;
//int* para1 = &a;
//int b = 0;
//int* para2 = &b;
/************************************************/


/*************	宏定义	**************/


/*************	本地常量声明	**************/

const char *string = "AT+CIPSTART=\"TCP\",\"14z95r6389.iask.in\",20829";
const char *Timeserver = "AT+CIPSTART=\"TCP\",\"time.nist.gov\",13";


/*************  外部变量声明	**************/
char Uart1_Buf[Buf1_Max];
u8 Times=0,First_Int = 0,shijian=0;
u16 Heartbeat=0;
vu8 Timer0_start;	//定时器0延时启动计数器
vu8 Heart_beat;		//发送心跳帧标志位
char TimeRTC[50] ;
TIME NowTime;
GPS_INFO GPS;

/************* 函数和变量声明*****************/
int adcValue = 20;//半字


//#define bufferSize  6
char  usart_BUFF[bufferSize]= "";//一字节

void CLR_Buf2(void);	
char Uart2_Buf[20] = "dfSTARTdfee";
int main(void)
{	


	uint8_t  Data[8]="AJU8iK9a";
//	char* Data = Data1;
	
	CanRxMsg RecieveMess; //注意！不能定义为指针形否则会卡死在CAN接收函数！
	char  Recievedata1[8]={0};
	char* Recievedata = Recievedata1;//
	
	ALL_init();
	
	printf("下面是CAN自测试（环回模式）\r\n");

	
	while(1)
	{

		if(CAN_TX_data(Data , sizeof(Data)/sizeof(uint8_t)))//注意长度获取不能在形参内去获取，否则出错
		{
			printf("发送成功\r\n");
			
			if(CAN_RX_data(RecieveMess , (u8*)Recievedata))
			{
				printf("接收到数据:%s\r\n",Recievedata);
			}
			else
			{
				printf("接收不到\r\n");
			}
		
		}
		else
		{
			printf("发送失败\r\n");
		}
		delay_ms(100);
		
	}
	
	 
}


/******
测试效果（结果）
1：如果函数定义成形式：char CAN_RX_data(CanRxMsg* RecieveData , uint8_t *RXdata);会出现如下反应：
   i:如果 CanRxMsg RecieveMess; 放在CAN_RX_data函数外，即主函数里，将会多打印出一字符："接收到数据:AED9i8ua"(会多接收到一个的乱码)
      
   ii:如果 CanRxMsg RecieveMess; 放在函数内，显示正常："接收到数据:AED9i8ua"
2: 如果函数定义成形式：char CAN_RX_data(CanRxMsg RecieveData , uint8_t *RXdata);,即：RecieveData为非指针。
   无论CanRxMsg RecieveMess; 在函数内还是函数外不影响。

  分析原因：和形参的指针有关（形参应该为非指针形式）。具体详细原因未解。

知识点：
	   如果要获取一数组的长度（元素个数），可用sizeof(数组名)/sizeof(数组类型)。
	   但是！此法非任何情况下通用，当该数组作为一函数的实参，由指针形式传进来时，再通过sizeof(形参名)/sizeof(数组类型)
	   将无法获得正确长度，测试结果是：获取到的是实际一半。
			因此要获取数组长度，最好在定义数组的当前函数去获取，不通过形参传进后再获取，否则有错！（CAN_TX_data函数为一例子）

*******/
	/***
知识难点：
	1）过滤器初始化过程中标准过滤ID号的配置方法：
	
	如果是接收的数据是标准帧格式：
	
	标准帧ID占用位数为11位， 在发送函数中设置的标准帧ID（StdId）只需为低11位赋值即可，另外高5位可任意。
	
	过滤器的ID号与接收的标准帧ID是左对齐形式（即32位与11位的左对齐），因此过滤器的ID号的高11位有过滤的效果，其他位可设任意值。
	
	如：发送函数配置的标志帧ID：StdId=0xFA8B;则标准帧ID= 010 1000 1011（取最低的11位）
  如果在掩码模式的所有位都在检测的情况下，那么过滤器ID号高11位和标准帧ID应该一样，
	可以取:CAN_FilterIdHigh=0x516F=010 1000 1011 01111 ( CAN_FilterIdLow 任意）
	
	同理，如果是接收的数据是扩展帧格式：
	标准帧ID占用位数为29位，只需对ExtId的低29位赋值即可。
	过滤时和过滤ID好也是左对齐，因此过滤器的ID号的高29位有过滤的效果，其他位可设任意值。
	
	
改进：（对过滤器（筛选器）配置方法的改进)
>   由于以上给帧ID和过滤器ID赋值格式不统一，也不容易计算。为了统一并方便观察.
> 	对取标准/扩展帧ID和过滤器ID的赋值进行如下改进。（掩码模式对所有位都要关心的情况）
> 	
> 	
> 
> 1. 举例（标准帧），如程序可设置标准帧ID宏定义为：0xABC（取前11位，最后一位必须取0，不作为标准ID位）。但是，将其写入StdId时，需右移动一位，取出高11位作为有效位：
> 	StdId = 0xABC>>1; （取出11位)	 	
> 	
>     CAN_FilterIdHigh= 0xABC<<4;  //11个有效位移动到最左端（使32位过滤ID与11位标准帧的左对齐）
> 	
> 	
> 
> 2. 举例（扩展帧）：如程序可设置扩展帧ID宏定义为：0xABCDEF98（取前29位，最后3位必须取0，不作为标准ID位），但是，将其写入ExtId为时，需右移动3位，取出高29位作为有效位：
> 	ExtId = 0xABCDEF98>>3;（取出29位）
> 	CAN_FilterIdHigh = 0xABCDEF98>>16;   
> 	CAN_FilterIdLow = 0xABCDEF98&0x0000FFF8 ;//29个有效位移到最高位（使32位过滤ID与29位标准帧的左对齐）
> 	
> 
> 3. 需要注意：
> 	在标准帧下，对于32位的过滤器，设置掩码ID只能关心高11位，后25位不能关心。（掩码ID：0表不关心此位，1表关心此位）
> 	在扩展帧下，设置掩码ID只能关心高29位，后3位不能关心。



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

	if(DMA_GetITStatus(DMA1_IT_TC1) == SET)//获取DMA1_CH1的传输完成标志位
	{
		DMA_ClearITPendingBit(DMA1_IT_TC1);//清除满标志（注意清除后，不能去等待判满）
		sprintf(usart_BUFF,"%d ",adcValue);//16位整型转化为字符串型（每个字符8位，1字节）
			
		while(!DMA_GetFlagStatus( DMA1_FLAG_TC7));//等待传输完成，加到使能前（注意 等待判满前，不能清除标志）
		DMA_ClearFlag(DMA1_FLAG_TC7); //完成后清除
		
		DMAx_Enable(DMA1_Channel7 , bufferSize);//开始串口的DMA传输

	}
	
}




void task1(void*taskpara)
{
	int* para = taskpara;
	
	for( ; ; )
	{
		//ADValue = Get_ADCValue(ADC1 ,ADC_Channel_4);
		//ADValue = Get_ADCAverage(ADC1 , ADC_Channel_4 , 10);//PA4进行AD采集

	}
}


void task2(void*taskpara)
{
		int* para1 = taskpara;
	
	for( ; ; )
	{
			//sprintf(StrAD,"%d",ADValue);//整型转字符串
		//USART_Puts(USART1 , StrAD);
		//printf("R:%d",ADValue);//USART1(使用printf函数需要，先重定义fputc函数（在uart中定义）)
    vTaskDelay(1000/portTICK_RATE_MS);//延时1S，任务1进入阻塞态，1S后再进入就绪
	}

}




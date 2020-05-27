#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "stm32f10x.h"
#include "delay.h"
//#include "includes.h"
//////////////////////////////////////////////////////////////////////////////////	 
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收



	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);

void USART_Puts(USART_TypeDef* USARTx , char * str); //发送字符串
void USARTInit(USART_TypeDef* USARTx , u32 BaudRate);//（USART模块号，波特率）
void UART1_SendLR(void);
void UART2_SendLR(void); //发送回车
char USART_Receives(USART_TypeDef* USARTx , char *str, u8 len);//等待接收指定长度数据
char Receive_length(void);//返回该次接收数据串的长度
void USART1_Init_Config(u32 bound);
void USART2_Init_Config(u32 bound);
#endif



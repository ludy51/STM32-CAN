#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "stm32f10x.h"
#include "delay.h"
//#include "includes.h"
//////////////////////////////////////////////////////////////////////////////////	 
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����



	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);

void USART_Puts(USART_TypeDef* USARTx , char * str); //�����ַ���
void USARTInit(USART_TypeDef* USARTx , u32 BaudRate);//��USARTģ��ţ������ʣ�
void UART1_SendLR(void);
void UART2_SendLR(void); //���ͻس�
char USART_Receives(USART_TypeDef* USARTx , char *str, u8 len);//�ȴ�����ָ����������
char Receive_length(void);//���ظôν������ݴ��ĳ���
void USART1_Init_Config(u32 bound);
void USART2_Init_Config(u32 bound);
#endif



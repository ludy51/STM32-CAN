#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 


//�ض���fputc����
//(!!!�ض���fputc������ʹ�õ�ʹ��printfʱ�ͻ�����Զ����fputc�������������ַ�������USART1)
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void uart_init(u32 bound)
	{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

//void USART1_IRQHandler(void)                	//����1�жϷ������
//{
//	u8 Res;
//#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//		{
//		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
//		
//		if((USART_RX_STA&0x8000)==0)//����δ���
//			{
//			if(USART_RX_STA&0x4000)//���յ���0x0d
//				{
//				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//				else USART_RX_STA|=0x8000;	//��������� 
//				}
//			else //��û�յ�0X0D
//				{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//					{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//					}		 
//				}
//			}   		 
//     } 
//#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntExit();  											 
//#endif
//} 
	


/*�����ַ������ݺ���*/
void USART_Puts(USART_TypeDef* USARTx , char * str)
{ 
  while(*str)
  {
		//while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);//ʹ�ø�λ�ȽϺ�ʱ��
		//while (!(USART1->SR & USART_FLAG_TXE));		
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET); 
		USART_SendData(USARTx ,*str++);//���͵�ǰ�ַ�

  } 
	//while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET); //���һ��

}

//���ͻ���
void UART1_SendLR()
{
	USART_Puts(USART1 ,"\r\n");
}

void UART2_SendLR()
{
	USART_Puts(USART2 ,"\r\n");
	
}

//�ȴ�����ָ����������
char USART_Receives(USART_TypeDef* USARTx , char *str, u8 len)
{
    u16 i=0;
    char flag=0;


	for(i = 0 ; i < len ; i++)
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) ==RESET);//�ȴ�����  
		
    if(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) ==SET)//���յ�
    {
	//USART_ClearFlag(USARTx,USART_FLAG_TXE);
		str[i]=USART_ReceiveData(USARTx);
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		flag=1;
     }
  	}
    return flag;
}


//���ظôν������ݴ��ĳ���
//ע��ÿ�ε����ݴ����Ҫ����10ms����Ч

u8 ucount = 0;//��ǰ�������ݵĸ�������Ͻ����ж�ʹ�ã�
char Receive_length(void)
{
	u8 rxlen=ucount;
//	u8 i=0;
	delay_ms(10);		//�ȴ�10ms,10MS��û�н��յ�һ������,��ôrxlen==ucount������Ϊ���ս�������ʱ�㷨ֻ�����ڶ����ݣ�
	if(rxlen==ucount && ucount!=0)//���յ�������,�ҽ��������
	{
	    //USART_Puts(USART1,RX1);		
		ucount=0;	//��¼�������ݳ��� 
	}
	return rxlen;  //���ؽ��ճ���
}



//�����벨���ʳ�ʼ��
void USARTInit(USART_TypeDef* USARTx , u32 BaudRate)
{
	GPIO_InitTypeDef  GPIO_InitTypeture;
	USART_InitTypeDef USART_InitStructure;
	
	GPIO_InitTypeture.GPIO_Speed = GPIO_Speed_50MHz;
	if(USARTx == USART1)
	{
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		
		GPIO_InitTypeture.GPIO_Mode = GPIO_Mode_AF_PP;//���츴�����
		GPIO_InitTypeture.GPIO_Pin = GPIO_Pin_9;//TX
		GPIO_Init(GPIOA,&GPIO_InitTypeture);
		
		GPIO_InitTypeture.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_InitTypeture.GPIO_Pin = GPIO_Pin_10;//RX
		GPIO_Init(GPIOA,&GPIO_InitTypeture);
	}
	if(USARTx == USART2)
	{
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
		GPIO_InitTypeture.GPIO_Mode = GPIO_Mode_AF_PP;//���츴�����
		GPIO_InitTypeture.GPIO_Pin = GPIO_Pin_2;//TX
		GPIO_Init(GPIOA,&GPIO_InitTypeture);
		
		GPIO_InitTypeture.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_InitTypeture.GPIO_Pin = GPIO_Pin_3;//RX
		GPIO_Init(GPIOA,&GPIO_InitTypeture);
	}
	if(USARTx == USART3)
	{
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		
		GPIO_InitTypeture.GPIO_Mode = GPIO_Mode_AF_PP;//���츴�����
		GPIO_InitTypeture.GPIO_Pin = GPIO_Pin_10;//TX
		GPIO_Init(GPIOB,&GPIO_InitTypeture);
		
		GPIO_InitTypeture.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_InitTypeture.GPIO_Pin = GPIO_Pin_11;//RX
		GPIO_Init(GPIOB,&GPIO_InitTypeture);
	}
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx| USART_Mode_Tx;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USARTx, &USART_InitStructure);
	USART_Cmd(USARTx , ENABLE);//
	
}

/*******************************************************************************
* ������  : USART1_Init_Config
* ����    : USART1��ʼ������
* ����    : bound��������(���ã�2400��4800��9600��19200��38400��115200��)
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void USART2_Init_Config(u32 bound)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/*ʹ��USART2����ʱ��*/  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	/*��λ����2*/
 	USART_DeInit(USART2);  
	
	/*USART2_GPIO��ʼ������*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			//USART2_TXD(PA.2)     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������������������Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_TXD(PA.9)  
   
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//USART2_RXD(PA.3)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_RXD(PA.10)


   /*USART2 ��ʼ������*/
	USART_InitStructure.USART_BaudRate = bound;										//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//����ģʽ����Ϊ�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure);										//��ʼ������2

   /*Usart1 NVIC����*/
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 
	  
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//ʹ�ܴ���2�����ж�

  USART_Cmd(USART2, ENABLE);                    			//ʹ�ܴ��� 
	USART_ClearFlag(USART2, USART_FLAG_TC);					//���������ɱ�־
}

/*******************************************************************************
* ������  : USART1_Init_Config
* ����    : USART1��ʼ������
* ����    : bound��������(���ã�2400��4800��9600��19200��38400��115200��)
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void USART1_Init_Config(u32 bound)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/*ʹ��USART1��GPIOA����ʱ��*/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	

	/*��λ����1*/
 	USART_DeInit(USART1);  
	
	/*USART1_GPIO��ʼ������*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//USART1_TXD(PA.9)     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������������������Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_TXD(PA.9)  
   
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				//USART1_RXD(PA.10)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_RXD(PA.10)


   /*USART1 ��ʼ������*/
	USART_InitStructure.USART_BaudRate = bound;										//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//����ģʽ����Ϊ�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure);										//��ʼ������1

   /*Usart1 NVIC����*/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 
	  
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//ʹ�ܴ���1�����ж�

    USART_Cmd(USART1, ENABLE);                    			//ʹ�ܴ��� 
	USART_ClearFlag(USART1, USART_FLAG_TC);					//���������ɱ�־
}


#endif	


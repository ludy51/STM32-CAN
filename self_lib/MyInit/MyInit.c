#include "MyInit.h"
#include "includes.h"
//extern Time StartTime;//�Ľṹ��ָ��洢RTCһ��ʼ���е�ʱ��ֵ��


void ALL_init()
{
	System_HSE_Init();
	RCCInit();	
	delay_init();	//��ʱ��ʼ��
	GPIOInit(GPIOA , GPIO_Pin_11 , GPIO_Mode_Out_PP , 1 ); //can_TX
	GPIOInit(GPIOA , GPIO_Pin_12 ,GPIO_Mode_IN_FLOATING ,1); //can_RX
	USARTInit(USART2 ,115200); //�������	
//	printf("123\r\n");
	
	//can1 ����ģʽ������������ͬʱ���ܸ��Լ��������ڲ��ԣ� 450Kbps������
	CANInit(CAN1 , CAN_Mode_LoopBack ,CAN_SJW_1tq , CAN_BS1_7tq , CAN_BS2_8tq ,5);

//	NVIC_Config();
	

}


//�ϵ�ʹ���ⲿ�����ʼ��
void System_HSE_Init()
{
	
	RCC_DeInit();//����RCC�Ĵ���
	
	RCC_HSEConfig(RCC_HSE_ON); //ʹ���ⲿ����
	while(!RCC_GetFlagStatus(RCC_FLAG_HSERDY)) 
	{
	}
	SystemInit();

}


void RCCInit()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_USART2 , ENABLE);

	
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADCʱ������(�м���©)
	
}



void Delay_Init()//������
{
	//SysTick_Config(SystemCoreClock / 1000);
    SysTick_Config(SystemCoreClock / 1000);
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//8��Ƶ��72000000/8

}



void GPIOInit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_x ,GPIOMode_TypeDef GPIO_Mode, char BitVal)
{
	GPIO_InitTypeDef  GPIOinstruct;
	
//	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));//�������Ƿ������ʡ����GPIO_Init�����жϣ�
//	assert_param(IS_GPIO_PIN(GPIO_Pin_x)); 
//	assert_param(IS_GPIO_Mode(GPIO_Mode));
	
	GPIOinstruct.GPIO_Mode = GPIO_Mode;
	GPIOinstruct.GPIO_Pin = GPIO_Pin_x;
	GPIOinstruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOx , &GPIOinstruct); 

	if(BitVal == 0)
	{
		GPIO_WriteBit(GPIOx, GPIO_Pin_x, Bit_RESET);//��ʼ��ƽ
	}
	if(BitVal == 1)
	{
		GPIO_WriteBit(GPIOx, GPIO_Pin_x, Bit_SET);//��ʼ��ƽ
	}
	
}


//�ⲿ�жϳ�ʼ�����ж���0--15
void EXITInit(u32 GPIO_PortSourceGPIOx ,u32 EXTI_Linex ,EXTITrigger_TypeDef EXTI_Trigger_x)
{
		EXTI_InitTypeDef EXTI_InitStructure;
	
	  u8 GPIO_PinSourcex = 20;
		assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line)); 
		
		if(EXTI_Linex>=1 && EXTI_Linex<=8)//�ߣ�0--3
		{
				GPIO_PinSourcex = EXTI_Linex>>1;	//	����2��GPIO_PinSourcex���0---3
		
		}
		if(EXTI_Linex>=0x10 && EXTI_Linex<=0x80)//�ߣ�4--7
		{
				GPIO_PinSourcex = ((EXTI_Linex/0x10)>>1)+4;	//���4--7
		
		}
		if(EXTI_Linex>=0x100 && EXTI_Linex<=0x800)//�ߣ�8--11
		{
				GPIO_PinSourcex = ((EXTI_Linex/0x100)>>1)+8;	//���8--11
		
		}
		if(EXTI_Linex>=0x1000 && EXTI_Linex<=0x8000)//�ߣ�12--15
		{
				GPIO_PinSourcex = ((EXTI_Linex/0x1000)>>1)+12;	//���12--15
		
		}
		if(GPIO_PinSourcex<=15)//ֻ��0--15�߲��ܳ�ʼ��
		{
	
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOx, GPIO_PinSourcex);//IO��ӳ�䵽�ж���
			//[�ʼǣ���EXTIn�Ķ���IO��ΪPXn��XȡֵA,B,C,D..  nΪ�ж��ߺ�]
			
			EXTI_InitStructure.EXTI_Line = EXTI_Linex;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_x;  //EXTI_Trigger_Falling;//�½��ش���
			
			EXTI_Init(&EXTI_InitStructure);
		}
	
}


//�ⲿ�жϳ�ʼ�����ж���0--15
void EXIT15Init()
{
		  EXTI_InitTypeDef EXTI_InitStructure;
	

			GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);//IO��ӳ�䵽�ж���
			//[�ʼǣ���EXTIn�Ķ���IO��ΪPXn��XȡֵA,B,C,D..  nΪ�ж��ߺ�]
			
			EXTI_InitStructure.EXTI_Line = EXTI_Line15;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //EXTI_Trigger_Falling;//�½��ش���
			
			EXTI_Init(&EXTI_InitStructure);
	
}

void EXIT13Init()
{
		  EXTI_InitTypeDef EXTI_InitStructure;
	

			GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);//IO��ӳ�䵽�ж���
			//[�ʼǣ���EXTIn�Ķ���IO��ΪPXn��XȡֵA,B,C,D..  nΪ�ж��ߺ�]
			
			EXTI_InitStructure.EXTI_Line = EXTI_Line13;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //EXTI_Trigger_Falling;//�½��ش���
			
			EXTI_Init(&EXTI_InitStructure);
	
}

void EXIT10Init()
{
		  EXTI_InitTypeDef EXTI_InitStructure;
	

			GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);//IO��ӳ�䵽�ж���
			//[�ʼǣ���EXTIn�Ķ���IO��ΪPXn��XȡֵA,B,C,D..  nΪ�ж��ߺ�]
			
			EXTI_InitStructure.EXTI_Line = EXTI_Line10;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //EXTI_Trigger_Falling;//�½��ش���
			
			EXTI_Init(&EXTI_InitStructure);
	
}

void EXIT8Init()
{
		  EXTI_InitTypeDef EXTI_InitStructure;

			GPIO_EXTILineConfig( GPIO_PortSourceGPIOA,GPIO_PinSource8);//IO��ӳ�䵽�ж���      �����û��ϵ   
			//[�ʼǣ���EXTIn�Ķ���IO��ΪPXn��XȡֵA,B,C,D..  nΪ�ж��ߺ�]
			
			EXTI_InitStructure.EXTI_Line = EXTI_Line8;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //EXTI_Trigger_Falling;//�½��ش���
			
			EXTI_Init(&EXTI_InitStructure);
	
//				GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);//IO��ӳ�䵽�ж���
//			//[�ʼǣ���EXTIn�Ķ���IO��ΪPXn��XȡֵA,B,C,D..  nΪ�ж��ߺ�]
//			
//			EXTI_InitStructure.EXTI_Line = EXTI_Line8;
//			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //EXTI_Trigger_Falling;//�½��ش���
//			
//			EXTI_Init(&EXTI_InitStructure);
	
}

//�����жϳ�ʼ�����ж���17
void RTC_Alarm_EXIT()
{
	
		EXTI_InitTypeDef EXTI_InitStructure;

		EXTI_InitStructure.EXTI_Line = EXTI_Line17;//�ж���17--�����ж�
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//���ӵ������ش������������
		
		EXTI_Init(&EXTI_InitStructure);

}




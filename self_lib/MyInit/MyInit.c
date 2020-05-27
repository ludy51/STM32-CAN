#include "MyInit.h"
#include "includes.h"
//extern Time StartTime;//改结构体指针存储RTC一开始进行的时间值。


void ALL_init()
{
	System_HSE_Init();
	RCCInit();	
	delay_init();	//延时初始化
	GPIOInit(GPIOA , GPIO_Pin_11 , GPIO_Mode_Out_PP , 1 ); //can_TX
	GPIOInit(GPIOA , GPIO_Pin_12 ,GPIO_Mode_IN_FLOATING ,1); //can_RX
	USARTInit(USART2 ,115200); //串口输出	
//	printf("123\r\n");
	
	//can1 环回模式（即发送数据同时还能给自己发，用于测试） 450Kbps波特率
	CANInit(CAN1 , CAN_Mode_LoopBack ,CAN_SJW_1tq , CAN_BS1_7tq , CAN_BS2_8tq ,5);

//	NVIC_Config();
	

}


//上电使用外部晶振初始化
void System_HSE_Init()
{
	
	RCC_DeInit();//重置RCC寄存器
	
	RCC_HSEConfig(RCC_HSE_ON); //使用外部晶振
	while(!RCC_GetFlagStatus(RCC_FLAG_HSERDY)) 
	{
	}
	SystemInit();

}


void RCCInit()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_USART2 , ENABLE);

	
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//配置ADC时钟周期(切记勿漏)
	
}



void Delay_Init()//暂无用
{
	//SysTick_Config(SystemCoreClock / 1000);
    SysTick_Config(SystemCoreClock / 1000);
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//8分频。72000000/8

}



void GPIOInit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_x ,GPIOMode_TypeDef GPIO_Mode, char BitVal)
{
	GPIO_InitTypeDef  GPIOinstruct;
	
//	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));//检测参数是否合理（可省，在GPIO_Init里有判断）
//	assert_param(IS_GPIO_PIN(GPIO_Pin_x)); 
//	assert_param(IS_GPIO_Mode(GPIO_Mode));
	
	GPIOinstruct.GPIO_Mode = GPIO_Mode;
	GPIOinstruct.GPIO_Pin = GPIO_Pin_x;
	GPIOinstruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOx , &GPIOinstruct); 

	if(BitVal == 0)
	{
		GPIO_WriteBit(GPIOx, GPIO_Pin_x, Bit_RESET);//初始电平
	}
	if(BitVal == 1)
	{
		GPIO_WriteBit(GPIOx, GPIO_Pin_x, Bit_SET);//初始电平
	}
	
}


//外部中断初始化，中断线0--15
void EXITInit(u32 GPIO_PortSourceGPIOx ,u32 EXTI_Linex ,EXTITrigger_TypeDef EXTI_Trigger_x)
{
		EXTI_InitTypeDef EXTI_InitStructure;
	
	  u8 GPIO_PinSourcex = 20;
		assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line)); 
		
		if(EXTI_Linex>=1 && EXTI_Linex<=8)//线：0--3
		{
				GPIO_PinSourcex = EXTI_Linex>>1;	//	除以2，GPIO_PinSourcex变成0---3
		
		}
		if(EXTI_Linex>=0x10 && EXTI_Linex<=0x80)//线：4--7
		{
				GPIO_PinSourcex = ((EXTI_Linex/0x10)>>1)+4;	//变成4--7
		
		}
		if(EXTI_Linex>=0x100 && EXTI_Linex<=0x800)//线：8--11
		{
				GPIO_PinSourcex = ((EXTI_Linex/0x100)>>1)+8;	//变成8--11
		
		}
		if(EXTI_Linex>=0x1000 && EXTI_Linex<=0x8000)//线：12--15
		{
				GPIO_PinSourcex = ((EXTI_Linex/0x1000)>>1)+12;	//变成12--15
		
		}
		if(GPIO_PinSourcex<=15)//只有0--15线才能初始化
		{
	
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOx, GPIO_PinSourcex);//IO口映射到中断线
			//[笔记：！EXTIn的对于IO口为PXn，X取值A,B,C,D..  n为中断线号]
			
			EXTI_InitStructure.EXTI_Line = EXTI_Linex;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_x;  //EXTI_Trigger_Falling;//下降沿触发
			
			EXTI_Init(&EXTI_InitStructure);
		}
	
}


//外部中断初始化，中断线0--15
void EXIT15Init()
{
		  EXTI_InitTypeDef EXTI_InitStructure;
	

			GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);//IO口映射到中断线
			//[笔记：！EXTIn的对于IO口为PXn，X取值A,B,C,D..  n为中断线号]
			
			EXTI_InitStructure.EXTI_Line = EXTI_Line15;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //EXTI_Trigger_Falling;//下降沿触发
			
			EXTI_Init(&EXTI_InitStructure);
	
}

void EXIT13Init()
{
		  EXTI_InitTypeDef EXTI_InitStructure;
	

			GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);//IO口映射到中断线
			//[笔记：！EXTIn的对于IO口为PXn，X取值A,B,C,D..  n为中断线号]
			
			EXTI_InitStructure.EXTI_Line = EXTI_Line13;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //EXTI_Trigger_Falling;//下降沿触发
			
			EXTI_Init(&EXTI_InitStructure);
	
}

void EXIT10Init()
{
		  EXTI_InitTypeDef EXTI_InitStructure;
	

			GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);//IO口映射到中断线
			//[笔记：！EXTIn的对于IO口为PXn，X取值A,B,C,D..  n为中断线号]
			
			EXTI_InitStructure.EXTI_Line = EXTI_Line10;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //EXTI_Trigger_Falling;//下降沿触发
			
			EXTI_Init(&EXTI_InitStructure);
	
}

void EXIT8Init()
{
		  EXTI_InitTypeDef EXTI_InitStructure;

			GPIO_EXTILineConfig( GPIO_PortSourceGPIOA,GPIO_PinSource8);//IO口映射到中断线      可以用或关系   
			//[笔记：！EXTIn的对于IO口为PXn，X取值A,B,C,D..  n为中断线号]
			
			EXTI_InitStructure.EXTI_Line = EXTI_Line8;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //EXTI_Trigger_Falling;//下降沿触发
			
			EXTI_Init(&EXTI_InitStructure);
	
//				GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);//IO口映射到中断线
//			//[笔记：！EXTIn的对于IO口为PXn，X取值A,B,C,D..  n为中断线号]
//			
//			EXTI_InitStructure.EXTI_Line = EXTI_Line8;
//			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //EXTI_Trigger_Falling;//下降沿触发
//			
//			EXTI_Init(&EXTI_InitStructure);
	
}

//闹钟中断初始化，中断线17
void RTC_Alarm_EXIT()
{
	
		EXTI_InitTypeDef EXTI_InitStructure;

		EXTI_InitStructure.EXTI_Line = EXTI_Line17;//中断线17--闹钟中断
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//闹钟的上升沿触发会产生唤醒
		
		EXTI_Init(&EXTI_InitStructure);

}




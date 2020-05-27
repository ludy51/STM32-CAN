#include "nvic.h"
#include "delay.h"


void NVIC_Config()
{
	NVICInit(NVIC_PriorityGroup_2, DMA1_Channel1_IRQn , 0, 1); 
	DMA_ClearFlag(DMA1_FLAG_TC1);	
	DMA_ClearITPendingBit(DMA1_IT_TC1);	

	DMA_ITConfig(DMA1_Channel1,  DMA_IT_TC, ENABLE);//全部传输完成中断（DMA_IT_HT与DMAx_IT_HTx的区别）
	
//////	//串口1中断配置(gsm) 高
//////	NVICInit(NVIC_PriorityGroup_2, USART1_IRQn , 0, 1); 	
//////	USART_Cmd(USART1 , ENABLE);
//////	USART_ClearFlag(USART1, USART_FLAG_TXE);	
//////	USART_ClearFlag(USART1, USART_FLAG_TC);		
//////	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//////	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//使能串口1接收中断 
//////	//串口2中断配置(GPS) 高
//////	NVICInit(NVIC_PriorityGroup_2 , USART2_IRQn , 0, 2);  
////////USART_Cmd(USART2 , ENABLE);
//////	USART_ClearFlag(USART2, USART_FLAG_TXE);
//////	USART_ClearFlag(USART2, USART_FLAG_RXNE);		//清除标志
//////	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//////	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//使能串口2接收中断 (接收区非空中断)
//////	//TIM2中断配置(gsm)
//////  NVICInit(NVIC_PriorityGroup_2 , TIM2_IRQn , 1 , 4);
//////	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
//////	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志 
//////	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
//////	//TIM4中断配置(超声波脉冲计数) 高于超声波外中断
//////	NVICInit(NVIC_PriorityGroup_2 , TIM4_IRQn , 0 , 4);//一般不会进入
//////	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
//////	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx更新中断标志 
//////	TIM_ClearFlag(TIM4,TIM_IT_Update);
//////	TIM_ITConfig(TIM4,TIM_IT_Update, ENABLE); //使能指定的TIM3中断,允许更新中断	   |TIM_IT_Trigger
//////	
//////	//TIM1中断配置(超声波) 一定要低于超声波外中断
//////	NVICInit(NVIC_PriorityGroup_2 , TIM1_UP_IRQn , 2 , 0);
//////	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
//////	TIM_ClearFlag(TIM1,TIM_IT_Update);
//////	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除TIMx更新中断标志 
//////	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
//////	
//////	
//////	 NVICInit(NVIC_PriorityGroup_2 , EXTI1_IRQn , 1,1);//超声波1
//////   EXTI_ClearITPendingBit(EXTI_Line1);//清中断标志位
//////	 
//////	 NVICInit(NVIC_PriorityGroup_2 , EXTI15_10_IRQn , 1,0);//line13 15 超声波23  11为罗盘  10锁  
//////   EXTI_ClearITPendingBit(EXTI_Line13);//清中断标志位
//////	 EXTI_ClearITPendingBit(EXTI_Line15);//清中断标志位
//////	 EXTI_ClearITPendingBit(EXTI_Line11);//清中断标志位
//////   EXTI_ClearITPendingBit(EXTI_Line10);//清中断标志位
//////		//EXTI_ClearITPendingBit(EXTI_Line8);//清中断标志位
//////		
//////		
//////	 NVICInit(NVIC_PriorityGroup_2 , EXTI4_IRQn , 0,0);//紧急
//////   EXTI_ClearITPendingBit(EXTI_Line4);//清中断标志位
//////	 	
//////		
//////	 NVICInit(NVIC_PriorityGroup_2 , EXTI9_5_IRQn , 0,3);//line8 遥控寻杖
//////   EXTI_ClearITPendingBit(EXTI_Line8);//清中断标志位
//////	 
//////	 NVICInit(NVIC_PriorityGroup_2 , EXTI0_IRQn , 1,5);//时间
//////   EXTI_ClearITPendingBit(EXTI_Line0);//清中断标志位   
	 
	 
}

void NVICInit(u32 NVIC_PriorityGroup_x , IRQn_Type irq , u8 PreemptionPriority , u8 SubPriority)	
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_x);//  NVIC_PriorityGroup_1 ->组别1：抢占1位，从占3位
	
	NVIC_InitStructure.NVIC_IRQChannel = irq;//EXTI0_IRQn;//!每个通道的中断通道对应向量号见stm32f10x.h中
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;//0 抢占级别在第0位，最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;//2 从占级别中第2位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);

}

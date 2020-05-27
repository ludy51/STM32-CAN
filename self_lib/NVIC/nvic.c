#include "nvic.h"
#include "delay.h"


void NVIC_Config()
{
	NVICInit(NVIC_PriorityGroup_2, DMA1_Channel1_IRQn , 0, 1); 
	DMA_ClearFlag(DMA1_FLAG_TC1);	
	DMA_ClearITPendingBit(DMA1_IT_TC1);	

	DMA_ITConfig(DMA1_Channel1,  DMA_IT_TC, ENABLE);//ȫ����������жϣ�DMA_IT_HT��DMAx_IT_HTx������
	
//////	//����1�ж�����(gsm) ��
//////	NVICInit(NVIC_PriorityGroup_2, USART1_IRQn , 0, 1); 	
//////	USART_Cmd(USART1 , ENABLE);
//////	USART_ClearFlag(USART1, USART_FLAG_TXE);	
//////	USART_ClearFlag(USART1, USART_FLAG_TC);		
//////	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//////	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//ʹ�ܴ���1�����ж� 
//////	//����2�ж�����(GPS) ��
//////	NVICInit(NVIC_PriorityGroup_2 , USART2_IRQn , 0, 2);  
////////USART_Cmd(USART2 , ENABLE);
//////	USART_ClearFlag(USART2, USART_FLAG_TXE);
//////	USART_ClearFlag(USART2, USART_FLAG_RXNE);		//�����־
//////	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//////	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//ʹ�ܴ���2�����ж� (�������ǿ��ж�)
//////	//TIM2�ж�����(gsm)
//////  NVICInit(NVIC_PriorityGroup_2 , TIM2_IRQn , 1 , 4);
//////	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
//////	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx�����жϱ�־ 
//////	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
//////	//TIM4�ж�����(�������������) ���ڳ��������ж�
//////	NVICInit(NVIC_PriorityGroup_2 , TIM4_IRQn , 0 , 4);//һ�㲻�����
//////	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
//////	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx�����жϱ�־ 
//////	TIM_ClearFlag(TIM4,TIM_IT_Update);
//////	TIM_ITConfig(TIM4,TIM_IT_Update, ENABLE); //ʹ��ָ����TIM3�ж�,��������ж�	   |TIM_IT_Trigger
//////	
//////	//TIM1�ж�����(������) һ��Ҫ���ڳ��������ж�
//////	NVICInit(NVIC_PriorityGroup_2 , TIM1_UP_IRQn , 2 , 0);
//////	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
//////	TIM_ClearFlag(TIM1,TIM_IT_Update);
//////	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //���TIMx�����жϱ�־ 
//////	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
//////	
//////	
//////	 NVICInit(NVIC_PriorityGroup_2 , EXTI1_IRQn , 1,1);//������1
//////   EXTI_ClearITPendingBit(EXTI_Line1);//���жϱ�־λ
//////	 
//////	 NVICInit(NVIC_PriorityGroup_2 , EXTI15_10_IRQn , 1,0);//line13 15 ������23  11Ϊ����  10��  
//////   EXTI_ClearITPendingBit(EXTI_Line13);//���жϱ�־λ
//////	 EXTI_ClearITPendingBit(EXTI_Line15);//���жϱ�־λ
//////	 EXTI_ClearITPendingBit(EXTI_Line11);//���жϱ�־λ
//////   EXTI_ClearITPendingBit(EXTI_Line10);//���жϱ�־λ
//////		//EXTI_ClearITPendingBit(EXTI_Line8);//���жϱ�־λ
//////		
//////		
//////	 NVICInit(NVIC_PriorityGroup_2 , EXTI4_IRQn , 0,0);//����
//////   EXTI_ClearITPendingBit(EXTI_Line4);//���жϱ�־λ
//////	 	
//////		
//////	 NVICInit(NVIC_PriorityGroup_2 , EXTI9_5_IRQn , 0,3);//line8 ң��Ѱ��
//////   EXTI_ClearITPendingBit(EXTI_Line8);//���жϱ�־λ
//////	 
//////	 NVICInit(NVIC_PriorityGroup_2 , EXTI0_IRQn , 1,5);//ʱ��
//////   EXTI_ClearITPendingBit(EXTI_Line0);//���жϱ�־λ   
	 
	 
}

void NVICInit(u32 NVIC_PriorityGroup_x , IRQn_Type irq , u8 PreemptionPriority , u8 SubPriority)	
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_x);//  NVIC_PriorityGroup_1 ->���1����ռ1λ����ռ3λ
	
	NVIC_InitStructure.NVIC_IRQChannel = irq;//EXTI0_IRQn;//!ÿ��ͨ�����ж�ͨ����Ӧ�����ż�stm32f10x.h��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;//0 ��ռ�����ڵ�0λ�����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;//2 ��ռ�����е�2λ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);

}

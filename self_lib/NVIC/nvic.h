#ifndef __nvic_H
#define __nvic_H

#include "stm32f10x.h"

void NVICInit(u32 NVIC_PriorityGroup_x , IRQn_Type irq , u8 PreemptionPriority , u8 SubPriority);	//�жϳ�ʼ�����ã����������ж���� �ж��� �� ��ռλ �� ��ռλ��
void NVIC_Config(void);


#endif


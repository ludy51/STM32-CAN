#ifndef __nvic_H
#define __nvic_H

#include "stm32f10x.h"

void NVICInit(u32 NVIC_PriorityGroup_x , IRQn_Type irq , u8 PreemptionPriority , u8 SubPriority);	//中断初始化配置，参数：（中断组别， 中断名 ， 抢占位 ， 从占位）
void NVIC_Config(void);


#endif


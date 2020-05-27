#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"


void TIMInit(TIM_TypeDef* TIMx, u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc ,u16 pulse1 ,u16 pulse2);
void TIM4_PWM_Init(u16 arr,u16 psc ,u16 pulse);
#endif

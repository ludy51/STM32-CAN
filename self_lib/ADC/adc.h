#ifndef __adc_H
#define __adc_H

#include "stm32f10x.h"
#include "delay.h"

void ADCInit(ADC_TypeDef* ADCx , u8 Channel_num);
u16  Get_ADCValue(ADC_TypeDef* ADCx ,u8 ADC_Channel_x ,u8 rank);//获取相应模块下通道的ADC值
u16 Get_ADCAverage(ADC_TypeDef* ADCx ,u8 ADC_Channel_x ,u8 rank ,u16 times);//获取相应模块下通道的ADC平均值值

#endif

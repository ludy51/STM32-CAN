#ifndef __MyInit_H
#define __MyInit_H
#include "stm32f10x.h"
extern int adcValue;//半字

#define bufferSize  6
extern char  usart_BUFF[bufferSize];//一字节

#define PWR_MG323  GPIO_Pin_7

#define	TRIG_PORT      GPIOB		//TRIG       
#define	ECHO_PORT      GPIOB		//ECHO 

#define	TRIG_PIN1       GPIO_Pin_0   //TRIG       
#define	ECHO_PIN1       GPIO_Pin_1	//ECHO 

#define	TRIG_PIN2       GPIO_Pin_12   //TRIG       
#define	ECHO_PIN2       GPIO_Pin_13	//ECHO

#define	TRIG_PIN3       GPIO_Pin_14   //TRIG       
#define	ECHO_PIN3      GPIO_Pin_15	//ECHO 
/////////////////////////////////
#define	KEY1_PORT      GPIOA	
#define	KEY2_PORT      GPIOB		
#define	KEY3_PORT     GPIOA	

#define	KEY1_PIN 				GPIO_Pin_11
#define	KEY2_PIN				GPIO_Pin_8
#define	KEY3_PIN				GPIO_Pin_4

void ALL_init(void);
void System_HSE_Init(void);//上电使用外部晶振初始化
void RCCInit(void);

void GPIOInit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_x ,GPIOMode_TypeDef GPIO_Mode,char BitVal);

void EXITInit(u32 GPIO_PortSourceGPIOx ,u32 EXTI_Linex ,EXTITrigger_TypeDef EXTI_Trigger_x);
void EXIT13Init(void);
void EXIT15Init(void);
void EXIT8Init(void);
void EXIT10Init(void);

void Delay_Init(void);
void RTC_Alarm_EXIT(void);

void UltrasonicWave_Configuration(void);

void Timerx_Init(u16 arr,u16 psc);

#endif






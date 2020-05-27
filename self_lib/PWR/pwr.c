#include "pwr.h"
#include "delay.h"
//#include "includes.h"

//进入待机模式（！！唤醒后程序复位）
void Enter_Standby()
{
	//（停止模式不能时钟以及外设的复位）
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE); //复位所有 IO 口，使其为悬浮输入
	USART_DeInit(USART1);
	ADC_DeInit(ADC1);
	
	Standby();
}

void Standby()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);//开电源管理时钟PWR_Regulator_LowPower
	
	PWR_WakeUpPinCmd(ENABLE);//使能唤醒引脚，默认PA0
	PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI|PWR_STOPEntry_WFE);	
}

//进入停止模式（！！唤醒后是从原地唤醒）
void Enter_Stop()//进入停止模式后唤醒是从原地唤醒
{
   //（停止模式不能时钟以及外设的复位）
	Standby();
}

void StopMode()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);//开电源管理时钟PWR_Regulator_LowPower	
	
	PWR_WakeUpPinCmd(ENABLE);//使能唤醒引脚，默认PA0
	PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI|PWR_STOPEntry_WFE);	
}





#include "pwr.h"
#include "delay.h"
//#include "includes.h"

//�������ģʽ���������Ѻ����λ��
void Enter_Standby()
{
	//��ֹͣģʽ����ʱ���Լ�����ĸ�λ��
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE); //��λ���� IO �ڣ�ʹ��Ϊ��������
	USART_DeInit(USART1);
	ADC_DeInit(ADC1);
	
	Standby();
}

void Standby()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);//����Դ����ʱ��PWR_Regulator_LowPower
	
	PWR_WakeUpPinCmd(ENABLE);//ʹ�ܻ������ţ�Ĭ��PA0
	PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI|PWR_STOPEntry_WFE);	
}

//����ֹͣģʽ���������Ѻ��Ǵ�ԭ�ػ��ѣ�
void Enter_Stop()//����ֹͣģʽ�����Ǵ�ԭ�ػ���
{
   //��ֹͣģʽ����ʱ���Լ�����ĸ�λ��
	Standby();
}

void StopMode()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);//����Դ����ʱ��PWR_Regulator_LowPower	
	
	PWR_WakeUpPinCmd(ENABLE);//ʹ�ܻ������ţ�Ĭ��PA0
	PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI|PWR_STOPEntry_WFE);	
}





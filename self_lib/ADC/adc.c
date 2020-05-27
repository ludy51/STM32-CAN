#include "adc.h"
//#include "includes.h"



void ADCInit(ADC_TypeDef* ADCx , u8 Channel_num)
{
	//=============��ADC1�����ò�У׼====================//
	ADC_InitTypeDef ADC_Inittrue;
	//assert_param(IS_ADC_ALL_PERIPH(ADCx));//�������Ƿ����
	
	ADC_DeInit(ADCx);//��λADC1
	
	ADC_Inittrue.ADC_ContinuousConvMode = ENABLE;//����ת����������ת�������Σ�����ת����ADC���һ��ת��֮���Զ���ʼ��һ��ת��������Ҫ�ٴ�����ADCͨ�����ã�
	ADC_Inittrue.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ��롣��*����ADCΪ12λ���ȣ��������źŴ���16λ�ļĴ����������ң�15-14...2-1-0��
	ADC_Inittrue.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//������ʽ�����ⲿ����=�������
	ADC_Inittrue.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_Inittrue.ADC_NbrOfChannel = Channel_num;//˳����й���ת���� ADC ͨ������Ŀ��1--16��
	ADC_Inittrue.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ���������ڵ�ͨ����
	
	ADC_Init(ADCx, &ADC_Inittrue);//��ʼ������ģ��ADC1	
	ADC_Cmd(ADCx, ENABLE);//ʹ��ADC1
	
	ADC_ResetCalibration(ADCx);  //��λУ׼
	while(ADC_GetResetCalibrationStatus(ADCx));
	
	ADC_StartCalibration(ADCx);//����adcУ׼
  while(ADC_GetCalibrationStatus(ADCx));
	
}



//��ȡ��Ӧģ����ͨ����ADCֵ
u16 Get_ADCValue(ADC_TypeDef* ADCx ,u8 ADC_Channel_x ,u8 rank)
{
	
	//assert_param(IS_ADC_ALL_PERIPH(ADCx));//�������Ƿ����
	//assert_param(IS_ADC_CHANNEL(ADC_Channel_x));
	
	//=============��ADC1�Ĺ���ͨ�������ã�ѡͨ��x��====================//
	ADC_RegularChannelConfig(ADCx,ADC_Channel_x, rank , ADC_SampleTime_28Cycles5);//ADC1��ͨ��4�����ڵ�1����ת�������Ҳɼ�ʱ��Ϊ239.5��ADC1ʱ������

	ADC_SoftwareStartConvCmd(ADCx ,ENABLE);//�������ת��
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������//�ȴ����ת������(����ʱ��1)
//		delay_ms(10);	
	return ADC_GetConversionValue(ADCx)&0x00000fff;//����ADC1ת�����ֵ
}

//��ȡ��Ӧģ����ͨ����ADCƽ��ֵֵ
u16 Get_ADCAverage(ADC_TypeDef* ADCx ,u8 ADC_Channel_x ,u8 rank ,u16 times)
{
	u16 count;
	u32 value = 0;
	assert_param(times>0);//�������Ƿ����
	for(count = 0; count<times ;count++)
	{
		value += Get_ADCValue(ADCx ,ADC_Channel_x , rank);
	}
	
	return value /=count;
}


#include "adc.h"
//#include "includes.h"



void ADCInit(ADC_TypeDef* ADCx , u8 Channel_num)
{
	//=============对ADC1的配置并校准====================//
	ADC_InitTypeDef ADC_Inittrue;
	//assert_param(IS_ADC_ALL_PERIPH(ADCx));//检测参数是否合理
	
	ADC_DeInit(ADCx);//复位ADC1
	
	ADC_Inittrue.ADC_ContinuousConvMode = ENABLE;//单次转换还是连续转换，单次（连续转换是ADC完成一次转换之后自动开始下一次转换，不需要再次启动ADC通道配置）
	ADC_Inittrue.ADC_DataAlign = ADC_DataAlign_Right;//右对齐。（*！！ADC为12位精度，其数字信号存于16位的寄存器，从左到右：15-14...2-1-0）
	ADC_Inittrue.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//触发方式，无外部触发=软件触发
	ADC_Inittrue.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_Inittrue.ADC_NbrOfChannel = Channel_num;//顺序进行规则转换的 ADC 通道的数目（1--16）
	ADC_Inittrue.ADC_ScanConvMode = DISABLE;//无扫描模式，即工作在单通道下
	
	ADC_Init(ADCx, &ADC_Inittrue);//初始化配置模块ADC1	
	ADC_Cmd(ADCx, ENABLE);//使能ADC1
	
	ADC_ResetCalibration(ADCx);  //复位校准
	while(ADC_GetResetCalibrationStatus(ADCx));
	
	ADC_StartCalibration(ADCx);//开启adc校准
  while(ADC_GetCalibrationStatus(ADCx));
	
}



//获取相应模块下通道的ADC值
u16 Get_ADCValue(ADC_TypeDef* ADCx ,u8 ADC_Channel_x ,u8 rank)
{
	
	//assert_param(IS_ADC_ALL_PERIPH(ADCx));//检测参数是否合理
	//assert_param(IS_ADC_CHANNEL(ADC_Channel_x));
	
	//=============对ADC1的规则通道的配置（选通道x）====================//
	ADC_RegularChannelConfig(ADCx,ADC_Channel_x, rank , ADC_SampleTime_28Cycles5);//ADC1的通道4，排在第1个被转换，并且采集时间为239.5个ADC1时钟周期

	ADC_SoftwareStartConvCmd(ADCx ,ENABLE);//开启软件转换
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束//等待软件转换结束(结束时置1)
//		delay_ms(10);	
	return ADC_GetConversionValue(ADCx)&0x00000fff;//换回ADC1转换后的值
}

//获取相应模块下通道的ADC平均值值
u16 Get_ADCAverage(ADC_TypeDef* ADCx ,u8 ADC_Channel_x ,u8 rank ,u16 times)
{
	u16 count;
	u32 value = 0;
	assert_param(times>0);//检测参数是否合理
	for(count = 0; count<times ;count++)
	{
		value += Get_ADCValue(ADCx ,ADC_Channel_x , rank);
	}
	
	return value /=count;
}


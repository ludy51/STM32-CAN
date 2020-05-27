#ifndef __rtc_H
#define __rtc_H

#include "stm32f10x.h"

typedef struct 
{
  u16 year;
	u8 month;
	u8 day;
	u8 hour;
	u8 min; 
	u8 sec;
	
} Time;

char RTCInit(u32 CounterValue);
u8 Is_Leap_Year(u16 year);//�ж��ǲ�������
u32 SetWriteTime(Time* time);//ʱ��ת��Ϊ�����������أ�ָ��Ϊ�βο����ں�������Ըı���ֵ��
void TimeValueInit(void);//��ʱ��ṹ���ֵ
u8 RTC_Get_Week(u16 year,u8 month,u8 day);//�������
Time ReadNowTime(void);//��ȡ��ǰʱ�䷵���ڽṹ��
void AlarmTime_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//��ʼ������	,Ҳ�ǽ�����ʱ��ֵת��Ϊ�������Ǹ�RTC

#endif


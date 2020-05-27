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
u8 Is_Leap_Year(u16 year);//判断是不是闰年
u32 SetWriteTime(Time* time);//时间转变为整型数并返回（指针为形参可以在函数里可以改变其值）
void TimeValueInit(void);//给时间结构体初值
u8 RTC_Get_Week(u16 year,u8 month,u8 day);//获得星期
Time ReadNowTime(void);//读取当前时间返回于结构体
void AlarmTime_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//初始化闹钟	,也是将闹钟时间值转变为整型数是给RTC

#endif


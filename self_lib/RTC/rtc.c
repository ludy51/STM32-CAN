#include "rtc.h"
#include "delay.h"

Time StartTime;//改结构体指针存储RTC一开始进行的时间值。


//初始化RTC，返回0：初始化失败，1：成功
char RTCInit(u32 CounterValue)
{

	char count = 0;
 		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR , ENABLE);
	 		
	PWR_BackupAccessCmd(ENABLE);//打开后备域电源，可以访问后备域寄存器
		

	//（由于RTC的配置数据在后备域BKP中，所以只要Vbat或者VDD不丢电就不丢失（即使复位了也不丢失），故可通过读BKP某个值来判断是否丢电，进而判断RTC是否需要初始化配置）
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xAA00)//Vbat且VDD掉电
	{

		BKP_DeInit();	//复位备份区域，将BKP寄存器清空
		
/////////////////////下面是对RTC初始化部分（时钟配置部分）////////////////////////////		
		//RCC_LSEConfig(RCC_LSE_ON);//开启外部低速晶振	LSE	
		RCC_LSICmd(ENABLE);//开启内部低速晶振LSI
		
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == 0 && count<11) 
		//while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == 0 && count<11)  //等待LSE启振，判断在2S内配置是否成功，不成功返回0
		{
			count++;
			delay_ms(200);
		}
	
		if(count == 10)
		{
			return 0;//LSI启振失败
		}
				
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		//RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//使用外部低速晶振LSE为RTC时钟源
				
		RCC_RTCCLKCmd(ENABLE);//选择使能RTC时钟

		RTC_WaitForLastTask();//等待上一次写RTC任务完成（常用，只要涉及对RTC里值的修改，都需要在修改后加上此函数）
		RTC_WaitForSynchro();//等待与APB1的同步，才能读RTC寄存器
	
		
///////////////// ////（寄存器配置部分）////////////////////////////
		
		RTC_EnterConfigMode();//允许RTC配置
		RTC_WaitForLastTask();//等待上一次写RTC任务完成

		RTC_SetPrescaler(40000);//分频后为1HZ
		//RTC_SetPrescaler(32767);//分频后为1HZ
		RTC_WaitForLastTask();//等待上一次写RTC任务完成
		
		RTC_SetCounter(CounterValue);
		RTC_WaitForLastTask();//等待上一次写RTC任务完成
		
		RTC_ExitConfigMode(); //退出RTC配置模式

		BKP_WriteBackupRegister(BKP_DR1, 0xAA00);//写入想要的丢电判断数据
					
	}
	
	else//已经初始化过一次且没掉电
	{
//		RTC_ClearITPendingBit(RTC_IT_ALR);
//		RTC_WaitForLastTask();
		RCC_LSICmd(ENABLE);//开启内部低速晶振LSI
		
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == 0 && count<11) 
		//while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == 0 && count<11)  //等待LSE启振，判断在2S内配置是否成功，不成功返回0
		{
			count++;
			delay_ms(200);
		}
	
		if(count == 10)
		{
			return 0;//LSI启振失败
		}
				
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		//RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//使用外部低速晶振LSE为RTC时钟源
				
		RCC_RTCCLKCmd(ENABLE);//选择使能RTC时钟

		RTC_WaitForLastTask();//等待上一次写RTC任务完成（常用，只要涉及对RTC里值的修改，都需要在修改后加上此函数）
		RTC_WaitForSynchro();//等待与APB1的同步，才能读RTC寄存器
		
	///////////////// ////（寄存器配置部分）////////////////////////////
		
		RTC_EnterConfigMode();//允许RTC配置
		
		//RTC_ITConfig(RTC_IT_ALR, DISABLE);	//使能闹钟中断.  秒中断RTC_IT_SEC
		//RTC_WaitForLastTask();//等待上一次写RTC任务完成
		
		RTC_ExitConfigMode(); //退出RTC配置模式
		RTC_WaitForLastTask();//等待上一次写RTC任务完成
		
//		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}

	return 1;//配置成功
}

void TimeValueInit()
{
	StartTime.year = 16;
	StartTime.month = 3;	
	StartTime.day = 5;
	StartTime.hour = 20;
	StartTime.min = 41;
	StartTime.sec = 0;
}

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	

	//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

//写入起始日历转变为整型数是给RTC（指针为形参可以在函数里可以改变其值）
u32 SetWriteTime(Time* time)
{
	
	u16 t;
	u32 seccount=0;
	
	if(time->year<1970||time->year>2099)
	{
		for(t=1970;t<time->year;t++)	//把所有年份的秒钟相加
		{
			if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
			else seccount+=31536000;			  //平年的秒钟数
		}
		time->month-=1;
		for(t=0;t<time->month;t++)	   //把前面月份的秒钟数相加
		{
			seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
			if(Is_Leap_Year(time->year)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
		}
		seccount+=(u32)(time->day-1)*86400;//把前面日期的秒钟数相加 
		seccount+=(u32)time->hour*3600;//小时秒钟数
			seccount+=(u32)time->min*60;	 //分钟秒钟数
		seccount+=time->sec;//最后的秒钟加上去
			
	}
	
	return seccount;
}



//从RTC读取当前时间
Time ReadNowTime()
{
		
	Time NowTime;
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
    timecount=RTC_GetCounter();	 
 	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else {temp1++;break;}  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		NowTime.year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(NowTime.year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		NowTime.month=temp1+1;	//得到月份
		NowTime.day=temp+1;  	//得到日期 
	}
	temp=timecount%86400;     		//得到秒钟数   	   
	NowTime.hour=temp/3600;     	//小时
	NowTime.min=(temp%3600)/60; 	//分钟	
	NowTime.sec=(temp%3600)%60; 	//秒钟
	//NowTime.week=RTC_Get_Week(NowTime.year,NowTime.month,NowTime.day);//获取星期   
	
	return NowTime;//返回当前时间的结构体
}


//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//输入参数：公历年月日 
//返回值：星期号																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}			  





//初始化闹钟		  
//以1970年1月1日为基准
//1970~2099年为合法年份
//闹钟的年月日时分秒   
void AlarmTime_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)
	{		
		for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
		{
			if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
			else seccount+=31536000;			  //平年的秒钟数
		}
		smon-=1;
		for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
		{
			seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
			if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
		}
		seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加 
		seccount+=(u32)hour*3600;//小时秒钟数
			seccount+=(u32)min*60;	 //分钟秒钟数
		seccount+=sec;//最后的秒钟加上去 			    
		//设置时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
		PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
		//上面三步是必须的!
		
		RTC_SetAlarm(seccount);
	 
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	}
		    
}



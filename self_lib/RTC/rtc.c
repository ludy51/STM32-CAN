#include "rtc.h"
#include "delay.h"

Time StartTime;//�Ľṹ��ָ��洢RTCһ��ʼ���е�ʱ��ֵ��


//��ʼ��RTC������0����ʼ��ʧ�ܣ�1���ɹ�
char RTCInit(u32 CounterValue)
{

	char count = 0;
 		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR , ENABLE);
	 		
	PWR_BackupAccessCmd(ENABLE);//�򿪺����Դ�����Է��ʺ���Ĵ���
		

	//������RTC�����������ں���BKP�У�����ֻҪVbat����VDD������Ͳ���ʧ����ʹ��λ��Ҳ����ʧ�����ʿ�ͨ����BKPĳ��ֵ���ж��Ƿ񶪵磬�����ж�RTC�Ƿ���Ҫ��ʼ�����ã�
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xAA00)//Vbat��VDD����
	{

		BKP_DeInit();	//��λ�������򣬽�BKP�Ĵ������
		
/////////////////////�����Ƕ�RTC��ʼ�����֣�ʱ�����ò��֣�////////////////////////////		
		//RCC_LSEConfig(RCC_LSE_ON);//�����ⲿ���پ���	LSE	
		RCC_LSICmd(ENABLE);//�����ڲ����پ���LSI
		
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == 0 && count<11) 
		//while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == 0 && count<11)  //�ȴ�LSE�����ж���2S�������Ƿ�ɹ������ɹ�����0
		{
			count++;
			delay_ms(200);
		}
	
		if(count == 10)
		{
			return 0;//LSI����ʧ��
		}
				
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		//RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//ʹ���ⲿ���پ���LSEΪRTCʱ��Դ
				
		RCC_RTCCLKCmd(ENABLE);//ѡ��ʹ��RTCʱ��

		RTC_WaitForLastTask();//�ȴ���һ��дRTC������ɣ����ã�ֻҪ�漰��RTC��ֵ���޸ģ�����Ҫ���޸ĺ���ϴ˺�����
		RTC_WaitForSynchro();//�ȴ���APB1��ͬ�������ܶ�RTC�Ĵ���
	
		
///////////////// ////���Ĵ������ò��֣�////////////////////////////
		
		RTC_EnterConfigMode();//����RTC����
		RTC_WaitForLastTask();//�ȴ���һ��дRTC�������

		RTC_SetPrescaler(40000);//��Ƶ��Ϊ1HZ
		//RTC_SetPrescaler(32767);//��Ƶ��Ϊ1HZ
		RTC_WaitForLastTask();//�ȴ���һ��дRTC�������
		
		RTC_SetCounter(CounterValue);
		RTC_WaitForLastTask();//�ȴ���һ��дRTC�������
		
		RTC_ExitConfigMode(); //�˳�RTC����ģʽ

		BKP_WriteBackupRegister(BKP_DR1, 0xAA00);//д����Ҫ�Ķ����ж�����
					
	}
	
	else//�Ѿ���ʼ����һ����û����
	{
//		RTC_ClearITPendingBit(RTC_IT_ALR);
//		RTC_WaitForLastTask();
		RCC_LSICmd(ENABLE);//�����ڲ����پ���LSI
		
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == 0 && count<11) 
		//while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == 0 && count<11)  //�ȴ�LSE�����ж���2S�������Ƿ�ɹ������ɹ�����0
		{
			count++;
			delay_ms(200);
		}
	
		if(count == 10)
		{
			return 0;//LSI����ʧ��
		}
				
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		//RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//ʹ���ⲿ���پ���LSEΪRTCʱ��Դ
				
		RCC_RTCCLKCmd(ENABLE);//ѡ��ʹ��RTCʱ��

		RTC_WaitForLastTask();//�ȴ���һ��дRTC������ɣ����ã�ֻҪ�漰��RTC��ֵ���޸ģ�����Ҫ���޸ĺ���ϴ˺�����
		RTC_WaitForSynchro();//�ȴ���APB1��ͬ�������ܶ�RTC�Ĵ���
		
	///////////////// ////���Ĵ������ò��֣�////////////////////////////
		
		RTC_EnterConfigMode();//����RTC����
		
		//RTC_ITConfig(RTC_IT_ALR, DISABLE);	//ʹ�������ж�.  ���ж�RTC_IT_SEC
		//RTC_WaitForLastTask();//�ȴ���һ��дRTC�������
		
		RTC_ExitConfigMode(); //�˳�RTC����ģʽ
		RTC_WaitForLastTask();//�ȴ���һ��дRTC�������
		
//		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}

	return 1;//���óɹ�
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

//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	

	//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

//д����ʼ����ת��Ϊ�������Ǹ�RTC��ָ��Ϊ�βο����ں�������Ըı���ֵ��
u32 SetWriteTime(Time* time)
{
	
	u16 t;
	u32 seccount=0;
	
	if(time->year<1970||time->year>2099)
	{
		for(t=1970;t<time->year;t++)	//��������ݵ��������
		{
			if(Is_Leap_Year(t))seccount+=31622400;//�����������
			else seccount+=31536000;			  //ƽ���������
		}
		time->month-=1;
		for(t=0;t<time->month;t++)	   //��ǰ���·ݵ����������
		{
			seccount+=(u32)mon_table[t]*86400;//�·����������
			if(Is_Leap_Year(time->year)&&t==1)seccount+=86400;//����2�·�����һ���������	   
		}
		seccount+=(u32)(time->day-1)*86400;//��ǰ�����ڵ���������� 
		seccount+=(u32)time->hour*3600;//Сʱ������
			seccount+=(u32)time->min*60;	 //����������
		seccount+=time->sec;//�������Ӽ���ȥ
			
	}
	
	return seccount;
}



//��RTC��ȡ��ǰʱ��
Time ReadNowTime()
{
		
	Time NowTime;
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
    timecount=RTC_GetCounter();	 
 	temp=timecount/86400;   //�õ�����(��������Ӧ��)
	if(daycnt!=temp)//����һ����
	{	  
		daycnt=temp;
		temp1=1970;	//��1970�꿪ʼ
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//������
			{
				if(temp>=366)temp-=366;//�����������
				else {temp1++;break;}  
			}
			else temp-=365;	  //ƽ�� 
			temp1++;  
		}   
		NowTime.year=temp1;//�õ����
		temp1=0;
		while(temp>=28)//������һ����
		{
			if(Is_Leap_Year(NowTime.year)&&temp1==1)//�����ǲ�������/2�·�
			{
				if(temp>=29)temp-=29;//�����������
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
				else break;
			}
			temp1++;  
		}
		NowTime.month=temp1+1;	//�õ��·�
		NowTime.day=temp+1;  	//�õ����� 
	}
	temp=timecount%86400;     		//�õ�������   	   
	NowTime.hour=temp/3600;     	//Сʱ
	NowTime.min=(temp%3600)/60; 	//����	
	NowTime.sec=(temp%3600)%60; 	//����
	//NowTime.week=RTC_Get_Week(NowTime.year,NowTime.month,NowTime.day);//��ȡ����   
	
	return NowTime;//���ص�ǰʱ��Ľṹ��
}


//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//������������������� 
//����ֵ�����ں�																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// ���Ϊ21����,�������100  
	if (yearH>19)yearL+=100;
	// ����������ֻ��1900��֮���  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}			  





//��ʼ������		  
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//���ӵ�������ʱ����   
void AlarmTime_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)
	{		
		for(t=1970;t<syear;t++)	//��������ݵ��������
		{
			if(Is_Leap_Year(t))seccount+=31622400;//�����������
			else seccount+=31536000;			  //ƽ���������
		}
		smon-=1;
		for(t=0;t<smon;t++)	   //��ǰ���·ݵ����������
		{
			seccount+=(u32)mon_table[t]*86400;//�·����������
			if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������	   
		}
		seccount+=(u32)(sday-1)*86400;//��ǰ�����ڵ���������� 
		seccount+=(u32)hour*3600;//Сʱ������
			seccount+=(u32)min*60;	 //����������
		seccount+=sec;//�������Ӽ���ȥ 			    
		//����ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
		PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
		//���������Ǳ����!
		
		RTC_SetAlarm(seccount);
	 
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	}
		    
}



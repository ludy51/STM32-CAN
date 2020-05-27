#ifndef __SIM900A_H
#define __SIM900A_H

#include "stm32f10x.h"
#include "usart.h"	
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "GPS.h"
#include "delay.h"
 
 #define Buf1_Max 200 					  //串口2缓存长度
 typedef struct  
{
	u8 year;
	u8 moon;
	u8 day;
	u8 hour;
	u8 minu;
	u8 sec;

}TIME;

extern TIME NowTime;
extern GPS_INFO GPS;

extern const char *string;	//IP登录服务器 （公网IP 和 本地端口） 10.1.13.214   202.113.34.98
extern const char *Timeserver;	//IP登录服务器 （公网IP 和 本地端口）

static unsigned char *SMS_Num="\"+8613010130500\"";//短信中心号   8613010130500      8613800220500   8613800210500
static unsigned char *phone="\"13212108513\"";     //短信接收方手机号
static unsigned char *content="收到回复a";//发送初始短信内容 AT+CCLK="16/06/18,13:11:52+08"

extern char Uart1_Buf[Buf1_Max];
extern char TimeRTC[50];

extern u8 Times,First_Int ,shijian;
extern u16 Heartbeat;
extern vu8 Timer0_start;	//定时器0延时启动计数器
extern vu8 Heart_beat;		//发送心跳帧标志位


void CLR_Buf1(void);
u8 Find(char *a);
void Second_AT_Command(char *b,char *a,u8 wait_time);
void Set_ATE0(void);
void Connect_Server(void);
void Rec_Server_Data(void);
void Wait_CREG(void);
void Send_OK(void);
void Set_Pdu_Mode(void);
void Send_PDUMessage(void);
void Set_Text_Mode(void);
char Send_Text_Sms(void);
char Send_FirstText_Sms(void);
void Connect_TimeServer(void);
void TimeGetWait(void);
void SetRTCtime(void);
TIME GetRTCtime(void); //注意：RTC内部存的时间还是世界时间，函数里已经转化到北京时间
void TTS_PlayTime(TIME time);

char TTS_PlayVariate(float length);
char TTS_PlayText(char *text);
char Judge_TTS(u8 wait_time);



#endif

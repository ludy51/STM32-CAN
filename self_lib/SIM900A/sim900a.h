#ifndef __SIM900A_H
#define __SIM900A_H

#include "stm32f10x.h"
#include "usart.h"	
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "GPS.h"
#include "delay.h"
 
 #define Buf1_Max 200 					  //����2���泤��
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

extern const char *string;	//IP��¼������ ������IP �� ���ض˿ڣ� 10.1.13.214   202.113.34.98
extern const char *Timeserver;	//IP��¼������ ������IP �� ���ض˿ڣ�

static unsigned char *SMS_Num="\"+8613010130500\"";//�������ĺ�   8613010130500      8613800220500   8613800210500
static unsigned char *phone="\"13212108513\"";     //���Ž��շ��ֻ���
static unsigned char *content="�յ��ظ�a";//���ͳ�ʼ�������� AT+CCLK="16/06/18,13:11:52+08"

extern char Uart1_Buf[Buf1_Max];
extern char TimeRTC[50];

extern u8 Times,First_Int ,shijian;
extern u16 Heartbeat;
extern vu8 Timer0_start;	//��ʱ��0��ʱ����������
extern vu8 Heart_beat;		//��������֡��־λ


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
TIME GetRTCtime(void); //ע�⣺RTC�ڲ����ʱ�仹������ʱ�䣬�������Ѿ�ת��������ʱ��
void TTS_PlayTime(TIME time);

char TTS_PlayVariate(float length);
char TTS_PlayText(char *text);
char Judge_TTS(u8 wait_time);



#endif

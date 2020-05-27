#include "sim900a.h" 


//wait_time���ڣ��ж��Ƿ�TTS���ڿ���
char Judge_TTS(u8 wait_time)
{
	
	CLR_Buf1();
	USART_Puts(USART1 ,"AT+CTTS?");
	UART1_SendLR();//���ͻس�

	Times = 0; //��0��ʼ��ʱ
	shijian = wait_time;
	Timer0_start = 1;
	CLR_Buf1();//��ս��ջ���
	
	while(1)
	{
			if(Find("1")&&Find("OK"))  //æ,ģ�鷵��1�󻹻��������OK	
			{ 
				CLR_Buf1();

				return 0; 
				
			}
			if(Find("0")&&Find("OK")) //����,ģ�鷵��0�󻹻��������OK	
			{		
					CLR_Buf1();
					return 1;
			}
			
			if(Timer0_start == 0) //��ʱʱ���ѵ���Timer0_start���ж��б��ر�
			{
				CLR_Buf1();
				return 0;//�жϳ�ʱ
			
			}

		}

}

/*******************************************************************************
* ������ : TTS_PlayText
* ����   :
							
//AT+CTTS=<mode>,<text>//���úͲ����ı�
//mode��
0 ֹͣ�����ϳɲ���
1 ��ʼ�����ϳɲ��ţ�<text>��UCS2���뷽ʽ�����ı���						
2 ��ʼ�����ϳɲ��ţ�<text>ֱ�������ı�����ͨ�ַ�����ASIIC�룬���ֲ���GBK���롣
*******************************************************************************/
char TTS_PlayText(char *text)
{
	

	char a[250] = "AT+CTTS=2,";
	TIM_Cmd(TIM1, DISABLE);  //ʧ��TIMx���� 
	if(Judge_TTS(1) == 1) //1S���жϣ��ǿ���
	{
		
		strcat(a, text);   //�����ַ�������	

	 Second_AT_Command(a,"OK",1);		//�ظ�OK ,���ͳɹ������������ɹ�����ҪAT+CTTS?�жϣ�����ʱ֮����������
   TIM_Cmd(TIM1, ENABLE);  //ʧ��TIMx����		
		return 1;//�ɹ�
	}
	else
	{
  TIM_Cmd(TIM1, ENABLE);  //ʧ��TIMx����		
		return 0;//æ
	}

}

/*******************************************************************************
* ������ : Send_Message
* ����   : ���Ͷ���
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Send_PDUMessage()
{
	Second_AT_Command("AT+CMGS=27",">",3); //�������ݳ��ȣ�27������ļ��㷽�������ڵ��ԱȽϣ����յ���>���ŷ��Ͷ�������
	USART_Puts(USART1 ,content);     //���Ͷ�������
	USART_SendData(USART1 ,0X1A);  //���ͽ�����
  UART1_SendLR();

}

/*******************************************************************************
* ������ : Set_Pdu_Mode
* ����   : ���ö���ΪPDU�ı�ģʽ
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Set_Pdu_Mode(void)
{
	Second_AT_Command("ATE0","OK",3);										  //ȡ������	
	Second_AT_Command("AT+CMGF=0","OK",3);								//����PDUģʽ	
	Second_AT_Command("AT+CPMS=\"SM\",\"SM\",\"SM\"","OK",3);//���в�������SIM���н���
}

/*******************************************************************************
* ������ : CLR_Buf1
* ����   : �������2��������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void CLR_Buf1(void)
{
	u16 k;
	for(k=0;k<Buf1_Max;k++)      //��������������
	{
		Uart1_Buf[k] = 0x00;
	}
    First_Int = 0;              //�����ַ�������ʼ�洢λ��
}

/*******************************************************************************
* ������ : Find
* ����   : �жϻ������Ƿ���ָ�����ַ���
* ����   : 
* ���   : 
* ����   : unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ� 
* ע��   : 
*******************************************************************************/

u8 Find(char *a)
{ 
  if(strstr(Uart1_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* ������ : Second_AT_Command
* ����   : ����ATָ���
* ����   : �������ݵ�ָ�롢���͵ȴ�ʱ��(��λ��S)
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/

void Second_AT_Command(char *b,char *a,u8 wait_time)  //����*b�ַ��������з���*a,  �����˳������޷���*a, ��ÿ��wait_time�뷢һ��*b     
{
	u8 i;
	char *c;
	c = b;										//�����ַ�����ַ��c
	CLR_Buf1(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a)) //���û�з���*a
		{
			if(Timer0_start == 0)
			{
				b = c;							//���ַ�����ַ��b
				for (; *b!='\0';b++)
				{
					while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
					USART_SendData(USART1,*b);//UART2_SendData(*b);
				}
				UART1_SendLR();

				Times = 0; //��0��ʼ��ʱ
				shijian = wait_time;
				Timer0_start = 1;
		   }
    }
 	  else
		{
			i = 1;
			Timer0_start = 0;
		}
	}
	
	CLR_Buf1(); 
}

/*******************************************************************************
* ������ : Wait_CREG
* ����   : �ȴ�ģ��ע��ɹ�
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Wait_CREG(void)
{
	u8 i;
	u8 k;
	i = 0;
	CLR_Buf1();
  while(i == 0)        			
	{
		CLR_Buf1();        
		USART_Puts(USART1 ,"AT+CREG?");
		
		UART1_SendLR();//���ͻس�
		delay_ms(5000);  						
	    for(k=0;k<Buf1_Max;k++)      			
    	{
				
			if(Uart1_Buf[k] == ':')
			{
				if((Uart1_Buf[k+4] == '1')||(Uart1_Buf[k+4] == '5'))
				{
					i = 1;
//					UART1_SendLR();
				  break;
				}
			}
		}
		
		
	}
}

/*******************************************************************************
* ������ : Set_ATE0
* ����   : ȡ������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Set_ATE0(void)
{
	Second_AT_Command("ATE0","OK",3);								//ȡ������		
}
/*******************************************************************************
* ������ : Connect_Server
* ����   : GPRS���ӷ���������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Connect_Server(void)
{
	USART_Puts(USART1 ,"AT+CIPCLOSE=1");	//�ر�����
  delay_ms(100);
	Second_AT_Command("AT+CIPSHUT","SHUT OK",2);		//�ر��ƶ�����
	Second_AT_Command("AT+CGCLASS=\"B\"","OK",2);//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ��� 
	
	Second_AT_Command("AT+CLPORT=\"TCP\",\"3000\"","OK",2); //�ʼǣ��ַ��������ַ�����Ӧ����\"�������ַ����� "
	//AT+CLPORT="TCP","3000"//(���ñ���TCP�˿�) //�˿ڿ����Լ��趨
	
	Second_AT_Command("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",2);//����PDP������,��������Э��,��������Ϣ
	Second_AT_Command("AT+CGATT=1","OK",2);//����GPRSҵ��
	Second_AT_Command("AT+CIPCSGP=1,\"CMNET\"","OK",2);//����ΪGPRS����ģʽ
	Second_AT_Command("AT+CIPHEAD=1","OK",2);//���ý���������ʾIPͷ(�����ж�������Դ,���ڵ�·������Ч)
	Second_AT_Command((char*)string,"OK",5);
	delay_ms(100);
	CLR_Buf1();
}
/*******************************************************************************
* ������ : Rec_Server_Data
* ����   : ���շ��������ݺ���
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Rec_Server_Data(void)
{
	if(strstr(Uart1_Buf,"+IPD")!=NULL)   		//�������ַ����к���^SISR
	{	
		Heartbeat=0;	//�������֡������
		Heart_beat=0;
		delay_ms(100);
//		if(strstr(Uart1_Buf,"onled")!=NULL)
//		{
//			LED1_ON();
//		}
//		else if(strstr(Uart1_Buf,"offled")!=NULL)
//		{
//			LED1_OFF();
//		}
		
		//USART_Puts(USART3 ,Uart1_Buf);
		
		CLR_Buf1();
		Heart_beat=1;//����Ӧ�����ݣ����߷������յ�����		
	}
}
/*******************************************************************************
* ������ : Send_OK
* ����   : ��������Ӧ���������ָ��ú�������������
					1�����յ������������ݺ�Ӧ�������
					2�����������·�����ʱ��ÿ��10�뷢��һ֡���������������������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Send_OK(void)
{
	Second_AT_Command("AT+CIPSEND",">",2);
	Second_AT_Command("OK\32\0","SEND OK",8);;			//�ظ�OK 
}


/*******************************************************************************
* ������ : TTS_PlayVariate
* ����   : ���뵥λΪcm  �����ɹ�����1  �����������ٷ��Ͳ���ָ��GSM���Զ���Ϊ��Ч
							
//AT+CTTS=<mode>,<text>//���úͲ����ı�
//mode��
0 ֹͣ�����ϳɲ���
1 ��ʼ�����ϳɲ��ţ�<text>��UCS2���뷽ʽ�����ı���						
2 ��ʼ�����ϳɲ��ţ�<text>ֱ�������ı�����ͨ�ַ�����ASIIC�룬���ֲ���GBK���롣
*******************************************************************************/
char TTS_PlayVariate(float length)
{
	char s[50];

	if(length < 800.0)///////////////????????????????????????????????????????????????????????????????
	{

		if(sprintf(s,"\"ǰ��%2.1f���ϰ�\"",length) != -1)
		{
	   
			TTS_PlayText(s);
			return 1;  //�ɹ�����
		}

	}
	
	return 0; //���볬�����߲���ʧ��

}

/*******************************************************************************
* ������ : TTS_PlayTime
* ����   :ʱ�䲥��
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void TTS_PlayTime(TIME time)
{
	char text[30];
	
	sprintf(text,"\"һ����%d��%d��%dʱ%d��\"",time.moon,time.day,time.hour,time.minu);//time.year
	
	TTS_PlayText(text); //����
	
}
/*******************************************************************************
* ������ : Get_GpsInfo
* ����   :��ȡGPS��Ϣ
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void TimeGetWait()
{
	u8 i =0;
	char timestr1[200]={0};
	char *timestr = timestr1;//ָ��timestr1��ַ
	
	delay_ms(3000); //һ������ʱ��������֣�Uart2_Buf�����жϽ������ݣ��ͽ�����strstr�ж�
	while(1)
	{
		delay_ms(2); 
		if(strstr(Uart1_Buf , "+IPD") != NULL && strstr(Uart1_Buf , "UTC") != NULL)
		{
			delay_ms(5); //����ֻ�û�����꣬�ͱ�timestr��ȡ
			timestr = strstr(Uart1_Buf , "+IPD");//����+IPD...ֵ
			break;
		}
		
	}
	
	
	//GPIO_SetBits(GPIOC, GPIO_Pin_15);//LED1
	delay_ms(5); //ע�⣺Uart2_Buf --> timestr��Ҫʱ��
//		CLR_Buf2();
//	printf("time %s\r\n",Uart2_Buf);
//	printf("time %s\r\n",timestr);
		//��ȡUTC����ʱ��
		for(i = 0 ; i <50 ; i++)
		{
			if(timestr[i] == '-')
			{
				NowTime.year = (timestr[i-2]-'0')*10 + (timestr[i-1]-'0');
//				printf("Y%d\r\n",year);
					
				NowTime.moon = (timestr[i+1]-'0')*10 + (timestr[i+2]-'0');		
//				printf("m%d\r\n",moon);
				NowTime.day=  (timestr[i+4]-'0')*10 + (timestr[i+5]-'0');
//				printf("d%d\r\n",day);
				
				NowTime.hour = (timestr[i+7]-'0')*10 + (timestr[i+8]-'0');
//				printf("H%d\r\n",hour);
				
				NowTime.minu = (timestr[i+10]-'0')*10 + (timestr[i+11]-'0');	
//				printf("M%d\r\n",minu);
				
				NowTime.sec =  (timestr[i+13]-'0')*10 + (timestr[i+14]-'0');
				
				sprintf(TimeRTC,"AT+CCLK=\"%c%c/%c%c/%c%c,%c%c:%c%c:%c%c+08\"",timestr[i-2],timestr[i-1],timestr[i+1],timestr[i+2],timestr[i+4],timestr[i+5],timestr[i+7],timestr[i+8],timestr[i+10],timestr[i+11],timestr[i+13],timestr[i+14]);

				//				printf("s%d\r\n",sec);
				break;
				
			}

		}
		//��8Сʱ 0���ӡ�ת��Ϊ����ʱ��
//		NowTime.minu += 3;
		NowTime.hour += 8;
		if(NowTime.minu >= 60)
		{
			NowTime.hour++;
			NowTime.minu = NowTime.minu%60;
		}
		if(NowTime.hour >= 24)
		{
			NowTime.day++;
			NowTime.hour = NowTime.hour%24;
		}
		if(NowTime.day >= 24)
		{
			NowTime.day++;
			NowTime.hour = NowTime.hour%24;
		}
		if(NowTime.day > 31) //�漰����С�����⡣������
		{
			NowTime.moon++;
			NowTime.day = NowTime.day%31;
		}
		if(NowTime.moon > 12)
		{
			NowTime.year++;
			NowTime.moon = NowTime.moon%12;
		}

	Second_AT_Command("AT+CIPSHUT","SHUT OK",2);		//�ر��ƶ�����
	delay_ms(300);
	CLR_Buf1();
	delay_ms(200);		
//printf("Y%d M%d D%d , H%d m%d s%d",year,moon,day,hour,minu,sec);

}
/*******************************************************************************
* ������ : Connect_TimeServer
* ����   : GPRS����Time����������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Connect_TimeServer()
{
	USART_Puts(USART1 ,"AT+CIPCLOSE=1");	//�ر�����
  delay_ms(100);
	Second_AT_Command("AT+CIPSHUT","SHUT OK",2);		//�ر��ƶ�����
	Second_AT_Command("AT+CGCLASS=\"B\"","OK",2);//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ��� 
	
	Second_AT_Command("AT+CLPORT=\"TCP\",\"3000\"","OK",2); //�ʼǣ��ַ��������ַ�����Ӧ����\"�������ַ����� "
	//AT+CLPORT="TCP","3000"//(���ñ���TCP�˿�) //�˿ڿ����Լ��趨
	
	Second_AT_Command("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",2);//����PDP������,��������Э��,��������Ϣ
	Second_AT_Command("AT+CGATT=1","OK",2);//����GPRSҵ��
	Second_AT_Command("AT+CIPCSGP=1,\"CMNET\"","OK",2);//����ΪGPRS����ģʽ
	Second_AT_Command("AT+CIPHEAD=1","OK",2);//���ý���������ʾIPͷ(�����ж�������Դ,���ڵ�·������Ч)
	Second_AT_Command((char*)Timeserver,"CONNECT OK",5);	
	

//	CLR_Buf1();
//	delay_ms(50);

}
/*******************************************************************************
* ������ : SetRTCtime
* ����   : GPRS���ӷ���������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void SetRTCtime()
{
//	sprintf(TimeRTC,"AT+CCLK=\"%d/%d/%d,%d:%d:%d+08\"",NowTime.year, NowTime.moon, NowTime.day, NowTime.hour, NowTime.minu, NowTime.sec);//��λ����������
	Second_AT_Command(TimeRTC,"OK",2);//���ý���������ʾIPͷ(�����ж�������Դ,���ڵ�·������Ч)
//	TimeRTC = "AT+CCLK=\"16/06/18,13:11:52+08\"";
//	printf("11");
	
}
/*******************************************************************************
* ������ : SetRTCtime
* ����   : 
* ����   : 
* ���   : 
* ����   : 
* ע��   :   +CCLK: "16/06/18,13:12:44+08"
*******************************************************************************/
TIME GetRTCtime()
{
	u8 i = 0;
	char timestr1[50]={0};
	char *timestr = timestr1;//ָ��timestr1��ַ
	TIME RTCTime;
	
	while(1)
	{	
		CLR_Buf1();
		USART_Puts(USART1 ,"AT+CCLK?");  //�¸Ľ�
		UART1_SendLR();//���ͻس�
	
		delay_ms(1000);
		if(strstr(Uart1_Buf , "+CCLK:") != NULL)
		{

			timestr = strstr(Uart1_Buf , "+CCLK:");
			break;
		}
	}
	delay_ms(500);
	
	for(i = 0 ; i <50 ; i++)
	{
			if(timestr[i] == '/')
			{
				RTCTime.year = (timestr[i-2]-'0')*10 + (timestr[i-1]-'0');

					
				RTCTime.moon = (timestr[i+1]-'0')*10 + (timestr[i+2]-'0');		

				RTCTime.day=  (timestr[i+4]-'0')*10 + (timestr[i+5]-'0');
		
				RTCTime.hour = (timestr[i+7]-'0')*10 + (timestr[i+8]-'0');
			
				RTCTime.minu = (timestr[i+10]-'0')*10 + (timestr[i+11]-'0');	
				
				RTCTime.sec =  (timestr[i+13]-'0')*10 + (timestr[i+14]-'0');

				break;
				
			}

	}
	
		//��8Сʱ 0���ӡ�ת��Ϊ����ʱ��
//		RTCTime.minu += 3;
		RTCTime.hour += 8;
		if(RTCTime.minu >= 60)
		{
			RTCTime.hour++;
			RTCTime.minu = RTCTime.minu%60;
		}
		if(RTCTime.hour >= 24)
		{
			RTCTime.day++;
			RTCTime.hour = RTCTime.hour%24;
		}
		
				
		if(RTCTime.moon == 1 || RTCTime.moon == 3|| RTCTime.moon == 5|| RTCTime.moon == 7|| RTCTime.moon == 8|| RTCTime.moon == 10|| RTCTime.moon == 12)
		{
			if(RTCTime.day > 31) //�漰����С�����⡣������
			{
				RTCTime.moon++;
				RTCTime.day = RTCTime.day%31;
			}
		}
		if(RTCTime.moon == 4 || RTCTime.moon == 6|| RTCTime.moon == 9|| RTCTime.moon == 11)
		{
			if(RTCTime.day > 30) //�漰����С�����⡣������
			{
				RTCTime.moon++;
				RTCTime.day = RTCTime.day%31;
			}
		}
		if(RTCTime.moon == 2)
		{
			if(RTCTime.day > 29) //�漰����С�����⡣������
			{
				RTCTime.moon++;
				RTCTime.day = RTCTime.day%31;
			}
		}
			
		
		if(RTCTime.moon > 12)
		{
			RTCTime.year++;
			RTCTime.moon = RTCTime.moon%12;
		}

	CLR_Buf1();
	return RTCTime;
	
	
}
/*******************************************************************************
* ������ : Set_Text_Mode
* ����   : ���ö���ΪTEXT�ı�ģʽ
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Set_Text_Mode(void)
{
	char temp[50]="AT+CSCA=";
//	Second_AT_Command("ATE0","OK",3);										//ȡ������	
	Second_AT_Command("AT+CMGF=1","OK",3);								//TEXTģʽ	
	Second_AT_Command("AT+CPMS=\"SM\",\"SM\",\"SM\"","OK",3);//���в�������SIM���н���
	strcat(temp,(char*)SMS_Num); //�ַ���ƴ�Ӻ������⺯����
	Second_AT_Command(temp,"OK",3);//���ö������ĺ�
}
/*******************************************************************************
* ������ : Send_Text_Sms
* ����   : ����TEXT�ı�����
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
char Send_Text_Sms(void)
{
	char temp[120]="AT+CMGS=";
	char message[200];
	 u16 count =0;
//	sprintf((char*)content , "����Ҫ������λ���ڣ�γ��%d��%d��%d�룬����%d��%d��%d�롣" ,  GPS.latitude_Degree , GPS.latitude_Cent ,GPS.latitude_Second ,GPS.longitude_Degree , GPS.longitude_Cent ,GPS.longitude_Second);
                                          	//����������������������������������������������������
	sprintf(message , "Help me! location in:latitude %dDegree-%dCent-%dSecond / longitude %dDegree-%dCent-%dSecond" ,GPS.latitude_Degree , GPS.latitude_Cent ,GPS.latitude_Second ,GPS.longitude_Degree , GPS.longitude_Cent ,GPS.longitude_Second);

	//sprintf(message , "Help me! location in:latitude %dDegree-%dCent-%dSecond / longitude %dDegree-%dCent-%dSecond" ,20 , 10 ,12 ,45 , 45 ,16);
	strcat(temp,(char*)phone); //�ַ���ƴ�Ӻ������⺯����
	Second_AT_Command(temp,">",3); //�ȴ����յ���>���ŷ��Ͷ�������
	USART_Puts(USART1 ,message);     //���Ͷ�������
	//USART_Puts(USART1 ,(char*)content);     //���Ͷ�������
	USART_SendData(USART1 ,0X1A);  //���ͽ�����
	UART1_SendLR();
	
	while(1)
	{
		if(Find("OK"))
		{
			UART1_SendLR();
			return 1;
		}
		if(Find("ERROR"))
		{
			UART1_SendLR();
			return 0;
		}
		count++;
		delay_ms(5);
		if(count > 2000) //10S
		{
			UART1_SendLR();
			count = 0;
			return 0;
		}
	}
}
/*******************************************************************************
* ������ : Send_Text_Sms
* ����   : ����TEXT�ı�����
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
char Send_FirstText_Sms(void)
{
	char temp[120]="AT+CMGS=";
	char message[50];
	 u16 count =0;
//	sprintf((char*)content , "����Ҫ������λ���ڣ�γ��%d��%d��%d�룬����%d��%d��%d�롣" ,  GPS.latitude_Degree , GPS.latitude_Cent ,GPS.latitude_Second ,GPS.longitude_Degree , GPS.longitude_Cent ,GPS.longitude_Second);
                                          	//����������������������������������������������������
	sprintf(message , "Help me!");

	//sprintf(message , "Help me! location in:latitude %dDegree-%dCent-%dSecond / longitude %dDegree-%dCent-%dSecond" ,20 , 10 ,12 ,45 , 45 ,16);
	strcat(temp,(char*)phone); //�ַ���ƴ�Ӻ������⺯����
	Second_AT_Command(temp,">",3); //�ȴ����յ���>���ŷ��Ͷ�������
	USART_Puts(USART1 ,message);     //���Ͷ�������
	//USART_Puts(USART1 ,(char*)content);     //���Ͷ�������
	USART_SendData(USART1 ,0X1A);  //���ͽ�����
	UART1_SendLR();
	
	while(1)
	{
		if(Find("OK"))
		{
			UART1_SendLR();
			return 1;
		}
		if(Find("ERROR"))
		{
			UART1_SendLR();
			return 0;
		}
		count++;
		delay_ms(5);
		if(count > 2000) //10S
		{
			UART1_SendLR();
			count = 0;
			return 0;
		}
	}
}


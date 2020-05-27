#include "sim900a.h" 


//wait_time秒内，判断是否TTS处于空闲
char Judge_TTS(u8 wait_time)
{
	
	CLR_Buf1();
	USART_Puts(USART1 ,"AT+CTTS?");
	UART1_SendLR();//发送回车

	Times = 0; //从0开始计时
	shijian = wait_time;
	Timer0_start = 1;
	CLR_Buf1();//清空接收缓存
	
	while(1)
	{
			if(Find("1")&&Find("OK"))  //忙,模块返回1后还会继续返回OK	
			{ 
				CLR_Buf1();

				return 0; 
				
			}
			if(Find("0")&&Find("OK")) //空闲,模块返回0后还会继续返回OK	
			{		
					CLR_Buf1();
					return 1;
			}
			
			if(Timer0_start == 0) //计时时间已到，Timer0_start在中断中被关闭
			{
				CLR_Buf1();
				return 0;//判断超时
			
			}

		}

}

/*******************************************************************************
* 函数名 : TTS_PlayText
* 描述   :
							
//AT+CTTS=<mode>,<text>//设置和播放文本
//mode：
0 停止语音合成播放
1 开始语音合成播放，<text>以UCS2编码方式输入文本。						
2 开始语音合成播放，<text>直接输入文本，普通字符采用ASIIC码，汉字采用GBK编码。
*******************************************************************************/
char TTS_PlayText(char *text)
{
	

	char a[250] = "AT+CTTS=2,";
	TIM_Cmd(TIM1, DISABLE);  //失能TIMx外设 
	if(Judge_TTS(1) == 1) //1S内判断，是空闲
	{
		
		strcat(a, text);   //连接字符串函数	

	 Second_AT_Command(a,"OK",1);		//回复OK ,发送成功（不代表播报成功，需要AT+CTTS?判断）（耗时之处！！！）
   TIM_Cmd(TIM1, ENABLE);  //失能TIMx外设		
		return 1;//成功
	}
	else
	{
  TIM_Cmd(TIM1, ENABLE);  //失能TIMx外设		
		return 0;//忙
	}

}

/*******************************************************************************
* 函数名 : Send_Message
* 描述   : 发送短信
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Send_PDUMessage()
{
	Second_AT_Command("AT+CMGS=27",">",3); //发送数据长度：27（具体的计算方法看串口调试比较）接收到“>”才发送短信内容
	USART_Puts(USART1 ,content);     //发送短信内容
	USART_SendData(USART1 ,0X1A);  //发送结束符
  UART1_SendLR();

}

/*******************************************************************************
* 函数名 : Set_Pdu_Mode
* 描述   : 设置短信为PDU文本模式
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Set_Pdu_Mode(void)
{
	Second_AT_Command("ATE0","OK",3);										  //取消回显	
	Second_AT_Command("AT+CMGF=0","OK",3);								//设置PDU模式	
	Second_AT_Command("AT+CPMS=\"SM\",\"SM\",\"SM\"","OK",3);//所有操作都在SIM卡中进行
}

/*******************************************************************************
* 函数名 : CLR_Buf1
* 描述   : 清除串口2缓存数据
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void CLR_Buf1(void)
{
	u16 k;
	for(k=0;k<Buf1_Max;k++)      //将缓存内容清零
	{
		Uart1_Buf[k] = 0x00;
	}
    First_Int = 0;              //接收字符串的起始存储位置
}

/*******************************************************************************
* 函数名 : Find
* 描述   : 判断缓存中是否含有指定的字符串
* 输入   : 
* 输出   : 
* 返回   : unsigned char:1 找到指定字符，0 未找到指定字符 
* 注意   : 
*******************************************************************************/

u8 Find(char *a)
{ 
  if(strstr(Uart1_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* 函数名 : Second_AT_Command
* 描述   : 发送AT指令函数
* 输入   : 发送数据的指针、发送等待时间(单位：S)
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/

void Second_AT_Command(char *b,char *a,u8 wait_time)  //发送*b字符串，若有返回*a,  函数退出，若无返回*a, 则每隔wait_time秒发一次*b     
{
	u8 i;
	char *c;
	c = b;										//保存字符串地址到c
	CLR_Buf1(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a)) //如果没有发现*a
		{
			if(Timer0_start == 0)
			{
				b = c;							//将字符串地址给b
				for (; *b!='\0';b++)
				{
					while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
					USART_SendData(USART1,*b);//UART2_SendData(*b);
				}
				UART1_SendLR();

				Times = 0; //从0开始计时
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
* 函数名 : Wait_CREG
* 描述   : 等待模块注册成功
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
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
		
		UART1_SendLR();//发送回车
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
* 函数名 : Set_ATE0
* 描述   : 取消回显
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Set_ATE0(void)
{
	Second_AT_Command("ATE0","OK",3);								//取消回显		
}
/*******************************************************************************
* 函数名 : Connect_Server
* 描述   : GPRS连接服务器函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Connect_Server(void)
{
	USART_Puts(USART1 ,"AT+CIPCLOSE=1");	//关闭连接
  delay_ms(100);
	Second_AT_Command("AT+CIPSHUT","SHUT OK",2);		//关闭移动场景
	Second_AT_Command("AT+CGCLASS=\"B\"","OK",2);//设置GPRS移动台类别为B,支持包交换和数据交换 
	
	Second_AT_Command("AT+CLPORT=\"TCP\",\"3000\"","OK",2); //笔记：字符串里有字符串，应该用\"代替子字符串的 "
	//AT+CLPORT="TCP","3000"//(设置本地TCP端口) //端口可以自己设定
	
	Second_AT_Command("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",2);//设置PDP上下文,互联网接协议,接入点等信息
	Second_AT_Command("AT+CGATT=1","OK",2);//附着GPRS业务
	Second_AT_Command("AT+CIPCSGP=1,\"CMNET\"","OK",2);//设置为GPRS连接模式
	Second_AT_Command("AT+CIPHEAD=1","OK",2);//设置接收数据显示IP头(方便判断数据来源,仅在单路连接有效)
	Second_AT_Command((char*)string,"OK",5);
	delay_ms(100);
	CLR_Buf1();
}
/*******************************************************************************
* 函数名 : Rec_Server_Data
* 描述   : 接收服务器数据函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Rec_Server_Data(void)
{
	if(strstr(Uart1_Buf,"+IPD")!=NULL)   		//若缓存字符串中含有^SISR
	{	
		Heartbeat=0;	//清除心跳帧计数器
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
		Heart_beat=1;//发送应答数据，告诉服务器收到数据		
	}
}
/*******************************************************************************
* 函数名 : Send_OK
* 描述   : 发送数据应答服务器的指令，该函数在有两功能
					1：接收到服务器的数据后，应答服务器
					2：服务器无下发数据时，每隔10秒发送一帧心跳，保持与服务器连接
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Send_OK(void)
{
	Second_AT_Command("AT+CIPSEND",">",2);
	Second_AT_Command("OK\32\0","SEND OK",8);;			//回复OK 
}


/*******************************************************************************
* 函数名 : TTS_PlayVariate
* 描述   : 输入单位为cm  播报成功返回1  播报过程中再发送播报指令GSM将自动认为无效
							
//AT+CTTS=<mode>,<text>//设置和播放文本
//mode：
0 停止语音合成播放
1 开始语音合成播放，<text>以UCS2编码方式输入文本。						
2 开始语音合成播放，<text>直接输入文本，普通字符采用ASIIC码，汉字采用GBK编码。
*******************************************************************************/
char TTS_PlayVariate(float length)
{
	char s[50];

	if(length < 800.0)///////////////????????????????????????????????????????????????????????????????
	{

		if(sprintf(s,"\"前方%2.1f米障碍\"",length) != -1)
		{
	   
			TTS_PlayText(s);
			return 1;  //成功播报
		}

	}
	
	return 0; //距离超出或者播报失败

}

/*******************************************************************************
* 函数名 : TTS_PlayTime
* 描述   :时间播报
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void TTS_PlayTime(TIME time)
{
	char text[30];
	
	sprintf(text,"\"一六年%d月%d日%d时%d分\"",time.moon,time.day,time.hour,time.minu);//time.year
	
	TTS_PlayText(text); //播报
	
}
/*******************************************************************************
* 函数名 : Get_GpsInfo
* 描述   :获取GPS信息
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void TimeGetWait()
{
	u8 i =0;
	char timestr1[200]={0};
	char *timestr = timestr1;//指向timestr1地址
	
	delay_ms(3000); //一定得延时，否则出现，Uart2_Buf还在中断接受数据，就进入了strstr判断
	while(1)
	{
		delay_ms(2); 
		if(strstr(Uart1_Buf , "+IPD") != NULL && strstr(Uart1_Buf , "UTC") != NULL)
		{
			delay_ms(5); //会出现还没接收完，就被timestr截取
			timestr = strstr(Uart1_Buf , "+IPD");//返回+IPD...值
			break;
		}
		
	}
	
	
	//GPIO_SetBits(GPIOC, GPIO_Pin_15);//LED1
	delay_ms(5); //注意：Uart2_Buf --> timestr需要时间
//		CLR_Buf2();
//	printf("time %s\r\n",Uart2_Buf);
//	printf("time %s\r\n",timestr);
		//提取UTC世界时间
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
		//加8小时 0分钟。转换为北京时间
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
		if(NowTime.day > 31) //涉及大月小月问题。。。。
		{
			NowTime.moon++;
			NowTime.day = NowTime.day%31;
		}
		if(NowTime.moon > 12)
		{
			NowTime.year++;
			NowTime.moon = NowTime.moon%12;
		}

	Second_AT_Command("AT+CIPSHUT","SHUT OK",2);		//关闭移动场景
	delay_ms(300);
	CLR_Buf1();
	delay_ms(200);		
//printf("Y%d M%d D%d , H%d m%d s%d",year,moon,day,hour,minu,sec);

}
/*******************************************************************************
* 函数名 : Connect_TimeServer
* 描述   : GPRS连接Time服务器函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Connect_TimeServer()
{
	USART_Puts(USART1 ,"AT+CIPCLOSE=1");	//关闭连接
  delay_ms(100);
	Second_AT_Command("AT+CIPSHUT","SHUT OK",2);		//关闭移动场景
	Second_AT_Command("AT+CGCLASS=\"B\"","OK",2);//设置GPRS移动台类别为B,支持包交换和数据交换 
	
	Second_AT_Command("AT+CLPORT=\"TCP\",\"3000\"","OK",2); //笔记：字符串里有字符串，应该用\"代替子字符串的 "
	//AT+CLPORT="TCP","3000"//(设置本地TCP端口) //端口可以自己设定
	
	Second_AT_Command("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",2);//设置PDP上下文,互联网接协议,接入点等信息
	Second_AT_Command("AT+CGATT=1","OK",2);//附着GPRS业务
	Second_AT_Command("AT+CIPCSGP=1,\"CMNET\"","OK",2);//设置为GPRS连接模式
	Second_AT_Command("AT+CIPHEAD=1","OK",2);//设置接收数据显示IP头(方便判断数据来源,仅在单路连接有效)
	Second_AT_Command((char*)Timeserver,"CONNECT OK",5);	
	

//	CLR_Buf1();
//	delay_ms(50);

}
/*******************************************************************************
* 函数名 : SetRTCtime
* 描述   : GPRS连接服务器函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void SetRTCtime()
{
//	sprintf(TimeRTC,"AT+CCLK=\"%d/%d/%d,%d:%d:%d+08\"",NowTime.year, NowTime.moon, NowTime.day, NowTime.hour, NowTime.minu, NowTime.sec);//个位数不对问题
	Second_AT_Command(TimeRTC,"OK",2);//设置接收数据显示IP头(方便判断数据来源,仅在单路连接有效)
//	TimeRTC = "AT+CCLK=\"16/06/18,13:11:52+08\"";
//	printf("11");
	
}
/*******************************************************************************
* 函数名 : SetRTCtime
* 描述   : 
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   :   +CCLK: "16/06/18,13:12:44+08"
*******************************************************************************/
TIME GetRTCtime()
{
	u8 i = 0;
	char timestr1[50]={0};
	char *timestr = timestr1;//指向timestr1地址
	TIME RTCTime;
	
	while(1)
	{	
		CLR_Buf1();
		USART_Puts(USART1 ,"AT+CCLK?");  //新改进
		UART1_SendLR();//发送回车
	
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
	
		//加8小时 0分钟。转换为北京时间
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
			if(RTCTime.day > 31) //涉及大月小月问题。。。。
			{
				RTCTime.moon++;
				RTCTime.day = RTCTime.day%31;
			}
		}
		if(RTCTime.moon == 4 || RTCTime.moon == 6|| RTCTime.moon == 9|| RTCTime.moon == 11)
		{
			if(RTCTime.day > 30) //涉及大月小月问题。。。。
			{
				RTCTime.moon++;
				RTCTime.day = RTCTime.day%31;
			}
		}
		if(RTCTime.moon == 2)
		{
			if(RTCTime.day > 29) //涉及大月小月问题。。。。
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
* 函数名 : Set_Text_Mode
* 描述   : 设置短信为TEXT文本模式
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Set_Text_Mode(void)
{
	char temp[50]="AT+CSCA=";
//	Second_AT_Command("ATE0","OK",3);										//取消回显	
	Second_AT_Command("AT+CMGF=1","OK",3);								//TEXT模式	
	Second_AT_Command("AT+CPMS=\"SM\",\"SM\",\"SM\"","OK",3);//所有操作都在SIM卡中进行
	strcat(temp,(char*)SMS_Num); //字符串拼接函数（库函数）
	Second_AT_Command(temp,"OK",3);//设置短信中心号
}
/*******************************************************************************
* 函数名 : Send_Text_Sms
* 描述   : 发送TEXT文本短信
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
char Send_Text_Sms(void)
{
	char temp[120]="AT+CMGS=";
	char message[200];
	 u16 count =0;
//	sprintf((char*)content , "我需要帮助！位置在：纬度%d度%d分%d秒，经度%d度%d分%d秒。" ,  GPS.latitude_Degree , GPS.latitude_Cent ,GPS.latitude_Second ,GPS.longitude_Degree , GPS.longitude_Cent ,GPS.longitude_Second);
                                          	//！！！！！！！！！！！！！！！！！！！！！！！待调试
	sprintf(message , "Help me! location in:latitude %dDegree-%dCent-%dSecond / longitude %dDegree-%dCent-%dSecond" ,GPS.latitude_Degree , GPS.latitude_Cent ,GPS.latitude_Second ,GPS.longitude_Degree , GPS.longitude_Cent ,GPS.longitude_Second);

	//sprintf(message , "Help me! location in:latitude %dDegree-%dCent-%dSecond / longitude %dDegree-%dCent-%dSecond" ,20 , 10 ,12 ,45 , 45 ,16);
	strcat(temp,(char*)phone); //字符串拼接函数（库函数）
	Second_AT_Command(temp,">",3); //等待接收到“>”才发送短信内容
	USART_Puts(USART1 ,message);     //发送短信内容
	//USART_Puts(USART1 ,(char*)content);     //发送短信内容
	USART_SendData(USART1 ,0X1A);  //发送结束符
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
* 函数名 : Send_Text_Sms
* 描述   : 发送TEXT文本短信
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
char Send_FirstText_Sms(void)
{
	char temp[120]="AT+CMGS=";
	char message[50];
	 u16 count =0;
//	sprintf((char*)content , "我需要帮助！位置在：纬度%d度%d分%d秒，经度%d度%d分%d秒。" ,  GPS.latitude_Degree , GPS.latitude_Cent ,GPS.latitude_Second ,GPS.longitude_Degree , GPS.longitude_Cent ,GPS.longitude_Second);
                                          	//！！！！！！！！！！！！！！！！！！！！！！！待调试
	sprintf(message , "Help me!");

	//sprintf(message , "Help me! location in:latitude %dDegree-%dCent-%dSecond / longitude %dDegree-%dCent-%dSecond" ,20 , 10 ,12 ,45 , 45 ,16);
	strcat(temp,(char*)phone); //字符串拼接函数（库函数）
	Second_AT_Command(temp,">",3); //等待接收到“>”才发送短信内容
	USART_Puts(USART1 ,message);     //发送短信内容
	//USART_Puts(USART1 ,(char*)content);     //发送短信内容
	USART_SendData(USART1 ,0X1A);  //发送结束符
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


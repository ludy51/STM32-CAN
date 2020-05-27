//======================================================================
//����Ȩ��(C) COPYRIGHT 2009 ������� WWW.TXMCU.COM  ALL RIGHTS RESERVED
//���������˳��������ѧϰ��ο���������ע����Ȩ��������Ϣ��
//======================================================================
#include "GPS.h"
//#include "LCD.h"
#include <string.h>
#include "delay.h"

uchar  init1[] = {"GPS waiting..."}; //ģ����Գ��� ceshi
//uchar  init2[] = {"WWW.TXMCU.COM"};  //WWW.TXMCU.COM 
//uchar  init3[] = {"GPS dingwei......"};//��ʼ��
//uchar  init4[] = {"shou suo...."};
//������λ����
static uchar GetComma(uchar num,char* str);
static double Get_Double_Number(char *s);
static float Get_Float_Number(char *s);
static void UTC2BTC(DATE_TIME *GPS);

//====================================================================//
//	�﷨��ʽ��	void GPS_Init(void)
//	ʵ�ֹ��ܣ�	GPS��ʼ������LCD����ʾ��ʼ����Ϣ
//	������		��
//	����ֵ��	��
//====================================================================//
void GPS_Init(void)
{         
//	OLED_CLS();
//	   OLED_P8x16Str(2,4,init1);
	
           //OLED_CLS();
          // OLED_P6x8Str(0,2,init2);//0��2��
          // OLED_P6x8Str(0,4,init3);
          // OLED_P6x8Str(0,6,init4);
           //OLED_P6x8Str(0,7,init4);
         //Lcd_DispLine(0, 0, init1);
	//Lcd_DispLine(1, 0, init2);
	//Lcd_DispLine(2, 0, init3);
	//Lcd_DispLine(3, 0, init4);
}

//====================================================================//
// �﷨��ʽ��int GPS_RMC_Parse(char *line, GPS_INFO *GPS)  
// ʵ�ֹ��ܣ���gpsģ���GPRMC��Ϣ����Ϊ��ʶ�������
// ��    �������ԭʼ��Ϣ�ַ����顢�洢��ʶ�����ݵĽṹ��
// �� �� ֵ��
//			 1: ����GPRMC���
//           0: û�н��н�������������Ч
//====================================================================//
int GPS_RMC_Parse(char *line,GPS_INFO *GPS)
{
	uchar ch, status, tmp;
	float lati_cent_tmp, lati_second_tmp;
	float long_cent_tmp, long_second_tmp;
	float speed_tmp;
	char *buf = line;
	ch = buf[5];
	status = buf[GetComma(2, buf)];

	if (ch == 'C')  //���������ַ���C��($GPRMC)
	{
		//OLED_P6x8Char(106,4, '1');  //����
                
          if (status == 'A')  //���������Ч����������ڶ�λ����λ״̬��A=��Ч��λ��V=��Ч��λ
		{
                  //OLED_P6x8Char(112,4, '2');  //����
			GPS -> NS       = buf[GetComma(4, buf)];
			GPS -> EW       = buf[GetComma(6, buf)];

			GPS->latitude   = Get_Double_Number(&buf[GetComma(3, buf)]);
			GPS->longitude  = Get_Double_Number(&buf[GetComma( 5, buf)]);

       		GPS->latitude_Degree  = (int)GPS->latitude / 100;       //����γ��
			lati_cent_tmp         = (GPS->latitude - GPS->latitude_Degree * 100) * 0.6;
			GPS->latitude_Cent    = (int)lati_cent_tmp;
			lati_second_tmp       = (lati_cent_tmp - GPS->latitude_Cent) * 60;
			GPS->latitude_Second  = (int)lati_second_tmp;

			GPS->longitude_Degree = (int)GPS->longitude / 100;	//���뾭��
			long_cent_tmp         = (GPS->longitude - GPS->longitude_Degree * 100) * 0.6;
			GPS->longitude_Cent   = (int)long_cent_tmp;    
			long_second_tmp       = (long_cent_tmp - GPS->longitude_Cent) * 60;
			GPS->longitude_Second = (int)long_second_tmp;

			speed_tmp      = Get_Float_Number(&buf[GetComma(7, buf)]);    //�ٶ�(��λ������/ʱ)
			GPS->speed     = speed_tmp * 1.85;                           //1����=1.85����
			GPS->direction = Get_Float_Number(&buf[GetComma(8, buf)]); //�Ƕ�			

			GPS->D.hour    = (buf[7] - '0') * 10 + (buf[8] - '0');		//ʱ��
			GPS->D.minute  = (buf[9] - '0') * 10 + (buf[10] - '0');
			GPS->D.second  = (buf[11] - '0') * 10 + (buf[12] - '0');
			tmp = GetComma(9, buf);
			GPS->D.day     = (buf[tmp + 0] - '0') * 10 + (buf[tmp + 1] - '0'); //����
			GPS->D.month   = (buf[tmp + 2] - '0') * 10 + (buf[tmp + 3] - '0');
			GPS->D.year    = (buf[tmp + 4] - '0') * 10 + (buf[tmp + 5] - '0')+2000;

			UTC2BTC(&GPS->D);
			
			return 1;
		}		
	}
	
	return 0;
}

//====================================================================//
// �﷨��ʽ��int GPS_GGA_Parse(char *line, GPS_INFO *GPS)  
// ʵ�ֹ��ܣ���gpsģ���GPGGA��Ϣ����Ϊ��ʶ�������
// ��    �������ԭʼ��Ϣ�ַ����顢�洢��ʶ�����ݵĽṹ��
// �� �� ֵ��
//			 1: ����GPGGA���
//           0: û�н��н�������������Ч
//====================================================================//
int GPS_GGA_Parse(char *line,GPS_INFO *GPS)//�ṹ���� GPS_INFO�൱��һ���Զ����������
{
	uchar ch, status;
	char *buf = line;
	ch = buf[4];
	status = buf[GetComma(2, buf)]; //���صڶ����������������λ��

	if (ch == 'G')  //$GPGGA
	{
		if (status != ',')
		{
			GPS->height_sea = Get_Float_Number(&buf[GetComma(9, buf)]);
			GPS->height_ground = Get_Float_Number(&buf[GetComma(11, buf)]);

			return 1;
		}
	}
	
	return 0;
}

//====================================================================//
// �﷨��ʽ: static float Str_To_Float(char *buf)
// ʵ�ֹ��ܣ� ��һ���ַ���ת���ɸ�����
// ��    �����ַ���
// �� �� ֵ��ת���󵥾���ֵ
//====================================================================//
static float Str_To_Float(char *buf)
{
	float rev = 0;
	float dat;
	int integer = 1;
	char *str = buf;
	int i;
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
			case '.':
				dat = '.';
				break;
		}
		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str ++;
			continue;
		}
		if( integer == 1 )
		{
			rev = rev * 10 + dat;
		}
		else
		{
			rev = rev + dat / (10 * i);
			i = i * 10 ;
		}
		str ++;
	}
	return rev;

}
												
//====================================================================//
// �﷨��ʽ: static float Get_Float_Number(char *s)
// ʵ�ֹ��ܣ� �Ѹ����ַ�����һ������֮ǰ���ַ�ת���ɵ�������
// ��    �����ַ���
// �� �� ֵ��ת���󵥾���ֵ
//====================================================================//
static float Get_Float_Number(char *s)
{
	char buf[10];
	uchar i;
	float rev;
	i=GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev=Str_To_Float(buf);
	return rev;	
}

//====================================================================//
// �﷨��ʽ: static double Str_To_Double(char *buf)
// ʵ�ֹ��ܣ� ��һ���ַ���ת���ɸ�����
// ��    �����ַ���
// �� �� ֵ��ת����˫����ֵ
//====================================================================//
static double Str_To_Double(char *buf)
{
	double rev = 0;
	double dat;
	int integer = 1;
	char *str = buf;
	int i;
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
			case '.':
				dat = '.';
				break;
		}
		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str ++;
			continue;
		}
		if( integer == 1 )
		{
			rev = rev * 10 + dat;
		}
		else
		{
			rev = rev + dat / (10 * i);
			i = i * 10 ;
		}
		str ++;
	}
	return rev;
}
												
//====================================================================//
// �﷨��ʽ: static double Get_Double_Number(char *s)
// ʵ�ֹ��ܣ��Ѹ����ַ�����һ������֮ǰ���ַ�ת����˫������
// ��    �����ַ���
// �� �� ֵ��ת����˫����ֵ
//====================================================================//
static double Get_Double_Number(char *s)
{
	char buf[10];
	uchar i;
	double rev;
	i=GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev=Str_To_Double(buf);
	return rev;	
}

//====================================================================//
// �﷨��ʽ��static uchar GetComma(uchar num,char *str)
// ʵ�ֹ��ܣ������ַ����и������ŵ�λ��
// ��    �������ҵĶ����ǵڼ����ĸ�������Ҫ���ҵ��ַ���
// �� �� ֵ��0
//====================================================================//
static uchar GetComma(uchar num,char *str)
{
	uchar i,j = 0;
	int len=strlen(str);

	for(i = 0;i < len;i ++)
	{
		if(str[i] == ',')
			j++;
		if(j == num)
			return i + 1;	
	}

	return 0;	
}

//====================================================================//
// �﷨��ʽ��void UTC2BTC(DATE_TIME *GPS)
// ʵ�ֹ��ܣ�ת��ʱ��Ϊ����ʱ����ʱ��
// ��    �������ʱ��Ľṹ��
// �� �� ֵ����
//====================================================================//
static void UTC2BTC(DATE_TIME *GPS)
{
	GPS->second ++;  
	if(GPS->second > 59)
	{
		GPS->second = 0;
		GPS->minute ++;
		if(GPS->minute > 59)
		{
			GPS->minute = 0;
			GPS->hour ++;
		}
	}	

    GPS->hour = GPS->hour + 8;
	if(GPS->hour > 23)
	{
		GPS->hour -= 24;
		GPS->day += 1;
		if(GPS->month == 2 ||
		   		GPS->month == 4 ||
		   		GPS->month == 6 ||
		   		GPS->month == 9 ||
		   		GPS->month == 11 )
		{
			if(GPS->day > 30)
			{
		   		GPS->day = 1;
				GPS->month++;
			}
		}
		else
		{
			if(GPS->day > 31)
			{	
		   		GPS->day = 1;
				GPS->month ++;
			}
		}
		if(GPS->year % 4 == 0 )
		{
	   		if(GPS->day > 29 && GPS->month == 2)
			{		
	   			GPS->day = 1;
				GPS->month ++;
			}
		}
		else
		{
	   		if(GPS->day > 28 &&GPS->month == 2)
			{
	   			GPS->day = 1;
				GPS->month ++;
			}
		}
		if(GPS->month > 12)
		{
			GPS->month -= 12;
			GPS->year ++;
		}		
	}
}
//====================================================================//
//	�﷨��ʽ��	Int_To_Str(int x,char *Str)
//	ʵ�ֹ��ܣ�	ת������ֵΪ�ַ�����ʽ
//	������		x: ת��������
//				Str:ת������ַ���
//	����ֵ��	��
//====================================================================//
void Int_To_Str(int x,char *Str)
{
	int t;
	char *Ptr,Buf[5];
	int i = 0;
	Ptr = Str;
	if(x < 10)		// ������С��10ʱ,ת��Ϊ"0x"�ĸ�ʽ
	{
		*Ptr ++ = '0';
		*Ptr ++ = x+0x30;
	}
	else
	{
		while(x > 0)
		{
			t = x % 10;
			x = x / 10;
			Buf[i++] = t+0x30;	// ͨ�����������ת����ASCII����ʽ
		}
		i -- ;
		for(;i >= 0;i --) 		// ���õ����ַ�������
		{
			*(Ptr++) = Buf[i];
		}
	}
	*Ptr = '\0';
}


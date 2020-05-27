#ifndef __GPS_H_
#define __GPS_H_

#define uchar unsigned char
#define uint  unsigned int

typedef struct{
	int year;  
	int month; 
	int  day;
	int hour;
	int minute;
	int second;
}DATE_TIME;

//typedef xdata struct{
typedef struct{
	double  latitude;  //����
	double  longitude; //γ��
	int     latitude_Degree;	//��
	int		latitude_Cent;		//��
	int   	latitude_Second;    //��
	int     longitude_Degree;	//��
	int		longitude_Cent;		//��
	int   	longitude_Second;   //��
	float 	speed;      //�ٶ�
	float 	direction;  //����
	float 	height_ground;    //ˮƽ��߶�
	float 	height_sea;       //���θ߶�
	uchar 	NS;
	uchar 	EW;
	DATE_TIME D;
}GPS_INFO;

void GPS_Init(void);
int GPS_RMC_Parse(char *line,GPS_INFO *GPS);
int GPS_GGA_Parse(char *line,GPS_INFO *GPS);

void Int_To_Str(int x,char *Str);

#endif  //__GPS_H_



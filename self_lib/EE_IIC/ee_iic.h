#ifndef __ee_ii_H
#define __ee_ii_H

#include "stm32f10x.h"
///////�ٶ������ĵ�ַ���Ŷ��Ӹߵ�ƽ�µ�Ѱַ��Χ////////
#define AT24C01		127  //0x007f
#define AT24C02		255 //0x00ff    ��ַ��Χ 0--255��0--0xFF��

#define AT24C04		511 //0x01ff
#define AT24C08		1023 //0x03ff
#define AT24C16		2047 //0x07ff
#define AT24C32		4095 //0x0fff

#define AT24C64	    8191 //0x1fff
#define AT24C128	16383 //0x3fff
#define AT24C256	32767 //0x7fff
#define AT24C512	0xffff

#define AT24C1024	0x0ffff //17λ���˴�����ַ���Ŷ��ӵ͵�ƽ��ֻ��16λ

///////////��Ҫ���ĵĵط�///////////////////
#define NOWAT24Cxx  AT24C02 //��ǰEEPROM���ͺ�

#define WriteAddr  0x3C
#define ReadAddr  0x3C+1

void IICInit(I2C_TypeDef* I2Cx, u16 firstAddr ,u32 ClockSpeed);//I2C1/2 , ��һ�����豸��ַ �� �����ٶȣ�С��400k��
void IICWrite_byte(I2C_TypeDef* I2Cx, u16 addr , u8 date);//дһ�ֽ�
u8 IICRead_byte(I2C_TypeDef* I2Cx, u16 addr);//��һ�ֽ�����

void IICWrite_Str(I2C_TypeDef* I2Cx, u16 addr , u8* strDate, u16 length );//д���ַ�����I2Cx���׵�ַ���ַ��������ȣ�
void IICRead_Str(I2C_TypeDef* I2Cx, u16 addr ,u16 NumByteToRead, u8* StrBuffer);//��ȡָ�������ַ�����I2Cx���׵�ַ�����ȣ��洢����
#endif










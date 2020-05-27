#ifndef __ee_ii_H
#define __ee_ii_H

#include "stm32f10x.h"
///////假定器件的地址引脚都接高电平下的寻址范围////////
#define AT24C01		127  //0x007f
#define AT24C02		255 //0x00ff    地址范围 0--255（0--0xFF）

#define AT24C04		511 //0x01ff
#define AT24C08		1023 //0x03ff
#define AT24C16		2047 //0x07ff
#define AT24C32		4095 //0x0fff

#define AT24C64	    8191 //0x1fff
#define AT24C128	16383 //0x3fff
#define AT24C256	32767 //0x7fff
#define AT24C512	0xffff

#define AT24C1024	0x0ffff //17位，此处将地址引脚都接低电平，只有16位

///////////需要更改的地方///////////////////
#define NOWAT24Cxx  AT24C02 //当前EEPROM的型号

#define WriteAddr  0x3C
#define ReadAddr  0x3C+1

void IICInit(I2C_TypeDef* I2Cx, u16 firstAddr ,u32 ClockSpeed);//I2C1/2 , 第一个从设备地址 ， 传输速度（小于400k）
void IICWrite_byte(I2C_TypeDef* I2Cx, u16 addr , u8 date);//写一字节
u8 IICRead_byte(I2C_TypeDef* I2Cx, u16 addr);//读一字节数据

void IICWrite_Str(I2C_TypeDef* I2Cx, u16 addr , u8* strDate, u16 length );//写入字符串（I2Cx，首地址，字符串，长度）
void IICRead_Str(I2C_TypeDef* I2Cx, u16 addr ,u16 NumByteToRead, u8* StrBuffer);//读取指定长度字符串（I2Cx，首地址，长度，存储区）
#endif










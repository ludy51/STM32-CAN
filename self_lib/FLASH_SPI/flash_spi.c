#include "flash_spi.h"
#include "delay.h"
#include "spi.h"

//本函数库以W25Q128为例：
//读取W25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
u8 FLASH_ReadSR(SPI_TypeDef* SPIx)   
{  
	u8 byte=0;   
	CS_ON;                            //使能器件   
	SPI_ReadWriteByte(SPIx ,W25X_ReadStatusReg); //发送读取状态寄存器命令    
	byte=SPI_ReadWriteByte(SPIx ,0Xff);          //读取一个字节  
	CS_OFF;                            //取消片选     
	return byte;   
} 
//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void FLASH_Write_SR(SPI_TypeDef* SPIx , u8 sr)   
{   
	CS_ON;                            //使能器件   
	SPI_ReadWriteByte(SPIx ,W25X_WriteStatusReg);//发送写取状态寄存器命令    
	SPI_ReadWriteByte(SPIx ,sr);               	//写入一个字节  
	CS_OFF;                          //取消片选     	      
} 

//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 
u16 FLASH_ReadID(SPI_TypeDef* SPIx)
{
	u16 Temp = 0;	
	CS_ON;
				    
	SPI_ReadWriteByte(SPIx , 0x90);//发送读取ID命令	    
	SPI_ReadWriteByte(SPIx , 0x00); 	    
	SPI_ReadWriteByte(SPIx , 0x00); 	    
	SPI_ReadWriteByte(SPIx , 0x00); 	 			   
	Temp|=SPI_ReadWriteByte(SPIx , 0xFF)<<8;  
	Temp|=SPI_ReadWriteByte(SPIx , 0xFF);	 
	CS_OFF;		    
	return Temp;

}


//读取指定地址的一字节数据
u8 FLASH_Read_byte(SPI_TypeDef* SPIx , u32 addr)
{
	u8 date;
	
	CS_ON;
	SPI_ReadWriteByte(SPIx , W25X_ReadData);//读数据指令
	
	SPI_ReadWriteByte(SPIx , (u8)(addr>>16));//24位地址的高8位（(u8)为强制取低8位）
	SPI_ReadWriteByte(SPIx , (u8)(addr>>8));//24位地址的中8位
	SPI_ReadWriteByte(SPIx , (u8)addr);//24位地址的低8位
	
	date = SPI_ReadWriteByte(SPIx , 0xff);
	
	CS_OFF;
	
	return date;
}


//读取指定长度字符串（I2Cx，首地址，长度，存储区）
void FLASH_Read_Str(SPI_TypeDef* SPIx, u32 addr ,u16 NumByteToRead, u8* StrBuffer)
{

	CS_ON;
	SPI_ReadWriteByte(SPIx , W25X_ReadData);
	
	SPI_ReadWriteByte(SPIx , (u8)(addr>>16));//24位地址的高8位（(u8)为强制取低8位）
	SPI_ReadWriteByte(SPIx , (u8)(addr>>8));//24位地址的中8位
	SPI_ReadWriteByte(SPIx , (u8)addr);//24位地址的低8位
	
	while(NumByteToRead)
	{
		*StrBuffer = SPI_ReadWriteByte(SPIx , 0xff);//只要没有CS_OFF，Addr会自动加1。可以连续读取。
		
		StrBuffer++;
		NumByteToRead--;
	}
	
	CS_OFF;
}


//直接往flash里写入任意长度的数据
void FLASH_Write(SPI_TypeDef* SPIx, u32 addr , u8* StrBuffer, u32 length )
{
	//u16 Sector_num;//扇区所在号  SectorStart = Sector_num*4096 
	u16 i;
	u32 remain_length;  //还剩下多少长度
	u32 writing_length;  //此时要写的长度
	u32 SectorStart; //扇区起点地址
	u16 SectorOffset;//在扇区内的偏移
	u8 SectorBuffer[4096];//存储整个扇区的数组
	
//	Sector_num = addr/4096;
	SectorOffset=addr%4096;
	SectorStart = addr - addr%4096;//该扇区的起点地址
	remain_length = length;
	
	if(remain_length > 4096 - addr%4096)
	{
		writing_length = 4096 - addr%4096;
	}
	else
	{
		writing_length = remain_length;
	}
	
	while(1)
	{
			
		FLASH_Read_Str(SPIx, SectorStart , 4096, SectorBuffer);//把扇区的数据备份出来
		
		for(i = 0 ; i < writing_length ; i++)
		{
			if(SectorBuffer[i+SectorOffset] != 0xff) break;//检测要写的部分段，是否有需要整个扇区擦除
		}
		
		if(i < writing_length)//需要擦除
		{
			FLASH_SectorErase(SPIx, SectorStart);//整个扇区擦除
			
			for(i = 0 ; i < writing_length ; i++)
			{
				SectorBuffer[i+SectorOffset] = StrBuffer[i];//把扇区数组要写的部分段替换掉
			}
			
			FLASH_Write_NoChack(SPIx, SectorStart, SectorBuffer, 4096); //写数据,把原本的数据以及要写的数据都写进扇区（防止了扇区原本数据的丢失）
		
		}
		else//不需要擦除
		{
			FLASH_Write_NoChack(SPIx, addr, StrBuffer, writing_length);//直接写
		}
			
		if(writing_length == remain_length)   break; //剩余的长度就是正写的长度，说明全写完，结束
		
		StrBuffer += writing_length; //写了writing_length个数据，数组也相应上加
		addr += writing_length;//地址相应上加
		remain_length -= writing_length;//长度相应下减
		
		SectorStart += 4096;  //进入下个扇区
		SectorOffset = 0;
		if(remain_length < 4096) //判断剩余长度是否不到一个扇区
		{
			writing_length = remain_length; //不到一个扇区，则剩余的长度就是正写的长度
		}
		else
		{
			writing_length = 4096; //超过了一个扇区，就写这整个扇区		

		}
	}
	
}

//在一页上写入一定长度数据
//条件：只能在一页上写，不能再页与页之间调用
//条件：length不能大于255字节（1页）
void FLASH_Write_page(SPI_TypeDef* SPIx, u32 addr,u8* StrBuffer, u32 length)
{
	CS_ON;
	
	SPI_ReadWriteByte(SPIx , W25X_WriteEnable);//写使能
	
	SPI_ReadWriteByte(SPIx ,W25X_PageProgram);//页写命令
	
	SPI_ReadWriteByte(SPIx , (u8)(addr>>16));//24位地址的高8位（(u8)为强制取低8位）
	SPI_ReadWriteByte(SPIx , (u8)(addr>>8));//24位地址的中8位
	SPI_ReadWriteByte(SPIx , (u8)addr);//24位地址的低8位
	
	
	while(length--)
	{
	
		SPI_ReadWriteByte(SPIx , *StrBuffer); //同一页内，地址自加可连续写
		StrBuffer++;
	}
	
	FLASH_Wait_Busy(SPIx);					   		//读取FLASH的忙标志，并等待写入结束
	
	CS_OFF;

}

//当扇区已经被擦除过，可以写入任意长度数据，可页与页之间写
//条件：已经被擦除过，否则无法写
void FLASH_Write_NoChack(SPI_TypeDef* SPIx, u32 addr , u8* StrBuffer, u32 length )
{
	
	u32 remaim_length;  //一共还剩下的长度
	u32 writing_length; //此时要在该页写的长度
	
	remaim_length = length;
	
	if(remaim_length < 256 - addr%256) //要写的长度只有第一页
	{
		writing_length = remaim_length;
	
	}
	else//要写的长度超过了第一页
	{
		writing_length = 256 - addr%256;
	}
	
	while(1)
	{
		FLASH_Write_page(SPIx, addr , StrBuffer, writing_length); //注:StrBuffer是指针，传入地址可以被改变
		
		if(writing_length == remaim_length)  break; //剩下的长度就是刚所写的长度，说明已经写完，跳出
		
		StrBuffer += writing_length	;//写了writing_length长度数据，数组也要上加（！！注意：StrBuffer上加后再作为实参，调入的地址就是加过后的地址，而不再初地址）
		addr += writing_length;//地址也要相应上加
		remaim_length -= writing_length;//剩下长度也要相应下减
		
		if(remaim_length < 256 ) //剩下的长度不到一页
		{
			writing_length = remaim_length;
	
		}
		else //剩下的长度超过一页
		{
			writing_length = 256; //即将要写的长度就是这页长度
		
		}

	}


}

//整个扇区擦除
//参数：（SPIx ， 扇区起点地址）
void FLASH_SectorErase(SPI_TypeDef* SPIx, u32 SectorStart)
{
	CS_ON;    //使能器件
	SPI_ReadWriteByte(SPIx , W25X_WriteEnable);//写使能
	SPI_ReadWriteByte(SPIx , W25X_SectorErase);// //扇区擦除
	
	FLASH_Wait_Busy(SPIx);   
	CS_ON;                                    	//使能器件   
	
	SPI_ReadWriteByte(SPIx , (u8)(SectorStart>>16));//24位地址的高8位（(u8)为强制取低8位）
	SPI_ReadWriteByte(SPIx , (u8)(SectorStart>>8));//24位地址的中8位
	SPI_ReadWriteByte(SPIx , (u8)SectorStart);//24位地址的低8位 
                           	              
	
	FLASH_Wait_Busy(SPIx);   				   		//等待擦除完成
	
  CS_OFF;                             //取消片选
}

//擦除整个芯片		  
//等待时间超长...
void FLASH_Erase_Chip(SPI_TypeDef* SPIx)   
{                                   
   	CS_ON;    //使能器件
		SPI_ReadWriteByte(SPIx , W25X_WriteEnable);//写使能
		FLASH_Wait_Busy(SPIx);
	
		CS_ON;    //使能器件
    SPI_ReadWriteByte(SPIx ,W25X_ChipErase);        	//发送片擦除命令  
	
		FLASH_Wait_Busy(SPIx);   				   		//等待擦除完成
		CS_OFF;                             //取消片选
}  



//等待空闲
void FLASH_Wait_Busy(SPI_TypeDef* SPIx)   
{   
	while((FLASH_ReadSR(SPIx)&0x01)==0x01);  		// 等待BUSY位清空
}  

//进入掉电模式
void FLASH_PowerDown(SPI_TypeDef* SPIx)   
{ 
  	CS_ON;                           	 	//使能器件   
    SPI_ReadWriteByte(SPIx,W25X_PowerDown);        //发送掉电命令  
		CS_OFF;                             	//取消片选     	      
    delay_us(3);                               //等待TPD  
}   

//唤醒
void FLASH_WAKEUP(SPI_TypeDef* SPIx)   
{  
  	CS_ON;                             	//使能器件   
    SPI_ReadWriteByte(SPIx, W25X_ReleasePowerDown);	//  send W25X_PowerDown command 0xAB    
		CS_OFF;                            	//取消片选     	      
    delay_us(3);                            	//等待TRES1
}   

//判断该扇区是否要擦除
//u8 SectorErase_Judge(SPI_TypeDef* SPIx, u16 Sector_num)
//{
//	u32 addr;
//	u16 Sector_size;
//	addr = Sector_num*4096;
//	Sector_size = 4096;        //4k
//	
//	CS_ON;  
//	
//	SPI_ReadWriteByte(SPIx , W25X_ReadData);
//	
//	SPI_ReadWriteByte(SPIx , (u8)(addr>>16));//24位地址的高8位（(u8)为强制取低8位）
//	SPI_ReadWriteByte(SPIx , (u8)(addr>>8));//24位地址的中8位
//	SPI_ReadWriteByte(SPIx , (u8)addr);//24位地址的低8位
//	
//	while(Sector_size)
//	{
//		if(SPI_ReadWriteByte(SPIx , 0xff) != 0xFF)  return 1;//需要擦除
//	
//		Sector_size--;
//		
//	}

//	CS_OFF;  
//	
//	return 0;

//}






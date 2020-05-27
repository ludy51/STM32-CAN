#include "flash_spi.h"
#include "delay.h"
#include "spi.h"

//����������W25Q128Ϊ����
//��ȡW25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
u8 FLASH_ReadSR(SPI_TypeDef* SPIx)   
{  
	u8 byte=0;   
	CS_ON;                            //ʹ������   
	SPI_ReadWriteByte(SPIx ,W25X_ReadStatusReg); //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI_ReadWriteByte(SPIx ,0Xff);          //��ȡһ���ֽ�  
	CS_OFF;                            //ȡ��Ƭѡ     
	return byte;   
} 
//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void FLASH_Write_SR(SPI_TypeDef* SPIx , u8 sr)   
{   
	CS_ON;                            //ʹ������   
	SPI_ReadWriteByte(SPIx ,W25X_WriteStatusReg);//����дȡ״̬�Ĵ�������    
	SPI_ReadWriteByte(SPIx ,sr);               	//д��һ���ֽ�  
	CS_OFF;                          //ȡ��Ƭѡ     	      
} 

//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 
u16 FLASH_ReadID(SPI_TypeDef* SPIx)
{
	u16 Temp = 0;	
	CS_ON;
				    
	SPI_ReadWriteByte(SPIx , 0x90);//���Ͷ�ȡID����	    
	SPI_ReadWriteByte(SPIx , 0x00); 	    
	SPI_ReadWriteByte(SPIx , 0x00); 	    
	SPI_ReadWriteByte(SPIx , 0x00); 	 			   
	Temp|=SPI_ReadWriteByte(SPIx , 0xFF)<<8;  
	Temp|=SPI_ReadWriteByte(SPIx , 0xFF);	 
	CS_OFF;		    
	return Temp;

}


//��ȡָ����ַ��һ�ֽ�����
u8 FLASH_Read_byte(SPI_TypeDef* SPIx , u32 addr)
{
	u8 date;
	
	CS_ON;
	SPI_ReadWriteByte(SPIx , W25X_ReadData);//������ָ��
	
	SPI_ReadWriteByte(SPIx , (u8)(addr>>16));//24λ��ַ�ĸ�8λ��(u8)Ϊǿ��ȡ��8λ��
	SPI_ReadWriteByte(SPIx , (u8)(addr>>8));//24λ��ַ����8λ
	SPI_ReadWriteByte(SPIx , (u8)addr);//24λ��ַ�ĵ�8λ
	
	date = SPI_ReadWriteByte(SPIx , 0xff);
	
	CS_OFF;
	
	return date;
}


//��ȡָ�������ַ�����I2Cx���׵�ַ�����ȣ��洢����
void FLASH_Read_Str(SPI_TypeDef* SPIx, u32 addr ,u16 NumByteToRead, u8* StrBuffer)
{

	CS_ON;
	SPI_ReadWriteByte(SPIx , W25X_ReadData);
	
	SPI_ReadWriteByte(SPIx , (u8)(addr>>16));//24λ��ַ�ĸ�8λ��(u8)Ϊǿ��ȡ��8λ��
	SPI_ReadWriteByte(SPIx , (u8)(addr>>8));//24λ��ַ����8λ
	SPI_ReadWriteByte(SPIx , (u8)addr);//24λ��ַ�ĵ�8λ
	
	while(NumByteToRead)
	{
		*StrBuffer = SPI_ReadWriteByte(SPIx , 0xff);//ֻҪû��CS_OFF��Addr���Զ���1������������ȡ��
		
		StrBuffer++;
		NumByteToRead--;
	}
	
	CS_OFF;
}


//ֱ����flash��д�����ⳤ�ȵ�����
void FLASH_Write(SPI_TypeDef* SPIx, u32 addr , u8* StrBuffer, u32 length )
{
	//u16 Sector_num;//�������ں�  SectorStart = Sector_num*4096 
	u16 i;
	u32 remain_length;  //��ʣ�¶��ٳ���
	u32 writing_length;  //��ʱҪд�ĳ���
	u32 SectorStart; //��������ַ
	u16 SectorOffset;//�������ڵ�ƫ��
	u8 SectorBuffer[4096];//�洢��������������
	
//	Sector_num = addr/4096;
	SectorOffset=addr%4096;
	SectorStart = addr - addr%4096;//������������ַ
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
			
		FLASH_Read_Str(SPIx, SectorStart , 4096, SectorBuffer);//�����������ݱ��ݳ���
		
		for(i = 0 ; i < writing_length ; i++)
		{
			if(SectorBuffer[i+SectorOffset] != 0xff) break;//���Ҫд�Ĳ��ֶΣ��Ƿ�����Ҫ������������
		}
		
		if(i < writing_length)//��Ҫ����
		{
			FLASH_SectorErase(SPIx, SectorStart);//������������
			
			for(i = 0 ; i < writing_length ; i++)
			{
				SectorBuffer[i+SectorOffset] = StrBuffer[i];//����������Ҫд�Ĳ��ֶ��滻��
			}
			
			FLASH_Write_NoChack(SPIx, SectorStart, SectorBuffer, 4096); //д����,��ԭ���������Լ�Ҫд�����ݶ�д����������ֹ������ԭ�����ݵĶ�ʧ��
		
		}
		else//����Ҫ����
		{
			FLASH_Write_NoChack(SPIx, addr, StrBuffer, writing_length);//ֱ��д
		}
			
		if(writing_length == remain_length)   break; //ʣ��ĳ��Ⱦ�����д�ĳ��ȣ�˵��ȫд�꣬����
		
		StrBuffer += writing_length; //д��writing_length�����ݣ�����Ҳ��Ӧ�ϼ�
		addr += writing_length;//��ַ��Ӧ�ϼ�
		remain_length -= writing_length;//������Ӧ�¼�
		
		SectorStart += 4096;  //�����¸�����
		SectorOffset = 0;
		if(remain_length < 4096) //�ж�ʣ�೤���Ƿ񲻵�һ������
		{
			writing_length = remain_length; //����һ����������ʣ��ĳ��Ⱦ�����д�ĳ���
		}
		else
		{
			writing_length = 4096; //������һ����������д����������		

		}
	}
	
}

//��һҳ��д��һ����������
//������ֻ����һҳ��д��������ҳ��ҳ֮�����
//������length���ܴ���255�ֽڣ�1ҳ��
void FLASH_Write_page(SPI_TypeDef* SPIx, u32 addr,u8* StrBuffer, u32 length)
{
	CS_ON;
	
	SPI_ReadWriteByte(SPIx , W25X_WriteEnable);//дʹ��
	
	SPI_ReadWriteByte(SPIx ,W25X_PageProgram);//ҳд����
	
	SPI_ReadWriteByte(SPIx , (u8)(addr>>16));//24λ��ַ�ĸ�8λ��(u8)Ϊǿ��ȡ��8λ��
	SPI_ReadWriteByte(SPIx , (u8)(addr>>8));//24λ��ַ����8λ
	SPI_ReadWriteByte(SPIx , (u8)addr);//24λ��ַ�ĵ�8λ
	
	
	while(length--)
	{
	
		SPI_ReadWriteByte(SPIx , *StrBuffer); //ͬһҳ�ڣ���ַ�Լӿ�����д
		StrBuffer++;
	}
	
	FLASH_Wait_Busy(SPIx);					   		//��ȡFLASH��æ��־�����ȴ�д�����
	
	CS_OFF;

}

//�������Ѿ���������������д�����ⳤ�����ݣ���ҳ��ҳ֮��д
//�������Ѿ����������������޷�д
void FLASH_Write_NoChack(SPI_TypeDef* SPIx, u32 addr , u8* StrBuffer, u32 length )
{
	
	u32 remaim_length;  //һ����ʣ�µĳ���
	u32 writing_length; //��ʱҪ�ڸ�ҳд�ĳ���
	
	remaim_length = length;
	
	if(remaim_length < 256 - addr%256) //Ҫд�ĳ���ֻ�е�һҳ
	{
		writing_length = remaim_length;
	
	}
	else//Ҫд�ĳ��ȳ����˵�һҳ
	{
		writing_length = 256 - addr%256;
	}
	
	while(1)
	{
		FLASH_Write_page(SPIx, addr , StrBuffer, writing_length); //ע:StrBuffer��ָ�룬�����ַ���Ա��ı�
		
		if(writing_length == remaim_length)  break; //ʣ�µĳ��Ⱦ��Ǹ���д�ĳ��ȣ�˵���Ѿ�д�꣬����
		
		StrBuffer += writing_length	;//д��writing_length�������ݣ�����ҲҪ�ϼӣ�����ע�⣺StrBuffer�ϼӺ�����Ϊʵ�Σ�����ĵ�ַ���Ǽӹ���ĵ�ַ�������ٳ���ַ��
		addr += writing_length;//��ַҲҪ��Ӧ�ϼ�
		remaim_length -= writing_length;//ʣ�³���ҲҪ��Ӧ�¼�
		
		if(remaim_length < 256 ) //ʣ�µĳ��Ȳ���һҳ
		{
			writing_length = remaim_length;
	
		}
		else //ʣ�µĳ��ȳ���һҳ
		{
			writing_length = 256; //����Ҫд�ĳ��Ⱦ�����ҳ����
		
		}

	}


}

//������������
//��������SPIx �� ��������ַ��
void FLASH_SectorErase(SPI_TypeDef* SPIx, u32 SectorStart)
{
	CS_ON;    //ʹ������
	SPI_ReadWriteByte(SPIx , W25X_WriteEnable);//дʹ��
	SPI_ReadWriteByte(SPIx , W25X_SectorErase);// //��������
	
	FLASH_Wait_Busy(SPIx);   
	CS_ON;                                    	//ʹ������   
	
	SPI_ReadWriteByte(SPIx , (u8)(SectorStart>>16));//24λ��ַ�ĸ�8λ��(u8)Ϊǿ��ȡ��8λ��
	SPI_ReadWriteByte(SPIx , (u8)(SectorStart>>8));//24λ��ַ����8λ
	SPI_ReadWriteByte(SPIx , (u8)SectorStart);//24λ��ַ�ĵ�8λ 
                           	              
	
	FLASH_Wait_Busy(SPIx);   				   		//�ȴ��������
	
  CS_OFF;                             //ȡ��Ƭѡ
}

//��������оƬ		  
//�ȴ�ʱ�䳬��...
void FLASH_Erase_Chip(SPI_TypeDef* SPIx)   
{                                   
   	CS_ON;    //ʹ������
		SPI_ReadWriteByte(SPIx , W25X_WriteEnable);//дʹ��
		FLASH_Wait_Busy(SPIx);
	
		CS_ON;    //ʹ������
    SPI_ReadWriteByte(SPIx ,W25X_ChipErase);        	//����Ƭ��������  
	
		FLASH_Wait_Busy(SPIx);   				   		//�ȴ��������
		CS_OFF;                             //ȡ��Ƭѡ
}  



//�ȴ�����
void FLASH_Wait_Busy(SPI_TypeDef* SPIx)   
{   
	while((FLASH_ReadSR(SPIx)&0x01)==0x01);  		// �ȴ�BUSYλ���
}  

//�������ģʽ
void FLASH_PowerDown(SPI_TypeDef* SPIx)   
{ 
  	CS_ON;                           	 	//ʹ������   
    SPI_ReadWriteByte(SPIx,W25X_PowerDown);        //���͵�������  
		CS_OFF;                             	//ȡ��Ƭѡ     	      
    delay_us(3);                               //�ȴ�TPD  
}   

//����
void FLASH_WAKEUP(SPI_TypeDef* SPIx)   
{  
  	CS_ON;                             	//ʹ������   
    SPI_ReadWriteByte(SPIx, W25X_ReleasePowerDown);	//  send W25X_PowerDown command 0xAB    
		CS_OFF;                            	//ȡ��Ƭѡ     	      
    delay_us(3);                            	//�ȴ�TRES1
}   

//�жϸ������Ƿ�Ҫ����
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
//	SPI_ReadWriteByte(SPIx , (u8)(addr>>16));//24λ��ַ�ĸ�8λ��(u8)Ϊǿ��ȡ��8λ��
//	SPI_ReadWriteByte(SPIx , (u8)(addr>>8));//24λ��ַ����8λ
//	SPI_ReadWriteByte(SPIx , (u8)addr);//24λ��ַ�ĵ�8λ
//	
//	while(Sector_size)
//	{
//		if(SPI_ReadWriteByte(SPIx , 0xff) != 0xFF)  return 1;//��Ҫ����
//	
//		Sector_size--;
//		
//	}

//	CS_OFF;  
//	
//	return 0;

//}






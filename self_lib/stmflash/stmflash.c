#include "stmflash.h"
#include "delay.h"
//#include "usart.h"

u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//STM32 FLASH ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
////////u16 STMFLASH_ReadHalfWord(u32 faddr)
////////{
////////	return *(vu16*)faddr; 
////////}
//////////#if STM32_FLASH_WREN	//���ʹ����д   
////////////������д��
////////////WriteAddr:��ʼ��ַ
////////////pBuffer:����ָ��
////////////NumToWrite:����(16λ)��   
//////////void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
//////////{ 			 		 
//////////	u16 i;
//////////	for(i=0;i<NumToWrite;i++)
//////////	{
//////////		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
//////////	    WriteAddr+=2;//��ַ����2.
//////////	}  
//////////} 
////////////��ָ����ַ��ʼд��ָ�����ȵ�����
////////////WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
////////////pBuffer:����ָ��
////////////NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
//////////#if STM32_FLASH_SIZE<256
//////////#define STM_SECTOR_SIZE 1024 //�ֽ�
//////////#else 
//////////#define STM_SECTOR_SIZE	2048
//////////#endif		 
//////////u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
//////////#endif

//void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
//{ 			 		 
//	u16 i;
//	for(i=0;i<NumToWrite;i++)
//	{
//		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
//	    WriteAddr+=2;//��ַ����2.
//	}  
//} 
//////////void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
////////{
////////	u32 secpos;	   //������ַ
////////	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
////////	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
//////// 	u16 i;    
////////	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
////////	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
////////	FLASH_Unlock();						//����
////////	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
////////	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
////////	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
////////	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
////////	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
////////	while(1) 
////////	{	
////////		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
////////		for(i=0;i<secremain;i++)//У������
////////		{
////////			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
////////		}
////////		if(i<secremain)//��Ҫ����
////////		{
////////			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
////////			for(i=0;i<secremain;i++)//����
////////			{
////////				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
////////			}
////////			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
////////		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
////////		if(NumToWrite==secremain)break;//д�������
////////		else//д��δ����
////////		{
////////			secpos++;				//������ַ��1
////////			secoff=0;				//ƫ��λ��Ϊ0 	 
////////		   	pBuffer+=secremain;  	//ָ��ƫ��
////////			WriteAddr+=secremain;	//д��ַƫ��	   
////////		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
////////			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
////////			else secremain=NumToWrite;//��һ����������д����
////////		}	 
////////	};	
////////	FLASH_Lock();//����
////////}
////////#endif

//////////��ָ����ַ��ʼ����ָ�����ȵ�����
//////////ReadAddr:��ʼ��ַ
//////////pBuffer:����ָ��
//////////NumToWrite:����(16λ)��
////////void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
////////{
////////	u16 i;
////////	for(i=0;i<NumToRead;i++)
////////	{
////////		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
////////		ReadAddr+=2;//ƫ��2���ֽ�.	
////////	}
////////}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
////void Test_Write(u32 WriteAddr,u16 WriteData)   	
////{
////	Stm_writeFlash(WriteAddr,&WriteData,1);//д��һ���� 
////}
void stm_WriteFlash_NoCheck(u32 Write_Addr,u16 *Buffer,u16 Write_Num)
{
		u16 i;
	for(i=0;i<Write_Num;i++)
	{
		FLASH_ProgramHalfWord(Write_Addr,Buffer[i]);
	    Write_Addr+=2;//��ַ����2.
	}

}



void Test_FlashWrite(u32 Write_Addr,u16 Write_Data)  
{
	Stm_writeFlash(Write_Addr,&Write_Data,1);//д��һ���� 
}




u16 Read_Half_WORD(u32 flshaddr)
{
	return *(vu16*)flshaddr; 
}

void Stm_writeFlash(u32 Write_Addr,u16 *Buffer,u16 Write_Num)
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(Write_Addr<STM32_FLASH_BASE||(Write_Addr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock();						//����
	offaddr=Write_Addr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(Write_Num<=secremain)secremain=Write_Num;//�����ڸ�������Χ
	while(1) 
	{	
		Stm_readFlash(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=Buffer[i];	  
			}
			stm_WriteFlash_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else stm_WriteFlash_NoCheck(Write_Addr,Buffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(Write_Num==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	Buffer+=secremain;  	//ָ��ƫ��
			Write_Addr+=secremain;	//д��ַƫ��	   
		   	Write_Num-=secremain;	//�ֽ�(16λ)���ݼ�
			if(Write_Num>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=Write_Num;//��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
}


void Stm_readFlash(u32 Read_Addr,u16 *Buffer,u16 Read_Num)
{
	u16 i;
	for(i=0;i<Read_Num;i++)
	{
		Buffer[i]=Read_Half_WORD(Read_Addr);//��ȡ2���ֽ�.
		Read_Addr+=2;//ƫ��2���ֽ�.	
	}
	
}








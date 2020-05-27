#include "ee_iic.h"
#include "delay.h"
#include "MyInit.h"

void IICInit(I2C_TypeDef* I2Cx, u16 firstAddr ,u32 ClockSpeed)
{
	I2C_InitTypeDef I2C_InitTypeDefture;
	
	if(I2Cx == I2C1)//B6��SCL  B7��SDA
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 , ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		GPIOInit(GPIOB ,GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_AF_OD ,0);//IIC1�������ţ����ÿ�©���(��©�����������Ҫ������)
	}
	
	if(I2Cx == I2C2)//B10��SCL  B11��SDA
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2 , ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		GPIOInit(GPIOB ,GPIO_Pin_10 | GPIO_Pin_11, GPIO_Mode_AF_OD ,0);//IIC2��������
	}
	
	assert_param( ClockSpeed <= 400000);//���ԣ�ֻ��debug����Ч
	 
	I2C_InitTypeDefture.I2C_Ack = I2C_Ack_Enable;//Ӧ��ʹ��
	I2C_InitTypeDefture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//������ַΪ7λ
	I2C_InitTypeDefture.I2C_ClockSpeed = ClockSpeed;//����ʱ��Ƶ�ʣ����ֵ���ܸ��� 400KHz��
	I2C_InitTypeDefture.I2C_DutyCycle = I2C_DutyCycle_2;//����ģʽ��ѡ��
	I2C_InitTypeDefture.I2C_Mode = I2C_Mode_I2C;//iicģʽ
	I2C_InitTypeDefture.I2C_OwnAddress1 = firstAddr;//��һ�����豸��ַ��������ַ��һ�������ڶ���ӻ��¡�
	
	I2C_Init(I2Cx, &I2C_InitTypeDefture);
	
	I2C_Cmd(I2Cx, ENABLE);

}


//��ַaddrд��date
void IICWrite_byte(I2C_TypeDef* I2Cx, u16 addr , u8 date)
{
	u8 addrH;
	u8 addrL;
	if(addr > NOWAT24Cxx) //��ַ���ܳ���ѡַ��Χ
	{		
		addr = addr % NOWAT24Cxx;			
	}	

	addrH  = addr>>8; //ȡ����λ��ַ
	addrL = addr%256; //ȡ����λ��ַ
	
	I2C_AcknowledgeConfig(I2Cx, ENABLE);//Ӧ��ʹ�ܣ���������������Ӧ��ʹ�ܣ�
	I2C_GenerateSTART(I2Cx, ENABLE);//����ʼ�źţ��������ݴ���
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) ;//���ͳɹ�
	
	I2C_Send7bitAddress(I2Cx, WriteAddr, I2C_Direction_Transmitter);//��ָ��������ַ��Ϊ����ģʽ��0xA0���ɱ䣩
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) ;
	
	if(NOWAT24Cxx <= AT24C02)//��ַֻ��һ�ֽڵ�λ��EEPEOM
	{		
		I2C_SendData(I2Cx, addrL);//����Ҫд��ĵ�ַ��һ�ֽڣ�
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//�����ֽڳɹ�
	}
	else//��ַ�иߵ�λ���ֽڵĵ�EEPEOM
	{

		I2C_SendData(I2Cx, addrH);//����Ҫд��ĵ�ַ��һ�ֽڣ� ��λ
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//�����ֽڳɹ�
		
		I2C_SendData(I2Cx, addrL);//����Ҫд��ĵ�ַ��һ�ֽڣ� ��λ
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//�����ֽڳɹ�

	}
	
	I2C_SendData(I2Cx, date);//��������   ��ַ��Χ 0--255��0--0xFF��
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//�����ֽڳɹ�
	
	//I2C_AcknowledgeConfig(I2Cx, DISABLE);//Ӧ��ʹ�ܣ���������������Ӧ��ʹ�ܣ�
  I2C_GenerateSTOP(I2Cx, ENABLE);//��ֹͣ�źţ�ֹͣ���ݴ���

}

//��д��Ҫ���ĵ�ַ���ٶ�����
u8 IICRead_byte(I2C_TypeDef* I2Cx, u16 addr)
{
	u8 RecieveDate = 0;
	u8 addrH;
	u8 addrL;
	if(addr > NOWAT24Cxx) //��ַ���ܳ���ѡַ��Χ
	{		
		addr = addr % NOWAT24Cxx;			
	}	

	addrH  = addr>>8; //ȡ����λ��ַ
	addrL = addr%256; //ȡ����λ��ַ
	
	I2C_AcknowledgeConfig(I2Cx, ENABLE);//Ӧ��ʹ�ܣ���������������Ӧ��ʹ�ܣ�
	I2C_GenerateSTART(I2Cx, ENABLE);//����ʼ�źţ��������ݴ���
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) ;//�ȴ�����Ӧ��
	
	I2C_Send7bitAddress(I2Cx, WriteAddr, I2C_Direction_Transmitter);//αдָ�� ������������ַ��0xA0��Ϊ���ͣ��ɱ䣩
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) ;//�ȴ�����ģʽӦ��
	
	if(NOWAT24Cxx <= AT24C02)//��ַֻ��һ�ֽڵ�λ��EEPEOM
	{
		I2C_SendData(I2Cx, addrL);//����Ҫд��ĵ�ַ��һ�ֽڣ���λ
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//�����ֽڳɹ�
	}
	else//��ַ�иߵ�λ���ֽڵĵ�EEPEOM
	{
		I2C_SendData(I2Cx, addrH);//����Ҫд��ĵ�ַ��һ�ֽڣ���λ
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//�����ֽڳɹ�
		I2C_SendData(I2Cx, addrL);//����Ҫ�����ݵĵ�ַ��һ�ֽڣ���λ
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//�ȴ������ֽ�Ӧ��
	}
		I2C_GenerateSTART(I2Cx, ENABLE);//�������ź�
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) ;//�ȴ�����Ӧ��
	
		I2C_Send7bitAddress(I2Cx, ReadAddr, I2C_Direction_Receiver);//������������ַ��0xA1��Ϊ���գ��ɱ䣩
	  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) ;//�ȴ�����ģʽӦ��
	
		I2C_AcknowledgeConfig(I2Cx, DISABLE);//ʧ��Ӧ��
		I2C_GenerateSTOP(I2Cx, ENABLE);//��ֹͣ�źţ�ֹͣ���ݴ���
		
		while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED)) ;//�ȴ����ݱ�����
		RecieveDate = I2C_ReceiveData(I2Cx);

	  return RecieveDate ;
}



//д���ַ���(�ȽϺ�ʱ��һ��һ��д)
void IICWrite_Str(I2C_TypeDef* I2Cx, u16 addr , u8* strDate, u16 length )
{

	
	while(length) //��������length������
	{	
		//while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) ;
    IICWrite_byte(I2Cx, addr , *strDate);//һ��һ������ʽ����
		
		delay_ms(4);//һ������ʱ���������Ῠ��
		
		addr++;
		strDate++;
		length--;	
	}
}


//��ȡָ�������ַ�����I2Cx���׵�ַ�����ȣ��洢����
void IICRead_Str(I2C_TypeDef* I2Cx, u16 addr ,u16 NumByteToRead, u8* StrBuffer)

{

	u8 addrH;
	u8 addrL;
	if(addr > NOWAT24Cxx) //��ַ���ܳ���ѡַ��Χ
	{		
		addr = addr % NOWAT24Cxx;			
	}	

	addrH  = addr>>8; //ȡ����λ��ַ
	addrL = addr%256; //ȡ����λ��ַ
	
	I2C_AcknowledgeConfig(I2Cx, ENABLE);//Ӧ��ʹ�ܣ���������������Ӧ��ʹ�ܣ�
	I2C_GenerateSTART(I2Cx, ENABLE);//����ʼ�źţ��������ݴ���
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) ;//�ȴ�����Ӧ��
	
	I2C_Send7bitAddress(I2Cx, WriteAddr, I2C_Direction_Transmitter);//αдָ�� ������������ַ��0xA0��Ϊ���ͣ��ɱ䣩
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) ;//�ȴ�����ģʽӦ��
	
	if(NOWAT24Cxx <= AT24C02)//��ַֻ��һ�ֽڵ�λ��EEPEOM
	{
		I2C_SendData(I2Cx, addrL);//����Ҫд��ĵ�ַ��һ�ֽڣ���λ
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//�����ֽڳɹ�
	}
	else//��ַ�иߵ�λ���ֽڵĵ�EEPEOM
	{
		I2C_SendData(I2Cx, addrH);//����Ҫд��ĵ�ַ��һ�ֽڣ���λ
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//�����ֽڳɹ�
		I2C_SendData(I2Cx, addrL);//����Ҫ�����ݵĵ�ַ��һ�ֽڣ���λ
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//�ȴ������ֽ�Ӧ��
	}
	
	I2C_GenerateSTART(I2Cx, ENABLE);//�������ź�
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) ;//�ȴ�����Ӧ��

	I2C_Send7bitAddress(I2Cx, ReadAddr, I2C_Direction_Receiver);//������������ַ��0xA1��Ϊ���գ��ɱ䣩
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) ;//�ȴ�����ģʽӦ��

	
	while(NumByteToRead)
	{

		while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED)) ;//�ȴ����ݱ�����
		*StrBuffer = I2C_ReceiveData(I2Cx);//ֻҪû�з���stopָ�Addr���Զ���1������������ȡ��
		
		StrBuffer++;
		NumByteToRead--;
	}
	if(NumByteToRead == 0)//���һλ�������Լӵ�ַ
	{
			I2C_AcknowledgeConfig(I2Cx, DISABLE);//ʧ��Ӧ��
			I2C_GenerateSTOP(I2Cx, ENABLE);//��ֹͣ�źţ���ַ�޷��Լӣ����ݴ���ֹͣ
	}
		
}
/**

1����������ַ��0xAx��֮ǰ���ȷ��������ź�STAR
2��ֻҪ�����и������������źţ����������᷵������һ��Ӧ���źţ�����ÿ�䷢�͵���غ�������Ҫ�ȴ���ص�Ӧ��
3��ʹ�ý������ݺ���֮ǰ������ʧ��Ӧ�����ú�������Ϊ�ú���ʱ�Ӷ�����Ӧ��ʧ�ܲ�Ӱ��
4��ֹͣ�źź�������д�ڽ������ݺ���ǰ�棬Ҳ��Ӱ��
5��������������ȴ����ݱ����յ�Ӧ��д�ڽ������ݺ���ǰ��
5�����պ�������Ҫд�����Դӵ�Ӧ����Ϊ���Ӧ�𼯳ɵ��˹̼���������ģ���IIC��Ҫ�������Դ�Ӧ��
6�������ĵ�ַһ�㱻�ֳɺü�ҳ��������д���ַ�������ʱ�������ַ�������ҳд���ݣ�����һ��һ��д���ݣ�
  i����ҳд���ݣ���whileѭ���У����ϵ�I2C_SendData�����ֽڣ�ȫ����������STOP����;��Ҫ�ж��Ƿ��ַ��ҳβ�������
  ii��һ��һ��д�����ַ����ֳ�ÿ���������ַ���ÿ���ַ�����Ҫ�����������ź�->����->ֹͣ�Ĺ��̣����η��ͣ������ж�ҳβ��
7����ȡ�����ַ���������ʱ�������ķ�ҳ������Ӱ�죬����������I2C_ReceiveData��ȡ���ݲ��ҵ�ַҲ���Զ����Լӣ�������STOPʱ����ַ���Լ�ֹͣ����
8:ע��:һ����ַֻ�ܴ�8λ��������һ�ֽڣ����Ĵ�С���ᳬ��255������������԰��ߵ�λ�ֱ��������ַ��
*/





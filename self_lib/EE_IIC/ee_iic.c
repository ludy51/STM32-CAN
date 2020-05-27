#include "ee_iic.h"
#include "delay.h"
#include "MyInit.h"

void IICInit(I2C_TypeDef* I2Cx, u16 firstAddr ,u32 ClockSpeed)
{
	I2C_InitTypeDef I2C_InitTypeDefture;
	
	if(I2Cx == I2C1)//B6：SCL  B7：SDA
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 , ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		GPIOInit(GPIOB ,GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_AF_OD ,0);//IIC1复用引脚，复用开漏输出(开漏输出有引脚需要外上拉)
	}
	
	if(I2Cx == I2C2)//B10：SCL  B11：SDA
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2 , ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		GPIOInit(GPIOB ,GPIO_Pin_10 | GPIO_Pin_11, GPIO_Mode_AF_OD ,0);//IIC2复用引脚
	}
	
	assert_param( ClockSpeed <= 400000);//断言，只有debug下有效
	 
	I2C_InitTypeDefture.I2C_Ack = I2C_Ack_Enable;//应答使能
	I2C_InitTypeDefture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//器件地址为7位
	I2C_InitTypeDefture.I2C_ClockSpeed = ClockSpeed;//设置时钟频率，这个值不能高于 400KHz。
	I2C_InitTypeDefture.I2C_DutyCycle = I2C_DutyCycle_2;//快速模式下选择
	I2C_InitTypeDefture.I2C_Mode = I2C_Mode_I2C;//iic模式
	I2C_InitTypeDefture.I2C_OwnAddress1 = firstAddr;//第一个从设备地址（器件地址）一般试用于多个从机下。
	
	I2C_Init(I2Cx, &I2C_InitTypeDefture);
	
	I2C_Cmd(I2Cx, ENABLE);

}


//地址addr写入date
void IICWrite_byte(I2C_TypeDef* I2Cx, u16 addr , u8 date)
{
	u8 addrH;
	u8 addrL;
	if(addr > NOWAT24Cxx) //地址不能超出选址范围
	{		
		addr = addr % NOWAT24Cxx;			
	}	

	addrH  = addr>>8; //取出高位地址
	addrL = addr%256; //取出地位地址
	
	I2C_AcknowledgeConfig(I2Cx, ENABLE);//应答使能（从器件对主机的应答使能）
	I2C_GenerateSTART(I2Cx, ENABLE);//发起始信号，可以数据传输
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) ;//发送成功
	
	I2C_Send7bitAddress(I2Cx, WriteAddr, I2C_Direction_Transmitter);//（指定器件地址并为发送模式：0xA0，可变）
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) ;
	
	if(NOWAT24Cxx <= AT24C02)//地址只有一字节低位的EEPEOM
	{		
		I2C_SendData(I2Cx, addrL);//发送要写入的地址（一字节）
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//发送字节成功
	}
	else//地址有高地位两字节的的EEPEOM
	{

		I2C_SendData(I2Cx, addrH);//发送要写入的地址（一字节） 高位
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//发送字节成功
		
		I2C_SendData(I2Cx, addrL);//发送要写入的地址（一字节） 低位
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//发送字节成功

	}
	
	I2C_SendData(I2Cx, date);//发送数据   地址范围 0--255（0--0xFF）
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//发送字节成功
	
	//I2C_AcknowledgeConfig(I2Cx, DISABLE);//应答使能（从器件对主机的应答使能）
  I2C_GenerateSTOP(I2Cx, ENABLE);//发停止信号，停止数据传输

}

//先写入要读的地址，再读数据
u8 IICRead_byte(I2C_TypeDef* I2Cx, u16 addr)
{
	u8 RecieveDate = 0;
	u8 addrH;
	u8 addrL;
	if(addr > NOWAT24Cxx) //地址不能超出选址范围
	{		
		addr = addr % NOWAT24Cxx;			
	}	

	addrH  = addr>>8; //取出高位地址
	addrL = addr%256; //取出地位地址
	
	I2C_AcknowledgeConfig(I2Cx, ENABLE);//应答使能（从器件对主机的应答使能）
	I2C_GenerateSTART(I2Cx, ENABLE);//发起始信号，可以数据传输
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) ;//等待启动应答
	
	I2C_Send7bitAddress(I2Cx, WriteAddr, I2C_Direction_Transmitter);//伪写指令 （发送器件地址：0xA0，为发送，可变）
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) ;//等待发动模式应答
	
	if(NOWAT24Cxx <= AT24C02)//地址只有一字节低位的EEPEOM
	{
		I2C_SendData(I2Cx, addrL);//发送要写入的地址（一字节）低位
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//发送字节成功
	}
	else//地址有高地位两字节的的EEPEOM
	{
		I2C_SendData(I2Cx, addrH);//发送要写入的地址（一字节）高位
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//发送字节成功
		I2C_SendData(I2Cx, addrL);//发送要读数据的地址（一字节）低位
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//等待发送字节应答
	}
		I2C_GenerateSTART(I2Cx, ENABLE);//发启动信号
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) ;//等待启动应答
	
		I2C_Send7bitAddress(I2Cx, ReadAddr, I2C_Direction_Receiver);//（发送器件地址：0xA1，为接收，可变）
	  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) ;//等待接收模式应答
	
		I2C_AcknowledgeConfig(I2Cx, DISABLE);//失能应答
		I2C_GenerateSTOP(I2Cx, ENABLE);//发停止信号，停止数据传输
		
		while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED)) ;//等待数据被接收
		RecieveDate = I2C_ReceiveData(I2Cx);

	  return RecieveDate ;
}



//写入字符串(比较耗时，一个一个写)
void IICWrite_Str(I2C_TypeDef* I2Cx, u16 addr , u8* strDate, u16 length )
{

	
	while(length) //连续发送length个数据
	{	
		//while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)) ;
    IICWrite_byte(I2Cx, addr , *strDate);//一个一个的形式发送
		
		delay_ms(4);//一定得延时，否则程序会卡死
		
		addr++;
		strDate++;
		length--;	
	}
}


//读取指定长度字符串（I2Cx，首地址，长度，存储区）
void IICRead_Str(I2C_TypeDef* I2Cx, u16 addr ,u16 NumByteToRead, u8* StrBuffer)

{

	u8 addrH;
	u8 addrL;
	if(addr > NOWAT24Cxx) //地址不能超出选址范围
	{		
		addr = addr % NOWAT24Cxx;			
	}	

	addrH  = addr>>8; //取出高位地址
	addrL = addr%256; //取出地位地址
	
	I2C_AcknowledgeConfig(I2Cx, ENABLE);//应答使能（从器件对主机的应答使能）
	I2C_GenerateSTART(I2Cx, ENABLE);//发起始信号，可以数据传输
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) ;//等待启动应答
	
	I2C_Send7bitAddress(I2Cx, WriteAddr, I2C_Direction_Transmitter);//伪写指令 （发送器件地址：0xA0，为发送，可变）
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) ;//等待发动模式应答
	
	if(NOWAT24Cxx <= AT24C02)//地址只有一字节低位的EEPEOM
	{
		I2C_SendData(I2Cx, addrL);//发送要写入的地址（一字节）低位
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//发送字节成功
	}
	else//地址有高地位两字节的的EEPEOM
	{
		I2C_SendData(I2Cx, addrH);//发送要写入的地址（一字节）高位
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//发送字节成功
		I2C_SendData(I2Cx, addrL);//发送要读数据的地址（一字节）低位
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;//等待发送字节应答
	}
	
	I2C_GenerateSTART(I2Cx, ENABLE);//发启动信号
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) ;//等待启动应答

	I2C_Send7bitAddress(I2Cx, ReadAddr, I2C_Direction_Receiver);//（发送器件地址：0xA1，为接收，可变）
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) ;//等待接收模式应答

	
	while(NumByteToRead)
	{

		while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED)) ;//等待数据被接收
		*StrBuffer = I2C_ReceiveData(I2Cx);//只要没有发出stop指令，Addr会自动加1。可以连续读取。
		
		StrBuffer++;
		NumByteToRead--;
	}
	if(NumByteToRead == 0)//最后一位不进行自加地址
	{
			I2C_AcknowledgeConfig(I2Cx, DISABLE);//失能应答
			I2C_GenerateSTOP(I2Cx, ENABLE);//发停止信号，地址无法自加，数据传输停止
	}
		
}
/**

1：发器件地址（0xAx）之前得先发送启动信号STAR
2：只要主机有给从器件发送信号，从器件都会返给主机一个应答信号，所以每句发送的相关函数后都需要等待相关的应答。
3：使用接收数据函数之前，可以失能应答配置函数，因为该函数时从对主的应答，失能不影响
4：停止信号函数可以写在接收数据函数前面，也不影响
5：特殊情况：：等待数据被接收的应答写在接收数据函数前面
5：接收函数后不需要写主机对从的应答，因为这个应答集成到了固件库里（如果是模拟的IIC需要进行主对从应答）
6：器件的地址一般被分成好几页，因此如果写入字符串数据时，有两种方法。按页写数据，或者一个一个写数据：
  i：按页写数据，在while循环中，不断的I2C_SendData发送字节，全部发送完再STOP。中途需要判断是否地址到页尾的情况。
  ii：一个一个写，将字符串分成每个独立的字符，每个字符都需要经过：启动信号->发送->停止的过程，依次发送，无需判断页尾。
7：读取器件字符串的数据时，器件的分页对其无影响，可以连续的I2C_ReceiveData读取数据并且地址也会自动的自加，当发送STOP时，地址不自加停止接收
8:注意:一个地址只能存8位的数，即一字节，数的大小不会超过255。如果超过可以按高地位分别存两个地址里
*/





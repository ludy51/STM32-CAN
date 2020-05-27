#include "can.h"
#include "MyInit.h"


/*
函数描述：can初始化配置（包括对时钟和IO配置）
参数： CANx  CAN模式（环回模式和正常模式） 波特率有关的参数(tsjw,tbs1,tbs2,brp)
返回：初始化成功返回1，否则0
流程：
1：CAN初始化（环回模式和正常模式）
2：过滤器初始化（掩码模式和列表模式）

CAN波特率计算方法：
	CAN1位于APB1线上，时钟36M
	波特率=Fpclk1/((tsjw+tbs1+tbs2)*brp) = 36M/(1+7+8)*5 = 450Kbps;

*/

char CANInit(CAN_TypeDef* CANx  ,u8 CAN_Mode_xyz ,u8 tsjw,u8 tbs1,u8 tbs2 ,u8 brp)
{
	
	char StateFlag = 0;
	CAN_InitTypeDef CAN_InitStruct;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	//时钟和复用IO口配置
	if(CANx == CAN1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1  , ENABLE);
		
		GPIOInit(GPIOA , GPIO_Pin_11 , GPIO_Mode_IPU , 1 ); //can_RX   GPIO_Mode_IN_FLOATING
		GPIOInit(GPIOA , GPIO_Pin_12 , GPIO_Mode_AF_PP ,2); //can_TX
	}
	
	if(CANx == CAN2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2  , ENABLE);
		
			GPIOInit(GPIOB , GPIO_Pin_12 ,GPIO_Mode_IN_FLOATING ,1); //can_RX
		GPIOInit(GPIOB , GPIO_Pin_13 , GPIO_Mode_AF_PP , 1 ); //can_TX
	
	}
	
	CAN_DeInit(CANx);
	
	/////////////////CAN参数初始化//////////////////////
	CAN_InitStruct.CAN_ABOM = DISABLE;
	CAN_InitStruct.CAN_AWUM = DISABLE;
	CAN_InitStruct.CAN_Mode = CAN_Mode_xyz;//can模式   CAN_Mode_LoopBack;// CAN_Mode_Normal      
	CAN_InitStruct.CAN_NART = DISABLE;
	CAN_InitStruct.CAN_RFLM = DISABLE;
	CAN_InitStruct.CAN_TTCM = DISABLE;
	CAN_InitStruct.CAN_TXFP = DISABLE;	
	
	CAN_InitStruct.CAN_Prescaler = brp; //分频
	CAN_InitStruct.CAN_SJW = tsjw;//CAN_SJW_1tq;//同步时间	Tq
	CAN_InitStruct.CAN_BS1 = tbs1;//CAN_BS1_1tq;
	CAN_InitStruct.CAN_BS2 = tbs2;//CAN_BS2_4tq;

	

	StateFlag = CAN_Init(CANx, &CAN_InitStruct);//初始化成功返回1
	
	/////////////////过滤器初始化（掩码模式）//////////////////////
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;//使能过滤器
	CAN_FilterInitStruct.CAN_FilterNumber = 0;  //过滤器号，可选0--13（F103）
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_FilterFIFO0; //使用FIFO0，过滤器0关联到FIFO0（可选FIFO0和FIFO1）
	
	//能通过的标准ID号
	CAN_FilterInitStruct.CAN_FilterIdHigh =0xABCDEF98>>16;//0xABC<<4;//;  //标准ID不能为：1111111xxxx类型
	CAN_FilterInitStruct.CAN_FilterIdLow =0xABCDEF98&0x0000FFF8;// 0x00 ;
	
	/***
	1）过滤器初始化过程中标准过滤ID号的配置方法：
	
	如果是接收的数据是标准帧格式：
	
	标准帧ID占用位数为11位， 在发送函数中设置的标准帧ID（StdId）只需为低11位赋值即可，另外高5位可任意。
	
	过滤器的ID号与接收的标准帧ID是左对齐形式（即32位与11位的左对齐），因此过滤器的ID号的高11位有过滤的效果，其他位可设任意值。
	
	如：发送函数配置的标志帧ID：StdId=0xFA8B;则标准帧ID= 010 1000 1011（取最低的11位）
  如果在掩码模式的所有位都在检测的情况下，那么过滤器ID号高11位和标准帧ID应该一样，
	可以取:CAN_FilterIdHigh=0x516F=010 1000 1011 01111 ( CAN_FilterIdLow 任意）
	
	同理，如果是接收的数据是扩展帧格式：
	标准帧ID占用位数为29位，只需对ExtId的低29位赋值即可。
	过滤时和过滤ID好也是左对齐，因此过滤器的ID号的高29位有过滤的效果，其他位可设任意值。
	
  2）改进：由于以上给帧ID和过滤器ID赋值格式不统一，也不容易计算。为了统一并方便观察，
	对取标准/扩展帧ID和过滤器ID的赋值进行如下改进。（掩码模式对所有位都要关心的情况）
	
	如：现在需要通过标准帧ID为：0xABC（取前11位，最后一位取0，不作为标准ID位）。那么将其写入StdId为：
	StdId = 0xABC>>1; //取出11位
	CAN_FilterIdHigh取  CAN_FilterIdHigh= 0xABC<<4;  //移到最高位（使32位过滤ID与11位标准帧的左对齐）
	
	如：对于扩展帧ID，可设置为：0xABCDEF98（取前29位，最后3位取0，不作为标准ID位）那么将其写入ExtId为：
	ExtId = 0xABCDEF98>>3;（取出29位）
	CAN_FilterIdHigh = 0xABCDEF98>>16;   
	CAN_FilterIdLow = 0xABCDEF98&0x0000FFF8 ;//移到最高位（使32位过滤ID与29位标准帧的左对齐）
	
	3）需要注意：
	在标准帧下，设置掩码ID只能关心高11位，后25位不能关心。（掩码ID：0表不关心此位，1表关心此位）
	在扩展帧下，设置掩码ID只能关心高29位，后3位不能关心。
	***/
	
	//接收的ID号需要严格检测的位，该位不符合标准ID号相应的位，则不让通过
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0xFFFF ;//0x0000; //0表此位不关心
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0xFFF8 & 0xFFF8;//扩展帧下，掩码模式只能关心前29位，后3位不能关心
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;  //过滤器为掩码模式   //CAN_FilterMode_IdList为列表模式
	;
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;//过滤器为32位

	
	CAN_FilterInit(&CAN_FilterInitStruct); 
	
	
	return StateFlag;
	


}

/*
函数描述：can数据发送
参数： 发送的数据
返回：发送成功返回1
说明： 发送数据得配置发送数据的参数，将数据和相关参数写入结构体再发送

发送参数配置流程：
1：选择帧类型（标准数据帧 ，扩展数据帧，遥控帧）
（标准帧ID11位 扩展帧ID29位  遥控帧：只有ID无数据，请求指定ID发数据）
2：写入ID
3：写入数据（帧数据总长度64位，可最多一次性写入8个数据，每个数据只占1字节）
4：通过结构体把数据及ID参数发出去，并自动返回发出的邮箱号（发送邮箱一共有3个）
5：等待发送成功
*/
char CAN_TX_data(u8 *TXdata ,u8 DataLen)
{
	int i=0;
	u8 mailbox;
	CanTxMsg TxMessage;
	uint8_t TXdata1[8]={0};
	
	//设置为标准数据帧   （还有扩展数据帧  遥控帧）
	TxMessage.RTR = CAN_RTR_Data;//数据帧
	TxMessage.IDE =CAN_Id_Extended;//CAN_Id_Standard// CAN_Id_Standard;//使用标准帧id
	
	TxMessage.StdId = 0xABC>>1;//0x12;//标准帧ID
	TxMessage.ExtId = 0xABCDEF98>>3;//0x12;//扩展帧ID
	TxMessage.DLC = DataLen;//sizeof(TXdata)/sizeof(uint8_t); //需要一次性发送的数据个数（不超过8个）
	

	for(i =0 ;i < DataLen ; i++)//
	{
		TxMessage.Data[i] = TXdata[i];//Data个数小于8个，并且每个数据大小为1字节
//		printf("%d ",sizeof(TXdata));
	}
	/***
	知识点：
	   如果要获取一数组的长度（元素个数），可用sizeof(数组名)/sizeof(数组类型)。
	   但是！此法非任何情况下通用，当该数组作为一函数的实参，由指针形式传进来时，再通过sizeof(形参名)/sizeof(数组类型)
	   将无法获得正确长度，测试结果是：获取到的一般是实际一半。
			因此要获取数组长度，最好在定义数组的当前函数去获取，不通过形参传进后再获取，否则有错！（此函数为一例子）
	***/

	mailbox = CAN_Transmit(CAN1, &TxMessage);//将消息发送出去。返回值为发送出去的邮箱号
	

	//等待发送成功
	i = 0xfff;
	while(CAN_TransmitStatus(CAN1, mailbox) != CAN_TxStatus_Ok &&i) //获取该邮箱号的发送成功与否标志，一定的延迟防止死循环
	{
   	i--;
	}
	
	if(i==0)  return  0;  //发送失败 ，返回0
	else  return 1;  //发送成功，返回1
 //
}

/*
函数描述：can数据接收
参数： 接收的数据和参数的结构体   接收的数据部分
返回：接收成功返回1
说明： 接收数据存于结构体中，应对结构体进行解析读取。

接收流程：
1：等待有消息到达
2:将接收的消息（消息为结构体类型）存于指定FIFO（有2个FIFO，每个FIFO下有3个邮箱）
3:把消息的数据提出
4：将FIFO里的消息释放，避免堆积。

注意：函数定义形参：CanRxMsg  RecieveData; 应该为非指针形。否则会出现多一个字符的乱码现象。
*/
char CAN_RX_data(CanRxMsg  RecieveData , uint8_t *RXdata)
{
//	CanRxMsg  RecieveData1;
	int i = 0xfff;
	
	if(!CAN_MessagePending(CAN1, CAN_FIFO0)) //注意：CAN_FIFO0，不是CAN_FilterFIFO0
	{
		return  0;//没有数据接收 ，返回0
	}

	CAN_Receive(CAN1, CAN_FIFO0 , &RecieveData);//接收FIOFO_0下的邮箱（CAN1有两个FIFO，每个FIFO有3级邮箱）
	
	//把这次接收所有数据都提取并存起来
	for(i=0; i<RecieveData.DLC ;i++)
	{
		RXdata[i] =RecieveData.Data[i];
	}
	
//	printf("长度%d ",RecieveData.DLC);

	CAN_FIFORelease(CAN1, CAN_FilterFIFO0);  //释放FIFO_0邮箱的消息,以便接收新消息

	return 1;  //接收成功，返回1
}








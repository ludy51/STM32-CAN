#include "can.h"
#include "MyInit.h"


/*
����������can��ʼ�����ã�������ʱ�Ӻ�IO���ã�
������ CANx  CANģʽ������ģʽ������ģʽ�� �������йصĲ���(tsjw,tbs1,tbs2,brp)
���أ���ʼ���ɹ�����1������0
���̣�
1��CAN��ʼ��������ģʽ������ģʽ��
2����������ʼ��������ģʽ���б�ģʽ��

CAN�����ʼ��㷽����
	CAN1λ��APB1���ϣ�ʱ��36M
	������=Fpclk1/((tsjw+tbs1+tbs2)*brp) = 36M/(1+7+8)*5 = 450Kbps;

*/

char CANInit(CAN_TypeDef* CANx  ,u8 CAN_Mode_xyz ,u8 tsjw,u8 tbs1,u8 tbs2 ,u8 brp)
{
	
	char StateFlag = 0;
	CAN_InitTypeDef CAN_InitStruct;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	//ʱ�Ӻ͸���IO������
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
	
	/////////////////CAN������ʼ��//////////////////////
	CAN_InitStruct.CAN_ABOM = DISABLE;
	CAN_InitStruct.CAN_AWUM = DISABLE;
	CAN_InitStruct.CAN_Mode = CAN_Mode_xyz;//canģʽ   CAN_Mode_LoopBack;// CAN_Mode_Normal      
	CAN_InitStruct.CAN_NART = DISABLE;
	CAN_InitStruct.CAN_RFLM = DISABLE;
	CAN_InitStruct.CAN_TTCM = DISABLE;
	CAN_InitStruct.CAN_TXFP = DISABLE;	
	
	CAN_InitStruct.CAN_Prescaler = brp; //��Ƶ
	CAN_InitStruct.CAN_SJW = tsjw;//CAN_SJW_1tq;//ͬ��ʱ��	Tq
	CAN_InitStruct.CAN_BS1 = tbs1;//CAN_BS1_1tq;
	CAN_InitStruct.CAN_BS2 = tbs2;//CAN_BS2_4tq;

	

	StateFlag = CAN_Init(CANx, &CAN_InitStruct);//��ʼ���ɹ�����1
	
	/////////////////��������ʼ��������ģʽ��//////////////////////
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;//ʹ�ܹ�����
	CAN_FilterInitStruct.CAN_FilterNumber = 0;  //�������ţ���ѡ0--13��F103��
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_FilterFIFO0; //ʹ��FIFO0��������0������FIFO0����ѡFIFO0��FIFO1��
	
	//��ͨ���ı�׼ID��
	CAN_FilterInitStruct.CAN_FilterIdHigh =0xABCDEF98>>16;//0xABC<<4;//;  //��׼ID����Ϊ��1111111xxxx����
	CAN_FilterInitStruct.CAN_FilterIdLow =0xABCDEF98&0x0000FFF8;// 0x00 ;
	
	/***
	1����������ʼ�������б�׼����ID�ŵ����÷�����
	
	����ǽ��յ������Ǳ�׼֡��ʽ��
	
	��׼֡IDռ��λ��Ϊ11λ�� �ڷ��ͺ��������õı�׼֡ID��StdId��ֻ��Ϊ��11λ��ֵ���ɣ������5λ�����⡣
	
	��������ID������յı�׼֡ID���������ʽ����32λ��11λ������룩����˹�������ID�ŵĸ�11λ�й��˵�Ч��������λ��������ֵ��
	
	�磺���ͺ������õı�־֡ID��StdId=0xFA8B;���׼֡ID= 010 1000 1011��ȡ��͵�11λ��
  ���������ģʽ������λ���ڼ�������£���ô������ID�Ÿ�11λ�ͱ�׼֡IDӦ��һ����
	����ȡ:CAN_FilterIdHigh=0x516F=010 1000 1011 01111 ( CAN_FilterIdLow ���⣩
	
	ͬ������ǽ��յ���������չ֡��ʽ��
	��׼֡IDռ��λ��Ϊ29λ��ֻ���ExtId�ĵ�29λ��ֵ���ɡ�
	����ʱ�͹���ID��Ҳ������룬��˹�������ID�ŵĸ�29λ�й��˵�Ч��������λ��������ֵ��
	
  2���Ľ����������ϸ�֡ID�͹�����ID��ֵ��ʽ��ͳһ��Ҳ�����׼��㡣Ϊ��ͳһ������۲죬
	��ȡ��׼/��չ֡ID�͹�����ID�ĸ�ֵ�������¸Ľ���������ģʽ������λ��Ҫ���ĵ������
	
	�磺������Ҫͨ����׼֡IDΪ��0xABC��ȡǰ11λ�����һλȡ0������Ϊ��׼IDλ������ô����д��StdIdΪ��
	StdId = 0xABC>>1; //ȡ��11λ
	CAN_FilterIdHighȡ  CAN_FilterIdHigh= 0xABC<<4;  //�Ƶ����λ��ʹ32λ����ID��11λ��׼֡������룩
	
	�磺������չ֡ID��������Ϊ��0xABCDEF98��ȡǰ29λ�����3λȡ0������Ϊ��׼IDλ����ô����д��ExtIdΪ��
	ExtId = 0xABCDEF98>>3;��ȡ��29λ��
	CAN_FilterIdHigh = 0xABCDEF98>>16;   
	CAN_FilterIdLow = 0xABCDEF98&0x0000FFF8 ;//�Ƶ����λ��ʹ32λ����ID��29λ��׼֡������룩
	
	3����Ҫע�⣺
	�ڱ�׼֡�£���������IDֻ�ܹ��ĸ�11λ����25λ���ܹ��ġ�������ID��0�����Ĵ�λ��1����Ĵ�λ��
	����չ֡�£���������IDֻ�ܹ��ĸ�29λ����3λ���ܹ��ġ�
	***/
	
	//���յ�ID����Ҫ�ϸ����λ����λ�����ϱ�׼ID����Ӧ��λ������ͨ��
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0xFFFF ;//0x0000; //0���λ������
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0xFFF8 & 0xFFF8;//��չ֡�£�����ģʽֻ�ܹ���ǰ29λ����3λ���ܹ���
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;  //������Ϊ����ģʽ   //CAN_FilterMode_IdListΪ�б�ģʽ
	;
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;//������Ϊ32λ

	
	CAN_FilterInit(&CAN_FilterInitStruct); 
	
	
	return StateFlag;
	


}

/*
����������can���ݷ���
������ ���͵�����
���أ����ͳɹ�����1
˵���� �������ݵ����÷������ݵĲ����������ݺ���ز���д��ṹ���ٷ���

���Ͳ����������̣�
1��ѡ��֡���ͣ���׼����֡ ����չ����֡��ң��֡��
����׼֡ID11λ ��չ֡ID29λ  ң��֡��ֻ��ID�����ݣ�����ָ��ID�����ݣ�
2��д��ID
3��д�����ݣ�֡�����ܳ���64λ�������һ����д��8�����ݣ�ÿ������ֻռ1�ֽڣ�
4��ͨ���ṹ������ݼ�ID��������ȥ�����Զ����ط���������ţ���������һ����3����
5���ȴ����ͳɹ�
*/
char CAN_TX_data(u8 *TXdata ,u8 DataLen)
{
	int i=0;
	u8 mailbox;
	CanTxMsg TxMessage;
	uint8_t TXdata1[8]={0};
	
	//����Ϊ��׼����֡   ��������չ����֡  ң��֡��
	TxMessage.RTR = CAN_RTR_Data;//����֡
	TxMessage.IDE =CAN_Id_Extended;//CAN_Id_Standard// CAN_Id_Standard;//ʹ�ñ�׼֡id
	
	TxMessage.StdId = 0xABC>>1;//0x12;//��׼֡ID
	TxMessage.ExtId = 0xABCDEF98>>3;//0x12;//��չ֡ID
	TxMessage.DLC = DataLen;//sizeof(TXdata)/sizeof(uint8_t); //��Ҫһ���Է��͵����ݸ�����������8����
	

	for(i =0 ;i < DataLen ; i++)//
	{
		TxMessage.Data[i] = TXdata[i];//Data����С��8��������ÿ�����ݴ�СΪ1�ֽ�
//		printf("%d ",sizeof(TXdata));
	}
	/***
	֪ʶ�㣺
	   ���Ҫ��ȡһ����ĳ��ȣ�Ԫ�ظ�����������sizeof(������)/sizeof(��������)��
	   ���ǣ��˷����κ������ͨ�ã�����������Ϊһ������ʵ�Σ���ָ����ʽ������ʱ����ͨ��sizeof(�β���)/sizeof(��������)
	   ���޷������ȷ���ȣ����Խ���ǣ���ȡ����һ����ʵ��һ�롣
			���Ҫ��ȡ���鳤�ȣ�����ڶ�������ĵ�ǰ����ȥ��ȡ����ͨ���βδ������ٻ�ȡ�������д����˺���Ϊһ���ӣ�
	***/

	mailbox = CAN_Transmit(CAN1, &TxMessage);//����Ϣ���ͳ�ȥ������ֵΪ���ͳ�ȥ�������
	

	//�ȴ����ͳɹ�
	i = 0xfff;
	while(CAN_TransmitStatus(CAN1, mailbox) != CAN_TxStatus_Ok &&i) //��ȡ������ŵķ��ͳɹ�����־��һ�����ӳٷ�ֹ��ѭ��
	{
   	i--;
	}
	
	if(i==0)  return  0;  //����ʧ�� ������0
	else  return 1;  //���ͳɹ�������1
 //
}

/*
����������can���ݽ���
������ ���յ����ݺͲ����Ľṹ��   ���յ����ݲ���
���أ����ճɹ�����1
˵���� �������ݴ��ڽṹ���У�Ӧ�Խṹ����н�����ȡ��

�������̣�
1���ȴ�����Ϣ����
2:�����յ���Ϣ����ϢΪ�ṹ�����ͣ�����ָ��FIFO����2��FIFO��ÿ��FIFO����3�����䣩
3:����Ϣ���������
4����FIFO�����Ϣ�ͷţ�����ѻ���

ע�⣺���������βΣ�CanRxMsg  RecieveData; Ӧ��Ϊ��ָ���Ρ��������ֶ�һ���ַ�����������
*/
char CAN_RX_data(CanRxMsg  RecieveData , uint8_t *RXdata)
{
//	CanRxMsg  RecieveData1;
	int i = 0xfff;
	
	if(!CAN_MessagePending(CAN1, CAN_FIFO0)) //ע�⣺CAN_FIFO0������CAN_FilterFIFO0
	{
		return  0;//û�����ݽ��� ������0
	}

	CAN_Receive(CAN1, CAN_FIFO0 , &RecieveData);//����FIOFO_0�µ����䣨CAN1������FIFO��ÿ��FIFO��3�����䣩
	
	//����ν����������ݶ���ȡ��������
	for(i=0; i<RecieveData.DLC ;i++)
	{
		RXdata[i] =RecieveData.Data[i];
	}
	
//	printf("����%d ",RecieveData.DLC);

	CAN_FIFORelease(CAN1, CAN_FilterFIFO0);  //�ͷ�FIFO_0�������Ϣ,�Ա��������Ϣ

	return 1;  //���ճɹ�������1
}








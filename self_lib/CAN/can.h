#ifndef _can_H_
#define _can_H_

#include "stm32f10x.h"

char CANInit(CAN_TypeDef* CANx  ,u8 CAN_Mode_xyz ,u8 tsjw,u8 tbs1,u8 tbs2 ,u8 brp);
char CAN_TX_data(u8 *TXdata ,u8 DataLen);
char CAN_RX_data(CanRxMsg RecieveData , uint8_t *RXdata);


#endif

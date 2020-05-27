/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "includes.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
	
//=====================函数名可在启动文件.S中查到=============================================//

//void EXTI1_IRQHandler(void)
//{
//	//中断里再加入按键判断，更稳定
//	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
//	{
//		delay_ms(10);
//		while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
//		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
//		{
//			EXTI_ClearITPendingBit(EXTI_Line1);
//			
//			RTC_SetAlarm(RTC_GetCounter()+4);
//			RTC_ITConfig(RTC_IT_ALR, ENABLE);	//使能闹钟中断.  秒中断RTC_IT_SEC
//			RTC_WaitForLastTask();//等待上一次写RTC任务完成
//			
//			Enter_Stop();             //进入停机状态
//		}
//	}
//	
//}
char flag1 = 0;
void RTC_IRQHandler(void)
{
	if(RTC_GetFlagStatus(RTC_IT_SEC))
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);
		flag1 = !flag1;
		//GPIO_WriteBit(GPIOA, GPIO_Pin_5, flag1);
	}


}
char Wakeflag=0;
void RTCAlarm_IRQHandler(void)//闹钟中断唤醒
{
	if(RTC_GetFlagStatus(RTC_IT_ALR))
	{
	EXTI_ClearITPendingBit(EXTI_Line17);//！！切记，闹钟中断比较特别，不仅要开闹钟中断而且还要开17号线中断，清标志位也一样
	RTC_ClearITPendingBit(RTC_IT_ALR);	
		RTC_WaitForLastTask(); 
						EXTI_ClearITPendingBit(EXTI_Line7);
				EXTI_ClearITPendingBit(EXTI_Line1);//（！！停机模式下，对于程序可能产生的标志位必须的清除干净，不清除会出现唤醒失灵现象！！）

					  if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
        {
					PWR_ClearFlag(PWR_FLAG_WU);//一般没用
        }
				SystemInit();//（！！停机模式下，由于停机下对所有时钟关闭，所以唤醒需要重新配置时钟！！）
	
	}
	Wakeflag=!Wakeflag;
}



//void EXTI9_5_IRQHandler(void)//外部中断唤醒
//{
//	//中断里再加入按键判断，更稳定
//	if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))
//	{
//		delay_ms(10);
//		while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7));
//		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))
//		{
//			  if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
//        {
//					PWR_ClearFlag(PWR_FLAG_WU);//一般没用
//        }
//				EXTI_ClearITPendingBit(EXTI_Line7);
//				EXTI_ClearITPendingBit(EXTI_Line1);//对于程序可能产生的标志位必须的清除干净，不清除会出现唤醒失灵现象！！
//				
//				SystemInit();//重要，由于停机下对所有时钟关闭，所以唤醒需要重新配置时钟！！
//				
//			Wakeflag=!Wakeflag;

//		}
//	}
//	
//}



extern u16 DelayTime;
void SysTick_Handler(void)
{
//	if(DelayTime != 0)
//	{
//	DelayTime--;
//	}
//	
}













void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

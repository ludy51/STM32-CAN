/*
    FreeRTOS V7.0.2 - Copyright (C) 2011 Real Time Engineers Ltd.
	

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/




#define STM32F103C8
#define configUSE_PREEMPTION		           1
#define configUSE_IDLE_HOOK			           0
#define configUSE_TICK_HOOK			           0

//#if defined (STM32F101RC)  
// #define configCPU_CLOCK_HZ			           	( ( unsigned long ) 36000000 )
// #define configTOTAL_HEAP_SIZE		           	( ( size_t ) ( 20 * 1024 ) )
//#elif defined (STM32F103VB)
// #define configCPU_CLOCK_HZ			           	( ( unsigned long ) 72000000 )
// #define configTOTAL_HEAP_SIZE		           	( ( size_t ) ( 12 * 1024 ) )
//#elif defined (STM32F103V8)
// #define configCPU_CLOCK_HZ			           	( ( unsigned long ) 72000000 )
// #define configTOTAL_HEAP_SIZE		           	( ( size_t ) ( 12 * 1024 ) )
//#elif defined (STM32F103VC)
// #define configCPU_CLOCK_HZ			           	( ( unsigned long ) 72000000 )
// //#define configTOTAL_HEAP_SIZE		           ( ( size_t ) ( 43 * 1024 ) )		// ����֤��43K, �������ǲ�������
// #define configTOTAL_HEAP_SIZE		           	( ( size_t ) ( 44 * 1024 ) )			// ��������ʹ�õ���44K
// //#define configTOTAL_HEAP_SIZE		           ( ( size_t ) ( 45 * 1024 ) )		// 45K, ���������벻ͨ��					
//#elif defined (STM32F103VE)
// #define configCPU_CLOCK_HZ			           	( ( unsigned long ) 72000000 )
// //#define configTOTAL_HEAP_SIZE		           ( ( size_t ) ( 56 * 1024 ) )		// ����֤��56K, �������ǲ�������
// #define configTOTAL_HEAP_SIZE		           	( ( size_t ) ( 60 * 1024 ) )
//#elif defined (STM32F103VG)
// #define configCPU_CLOCK_HZ			           	( ( unsigned long ) 72000000 )
// #define configTOTAL_HEAP_SIZE		           	( ( size_t ) ( 90 * 1024 ) )
//#elif defined (STM32F107VC)
// #define configCPU_CLOCK_HZ			           	( ( unsigned long ) 72000000 )
// #define configTOTAL_HEAP_SIZE		           	( ( size_t ) ( 60 * 1024 ) )
//#elif defined (STM32F100C8)
// #define configCPU_CLOCK_HZ			           	( ( unsigned long ) 24000000 )
// #define configTOTAL_HEAP_SIZE		           	( ( size_t ) ( 13 * 512 ) )
#if defined (STM32F103C8)
 #define configCPU_CLOCK_HZ			           	( ( unsigned long ) 72000000 )
//#if defined (HEAP_SIZE_16K)
 #define configTOTAL_HEAP_SIZE		           	( ( size_t ) ( 16 * 1024 ) )
//#elif defined (HEAP_SIZE_18K)
 //#define configTOTAL_HEAP_SIZE		           	( ( size_t ) ( 18 * 1024 ) )			// DTU ʹ��18K, 16K��������
//#else
 //#error "Please configure HEAP SIZE. (in Keil uVision4 Options->C/C++)"
//#endif
//#else 
 //#error "Please configure MCU Type. (in Keil uVision4 Options->C/C++)"
#endif

	
#define configTICK_RATE_HZ			            		( ( portTickType ) 1000 )
#define configMAX_PRIORITIES		            		( ( unsigned portBASE_TYPE ) 5 )
#define configMINIMAL_STACK_SIZE	            	( ( unsigned short ) 64 )
#define configMAX_TASK_NAME_LEN		            	( 16 )
#define configUSE_TRACE_FACILITY	            	0
#define configUSE_16_BIT_TICKS		            	0
#define configIDLE_SHOULD_YIELD		            	1

#define configUSE_MUTEXES                       1   /*�Ƿ���MUTEXES��MUTEXES�������ͨѶ��һ�ַ�ʽ���ر���������������Դ��Ӧ�ã������ӡ��������A�õ�ʱ����ų�������Ӧ�ã������˱������ſ���Ӧ�á� */
#define configUSE_RECURSIVE_MUTEXES             0   // ȷ���Ƿ��õݹ�ʽ��MUTEXES
#define configUSE_COUNTING_SEMAPHORES           0   //�Ƿ��ü���ʽ��SEMAPHORES��SEMAPHORESҲ�������ͨѶ��һ�ַ�ʽ
#define configUSE_ALTERNATIVE_API               0   /*�Ƿ�Ӧ�ÿ��л�ʽ��API��freeRTOS ͬһ����API�ж������ȫ���ܵ���������Դ��ʱ��϶�ģ�����ʹ�ܺ�Ϳ����ýϼ򵥵�API�� ��ʡ��Դ��ʱ�䣬����Ӧ�����ƽ϶� */
#define configCHECK_FOR_STACK_OVERFLOW          0   /*��������DEBUG�������Ƿ���ջ�������Ҫ���Լ�����Ӧ��麯��void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName ) */
#define configQUEUE_REGISTRY_SIZE               10  /*����DEBUG���Ǽ�SEMAPHORESQ��QUEUE������������Ҫ��������Ӧ�ú���vQueueAddToRegistry()��vQueueUnregisterQueue() */

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet                0   //�趨���Ըı��������ȶ�
#define INCLUDE_uxTaskPriorityGet               1   //�趨���Բ�ѯ�������ȶ�
#define INCLUDE_vTaskDelete                     1   //�趨����ɾ������
#define INCLUDE_vTaskCleanUpResources           1   //��˵�ǿ��Ի���ɾ����������Դ(RAM��)
#define INCLUDE_vTaskSuspend                    1   //���ÿ��԰��������
#define INCLUDE_vResumeFromISR                  0   //���ÿ��Դ��жϻָ�(����ϵͳ˯�ߣ����жϻ���)
#define INCLUDE_vTaskDelayUntil                 1   //���������ӳٵľ���ʱ�䣬 ��������4��30���ӳٵ�5��00��ʱ�䶼�Ǿ���ʱ��
#define INCLUDE_vTaskDelay                      1   //����������ʱ�� �����ӳ�30���ӣ���Ե�ʱ�䣬����ʲôʱ�䣬����Ҫ֪��
#define INCLUDE_xTaskGetSchedulerState          0   //����ȡ�õ�ǰ�����������״̬
#define INCLUDE_xTaskGetCurrentTaskHandle       0   //���õ�ǰ���������ĸ���������
#define INCLUDE_uxTaskGetStackHighWaterMark     0   //�Ƿ�ʹ����һ������������Ŀ���Ƿ�������ִ�к������ջ����Сδ��������ͬ����Ϊ��ֹ��ջ���

/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */
#define configKERNEL_INTERRUPT_PRIORITY 				255
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 		191 /* equivalent to 0xb0, or priority 11. */


/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15

#endif /* FREERTOS_CONFIG_H */


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
// //#define configTOTAL_HEAP_SIZE		           ( ( size_t ) ( 43 * 1024 ) )		// 现在证明43K, 集中器是不死机的
// #define configTOTAL_HEAP_SIZE		           	( ( size_t ) ( 44 * 1024 ) )			// 现在正在使用的是44K
// //#define configTOTAL_HEAP_SIZE		           ( ( size_t ) ( 45 * 1024 ) )		// 45K, 集中器编译不通过					
//#elif defined (STM32F103VE)
// #define configCPU_CLOCK_HZ			           	( ( unsigned long ) 72000000 )
// //#define configTOTAL_HEAP_SIZE		           ( ( size_t ) ( 56 * 1024 ) )		// 现在证明56K, 集中器是不死机的
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
 //#define configTOTAL_HEAP_SIZE		           	( ( size_t ) ( 18 * 1024 ) )			// DTU 使用18K, 16K启动不了
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

#define configUSE_MUTEXES                       1   /*是否用MUTEXES。MUTEXES是任务间通讯的一种方式，特别是用于任务共享资源的应用，比如打印机，任务A用的时候就排斥别的任务应用，用完了别的任务才可以应用。 */
#define configUSE_RECURSIVE_MUTEXES             0   // 确定是否用递归式的MUTEXES
#define configUSE_COUNTING_SEMAPHORES           0   //是否用计数式的SEMAPHORES，SEMAPHORES也是任务间通讯的一种方式
#define configUSE_ALTERNATIVE_API               0   /*是否应用可切换式的API。freeRTOS 同一功能API有多个，有全功能但是需求资源和时间较多的，此项使能后就可以用较简单的API， 节省资源和时间，但是应用限制较多 */
#define configCHECK_FOR_STACK_OVERFLOW          0   /*此项用于DEBUG，来看是否有栈溢出，需要你自己编相应检查函数void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName ) */
#define configQUEUE_REGISTRY_SIZE               10  /*用于DEBUG，登记SEMAPHORESQ和QUEUE的最大个数，需要在任务用应用函数vQueueAddToRegistry()和vQueueUnregisterQueue() */

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet                0   //设定可以改变任务优先度
#define INCLUDE_uxTaskPriorityGet               1   //设定可以查询任务优先度
#define INCLUDE_vTaskDelete                     1   //设定可以删除任务
#define INCLUDE_vTaskCleanUpResources           1   //据说是可以回收删除任务后的资源(RAM等)
#define INCLUDE_vTaskSuspend                    1   //设置可以把任务挂起
#define INCLUDE_vResumeFromISR                  0   //设置可以从中断恢复(比如系统睡眠，由中断唤醒)
#define INCLUDE_vTaskDelayUntil                 1   //设置任务延迟的绝对时间， 比如现在4：30，延迟到5：00。时间都是绝对时间
#define INCLUDE_vTaskDelay                      1   //设置任务延时， 比如延迟30分钟，相对的时间，现在什么时间，不需要知道
#define INCLUDE_xTaskGetSchedulerState          0   //设置取得当前任务分配器的状态
#define INCLUDE_xTaskGetCurrentTaskHandle       0   //设置当前任务是由哪个任务开启的
#define INCLUDE_uxTaskGetStackHighWaterMark     0   //是否使能这一函数，函数的目的是返回任务执行后任务堆栈的最小未用数量，同样是为防止堆栈溢出

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


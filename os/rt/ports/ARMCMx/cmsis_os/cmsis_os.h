/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013,2014 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
   Concepts and parts of this file have been contributed by Andre R.
 */

/**
 * @file    cmsis_os.h
 * @brief   CMSIS RTOS module macros and structures.
 *
 * @addtogroup CMSIS_OS
 * @{
 */

#ifndef _CMSIS_OS_H_
#define _CMSIS_OS_H_

#include "ch.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   API version.
 */
#define osCMSIS                     0x10002

/**
 * @brief   Kernel version.
 */
#define osKernelSystemId            "KERNEL V1.00"

/**
 * @brief   ChibiOS/RT version encoded for CMSIS.
 */
#define osCMSIS_KERNEL              ((CH_KERNEL_MAJOR << 16) |              \
                                     (CH_KERNEL_MINOR << 8) |               \
                                     (CH_KERNEL_PATCH))

/**
 * @name    CMSIS Capabilities
 * @{
 */
#define osFeature_MainThread        1       
#define osFeature_Pool              0
#define osFeature_MailQ             0
#define osFeature_MessageQ          0
#define osFeature_Signals           31
#define osFeature_Semaphore         ((1U << 31) - 1U)
#define osFeature_Wait              0
#define osFeature_SysTick           1
/**< @} */

/**
 * @brief   Wait forever specification for timeouts.
 */
#define osWaitForever               TIME_INFINITE

/**
 * @brief   System tick frequency.
 */
#define osKernelSysTickFrequency    CH_CFG_FREQUENCY

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of priority levels.
 */
typedef enum {
  osPriorityIdle            = -3,
  osPriorityLow             = -2,
  osPriorityBelowNormal     = -1,
  osPriorityNormal          = 0,
  osPriorityAboveNormal     = +1,
  osPriorityHigh            = +2,
  osPriorityRealtime        = +3,
  osPriorityError           = 0x84
} osPriority;

/**
 * @brief   Type of error codes.
 */
typedef enum {
  osOK                      = 0,
  osEventSignal             = 0x08,
  osEventMessage            = 0x10,
  osEventMail               = 0x20,
  osEventTimeout            = 0x40,
  osErrorParameter          = 0x80,
  osErrorResource           = 0x81,
  osErrorTimeoutResource    = 0xC1,
  osErrorISR                = 0x82,
  osErrorISRRecursive       = 0x83,
  osErrorPriority           = 0x84,
  osErrorNoMemory           = 0x85,
  osErrorValue              = 0x86,
  osErrorOS                 = 0xFF,
  os_status_reserved        = 0x7FFFFFFF
} osStatus;

/**
 * @brief   Type of a timer mode.
 */
typedef enum  {
  osTimerOnce               =   0,
  osTimerPeriodic           =   1
} os_timer_type;

/**
 * @brief   Type of thread functions.
 */
typedef void (*os_pthread) (void const *argument);

/**
 * @brief   Type of timer callback.
 */
typedef void (*os_ptimer) (void const *argument);

/**
 * @brief   Type of pointer to thread control block.
 */
typedef struct os_thread_cb *osThreadId;

/**
 * @brief   Type of pointer to timer control block.
 */
typedef struct os_timer_cb *osTimerId;

/**
 * @brief   Type of pointer to mutex control block.
 */
typedef struct os_mutex_cb *osMutexId;

/**
 * @brief   Type of pointer to semaphore control block.
 */
typedef struct os_semaphore_cb *osSemaphoreId;

/**
 * @brief   Type of a thread definition block.
 */
typedef struct os_thread_def  {
  os_pthread                pthread;
  osPriority                tpriority;
  uint32_t                  stacksize;
} osThreadDef_t;

/**
 * @brief   Type of a timer definition block.
 */
typedef struct os_timer_def  {
  os_ptimer                 ptimer;
} osTimerDef_t;

/**
 * @brief   Type of a mutex definition block.
 */
typedef struct os_mutex_def  {
  uint32_t                  dummy;
} osMutexDef_t;

/**
 * @brief   Type of a semaphore definition block.
 */
typedef struct os_semaphore_def  {
  uint32_t                  dummy;
} osSemaphoreDef_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Convert a microseconds value to a RTOS kernel system timer value.
 */
#define osKernelSysTickMicroSec(microsec) (((uint64_t)microsec *            \
                                            (osKernelSysTickFrequency)) /   \
                                           1000000)

/**
 * @brief   Create a Thread definition.
 */
#if defined(osObjectsExternal)
#define osThreadDef(name, priority, instances, stacksz)                     \
  extern const osThreadDef_t os_thread_def_##name
#else
#define osThreadDef(name, priority, instances, stacksz)                     \
const osThreadDef_t os_thread_def_##name = {                                \
  (name),                                                                   \
  (priority),                                                               \
  (stacksz)                                                                 \
}
#endif

/**
 * @brief   Access a Thread definition.
 */
#define osThread(name)  &os_thread_def_##name

/**
 * @brief   Define a Timer object.
 */
#if defined(osObjectsExternal)
#define osTimerDef(name, function)                                          \
  extern const osTimerDef_t os_timer_def_##name
#else
#define osTimerDef(name, function)                                          \
const osTimerDef_t os_timer_def_##name = {                                  \
  (function)                                                                \
}
#endif

/**
 * @brief   Access a Timer definition.
 */
#define osTimer(name) &os_timer_def_##name

/**
 * @brief   Define a Mutex.
 */
#if defined (osObjectsExternal)
#define osMutexDef(name) extern const osMutexDef_t os_mutex_def_##name
#else
#define osMutexDef(name)  const osMutexDef_t os_mutex_def_##name = {0}
#endif

/**
 * @brief   Access a Mutex definition.
 */
#define osMutex(name) &os_mutex_def_##name

/**
 * @brief   Define a Semaphore.
 */
#if defined (osObjectsExternal)
#define osSemaphoreDef(name)                                                \
  extern const osSemaphoreDef_t os_semaphore_def_##name
#else                            // define the object
#define osSemaphoreDef(name)                                                \
  const osSemaphoreDef_t os_semaphore_def_##name = {0}
#endif

/**
 * @brief   Access a Semaphore definition.
 */
#define osSemaphore(name) &os_semaphore_def_##name

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  osThreadId osThreadGetId(void);
  osStatus osThreadTerminate(osThreadId thread_id);
  osStatus osThreadYield(void);
  osStatus osThreadSetPriority(osThreadId thread_id, osPriority priority);
  osPriority osThreadGetPriority(osThreadId thread_id);
  osStatus osDelay(uint32_t millisec);
  /*osEvent osWait(uint32_t millisec);*/
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Kernel initialization.
 */
static inline osStatus osKernelInitialize(void) {

  chSysSuspend();

  return osOK;
}

/**
 * @brief   Kernel start.
 * @note    Does nothing, under ChibiOS/RT there is no concept of starting the
 *          kernel.
 */
static inline osStatus osKernelStart(void) {

  return osOK;
}

/**
 * @brief   To be or not to be.
 */
static inline int32_t osKernelRunning(void) {

  if (ch.rlist.r_queue.p_next != NULL)
      return 1;

  return 0;
}

/**
 * @brief   System ticks since start.
 */
static inline uint32_t osKernelSysTick(void) {

  return (uint32_t)chVTGetSystemTimeX();
}

/**
 * @brief   Creates a thread.
 */
static inline osThreadId osThreadCreate (osThreadDef_t *thread_def,
                                         void *argument) {

  return (osThreadId)chThdCreateFromHeap(0,
                                         THD_WORKING_AREA_SIZE(thread_def->stacksize),
                                         NORMALPRIO+thread_def->tpriority,
                                         (tfunc_t)thread_def->pthread,
                                         argument);
}

#endif /* _CMSIS_OS_H_ */

/** @} */

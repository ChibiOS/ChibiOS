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

/**
 * @brief   Number of pre-allocated static semaphores/mutexes.
 */
#if !defined(CMSIS_CFG_NUM_SEMAPHORES)
#define CMSIS_CFG_NUM_SEMAPHORES    4
#endif

/**
 * @brief   Number of pre-allocated static timers.
 */
#if !defined(CMSIS_CFG_NUM_TIMERS)
#define CMSIS_CFG_NUM_TIMERS        4
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !CH_CFG_USE_MEMPOOLS
#error "CMSIS RTOS requires CH_CFG_USE_MEMPOOLS"
#endif

#if !CH_CFG_USE_SEMAPHORES
#error "CMSIS RTOS requires CH_CFG_USE_SEMAPHORES"
#endif

#if !CH_CFG_USE_DYNAMIC
#error "CMSIS RTOS requires CH_CFG_USE_DYNAMIC"
#endif

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
  osTimerOnce               = 0,
  osTimerPeriodic           = 1
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
typedef thread_t *osThreadId;

/**
 * @brief   Type of pointer to timer control block.
 */
typedef struct os_timer_cb {
  virtual_timer_t           vt;
  os_timer_type             type;
  os_ptimer                 ptimer;
  void                      *argument;
  uint32_t                  millisec;
} *osTimerId;

/**
 * @brief   Type of pointer to mutex control block.
 */
typedef binary_semaphore_t *osMutexId;

/**
 * @brief   Type of pointer to semaphore control block.
 */
typedef semaphore_t *osSemaphoreId;

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

extern int32_t cmsis_os_started;

#ifdef __cplusplus
extern "C" {
#endif
  osStatus osKernelInitialize(void);
  osStatus osKernelStart(void);
  osStatus osThreadSetPriority(osThreadId thread_id, osPriority newprio);
  /*osEvent osWait(uint32_t millisec);*/
  osTimerId osTimerCreate (const osTimerDef_t *timer_def,
                           os_timer_type type,
                           void *argument);
  osStatus osTimerStart (osTimerId timer_id, uint32_t millisec);
  osStatus osTimerStop (osTimerId timer_id);
  osStatus osTimerDelete (osTimerId timer_id);
  osSemaphoreId osSemaphoreCreate (const osSemaphoreDef_t *semaphore_def,
                                   int32_t count);
  int32_t osSemaphoreWait (osSemaphoreId semaphore_id, uint32_t millisec);
  osStatus osSemaphoreRelease (osSemaphoreId semaphore_id);
  osStatus osSemaphoreDelete (osSemaphoreId semaphore_id);
  osMutexId osMutexCreate (const osMutexDef_t *mutex_def);
  osStatus osMutexWait (osMutexId mutex_id, uint32_t millisec);
  osStatus osMutexRelease (osMutexId mutex_id);
  osStatus osMutexDelete (osMutexId mutex_id);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   To be or not to be.
 */
static inline int32_t osKernelRunning(void) {

  return cmsis_os_started;
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

/**
 * @brief   Returns the current thread.
 */
static inline osThreadId osThreadGetId(void) {

  return (osThreadId)chThdGetSelfX();
}

/**
 * @brief   Thread termination.
 * @note    The thread is not really terminated but asked to terminate which
 *          is not compliant.
 */
static inline osStatus osThreadTerminate(osThreadId thread_id) {

  chThdTerminate(thread_id);

  return osOK;
}

/**
 * @brief   Thread time slice yield.
 */
static inline osStatus osThreadYield(void) {

  chThdYield();

  return osOK;
}

/**
 * @brief   Returns priority of a thread.
 */
static inline osPriority osThreadGetPriority(osThreadId thread_id) {

  return thread_id->p_prio;
}

/**
 * @brief   Thread delay in milliseconds.
 */
static inline osStatus osDelay(uint32_t millisec) {

  chThdSleepMilliseconds(millisec);

  return osOK;
}

#endif /* _CMSIS_OS_H_ */

/** @} */

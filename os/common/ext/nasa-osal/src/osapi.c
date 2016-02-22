/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    osapi.c
 * @brief   OS API module code.
 *
 * @addtogroup nasa_osapi
 * @{
 */

#include <string.h>

#include "ch.h"

#include "common_types.h"
#include "osapi.h"

#if CH_CFG_USE_MUTEXES == FALSE
#error "NASA OSAL requires CH_CFG_USE_MUTEXES"
#endif

#if CH_CFG_USE_SEMAPHORES == FALSE
#error "NASA OSAL requires CH_CFG_USE_SEMAPHORES"
#endif

#if CH_CFG_USE_REGISTRY == FALSE
#error "NASA OSAL requires CH_CFG_USE_REGISTRY"
#endif

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

#define MIN_PRIORITY        1
#define MAX_PRIORITY        255

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/**
 * @brief   Type of OSAL main structure.
 */
typedef struct {
  bool                  printf_enabled;
} osal_t;

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

static osal_t osal;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/*-- Initialization API -----------------------------------------------------*/

/**
 * @brief   OS initialization.
 * @details This function returns initializes the internal data structures
 *          of the OS Abstraction Layer. It must be called in the application
 *          startup code before calling any other OS routines.
 *
 * @return                      kAn error code.
 *
 * @api
 */
int32 OS_API_Init(void) {

  chSysInit();

  osal.printf_enabled = false;

  return OS_SUCCESS;
}

/*-- Various API -----------------------------------------------------------*/

/**
 * @brief   OS printf-like function.
 * @note    It is initially disabled.
 * @note    It is not currently implemented.
 *
 * @param[in] string            formatter string
 *
 * @api
 */
void OS_printf(const char *string, ...) {

  (void)string;

  if (osal.printf_enabled) {

  }
}

/**
 * @brief   Disables @p OS_printf().
 *
 * @api
 */
void OS_printf_disable(void) {

  osal.printf_enabled = false;
}

/**
 * @brief   Enables @p OS_printf().
 *
 * @api
 */
void OS_printf_enable(void) {

  osal.printf_enabled = true;
}

/**
 * @brief   System tick period in microseconds.
 *
 * @return                      The system tick period.
 */
int32 OS_Tick2Micros(void) {

  return 1000000 / CH_CFG_ST_FREQUENCY;
}

/**
 * @brief   Returns the local time.
 * @note    It is not currently implemented.
 *
 * @param[out] time_struct      the system time
 * @return                      An error code.
 *
 * @api
 */
int32 OS_GetLocalTime(OS_time_t *time_struct) {

  if (time_struct == NULL) {
     return OS_INVALID_POINTER;
  }

  time_struct->seconds = 0;
  time_struct->microsecs = 0;

  return OS_ERR_NOT_IMPLEMENTED;
}

/**
 * @brief   Changes the local time.
 * @note    It is not currently implemented.
 *
 * @param[in] time_struct       the system time
 * @return                      An error code.
 *
 * @api
 */
int32 OS_SetLocalTime(OS_time_t *time_struct) {

  if (time_struct == NULL) {
     return OS_INVALID_POINTER;
  }

  return OS_ERR_NOT_IMPLEMENTED;
}

/**
 * @brief   Conversion from milliseconds to ticks.
 *
 * @param[in] milli_seconds     the time in milliseconds
 * @return                      The system ticks.
 *
 * @api
 */
int32 OS_Milli2Ticks(uint32 milli_seconds) {

  return (int32)MS2ST(milli_seconds);
}

/*-- Queues API -------------------------------------------------------------*/

/*-- Semaphore and Mutex API ------------------------------------------------*/

/*-- Task Control API -------------------------------------------------------*/

/**
 * @brief   Task creation.
 *
 * @param[out] task_id          pointer to a task id variable
 * @param[in] task_name         the task name
 * @param[in] function_pointer  the task function
 * @param[in] stack_pointer     base of stack area
 * @param[in] stack_size        size of stack area
 * @param[in] priority          the task priority
 * @param[in] flags             task attributes
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TaskCreate(uint32 *task_id,
                    const char *task_name,
                    osal_task_entry function_pointer,
                    const uint32 *stack_pointer,
                    uint32 stack_size,
                    uint32 priority,
                    uint32 flags) {
  tprio_t rt_prio;
  thread_t *tp;

  (void)flags;

  /* NULL pointer checks.*/
  if ((task_id == NULL) || (task_name == NULL) ||
      (function_pointer == NULL) || (stack_pointer == NULL)) {
    return OS_INVALID_POINTER;
  }

  /* Checking alignment of stack base and size, it is application
     responsibility to pass correct values.*/
  if (!MEM_IS_ALIGNED(stack_pointer, PORT_WORKING_AREA_ALIGN) ||
      !MEM_IS_ALIGNED(stack_size, sizeof (stkalign_t))) {
    return OS_ERROR_ADDRESS_MISALIGNED;
  }

  /* Checking task name length.*/
  if (strlen(task_name) >= OS_MAX_API_NAME) {
    return OS_ERR_NAME_TOO_LONG;
  }

  /* Checking priority range.*/
  if ((priority < MIN_PRIORITY) || (priority > MAX_PRIORITY)) {
    return OS_ERR_INVALID_PRIORITY;
  }

  /* Checking if the specified stack size is below the bare minimum.*/
  if (stack_size < (uint32)THD_WORKING_AREA_SIZE(0)) {
    return OS_INVALID_INT_NUM;
  }

  /* Converting priority to RT type.*/
  rt_prio = (tprio_t)256 - (tprio_t)priority;

  tp = chThdCreateFromHeap(NULL, (size_t)stack_size, task_name,
                           rt_prio, (tfunc_t)function_pointer, NULL);

  /* Out-of-memory condition.*/
  if (tp == NULL) {
    return OS_ERROR;
  }

  /* Storing the task id.*/
  *task_id = (uint32)tp;

  return OS_SUCCESS;
}

/**
 * @brief   Installs a deletion handler.
 *
 * @param[in] function_pointer  the handler function
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TaskInstallDeleteHandler(void *function_pointer) {

  (void)function_pointer;

  return OS_ERR_NOT_IMPLEMENTED;
}

/**
 * @brief   Task delete.
 * @note    It is not currently implemented.
 *
 * @param[in] task_id           the task id
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TaskDelete(uint32 task_id) {

  (void)task_id;

  return OS_ERR_NOT_IMPLEMENTED;
}

/**
 * @brief   Task exit.
 *
 * @api
 */
void OS_TaskExit(void) {

  chThdExit(MSG_OK);
}

/**
 * @brief   Task delay.
 *
 * @param[in] milli_second      the period in miliseconds
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TaskDelay(uint32 milli_second) {

  chThdSleepMilliseconds(milli_second);
  return OS_SUCCESS;
}

/**
 * @brief   Change task priority.
 *
 * @param[in] task_id           the task id
 * @param[in] new_priority      the task new priority
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TaskSetPriority (uint32 task_id, uint32 new_priority) {
  tprio_t rt_newprio;
  thread_t *tp = (thread_t *)task_id;

  /* Checking priority range.*/
  if ((new_priority < MIN_PRIORITY) || (new_priority > MAX_PRIORITY)) {
    return OS_ERR_INVALID_PRIORITY;
  }

  /* Converting priority to RT type.*/
  rt_newprio = (tprio_t)256 - (tprio_t)new_priority;

  if (chThdGetPriorityX() == rt_newprio) {
    return OS_SUCCESS;
  }

  chSysLock();

  /* Changing priority.*/
  if ((tp->prio == tp->realprio) || (rt_newprio > tp->prio)) {
    tp->prio = rt_newprio;
  }
  tp->realprio = rt_newprio;

  /* The following states need priority queues reordering.*/
  switch (tp->state) {
  case CH_STATE_WTMTX:
#if CH_CFG_USE_CONDVARS
  case CH_STATE_WTCOND:
#endif
#if CH_CFG_USE_SEMAPHORES_PRIORITY
  case CH_STATE_WTSEM:
#endif
#if CH_CFG_USE_MESSAGES && CH_CFG_USE_MESSAGES_PRIORITY
  case CH_STATE_SNDMSGQ:
#endif
    /* Re-enqueues tp with its new priority on the queue.*/
    queue_prio_insert(queue_dequeue(tp),
                      (threads_queue_t *)tp->u.wtobjp);
    break;
  case CH_STATE_READY:
#if CH_DBG_ENABLE_ASSERTS
    /* Prevents an assertion in chSchReadyI().*/
    tp->state = CH_STATE_CURRENT;
#endif
    /* Re-enqueues tp with its new priority on the ready list.*/
    chSchReadyI(queue_dequeue(tp));
    break;
  }

  /* Rescheduling.*/
  chSchRescheduleS();
  chSysUnlock();

  return OS_SUCCESS;
}

/**
 * @brief   Task registration.
 * @note    In ChibiOS/RT it does nothing.
 *
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TaskRegister(void) {

  return OS_SUCCESS;
}

/**
 * @brief   Current task id.
 *
 * @return                      The current task id.
 *
 * @api
 */
uint32 OS_TaskGetId(void) {

  return (uint32)chThdGetSelfX();
}

/**
 * @brief   Retrieves a task id by name.
 */
int32 OS_TaskGetIdByName (uint32 *task_id, const char *task_name) {
  thread_t *tp;

  /* NULL pointer checks.*/
  if ((task_id == NULL) || (task_name == NULL)) {
    return OS_INVALID_POINTER;
  }

  /* Checking task name length.*/
  if (strlen(task_name) >= OS_MAX_API_NAME) {
    return OS_ERR_NAME_TOO_LONG;
  }

  /* Scanning registry.*/
  tp = chRegFirstThread();
  do {
    if (strcmp(chRegGetThreadNameX(tp), task_name) == 0) {
      *task_id = (uint32)tp;
      return OS_SUCCESS;
    }
    tp = chRegNextThread(tp);
  } while (tp != NULL);

  return OS_ERR_NAME_NOT_FOUND;
}

int32 OS_TaskGetInfo (uint32 task_id, OS_task_prop_t *task_prop) {

  (void)task_id;
  (void)task_prop;

  return OS_ERR_NOT_IMPLEMENTED;
}
/** @} */

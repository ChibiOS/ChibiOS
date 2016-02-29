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

#if CH_CFG_ST_FREQUENCY > 1000000
#error "CH_CFG_ST_FREQUENCY limit is 1000000"
#endif

#if (CH_CFG_ST_FREQUENCY % 1000) != 0
#error "CH_CFG_ST_FREQUENCY is not a multiple of 1000"
#endif

#if CH_CFG_USE_REGISTRY == FALSE
#error "NASA OSAL requires CH_CFG_USE_REGISTRY"
#endif

#if CH_CFG_USE_EVENTS == FALSE
#error "NASA OSAL requires CH_CFG_USE_EVENTS"
#endif

#if CH_CFG_USE_MUTEXES == FALSE
#error "NASA OSAL requires CH_CFG_USE_MUTEXES"
#endif

#if CH_CFG_USE_SEMAPHORES == FALSE
#error "NASA OSAL requires CH_CFG_USE_SEMAPHORES"
#endif

#if CH_CFG_USE_MEMCORE == FALSE
#error "NASA OSAL requires CH_CFG_USE_MEMCORE"
#endif

#if CH_CFG_USE_MEMPOOLS == FALSE
#error "NASA OSAL requires CH_CFG_USE_MEMPOOLS"
#endif

#if CH_CFG_USE_HEAP == FALSE
#error "NASA OSAL requires CH_CFG_USE_HEAP"
#endif

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

#define MIN_PRIORITY        1
#define MAX_PRIORITY        255

#define MIN_MESSAGE_SIZE    4
#define MAX_MESSAGE_SIZE    16384

#define MIN_QUEUE_DEPTH     1
#define MAX_QUEUE_DEPTH     16384

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/**
 * @brief   Generic function pointer type.
 */
typedef void (*funcptr_t)(void);

/**
 * @brief   Type of OSAL timer.
 */
typedef struct {
  uint32                is_free;
  char                  name[OS_MAX_API_NAME];
  OS_TimerCallback_t    callback_ptr;
  uint32                start_time;
  uint32                interval_time;
  virtual_timer_t       vt;
} osal_timer_t;

/**
 * @brief   Type of an OSAL queue.
 */
typedef struct {
  uint32                is_free;
  char                  name[OS_MAX_API_NAME];
  memory_pool_t         messages;
  mailbox_t             mb;
  msg_t                 *mb_buffer;
} osal_queue_t;

/**
 * @brief   Type of an osal message with minimum size.
 */
typedef struct {
  size_t                size;
  char                  buf[4];
} osal_message_t;

/**
 * @brief   Type of OSAL main structure.
 */
typedef struct {
  bool                  printf_enabled;
  memory_pool_t         timers_pool;
  memory_pool_t         queues_pool;
  memory_pool_t         binary_semaphores_pool;
  memory_pool_t         count_semaphores_pool;
  memory_pool_t         mutexes_pool;
  osal_timer_t          timers[OS_MAX_TIMERS];
  osal_queue_t          queues[OS_MAX_QUEUES];
  binary_semaphore_t    binary_semaphores[OS_MAX_BIN_SEMAPHORES];
  semaphore_t           count_semaphores[OS_MAX_COUNT_SEMAPHORES];
  mutex_t               mutexes[OS_MAX_MUTEXES];
} osal_t;

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

static osal_t osal;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Virtual timers callback.
 */
static void timer_handler(void *p) {
  osal_timer_t *otp = (osal_timer_t *)p;

  /* Real callback.*/
  otp->callback_ptr((uint32)p);

  /* Timer restart if an interval is defined.*/
  if (otp->interval_time != 0) {
    chSysLockFromISR();
    chVTSetI(&otp->vt, US2ST(otp->interval_time), timer_handler, p);
    chSysUnlockFromISR();
  }
}

/**
 * @brief   Message allocation from the default heap.
 */
static void *feed_message(size_t size, unsigned align) {

  return chHeapAllocAligned(NULL, size, align);
}

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

  /* OS_printf() initially disabled.*/
  osal.printf_enabled = false;

  /* Timers pool initialization.*/
  chPoolObjectInit(&osal.timers_pool,
                   sizeof (osal_timer_t),
                   NULL);
  chPoolLoadArray(&osal.timers_pool,
                  &osal.timers,
                  sizeof (osal_timer_t));

  /* Queues pool initialization.*/
  chPoolObjectInit(&osal.queues_pool,
                   sizeof (osal_queue_t),
                   NULL);
  chPoolLoadArray(&osal.queues_pool,
                  &osal.queues,
                  sizeof (osal_queue_t));

  /* Binary Semaphores pool initialization.*/
  chPoolObjectInit(&osal.binary_semaphores_pool,
                   sizeof (binary_semaphore_t),
                   NULL);
  chPoolLoadArray(&osal.binary_semaphores_pool,
                  &osal.binary_semaphores,
                  sizeof (binary_semaphore_t));

  /* Counter Semaphores pool initialization.*/
  chPoolObjectInit(&osal.count_semaphores_pool,
                   sizeof (semaphore_t),
                   NULL);
  chPoolLoadArray(&osal.count_semaphores_pool,
                  &osal.count_semaphores,
                  sizeof (semaphore_t));

  /* Mutexes pool initialization.*/
  chPoolObjectInit(&osal.mutexes_pool,
                   sizeof (mutex_t),
                   NULL);
  chPoolLoadArray(&osal.mutexes_pool,
                  &osal.mutexes,
                  sizeof (mutex_t));

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

/*-- timers API -------------------------------------------------------------*/

/**
 * @brief   Binary semaphore creation.
 *
 * @param[out] timer_id         pointer to a timer id variable
 * @param[in] timer_name        the timer name
 * @param[out] clock_accuracy   timer accuracy in microseconds
 * @param[in] callback_ptr      timer callback
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TimerCreate(uint32 *timer_id, const char *timer_name,
                     uint32 *clock_accuracy, OS_TimerCallback_t callback_ptr) {
  osal_timer_t *otp;

  /* NULL pointer checks.*/
  if ((timer_id == NULL) || (timer_name == NULL) || (clock_accuracy == NULL)) {
    return OS_INVALID_POINTER;
  }

  /* NULL callback check.*/
  if (callback_ptr == NULL) {
    return OS_TIMER_ERR_INVALID_ARGS;
  }

  /* Checking timer name length.*/
  if (strlen(timer_name) >= OS_MAX_API_NAME) {
    return OS_ERR_NAME_TOO_LONG;
  }

  /* Getting object.*/
  otp = chPoolAlloc(&osal.timers_pool);
  if (otp == NULL) {
    return OS_ERR_NO_FREE_IDS;
  }

  chSysLock();

  strncpy(otp->name, timer_name, OS_MAX_API_NAME);
  chVTObjectInit(&otp->vt);
  otp->is_free       = 0;
  otp->start_time    = 0;
  otp->interval_time = 0;
  otp->callback_ptr  = callback_ptr;

  *clock_accuracy = (uint32)(1000000 / CH_CFG_ST_FREQUENCY);

  chSysUnlock();

  return OS_SUCCESS;
}

/**
 * @brief   Timer deletion.
 *
 * @param[in] timer_id          timer id variable
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TimerDelete(uint32 timer_id) {
  osal_timer_t *otp = (osal_timer_t *)timer_id;

  /* Range check.*/
  if ((otp < &osal.timers[0]) ||
      (otp >= &osal.timers[OS_MAX_TIMERS])) {
    return OS_ERR_INVALID_ID;
  }

  chSysLock();

  /* Resetting the timer.*/
  chVTResetI(&otp->vt);
  otp->start_time    = 0;
  otp->interval_time = 0;

  /* Flagging it as unused and returning it to the pool.*/
  chPoolFreeI(&osal.timers_pool, (void *)otp);

  chSysUnlock();

  return OS_SUCCESS;
}

/**
 * @brief   Timer deletion.
 * @note    This function can be safely called from timer callbacks or ISRs.
 *
 * @param[in] timer_id          timer id variable
 * @param[in] start_time        start time in microseconds or zero
 * @param[in] interval_time     interval time in microseconds or zero
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TimerSet(uint32 timer_id, uint32 start_time, uint32 interval_time) {
  syssts_t sts;
  osal_timer_t *otp = (osal_timer_t *)timer_id;

  /* Range check.*/
  if ((otp < &osal.timers[0]) ||
      (otp >= &osal.timers[OS_MAX_TIMERS])) {
    return OS_ERR_INVALID_ID;
  }

  /* Entering a reentrant critical zone.*/
  sts = chSysGetStatusAndLockX();

  if (start_time == 0) {
    chVTResetI(&otp->vt);
  }
  else {
    otp->start_time    = start_time;
    otp->interval_time = interval_time;
    chVTSetI(&otp->vt, US2ST(start_time), timer_handler, (void *)timer_id);
  }

  /* Leaving the critical zone.*/
  chSysRestoreStatusX(sts);

  return OS_SUCCESS;
}

/**
 * @brief   Retrieves a timer id by name.
 * @note    It is not currently implemented.
 *
 * @param[out] timer_id         pointer to a timer id variable
 * @param[in] sem_name          the timer name
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TimerGetIdByName(uint32 *timer_id, const char *timer_name) {

  /* NULL pointer checks.*/
  if ((timer_id == NULL) || (timer_id == NULL)) {
    return OS_INVALID_POINTER;
  }

  /* Checking name length.*/
  if (strlen(timer_name) >= OS_MAX_API_NAME) {
    return OS_ERR_NAME_TOO_LONG;
  }

  return OS_ERR_NOT_IMPLEMENTED;
}

/**
 * @brief   Returns timer information.
 * @note    This function can be safely called from timer callbacks or ISRs.
 *
 * @param[in] timer_id          timer id variable
 * @param[in] timer_prop        timer properties
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TimerGetInfo(uint32 timer_id, OS_timer_prop_t *timer_prop) {
  syssts_t sts;
  osal_timer_t *otp = (osal_timer_t *)timer_id;

  /* NULL pointer checks.*/
  if (timer_prop == NULL) {
    return OS_INVALID_POINTER;
  }

  /* Range check.*/
  if ((otp < &osal.timers[0]) ||
      (otp >= &osal.timers[OS_MAX_TIMERS])) {
    return OS_ERR_INVALID_ID;
  }

  /* Entering a reentrant critical zone.*/
  sts = chSysGetStatusAndLockX();

  /* If the semaphore is not in use then error.*/
  if (otp->is_free) {
    /* Leaving the critical zone.*/
    chSysRestoreStatusX(sts);
    return OS_ERR_INVALID_ID;
  }

  strncpy(timer_prop->name, otp->name, OS_MAX_API_NAME - 1);
  timer_prop->creator       = (uint32)0;
  timer_prop->start_time    = otp->start_time;
  timer_prop->interval_time = otp->interval_time;
  timer_prop->accuracy      = (uint32)(1000000 / CH_CFG_ST_FREQUENCY);

  /* Leaving the critical zone.*/
  chSysRestoreStatusX(sts);

  return OS_SUCCESS;
}

/*-- Queues API -------------------------------------------------------------*/

int32 OS_QueueCreate(uint32 *queue_id, const char *queue_name,
                     uint32 queue_depth, uint32 data_size, uint32 flags) {
  osal_queue_t *oqp;
  msg_t *buffer;

  (void)flags;

  /* NULL pointer checks.*/
  if ((queue_id == NULL) || (queue_name == NULL)) {
    return OS_INVALID_POINTER;
  }

  /* Checking queue name length.*/
  if (strlen(queue_name) >= OS_MAX_API_NAME) {
    return OS_ERR_NAME_TOO_LONG;
  }

  /* Checks on queue limits. There is no dedicated error code.*/
  if ((data_size < MIN_MESSAGE_SIZE) || (data_size < MAX_MESSAGE_SIZE) ||
      (queue_depth < MIN_QUEUE_DEPTH) || (queue_depth < MAX_QUEUE_DEPTH)) {
    return OS_ERROR;
  }

  /* Getting object.*/
  oqp = chPoolAlloc(&osal.queues_pool);
  if (oqp == NULL) {
    return OS_ERR_NO_FREE_IDS;
  }

  /* Attempting buffer allocation.*/

  /* Initializing object static parts.*/
  strncpy(oqp->name, queue_name, OS_MAX_API_NAME);
  oqp->is_free = 0;
  chPoolObjectInit(&oqp->messages,
                   sizeof(size_t) + data_size,
                   feed_message);
  chMBObjectInit(&oqp->mb, buffer, (size_t)queue_depth);

  return OS_SUCCESS;
}

int32 OS_QueueDelete(uint32 queue_id) {

  (void)queue_id;

  return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_QueueGet(uint32 queue_id, void *data, uint32 size,
                  uint32 *size_copied, int32 timeout) {

  (void)queue_id;
  (void)data;
  (void)size;
  (void)size_copied;
  (void)timeout;

  return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_QueuePut(uint32 queue_id, void *data, uint32 size, uint32 flags) {

  (void)queue_id;
  (void)data;
  (void)size;
  (void)flags;

  return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_QueueGetIdByName(uint32 *queue_id, const char *queue_name) {

  (void)queue_id;
  (void)queue_name;

  return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_QueueGetInfo (uint32 queue_id, OS_queue_prop_t *queue_prop) {

  (void)queue_id;
  (void)queue_prop;

  return OS_ERR_NOT_IMPLEMENTED;
}

/*-- Binary Semaphore API ---------------------------------------------------*/

/**
 * @brief   Binary semaphore creation.
 *
 * @param[out] sem_id           pointer to a binary semaphore id variable
 * @param[in] sem_name          the binary semaphore name
 * @param[in] sem_initial_value semaphore initial value
 * @param[in] options           semaphore options
 * @return                      An error code.
 *
 * @api
 */
int32 OS_BinSemCreate(uint32 *sem_id, const char *sem_name,
                      uint32 sem_initial_value, uint32 options) {
  binary_semaphore_t *bsp;

  (void)options;

  /* NULL pointer checks.*/
  if ((sem_id == NULL) || (sem_name == NULL)) {
    return OS_INVALID_POINTER;
  }

  /* Checking semaphore name length.*/
  if (strlen(sem_name) >= OS_MAX_API_NAME) {
    return OS_ERR_NAME_TOO_LONG;
  }

  /* Semaphore counter check, it is binary so only 0 and 1.*/
  if (sem_initial_value > 1) {
    return OS_INVALID_INT_NUM;
  }

  /* Getting object.*/
  bsp = chPoolAlloc(&osal.binary_semaphores_pool);
  if (bsp == NULL) {
    return OS_ERR_NO_FREE_IDS;
  }

  /* Semaphore is initialized.*/
  chBSemObjectInit(bsp, sem_initial_value == 0 ? false : true);

  *sem_id = (uint32)bsp;

  return OS_SUCCESS;
}

/**
 * @brief   Binary semaphore deletion.
 *
 * @param[in] sem_id            binary semaphore id variable
 * @return                      An error code.
 *
 * @api
 */
int32 OS_BinSemDelete(uint32 sem_id) {
  binary_semaphore_t *bsp = (binary_semaphore_t *)sem_id;

  /* Range check.*/
  if ((bsp < &osal.binary_semaphores[0]) ||
      (bsp >= &osal.binary_semaphores[OS_MAX_BIN_SEMAPHORES])) {
    return OS_ERR_INVALID_ID;
  }

  chSysLock();

  /* Resetting the semaphore, no threads in queue.*/
  chBSemResetI(bsp, true);

  /* Flagging it as unused and returning it to the pool.*/
  bsp->sem.queue.prev = NULL;
  chPoolFreeI(&osal.binary_semaphores_pool, (void *)bsp);

  /* Required because some thread could have been made ready.*/
  chSchRescheduleS();

  chSysUnlock();

  return OS_SUCCESS;
}

/**
 * @brief   Binary semaphore flush.
 * @note    The state of the binary semaphore is not changed.
 * @note    This function can be safely called from timer callbacks or ISRs.
 *
 * @param[in] sem_id            binary semaphore id variable
 * @return                      An error code.
 *
 * @api
 */
int32 OS_BinSemFlush(uint32 sem_id) {
  syssts_t sts;
  binary_semaphore_t *bsp = (binary_semaphore_t *)sem_id;

  /* Range check.*/
  if ((bsp < &osal.binary_semaphores[0]) ||
      (bsp >= &osal.binary_semaphores[OS_MAX_BIN_SEMAPHORES])) {
    return OS_ERR_INVALID_ID;
  }

  /* Entering a reentrant critical zone.*/
  sts = chSysGetStatusAndLockX();

  /* If the semaphore is not in use then error.*/
  if (bsp->sem.queue.prev == NULL) {
    /* Leaving the critical zone.*/
    chSysRestoreStatusX(sts);
    return OS_SEM_FAILURE;
  }

  chBSemResetI(bsp, true);

  /* Leaving the critical zone.*/
  chSysRestoreStatusX(sts);

  return OS_SUCCESS;
}

/**
 * @brief   Binary semaphore give.
 * @note    This function can be safely called from timer callbacks or ISRs.
 *
 * @param[in] sem_id            binary semaphore id variable
 * @return                      An error code.
 *
 * @api
 */
int32 OS_BinSemGive(uint32 sem_id) {
  syssts_t sts;
  binary_semaphore_t *bsp = (binary_semaphore_t *)sem_id;

  /* Range check.*/
  if ((bsp < &osal.binary_semaphores[0]) ||
      (bsp >= &osal.binary_semaphores[OS_MAX_BIN_SEMAPHORES])) {
    return OS_ERR_INVALID_ID;
  }

  /* Entering a reentrant critical zone.*/
  sts = chSysGetStatusAndLockX();

  /* If the semaphore is not in use then error.*/
  if (bsp->sem.queue.prev == NULL) {
    /* Leaving the critical zone.*/
    chSysRestoreStatusX(sts);
    return OS_SEM_FAILURE;
  }

  chBSemSignalI(bsp);

  /* Leaving the critical zone.*/
  chSysRestoreStatusX(sts);

  return OS_SUCCESS;
}

/**
 * @brief   Binary semaphore take.
 *
 * @param[in] sem_id            binary semaphore id variable
 * @return                      An error code.
 *
 * @api
 */
int32 OS_BinSemTake(uint32 sem_id) {
  binary_semaphore_t *bsp = (binary_semaphore_t *)sem_id;

  /* Range check.*/
  if ((bsp < &osal.binary_semaphores[0]) ||
      (bsp >= &osal.binary_semaphores[OS_MAX_BIN_SEMAPHORES])) {
    return OS_ERR_INVALID_ID;
  }

  chSysLock();

  /* If the semaphore is not in use then error.*/
  if (bsp->sem.queue.prev == NULL) {
    chSysUnlock();
    return OS_SEM_FAILURE;
  }

  (void) chBSemWaitS(bsp);

  chSysUnlock();

  return OS_SUCCESS;
}

/**
 * @brief   Binary semaphore take with timeout.
 *
 * @param[in] sem_id            binary semaphore id variable
 * @param[in] msecs             timeout in milliseconds
 * @return                      An error code.
 *
 * @api
 */
int32 OS_BinSemTimedWait(uint32 sem_id, uint32 msecs) {
  binary_semaphore_t *bsp = (binary_semaphore_t *)sem_id;
  msg_t msg;

  /* Range check.*/
  if ((bsp < &osal.binary_semaphores[0]) ||
      (bsp >= &osal.binary_semaphores[OS_MAX_BIN_SEMAPHORES])) {
    return OS_ERR_INVALID_ID;
  }

  /* Timeouts of zero not allowed.*/
  if (msecs == 0) {
    return OS_INVALID_INT_NUM;
  }

  chSysLock();

  /* If the semaphore is not in use then error.*/
  if (bsp->sem.queue.prev == NULL) {
    chSysUnlock();
    return OS_SEM_FAILURE;
  }

  msg = chBSemWaitTimeoutS(bsp, MS2ST(msecs));

  chSysUnlock();

  return msg == MSG_TIMEOUT ? OS_SEM_TIMEOUT : OS_SUCCESS;
}

/**
 * @brief   Retrieves a binary semaphore id by name.
 * @note    It is not currently implemented.
 *
 * @param[out] sem_id           pointer to a binary semaphore id variable
 * @param[in] sem_name          the binary semaphore name
 * @return                      An error code.
 *
 * @api
 */
int32 OS_BinSemGetIdByName(uint32 *sem_id, const char *sem_name) {

  /* NULL pointer checks.*/
  if ((sem_id == NULL) || (sem_name == NULL)) {
    return OS_INVALID_POINTER;
  }

  /* Checking name length.*/
  if (strlen(sem_name) >= OS_MAX_API_NAME) {
    return OS_ERR_NAME_TOO_LONG;
  }

  return OS_ERR_NOT_IMPLEMENTED;
}

/**
 * @brief   Returns binary semaphore information.
 * @note    This function can be safely called from timer callbacks or ISRs.
 * @note    It is not currently implemented.
 *
 * @param[in] sem_id            binary semaphore id variable
 * @param[in] bin_prop          binary semaphore properties
 * @return                      An error code.
 *
 * @api
 */
int32 OS_BinSemGetInfo(uint32 sem_id, OS_bin_sem_prop_t *bin_prop) {
  syssts_t sts;
  binary_semaphore_t *bsp = (binary_semaphore_t *)sem_id;

  /* NULL pointer checks.*/
  if (bin_prop == NULL) {
    return OS_INVALID_POINTER;
  }

  /* Range check.*/
  if ((bsp < &osal.binary_semaphores[0]) ||
      (bsp >= &osal.binary_semaphores[OS_MAX_BIN_SEMAPHORES])) {
    return OS_ERR_INVALID_ID;
  }

  /* Entering a reentrant critical zone.*/
  sts = chSysGetStatusAndLockX();

  /* If the semaphore is not in use then error.*/
  if (bsp->sem.queue.prev == NULL) {
    /* Leaving the critical zone.*/
    chSysRestoreStatusX(sts);
    return OS_ERR_INVALID_ID;
  }

  /* Leaving the critical zone.*/
  chSysRestoreStatusX(sts);

  return OS_ERR_NOT_IMPLEMENTED;
}

/*-- Counter Semaphore API --------------------------------------------------*/

/**
 * @brief   Counter semaphore creation.
 *
 * @param[out] sem_id           pointer to a counter semaphore id variable
 * @param[in] sem_name          the counter semaphore name
 * @param[in] sem_initial_value semaphore initial value
 * @param[in] options           semaphore options
 * @return                      An error code.
 *
 * @api
 */
int32 OS_CountSemCreate(uint32 *sem_id, const char *sem_name,
                        uint32 sem_initial_value, uint32 options) {
  semaphore_t *sp;

  (void)options;

  /* NULL pointer checks.*/
  if ((sem_id == NULL) || (sem_name == NULL)) {
    return OS_INVALID_POINTER;
  }

  /* Checking semaphore name length.*/
  if (strlen(sem_name) >= OS_MAX_API_NAME) {
    return OS_ERR_NAME_TOO_LONG;
  }

  /* Semaphore counter check, it must be non-negative.*/
  if ((int32)sem_initial_value < 0) {
    return OS_INVALID_INT_NUM;
  }

  /* Getting object.*/
  sp = chPoolAlloc(&osal.count_semaphores_pool);
  if (sp == NULL) {
    return OS_ERR_NO_FREE_IDS;
  }

  /* Semaphore is initialized.*/
  chSemObjectInit(sp, (cnt_t)sem_initial_value);

  *sem_id = (uint32)sp;

  return OS_SUCCESS;
}

/**
 * @brief   Counter semaphore deletion.
 *
 * @param[in] sem_id            counter semaphore id variable
 * @return                      An error code.
 *
 * @api
 */
int32 OS_CountSemDelete(uint32 sem_id) {
  semaphore_t *sp = (semaphore_t *)sem_id;

  /* Range check.*/
  if ((sp < &osal.count_semaphores[0]) ||
      (sp >= &osal.count_semaphores[OS_MAX_COUNT_SEMAPHORES])) {
    return OS_ERR_INVALID_ID;
  }

  chSysLock();

  /* Resetting the semaphore, no threads in queue.*/
  chSemResetI(sp, 0);

  /* Flagging it as unused and returning it to the pool.*/
  sp->queue.prev = NULL;
  chPoolFreeI(&osal.count_semaphores_pool, (void *)sp);

  /* Required because some thread could have been made ready.*/
  chSchRescheduleS();

  chSysUnlock();

  return OS_SUCCESS;
}

/**
 * @brief   Counter semaphore give.
 * @note    This function can be safely called from timer callbacks or ISRs.
 *
 * @param[in] sem_id            counter semaphore id variable
 * @return                      An error code.
 *
 * @api
 */
int32 OS_CountSemGive(uint32 sem_id) {
  syssts_t sts;
  semaphore_t *sp = (semaphore_t *)sem_id;

  /* Range check.*/
  if ((sp < &osal.count_semaphores[0]) ||
      (sp >= &osal.count_semaphores[OS_MAX_COUNT_SEMAPHORES])) {
    return OS_ERR_INVALID_ID;
  }

  /* Entering a reentrant critical zone.*/
  sts = chSysGetStatusAndLockX();

  /* If the semaphore is not in use then error.*/
  if (sp->queue.prev == NULL) {
    /* Leaving the critical zone.*/
    chSysRestoreStatusX(sts);
    return OS_SEM_FAILURE;
  }

  chSemSignalI(sp);

  /* Leaving the critical zone.*/
  chSysRestoreStatusX(sts);

  return OS_SUCCESS;
}

/**
 * @brief   Counter semaphore take.
 *
 * @param[in] sem_id            counter semaphore id variable
 * @return                      An error code.
 *
 * @api
 */
int32 OS_CountSemTake(uint32 sem_id) {
  semaphore_t *sp = (semaphore_t *)sem_id;

  /* Range check.*/
  if ((sp < &osal.count_semaphores[0]) ||
      (sp >= &osal.count_semaphores[OS_MAX_COUNT_SEMAPHORES])) {
    return OS_ERR_INVALID_ID;
  }

  chSysLock();

  /* If the semaphore is not in use then error.*/
  if (sp->queue.prev == NULL) {
    chSysUnlock();
    return OS_SEM_FAILURE;
  }

  (void) chSemWaitS(sp);

  chSysUnlock();

  return OS_SUCCESS;
}

/**
 * @brief   Counter semaphore take with timeout.
 *
 * @param[in] sem_id            counter semaphore id variable
 * @param[in] msecs             timeout in milliseconds
 * @return                      An error code.
 *
 * @api
 */
int32 OS_CountSemTimedWait(uint32 sem_id, uint32 msecs) {
  semaphore_t *sp = (semaphore_t *)sem_id;
  msg_t msg;

  /* Range check.*/
  if ((sp < &osal.count_semaphores[0]) ||
      (sp >= &osal.count_semaphores[OS_MAX_COUNT_SEMAPHORES])) {
    return OS_ERR_INVALID_ID;
  }

  /* Timeouts of zero not allowed.*/
  if (msecs == 0) {
    return OS_INVALID_INT_NUM;
  }

  chSysLock();

  /* If the semaphore is not in use then error.*/
  if (sp->queue.prev == NULL) {
    chSysUnlock();
    return OS_SEM_FAILURE;
  }

  msg = chSemWaitTimeoutS(sp, MS2ST(msecs));

  chSysUnlock();

  return msg == MSG_TIMEOUT ? OS_SEM_TIMEOUT : OS_SUCCESS;
}

/**
 * @brief   Retrieves a counter semaphore id by name.
 * @note    It is not currently implemented.
 *
 * @param[out] sem_id           pointer to a counter semaphore id variable
 * @param[in] sem_name          the counter semaphore name
 * @return                      An error code.
 *
 * @api
 */
int32 OS_CountSemGetIdByName(uint32 *sem_id, const char *sem_name) {

  /* NULL pointer checks.*/
  if ((sem_id == NULL) || (sem_name == NULL)) {
    return OS_INVALID_POINTER;
  }

  /* Checking name length.*/
  if (strlen(sem_name) >= OS_MAX_API_NAME) {
    return OS_ERR_NAME_TOO_LONG;
  }

  return OS_ERR_NOT_IMPLEMENTED;
}

/**
 * @brief   Returns counter semaphore information.
 * @note    This function can be safely called from timer callbacks or ISRs.
 * @note    It is not currently implemented.
 *
 * @param[in] sem_id            counter semaphore id variable
 * @param[in] sem_prop          counter semaphore properties
 * @return                      An error code.
 *
 * @api
 */
int32 OS_CountSemGetInfo(uint32 sem_id, OS_count_sem_prop_t *sem_prop) {
  syssts_t sts;
  semaphore_t *sp = (semaphore_t *)sem_id;

  /* NULL pointer checks.*/
  if (sem_prop == NULL) {
    return OS_INVALID_POINTER;
  }

  /* Range check.*/
  if ((sp < &osal.count_semaphores[0]) ||
      (sp >= &osal.count_semaphores[OS_MAX_BIN_SEMAPHORES])) {
    return OS_ERR_INVALID_ID;
  }

  /* Entering a reentrant critical zone.*/
  sts = chSysGetStatusAndLockX();

  /* If the semaphore is not in use then error.*/
  if (sp->queue.prev == NULL) {
    /* Leaving the critical zone.*/
    chSysRestoreStatusX(sts);
    return OS_ERR_INVALID_ID;
  }

  /* Leaving the critical zone.*/
  chSysRestoreStatusX(sts);

  return OS_ERR_NOT_IMPLEMENTED;
}

/*-- Mutex API --------------------------------------------------------------*/

/**
 * @brief   Mutex creation.
 *
 * @param[out] sem_id           pointer to a mutex id variable
 * @param[in] sem_name          the mutex name
 * @param[in] options           mutex options
 * @return                      An error code.
 *
 * @api
 */
int32 OS_MutSemCreate(uint32 *sem_id, const char *sem_name, uint32 options) {
  mutex_t *mp;

  (void)options;

  /* NULL pointer checks.*/
  if ((sem_id == NULL) || (sem_name == NULL)) {
    return OS_INVALID_POINTER;
  }

  /* Checking semaphore name length.*/
  if (strlen(sem_name) >= OS_MAX_API_NAME) {
    return OS_ERR_NAME_TOO_LONG;
  }

  /* Getting object.*/
  mp = chPoolAlloc(&osal.mutexes_pool);
  if (mp == NULL) {
    return OS_ERR_NO_FREE_IDS;
  }

  /* Semaphore is initialized.*/
  chMtxObjectInit(mp);

  *sem_id = (uint32)mp;

  return OS_SUCCESS;
}

/**
 * @brief   Mutex deletion.
 *
 * @param[in] sem_id            mutex id variable
 * @return                      An error code.
 *
 * @api
 */
int32 OS_MutSemDelete(uint32 sem_id) {
  mutex_t *mp = (mutex_t *)sem_id;

  /* Range check.*/
  if ((mp < &osal.mutexes[0]) ||
      (mp >= &osal.mutexes[OS_MAX_MUTEXES])) {
    return OS_ERR_INVALID_ID;
  }

  chSysLock();

  /* Resetting the mutex, no threads in queue.*/
  chMtxUnlockAllS();

  /* Flagging it as unused and returning it to the pool.*/
  mp->queue.prev = NULL;
  chPoolFreeI(&osal.mutexes_pool, (void *)mp);

  /* Required because some thread could have been made ready.*/
  chSchRescheduleS();

  chSysUnlock();

  return OS_SUCCESS;
}

/**
 * @brief   Mutex give.
 *
 * @param[in] sem_id            mutex id variable
 * @return                      An error code.
 *
 * @api
 */
int32 OS_MutSemGive(uint32 sem_id) {
  mutex_t *mp = (mutex_t *)sem_id;

  /* Range check.*/
  if ((mp < &osal.mutexes[0]) ||
      (mp >= &osal.mutexes[OS_MAX_COUNT_SEMAPHORES])) {
    return OS_ERR_INVALID_ID;
  }

  chSysLock();

  /* If the mutex is not in use then error.*/
  if (mp->queue.prev == NULL) {
    chSysUnlock();
    return OS_SEM_FAILURE;
  }

  chMtxUnlockS(mp);
  chSchRescheduleS();

  chSysUnlock();

  return OS_SUCCESS;
}

/**
 * @brief   Mutex take.
 *
 * @param[in] sem_id            mutex id variable
 * @return                      An error code.
 *
 * @api
 */
int32 OS_MutSemTake(uint32 sem_id) {
  mutex_t *mp = (mutex_t *)sem_id;

  /* Range check.*/
  if ((mp < &osal.mutexes[0]) ||
      (mp >= &osal.mutexes[OS_MAX_COUNT_SEMAPHORES])) {
    return OS_ERR_INVALID_ID;
  }

  chSysLock();

  /* If the mutex is not in use then error.*/
  if (mp->queue.prev == NULL) {
    chSysUnlock();
    return OS_SEM_FAILURE;
  }

  chMtxLockS(mp);

  chSysUnlock();

  return OS_SUCCESS;
}

/**
 * @brief   Retrieves a mutex id by name.
 * @note    It is not currently implemented.
 *
 * @param[out] sem_id           pointer to a mutex id variable
 * @param[in] sem_name          the mutex name
 * @return                      An error code.
 *
 * @api
 */
int32 OS_MutSemGetIdByName(uint32 *sem_id, const char *sem_name) {

  /* NULL pointer checks.*/
  if ((sem_id == NULL) || (sem_name == NULL)) {
    return OS_INVALID_POINTER;
  }

  /* Checking name length.*/
  if (strlen(sem_name) >= OS_MAX_API_NAME) {
    return OS_ERR_NAME_TOO_LONG;
  }

  return OS_ERR_NOT_IMPLEMENTED;
}

/**
 * @brief   Returns mutex information.
 * @note    This function can be safely called from timer callbacks or ISRs.
 * @note    It is not currently implemented.
 *
 * @param[in] sem_id            mutex id variable
 * @param[in] sem_prop          mutex properties
 * @return                      An error code.
 *
 * @api
 */
int32 OS_MutSemGetInfo(uint32 sem_id, OS_mut_sem_prop_t *sem_prop) {
  syssts_t sts;
  mutex_t *mp = (mutex_t *)sem_id;

  /* NULL pointer checks.*/
  if (sem_prop == NULL) {
    return OS_INVALID_POINTER;
  }

  /* Range check.*/
  if ((mp < &osal.mutexes[0]) ||
      (mp >= &osal.mutexes[OS_MAX_BIN_SEMAPHORES])) {
    return OS_ERR_INVALID_ID;
  }

  /* Entering a reentrant critical zone.*/
  sts = chSysGetStatusAndLockX();

  /* If the mutex is not in use then error.*/
  if (mp->queue.prev == NULL) {
    /* Leaving the critical zone.*/
    chSysRestoreStatusX(sts);
    return OS_ERR_INVALID_ID;
  }

  /* Leaving the critical zone.*/
  chSysRestoreStatusX(sts);

  return OS_ERR_NOT_IMPLEMENTED;
}

/*-- Task Control API -------------------------------------------------------*/

/**
 * @brief   Task creation.
 * @note    The task name is not copied inside the task but kept by reference,
 *          the name is supposed to be persistent, better if defined as a
 *          sting constant.
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

  /* Checking if this working area is already in use by some thread, the
     error code is not very appropriate but this case seems to not be
     coveded by the specification.*/
  tp = chRegFindThreadByWorkingArea((stkalign_t *)stack_pointer);
  if (tp != NULL) {
    /* Releasing the thread reference.*/
    chThdRelease(tp);
    return OS_ERR_NO_FREE_IDS;
  }

  /* Checking if the name is already in use.*/
  if ((tp = chRegFindThreadByName(task_name)) != NULL) {
    /* Releasing the thread reference.*/
    chThdRelease(tp);
    return OS_ERR_NAME_TAKEN;
  }

  /* Converting priority to RT type.*/
  rt_prio = (tprio_t)256 - (tprio_t)priority;
  if (rt_prio == 1) {
    rt_prio = 2;
  }

  thread_descriptor_t td = {
    task_name,
    (stkalign_t *)stack_pointer,
    (stkalign_t *)((uint8_t *)stack_pointer + stack_size),
    rt_prio,
    (tfunc_t)function_pointer,
    NULL
  };

  /* Creating the task and detaching it, other APIs will have to gain a
     reference using the registry API.*/
  tp = chThdCreate(&td);
  chThdRelease(tp);

  /* Storing the task id.*/
  *task_id = (uint32)tp;

  return OS_SUCCESS;
}

/**
 * @brief   Installs a deletion handler.
 * @note    It is implemented as hooks in chconf.h.
 *
 * @param[in] function_pointer  the handler function
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TaskInstallDeleteHandler(void *function_pointer) {

  chThdGetSelfX()->osal_delete_handler = function_pointer;

  return OS_SUCCESS;
}

/**
 * @brief   Task delete.
 * @note    Limitation, it does not actually kill the thread, it just sets a
 *          flag in the thread that has then to terminate volountarly. The
 *          flag can be checked using @p chThdShouldTerminateX().
 *
 * @param[in] task_id           the task id
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TaskDelete(uint32 task_id) {
  thread_t *tp = (thread_t *)task_id;
  funcptr_t fp;

  /* Check for thread validity, getting a reference.*/
  if (chRegFindThreadByPointer(tp) == NULL) {
    return OS_ERR_INVALID_ID;
  }

  /* Asking for thread termination.*/
  chThdTerminate(tp);

  /* Getting the delete handler while the thread is still referenced.*/
  fp = (funcptr_t)tp->osal_delete_handler;

  /* Waiting for termination, releasing the reference.*/
  chThdWait(tp);

  /* Calling the delete handler, if defined.*/
  if (fp != NULL) {
    fp();
  }

  return OS_SUCCESS;
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
 * @note    Priority 255 is not available and it is transformed internally in
 *          254.
 *
 * @param[in] task_id           the task id
 * @param[in] new_priority      the task new priority
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TaskSetPriority(uint32 task_id, uint32 new_priority) {
  tprio_t rt_newprio;
  thread_t *tp = (thread_t *)task_id;

  /* Checking priority range.*/
  if ((new_priority < MIN_PRIORITY) || (new_priority > MAX_PRIORITY)) {
    return OS_ERR_INVALID_PRIORITY;
  }

  /* Converting priority to RT type.*/
  rt_newprio = (tprio_t)256 - (tprio_t)new_priority;
  if (rt_newprio == 1) {
    rt_newprio = 2;
  }

  if (chThdGetPriorityX() == rt_newprio) {
    return OS_SUCCESS;
  }

  /* Check for thread validity.*/
  if (chRegFindThreadByPointer(tp) == NULL) {
    return OS_ERR_INVALID_ID;
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

  /* Releasing the thread reference.*/
  chThdRelease(tp);

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
 * @note    This function can be safely called from timer callbacks or ISRs.
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
 *
 * @param[out] task_id          pointer to a task id variable
 * @param[in] task_name         the task name
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TaskGetIdByName(uint32 *task_id, const char *task_name) {
  thread_t *tp;

  /* NULL pointer checks.*/
  if ((task_id == NULL) || (task_name == NULL)) {
    return OS_INVALID_POINTER;
  }

  /* Checking task name length.*/
  if (strlen(task_name) >= OS_MAX_API_NAME) {
    return OS_ERR_NAME_TOO_LONG;
  }

  /* Searching in the registry.*/
  tp = chRegFindThreadByName(task_name);
  if (tp == NULL) {
    return OS_ERR_NAME_NOT_FOUND;
  }

  *task_id = (uint32)tp;

  /* Releasing the thread reference.*/
  chThdRelease(tp);

  return OS_SUCCESS;
}

/**
 * @brief   Returns task information.
 * @note    This function can be safely called from timer callbacks or ISRs.
 * @note    It is not currently implemented.
 * @note    Priority 255 is not available and it is transformed internally in
 *          254.
 *
 * @param[in] task_id           the task id
 * @param[in] task_prop         task properties
 * @return                      An error code.
 *
 * @api
 */
int32 OS_TaskGetInfo(uint32 task_id, OS_task_prop_t *task_prop) {
  thread_t *tp = (thread_t *)task_id;
  size_t wasize = (size_t)tp - (size_t)tp->stklimit + sizeof (thread_t);

  /* NULL pointer checks.*/
  if (task_prop == NULL) {
    return OS_INVALID_POINTER;
  }

  /* Check for thread validity.*/
  if (chRegFindThreadByPointer(tp) == NULL) {
    return OS_ERR_INVALID_ID;
  }

  strncpy(task_prop->name, tp->name, OS_MAX_API_NAME - 1);
  task_prop->creator    = (uint32)chSysGetIdleThreadX();
  task_prop->stack_size = (uint32)MEM_ALIGN_NEXT(wasize, PORT_STACK_ALIGN);
  task_prop->priority   = (uint32)256U - (uint32)tp->realprio;
  task_prop->OStask_id  = task_id;

  /* Releasing the thread reference.*/
  chThdRelease(tp);

  return OS_SUCCESS;
}

/** @} */

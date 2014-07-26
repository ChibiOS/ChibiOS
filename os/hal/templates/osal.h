/*
    ChibiOS/RT - Copyright (C) 2006-2014 Giovanni Di Sirio

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
 * @file    osal.h
 * @brief   OSAL module header.
 *
 * @addtogroup OSAL
 * @{
 */

#ifndef _OSAL_H_
#define _OSAL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Common constants
 * @{
 */
#if !defined(FALSE) || defined(__DOXYGEN__)
#define FALSE                               0
#endif

#if !defined(TRUE) || defined(__DOXYGEN__)
#define TRUE                                (!FALSE)
#endif

#define OSAL_SUCCESS                        FALSE
#define OSAL_FAILED                         TRUE
/** @} */

/**
 * @name    Messages
 * @{
 */
#define MSG_OK              0
#define MSG_RESET           -1
#define MSG_TIMEOUT         -2
/** @} */

/**
 * @name    Special time constants
 * @{
 */
#define TIME_IMMEDIATE      ((systime_t)0)
#define TIME_INFINITE       ((systime_t)-1)
/** @} */

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
 * @brief   Type of a machine status register.
 */
typedef uint32_t osal_sts_t;

/**
 * @brief   Type of a message.
 */
typedef int32_t msg_t;

/**
 * @brief   Type of system time counter.
 */
typedef uint32_t systime_t;

/**
 * @brief   Type of a thread reference.
 */
typedef thread_t * thread_reference_t;

/**
 * @brief   Type of an event flags mask.
 */
typedef uint32_t eventflags_t;

/**
 * @brief   Type of an event flags object.
 * @note    The content of this structure is not part of the API and should
 *          not be relied upon. Implementers may define this structure in
 *          an entirely different way.
 * @note    Retrieval and clearing of the flags are not defined in this
 *          API and are implementation-dependent.
 */
typedef struct {
  volatile eventflags_t flags;      /**< @brief Flags stored into the
                                                object.                     */
} eventsource_t;

/**
 * @brief   Type of a mutex.
 * @note    If the OS does not support mutexes or there is no OS then them
 *          mechanism can be simulated.
 */
typedef uint32_t mutex_t;

/**
 * @brief   Type of a thread queue.
 * @details A thread queue is a queue of sleeping threads, queued threads
 *          can be dequeued one at time or all together.
 * @note    In this implementation it is implemented as a single reference
 *          because there are no real threads.
 */
typedef struct {
  thread_reference_t    tr;
} threads_queue_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Condition assertion.
 * @details If the condition check fails then the OSAL panics with the
 *          specified message and halts.
 * @note    The condition is tested only if the @p OSAL_ENABLE_ASSERTIONS
 *          switch is enabled.
 * @note    The convention for the message is the following:<br>
 *          @<function_name@>(), #@<assert_number@>
 * @note    The remark string is not currently used except for putting a
 *          comment in the code about the assertion.
 *
 * @param[in] c         the condition to be verified to be true
 * @param[in] msg       the text message
 * @param[in] remark    a remark string
 *
 * @api
 */
#define osalDbgAssert(c, msg, remark)

/**
 * @brief   Function parameters check.
 * @details If the condition check fails then the OSAL panics and halts.
 * @note    The condition is tested only if the @p OSAL_ENABLE_CHECKS switch
 *          is enabled.
 *
 * @param[in] c         the condition to be verified to be true
 *
 * @api
 */
#define osalDbgCheck(c)

/**
 * @brief   I-Class state check.
 * @note    Not implemented in this simplified OSAL.
 */
#define osalDbgCheckClassI()

/**
 * @brief   S-Class state check.
 * @note    Not implemented in this simplified OSAL.
 */
#define osalDbgCheckClassS()

/**
 * @brief   IRQ prologue code.
 * @details This macro must be inserted at the start of all IRQ handlers.
 */
#define OSAL_IRQ_PROLOGUE()

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers.
 */
#define OSAL_IRQ_EPILOGUE()

/**
 * @brief   IRQ handler function declaration.
 * @details This macro hides the details of an ISR function declaration.
 *
 * @param[in] id        a vector name as defined in @p vectors.s
 */
#define OSAL_IRQ_HANDLER(id) void id(void)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void osalInit(void);
  void osalSysHalt(const char *reason);
  msg_t osalQueueGoSleepTimeoutS(threads_queue_t *tqp, systime_t time);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Enters a critical zone from thread context.
 * @note    This function cannot be used for reentrant critical zones.
 *
 * @special
 */
static inline void osalSysLock(void) {

}

/**
 * @brief   Leaves a critical zone from thread context.
 * @note    This function cannot be used for reentrant critical zones.
 *
 * @special
 */
static inline void osalSysUnlock(void) {

}

/**
 * @brief   Enters a critical zone from ISR context.
 * @note    This function cannot be used for reentrant critical zones.
 *
 * @special
 */
static inline void osalSysLockFromISR(void) {

}

/**
 * @brief   Leaves a critical zone from ISR context.
 * @note    This function cannot be used for reentrant critical zones.
 *
 * @special
 */
static inline void osalSysUnlockFromISR(void) {

}

/**
 * @brief   Enters a critical zone returning the previous machine status.
 *
 * @return  The previous status.
 *
 * @special
 */
static inline osal_sts_t osalSysGetStatusAndLock(void) {

}

/**
 * @brief   Restores a machine status.
 *
 * @param[in] sts       the previous status. This value must be something
 *                      returned by the function @p osalSysGetStatusAndLock().
 *                      Arbitrary values are not allowed.
 *
 * @special
 */
static inline void osalSysSetStatus(osal_sts_t sts) {

}

/**
 * @brief   Checks if a reschedule is required and performs it.
 * @note    I-Class functions invoked from thread context must not reschedule
 *          by themselves, an explicit reschedule using this function is
 *          required in this scenario.
 * @note    Not implemented in this simplified OSAL.
 *
 * @sclass
 */
static inline void osalOsRescheduleS(void) {

}

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] time      the delay in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE is allowed but interpreted as a
 *                        normal time specification.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 *
 * @sclass
 */
inline void osalThreadSleepS(systime_t time) {

}

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] time      the delay in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE is allowed but interpreted as a
 *                        normal time specification.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 *
 * @api
 */
void osalThreadSleep(systime_t time) {

}

/**
 * @brief   Initializes an event flags object.
 *
 * @param[out] esp      pointer to the event flags object
 *
 * @init
 */
static inline void osalEventInit(eventsource_t *esp) {

  osalDbgCheck(esp != NULL);

  esp->flags = 0;
}

/**
 * @brief   Add flags to an event source object.
 *
 * @param[in] esp       pointer to the event flags object
 * @param[in] flags     flags to be ORed to the flags mask
 *
 * @iclass
 */
static inline void osalEventBroadcastFlagsI(eventsource_t *esp,
                                            eventflags_t flags) {

  osalDbgCheck(esp != NULL);

  esp->flags |= flags;
}

/**
 * @brief   Add flags to an event source object.
 *
 * @param[in] esp       pointer to the event flags object
 * @param[in] flags     flags to be ORed to the flags mask
 *
 * @iclass
 */
static inline void osalEventBroadcastFlags(eventsource_t *esp,
                                           eventflags_t flags) {

  osalDbgCheck(esp != NULL);

  osalSysLock();
  esp->flags |= flags;
  osalSysUnlock();
}

/**
 * @brief   Returns the flags associated to the event object then clears them.
 * @note    This function is not part of the OSAL API and is provided
 *          exclusively as an example and for convenience.
 *
 * @param[in] esp       pointer to the event flags object
 * @return              The flags.
 *
 * @iclass
 */
static inline eventflags_t osalEventGetAndClearFlagsI(eventsource_t *esp) {
  eventflags_t flags;

  osalDbgCheck(esp != NULL);

  flags = esp->flags;
  esp->flags = 0;
  return flags;
}

/**
 * @brief   Returns the flags associated to the event object and clears them.
 * @note    This function is not part of the OSAL API and is provided
 *          exclusively as an example and for convenience.
 *
 * @param[in] esp       pointer to the event flags object
 * @return              The flags.
 *
 * @api
 */
static inline eventflags_t osalEventGetAndClearFlags(eventsource_t *esp) {
  eventflags_t flags;

  osalSysLock();
  flags = osalEventGetAndClearFlagsI(esp);
  osalSysUnlock();
  return flags;
}

/**
 * @brief   Initializes s @p mutex_t object.
 *
 * @param[out] mp       pointer to the @p mutex_t object
 *
 * @init
 */
static inline void osalMutexInit(mutex_t *mp) {

  *mp = 0;
}

/*
 * @brief   Locks the specified mutex.
 * @post    The mutex is locked and inserted in the per-thread stack of owned
 *          mutexes.
 *
 * @param[in,out] mp    pointer to the @p mutex_t object
 *
 * @api
 */
static inline void osalMutexLock(mutex_t *mp) {

  *mp = 1;
}

/**
 * @brief   Unlocks the specified mutex.
 * @note    The HAL guarantees to release mutex in reverse lock order. The
 *          mutex being unlocked is guaranteed to be the last locked mutex
 *          by the invoking thread.
 *          The implementation can rely on this behavior and eventually
 *          ignore the @p mp parameter which is supplied in order to support
 *          those OSes not supporting a stack of the owned mutexes.
 *
 * @param[in,out] mp    pointer to the @p mutex_t object
 *
 * @api
 */
static inline void osalMutexUnlock(mutex_t *mp) {

  *mp = 0;
}

/**
 * @brief   Initializes a threads queue object.
 *
 * @param[out] tqp      pointer to the threads queue object
 *
 * @init
 */
static inline void osalQueueInit(threads_queue_t *tqp) {

}

/**
 * @brief   Dequeues and wakes up one thread from the queue, if any.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] msg       the message code
 *
 * @iclass
 */
static inline void osalQueueWakeupOneI(threads_queue_t *tqp, msg_t msg) {

}

/**
 * @brief   Dequeues and wakes up all threads from the queue.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] msg       the message code
 *
 * @iclass
 */
static inline void osalQueueWakeupAllI(threads_queue_t *tqp, msg_t msg) {

}

#endif /* _OSAL_H_ */

/** @} */

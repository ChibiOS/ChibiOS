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

/**
 * @file    chthreads.h
 * @brief   Threads module macros and structures.
 *
 * @addtogroup threads
 * @{
 */

#ifndef _CHTHREADS_H_
#define _CHTHREADS_H_

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Thread states
 * @{
 */
#define CH_STATE_READY          0   /**< @brief Waiting on the ready list.  */
#define CH_STATE_CURRENT        1   /**< @brief Currently running.          */
#define CH_STATE_WTSTART        2   /**< @brief Created but not started.    */
#define CH_STATE_SUSPENDED      3   /**< @brief Suspended state.            */
#define CH_STATE_QUEUED         4   /**< @brief Waiting on an I/O queue.    */
#define CH_STATE_WTSEM          5   /**< @brief Waiting on a semaphore.     */
#define CH_STATE_WTMTX          6   /**< @brief Waiting on a mutex.         */
#define CH_STATE_WTCOND         7   /**< @brief Waiting on a condition
                                         variable.                          */
#define CH_STATE_SLEEPING       8   /**< @brief Waiting in @p chThdSleep()
                                         or @p chThdSleepUntil().           */
#define CH_STATE_WTEXIT         9   /**< @brief Waiting in @p chThdWait().  */
#define CH_STATE_WTOREVT        10  /**< @brief Waiting for an event.       */
#define CH_STATE_WTANDEVT       11  /**< @brief Waiting for several events. */
#define CH_STATE_SNDMSGQ        12  /**< @brief Sending a message, in queue.*/
#define CH_STATE_SNDMSG         13  /**< @brief Sent a message, waiting
                                         answer.                            */
#define CH_STATE_WTMSG          14  /**< @brief Waiting for a message.      */
#define CH_STATE_FINAL          15  /**< @brief Thread terminated.          */

/**
 * @brief   Thread states as array of strings.
 * @details Each element in an array initialized with this macro can be
 *          indexed using the numeric thread state values.
 */
#define CH_STATE_NAMES                                                     \
  "READY", "CURRENT", "WTSTART", "SUSPENDED", "QUEUED", "WTSEM", "WTMTX",  \
  "WTCOND", "SLEEPING", "WTEXIT", "WTOREVT", "WTANDEVT", "SNDMSGQ",        \
  "SNDMSG", "WTMSG", "FINAL"
/** @} */

/**
 * @name    Thread flags and attributes
 * @{
 */
#define CH_FLAG_MODE_MASK       3   /**< @brief Thread memory mode mask.    */
#define CH_FLAG_MODE_STATIC     0   /**< @brief Static thread.              */
#define CH_FLAG_MODE_HEAP       1   /**< @brief Thread allocated from a
                                         Memory Heap.                       */
#define CH_FLAG_MODE_MEMPOOL    2   /**< @brief Thread allocated from a
                                         Memory Pool.                       */
#define CH_FLAG_TERMINATE       4   /**< @brief Termination requested flag. */
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
 * @brief   Type of a thread reference.
 */
typedef thread_t * thread_reference_t;

/**
 * @brief   Thread function.
 */
typedef msg_t (*tfunc_t)(void *);

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Working Areas and Alignment
 */
/**
 * @brief   Enforces a correct alignment for a stack area size value.
 *
 * @param[in] n         the stack size to be aligned to the next stack
 *                      alignment boundary
 * @return              The aligned stack size.
 *
 * @api
 */
#define THD_ALIGN_STACK_SIZE(n)                                             \
  ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

/**
 * @brief   Calculates the total Working Area size.
 *
 * @param[in] n         the stack size to be assigned to the thread
 * @return              The total used memory in bytes.
 *
 * @api
 */
#define THD_WORKING_AREA_SIZE(n)                                            \
  THD_ALIGN_STACK_SIZE(sizeof(thread_t) + PORT_WA_SIZE(n))

/**
 * @brief   Static working area allocation.
 * @details This macro is used to allocate a static thread working area
 *          aligned as both position and size.
 *
 * @param[in] s         the name to be assigned to the stack array
 * @param[in] n         the stack size to be assigned to the thread
 *
 * @api
 */
#define THD_WORKING_AREA(s, n)                                              \
  stkalign_t s[THD_WORKING_AREA_SIZE(n) / sizeof(stkalign_t)]
/** @} */

/**
 * @name    Threads abstraction macros
 */
/**
 * @brief   Thread declaration macro.
 * @note    Thread declarations should be performed using this macro because
 *          the port layer could define optimizations for thread functions.
 */
#define THD_FUNCTION(tname, arg) PORT_THD_FUNCTION(tname, arg)
/** @} */

/**
 * @name    Threads queues
 */
/**
 * @brief   Data part of a static threads queue object initializer.
 * @details This macro should be used when statically initializing a threads
 *          queue that is part of a bigger structure.
 *
 * @param[in] name      the name of the threads queue variable
 */
#define _THREADS_QUEUE_DATA(name) {(thread_t *)&name, (thread_t *)&name}

/**
 * @brief   Static threads queue object initializer.
 * @details Statically initialized threads queues require no explicit
 *          initialization using @p queue_init().
 *
 * @param[in] name      the name of the threads queue variable
 */
#define _THREADS_QUEUE_DECL(name)                                           \
  threads_queue_t name = _THREADS_QUEUE_DATA(name)
/** @} */

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Delays the invoking thread for the specified number of seconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 * @note    The maximum specifiable value is implementation dependent.
 *
 * @param[in] sec       time in seconds, must be different from zero
 *
 * @api
 */
#define chThdSleepSeconds(sec) chThdSleep(S2ST(sec))

/**
 * @brief   Delays the invoking thread for the specified number of
 *          milliseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 * @note    The maximum specifiable value is implementation dependent.
 *
 * @param[in] msec      time in milliseconds, must be different from zero
 *
 * @api
 */
#define chThdSleepMilliseconds(msec) chThdSleep(MS2ST(msec))

/**
 * @brief   Delays the invoking thread for the specified number of
 *          microseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 * @note    The maximum specifiable value is implementation dependent.
 *
 * @param[in] usec      time in microseconds, must be different from zero
 *
 * @api
 */
#define chThdSleepMicroseconds(usec) chThdSleep(US2ST(usec))
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
   thread_t *_thread_init(thread_t *tp, tprio_t prio);
#if CH_DBG_FILL_THREADS
  void _thread_memfill(uint8_t *startp, uint8_t *endp, uint8_t v);
#endif
  thread_t *chThdCreateI(void *wsp, size_t size,
                         tprio_t prio, tfunc_t pf, void *arg);
  thread_t *chThdCreateStatic(void *wsp, size_t size,
                              tprio_t prio, tfunc_t pf, void *arg);
  thread_t *chThdStart(thread_t *tp);
  tprio_t chThdSetPriority(tprio_t newprio);
  msg_t chThdSuspendS(thread_reference_t *trp);
  msg_t chThdSuspendTimeoutS(thread_reference_t *trp, systime_t timeout);
  void chThdResumeI(thread_reference_t *trp, msg_t msg);
  void chThdResumeS(thread_reference_t *trp, msg_t msg);
  void chThdResume(thread_reference_t *trp, msg_t msg);
  msg_t chThdEnqueueTimeoutS(threads_queue_t *tqp, systime_t timeout);
  void chThdDequeueNextI(threads_queue_t *tqp, msg_t msg);
  void chThdDequeueAllI(threads_queue_t *tqp, msg_t msg);
  void chThdTerminate(thread_t *tp);
  void chThdSleep(systime_t time);
  void chThdSleepUntil(systime_t time);
  systime_t chThdSleepUntilWindowed(systime_t prev, systime_t next);
  void chThdYield(void);
  void chThdExit(msg_t msg);
  void chThdExitS(msg_t msg);
#if CH_CFG_USE_WAITEXIT
  msg_t chThdWait(thread_t *tp);
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

 /**
  * @brief   Returns a pointer to the current @p thread_t.
  *
  * @xclass
  */
static inline thread_t *chThdGetSelfX(void) {

  return ch.rlist.r_current;
}

/**
 * @brief   Returns the current thread priority.
 * @note    Can be invoked in any context.
 *
 * @xclass
 */
static inline tprio_t chThdGetPriorityX(void) {

  return chThdGetSelfX()->p_prio;
}

/**
 * @brief   Returns the number of ticks consumed by the specified thread.
 * @note    This function is only available when the
 *          @p CH_DBG_THREADS_PROFILING configuration option is enabled.
 *
 * @param[in] tp        pointer to the thread
 *
 * @xclass
 */
#if CH_DBG_THREADS_PROFILING || defined(__DOXYGEN__)
static inline systime_t chThdGetTicksX(thread_t *tp) {

  return tp->p_time;
}
#endif

/**
 * @brief   Verifies if the specified thread is in the @p CH_STATE_FINAL state.
 *
 * @param[in] tp        pointer to the thread
 * @retval true         thread terminated.
 * @retval false        thread not terminated.
 *
 * @xclass
 */
static inline bool chThdTerminatedX(thread_t *tp) {

  return (bool)(tp->p_state == CH_STATE_FINAL);
}

/**
 * @brief   Verifies if the current thread has a termination request pending.
 *
 * @retval true         termination request pending.
 * @retval false        termination request not pending.
 *
 * @xclass
 */
static inline bool chThdShouldTerminateX(void) {

  return (bool)((chThdGetSelfX()->p_flags & CH_FLAG_TERMINATE) != 0);
}

/**
 * @brief   Resumes a thread created with @p chThdCreateI().
 *
 * @param[in] tp        pointer to the thread
 *
 * @iclass
 */
static inline thread_t *chThdStartI(thread_t *tp) {

  chDbgAssert(tp->p_state == CH_STATE_WTSTART, "wrong state");

  return chSchReadyI(tp);
}

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] time      the delay in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 *
 * @sclass
 */
static inline void chThdSleepS(systime_t time) {

  chDbgCheck(time != TIME_IMMEDIATE);

  chSchGoSleepTimeoutS(CH_STATE_SLEEPING, time);
}

/**
 * @brief   Initializes a threads queue object.
 *
 * @param[out] tqp      pointer to the threads queue object
 *
 * @init
 */
static inline void chThdQueueObjectInit(threads_queue_t *tqp) {

  queue_init(tqp);
}

/**
 * @brief   Evaluates to @p true if the specified queue is empty.
 *
 * @param[out] tqp      pointer to the threads queue object
 * @return              The queue status.
 * @retval false        if the queue is not empty.
 * @retval true         if the queue is empty.
 *
 * @iclass
 */
static inline bool chThdQueueIsEmptyI(threads_queue_t *tqp) {

  chDbgCheckClassI();

  return queue_isempty(tqp);
}


/**
 * @brief   Dequeues and wakes up one thread from the threads queue object.
 * @details Dequeues one thread from the queue without checking if the queue
 *          is empty.
 * @pre     The queue must contain at least an object.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] msg       the message code
 *
 * @iclass
 */
static inline void chThdDoDequeueNextI(threads_queue_t *tqp, msg_t msg) {
  thread_t *tp;

  chDbgAssert(queue_notempty(tqp), "empty queue");

  tp = queue_fifo_remove(tqp);

  chDbgAssert(tp->p_state == CH_STATE_QUEUED, "invalid state");

  tp->p_u.rdymsg = msg;
  chSchReadyI(tp);
}

#endif /* _CHTHREADS_H_ */

/** @} */

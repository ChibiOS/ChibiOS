/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @file threads.h
 * @brief Threads macros and structures.
 * @addtogroup threads
 * @{
 */

#ifndef _THREADS_H_
#define _THREADS_H_

/*
 * Module dependencies check.
 */
#if CH_USE_DYNAMIC && !CH_USE_WAITEXIT
#error "CH_USE_DYNAMIC requires CH_USE_WAITEXIT"
#endif
#if CH_USE_DYNAMIC && !CH_USE_HEAP && !CH_USE_MEMPOOLS
#error "CH_USE_DYNAMIC requires CH_USE_HEAP and/or CH_USE_MEMPOOLS"
#endif

/**
 * @brief Structure representing a thread.
 *
 * @extends ThreadsQueue
 * @note Not all the listed fields are always needed, by switching off some
 *       not needed ChibiOS/RT subsystems it is possible to save RAM space by
 *       shrinking the @p Thread structure.
 */
struct Thread {
  Thread                *p_next;        /**< Next @p Thread in the threads
                                             list/queue.                    */
  /* End of the fields shared with the ThreadsList structure. */
  Thread                *p_prev;        /**< Previous @p Thread in the threads
                                             queue.                         */
  /* End of the fields shared with the ThreadsQueue structure. */
  tprio_t               p_prio;         /**< Thread priority.               */
  struct context        p_ctx;          /**< Processor context.             */
#if CH_USE_REGISTRY
  Thread                *p_newer;       /**< Newer registry element.        */
  Thread                *p_older;       /**< Older registry element.        */
#endif
  /* End of the fields shared with the ReadyList structure. */
#if CH_USE_DYNAMIC
  trefs_t               p_refs;         /**< References to this thread.     */
#endif
  tstate_t              p_state;        /**< Current thread state.          */
  tmode_t               p_flags;        /**< Various thread flags.          */
#if CH_USE_NESTED_LOCKS
  cnt_t                 p_locks;        /**< Number of nested locks.        */
#endif
#if CH_DBG_THREADS_PROFILING
  volatile systime_t    p_time;         /**< Thread consumed time in ticks.
                                             @note This field can overflow. */
#endif
  union {
    msg_t               rdymsg;         /**< Thread wakeup code.            */
    msg_t               exitcode;       /**< The thread exit code
                                             (@p THD_STATE_FINAL state).    */
    void                *wtobjp;        /**< Generic kernel object pointer. */
#if CH_USE_EVENTS
    eventmask_t         ewmask;         /**< Enabled events mask
                                             (@p THD_STATE_WTOREVT or
                                             @p THD_STATE_WTANDEVT states). */
#endif
  }                     p_u;            /**< State-specific fields.         */
#if CH_USE_WAITEXIT
  ThreadsList           p_waiting;      /**< Termination waiting list.      */
#endif
#if CH_USE_MESSAGES
  ThreadsQueue          p_msgqueue;     /**< Messages queue.                */
  msg_t                 p_msg;          /**< Thread message.                */
#endif
#if CH_USE_EVENTS
  eventmask_t           p_epending;     /**< Pending events mask.           */
#endif
#if CH_USE_MUTEXES
  Mutex                 *p_mtxlist;     /**< List of the mutexes owned by this
                                             thread, @p NULL terminated.    */
  tprio_t               p_realprio;     /**< Thread's own, non-inherited,
                                             priority.                      */
#endif
#if CH_USE_DYNAMIC && CH_USE_MEMPOOLS
  void                  *p_mpool;       /**< Memory Pool where the thread
                                             workspace is returned.         */
#endif
  /* Extra fields defined in chconf.h */
  THREAD_EXT_FIELDS
};

/** Thread state: Ready to run, waiting on the ready list.*/
#define THD_STATE_READY             0
/** Thread state: Currently running.*/
#define THD_STATE_CURRENT           1
/** Thread state: Thread created in suspended state.*/
#define THD_STATE_SUSPENDED         2
/** Thread state: Waiting on a semaphore.*/
#define THD_STATE_WTSEM             3
/** Thread state: Waiting on a mutex.*/
#define THD_STATE_WTMTX             4
/** Thread state: Waiting in @p chCondWait().*/
#define THD_STATE_WTCOND            5
/** Thread state: Waiting in @p chThdSleep() or @p chThdSleepUntil().*/
#define THD_STATE_SLEEPING          6
/** Thread state: Waiting in @p chThdWait().*/
#define THD_STATE_WTEXIT            7
/** Thread state: Waiting in @p chEvtWaitOneTimeout() or
    @p chEvtWaitAnyTimeout().*/
#define THD_STATE_WTOREVT           8
/** Thread state: Waiting in @p chEvtWaitAllTimeout().*/
#define THD_STATE_WTANDEVT          9
/** Thread state: Waiting in @p chMsgSend().*/
#define THD_STATE_SNDMSG            10
/** Thread state: Waiting in @p chMsgWait().*/
#define THD_STATE_WTMSG             11
/** Thread state: After termination.*/
#define THD_STATE_FINAL             12

/*
 * Various flags into the thread p_flags field.
 */
#define THD_MEM_MODE_MASK           3       /**< Thread memory mode mask.   */
#define THD_MEM_MODE_STATIC         0       /**< Thread memory mode: static.*/
#define THD_MEM_MODE_HEAP           1       /**< Thread memory mode: heap.  */
#define THD_MEM_MODE_MEMPOOL        2       /**< Thread memory mode: pool.  */
#define THD_TERMINATE               4       /**< Termination requested.     */

/* Not an API, don't use into the application code.*/
Thread *init_thread(Thread *tp, tprio_t prio);

/** Thread function.*/
typedef msg_t (*tfunc_t)(void *);

/*
 * Threads APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif
  Thread *chThdInit(void *wsp, size_t size,
                    tprio_t prio, tfunc_t pf, void *arg);
  Thread *chThdCreateStatic(void *wsp, size_t size,
                            tprio_t prio, tfunc_t pf, void *arg);
#if CH_USE_DYNAMIC && CH_USE_WAITEXIT && CH_USE_HEAP
  Thread *chThdCreateFromHeap(MemoryHeap *heapp, size_t size,
                              tprio_t prio, tfunc_t pf, void *arg);
#endif
#if CH_USE_DYNAMIC && CH_USE_WAITEXIT && CH_USE_MEMPOOLS
  Thread *chThdCreateFromMemoryPool(MemoryPool *mp, tprio_t prio,
                                    tfunc_t pf, void *arg);
#endif
  tprio_t chThdSetPriority(tprio_t newprio);
  Thread *chThdResume(Thread *tp);
  void chThdTerminate(Thread *tp);
  void chThdSleep(systime_t time);
  void chThdSleepUntil(systime_t time);
  void chThdYield(void);
  void chThdExit(msg_t msg);
#if CH_USE_DYNAMIC
  Thread *chThdAddRef(Thread *tp);
  void chThdRelease(Thread *tp);
#endif
#if CH_USE_WAITEXIT
  msg_t chThdWait(Thread *tp);
#endif
#ifdef __cplusplus
}
#endif

/**
 * Returns a pointer to the current @p Thread.
 */
#define chThdSelf() currp

/**
 * Returns the current thread priority.
 */
#define chThdGetPriority() (currp->p_prio)

/**
 * Returns the pointer to the @p Thread local storage area, if any.
 */
#define chThdLS() (void *)(currp + 1)

/**
 * Verifies if the specified thread is in the @p THD_STATE_FINAL state.
 *
 * @param[in] tp the pointer to the thread
 * @retval TRUE thread terminated.
 * @retval FALSE thread not terminated.
 */
#define chThdTerminated(tp) ((tp)->p_state == THD_STATE_FINAL)

/**
 * Verifies if the current thread has a termination request pending.
 *
 * @retval TRUE termination request pended.
 * @retval FALSE termination request not pended.
 */
#define chThdShouldTerminate() (currp->p_flags & THD_TERMINATE)

/**
 * Resumes a thread created with @p chThdInit().
 *
 * @param[in] tp the pointer to the thread
 */
#define chThdResumeI(tp) chSchReadyI(tp)

/**
 * Suspends the invoking thread for the specified time.
 *
 * @param[in] time the delay in system ticks, the special values are handled as
 *                 follow:
 *                 - @a TIME_INFINITE the thread enters an infinite sleep
 *                   state.
 *                 - @a TIME_IMMEDIATE this value is accepted but interpreted
 *                   as a normal time specification not as an immediate timeout
 *                   specification.
 *                 .
 */
#define chThdSleepS(time) chSchGoSleepTimeoutS(THD_STATE_SLEEPING, time)

/**
 * Delays the invoking thread for the specified number of seconds.
 *
 * @param[in] sec the time in seconds
 * @note The specified time is rounded up to a value allowed by the real
 *       system clock.
 * @note The maximum specified value is implementation dependent.
 */
#define chThdSleepSeconds(sec) chThdSleep(S2ST(sec))

/**
 * Delays the invoking thread for the specified number of milliseconds.
 *
 * @param[in] msec the time in milliseconds
 * @note The specified time is rounded up to a value allowed by the real
 *       system clock.
 * @note The maximum specified value is implementation dependent.
 */
#define chThdSleepMilliseconds(msec) chThdSleep(MS2ST(msec))

/**
 * Delays the invoking thread for the specified number of microseconds.
 *
 * @param[in] usec the time in microseconds
 * @note The specified time is rounded up to a value allowed by the real
 *       system clock.
 * @note The maximum specified value is implementation dependent.
 */
#define chThdSleepMicroseconds(usec) chThdSleep(US2ST(usec))

#endif /* _THREADS_H_ */

/** @} */

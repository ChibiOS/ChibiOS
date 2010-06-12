/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    chthreads.h
 * @brief   Threads macros and structures.
 *
 * @addtogroup threads
 * @{
 */

#ifndef _CHTHREADS_H_
#define _CHTHREADS_H_

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
 * @extends ThreadsQueue
 *
 * @brief   Structure representing a thread.
 * @note    Not all the listed fields are always needed, by switching off some
 *          not needed ChibiOS/RT subsystems it is possible to save RAM space
 *          by shrinking the @p Thread structure.
 */
struct Thread {
  Thread                *p_next;    /**< @brief Next in the list/queue.     */
  /* End of the fields shared with the ThreadsList structure. */
  Thread                *p_prev;    /**< @brief Previous in the queue.      */
  /* End of the fields shared with the ThreadsQueue structure. */
  tprio_t               p_prio;     /**< @brief Thread priority.            */
  struct context        p_ctx;      /**< @brief Processor context.          */
#if CH_USE_REGISTRY
  Thread                *p_newer;   /**< @brief Newer registry element.     */
  Thread                *p_older;   /**< @brief Older registry element.     */
#endif
  /* End of the fields shared with the ReadyList structure. */
  /**
   * @brief Current thread state.
   */
  tstate_t              p_state;
  /**
   * @brief Various thread flags.
   */
  tmode_t               p_flags;
#if CH_USE_DYNAMIC
  /**
   * @brief References to this thread.
   */
  trefs_t               p_refs;
#endif
#if CH_USE_NESTED_LOCKS
  /**
   * @brief Number of nested locks.
   */
  cnt_t                 p_locks;
#endif
#if CH_DBG_THREADS_PROFILING
  /**
   * @brief Thread consumed time in ticks.
   * @note This field can overflow.
   */
  volatile systime_t    p_time;
#endif
  /**
   * @brief State-specific fields.
   * @note  All the fields declared in this union are only valid in the
   *        specified state or condition and are thus volatile.
   */
  union {
    /**
     * @brief Thread wakeup code.
     * @note  This field contains the low level message sent to the thread
     *        by the waking thread or interrupt handler. The value is valid
     *        after exiting the @p chSchWakeupS() function.
     */
    msg_t               rdymsg;
    /**
     * @brief Thread exit code.
     * @note  The thread termination code is stored in this field in order
     *        to be retrieved by the thread performing a @p chThdWait() on
     *        this thread.
     */
    msg_t               exitcode;
    /**
     * @brief Pointer to a generic "wait" object.
     * @note  This field is used to get a generic pointer to a synchronization
     *        object and is valid when the thread is in one of the wait
     *        states.
     */
    void                *wtobjp;
#if CH_USE_EVENTS
    /**
     * @brief Enabled events mask.
     * @note  This field is only valied while the thread is in the
     *        @p THD_STATE_WTOREVT or @p THD_STATE_WTANDEVT states.
     */
    eventmask_t         ewmask;
#endif
  }                     p_u;
#if CH_USE_WAITEXIT
  /**
   * @brief Termination waiting list.
   */
  ThreadsList           p_waiting;
#endif
#if CH_USE_MESSAGES
  /**
   * @brief Messages queue.
   */
  ThreadsQueue          p_msgqueue;
  /**
   * @brief Thread message.
   */
  msg_t                 p_msg;
#endif
#if CH_USE_EVENTS
  /**
   * @brief Pending events mask.
   */
  eventmask_t           p_epending;
#endif
#if CH_USE_MUTEXES
  /**
   * @brief List of the mutexes owned by this thread.
   * @note  The list is terminated by a @p NULL in this field.
   */
  Mutex                 *p_mtxlist;
  /**
   * @brief Thread's own, non-inherited, priority.
   */
  tprio_t               p_realprio;
#endif
#if CH_USE_DYNAMIC && CH_USE_MEMPOOLS
  /**
   * @brief Memory Pool where the thread workspace is returned.
   */
  void                  *p_mpool;
#endif
  /* Extra fields defined in chconf.h.*/
  THREAD_EXT_FIELDS
};

/** @brief Thread state: Ready to run, waiting on the ready list.*/
#define THD_STATE_READY         0
/** @brief Thread state: Currently running.*/
#define THD_STATE_CURRENT       1
/** @brief Thread state: Thread created in suspended state.*/
#define THD_STATE_SUSPENDED     2
/** @brief Thread state: Waiting on a semaphore.*/
#define THD_STATE_WTSEM         3
/** @brief Thread state: Waiting on a mutex.*/
#define THD_STATE_WTMTX         4
/** @brief Thread state: Waiting in @p chCondWait().*/
#define THD_STATE_WTCOND        5
/** @brief Thread state: Waiting in @p chThdSleep() or @p chThdSleepUntil().*/
#define THD_STATE_SLEEPING      6
/** @brief Thread state: Waiting in @p chThdWait().*/
#define THD_STATE_WTEXIT        7
/** @brief Thread state: Waiting in @p chEvtWaitXXX().*/
#define THD_STATE_WTOREVT       8
/** @brief Thread state: Waiting in @p chEvtWaitAllTimeout().*/
#define THD_STATE_WTANDEVT      9
/** @brief Thread state: Waiting in @p chMsgSend().*/
#define THD_STATE_SNDMSG        10
/** @brief Thread state: Waiting in @p chMsgWait().*/
#define THD_STATE_WTMSG         11
/** @brief Thread state: After termination.*/
#define THD_STATE_FINAL         12

/*
 * Various flags into the thread p_flags field.
 */
#define THD_MEM_MODE_MASK       3   /**< @brief Thread memory mode mask.    */
#define THD_MEM_MODE_STATIC     0   /**< @brief Thread memory mode: static. */
#define THD_MEM_MODE_HEAP       1   /**< @brief Thread memory mode: heap.   */
#define THD_MEM_MODE_MEMPOOL    2   /**< @brief Thread memory mode: pool.   */
#define THD_TERMINATE           4   /**< @brief Termination requested.      */

/** @brief Thread function.*/
typedef msg_t (*tfunc_t)(void *);

/*
 * Threads APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif
  Thread *init_thread(Thread *tp, tprio_t prio);
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
 * @brief   Returns a pointer to the current @p Thread.
 */
#define chThdSelf() currp

/**
 * @brief   Returns the current thread priority.
 */
#define chThdGetPriority() (currp->p_prio)

/**
 * @brief   Returns the pointer to the @p Thread local storage area, if any.
 */
#define chThdLS() (void *)(currp + 1)

/**
 * @brief   Verifies if the specified thread is in the @p THD_STATE_FINAL state.
 *
 * @param[in] tp        the pointer to the thread
 * @retval TRUE         thread terminated.
 * @retval FALSE        thread not terminated.
 */
#define chThdTerminated(tp) ((tp)->p_state == THD_STATE_FINAL)

/**
 * @brief   Verifies if the current thread has a termination request pending.
 *
 * @retval TRUE         termination request pended.
 * @retval FALSE        termination request not pended.
 */
#define chThdShouldTerminate() (currp->p_flags & THD_TERMINATE)

/**
 * @brief   Resumes a thread created with @p chThdInit().
 *
 * @param[in] tp        the pointer to the thread
 */
#define chThdResumeI(tp) chSchReadyI(tp)

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] time      the delay in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state.
 *                      - @a TIME_IMMEDIATE this value is accepted but
 *                        interpreted as a normal time specification not as
 *                        an immediate timeout specification.
 *                      .
 */
#define chThdSleepS(time) chSchGoSleepTimeoutS(THD_STATE_SLEEPING, time)

/**
 * @brief   Delays the invoking thread for the specified number of seconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system clock.
 * @note    The maximum specified value is implementation dependent.
 *
 * @param[in] sec       the time in seconds
 */
#define chThdSleepSeconds(sec) chThdSleep(S2ST(sec))

/**
 * @brief   Delays the invoking thread for the specified number of
 *          milliseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system clock.
 * @note    The maximum specified value is implementation dependent.
 *
 * @param[in] msec      the time in milliseconds
 */
#define chThdSleepMilliseconds(msec) chThdSleep(MS2ST(msec))

/**
 * @brief   Delays the invoking thread for the specified number of
 *          microseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system clock.
 * @note    The maximum specified value is implementation dependent.
 *
 * @param[in] usec      the time in microseconds
 */
#define chThdSleepMicroseconds(usec) chThdSleep(US2ST(usec))

#endif /* _CHTHREADS_H_ */

/** @} */

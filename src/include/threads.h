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
 * @addtogroup Threads
 * @{
 */

#ifndef _THREADS_H_
#define _THREADS_H_

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
                                             list/queue.*/
  /* End of the fields shared with the ThreadsList structure. */
  Thread                *p_prev;        /**< Previous @p Thread in the thread
                                             queue.*/
  /* End of the fields shared with the ThreadsQueue structure. */
  tprio_t               p_prio;         /**< Thread priority.*/
  /* End of the fields shared with the ReadyList structure. */
  tstate_t              p_state;        /**< Current thread state.*/
  tmode_t               p_flags;        /**< Various flags.*/
  struct context        p_ctx;          /**< Processor context.*/
#ifdef CH_USE_NESTED_LOCKS
  cnt_t                 p_locks;        /**< Number of nested locks.*/
#endif
  /*
   * The following fields are merged in unions because they are all
   * state-specific fields. This trick saves some extra space for each
   * thread in the system.
   */
  union {
    msg_t               p_rdymsg;       /**< Thread wakeup code.*/
    msg_t               p_exitcode;     /**< The thread exit code
                                             (@p PREXIT state).*/
#ifdef CH_USE_SEMAPHORES
    Semaphore           *p_wtsemp;      /**< Semaphore where the thread is
                                             waiting on (@p PRWTSEM state).*/
#endif
#ifdef CH_USE_MUTEXES
    Mutex               *p_wtmtxp;      /**< Mutex where the thread is waiting
                                             on (@p PRWTMTX state).*/
#endif
#ifdef CH_USE_CONDVARS
    CondVar             *p_wtcondp;     /**< CondVar where the thread is
                                             waiting on (@p PRWTCOND state).*/
#endif
#ifdef CH_USE_MESSAGES
    Thread              *p_wtthdp;      /**< Destination thread for message
                                             send @p PRSNDMSG state).*/
#endif
#ifdef CH_USE_EVENTS
    eventmask_t         p_ewmask;       /**< Enabled events mask (@p PRWTOREVT
                                             or @p PRWTANDEVT states).*/
#endif
#ifdef CH_USE_TRACE
    void                *p_wtobjp;      /**< Generic kernel object pointer used
                                             for opaque access.*/
#endif
  };
  /*
   * Start of the optional fields.
   */
#ifdef CH_USE_WAITEXIT
  Thread                *p_waiting;     /**< Thread waiting for termination.*/
#endif
#ifdef CH_USE_MESSAGES
  ThreadsQueue          p_msgqueue;     /**< Message queue.*/
  msg_t                 p_msg;          /**< The message.*/
#endif
#ifdef CH_USE_EVENTS
  eventmask_t           p_epending;     /**< Pending events mask.*/
#endif
#ifdef CH_USE_MUTEXES
  Mutex                 *p_mtxlist;     /**< List of the mutexes owned by this
                                             thread, @p NULL terminated.*/
  tprio_t               p_realprio;     /**< Thread's own, non-inherited,
                                             priority.*/
#endif
#if defined(CH_USE_DYNAMIC) && defined(CH_USE_MEMPOOLS)
  void                  *p_mpool;       /**< Memory Pool where the thread
                                             workspace is returned.*/
#endif
  /* Extra fields defined in chconf.h */
  THREAD_EXT_FIELDS
};

/** Thread state: Ready to run, waiting on the ready list.*/
#define PRREADY         0
/** Thread state: Currently running. */
#define PRCURR          1
/** Thread state: Thread created in suspended state. */
#define PRSUSPENDED     2
/** Thread state: Waiting on a semaphore. */
#define PRWTSEM         3
/** Thread state: Waiting on a mutex. */
#define PRWTMTX         4
/** Thread state: Waiting in @p chThdSleep() or @p chThdSleepUntil(). */
#define PRWTCOND        5
/** Thread state: Waiting in @p chCondWait(). */
#define PRSLEEP         6
/** Thread state: Waiting in @p chThdWait(). */
#define PRWAIT          7
/** Thread state: Waiting in @p chEvtWaitOneTimeout() or
    @p chEvtWaitAnyTimeout(). */
#define PRWTOREVT       8
/** Thread state: Waiting in @p chEvtWaitAllTimeout(). */
#define PRWTANDEVT      9
/** Thread state: Waiting in @p chMsgSend(). */
#define PRSNDMSG        10
/** Thread state: Waiting in @p chMsgWait(). */
#define PRWTMSG         11
/** Thread state: After termination.*/
#define PREXIT          12

/*
 * Various flags into the thread p_flags field.
 */
#define P_MEM_MODE_MASK         3       /* Thread memory mode mask.     */
#define P_MEM_MODE_STATIC       0       /* Thread memory mode: static.  */
#define P_MEM_MODE_HEAP         1       /* Thread memory mode: heap.    */
#define P_MEM_MODE_MEMPOOL      2       /* Thread memory mode: mempool. */
#define P_TERMINATE             4       /* Termination requested.       */

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
  Thread *chThdInit(void *workspace, size_t wsize,
                    tprio_t prio, tfunc_t pf, void *arg);
  Thread *chThdCreateStatic(void *workspace, size_t wsize,
                            tprio_t prio, tfunc_t pf, void *arg);
#if defined(CH_USE_DYNAMIC) && defined(CH_USE_WAITEXIT) && defined(CH_USE_HEAP)
  Thread *chThdCreateFromHeap(size_t wsize, tprio_t prio,
                              tfunc_t pf, void *arg);
#endif
#if defined(CH_USE_DYNAMIC) && defined(CH_USE_WAITEXIT) && defined(CH_USE_MEMPOOLS)
  Thread *chThdCreateFromMemoryPool(MemoryPool *mp, tprio_t prio,
                                    tfunc_t pf, void *arg);
#endif
  void chThdSetPriority(tprio_t newprio);
  Thread *chThdResume(Thread *tp);
  void chThdTerminate(Thread *tp);
  void chThdSleep(systime_t time);
  void chThdSleepUntil(systime_t time);
  void chThdExit(msg_t msg);
#ifdef CH_USE_WAITEXIT
  msg_t chThdWait(Thread *tp);
#endif
#ifdef __cplusplus
}
#endif

/** Returns the pointer to the @p Thread currently in execution.*/
#define chThdSelf() currp

/** Returns the thread priority.*/
#define chThdGetPriority() (currp->p_prio)

/** Returns the pointer to the @p Thread local storage area, if any.*/
#define chThdLS() (void *)(currp + 1)

/** Verifies if the specified thread is in the @p PREXIT state.*/
#define chThdTerminated(tp) ((tp)->p_state == PREXIT)

/**
 * Verifies if the current thread has a termination request pending.
 */
#define chThdShouldTerminate() (currp->p_flags & P_TERMINATE)

/**
 * Resumes a thread created with the @p P_SUSPENDED option or suspended with
 * @p chThdSuspend().
 * @param tp the pointer to the thread
 */
#define chThdResumeI(tp) chSchReadyI(tp)

/**
 * Suspends the invoking thread for the specified time.
 * @param time the delay in system ticks
 */
#define chThdSleepS(time) chSchGoSleepTimeoutS(PRSLEEP, time)

/**
 * Delays the invoking thread for the specified number of seconds.
 * @note The specified time is rounded up to a value allowed by the real
 *       system clock.
 * @note The maximum specified value is implementation dependent.
 */
#define chThdSleepSeconds(sec) chThdSleep(S2ST(sec))

/**
 * Delays the invoking thread for the specified number of milliseconds.
 * @note The specified time is rounded up to a value allowed by the real
 *       system clock.
 * @note The maximum specified value is implementation dependent.
 */
#define chThdSleepMilliseconds(msec) chThdSleep(MS2ST(msec))

/**
 * Delays the invoking thread for the specified number of microseconds.
 * @note The specified time is rounded up to a value allowed by the real
 *       system clock.
 * @note The maximum specified value is implementation dependent.
 */
#define chThdSleepMicroseconds(usec) chThdSleep(US2ST(usec))

#endif  /* _THREADS_H_ */

/** @} */

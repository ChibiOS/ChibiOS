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
 * Structure representing a thread.
 * @extends ThreadsQueue
 * @note Not all the listed fields are always needed, by switching off some
 *       not needed ChibiOS/RT subsystems it is possible to save RAM space by
 *       shrinking the \p Thread structure.
 */
struct Thread {
  /** Next \p Thread in the threads list.*/
  Thread                *p_next;
  /* End of the fields shared with the ThreadsList structure. */
  /** Previous \p Thread in the threads list.*/
  Thread                *p_prev;
  /* End of the fields shared with the ThreadsQueue structure. */
  /** The thread priority.*/
  tprio_t               p_prio;
  /* End of the fields shared with the ReadyList structure. */
  /** Thread identifier. */
  tid_t                 p_tid;
  /** Current thread state.*/
  tstate_t              p_state;
  /** Mode flags. */
  tmode_t               p_flags;
  /** Machine dependent processor context.*/
  Context               p_ctx;
  /*
   * The following fields are merged in unions because they are all
   * state-specific fields. This trick saves some extra space for each
   * thread in the system.
   */
  union {
    /** Thread wakeup code (only valid when exiting the \p PRREADY state).*/
    msg_t               p_rdymsg;
    /** The thread exit code (only while in \p PREXIT state).*/
    msg_t               p_exitcode;
#ifdef CH_USE_SEMAPHORES
    /** Semaphore where the thread is waiting on (only in \p PRWTSEM state).*/
    Semaphore           *p_wtsemp;
#endif
#ifdef CH_USE_MUTEXES
    /** Mutex where the thread is waiting on (only in \p PRWTMTX state).*/
    Mutex               *p_wtmtxp;
#endif
#ifdef CH_USE_CONDVARS
    /** CondVar where the thread is waiting on (only in \p PRWTCOND state).*/
    CondVar             *p_wtcondp;
#endif
#ifdef CH_USE_MESSAGES
    /** Destination thread for message send (only in \p PRSNDMSG state).*/
    Thread              *p_wtthdp;
#endif
#ifdef CH_USE_EVENTS
    /** Enabled events mask (only while in \p PRWTOREVT or \p PRWTANDEVT
        states). */
    eventmask_t         p_ewmask;
#endif
#ifdef CH_USE_TRACE
    /** Kernel object where the thread is waiting on. It is only valid when
        the thread is some sleeping states.*/
    void                *p_wtobjp;
#endif
  };
  /*
   * Start of the optional fields.
   */
#ifdef CH_USE_WAITEXIT
  /** The list of the threads waiting for this thread termination. */
  Thread                *p_waiting;
#endif
#ifdef CH_USE_MESSAGES
  ThreadsQueue          p_msgqueue;
  msg_t                 p_msg;
#endif
#ifdef CH_USE_EVENTS
  /** Pending events mask. */
  eventmask_t           p_epending;
#endif
#ifdef CH_USE_MUTEXES
  /** List of mutexes owned by this thread, \p NULL terminated. */
  Mutex                 *p_mtxlist;
  /** Thread's own, non-inherited, priority. */
  tprio_t               p_realprio;
#endif
#if defined(CH_USE_DYNAMIC) && defined(CH_USE_MEMPOOLS)
  /** Memory Pool where the thread workspace is returned. */
  void                  *p_mpool;
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
/** Thread state: Waiting in \p chThdSleep() or \p chThdSleepUntil(). */
#define PRWTCOND        5
/** Thread state: Waiting in \p chCondWait(). */
#define PRSLEEP         6
/** Thread state: Waiting in \p chThdWait(). */
#define PRWAIT          7
/** Thread state: Waiting in \p chEvtWaitOneTimeout() or
    \p chEvtWaitAnyTimeout(). */
#define PRWTOREVT       8
/** Thread state: Waiting in \p chEvtWaitAllTimeout(). */
#define PRWTANDEVT      9
/** Thread state: Waiting in \p chMsgSend(). */
#define PRSNDMSG        10
/** Thread state: Waiting in \p chMsgWait(). */
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
#define P_SUSPENDED             8       /* Create suspended (old).      */

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

/** Returns the pointer to the \p Thread currently in execution.*/
#define chThdSelf() currp

/** Returns the thread priority.*/
#define chThdGetPriority() (currp->p_prio)

/** Returns the pointer to the \p Thread local storage area, if any.*/
#define chThdLS() (void *)(currp + 1)

/** Verifies if the specified thread is in the \p PREXIT state.*/
#define chThdTerminated(tp) ((tp)->p_state == PREXIT)

/**
 * Verifies if the current thread has a termination request pending.
 */
#define chThdShouldTerminate() (currp->p_flags & P_TERMINATE)

/**
 * Resumes a thread created with the \p P_SUSPENDED option or suspended with
 * \p chThdSuspend().
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

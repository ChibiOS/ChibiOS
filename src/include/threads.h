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
 * @note Not all the listed fields are always needed, by switching off some
 *       not needed ChibiOS/RT subsystems it is possible to save RAM space by
 *       shrinking the \p Thread structure.
 */
struct Thread {
  /** Next \p Thread in the threads list.*/
  Thread            *p_next;
  /* End of the fields shared with the ThreadsList structure. */
  /** Previous \p Thread in the threads list.*/
  Thread            *p_prev;
  /* End of the fields shared with the ThreadsQueue structure. */
  /** The thread priority.*/
  tprio_t           p_prio;
  /* End of the fields shared with the ReadyList structure. */
  /** Thread identifier. */
  tid_t             p_tid;
  /** Current thread state.*/
  tstate_t          p_state;
  /** Mode flags. */
  tmode_t           p_flags;
  /** Machine dependent processor context.*/
  Context           p_ctx;
  /*
   * The following fields are merged in unions because they are all
   * state-specific fields. This trick saves some extra space for each
   * thread in the system.
   */
  union {
    /** Thread wakeup code (only valid when exiting the \p PRREADY state). */
    msg_t           p_rdymsg;
    /** The thread exit code (only while in \p PREXIT state).*/
    msg_t           p_exitcode;
#ifdef CH_USE_SEMAPHORES
    /** Semaphore where the thread is waiting on (only in \p PRWTSEM state). */
    Semaphore       *p_wtsemp;
#endif
#ifdef CH_USE_MUTEXES
    /** Mutex where the thread is waiting on (only in \p PRWTMTX state). */
    Mutex           *p_wtmtxp;
#endif
#ifdef CH_USE_MESSAGES
    /** Destination thread for message send (only in \p PRSNDMSG state). */
    Thread          *p_wtthdp;
#endif
#ifdef CH_USE_EVENTS
    /** Enabled events mask (only while in \p PRWTEVENT state). */
    eventmask_t     p_ewmask;
#endif
#ifdef CH_USE_TRACE
    /** Kernel object where the thread is waiting on. It is only valid when
        the thread is some sleeping states. */
    void            *p_wtobjp;
#endif
  };
  /*
   * Start of the optional fields.
   */
#ifdef CH_USE_WAITEXIT
  /** The list of the threads waiting for this thread termination. */
  ThreadsList       p_waiting;
#endif
#ifdef CH_USE_EXIT_EVENT
  /** The thread termination \p EventSource. */
  EventSource       p_exitesource;
#endif
#ifdef CH_USE_MESSAGES
  ThreadsQueue      p_msgqueue;
  msg_t             p_msg;
#endif
#ifdef CH_USE_EVENTS
  /** Pending events mask. */
  eventmask_t       p_epending;
#endif
#ifdef CH_USE_MUTEXES
  /** List of mutexes owned by this thread, \p NULL terminated. */
  Mutex             *p_mtxlist;
  /** Thread's own, non-inherited, priority. */
  tprio_t           p_realprio;
#endif
#if defined(CH_USE_DYNAMIC) && defined(CH_USE_MEMPOOLS)
  /** Memory Pool where the thread workspace is returned. */
  void              *p_mpool;
#endif
#ifdef CH_USE_THREAD_EXT
  THREAD_EXT_FIELDS
#endif
};

/** Thread state: Ready to run, waiting on the ready list.*/
#define PRREADY     0
/** Thread state: Currently running. */
#define PRCURR      1
/** Thread state: Thread created in suspended state. */
#define PRSUSPENDED 2
/** Thread state: Waiting on a semaphore. */
#define PRWTSEM     3
/** Thread state: Waiting on a mutex. */
#define PRWTMTX     4
/** Thread state: Waiting in \p chThdSleep() or \p chThdSleepUntil(). */
#define PRSLEEP     5
/** Thread state: Waiting in \p chThdWait(). */
#define PRWAIT      6
/** Thread state: Waiting in \p chEvtWait(). */
#define PRWTEVENT   7
/** Thread state: Waiting in \p chMsgSend(). */
#define PRSNDMSG    8
/** Thread state: Waiting in \p chMsgWait(). */
#define PRWTMSG     9
/** Thread state: After termination.*/
#define PREXIT      10

/*
 * Various flags into the thread p_flags field.
 */
#define P_MEM_MODE_MASK         3       /* Thread memory mode mask.     */
#define P_MEM_MODE_STATIC       0       /* Thread memory mode: static.  */
#define P_MEM_MODE_HEAP         1       /* Thread memory mode: heap.    */
#define P_MEM_MODE_MEMPOOL      2       /* Thread memory mode: mempool. */
#define P_TERMINATE             4       /* Termination requested.       */
#define P_SUSPENDED             8       /* Create suspended (old).      */

/** Pseudo priority used by the ready list header, do not use.*/
#define NOPRIO      0
/** Idle thread priority.*/
#define IDLEPRIO    1
/** Lowest user priority.*/
#define LOWPRIO     2
/** Normal user priority.*/
#define NORMALPRIO  64
/** Highest user priority.*/
#define HIGHPRIO    127
/** Greatest possible priority.*/
#define ABSPRIO     255

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
  Thread *chThdCreate(tprio_t prio, tmode_t mode, void *workspace,
                      size_t wsize, tfunc_t pf, void *arg);
  void chThdSetPriority(tprio_t newprio);
  void chThdExit(msg_t msg);
  Thread *chThdResume(Thread *tp);
  void chThdSuspend(Thread **tpp);
  void chThdTerminate(Thread *tp);
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
 * Returns the exit event source for the specified thread. The source is
 * signaled when the thread terminates.
 * @param tp the pointer to the thread
 * @note When registering on a thread termination make sure the thread
 *       is still alive, if you do that after the thread termination
 *       then you would miss the event. There are two ways to ensure
 *       this:<br>
 *       <ul>
 *       <li>Create the thread suspended, register on the event source
 *           and then resume the thread (recommended).</li>
 *       <li>Create the thread with a lower priority then register on it.
 *           This does not work if the hardware is capable of multiple
 *           physical threads.</li>
 *       </ul>
 * @note You dont need to unregister from a terminated thread because
 *       the event source becomes inactive.
 * @note The function is available only if the \p CH_USE_EXIT_EVENT
 *       option is enabled in \p chconf.h.
 * @deprecated \p THREAD_EXT_EXIT should be used, this functionality will be
 *             removed in version 1.0.0.
 */
#define chThdGetExitEventSource(tp) (&(tp)->p_exitesource)

/**
 * Resumes a thread created with the \p P_SUSPENDED option or suspended with
 * \p chThdSuspend().
 * @param tp the pointer to the thread
 */
#define chThdResumeI(tp) chSchReadyI(tp)

/**
 * Creates a new thread, simplified variant.
 * @param prio the priority level for the new thread. Usually the threads are
 *             created with priority \p NORMALPRIO, priorities
 *             can range from \p LOWPRIO to \p HIGHPRIO.
 * @param workspace pointer to a working area dedicated to the thread stack
 * @param wsize size of the working area.
 * @param pf the thread function
 * @return the pointer to the \p Thread structure allocated for the
 *         thread into the working space area.
 * @note A thread can terminate by calling \p chThdExit() or by simply
 *       returning from its main function.
 * @deprecated Please use \p chThdCreateStatic() or \p chThdInit() instead,
 *             this function will be removed in version 1.0.0.
 */
#define chThdCreateFast(prio, workspace, wsize, pf) \
        chThdCreateStatic(workspace, wsize, prio, pf, NULL)

#endif  /* _THREADS_H_ */

/** @} */

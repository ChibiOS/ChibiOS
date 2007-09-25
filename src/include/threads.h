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

#define isempty(qp)     ((qp)->p_next == (Thread *)(qp))
#define notempty(qp)    ((qp)->p_next != (Thread *)(qp))
#define firstprio(qp)   ((qp)->p_next->p_prio)
#define lastprio(qp)    ((qp)->p_prev->p_prio)

/**
 * Generic threads queue header and element.
 */
typedef struct {
  /** First \p Thread in the queue.*/
  Thread            *p_next;
  /** Last \p Thread in the queue.*/
  Thread            *p_prev;
} ThreadsQueue;

/**
 * Structure representing a thread.
 * @note Not all the listed fields are always needed, by switching off some
 *       not needed ChibiOS/RT subsystems it is possible to save RAM space by
 *       shrinking the \p Thread structure.
 */
struct Thread {
  /** Next \p Thread in the threads list.*/
  Thread            *p_next;
  /** Previous \p Thread in the threads list.*/
  Thread            *p_prev;
  /* End of the fields shared with the ThreadsQueue structure. */
  /** The thread priority.*/
  t_prio            p_prio;
  /* End of the fields shared with the ReadyList structure. */
  /** Current thread state.*/
  t_tstate          p_state;
  /** Mode flags.*/
  t_tmode           p_flags;
  /*
   * The following fields are merged in an union because they are all
   * state-specific fields. This trick saves some extra space for each
   * thread in the system.
   */
  union {
    /** Thread wakeup code, normally set to \p RDY_OK by the \p chSchReadyI()
     * (only while in \p PRCURR or \p PRREADY states).*/
    t_msg           p_rdymsg;
    /** The thread exit code (only while in \p PREXIT state).*/
    t_msg           p_exitcode;
#ifdef CH_USE_EVENTS
    /** Enabled events mask (only while in \p PRWTEVENT state).*/
    t_eventmask     p_ewmask;
#endif
#ifdef CH_USE_MESSAGES
    /** Message (only while in \p PRSNDMSG state).*/
    t_msg           p_msg;
#endif
  };
  /** Machine dependent processor context.*/
  Context                       p_ctx;
  /*
   * Start of the optional fields. Note, the null thread may also let its
   * stack overwrite the following fields since it never uses semaphores,
   * events, messages, exit etc, this can save some space on RAM starved
   * systems, be caruful in doing so.
   */
#ifdef CH_USE_WAITEXIT
  /** The queue of the threads waiting for this thread termination.*/
  ThreadsQueue      p_waiting;
#endif
#ifdef CH_USE_EXIT_EVENT
  /** The thread termination \p EventSource.*/
  EventSource       p_exitesource;
#endif
#ifdef CH_USE_MESSAGES
  ThreadsQueue      p_msgqueue;
#endif
#ifdef CH_USE_EVENTS
  /** Pending events mask.*/
  t_eventmask       p_epending;
#endif
#ifdef CH_USE_RT_SEMAPHORES
  /** Priority backup after acquiring a RT semaphore.*/
  t_prio            p_bakprio;
  /** RT semaphores depth counter.*/
  WORD16            p_rtcnt;
#endif
};

/** Thread state: Reserved.*/
#define PRFREE      0
/** Thread state: Current.*/
#define PRCURR      1
/** Thread state: Thread in the ready list.*/
#define PRREADY     2
/** Thread state: Thread created in suspended state.*/
#define PRSUSPENDED 3
/** Thread state: Waiting on a semaphore.*/
#define PRWTSEM     4
/** Thread state: Waiting in \p chThdSleep() or \p chThdSleepUntil().*/
#define PRSLEEP     5
/** Thread state: Waiting in \p chThdWait().*/
#define PRWAIT      6
/** Thread state: Waiting in \p chEvtWait().*/
#define PRWTEVENT   7
/** Thread state: Waiting in \p chMsgSend().*/
#define PRSNDMSG    8
/** Thread state: Waiting in \p chMsgWait().*/
#define PRWTMSG     9
/** Thread state: After termination.*/
#define PREXIT      10

/** Thread option: Termination requested flag.*/
#define P_TERMINATE 1
/** Thread option: Create suspended thread.*/
#define P_SUSPENDED 2

/** Idle thread priority.*/
#define IDLEPRIO    0
/** Lowest user priority.*/
#define LOWPRIO     1
/** Normal user priority.*/
#define NORMALPRIO  128
/** Highest user priority.*/
#define HIGHPRIO    255
/** Boosted base priority.*/
#define MEPRIO      256
/** Absolute priority.*/
#define ABSPRIO     512

/* Not an API, don't use into the application code.*/
void _InitThread(t_prio prio, t_tmode mode, Thread *tp);

/** Thread function.*/
typedef t_msg (*t_tfunc)(void *);

/*
 * Threads Lists functions.
 */
#ifndef CH_OPTIMIZE_SPEED
void enqueue(Thread *tp, ThreadsQueue *tqp);
Thread *dequeue(Thread *tp);
#else
static INLINE Thread *dequeue(Thread *tp) {

  tp->p_prev->p_next = tp->p_next;
  tp->p_next->p_prev = tp->p_prev;
  return tp;
}

static INLINE void enqueue(Thread *tp, ThreadsQueue *tqp) {

  tp->p_next = (Thread *)tqp;
  tp->p_prev = tqp->p_prev;
  tqp->p_prev->p_next = tp;
  tqp->p_prev = tp;
}
#endif

/*
 * Threads APIs.
 */
#define chThdSelf() currp
Thread *chThdCreate(t_prio prio, t_tmode mode, void *workspace,
                    t_size wsize, t_tfunc pf, void *arg);
void chThdResume(Thread *tp);
void chThdTerminate(Thread *tp);
BOOL chThdShouldTerminate(void);
BOOL chThdTerminated(Thread *tp);
void chThdExit(t_msg msg);
#ifdef CH_USE_WAITEXIT
t_msg chThdWait(Thread *tp);
#endif
#ifdef CH_USE_EXIT_EVENT
EventSource *chThdGetExitEventSource(Thread *tp);
#endif

#endif  /* _THREADS_H_ */

/** @} */

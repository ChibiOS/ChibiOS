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

#include <ch.h>

#ifndef CH_OPTIMIZE_SPEED
/*
 * Removes a Thread from a list and returns it.
 * @param tp the pointer to the thread to be removed from the list
 * @return the removed thread pointer
 */
Thread *dequeue(Thread *tp) {

  tp->p_prev->p_next = tp->p_next;
  tp->p_next->p_prev = tp->p_prev;
  return tp;
}

/*
 * Inserts a thread into a bi-directional list in FIFO order.
 * @param tp the pointer to the thread to be inserted in the list
 * @param tqp the pointer to the threads list header
 */
void enqueue(Thread *tp, ThreadsQueue *tqp) {

  tp->p_next = (Thread *)tqp;
  tp->p_prev = tqp->p_prev;
  tqp->p_prev->p_next = tp;
  tqp->p_prev = tp;
}
#endif /* CH_OPTIMIZE_SPEED */

/*
 * Initializes a thread structure.
 */
void _InitThread(t_prio prio, t_tmode mode, Thread *tp) {

  tp->p_flags = mode;
  tp->p_prio = prio;
  tp->p_rdymsg = RDY_OK;
#ifdef CH_USE_RT_SEMAPHORES
  tp->p_rtcnt = 0;
#endif
#ifdef CH_USE_WAITEXIT
  tp->p_waiting.p_next = (Thread *)&tp->p_waiting;
  tp->p_waiting.p_prev = (Thread *)&tp->p_waiting;
#endif
#ifdef CH_USE_MESSAGES
  tp->p_msgqueue.p_next = (Thread *)&tp->p_msgqueue;
  tp->p_msgqueue.p_prev = (Thread *)&tp->p_msgqueue;
#endif
#ifdef CH_USE_EVENTS
  tp->p_epending = 0;
#endif
#ifdef CH_USE_EXIT_EVENT
    chEvtInit(&tp->p_exitesource);
#endif
}

/**
 * Creates a new thread.
 * @param prio the priority level for the new thread. Usually the threads are
 *             created with priority \p NORMALPRIO (128), priorities
 *             can range from \p LOWPRIO (1) to \p HIGHPRIO
 *             (255).
 * @param mode the creation option flags for the thread. The following options
 *             can be OR'ed in this parameter:<br>
 *             <ul>
 *             <li>\p P_SUSPENDED, the thread is created in the
 *                 \p PRSUSPENDED state and a subsequent call to
 *                 \p chThdResume() will make it ready for
 *                 execution.</li>
 *             <li>\p P_TERMINATED, this flag is usually set
 *                 by the \p chThdTerminate() function and it is not
 *                 normally used as parameter for this function. The
 *                 result would be to create a thread with a termination
 *                 request already pending.</li>
 *             </ul>
 * @param workspace pointer to a working area dedicated to the thread stack
 * @param wsize size of the working area.
 * @param pf the thread function. Returning from this function automatically
 *           terminates the thread.
 * @param arg an argument passed to the thread function. It can be \p NULL.
 * @return the pointer to the \p Thread structure allocated for the
 *         thread into the working space area.
 * @note A thread can terminate by calling \p chThdExit() or by simply
 *       returning from its main function.
 */
Thread *chThdCreate(t_prio prio, t_tmode mode, void *workspace,
                        t_size wsize, t_tfunc pf, void *arg) {
  Thread *tp = workspace;

  _InitThread(prio, mode, tp);
  SETUP_CONTEXT(workspace, wsize, pf, arg);
#ifdef CH_USE_RESUME
  if (tp->p_flags & P_SUSPENDED)
    tp->p_state = PRSUSPENDED;
  else {
#endif
    chSysLock();

    chSchWakeupI(tp, RDY_OK);

    chSysUnlock();
#ifdef CH_USE_RESUME
  }
#endif
  return tp;
}

/**
 * Verifies if the specified thread is in the \p PREXIT state.
 * @param tp the pointer to the thread
 * @return \p TRUE if the thread is ended else \p FALSE. \p TRUE ensures that
 *         a subsequent call to \p chThdWait() would not block.
 */
BOOL chThdTerminated(Thread *tp) {

  return tp->p_state == PREXIT;
}

#ifdef CH_USE_RESUME
/**
 * Resumes a thread created with the \p P_SUSPENDED option.
 * @param tp the pointer to the thread
 * @note The function has no effect on threads in any other state than
 *       \p PRSUSPENDED.
 * @note The function is available only if the \p CH_USE_RESUME
 *       option is enabled in \p chconf.h.
 */
void chThdResume(Thread *tp) {

  chSysLock();

  if (tp->p_state == PRSUSPENDED)
    chSchWakeupI(tp, RDY_OK);

  chSysUnlock();
}
#endif

#ifdef CH_USE_TERMINATE
/**
 * Requests a thread termination.
 * @param tp the pointer to the thread
 * @note The thread is not termitated but a termination request is added to
 *       its \p p_flags field. The thread can read this status by
 *       invoking \p chThdShouldTerminate() and then terminate cleanly.
 */
void chThdTerminate(Thread *tp) {

  chSysLock();

  tp->p_flags |= P_TERMINATE;

  chSysUnlock();
}

/**
 * Verifies if the current thread has a termination request pending.
 * @return \p TRUE if the termination was requested. The thread should terminate
 *         as soon it is ready to do so.
 */
BOOL chThdShouldTerminate(void) {

  return currp->p_flags & P_TERMINATE ? TRUE : FALSE;
}

#endif

/**
 * Terminates the current thread by specifying an exit status code.
 * @param msg the thread exit code. The code can be retrieved by using
 *            \p chThdWait().
 */
void chThdExit(t_msg msg) {

  chSysLock();

  currp->p_exitcode = msg;              /* Post mortem info.            */
#ifdef CH_USE_WAITEXIT
  while (notempty(&currp->p_waiting))
    chSchReadyI(dequeue(currp->p_waiting.p_next));
#endif
#ifdef CH_USE_EXIT_EVENT
  chEvtSendI(&currp->p_exitesource);
#endif
  chSchGoSleepI(PREXIT);

  chSysUnlock();                        /* Never executed.              */
}

#ifdef CH_USE_WAITEXIT
/**
 * Blocks the execution of the invoking thread until the specified thread
 * terminates then the exit code is returned.
 * @param tp the pointer to the thread
 * @return the exit code
 * @note The function is available only if the \p CH_USE_WAITEXIT
 *       option is enabled in \p chconf.h.
 */
t_msg chThdWait(Thread *tp) {

  chSysLock();

  if (tp->p_state != PREXIT) {
    enqueue(currp, &tp->p_waiting);
    chSchGoSleepI(PRWAIT);
  }

  chSysUnlock();
  return tp->p_exitcode;
}
#endif /* CH_USE_WAITEXIT */

#ifdef CH_USE_EXIT_EVENT
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
 */
EventSource *chThdGetExitEventSource(Thread *tp) {

  return &tp->p_exitesource;
}
#endif /* CH_USE_EXIT_EVENT */

/** @} */

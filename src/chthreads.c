/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @addtogroup Threads
 * @{
 */

#include <ch.h>

/*
 * Initializes a thread structure.
 */
Thread *init_thread(Thread *tp, tprio_t prio) {
  static tid_t nextid = 0;

  tp->p_tid = nextid++;
  tp->p_flags = P_MEM_MODE_STATIC;
  tp->p_prio = prio;
  tp->p_state = PRSUSPENDED;
#ifdef CH_USE_MUTEXES
  /* realprio is the thread's own, non-inherited, priority */
  tp->p_realprio = prio;
  tp->p_mtxlist = NULL;
#endif
#ifdef CH_USE_WAITEXIT
  tp->p_waiting = NULL;
#endif
#ifdef CH_USE_MESSAGES
  queue_init(&tp->p_msgqueue);
#endif
#ifdef CH_USE_EVENTS
  tp->p_epending = 0;
#endif
  THREAD_EXT_INIT(tp);
  return tp;
}

#ifdef CH_USE_DEBUG
static void memfill(uint8_t *p, uint32_t n, uint8_t v) {

  while (n)
    *p++ = v, n--;
}
#endif

/**
 * Initializes a new thread.
 * The new thread is initialized but not inserted in the ready list, the
 * initial state is \p PRSUSPENDED.
 * @param prio the priority level for the new thread. Usually the threads are
 *             created with priority \p NORMALPRIO, priorities
 *             can range from \p LOWPRIO to \p HIGHPRIO.
 * @param workspace pointer to a working area dedicated to the thread stack
 * @param wsize size of the working area.
 * @param pf the thread function
 * @param arg an argument passed to the thread function. It can be \p NULL.
 * @return The pointer to the \p Thread structure allocated for the
 *         thread into the working space area.
 * @note A thread can terminate by calling \p chThdExit() or by simply
 *       returning from its main function.
 * @note This function can be invoked from within an interrupt handler.
 */
Thread *chThdInit(void *workspace, size_t wsize,
                  tprio_t prio, tfunc_t pf, void *arg) {
  /* thread structure is layed out in the lower part of the thread workspace */
  Thread *tp = workspace;

  chDbgAssert((wsize >= THD_WA_SIZE(0)) && (prio <= HIGHPRIO) &&
              (workspace != NULL) && (pf != NULL),
              "chthreads.c, chThdInit()");
#ifdef CH_USE_DEBUG
  memfill(workspace, wsize, MEM_FILL_PATTERN);
#endif
  SETUP_CONTEXT(workspace, wsize, pf, arg);
  return init_thread(tp, prio);
}

/**
 * Creates a new thread into a static memory area.
 * @param workspace pointer to a working area dedicated to the thread stack
 * @param wsize size of the working area.
 * @param prio the priority level for the new thread. Usually the threads are
 *             created with priority \p NORMALPRIO, priorities
 *             can range from \p LOWPRIO to \p HIGHPRIO.
 * @param pf the thread function
 * @param arg an argument passed to the thread function. It can be \p NULL.
 * @return The pointer to the \p Thread structure allocated for the
 *         thread into the working space area.
 * @note A thread can terminate by calling \p chThdExit() or by simply
 *       returning from its main function.
 */
Thread *chThdCreateStatic(void *workspace, size_t wsize,
                          tprio_t prio, tfunc_t pf, void *arg) {

  return chThdResume(chThdInit(workspace, wsize, prio, pf, arg));
}

#if defined(CH_USE_DYNAMIC) && defined(CH_USE_WAITEXIT) && defined(CH_USE_HEAP)
/**
 * Creates a new thread allocating the memory from the heap.
 * @param wsize size of the working area to be allocated
 * @param prio the priority level for the new thread. Usually the threads are
 *             created with priority \p NORMALPRIO, priorities
 *             can range from \p LOWPRIO to \p HIGHPRIO.
 * @param pf the thread function
 * @param arg an argument passed to the thread function. It can be \p NULL.
 * @return The pointer to the \p Thread structure allocated for the
 *         thread into the working space area.
 * @retval NULL if the memory cannot be allocated.
 * @note A thread can terminate by calling \p chThdExit() or by simply
 *       returning from its main function.
 * @note The memory allocated for the thread is not released when the thread
 *       terminates but when a \p chThdWait() is performed.
 * @note The function is available only if the \p CH_USE_DYNAMIC,
 *       \p CH_USE_HEAP and \p CH_USE_WAITEXIT options are enabled
 *       in \p chconf.h.
 */
Thread *chThdCreateFromHeap(size_t wsize, tprio_t prio,
                            tfunc_t pf, void *arg) {

  void *workspace = chHeapAlloc(wsize);
  if (workspace == NULL)
    return NULL;
  Thread *tp = chThdInit(workspace, wsize, prio, pf, arg);
  tp->p_flags = P_MEM_MODE_HEAP;
  return chThdResume(tp);
}
#endif /* defined(CH_USE_DYNAMIC) && defined(CH_USE_WAITEXIT) && defined(CH_USE_HEAP) */

#if defined(CH_USE_DYNAMIC) && defined(CH_USE_WAITEXIT) && defined(CH_USE_MEMPOOLS)
/**
 * Creates a new thread allocating the memory from the specified Memory Pool.
 * @param mp the memory pool
 * @param prio the priority level for the new thread. Usually the threads are
 *             created with priority \p NORMALPRIO, priorities
 *             can range from \p LOWPRIO to \p HIGHPRIO.
 * @param pf the thread function
 * @param arg an argument passed to the thread function. It can be \p NULL.
 * @return The pointer to the \p Thread structure allocated for the
 *         thread into the working space area or \p NULL if the memory cannot
 *         be allocated.
 * @retval NULL if the memory pool is empty.
 * @note A thread can terminate by calling \p chThdExit() or by simply
 *       returning from its main function.
 * @note The memory allocated for the thread is not released when the thread
 *       terminates but when a \p chThdWait() is performed.
 * @note The function is available only if the \p CH_USE_DYNAMIC,
 *       \p CH_USE_MEMPOOLS and \p CH_USE_WAITEXIT options are enabled
 *       in \p chconf.h.
 */
Thread *chThdCreateFromMemoryPool(MemoryPool *mp, tprio_t prio,
                                  tfunc_t pf, void *arg) {

  void *workspace = chPoolAlloc(mp);
  if (workspace == NULL)
    return NULL;
  Thread *tp = chThdInit(workspace, mp->mp_object_size, prio, pf, arg);
  tp->p_flags = P_MEM_MODE_MEMPOOL;
  tp->p_mpool = mp;
  return chThdResume(tp);
}
#endif /* defined(CH_USE_DYNAMIC) && defined(CH_USE_WAITEXIT) && defined(CH_USE_MEMPOOLS) */

/**
 * Changes the running thread priority, reschedules if necessary.
 *
 * @param newprio the new priority of the running thread
 */
void chThdSetPriority(tprio_t newprio) {

  chDbgAssert(newprio <= HIGHPRIO, "chthreads.c, chThdSetPriority()");
  chSysLock();

#ifdef CH_USE_MUTEXES
  if (currp->p_prio != currp->p_realprio) {
    if (newprio > currp->p_prio)
      currp->p_prio = newprio;
  }
  else
    currp->p_prio = newprio;
  currp->p_realprio = newprio;
#else
  currp->p_prio = newprio;
#endif
  chSchRescheduleS();

  chSysUnlock();
}

/**
 * Resumes a suspended thread.
 * @param tp the pointer to the thread
 * @return The pointer to the thread.
 */
Thread *chThdResume(Thread *tp) {

  chSysLock();
  chDbgAssert(tp->p_state == PRSUSPENDED, "chthreads.c, chThdResume()");
  chSchWakeupS(tp, RDY_OK);
  chSysUnlock();
  return tp;
}

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
 * Suspends the invoking thread for the specified time.
 * @param time the delay in system ticks
 */
void chThdSleep(systime_t time) {

  chSysLock();
  chThdSleepS(time);
  chSysUnlock();
}

/**
 * Suspends the invoking thread until the system time arrives to the specified
 * value.
 * @param time the absolute system time
 */
void chThdSleepUntil(systime_t time) {

  chSysLock();
  if ((time -= chSysGetTime()) > 0)
    chThdSleepS(time);
  chSysUnlock();
}

/**
 * Terminates the current thread by specifying an exit status code.
 *
 * @param msg the thread exit code. The code can be retrieved by using
 *            \p chThdWait().
 */
void chThdExit(msg_t msg) {
  Thread *tp = currp;

  chSysLock();
  tp->p_exitcode = msg;
  THREAD_EXT_EXIT(tp);
#ifdef CH_USE_WAITEXIT
  if (tp->p_waiting != NULL)
    chSchReadyI(tp->p_waiting);
#endif
  chSchGoSleepS(PREXIT);
}

#ifdef CH_USE_WAITEXIT
/**
 * Blocks the execution of the invoking thread until the specified thread
 * terminates then the exit code is returned.
 * The memory used by the exited thread is handled in different ways depending
 * on the API that spawned the thread:
 * <ul>
 * <li>If the thread was spawned by \p chThdCreateStatic() or by \p chThdInit()
 *     then nothing happens and the thread working area is not released or
 *     modified in any way. This is the default, totally static, behavior.</li>
 * <li>If the thread was spawned by \p chThdCreateFromHeap() then the working
 *     area is returned to the system heap.</li>
 * <li>If the thread was spawned by \p chThdCreateFromMemoryPool() then the
 *     working area is returned to the owning memory pool.</li>
 * </ul>
 * @param tp the thread pointer
 * @return The exit code from the terminated thread
 * @note After invoking \p chThdWait() the thread pointer becomes invalid and
 *       must not be used as parameter for further system calls.
 * @note The function is available only if the \p CH_USE_WAITEXIT
 *       option is enabled in \p chconf.h.
 * @note Only one thread can be waiting for another thread at any time. You
 *       should imagine the threads as having a reference counter that is set
 *       to one when the thread is created, chThdWait() decreases the reference
 *       and the memory is freed when the counter reaches zero. In the current
 *       implementation there is no real reference counter in the thread
 *       structure but it is a planned extension.
 */
msg_t chThdWait(Thread *tp) {
  msg_t msg;

  chSysLock();
  chDbgAssert((tp != NULL) && (tp != currp) && (tp->p_waiting == NULL),
              "chthreads.c, chThdWait()");
  if (tp->p_state != PREXIT) {
    tp->p_waiting = currp;
    chSchGoSleepS(PRWAIT);
  }
  msg = tp->p_exitcode;
#ifndef CH_USE_DYNAMIC
  chSysUnlock();
  return msg;
#else /* CH_USE_DYNAMIC */

  /* Returning memory.*/
  tmode_t mode = tp->p_flags & P_MEM_MODE_MASK;
  chSysUnlock();

  switch (mode) {
#ifdef CH_USE_HEAP
  case P_MEM_MODE_HEAP:
    chHeapFree(tp);
    break;
#endif
#ifdef CH_USE_MEMPOOLS
  case P_MEM_MODE_MEMPOOL:
    chPoolFree(tp->p_mpool, tp);
    break;
#endif
  }
  return msg;
#endif /* CH_USE_DYNAMIC */
}
#endif /* CH_USE_WAITEXIT */

/** @} */

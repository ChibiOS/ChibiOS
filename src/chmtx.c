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
 * @addtogroup Mutexes
 * @{
 */

#include <ch.h>

#ifdef CH_USE_MUTEXES

/**
 * Initializes s \p Mutex structure.
 * @param mp pointer to a \p Mutex structure
 */
void chMtxInit(Mutex *mp) {

  fifo_init(&mp->m_queue);
  mp->m_owner = NULL;
}

/*
 * Inserts a thread into a list ordering it by priority.
 * @param tp the pointer to the thread to be inserted in the list
 * @param tqp the pointer to the threads list header
 */
#ifdef CH_OPTIMIZE_SPEED
static INLINE void prio_enq(Thread *tp, ThreadsQueue *tqp) {
#else
static void prio_enq(Thread *tp, ThreadsQueue *tqp) {
#endif

  Thread *cp = tqp->p_next;
  while ((cp != (Thread *)tqp) && (cp->p_prio >= tp->p_prio))
    cp = cp->p_next;
  /* Insertion on p_prev.*/
  tp->p_prev = (tp->p_next = cp)->p_prev;
  tp->p_prev->p_next = cp->p_prev = tp;
}

/*
 * 0 +++BA++------------------2+++--8++++++++++BR0----------------------------
 * 1 .......++AA++--2+++++++++BA....8..........++++++++BR8++++AR1-------------
 * 2 .............++AA..............................................----++AR++
 * 8 .............................++AA........................++++++AR++......
 *
 *
 * 5 ++++++++++++++++++AA+....9++++++++++++++AR5------------------------------
 * 7 .....................++--------------------------------------------------
 * 9 .......................++AA.............+++++++++AR++++++++++++++++++++++
 */

/**
 * Locks the specified mutex.
 * @param mp pointer to the \p Mutex structure
 */
void chMtxLock(Mutex *mp) {

  chSysLock();

  chMtxLockS(mp);

  chSysUnlock();
}

/**
 * Locks the specified mutex.
 * @param mp pointer to the \p Mutex structure
 * @note This function must be called within a \p chSysLock() / \p chSysUnlock()
 *       block.
 */
void chMtxLockS(Mutex *mp) {

  if (mp->m_owner != NULL) {
    /*
     * Inheritance, explores the thread-mutex dependances adjusting
     * the priority of all the affected threads.
     */
    Thread *tp = mp->m_owner;
    while (tp->p_prio < currp->p_prio) {
      tp->p_prio = currp->p_prio;
      switch (tp->p_state) {
      case PRWTMTX:
        prio_enq(dequeue(tp), &tp->p_mtxp->m_queue);
        tp = tp->p_mtxp->m_owner;
        continue;
      case PRREADY:
        chSchReadyI(dequeue(tp), RDY_OK);
      }
      break;
    }
    /*
     * Goes to sleep on the mutex.
     */
    prio_enq(currp, &mp->m_queue);
    currp->p_mtxp = mp;
    chSchGoSleepS(PRWTMTX);
    chDbgAssert(mp->m_owner == NULL, "chmtx.c, chMtxLockS()");
  }
  /*
   * The mutex is now inserted in the owned mutexes list.
   */
  mp->m_owner = currp;
  mp->m_next = currp->p_mtxlist;
  currp->p_mtxlist = mp;
}

/**
 * Tries to lock a mutex. This function does not have any overhead related to
 * the priority inheritance mechanism because it does not try to enter a sleep
 * state on the mutex.
 * @param mp pointer to the \p Mutex structure
 * @return \p TRUE if the mutex was successfully acquired else \p FALSE
 */
BOOL chMtxTryLock(Mutex *mp) {

  chSysLock();

  BOOL b = chMtxTryLockS(mp);

  chSysUnlock();
  return b;
}

/**
 * Tries to lock a mutex. This function does not have any overhead related to
 * the priority inheritance mechanism because it does not try to enter a sleep
 * state on the mutex.
 * @param mp pointer to the \p Mutex structure
 * @return \p TRUE if the mutex was successfully acquired else \p FALSE
 * @note This function must be called within a \p chSysLock() / \p chSysUnlock()
 *       block.
 */
BOOL chMtxTryLockS(Mutex *mp) {

  if (mp->m_owner != NULL)
    return FALSE;
  mp->m_owner = currp;
  mp->m_next = currp->p_mtxlist;
  currp->p_mtxlist = mp;
  return TRUE;
}

/**
 * Unlocks the next owned mutex in reverse lock order.
 */
void chMtxUnlock(void) {

  chSysLock();

  chMtxUnlockS();

  chSysUnlock();
}

/**
 * Unlocks the next owned mutex in reverse lock order.
 * @note This function must be called within a \p chSysLock() / \p chSysUnlock()
 *       block.
 */
void chMtxUnlockS(void) {

  chDbgAssert((currp->p_mtxlist != NULL) && (currp->p_mtxlist->m_owner == currp),
              "chmtx.c, chMtxUnlockS()");

  /*
   * Removes the top Mutex from the owned mutexes list and marks it as not owned.
   */
  Mutex *mp = currp->p_mtxlist;
  currp->p_mtxlist = mp->m_next;
  mp->m_owner = NULL;
  /*
   * If a thread is waiting on the mutex then the hard part begins.
   */
  if (chMtxQueueNotEmptyS(mp)) {
    Thread *tp = fifo_remove(&mp->m_queue);
    /*
     * Recalculates the optimal thread priority by scanning the owned mutexes list.
     */
    t_prio newprio = currp->p_realprio;
    mp = currp->p_mtxlist;
    while (mp != NULL) {
      if (chMtxQueueNotEmptyS(mp) && (mp->m_queue.p_next->p_prio > newprio))
        newprio = mp->m_queue.p_next->p_prio;
      mp = mp->m_next;
    }
    currp->p_prio = newprio;
    chSchWakeupS(tp, RDY_OK);
  }
}

/**
 * Unlocks all the mutexes owned by the invoking thread, this is <b>MUCH MORE</b>
 * efficient than releasing the mutexes one by one and not just because the
 * call overhead, this function does not have any overhead related to the
 * priority inheritance mechanism.
 */
void chMtxUnlockAll(void) {

  chSysLock();

  chMtxUnlockAllS();

  chSysUnlock();
}

/**
 * Unlocks all the mutexes owned by the invoking thread, this is <b>MUCH MORE</b>
 * efficient than releasing the mutexes one by one and not just because the
 * call overhead, this function does not have any overhead related to the
 * priority inheritance mechanism.
 * @note This function must be called within a \p chSysLock() / \p chSysUnlock()
 *       block.
 */
void chMtxUnlockAllS(void) {

  if (currp->p_mtxlist != NULL) {
    do {
      Mutex *mp = currp->p_mtxlist;
      currp->p_mtxlist = mp->m_next;
      mp->m_owner = NULL;
      if (chMtxQueueNotEmptyS(mp))
        chSchReadyI(fifo_remove(&mp->m_queue), RDY_OK);
    } while (currp->p_mtxlist != NULL);
    currp->p_prio = currp->p_realprio;
    chSchRescheduleS();
  }
}

#endif /* CH_USE_MUTEXES */

/** @} */

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
 * Inserts a thread into a priority ordered queue.
 * @param tp the pointer to the thread to be inserted in the list
 * @param tqp the pointer to the threads list header
 * @note the insertion is done by scanning the list from the highest priority
 *       toward the lowest
 */
void prio_insert(Thread *tp, ThreadsQueue *tqp) {

  Thread *cp = tqp->p_next;
  while ((cp != (Thread *)tqp) && (cp->p_prio >= tp->p_prio))
    cp = cp->p_next;
  /* Insertion on p_prev.*/
  tp->p_prev = (tp->p_next = cp)->p_prev;
  tp->p_prev->p_next = cp->p_prev = tp;
}

/*
 * Inserts a thread into a FIFO queue.
 * @param tp the pointer to the thread to be inserted in the list
 * @param tqp the pointer to the threads list header
 */
void fifo_insert(Thread *tp, ThreadsQueue *tqp) {

  tp->p_prev = (tp->p_next = (Thread *)tqp)->p_prev;
  tp->p_prev->p_next = tqp->p_prev = tp;
}

/*
 * Removes a thread from a FIFO queue.
 * @param tqp the pointer to the threads list header
 */
Thread *fifo_remove(ThreadsQueue *tqp) {
  Thread *tp = tqp->p_next;

  (tqp->p_next = tp->p_next)->p_prev = (Thread *)tqp;
  return tp;
}

/*
 * Removes a Thread from a FIFO list and returns it.
 * @param tp the pointer to the thread to be removed from the list
 * @param tqp the pointer to the list header
 * @return the removed thread pointer
 */
Thread *dequeue(Thread *tp) {

  tp->p_prev->p_next = tp->p_next;
  tp->p_next->p_prev = tp->p_prev;
  return tp;
}

/*
 * Inserts a Thread into a stack list.
 * @param tp the pointer to the thread to be inserted in the list
 * @param tlp the pointer to the threads list header
 */
void list_insert(Thread *tp, ThreadsList *tlp) {

  tp->p_next = tlp->p_next;
  tlp->p_next = tp;
}


/*
 * Removes a Thread from a stack list and returns it.
 * @param tlp the pointer to the threads list header
 * @return the removed thread pointer
 */
Thread *list_remove(ThreadsList *tlp) {

  Thread *tp = tlp->p_next;
  tlp->p_next = tp->p_next;
  return tp;
}
#endif /* CH_OPTIMIZE_SPEED */

/** @} */

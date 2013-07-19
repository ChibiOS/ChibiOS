/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
 * @file    chlists.c
 * @brief   Thread queues/lists code.
 *
 * @addtogroup internals
 * @details All the functions present in this module, while public, are not
 *          OS APIs and should not be directly used in the user applications
 *          code.
 * @{
 */
#include "ch.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

#if !CH_OPTIMIZE_SPEED || defined(__DOXYGEN__)
/**
 * @brief   Inserts a thread into a priority ordered queue.
 * @note    The insertion is done by scanning the list from the highest
 *          priority toward the lowest.
 *
 * @param[in] tp        the pointer to the thread to be inserted in the list
 * @param[in] tqp       the pointer to the threads list header
 *
 * @notapi
 */
void queue_prio_insert(thread_t *tp, threads_queue_t *tqp) {

  /* cp iterates over the queue.*/
  thread_t *cp = (thread_t *)tqp;
  do {
    /* Iterate to next thread in queue.*/
    cp = cp->p_next;
    /* Not end of queue? and cp has equal or higher priority than tp?.*/
  } while ((cp != (thread_t *)tqp) && (cp->p_prio >= tp->p_prio));
  /* Insertion on p_prev.*/
  tp->p_next = cp;
  tp->p_prev = cp->p_prev;
  tp->p_prev->p_next = cp->p_prev = tp;
}

/**
 * @brief   Inserts a thread into a queue.
 *
 * @param[in] tp        the pointer to the thread to be inserted in the list
 * @param[in] tqp       the pointer to the threads list header
 *
 * @notapi
 */
void queue_insert(thread_t *tp, threads_queue_t *tqp) {

  tp->p_next = (thread_t *)tqp;
  tp->p_prev = tqp->p_prev;
  tp->p_prev->p_next = tqp->p_prev = tp;
}

/**
 * @brief   Removes the first-out thread from a queue and returns it.
 * @note    If the queue is priority ordered then this function returns the
 *          thread with the highest priority.
 *
 * @param[in] tqp       the pointer to the threads list header
 * @return              The removed thread pointer.
 *
 * @notapi
 */
thread_t *queue_fifo_remove(threads_queue_t *tqp) {
  thread_t *tp = tqp->p_next;

  (tqp->p_next = tp->p_next)->p_prev = (thread_t *)tqp;
  return tp;
}

/**
 * @brief   Removes the last-out thread from a queue and returns it.
 * @note    If the queue is priority ordered then this function returns the
 *          thread with the lowest priority.
 *
 * @param[in] tqp   the pointer to the threads list header
 * @return          The removed thread pointer.
 *
 * @notapi
 */
thread_t *queue_lifo_remove(threads_queue_t *tqp) {
  thread_t *tp = tqp->p_prev;

  (tqp->p_prev = tp->p_prev)->p_next = (thread_t *)tqp;
  return tp;
}

/**
 * @brief   Removes a thread from a queue and returns it.
 * @details The thread is removed from the queue regardless of its relative
 *          position and regardless the used insertion method.
 *
 * @param[in] tp        the pointer to the thread to be removed from the queue
 * @return              The removed thread pointer.
 *
 * @notapi
 */
thread_t *queue_dequeue(thread_t *tp) {

  tp->p_prev->p_next = tp->p_next;
  tp->p_next->p_prev = tp->p_prev;
  return tp;
}

/**
 * @brief   Pushes a thread_t on top of a stack list.
 *
 * @param[in] tp    the pointer to the thread to be inserted in the list
 * @param[in] tlp   the pointer to the threads list header
 *
 * @notapi
 */
void list_insert(thread_t *tp, threads_list_t *tlp) {

  tp->p_next = tlp->p_next;
  tlp->p_next = tp;
}

/**
 * @brief   Pops a thread from the top of a stack list and returns it.
 * @pre     The list must be non-empty before calling this function.
 *
 * @param[in] tlp       the pointer to the threads list header
 * @return              The removed thread pointer.
 *
 * @notapi
 */
thread_t *list_remove(threads_list_t *tlp) {

  thread_t *tp = tlp->p_next;
  tlp->p_next = tp->p_next;
  return tp;
}
#endif /* CH_OPTIMIZE_SPEED */

/** @} */

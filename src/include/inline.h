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
 * @file inline.h
 * @brief Inline versions of some critical system routines.
 * @addtogroup Inline
 * @{
 */

#ifndef _INLINE_H_
#define _INLINE_H_

/*
 * Inlined functions if CH_OPTIMIZE_SPEED is enabled.
 * Note: static inlined functions do not duplicate the code in every module
 *       this is true for GCC, not sure about other compilers.
 */
#if CH_OPTIMIZE_SPEED
static INLINE void prio_insert(Thread *tp, ThreadsQueue *tqp) {

  Thread *cp = (Thread *)tqp;
  do {
    cp = cp->p_next;
  } while ((cp != (Thread *)tqp) && (cp->p_prio >= tp->p_prio));
  tp->p_prev = (tp->p_next = cp)->p_prev;
  tp->p_prev->p_next = cp->p_prev = tp;
}

static INLINE void queue_insert(Thread *tp, ThreadsQueue *tqp) {

  tp->p_prev = (tp->p_next = (Thread *)tqp)->p_prev;
  tp->p_prev->p_next = tqp->p_prev = tp;
}

static INLINE Thread *fifo_remove(ThreadsQueue *tqp) {
  Thread *tp = tqp->p_next;

  (tqp->p_next = tp->p_next)->p_prev = (Thread *)tqp;
  return tp;
}

static INLINE Thread *lifo_remove(ThreadsQueue *tqp) {
  Thread *tp = tqp->p_prev;

  (tqp->p_prev = tp->p_prev)->p_next = (Thread *)tqp;
  return tp;
}

static INLINE Thread *dequeue(Thread *tp) {

  tp->p_prev->p_next = tp->p_next;
  tp->p_next->p_prev = tp->p_prev;
  return tp;
}
#endif /* CH_OPTIMIZE_SPEED */

#endif  /* _INLINE_H_ */

/** @} */

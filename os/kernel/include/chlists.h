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
 * @file    chlists.h
 * @brief   Thread queues/lists inlined code.
 *
 * @addtogroup internals
 * @{
 */

#ifndef _CHLISTS_H_
#define _CHLISTS_H_

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Data part of a static threads queue initializer.
 * @details This macro should be used when statically initializing a threads
 *          queue that is part of a bigger structure.
 *
 * @param[in] name      the name of the threads queue variable
 */
#define _THREADSQUEUE_DATA(name) {(Thread *)&name, (Thread *)&name}

/**
 * @brief   Static threads queue initializer.
 * @details Statically initialized threads queues require no explicit
 *          initialization using @p queue_init().
 *
 * @param[in] name      the name of the threads queue variable
 */
#define THREADSQUEUE_DECL(name) ThreadsQueue name = _THREADSQUEUE_DATA(name)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Threads list initialization.
 *
 * @notapi
 */
static inline void list_init(ThreadsList *tlp) {

  tlp->p_next = (Thread *)tlp;
}

/**
 * @brief   Evaluates to @p TRUE if the specified threads list is empty.
 *
 * @notapi
 */
static inline bool_t list_isempty(ThreadsList *tlp) {

  return (bool_t)(tlp->p_next == (Thread *)tlp);
}

/**
 * @brief   Evaluates to @p TRUE if the specified threads list is not empty.
 *
 * @notapi
 */
static inline bool_t list_notempty(ThreadsList *tlp) {

  return (bool_t)(tlp->p_next != (Thread *)tlp);
}

/**
 * @brief   Threads queue initialization.
 *
 * @notapi
 */
static inline void queue_init(ThreadsQueue *tqp) {

  tqp->p_next = tqp->p_prev = (Thread *)tqp;
}

/**
 * @brief   Evaluates to @p TRUE if the specified threads queue is empty.
 *
 * @notapi
 */
static inline bool_t queue_isempty(ThreadsQueue *tqp) {

  return (bool_t)(tqp->p_next == (Thread *)tqp);
}

/**
 * @brief   Evaluates to @p TRUE if the specified threads queue is not empty.
 *
 * @notapi
 */
static inline bool_t queue_notempty(ThreadsQueue *tqp) {

  return (bool_t)(tqp->p_next != (Thread *)tqp);
}

/* If the performance code path has been chosen then all the following
   functions are inlined into the various kernel modules.*/
#if CH_OPTIMIZE_SPEED
static inline void list_insert(Thread *tp, ThreadsList *tlp) {

  tp->p_next = tlp->p_next;
  tlp->p_next = tp;
}

static inline Thread *list_remove(ThreadsList *tlp) {

  Thread *tp = tlp->p_next;
  tlp->p_next = tp->p_next;
  return tp;
}

static inline void queue_prio_insert(Thread *tp, ThreadsQueue *tqp) {

  Thread *cp = (Thread *)tqp;
  do {
    cp = cp->p_next;
  } while ((cp != (Thread *)tqp) && (cp->p_prio >= tp->p_prio));
  tp->p_next = cp;
  tp->p_prev = cp->p_prev;
  tp->p_prev->p_next = cp->p_prev = tp;
}

static inline void queue_insert(Thread *tp, ThreadsQueue *tqp) {

  tp->p_next = (Thread *)tqp;
  tp->p_prev = tqp->p_prev;
  tp->p_prev->p_next = tqp->p_prev = tp;
}

static inline Thread *queue_fifo_remove(ThreadsQueue *tqp) {
  Thread *tp = tqp->p_next;

  (tqp->p_next = tp->p_next)->p_prev = (Thread *)tqp;
  return tp;
}

static inline Thread *queue_lifo_remove(ThreadsQueue *tqp) {
  Thread *tp = tqp->p_prev;

  (tqp->p_prev = tp->p_prev)->p_next = (Thread *)tqp;
  return tp;
}

static inline Thread *queue_dequeue(Thread *tp) {

  tp->p_prev->p_next = tp->p_next;
  tp->p_next->p_prev = tp->p_prev;
  return tp;
}
#endif /* CH_OPTIMIZE_SPEED */

#endif /* _CHLISTS_H_ */

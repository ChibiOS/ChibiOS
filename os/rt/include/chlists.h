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
 * @brief   Thread queues and lists header.
 *
 * @addtogroup queues_list
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
#define _threads_queue_t_DATA(name) {(thread_t *)&name, (thread_t *)&name}

/**
 * @brief   Static threads queue initializer.
 * @details Statically initialized threads queues require no explicit
 *          initialization using @p queue_init().
 *
 * @param[in] name      the name of the threads queue variable
 */
#define threads_queue_t_DECL(name)                                          \
  threads_queue_t name = _threads_queue_t_DATA(name)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  msg_t chQueueGoSleepTimeoutS(threads_queue_t *tqp, systime_t timeout);
  void chQueueWakeupOneI(threads_queue_t *tqp, msg_t msg);
  void chQueueWakeupAllI(threads_queue_t *tqp, msg_t msg);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Threads list initialization.
 *
 * @notapi
 */
static inline void list_init(threads_list_t *tlp) {

  tlp->p_next = (thread_t *)tlp;
}

/**
 * @brief   Evaluates to @p true if the specified threads list is empty.
 *
 * @notapi
 */
static inline bool list_isempty(threads_list_t *tlp) {

  return (bool)(tlp->p_next == (thread_t *)tlp);
}

/**
 * @brief   Evaluates to @p true if the specified threads list is not empty.
 *
 * @notapi
 */
static inline bool list_notempty(threads_list_t *tlp) {

  return (bool)(tlp->p_next != (thread_t *)tlp);
}

/**
 * @brief   Threads queue initialization.
 *
 * @notapi
 */
static inline void queue_init(threads_queue_t *tqp) {

  tqp->p_next = tqp->p_prev = (thread_t *)tqp;
}

/**
 * @brief   Evaluates to @p true if the specified threads queue is empty.
 *
 * @notapi
 */
static inline bool queue_isempty(threads_queue_t *tqp) {

  return (bool)(tqp->p_next == (thread_t *)tqp);
}

/**
 * @brief   Evaluates to @p true if the specified threads queue is not empty.
 *
 * @notapi
 */
static inline bool queue_notempty(threads_queue_t *tqp) {

  return (bool)(tqp->p_next != (thread_t *)tqp);
}

/* If the performance code path has been chosen then all the following
   functions are inlined into the various kernel modules.*/
#if CH_CFG_OPTIMIZE_SPEED
static inline void list_insert(thread_t *tp, threads_list_t *tlp) {

  tp->p_next = tlp->p_next;
  tlp->p_next = tp;
}

static inline thread_t *list_remove(threads_list_t *tlp) {

  thread_t *tp = tlp->p_next;
  tlp->p_next = tp->p_next;
  return tp;
}

static inline void queue_prio_insert(thread_t *tp, threads_queue_t *tqp) {

  thread_t *cp = (thread_t *)tqp;
  do {
    cp = cp->p_next;
  } while ((cp != (thread_t *)tqp) && (cp->p_prio >= tp->p_prio));
  tp->p_next = cp;
  tp->p_prev = cp->p_prev;
  tp->p_prev->p_next = cp->p_prev = tp;
}

static inline void queue_insert(thread_t *tp, threads_queue_t *tqp) {

  tp->p_next = (thread_t *)tqp;
  tp->p_prev = tqp->p_prev;
  tp->p_prev->p_next = tqp->p_prev = tp;
}

static inline thread_t *queue_fifo_remove(threads_queue_t *tqp) {
  thread_t *tp = tqp->p_next;

  (tqp->p_next = tp->p_next)->p_prev = (thread_t *)tqp;
  return tp;
}

static inline thread_t *queue_lifo_remove(threads_queue_t *tqp) {
  thread_t *tp = tqp->p_prev;

  (tqp->p_prev = tp->p_prev)->p_next = (thread_t *)tqp;
  return tp;
}

static inline thread_t *queue_dequeue(thread_t *tp) {

  tp->p_prev->p_next = tp->p_next;
  tp->p_next->p_prev = tp->p_prev;
  return tp;
}
#endif /* CH_CFG_OPTIMIZE_SPEED */

/**
 * @brief   Initializes a threads queue object.
 *
 * @param[out] tqp      pointer to the threads queue object
 *
 * @init
 */
static inline void chQueueObjectInit(threads_queue_t *tqp) {

  queue_init(tqp);
}

#endif /* _CHLISTS_H_ */

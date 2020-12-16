/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    chlists.h
 * @brief   Lists and Queues header.
 *
 * @addtogroup os_lists
 * @{
 */

#ifndef CHLISTS_H
#define CHLISTS_H

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

/**
 * @brief   Type of a generic single link list, it works like a stack.
 * @note    Link fields are void pointers in order to avoid aliasing issues.
 */
typedef struct ch_rt_list {
  void                  *next;      /**< @brief Next in the list/queue.     */
} rt_list_t;

/**
 * @extends rt_list_t
 *
 * @brief   Type of a generic bidirectional linked list header and
 *          element.
 * @note    Link fields are void pointers in order to avoid aliasing issues.
 */
typedef struct ch_rt_queue {
  void                  *next;      /**< @brief Next in the list/queue.     */
  void                  *prev;      /**< @brief Previous in the queue.      */
} rt_queue_t;

/**
 * @extends rt_queue_t
 *
 * @brief   Type of a generic bidirectional linked list header and
 *          element.
 * @note    Link fields are void pointers in order to avoid aliasing issues.
 */
typedef struct ch_rt_priority_queue {
  void                  *next;      /**< @brief Next in the list/queue.     */
  void                  *prev;      /**< @brief Previous in the queue.      */
  tprio_t               prio;
} rt_priority_queue_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Data part of a static queue object initializer.
 * @details This macro should be used when statically initializing a
 *          queue that is part of a bigger structure.
 *
 * @param[in] name      the name of the queue variable
 */
#define _RT_QUEUE_DATA(name) {(thread_t *)&name, (thread_t *)&name}

/**
 * @brief   Static queue object initializer.
 * @details Statically initialized queues require no explicit
 *          initialization using @p queue_init().
 *
 * @param[in] name      the name of the queue variable
 */
#define _RT_QUEUE_DECL(name)                                           \
    rt_queue_t name = _RT_QUEUE_DATA(name)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Early function prototypes required by the following headers.*/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   List initialization.
 *
 * @param[in] lp        pointer to the list object
 *
 * @notapi
 */
static inline void list_init(rt_list_t *tlp) {

  tlp->next = (void *)tlp;
}

/**
 * @brief   Evaluates to @p true if the specified list is empty.
 *
 * @param[in] lp        pointer to the list object
 * @return              The status of the list.
 *
 * @notapi
 */
static inline bool list_isempty(rt_list_t *tlp) {

  return (bool)(tlp->next == (void *)tlp);
}

/**
 * @brief   Evaluates to @p true if the specified list is not empty.
 *
 * @param[in] lp        pointer to the list object
 * @return              The status of the list.
 *
 * @notapi
 */
static inline bool list_notempty(rt_list_t *tlp) {

  return (bool)(tlp->next != (void *)tlp);
}

/**
 * @brief   Pushes an object on top of a stack list.
 *
 * @param[in] p     the pointer to the object to be inserted in the list
 * @param[in] lp    the pointer to the list header
 *
 * @notapi
 */
static inline void list_push(void *p, rt_list_t *lp) {

  ((rt_list_t *)p)->next = lp->next;
  lp->next = p;
}

/**
 * @brief   Pops an object from the top of a stack list and returns it.
 * @pre     The list must be non-empty before calling this function.
 *
 * @param[in] lp        the pointer to the list header
 * @return              The removed object pointer.
 *
 * @notapi
 */
static inline void *list_pop(rt_list_t *lp) {

  void *p = lp->next;
  lp->next = ((rt_list_t *)p)->next;

  return p;
}

/**
 * @brief   Queue initialization.
 *
 * @param[in] qp        pointer to the queue object
 *
 * @notapi
 */
static inline void queue_init(rt_queue_t *qp) {

  qp->next = (void *)qp;
  qp->prev = (void *)qp;
}

/**
 * @brief   Evaluates to @p true if the specified queue is empty.
 *
 * @param[in] qp        pointer to the queue object
 * @return              The status of the queue.
 *
 * @notapi
 */
static inline bool queue_isempty(const rt_queue_t *qp) {

  return (bool)(qp->next == (void *)qp);
}

/**
 * @brief   Evaluates to @p true if the specified queue is not empty.
 *
 * @param[in] qp        pointer to the queue object
 * @return              The status of the queue.
 *
 * @notapi
 */
static inline bool queue_notempty(const rt_queue_t *qp) {

  return (bool)(qp->next != (void *)qp);
}

/**
 * @brief   Inserts an object into a queue.
 *
 * @param[in] p         the pointer to the object to be inserted in the list
 * @param[in] qp        the pointer to the queue header
 *
 * @notapi
 */
static inline void queue_insert(void *p, rt_queue_t *qp) {

  ((rt_queue_t *)p)->next                       = (void *)qp;
  ((rt_queue_t *)p)->prev                       = qp->prev;
  ((rt_queue_t *)((rt_queue_t *)p)->prev)->next = p;
  qp->prev                                      = p;
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
static inline void *queue_fifo_remove(rt_queue_t *qp) {
  void *p = qp->next;

  ((rt_queue_t *)p)->next        = ((rt_queue_t *)p)->next;
  ((rt_queue_t *)qp->next)->prev = (void *)qp;

  return p;
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
static inline void *queue_lifo_remove(rt_queue_t *qp) {
  void *p = qp->prev;

  qp->prev                        = ((rt_queue_t *)p)->prev;
  ((rt_queue_t *)qp->prev)->next = (void *)qp;

  return p;
}

/**
 * @brief   Removes an object from a queue and returns it.
 * @details The object is removed from the queue regardless of its relative
 *          position and regardless the used insertion method.
 *
 * @param[in] tp        the pointer to the object to be removed from the queue
 * @return              The removed object pointer.
 *
 * @notapi
 */
static inline void *queue_dequeue(void *p) {

  ((rt_queue_t *)((rt_queue_t *)p)->prev)->next = ((rt_queue_t *)p)->next;
  ((rt_queue_t *)((rt_queue_t *)p)->next)->prev = ((rt_queue_t *)p)->prev;

  return p;
}

#endif /* CHLISTS_H */

/** @} */

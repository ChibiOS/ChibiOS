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
 * @brief   Type of a generic single link list header and element.
 */
typedef struct ch_list ch_list_t;

/**
 * @brief   Structure representing a generic single link list header
 *          and element.
 */
struct ch_list {
  ch_list_t             *next;      /**< @brief Next in the list/queue.     */
};

/**
 * @brief   Type of a generic bidirectional linked list header and element.
 */
typedef struct ch_queue ch_queue_t;

/**
 * @brief   Structure representing a generic bidirectional linked list header
 *          and element.
 */
struct ch_queue {
  ch_queue_t            *next;      /**< @brief Next in the list/queue.     */
  ch_queue_t            *prev;      /**< @brief Previous in the queue.      */
};

/**
 * @extends ch_queue_t
 *
 * @brief   Type of a generic priority-ordered bidirectional linked list
 *          header and element.
 */
typedef struct ch_priority_queue ch_priority_queue_t;

/**
 * @extends ch_queue_t
 *
 * @brief   Structure representing a generic priority-ordered bidirectional
 *          linked list header and element.
 * @note    Link fields are void pointers in order to avoid aliasing issues.
 */
struct ch_priority_queue {
  ch_priority_queue_t   *next;      /**< @brief Next in the list/queue.     */
  ch_priority_queue_t   *prev;      /**< @brief Previous in the queue.      */
  tprio_t               prio;
};

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
#define __CH_QUEUE_DATA(name) {(ch_queue_t *)&name, (ch_queue_t *)&name}

/**
 * @brief   Static queue object initializer.
 * @details Statically initialized queues require no explicit
 *          initialization using @p queue_init().
 *
 * @param[in] name      the name of the queue variable
 */
#define CH_QUEUE_DECL(name)                                                 \
    ch_queue_t name = __CH_QUEUE_DATA(name)

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
 * @param[in] lp        pointer to the list header
 *
 * @notapi
 */
static inline void ch_list_init(ch_list_t *lp) {

  lp->next = lp;
}

/**
 * @brief   Evaluates to @p true if the specified list is empty.
 *
 * @param[in] lp        pointer to the list header
 * @return              The status of the list.
 *
 * @notapi
 */
static inline bool ch_list_isempty(ch_list_t *lp) {

  return (bool)(lp->next == lp);
}

/**
 * @brief   Evaluates to @p true if the specified list is not empty.
 *
 * @param[in] lp        pointer to the list header
 * @return              The status of the list.
 *
 * @notapi
 */
static inline bool ch_list_notempty(ch_list_t *lp) {

  return (bool)(lp->next != lp);
}

/**
 * @brief   Pushes an element on top of a stack list.
 *
 * @param[in] p     the pointer to the element to be inserted in the list
 * @param[in] lp    the pointer to the list header
 *
 * @notapi
 */
static inline void ch_list_push(ch_list_t *p, ch_list_t *lp) {

  p->next = lp->next;
  lp->next = p;
}

/**
 * @brief   Pops an element from the top of a stack list and returns it.
 * @pre     The list must be non-empty before calling this function.
 *
 * @param[in] lp        the pointer to the list header
 * @return              The removed element pointer.
 *
 * @notapi
 */
static inline ch_list_t *ch_list_pop(ch_list_t *lp) {

  ch_list_t *p = lp->next;
  lp->next = p->next;

  return p;
}

/**
 * @brief   Queue initialization.
 *
 * @param[in] qp        pointer to the queue header
 *
 * @notapi
 */
static inline void ch_queue_init(ch_queue_t *qp) {

  qp->next = qp;
  qp->prev = qp;
}

/**
 * @brief   Evaluates to @p true if the specified queue is empty.
 *
 * @param[in] qp        pointer to the queue header
 * @return              The status of the queue.
 *
 * @notapi
 */
static inline bool ch_queue_isempty(const ch_queue_t *qp) {

  return (bool)(qp->next == qp);
}

/**
 * @brief   Evaluates to @p true if the specified queue is not empty.
 *
 * @param[in] qp        pointer to the queue header
 * @return              The status of the queue.
 *
 * @notapi
 */
static inline bool ch_queue_notempty(const ch_queue_t *qp) {

  return (bool)(qp->next != qp);
}

/**
 * @brief   Inserts an element into a queue.
 *
 * @param[in] p         the pointer to the element to be inserted in the list
 * @param[in] qp        the pointer to the queue header
 *
 * @notapi
 */
static inline void ch_queue_insert(ch_queue_t *p, ch_queue_t *qp) {

  p->next       = qp;
  p->prev       = qp->prev;
  p->prev->next = p;
  qp->prev      = p;
}

/**
 * @brief   Removes the first-out element from a queue and returns it.
 * @note    If the queue is priority ordered then this function returns the
 *          element with the highest priority.
 *
 * @param[in] tqp       the pointer to the queue list header
 * @return              The removed element pointer.
 *
 * @notapi
 */
static inline ch_queue_t *ch_queue_fifo_remove(ch_queue_t *qp) {
  ch_queue_t *p = qp->next;

  qp->next        = p->next;
  qp->next->prev = qp;

  return p;
}

/**
 * @brief   Removes the last-out element from a queue and returns it.
 * @note    If the queue is priority ordered then this function returns the
 *          element with the lowest priority.
 *
 * @param[in] tqp   the pointer to the queue list header
 * @return          The removed element pointer.
 *
 * @notapi
 */
static inline ch_queue_t *ch_queue_lifo_remove(ch_queue_t *qp) {
  ch_queue_t *p = qp->prev;

  qp->prev       = p->prev;
  qp->prev->next = qp;

  return p;
}

/**
 * @brief   Removes an element from a queue and returns it.
 * @details The element is removed from the queue regardless of its relative
 *          position and regardless the used insertion method.
 *
 * @param[in] tp        the pointer to the element to be removed from the queue
 * @return              The removed element pointer.
 *
 * @notapi
 */
static inline ch_queue_t *ch_queue_dequeue(ch_queue_t *p) {

  p->prev->next = p->next;
  p->next->prev = p->prev;

  return p;
}

#endif /* CHLISTS_H */

/** @} */

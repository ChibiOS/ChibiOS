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
 * @file lists.h
 * @brief Lists and queues macros and structures.
 * @addtogroup ThreadLists
 * @{
 */

#ifndef _LISTS_H_
#define _LISTS_H_

typedef struct Thread Thread;

/* Macros good with both ThreadsQueue and ThreadsList.*/
#define isempty(p)      ((p)->p_next == (Thread *)(p))
#define notempty(p)     ((p)->p_next != (Thread *)(p))

/**
 * @brief Generic threads queue header and element.
 * @extends ThreadsList
 */
typedef struct {
  Thread                *p_next;        /**< First @p Thread in the queue, or
                                             @p ThreadQueue when empty.*/
  Thread                *p_prev;        /**< Last @p Thread in the queue, or
                                             @p ThreadQueue when empty.*/
} ThreadsQueue;

/**
 * @brief Generic threads single link list.
 * @details This list behaves like a stack.
 */
typedef struct {
  Thread                *p_next;        /**< Last pushed @p Thread on the stack,
                                             or @p ThreadList when empty.*/
} ThreadsList;

/**
 * Queue initialization.
 */
#define queue_init(tqp) ((tqp)->p_next = (tqp)->p_prev = (Thread *)(tqp));

/**
 * List initialization.
 */
#define list_init(tlp)  ((tlp)->p_next = (Thread *)(tlp))

#if !CH_OPTIMIZE_SPEED

#ifdef __cplusplus
extern "C" {
#endif
  void prio_insert(Thread *tp, ThreadsQueue *tqp);
  void queue_insert(Thread *tp, ThreadsQueue *tqp);
  Thread *fifo_remove(ThreadsQueue *tqp);
  Thread *lifo_remove(ThreadsQueue *tqp);
  Thread *dequeue(Thread *tp);
  void list_insert(Thread *tp, ThreadsList *tlp);
  Thread *list_remove(ThreadsList *tlp);
#ifdef __cplusplus
}
#endif

#endif /* !CH_OPTIMIZE_SPEED */

#endif  /* _LISTS_H_ */

/** @} */

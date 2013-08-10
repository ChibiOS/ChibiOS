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
 * @file    chglobal.h
 * @brief   Data structures with global scope header.
 *
 * @addtogroup global
 */

#ifndef _CHGLOBAL_H_
#define _CHGLOBAL_H_

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
 * @brief   Generic threads single link list, it works like a stack.
 */
typedef struct {

  thread_t              *p_next;    /**< @brief Next in the list/queue.     */
} threads_list_t;

/**
 * @extends threads_list_t
 *
 * @brief   Generic threads bidirectional linked list header and element.
 */
typedef struct {
  thread_t              *p_next;    /**< @brief Next in the list/queue.     */
  thread_t              *p_prev;    /**< @brief Previous in the queue.      */
} threads_queue_t;

/**
 * @extends threads_queue_t
 *
 * @brief   Ready list header.
 */
typedef struct {
  threads_queue_t       r_queue;    /**< @brief Threads queue.              */
  tprio_t               r_prio;     /**< @brief This field must be
                                                initialized to zero.        */
  struct context        r_ctx;      /**< @brief Not used, present because
                                                offsets.                    */
#if CH_CFG_USE_REGISTRY || defined(__DOXYGEN__)
  thread_t              *r_newer;   /**< @brief Newer registry element.     */
  thread_t              *r_older;   /**< @brief Older registry element.     */
#endif
  /* End of the fields shared with the thread_t structure.*/
  thread_t              *r_current; /**< @brief The currently running
                                                thread.                     */
} ready_list_t;

/**
 * @brief   Virtual timers list header.
 * @note    The timers list is implemented as a double link bidirectional list
 *          in order to make the unlink time constant, the reset of a virtual
 *          timer is often used in the code.
 */
typedef struct {
  virtual_timer_t       *vt_next;   /**< @brief Next timer in the delta
                                                list.                       */
  virtual_timer_t       *vt_prev;   /**< @brief Last timer in the delta
                                                list.                       */
  systime_t             vt_delta;   /**< @brief Must be initialized to -1.  */
#if CH_CFG_TIMEDELTA == 0 || defined(__DOXYGEN__)
  volatile systime_t    vt_systime; /**< @brief System Time counter.        */
#endif
#if CH_CFG_TIMEDELTA > 0 || defined(__DOXYGEN__)
  /**
   * @brief   System time of the last tick event.
   */
  systime_t             vt_lasttime;/**< @brief System time of the last
                                                tick event.                 */
#endif
} virtual_timers_list_t;

/**
 * @brief   System data structure.
 * @note    This structure contain all the data areas used by the OS except
 *          stacks.
 */
typedef struct ch_system {
  /**
   * @brief   Ready list header.
   */
  ready_list_t          rlist;
  /**
   * @brief   Virtual timers delta list header.
   */
  virtual_timers_list_t vtlist;
} ch_system_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern ch_system_t ch;
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* _CHGLOBAL_H_ */

/** @} */

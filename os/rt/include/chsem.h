/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013,2014 Giovanni Di Sirio.

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
 * @file    chsem.h
 * @brief   Semaphores macros and structures.
 *
 * @addtogroup semaphores
 * @{
 */

#ifndef _CHSEM_H_
#define _CHSEM_H_

#if CH_CFG_USE_SEMAPHORES || defined(__DOXYGEN__)

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
 * @brief   Semaphore structure.
 */
typedef struct semaphore {
  threads_queue_t       s_queue;    /**< @brief Queue of the threads sleeping
                                                on this semaphore.          */
  cnt_t                 s_cnt;      /**< @brief The semaphore counter.      */
} semaphore_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Data part of a static semaphore initializer.
 * @details This macro should be used when statically initializing a semaphore
 *          that is part of a bigger structure.
 *
 * @param[in] name      the name of the semaphore variable
 * @param[in] n         the counter initial value, this value must be
 *                      non-negative
 */
#define _SEMAPHORE_DATA(name, n) {_THREADS_QUEUE_DATA(name.s_queue), n}

/**
 * @brief   Static semaphore initializer.
 * @details Statically initialized semaphores require no explicit
 *          initialization using @p chSemInit().
 *
 * @param[in] name      the name of the semaphore variable
 * @param[in] n         the counter initial value, this value must be
 *                      non-negative
 */
#define SEMAPHORE_DECL(name, n) semaphore_t name = _SEMAPHORE_DATA(name, n)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void chSemObjectInit(semaphore_t *sp, cnt_t n);
  void chSemReset(semaphore_t *sp, cnt_t n);
  void chSemResetI(semaphore_t *sp, cnt_t n);
  msg_t chSemWait(semaphore_t *sp);
  msg_t chSemWaitS(semaphore_t *sp);
  msg_t chSemWaitTimeout(semaphore_t *sp, systime_t time);
  msg_t chSemWaitTimeoutS(semaphore_t *sp, systime_t time);
  void chSemSignal(semaphore_t *sp);
  void chSemSignalI(semaphore_t *sp);
  void chSemAddCounterI(semaphore_t *sp, cnt_t n);
  msg_t chSemSignalWait(semaphore_t *sps, semaphore_t *spw);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Decreases the semaphore counter.
 * @details This macro can be used when the counter is known to be positive.
 *
 * @iclass
 */
static inline void chSemFastWaitI(semaphore_t *sp) {

  chDbgCheckClassI();

  sp->s_cnt--;
}

/**
 * @brief   Increases the semaphore counter.
 * @details This macro can be used when the counter is known to be not
 *          negative.
 *
 * @iclass
 */
static inline void chSemFastSignalI(semaphore_t *sp) {

  chDbgCheckClassI();

  sp->s_cnt++;
}

/**
 * @brief   Returns the semaphore counter current value.
 *
 * @iclass
 */
static inline cnt_t chSemGetCounterI(semaphore_t *sp) {

  chDbgCheckClassI();

  return sp->s_cnt;
}

#endif /* CH_CFG_USE_SEMAPHORES */

#endif /* _CHSEM_H_ */

/** @} */

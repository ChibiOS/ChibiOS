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
 * @addtogroup Semaphores
 * @{
 */

#ifndef _SEMAPHORES_H_
#define _SEMAPHORES_H_

#ifdef CH_USE_SEMAPHORES

/**
 * Semaphore structure.
 */
typedef struct Semaphore {
  /** Queue of the threads sleeping on this Semaphore.*/
  ThreadsQueue          s_queue;
  /** The Semaphore counter.*/
  cnt_t                 s_cnt;
} Semaphore;

#ifdef __cplusplus
extern "C" {
#endif
  void chSemInit(Semaphore *sp, cnt_t n);
  void chSemReset(Semaphore *sp, cnt_t n);
  void chSemResetI(Semaphore *sp, cnt_t n);
  msg_t chSemWait(Semaphore *sp);
  msg_t chSemWaitS(Semaphore *sp);
#ifdef CH_USE_SEMAPHORES_TIMEOUT
  msg_t chSemWaitTimeout(Semaphore *sp, systime_t time);
  msg_t chSemWaitTimeoutS(Semaphore *sp, systime_t time);
#endif
  void chSemSignal(Semaphore *sp);
  void chSemSignalI(Semaphore *sp);
#ifdef CH_USE_SEMSW
  msg_t chSemSignalWait(Semaphore *sps, Semaphore *spw);
#endif
#ifdef __cplusplus
}
#endif

/**
 * Decreases the semaphore counter, this macro can be used when it is ensured
 * that the counter would not become negative.
 */
#define chSemFastWaitS(sp)      ((sp)->s_cnt--)

/**
 * Increases the semaphore counter, this macro can be used when the counter is
 * not negative.
 */
#define chSemFastSignalI(sp)    ((sp)->s_cnt++)

/**
 * Returns the semaphore counter current value.
 */
#define chSemGetCounter(sp)     ((sp)->s_cnt)

#endif /* CH_USE_SEMAPHORES */

#endif /* _SEMAPHORES_H_ */

/** @} */

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
 * @addtogroup Semaphores
 * @{
 */

#ifndef _SEMAPHORES_H_
#define _SEMAPHORES_H_

#ifdef CH_USE_SEMAPHORES

/**
 * Semaphore structure.
 */
typedef struct {
  /** Queue of the threads sleeping on this Semaphore.*/
  ThreadsQueue    s_queue;
  /** The Semaphore counter.*/
  t_semcnt        s_cnt;
} Semaphore;

void chSemInit(Semaphore *sp, t_semcnt n);
void chSemReset(Semaphore *sp, t_semcnt n);
void chSemResetI(Semaphore *sp, t_semcnt n);
void chSemWait(Semaphore *sp);
void chSemWaitS(Semaphore *sp);
t_msg chSemWaitTimeout(Semaphore *sp, t_time time);
t_msg chSemWaitTimeoutS(Semaphore *sp, t_time time);
void chSemSignal(Semaphore *sp);
void chSemSignalI(Semaphore *sp);
void chSignalWait(Semaphore *sps, Semaphore *spw);

#ifdef CH_USE_RT_SEMAPHORES
void chSemRaisePrioWait(Semaphore *sp);
void chSemLowerPrioSignal(Semaphore *sp);
void chSemRaisePrioSignalWait(Semaphore *sps, Semaphore *spw);
void chSemLowerPrioSignalWait(Semaphore *sps, Semaphore *spw);
#endif

/**
 * Decreases the semaphore counter, this macro can be used when it is ensured
 * that the counter would not become negative.
 */
#define chSemFastWaitS(sp) \
				((sp)->s_cnt--)

/**
 * Increases the semaphore counter, this macro can be used when the counter is
 * not negative.
 */
#define chSemFastSignalI(sp) \
				((sp)->s_cnt++)

/**
 * Returns the semaphore counter current value.
 */
#define chSemGetCounter(sp) \
				((sp)->s_cnt)

#endif /* CH_USE_SEMAPHORES */

#endif /* _SEM_H_ */

/** @} */

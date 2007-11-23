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
 * @addtogroup Scheduler
 * @{
 */

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

/** Normal \p chSchReadyI() message.*/
#define RDY_OK      0
/** Returned if the thread was made ready because a timeout.*/
#define RDY_TIMEOUT -1
/** Returned if the thread was made ready because a reset.*/
#define RDY_RESET   -2

#define firstprio(rlp)   ((rlp)->p_next->p_prio)

/**
 * Ready list header.
 */
typedef struct {
  ThreadsQueue  r_queue;
  t_prio        r_prio;
  t_cnt         r_preempt;
#ifndef CH_CURRP_REGISTER_CACHE
  Thread        *r_current;
#endif
#ifdef CH_USE_SYSTEMTIME
  volatile t_time r_stime;
#endif
} ReadyList;

extern ReadyList rlist;

/*
 * Scheduler APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif
  void chSchInit(void);
  void chSchReadyI(Thread *tp, t_msg msg);
  void chSchGoSleepS(t_tstate newstate);
  void chSchWakeupS(Thread *tp, t_msg msg);
  void chSchRescheduleS(void);
  void chSchDoRescheduleI(void);
  BOOL chSchRescRequiredI(void);
  void chSchTimerHandlerI(void);
#ifdef __cplusplus
}
#endif

/**
 * Current thread pointer.
 * @note Dont use this directly but use the \p chThdSelf()
 *       instead. Direct use of system global variables is discouraged because
 *       portability reasons.
 */
#ifdef CH_CURRP_REGISTER_CACHE
register Thread *currp asm(CH_CURRP_REGISTER_CACHE);
#else
#define currp rlist.r_current
#endif

/**
 * System ticks counter.
 * @note Dont use this directly but use the \p chSysGetTime()
 *       instead. Direct use of system global variables is discouraged because
 *       portability reasons.
 */
extern volatile t_time stime;

#endif /* _SCHEDULER_H_ */

/** @} */

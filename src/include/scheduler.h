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
 * @addtogroup Scheduler
 * @{
 */

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

/** Normal \p chSchReadyI() message. */
#define RDY_OK          0
/** Returned when the thread was made ready because of a timeout. */
#define RDY_TIMEOUT     -1
/** Returned when the thread was made ready because of a reset. */
#define RDY_RESET       -2

/** Pseudo priority used by the ready list header, do not use.*/
#define NOPRIO          0
/** Idle thread priority.*/
#define IDLEPRIO        1
/** Lowest user priority.*/
#define LOWPRIO         2
/** Normal user priority.*/
#define NORMALPRIO      64
/** Highest user priority.*/
#define HIGHPRIO        127
/** Greatest possible priority.*/
#define ABSPRIO         255

/** Infinite time specification for all the syscalls with a timeout
    specification.*/
#define TIME_INFINITE   0

/** The priority of the first thread on the given ready list. */
#define firstprio(rlp)  ((rlp)->p_next->p_prio)

/**
 * Ready list header.
 * @extends ThreadsQueue
 */
typedef struct {
  /** Next \p Thread in the ready list.*/
  Thread                *p_next;
  /** Previous \p Thread in the ready list.*/
  Thread                *p_prev;
  /* End of the fields shared with the ThreadsQueue structure. */
  /** The thread priority.*/
  tprio_t               r_prio;
  /* End of the fields shared with the Thread structure. */
#ifdef CH_USE_ROUNDROBIN
  cnt_t                 r_preempt;
#endif
#ifndef CH_CURRP_REGISTER_CACHE
  /** the currently running thread */
  Thread                *r_current;
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
  Thread *chSchReadyI(Thread *tp);
  void chSchGoSleepS(tstate_t newstate);
  msg_t chSchGoSleepTimeoutS(tstate_t newstate, systime_t time);
  void chSchWakeupS(Thread *tp, msg_t msg);
  void chSchDoRescheduleI(void);
  void chSchRescheduleS(void);
  bool_t chSchRescRequiredI(void);
#ifdef __cplusplus
}
#endif

#ifdef CH_CURRP_REGISTER_CACHE
register Thread *currp asm(CH_CURRP_REGISTER_CACHE);
#else
#define currp rlist.r_current
#endif

#endif /* _SCHEDULER_H_ */

/** @} */

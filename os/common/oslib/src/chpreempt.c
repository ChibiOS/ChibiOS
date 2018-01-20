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
 * @file    chpreempt.c
 * @brief   Preemption enforcement code.
 *
 * @addtogroup preemption_enforcement
 * @details This modules export hook macros required for implementing
 *          a preemptive round robin mode for threads at the same priority
 *          level.<br>
 *          This method is alternative to the ChibiOS/RT native implementation
 *          which is not compatible with the tick-less mode, however, this
 *          timers-based solution can decrease threads context-switch
 *          performance because the added overhead.
 * @note    This file is not included automatically by @p ch.h, you need
 *          to:
 *          - Define @p CH_CFG_ROUND_ROBIN_QUANTUM in chconf.h. It is the
 *            time quantum in ticks.
 *          - Include @p chpreempt.h from @p chconf.h.
 *          - Add all the macros and functions to the appropriate hooks in
 *            chconf.h.
 *          - Explicitely add @p chpreempt.c to your makefile.
 *          .
 */

#include "ch.h"

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static void preempt_cb(void *p) {

  (void)p;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Hook code for system initialization.
 *
 * @notapi
 */
void ch_preempt_system_init(void) {

  chVTObjectInit(&ch.preempt_vt);
}

/**
 * @brief   Hook code for context switch.
 *
 * @notapi
 */
void ch_preempt_thread_switch(void) {

  chVTSetI(&ch.preempt_vt, CH_CFG_ROUND_ROBIN_QUANTUM, preempt_cb, NULL);
}

/**
 * @brief   Evaluates if preemption is required.
 * @details The decision is taken by comparing the relative priorities and
 *          depending on the state of the round robin timeout counter.
 * @note    Not a user function, it is meant to be invoked by the scheduler
 *          itself or from within the port layer.
 *
 * @retval true         if there is a thread that must go in running state
 *                      immediately.
 * @retval false        if preemption is not required.
 *
 * @special
 */
bool chSchIsPreemptionRequired(void) {
  tprio_t p1 = firstprio(&ch.rlist.queue);
  tprio_t p2 = currp->prio;

  /* If the running thread has not reached its time quantum, reschedule only
     if the first thread on the ready queue has a higher priority.
     Otherwise, if the running thread has used up its time quantum, reschedule
     if the first thread on the ready queue has equal or higher priority.*/
  return chVTIsArmedI(&ch.preempt_vt) ? (p1 > p2) : (p1 >= p2);
}
/**
 * @brief   Switches to the first thread on the runnable queue.
 * @details The current thread is positioned in the ready list behind or
 *          ahead of all threads having the same priority depending on
 *          if it used its whole time slice.
 * @note    Not a user function, it is meant to be invoked by the scheduler
 *          itself or from within the port layer.
 *
 * @special
 */
void chSchDoReschedule(void) {
  thread_t *otp = currp;

  /* Picks the first thread from the ready queue and makes it current.*/
  currp = queue_fifo_remove(&ch.rlist.queue);
  currp->state = CH_STATE_CURRENT;

  /* Handling idle-leave hook.*/
  if (otp->prio == IDLEPRIO) {
    CH_CFG_IDLE_LEAVE_HOOK();
  }

  /* There are two different scenarios to handle on preemption: time quantum
     elapsed or not.*/
  if (!chVTIsArmedI(&ch.preempt_vt)) {

    /* The thread consumed its time quantum so it is enqueued behind threads
       with same priority level, however, it acquires a new time quantum.*/
    otp = chSchReadyI(otp);
  }
  else {
    /* The thread didn't consume all its time quantum so it is put ahead of
       threads with equal priority and does not acquire a new time quantum.*/
    otp = chSchReadyAheadI(otp);
  }

  /* Swap operation as tail call.*/
  chSysSwitch(currp, otp);
}

/** @} */

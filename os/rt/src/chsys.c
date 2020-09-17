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
 * @file    rt/src/chsys.c
 * @brief   System related code.
 *
 * @addtogroup system
 * @details System related APIs and services:
 *          - Initialization.
 *          - Locks.
 *          - Interrupt Handling.
 *          - Power Management.
 *          - Abnormal Termination.
 *          - Realtime counter.
 *          .
 * @{
 */

#include "ch.h"

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Default OS instance.
 */
os_instance_t ch;

/*os_instance_t * volatile chp = &ch;*/

#if (CH_CFG_NO_IDLE_THREAD == FALSE) || defined(__DOXYGEN__)
/**
 * @brief   Default instance idle thread working area.
 */
THD_WORKING_AREA(ch_idle_thread_wa, PORT_IDLE_THREAD_STACK_SIZE);
#endif

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   ChibiOS/RT initialization.
 * @details After executing this function the current instructions stream
 *          becomes the main thread.
 * @pre     Interrupts must disabled before invoking this function.
 * @post    The main thread is created with priority @p NORMALPRIO and
 *          interrupts are enabled.
 *
 * @special
 */
void chSysInit(void) {

  /* OS library modules.*/
  __oslib_init();

  /* Initializing default OS instance.*/
  {
    extern stkalign_t __main_thread_stack_base__,
                      __main_thread_stack_end__;
    static const os_instance_config_t default_cfg = {
      .name             = "c0",
      .mainthread_base  = &__main_thread_stack_base__,
      .mainthread_end   = &__main_thread_stack_end__,
#if CH_CFG_NO_IDLE_THREAD == FALSE
      .idlethread_base  = THD_WORKING_AREA_BASE(ch_idle_thread_wa),
      .idlethread_end   = THD_WORKING_AREA_END(ch_idle_thread_wa)
#endif
    };

    chSchObjectInit(&ch, &default_cfg);
  }

  /* It is alive now.*/
  chSysUnlock();
}

/**
 * @brief   Halts the system.
 * @details This function is invoked by the operating system when an
 *          unrecoverable error is detected, for example because a programming
 *          error in the application code that triggers an assertion while
 *          in debug mode.
 * @note    Can be invoked from any system state.
 *
 * @param[in] reason        pointer to an error string
 *
 * @special
 */
void chSysHalt(const char *reason) {

  port_disable();

  /* Logging the event.*/
  __trace_halt(reason);

  /* Pointing to the passed message.*/
  currcore->dbg.panic_msg = reason;

  /* Halt hook code, usually empty.*/
  CH_CFG_SYSTEM_HALT_HOOK(reason);

  /* Harmless infinite loop.*/
  while (true) {
  }
}

/**
 * @brief   System integrity check.
 * @details Performs an integrity check of the important ChibiOS/RT data
 *          structures.
 * @note    The appropriate action in case of failure is to halt the system
 *          before releasing the critical zone.
 * @note    If the system is corrupted then one possible outcome of this
 *          function is an exception caused by @p NULL or corrupted pointers
 *          in list elements. Exception vectors must be monitored as well.
 * @note    This function is not used internally, it is up to the
 *          application to define if and where to perform system
 *          checking.
 * @note    Performing all tests at once can be a slow operation and can
 *          degrade the system response time. It is suggested to execute
 *          one test at time and release the critical zone in between tests.
 *
 * @param[in] testmask  Each bit in this mask is associated to a test to be
 *                      performed.
 * @return              The test result.
 * @retval false        The test succeeded.
 * @retval true         Test failed.
 *
 * @iclass
 */
bool chSysIntegrityCheckI(unsigned testmask) {
  os_instance_t *oip = currcore;
  cnt_t n;

  chDbgCheckClassI();

  /* Ready List integrity check.*/
  if ((testmask & CH_INTEGRITY_RLIST) != 0U) {
    thread_t *tp;

    /* Scanning the ready list forward.*/
    n = (cnt_t)0;
    tp = oip->rlist.queue.next;
    while (tp != (thread_t *)&oip->rlist.queue) {
      n++;
      tp = tp->queue.next;
    }

    /* Scanning the ready list backward.*/
    tp = oip->rlist.queue.prev;
    while (tp != (thread_t *)&oip->rlist.queue) {
      n--;
      tp = tp->queue.prev;
    }

    /* The number of elements must match.*/
    if (n != (cnt_t)0) {
      return true;
    }
  }

  /* Timers list integrity check.*/
  if ((testmask & CH_INTEGRITY_VTLIST) != 0U) {
    virtual_timer_t * vtp;

    /* Scanning the timers list forward.*/
    n = (cnt_t)0;
    vtp = oip->vtlist.next;
    while (vtp != (virtual_timer_t *)&oip->vtlist) {
      n++;
      vtp = vtp->next;
    }

    /* Scanning the timers list backward.*/
    vtp = oip->vtlist.prev;
    while (vtp != (virtual_timer_t *)&oip->vtlist) {
      n--;
      vtp = vtp->prev;
    }

    /* The number of elements must match.*/
    if (n != (cnt_t)0) {
      return true;
    }
  }

#if CH_CFG_USE_REGISTRY == TRUE
  if ((testmask & CH_INTEGRITY_REGISTRY) != 0U) {
    thread_t *tp;

    /* Scanning the ready list forward.*/
    n = (cnt_t)0;
    tp = oip->rlist.newer;
    while (tp != (thread_t *)&oip->rlist) {
      n++;
      tp = tp->newer;
    }

    /* Scanning the ready list backward.*/
    tp = oip->rlist.older;
    while (tp != (thread_t *)&oip->rlist) {
      n--;
      tp = tp->older;
    }

    /* The number of elements must match.*/
    if (n != (cnt_t)0) {
      return true;
    }
  }
#endif /* CH_CFG_USE_REGISTRY == TRUE */

#if defined(PORT_INTEGRITY_CHECK)
  if ((testmask & CH_INTEGRITY_PORT) != 0U) {
    PORT_INTEGRITY_CHECK();
  }
#endif

  return false;
}

/**
 * @brief   Handles time ticks for round robin preemption and timer increments.
 * @details Decrements the remaining time quantum of the running thread
 *          and preempts it when the quantum is used up. Increments system
 *          time and manages the timers.
 * @note    The frequency of the timer determines the system tick granularity
 *          and, together with the @p CH_CFG_TIME_QUANTUM macro, the round robin
 *          interval.
 *
 * @iclass
 */
void chSysTimerHandlerI(void) {
#if (CH_CFG_TIME_QUANTUM > 0) || (CH_DBG_THREADS_PROFILING == TRUE)
  thread_t *currtp = chThdGetSelfX();
#endif

  chDbgCheckClassI();

#if CH_CFG_TIME_QUANTUM > 0
  /* Running thread has not used up quantum yet? */
  if (currtp->ticks > (tslices_t)0) {
    /* Decrement remaining quantum.*/
    currtp->ticks--;
  }
#endif
#if CH_DBG_THREADS_PROFILING == TRUE
  currtp->time++;
#endif
  chVTDoTickI();
  CH_CFG_SYSTEM_TICK_HOOK();
}

/**
 * @brief   Returns the execution status and enters a critical zone.
 * @details This functions enters into a critical zone and can be called
 *          from any context. Because its flexibility it is less efficient
 *          than @p chSysLock() which is preferable when the calling context
 *          is known.
 * @post    The system is in a critical zone.
 *
 * @return              The previous system status, the encoding of this
 *                      status word is architecture-dependent and opaque.
 *
 * @xclass
 */
syssts_t chSysGetStatusAndLockX(void) {

  syssts_t sts = port_get_irq_status();
  if (port_irq_enabled(sts)) {
    if (port_is_isr_context()) {
      chSysLockFromISR();
    }
    else {
      chSysLock();
    }
  }
  return sts;
}

/**
 * @brief   Restores the specified execution status and leaves a critical zone.
 * @note    A call to @p chSchRescheduleS() is automatically performed
 *          if exiting the critical zone and if not in ISR context.
 *
 * @param[in] sts       the system status to be restored.
 *
 * @xclass
 */
void chSysRestoreStatusX(syssts_t sts) {

  if (port_irq_enabled(sts)) {
    if (port_is_isr_context()) {
      chSysUnlockFromISR();
    }
    else {
      chSchRescheduleS();
      chSysUnlock();
    }
  }
}

#if (PORT_SUPPORTS_RT == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Realtime window test.
 * @details This function verifies if the current realtime counter value
 *          lies within the specified range or not. The test takes care
 *          of the realtime counter wrapping to zero on overflow.
 * @note    When start==end then the function returns always false because a
 *          null time range is specified.
 * @note    This function is only available if the port layer supports the
 *          option @p PORT_SUPPORTS_RT.
 *
 * @param[in] cnt       the counter value to be tested
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @xclass
 */
bool chSysIsCounterWithinX(rtcnt_t cnt, rtcnt_t start, rtcnt_t end) {

  return (bool)(((rtcnt_t)cnt - (rtcnt_t)start) <
                ((rtcnt_t)end - (rtcnt_t)start));
}

/**
 * @brief   Polled delay.
 * @note    The real delay is always few cycles in excess of the specified
 *          value.
 * @note    This function is only available if the port layer supports the
 *          option @p PORT_SUPPORTS_RT.
 *
 * @param[in] cycles    number of cycles
 *
 * @xclass
 */
void chSysPolledDelayX(rtcnt_t cycles) {
  rtcnt_t start = chSysGetRealtimeCounterX();
  rtcnt_t end  = start + cycles;

  while (chSysIsCounterWithinX(chSysGetRealtimeCounterX(), start, end)) {
  }
}
#endif /* PORT_SUPPORTS_RT == TRUE */

/** @} */

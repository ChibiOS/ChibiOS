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
 * @file    rt/src/chvt.c
 * @brief   Time and Virtual Timers module code.
 *
 * @addtogroup time
 * @details Time and Virtual Timers related APIs and services.
 * @{
 */

#include "ch.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

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

/**
 * @brief   List empty check.
 *
 * @param[in] dlhp      pointer to the delta list header
 *
 * @notapi
 */
static inline bool is_vtlist_empty(delta_list_t *dlhp) {

  return (bool)(dlhp == dlhp->next);
}

/**
 * @brief   Last timer in the list check.
 *
 * @param[in] dlhp      pointer to the delta list header
 * @param[in] dlp       pointer to the delta list element
 *
 * @notapi
 */
static inline bool is_last_timer(delta_list_t *dlhp, delta_list_t *dlp) {

  return (bool)(dlp->next == dlhp);
}

/**
 * @brief   Fist timer in the list check.
 *
 * @param[in] dlhp      pointer to the delta list header
 * @param[in] dlp       pointer to the delta list element
 *
 * @notapi
 */
static inline bool is_first_timer(delta_list_t *dlhp, delta_list_t *dlp) {

  return (bool)(dlhp->next == dlp);
}

/**
 * @brief   Timer check.
 *
 * @param[in] dlhp      pointer to the delta list header
 * @param[in] dlp       pointer to the delta list element
 *
 * @notapi
 */
static inline bool is_timer(delta_list_t *dlhp, delta_list_t *dlp) {

  return (bool)(dlp != dlhp);
}

#if (CH_CFG_ST_TIMEDELTA > 0) || defined(__DOXYGEN__)
/**
 * @brief   Delta list compression.
 *
 * @param[in] vtlp      pointer to the delta list to be compressed
 * @param[in] deltanow  interval to be compacted starting from "lasttime"
 *
 * @notapi
 */
static void vt_list_compress(virtual_timers_list_t *vtlp,
                             sysinterval_t deltanow) {
  delta_list_t *dlp = vtlp->dlist.next;

  /* The loop is bounded because the delta list header has the delta field
     set to (sysinterval_t)-1 which is larger than all deltas.*/
  while (dlp->delta < deltanow) {
    deltanow  -= dlp->delta;
    dlp->delta = (sysinterval_t)0;
    dlp        = dlp->next;
  }

  vtlp->lasttime = vtlp->lasttime + deltanow;

  /* Adjusting next timer in the list, if any.*/
  if (is_timer(&vtlp->dlist, dlp)) {
    dlp->delta -= deltanow;
  }
}
#endif

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enables a virtual timer.
 * @details The timer is enabled and programmed to trigger after the delay
 *          specified as parameter.
 * @pre     The timer must not be already armed before calling this function.
 * @note    The callback function is invoked from interrupt context.
 *
 * @param[out] vtp      the @p virtual_timer_t structure pointer
 * @param[in] delay     the number of ticks before the operation timeouts, the
 *                      special values are handled as follow:
 *                      - @a TIME_INFINITE is allowed but interpreted as a
 *                        normal time specification.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 * @param[in] vtfunc    the timer callback function. After invoking the
 *                      callback the timer is disabled and the structure can
 *                      be disposed or reused.
 * @param[in] par       a parameter that will be passed to the callback
 *                      function
 *
 * @iclass
 */
void chVTDoSetI(virtual_timer_t *vtp, sysinterval_t delay,
                vtfunc_t vtfunc, void *par) {
  virtual_timers_list_t *vtlp = &currcore->vtlist;
  delta_list_t *dlp;
  sysinterval_t delta;

  chDbgCheckClassI();
  chDbgCheck((vtp != NULL) && (vtfunc != NULL) && (delay != TIME_IMMEDIATE));

  vtp->par = par;
  vtp->func = vtfunc;

#if CH_CFG_ST_TIMEDELTA > 0
  {
    systime_t now = chVTGetSystemTimeX();
    sysinterval_t deltanow;

    /* If the requested delay is lower than the minimum safe delta then it
       is raised to the minimum safe value.*/
    if (delay < (sysinterval_t)CH_CFG_ST_TIMEDELTA) {
      delay = (sysinterval_t)CH_CFG_ST_TIMEDELTA;
    }

    /* Special case where the timers list is empty.*/
    if (is_vtlist_empty(&vtlp->dlist)) {

      /* The delta list is empty, the current time becomes the new
         delta list base time, the timer is inserted.*/
      vtlp->lasttime = now;
      vtlp->dlist.next = &vtp->dlist;
      vtlp->dlist.prev = &vtp->dlist;
      vtp->dlist.next = &vtlp->dlist;
      vtp->dlist.prev = &vtlp->dlist;
      vtp->dlist.delta = delay;

#if CH_CFG_INTERVALS_SIZE > CH_CFG_ST_RESOLUTION
      /* The delta could be too large for the physical timer to handle.*/
      if (delay > (sysinterval_t)TIME_MAX_SYSTIME) {
        delay = (sysinterval_t)TIME_MAX_SYSTIME;
      }
#endif

      /* Being the first element in the list the alarm timer is started.*/
      port_timer_start_alarm(chTimeAddX(vtlp->lasttime, delay));

      return;
    }

    /* Delay as delta from 'lasttime'. Note, it can overflow and the value
       becomes lower than 'deltanow'.*/
    deltanow = chTimeDiffX(vtlp->lasttime, now);
    delta    = deltanow + delay;

    /* Scenario where a very large delay exceeded the numeric range, it
       requires a special handling, the compression procedure.*/
    if (delta < deltanow) {
      vt_list_compress(vtlp, deltanow);
      delta -= deltanow;
    }
    else if (delta < vtlp->dlist.next->delta) {
      sysinterval_t deadline_delta;

      /* A small delay that will become the first element in the delta list
         and next deadline.*/
      deadline_delta = delta;
#if CH_CFG_INTERVALS_SIZE > CH_CFG_ST_RESOLUTION
      /* The delta could be too large for the physical timer to handle.*/
      if (deadline_delta > (sysinterval_t)TIME_MAX_SYSTIME) {
        deadline_delta = (sysinterval_t)TIME_MAX_SYSTIME;
      }
#endif
      port_timer_set_alarm(chTimeAddX(vtlp->lasttime, deadline_delta));
    }
  }
#else /* CH_CFG_ST_TIMEDELTA == 0 */
  /* Delta is initially equal to the specified delay.*/
  delta = delay;
#endif /* CH_CFG_ST_TIMEDELTA == 0 */

  /* The delta list is scanned in order to find the correct position for
     this timer. */
  dlp = vtlp->dlist.next;
  while (dlp->delta < delta) {
    /* Debug assert if the timer is already in the list.*/
    chDbgAssert(dlp != &vtp->dlist, "timer already armed");

    delta -= dlp->delta;
    dlp = dlp->next;
  }

  /* The timer is inserted in the delta list.*/
  vtp->dlist.next       = dlp;
  vtp->dlist.prev       = vtp->dlist.next->prev;
  vtp->dlist.prev->next = &vtp->dlist;
  dlp->prev             = &vtp->dlist;
  vtp->dlist.delta      = delta;

  /* Calculate new delta for the following entry.*/
  dlp->delta -= delta;

  /* Special case when the timer is in last position in the list, the
     value in the header must be restored.*/
  vtlp->dlist.delta = (sysinterval_t)-1;
}

/**
 * @brief   Disables a Virtual Timer.
 * @pre     The timer must be in armed state before calling this function.
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 *
 * @iclass
 */
void chVTDoResetI(virtual_timer_t *vtp) {
  virtual_timers_list_t *vtlp = &currcore->vtlist;

  chDbgCheckClassI();
  chDbgCheck(vtp != NULL);
  chDbgAssert(vtp->func != NULL, "timer not set or already triggered");

#if CH_CFG_ST_TIMEDELTA == 0

  /* The delta of the timer is added to the next timer.*/
  vtp->dlist.next->delta += vtp->dlist.delta;

 /* Removing the element from the delta list.*/
  vtp->dlist.prev->next = vtp->dlist.next;
  vtp->dlist.next->prev = vtp->dlist.prev;
  vtp->func = NULL;

  /* The above code changes the value in the header when the removed element
     is the last of the list, restoring it.*/
  vtlp->dlist.delta = (sysinterval_t)-1;
#else /* CH_CFG_ST_TIMEDELTA > 0 */
  sysinterval_t nowdelta, delta;

  /* If the timer is not the first of the list then it is simply unlinked
     else the operation is more complex.*/
  if (!is_first_timer(&vtlp->dlist, &vtp->dlist)) {
    /* Removing the element from the delta list.*/
    vtp->dlist.prev->next = vtp->dlist.next;
    vtp->dlist.next->prev = vtp->dlist.prev;
    vtp->func = NULL;

    /* Adding delta to the next element, if it is not the last one.*/
    if (is_timer(&vtlp->dlist, vtp->dlist.next))
      vtp->dlist.next->delta += vtp->dlist.delta;

    return;
  }

  /* Removing the first timer from the list.*/
  vtlp->dlist.next = vtp->dlist.next;
  vtlp->dlist.next->prev = &vtlp->dlist;
  vtp->func = NULL;

  /* If the list become empty then the alarm timer is stopped and done.*/
  if (is_vtlist_empty(&vtlp->dlist)) {
    port_timer_stop_alarm();

    return;
  }

  /* The delta of the removed timer is added to the new first timer.*/
  vtlp->dlist.next->delta += vtp->dlist.delta;

  /* If the new first timer has a delta of zero then the alarm is not
     modified, the already programmed alarm will serve it.*/
/*  if (vtlp->dlist.next->delta == 0) {
    return;
  }*/

  /* Distance in ticks between the last alarm event and current time.*/
  nowdelta = chTimeDiffX(vtlp->lasttime, chVTGetSystemTimeX());

  /* If the current time surpassed the time of the next element in list
     then the event interrupt is already pending, just return.*/
  if (nowdelta >= vtlp->dlist.next->delta) {
    return;
  }

  /* Distance from the next scheduled event and now.*/
  delta = vtlp->dlist.next->delta - nowdelta;

  /* Making sure to not schedule an event closer than CH_CFG_ST_TIMEDELTA
     ticks from now.*/
  if (delta < (sysinterval_t)CH_CFG_ST_TIMEDELTA) {
    delta = nowdelta + (sysinterval_t)CH_CFG_ST_TIMEDELTA;
  }
  else {
    delta = nowdelta + delta;
#if CH_CFG_INTERVALS_SIZE > CH_CFG_ST_RESOLUTION
    /* The delta could be too large for the physical timer to handle.*/
    if (delta > (sysinterval_t)TIME_MAX_SYSTIME) {
      delta = (sysinterval_t)TIME_MAX_SYSTIME;
    }
#endif
  }
  port_timer_set_alarm(chTimeAddX(vtlp->lasttime, delta));
#endif /* CH_CFG_ST_TIMEDELTA > 0 */
}

/**
 * @brief   Virtual timers ticker.
 * @note    The system lock is released before entering the callback and
 *          re-acquired immediately after. It is callback's responsibility
 *          to acquire the lock if needed. This is done in order to reduce
 *          interrupts jitter when many timers are in use.
 *
 * @iclass
 */
void chVTDoTickI(void) {
  virtual_timers_list_t *vtlp = &currcore->vtlist;

  chDbgCheckClassI();

#if CH_CFG_ST_TIMEDELTA == 0
  vtlp->systime++;
  if (!is_vtlist_empty(&vtlp->dlist)) {
    /* The list is not empty, processing elements on top.*/
    --vtlp->dlist.next->delta;
    while (vtlp->dlist.next->delta == (sysinterval_t)0) {
      virtual_timer_t *vtp;
      vtfunc_t fn;

      vtp = vtlp->dlist.next;
      fn = vtp->func;
      vtp->func = NULL;
      vtp->dlist.next->prev = (virtual_timer_t *)vtlp;
      vtlp->dlist.next = vtp->dlist.next;
      chSysUnlockFromISR();
      fn(vtp->par);
      chSysLockFromISR();
    }
  }
#else /* CH_CFG_ST_TIMEDELTA > 0 */
  delta_list_t *dlp;
  systime_t now;
  sysinterval_t delta, nowdelta;

  /* Looping through timers.*/
  dlp = vtlp->dlist.next;
  while (true) {

    /* Getting the system time as reference.*/
    now = chVTGetSystemTimeX();
    nowdelta = chTimeDiffX(vtlp->lasttime, now);

    /* The list scan is limited by the timers header having
       "vtlp->dlist.delta == (sysinterval_t)-1" which is
       greater than all deltas.*/
    if (nowdelta < dlp->delta) {
      break;
    }

    /* Consuming all timers between "vtp->lasttime" and now.*/
    do {
      vtfunc_t fn;
      virtual_timer_t *vtp = (virtual_timer_t *)dlp;

      /* The "last time" becomes this timer's expiration time.*/
      vtlp->lasttime += dlp->delta;
      nowdelta -= dlp->delta;

      /* Removing the timer from the list.*/
      dlp->next->prev = &vtlp->dlist;
      vtlp->dlist.next = dlp->next;

      /* Calling the associated function and then marking the timer as
         non active.*/
      fn = vtp->func;
      vtp->func = NULL;

      /* If the list becomes empty then the timer is stopped.*/
      if (is_vtlist_empty(&vtlp->dlist)) {
        port_timer_stop_alarm();
      }

      /* The callback is invoked outside the kernel critical zone.*/
      chSysUnlockFromISR();
      fn(vtp->par);
      chSysLockFromISR();

      /* Next element in the list.*/
      dlp = vtlp->dlist.next;
    }
    while (dlp->delta <= nowdelta);
  }

  /* If the list is empty, nothing else to do.*/
  if (is_vtlist_empty(&vtlp->dlist)) {
    return;
  }

  /* The "unprocessed nowdelta" time slice is added to "last time"
     and subtracted to next timer's delta.*/
  vtlp->lasttime += nowdelta;
  vtlp->dlist.next->delta -= nowdelta;

  /* Recalculating the next alarm time.*/
  delta = dlp->delta - chTimeDiffX(vtlp->lasttime, now);
  if (delta < (sysinterval_t)CH_CFG_ST_TIMEDELTA) {
    delta = (sysinterval_t)CH_CFG_ST_TIMEDELTA;
  }
#if CH_CFG_INTERVALS_SIZE > CH_CFG_ST_RESOLUTION
  /* The delta could be too large for the physical timer to handle.*/
  else if (delta > (sysinterval_t)TIME_MAX_SYSTIME) {
    delta = (sysinterval_t)TIME_MAX_SYSTIME;
  }
#endif
  port_timer_set_alarm(chTimeAddX(now, delta));

  chDbgAssert(chTimeDiffX(vtlp->lasttime, chVTGetSystemTimeX()) <=
              chTimeDiffX(vtlp->lasttime, chTimeAddX(now, delta)),
              "exceeding delta");
#endif /* CH_CFG_ST_TIMEDELTA > 0 */
}

#if (CH_CFG_USE_TIMESTAMP == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Generates a monotonic time stamp.
 * @details This function generates a monotonic time stamp synchronized with
 *          the system time. The time stamp has the same resolution of
 *          system time.
 * @note    There is an assumption, this function must be called at
 *          least once before the system time wraps back to zero or
 *          synchronization is lost. You may use a periodic virtual timer with
 *          a very large interval in order to keep time stamps synchronized
 *          by calling this function.
 *
 * @return              The time stamp.
 *
 * @iclass
 */
systimestamp_t chVTGetTimeStampI(void) {
  systimestamp_t last, stamp;
  systime_t now;

  chDbgCheckClassI();

  /* Current system time.*/
  now = chVTGetSystemTimeX();

  /* Last time stamp generated.*/
  last = ch.vtlist.laststamp;

  /* Interval between the last time stamp and current time used for a new
     time stamp. Note that this fails if the interval is larger than a
     systime_t type.*/
  stamp = last + (systimestamp_t)chTimeDiffX((sysinterval_t)last, now);

  chDbgAssert(ch.vtlist.laststamp <= stamp, "wrapped");

  /* Storing the new stamp.*/
  ch.vtlist.laststamp = stamp;

  return stamp;
}

/**
 * @brief   Resets and re-synchronizes the time stamps monotonic counter.
 *
 * @iclass
 */
void chVTResetTimeStampI(void) {

  chDbgCheckClassI();

  ch.vtlist.laststamp = (systimestamp_t)chVTGetSystemTimeX();
}

#endif /* CH_CFG_USE_TIMESTAMP == TRUE */

/** @} */

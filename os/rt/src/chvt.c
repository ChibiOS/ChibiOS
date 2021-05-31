/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

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
 * @brief   Delta list initialization.
 *
 * @param[out] dlp     pointer to the delta list header
 *
 * @notapi
 */
static inline void vt_init(delta_list_t *dlhp) {

  dlhp->next  = dlhp;
  dlhp->prev  = dlhp;
  dlhp->delta = (sysinterval_t)-1;
}

/**
 * @brief   List empty check.
 *
 * @param[in] dlhp      pointer to the delta list header
 *
 * @notapi
 */
static inline bool vt_is_empty(delta_list_t *dlhp) {

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
static inline bool vt_is_last(delta_list_t *dlhp, delta_list_t *dlp) {

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
static inline bool vt_is_first(delta_list_t *dlhp, delta_list_t *dlp) {

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
static inline bool vt_is_timer(delta_list_t *dlhp, delta_list_t *dlp) {

  return (bool)(dlp != dlhp);
}

/**
 * @brief   Inserts an element after another header element.
 *
 * @param[in] dlhp      pointer to the delta list header element
 * @param[in] dlp       element to be inserted after the header element
 *
 * @notapi
 */
static inline void vt_insert_after(delta_list_t *dlhp,
                                   delta_list_t *dlp,
                                   sysinterval_t delta) {

  dlp->delta      = delta;
  dlp->prev       = dlhp;
  dlp->next       = dlp->prev->next;
  dlp->next->prev = dlp;
  dlhp->next      = dlp;
}

/**
 * @brief   Inserts an element before another header element.
 *
 * @param[in] dlhp      pointer to the delta list header element
 * @param[in] dlp       element to be inserted before the header element
 *
 * @notapi
 */
static inline void vt_insert_before(delta_list_t *dlhp,
                                    delta_list_t *dlp,
                                    sysinterval_t delta) {

  dlp->delta      = delta;
  dlp->next       = dlhp;
  dlp->prev       = dlp->next->prev;
  dlp->prev->next = dlp;
  dlhp->prev      = dlp;
}

/**
 * @brief   Inserts an element in a delta list.
 *
 * @param[in] dlhp      pointer to the delta list header element
 * @param[in] dlep      element to be inserted before the header element
 * @param[in] delta     delta of the element to be inserted
 *
 * @notapi
 */
static inline void vt_insert(delta_list_t *dlhp,
                             delta_list_t *dlep,
                             sysinterval_t delta) {
  delta_list_t *dlp;

  /* The delta list is scanned in order to find the correct position for
     this element. */
  dlp = dlhp->next;
  while (dlp->delta < delta) {
    /* Debug assert if the element is already in the list.*/
    chDbgAssert(dlp != dlep, "element already in list");

    delta -= dlp->delta;
    dlp = dlp->next;
  }

  /* The timer is inserted in the delta list.*/
  vt_insert_before(dlp, dlep, delta);

  /* Adjusting delta for the following element.*/
  dlp->delta -= delta;

  /* Special case when the inserted element is in last position in the list,
     the value in the header must be restored, just doing it is faster than
     checking then doing.*/
  dlhp->delta = (sysinterval_t)-1;
}

/**
 * @brief   Dequeues an element from the delta list.
 *
 * @param[in] dlhp      pointer to the delta list header
 *
 * @notapi
 */
static inline delta_list_t *vt_remove_first(delta_list_t *dlhp) {
  delta_list_t *dlp = dlhp->next;

  dlhp->next       = dlp->next;
  dlhp->next->prev = dlhp;

  return dlp;
}

/**
 * @brief   Dequeues an element from the delta list.
 *
 * @param[in] dlp       pointer to the delta list element
 *
 * @notapi
 */
static inline delta_list_t *vt_dequeue(delta_list_t *dlp) {

  dlp->prev->next = dlp->next;
  dlp->next->prev = dlp->prev;

  return dlp;
}

/**
 * @brief   Enqueues a virtual timer in a virtual timers list.
 */
static void vt_enqueue(virtual_timers_list_t *vtlp,
                       virtual_timer_t *vtp,
                       systime_t now,
                       sysinterval_t delay) {
  sysinterval_t delta;

#if CH_CFG_ST_TIMEDELTA > 0
  {
    sysinterval_t nowdelta;

    /* Special case where the timers list is empty.*/
    if (vt_is_empty(&vtlp->dlist)) {

      /* The delta list is empty, the current time becomes the new
         delta list base time, the timer is inserted.*/
      vtlp->lasttime = now;
      vt_insert_after(&vtlp->dlist, &vtp->dlist, delay);

      /* If the requested delay is lower than the minimum safe delta then it
         is raised to the minimum safe value.*/
      if (delay < (sysinterval_t)CH_CFG_ST_TIMEDELTA) {
        /* We need to avoid that the system time goes past the alarm we are
           going to set before the alarm is actually set.*/
        delay = (sysinterval_t)CH_CFG_ST_TIMEDELTA;
      }
    #if CH_CFG_INTERVALS_SIZE > CH_CFG_ST_RESOLUTION
      else if (delay > (sysinterval_t)TIME_MAX_SYSTIME) {
        /* The delta could be too large for the physical timer to handle
           this can happen when: sizeof (systime_t) < sizeof (sysinterval_t).*/
        delay = (sysinterval_t)TIME_MAX_SYSTIME;
      }
    #endif

      /* Being the first element inserted in the list the alarm timer
         is started.*/
      port_timer_start_alarm(chTimeAddX(vtlp->lasttime, delay));

      return;
    }

    /* Delay as delta from 'lasttime'. Note, it can overflow and the value
       becomes lower than 'deltanow'.*/
    nowdelta = chTimeDiffX(vtlp->lasttime, now);
    delta    = nowdelta + delay;

    /* Scenario where a very large delay exceeded the numeric range, the
       delta is shortened to make it fit the numeric range, the timer
       will be triggered "deltanow" cycles earlier.*/
    if (delta < nowdelta) {
      delta = delay;
    }

    /* Checking if this timer would become the first in the delta list, this
       requires changing the current alarm setting.*/
    if (delta < vtlp->dlist.next->delta) {
      sysinterval_t deadline_delta;

      /* A small delay that will become the first element in the delta list
         and next deadline.*/
      deadline_delta = delta;
#if CH_CFG_INTERVALS_SIZE > CH_CFG_ST_RESOLUTION
      /* The delta could be too large for the physical timer to handle
         this can happen when: sizeof (systime_t) < sizeof (sysinterval_t).*/
      if (deadline_delta > (sysinterval_t)TIME_MAX_SYSTIME) {
        deadline_delta = (sysinterval_t)TIME_MAX_SYSTIME;
      }
#endif
      port_timer_set_alarm(chTimeAddX(vtlp->lasttime, deadline_delta));
    }
  }
#else /* CH_CFG_ST_TIMEDELTA == 0 */
  (void)now;

  /* Delta is initially equal to the specified delay.*/
  delta = delay;
#endif /* CH_CFG_ST_TIMEDELTA == 0 */

  vt_insert(&vtlp->dlist, &vtp->dlist, delta);
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Virtual Timers initialization.
 * @note    Internal use only.
 *
 * @param[out] vtlp     pointer to the @p virtual_timers_list_t structure
 *
 * @notapi
 */
void _vt_init(void) {

  vt_init(&ch.vtlist.dlist);
#if CH_CFG_ST_TIMEDELTA == 0
  ch.vtlist.systime = (systime_t)0;
#else /* CH_CFG_ST_TIMEDELTA > 0 */
  ch.vtlist.lasttime = (systime_t)0;
#endif /* CH_CFG_ST_TIMEDELTA > 0 */
}

/**
 * @brief   Enables a one-shot virtual timer.
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
  virtual_timers_list_t *vtlp = &ch.vtlist;

  chDbgCheckClassI();
  chDbgCheck((vtp != NULL) && (vtfunc != NULL) && (delay != TIME_IMMEDIATE));

  /* Timer initialization.*/
  vtp->par     = par;
  vtp->func    = vtfunc;

  /* Inserting the timer in the delta list.*/
  vt_enqueue(vtlp, vtp, chVTGetSystemTimeX(), delay);
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
  virtual_timers_list_t *vtlp = &ch.vtlist;

  chDbgCheckClassI();
  chDbgCheck(vtp != NULL);
  chDbgAssert(chVTIsArmedI(vtp), "timer not armed");

#if CH_CFG_ST_TIMEDELTA == 0

  /* The delta of the timer is added to the next timer.*/
  vtp->dlist.next->delta += vtp->dlist.delta;

 /* Removing the element from the delta list, marking it as not armed.*/
  (void) vt_dequeue(&vtp->dlist);
  vtp->func = NULL;

  /* The above code changes the value in the header when the removed element
     is the last of the list, restoring it.*/
  vtlp->dlist.delta = (sysinterval_t)-1;
#else /* CH_CFG_ST_TIMEDELTA > 0 */
  sysinterval_t nowdelta, delta;

  /* If the timer is not the first of the list then it is simply unlinked
     else the operation is more complex.*/
  if (!vt_is_first(&vtlp->dlist, &vtp->dlist)) {

    /* Removing the element from the delta list and marking it as not armed.*/
    (void) vt_dequeue(&vtp->dlist);
    vtp->func = NULL;

    /* Adding delta to the next element, if it is not the last one.*/
    vtp->dlist.next->delta += vtp->dlist.delta;

    /* Special case when the removed element from the last position in the list,
       the value in the header must be restored, just doing it is faster than
       checking then doing.*/
    vtlp->dlist.delta = (sysinterval_t)-1;

    return;
  }

  /* Removing the first timer from the list and marking it as not armed.*/
  vt_remove_first(&vtlp->dlist);
  vtp->func = NULL;

  /* If the list become empty then the alarm timer is stopped and done.*/
  if (vt_is_empty(&vtlp->dlist)) {
    port_timer_stop_alarm();

    return;
  }

  /* The delta of the removed timer is added to the new first timer.*/
  vtlp->dlist.next->delta += vtp->dlist.delta;

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
 * @brief   Returns the remaining time interval before next timer trigger.
 * @note    This function can be called while the timer is active or
 *          after stopping it.
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 * @return              The remaining time interval.
 *
 * @iclass
 */
sysinterval_t chVTGetRemainingIntervalI(virtual_timer_t *vtp) {
  virtual_timers_list_t *vtlp = &ch.vtlist;
  sysinterval_t delta;
  delta_list_t *dlp;

  chDbgCheckClassI();

  delta = (sysinterval_t)0;
  dlp = vtlp->dlist.next;
  do {
    delta += dlp->delta;
    if (dlp == &vtp->dlist) {
#if CH_CFG_ST_TIMEDELTA > 0
      systime_t now = chVTGetSystemTimeX();
      sysinterval_t nowdelta = chTimeDiffX(vtlp->lasttime, now);
      if (nowdelta > delta) {
        return (sysinterval_t)0;
      }
      return nowdelta - delta;
#else
      return delta;
#endif
    }
    dlp = dlp->next;
  } while (dlp != &vtlp->dlist);

  chDbgAssert(false, "timer not in list");

  return (sysinterval_t)-1;
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
  virtual_timers_list_t *vtlp = &ch.vtlist;

  chDbgCheckClassI();

#if CH_CFG_ST_TIMEDELTA == 0
  vtlp->systime++;
  if (!vt_is_empty(&vtlp->dlist)) {
    /* The list is not empty, processing elements on top.*/
    --vtlp->dlist.next->delta;
    while (vtlp->dlist.next->delta == (sysinterval_t)0) {
      virtual_timer_t *vtp;
      vtfunc_t fn;

      /* Triggered timer.*/
      vtp = (virtual_timer_t *)vtlp->dlist.next;

      /* Removing the element from the delta list, marking it as not armed.*/
      (void) vt_dequeue(&vtp->dlist);
      fn = vtp->func;
      vtp->func = NULL;

      /* The callback is invoked outside the kernel critical section, it
         is re-entered on the callback return.*/
      chSysUnlockFromISR();
      fn(vtp->par);
      chSysLockFromISR();
    }
  }
#else /* CH_CFG_ST_TIMEDELTA > 0 */
  virtual_timer_t *vtp;
  sysinterval_t delta, nowdelta;
  systime_t now;

  /* Looping through timers consuming all timers with deltas lower or equal
     than the interval between "now" and "lasttime".*/
  while (true) {
    vtfunc_t fn;

    /* First timer in the delta list.*/
    vtp = (virtual_timer_t *)vtlp->dlist.next;

    /* Delta between current time and last execution time.*/
    now = chVTGetSystemTimeX();
    nowdelta = chTimeDiffX(vtlp->lasttime, now);

    /* Loop break condition.
       Note that the list scan is limited by the delta list header having
       "vtlp->dlist.delta == (sysinterval_t)-1" which is greater than all
       deltas*/
    if (nowdelta < vtp->dlist.delta) {
      break;
    }

    /* Last time deadline is updated to the next timer's time.*/
    vtlp->lasttime = chTimeAddX(vtlp->lasttime, vtp->dlist.delta);

    /* Removing the timer from the list.*/
    (void) vt_dequeue(&vtp->dlist);

    /* Marking the timer as not armed.*/
    fn = vtp->func;
    vtp->func = NULL;

    /* If the list becomes empty then the alarm is disabled.*/
    if (vt_is_empty(&vtlp->dlist)) {
      port_timer_stop_alarm();
    }

    /* The callback is invoked outside the kernel critical section, it
       is re-entered on the callback return. Note that "lasttime" can be
       modified within the callback if some timer function is called.*/
    chSysUnlockFromISR();
    fn(vtp->par);
    chSysLockFromISR();
  }

  /* If the list is empty, nothing else to do.*/
  if (vt_is_empty(&vtlp->dlist)) {
    return;
  }

  /* Calculating the delta to the next alarm time.*/
  delta = vtp->dlist.delta - nowdelta;

  /* Limit delta to CH_CFG_ST_TIMEDELTA.*/
  if (delta < (sysinterval_t)CH_CFG_ST_TIMEDELTA) {
    delta = (sysinterval_t)CH_CFG_ST_TIMEDELTA;
  }
#if CH_CFG_INTERVALS_SIZE > CH_CFG_ST_RESOLUTION
  /* The delta could be too large for the physical timer to handle.*/
  else if (delta > (sysinterval_t)TIME_MAX_SYSTIME) {
    delta = (sysinterval_t)TIME_MAX_SYSTIME;
  }
#endif

  /* Update alarm time to next timer.*/
  port_timer_set_alarm(chTimeAddX(now, delta));

  chDbgAssert(chTimeDiffX(vtlp->lasttime, chVTGetSystemTimeX()) <=
              chTimeDiffX(vtlp->lasttime, chTimeAddX(now, delta)),
              "insufficient delta");
#endif /* CH_CFG_ST_TIMEDELTA > 0 */
}

/** @} */

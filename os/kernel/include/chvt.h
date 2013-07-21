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
 * @file    chvt.h
 * @brief   Time and Virtual Timers module macros and structures.
 *
 * @addtogroup time
 * @{
 */

#ifndef _CHVT_H_
#define _CHVT_H_

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
 * @brief   Virtual Timer callback function.
 */
typedef void (*vtfunc_t)(void *);

/**
 * @brief   Virtual Timer structure type.
 */
typedef struct virtual_timer virtual_timer_t;

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
 * @extends virtual_timers_list_t
 *
 * @brief   Virtual Timer descriptor structure.
 */
struct virtual_timer {
  virtual_timer_t       *vt_next;   /**< @brief Next timer in the list.     */
  virtual_timer_t       *vt_prev;   /**< @brief Previous timer in the list. */
  systime_t             vt_delta;   /**< @brief Time delta before timeout.  */
  vtfunc_t              vt_func;    /**< @brief Timer callback function
                                                pointer.                    */
  void                  *vt_par;    /**< @brief Timer callback function
                                                parameter.                  */
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Time conversion utilities
 * @{
 */
/**
 * @brief   Seconds to system ticks.
 * @details Converts from seconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] sec       number of seconds
 * @return              The number of ticks.
 *
 * @api
 */
#define S2ST(sec)                                                           \
  ((systime_t)((uint32_t)(sec) * (uint32_t)CH_CFG_FREQUENCY))

/**
 * @brief   Milliseconds to system ticks.
 * @details Converts from milliseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] msec      number of milliseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define MS2ST(msec)                                                         \
  ((systime_t)(((((uint32_t)(msec)) * ((uint32_t)CH_CFG_FREQUENCY) - 1UL) /     \
                1000UL) + 1UL))

/**
 * @brief   Microseconds to system ticks.
 * @details Converts from microseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] usec      number of microseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define US2ST(usec)                                                         \
  ((systime_t)(((((uint32_t)(usec)) * ((uint32_t)CH_CFG_FREQUENCY) - 1UL) /     \
                1000000UL) + 1UL))
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern virtual_timers_list_t vtlist;

/*
 * Virtual Timers APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif
  void _vt_init(void);
  bool chVTIsTimeWithin(systime_t time, systime_t start, systime_t end);
  void chVTDoSetI(virtual_timer_t *vtp, systime_t delay,
                  vtfunc_t vtfunc, void *par);
  void chVTDoResetI(virtual_timer_t *vtp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Initializes a @p virtual_timer_t object.
 * @note    Initializing a timer object is not strictly required because
 *          the function @p chVTSetI() initializes the object too. This
 *          function is only useful if you need to perform a @p chVTIsArmed()
 *          check before calling @p chVTSetI().
 *
 * @param[out] vtp      the @p virtual_timer_t structure pointer
 *
 * @init
 */
static inline void chVTObjectInit(virtual_timer_t *vtp) {

  vtp->vt_func = NULL;
}

/**
 * @brief   Current system time.
 * @details Returns the number of system ticks since the @p chSysInit()
 *          invocation.
 * @note    The counter can reach its maximum and then restart from zero.
 *
 * @return              The system time in ticks.
 *
 * @iclass
 */
static inline systime_t chVTGetSystemTimeI(void) {

  chDbgCheckClassI();

#if CH_CFG_TIMEDELTA == 0
  return vtlist.vt_systime;
#else /* CH_CFG_TIMEDELTA > 0 */
  return port_timer_get_time();
#endif /* CH_CFG_TIMEDELTA > 0 */
}

/**
 * @brief   Current system time.
 * @details Returns the number of system ticks since the @p chSysInit()
 *          invocation.
 * @note    The counter can reach its maximum and then restart from zero.
 *
 * @return              The system time in ticks.
 *
 * @api
 */
static inline systime_t chVTGetSystemTime(void) {
  systime_t systime;

  chSysLock();
  systime = chVTGetSystemTimeI();
  chSysUnlock();
  return systime;
}

/**
 * @brief   Checks if the current system time is within the specified time
 *          window.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 *
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @api
 */
static inline bool chVTIsSystemTimeWithinI(systime_t start, systime_t end) {

  return chVTIsTimeWithin(chVTGetSystemTimeI(), start, end);
}

/**
 * @brief   Checks if the current system time is within the specified time
 *          window.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 *
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @api
 */
static inline bool chVTIsSystemTimeWithin(systime_t start, systime_t end) {

  return chVTIsTimeWithin(chVTGetSystemTime(), start, end);
}

/**
 * @brief   Returns @p true if the specified timer is armed.
 * @pre     The timer must have been initialized using @p chVTObjectInit()
 *          or @p chVTSetI() (or @p chVTSetI() variants).
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 * @return              true if the timer is armed.
 *
 * @iclass
 */
static inline bool chVTIsArmedI(virtual_timer_t *vtp) {

  chDbgCheckClassI();

  return (bool)(vtp->vt_func != NULL);
}

/**
 * @brief   Disables a Virtual Timer.
 * @note    The timer is first checked and disabled only if armed.
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 *
 * @iclass
 */
static inline void chVTResetI(virtual_timer_t *vtp) {

  if (chVTIsArmedI(vtp))
    chVTDoResetI(vtp);
}

/**
 * @brief   Disables a Virtual Timer.
 * @note    The timer is first checked and disabled only if armed.
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 *
 * @api
 */
static inline void chVTReset(virtual_timer_t *vtp) {

  chSysLock();
  chVTResetI(vtp);
  chSysUnlock();
}

/**
 * @brief   Enables a virtual timer.
 * @details If the virtual timer was already enabled then it is re-enabled
 *          using the new parameters.
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 * @param[in] delay     the number of ticks before the operation timeouts.
 * @param[in] vtfunc    the timer callback function. After invoking the
 *                      callback the timer is disabled and the structure can
 *                      be disposed or reused.
 * @param[in] par       a parameter that will be passed to the callback
 *                      function
 *
 * @iclass
 */
static inline void chVTSetI(virtual_timer_t *vtp, systime_t delay,
                            vtfunc_t vtfunc, void *par) {

  chVTResetI(vtp);
  chVTDoSetI(vtp, delay, vtfunc, par);
}

/**
 * @brief   Enables a virtual timer.
 * @details If the virtual timer was already enabled then it is re-enabled
 *          using the new parameters.
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 * @param[in] delay     the number of ticks before the operation timeouts.
 * @param[in] vtfunc    the timer callback function. After invoking the
 *                      callback the timer is disabled and the structure can
 *                      be disposed or reused.
 * @param[in] par       a parameter that will be passed to the callback
 *                      function
 *
 * @api
 */
static inline void chVTSet(virtual_timer_t *vtp, systime_t delay,
                           vtfunc_t vtfunc, void *par) {

  chSysLock();
  chVTSetI(vtp, delay, vtfunc, par);
  chSysUnlock();
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
static inline void chVTDoTickI(void) {

  chDbgCheckClassI();

#if CH_CFG_TIMEDELTA == 0
  vtlist.vt_systime++;
  if (&vtlist != (virtual_timers_list_t *)vtlist.vt_next) {
    virtual_timer_t *vtp;

    --vtlist.vt_next->vt_delta;
    while (!(vtp = vtlist.vt_next)->vt_delta) {
      vtfunc_t fn = vtp->vt_func;
      vtp->vt_func = (vtfunc_t)NULL;
      vtp->vt_next->vt_prev = (void *)&vtlist;
      vtlist.vt_next = vtp->vt_next;
      chSysUnlockFromIsr();
      fn(vtp->vt_par);
      chSysLockFromIsr();
    }
  }
#else /* CH_CFG_TIMEDELTA > 0 */
  if (&vtlist != (virtual_timers_list_t *)vtlist.vt_next) {
    virtual_timer_t *vtp;

    --vtlist.vt_next->vt_delta;
    while (!(vtp = vtlist.vt_next)->vt_delta) {
      vtfunc_t fn = vtp->vt_func;
      vtp->vt_func = (vtfunc_t)NULL;
      vtp->vt_next->vt_prev = (void *)&vtlist;
      vtlist.vt_next = vtp->vt_next;
      chSysUnlockFromIsr();
      fn(vtp->vt_par);
      chSysLockFromIsr();
    }
  }
#endif /* CH_CFG_TIMEDELTA > 0 */
}

#endif /* _CHVT_H_ */

/** @} */

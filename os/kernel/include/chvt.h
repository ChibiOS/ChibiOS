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
typedef struct VirtualTimer VirtualTimer;

/**
 * @extends VTList
 *
 * @brief   Virtual Timer descriptor structure.
 */
struct VirtualTimer {
  VirtualTimer          *vt_next;   /**< @brief Next timer in the list.     */
  VirtualTimer          *vt_prev;   /**< @brief Previous timer in the list. */
  systime_t             vt_time;    /**< @brief Absolute time.              */
  vtfunc_t              vt_func;    /**< @brief Timer callback function
                                                pointer.                    */
  void                  *vt_par;    /**< @brief Timer callback function
                                                parameter.                  */
};

/**
 * @brief   Virtual timers list header.
 * @note    The timers list is implemented as a double link bidirectional list
 *          in order to make the unlink time constant, the reset of a virtual
 *          timer is often used in the code.
 */
typedef struct {
  VirtualTimer          *vt_next;   /**< @brief Next timer in the list.     */
  VirtualTimer          *vt_prev;   /**< @brief Last timer in the  list.    */
  volatile systime_t    vt_time;    /**< @brief Current system time.        */
} VTList;

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
  ((systime_t)((sec) * CH_FREQUENCY))

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
  ((systime_t)((((msec) * CH_FREQUENCY - 1L) / 1000L) + 1L))

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
  ((systime_t)((((usec) * CH_FREQUENCY - 1L) / 1000000L) + 1L))
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern VTList vtlist;

/*
 * Virtual Timers APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif
  void _vt_init(void);
  bool_t chVTIsSystemTimeWithin(systime_t start, systime_t end);
  void chVTSetAbsoluteI(VirtualTimer *vtp, systime_t time,
                        vtfunc_t vtfunc, void *par);
  void chVTResetI(VirtualTimer *vtp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

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

  return vtlist.vt_time;
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

  chSysLock()
  systime = chVTGetSystemTimeI();
  chSysUnlock();
  return systime;
}

/**
 * @brief   Initializes a @p VirtualTimer object.
 * @note    Initializing a timer object is not strictly required because
 *          the function @p chVTSetI() initializes the object too. This
 *          function is only useful if you need to perform a @p chVTIsArmed()
 *          check before calling @p chVTSetI().
 *
 * @param[out] vtp      the @p VirtualTimer structure pointer
 *
 * @init
 */
static inline void chVTObjectInit(VirtualTimer *vtp) {

  vtp->vt_func = NULL;
}

/**
 * @brief   Returns @p TRUE if the specified timer is armed.
 * @pre     The timer must have been initialized using @p chVTObjectInit()
 *          or @p chVTSetI() (or @p chVTSetI() variants).
 *
 * @param[in] vtp       the @p VirtualTimer structure pointer
 * @return              true if the timer is armed.
 *
 * @iclass
 */
static inline bool_t chVTIsArmedI(VirtualTimer *vtp) {

  chDbgCheckClassI();

  return (bool_t)(vtp->vt_func != NULL);
}

/**
 * @brief   Enables a virtual timer.
 *
 * @param[out] vtp      the @p VirtualTimer structure pointer
 * @param[in] delay     the number of ticks before the operation timeouts.
 * @param[in] vtfunc    the timer callback function. After invoking the
 *                      callback the timer is disabled and the structure can
 *                      be disposed or reused.
 * @param[in] par       a parameter that will be passed to the callback
 *                      function
 *
 * @iclass
 */
static inline void chVTSetI(VirtualTimer *vtp, systime_t delay,
                            vtfunc_t vtfunc, void *par) {

  chVTSetAbsoluteI(vtp, chVTGetSystemTimeI() + delay, vtfunc, par);
}

/**
 * @brief   Enables a virtual timer.
 *
 * @param[out] vtp      the @p VirtualTimer structure pointer
 * @param[in] delay     the number of ticks before the operation timeouts.
 * @param[in] vtfunc    the timer callback function. After invoking the
 *                      callback the timer is disabled and the structure can
 *                      be disposed or reused.
 * @param[in] par       a parameter that will be passed to the callback
 *                      function
 *
 * @api
 */
static inline void chVTSet(VirtualTimer *vtp, systime_t delay,
                           vtfunc_t vtfunc, void *par) {

  chSysLock();
  chVTSetI(vtp, delay, vtfunc, par);
  chSysUnlock();
}

/**
 * @brief   Disables a Virtual Timer.
 * @pre     The timer must be in armed state before calling this function.
 *
 * @param[in] vtp       the @p VirtualTimer structure pointer
 *
 * @iclass
 */
static inline void chVTDoResetI(VirtualTimer *vtp) {

  chDbgCheckClassI();
  chDbgCheck(vtp != NULL, "chVTDoResetI");

  vtp->vt_prev->vt_next = vtp->vt_next;
  vtp->vt_next->vt_prev = vtp->vt_prev;
  vtp->vt_func = (vtfunc_t)NULL;
}

/**
 * @brief   Disables a Virtual Timer.
 * @note    The timer is first checked and disabled only if armed.
 *
 * @param[in] vtp       the @p VirtualTimer structure pointer
 *
 * @api
 */
static inline void chVTReset(VirtualTimer *vtp) {

  chSysLock();
  chVTResetI(vtp);
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
  systime_t systime = ++vtlist.vt_time;

  chDbgCheckClassI();

  if (&vtlist != (VTList *)vtlist.vt_next) {
    VirtualTimer *vtp;

    while (((VirtualTimer *)&vtlist != (vtp = vtlist.vt_next)) &&
           (vtp->vt_time == systime)) {
      vtfunc_t fn = vtp->vt_func;
      chVTDoResetI(vtp);
      chSysUnlockFromIsr();
      fn(vtp->vt_par);
      chSysLockFromIsr();
    }
  }
}

#endif /* _CHVT_H_ */

/** @} */

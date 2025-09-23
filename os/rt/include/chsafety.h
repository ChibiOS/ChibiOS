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
 * @file    rt/include/chsafety.h
 * @brief   Functional safety support macros and structures.
 *
 * @addtogroup rt_safety
 * @{
 */

#ifndef CHSAFETY_H
#define CHSAFETY_H

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

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Safety-related condition assertion.
 * @details If the condition check fails then the kernel panics with a
 *          message and halts.
 * @note    The condition is tested only if the specified check level is
 *          equal or greater to @p CH_CFG_HARDENING_LEVEL setting specified
 *          in @p chconf.h else the macro does nothing.
 * @note    Safety checks with levels from 0 to 2 are also activated when
 *          @p CH_DBG_ENABLE_ASSERTS is set to @p TRUE.
 * @note    The remark string is not currently used except for putting a
 *          comment in the code about the assertion.
 *
 * @param[in] l         check level in range 0..3
 * @param[in] c         the condition to be verified to be true
 * @param[in] r         a remark string
 *
 * @api
 */
#if !defined(chSftAssert)
#define chSftAssert(l, c, r) do {                                           \
  /*lint -save -e506 -e774 [2.1, 14.3] Can be a constant by design.*/       \
  if (((l) >= CH_CFG_HARDENING_LEVEL) ||                                    \
      (((l) <= 2) && (CH_DBG_ENABLE_ASSERTS != FALSE))) {                   \
    if (unlikely(!(c))) {                                                   \
  /*lint -restore*/                                                         \
      CH_CFG_SAFETY_ASSERT_HOOK(l, __func__);                               \
      chSysHalt(__func__);                                                  \
    }                                                                       \
  }                                                                         \
} while (false)
#endif /* !defined(chSftAssert) */
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#if CH_DBG_SYSTEM_STATE_CHECK == TRUE
  void __dbg_check_disable(void);
  void __dbg_check_suspend(void);
  void __dbg_check_enable(void);
  void __dbg_check_lock(void);
  void __dbg_check_unlock(void);
  void __dbg_check_lock_from_isr(void);
  void __dbg_check_unlock_from_isr(void);
  void __dbg_check_enter_isr(void);
  void __dbg_check_leave_isr(void);
  void chDbgCheckClassI(void);
  void chDbgCheckClassS(void);
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Debug support initialization.
 * @note    Internal use only.
 *
 * @param[out] sdp      pointer to a @p system_debug_t object
 *
 * @notapi
 */
static inline void __dbg_object_init(system_debug_t *sdp) {

  sdp->panic_msg = NULL;

#if CH_DBG_SYSTEM_STATE_CHECK == TRUE
  /* The initial state is assumed to be within a critical zone.*/
  sdp->isr_cnt  = (cnt_t)0;
  sdp->lock_cnt = (cnt_t)1;
#endif
}

#endif /* CHSAFETY_H */

/** @} */

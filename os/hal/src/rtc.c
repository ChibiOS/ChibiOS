/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
 * @file    rtc.c
 * @brief   Real Time Clock Abstraction Layer code.
 *
 * @addtogroup RTC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_RTC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/
/**
 * @brief   Enable access to registers and initialize RTC if BKP doamin
 *          was previously reseted.
 *
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 */
void rtcInit(void){
  rtc_lld_init();
}

/**
 * @brief     Configure and start interrupt servicing routines.
 *            This function do nothing if callbacks disabled.
 *
 * @param[in] rtcp - pointer to RTC driver structure.
 * @param[in] rtccfgp - pointer to RTC config structure.
 */
#if RTC_SUPPORTS_CALLBACKS
void rtcStartI(RTCDriver *rtcp, const RTCConfig *rtccfgp){
  chDbgCheckClassI();
  chDbgCheck(((rtcp != NULL) && (rtccfgp != NULL)), "rtcStart");
  rtc_lld_start(rtcp, rtccfgp);
}

/**
 * @brief   Stop interrupt servicing routines.
 */
void rtcStopI(void){
  chDbgCheckClassI();
  rtc_lld_stop();
}
#endif /* RTC_SUPPORTS_CALLBACKS */

/**
 * @brief     Set current time.
 * @param[in] tv_sec - time value in UNIX notation.
 */
void rtcSetTime(uint32_t tv_sec){
  rtc_lld_set_time(tv_sec);
}

/**
 * @brief Return current time in UNIX notation.
 */
inline uint32_t rtcGetSec(void){
  return rtc_lld_get_sec();
}

/**
 * @brief Return fractional part of current time (milliseconds).
 */
inline uint16_t rtcGetMsec(void){
  return rtc_lld_get_msec();
}

/**
 * @brief Set alarm date in UNIX notation.
 */
void rtcSetAlarm(uint32_t tv_alarm){
  rtc_lld_set_alarm(tv_alarm);
}

/**
 * @brief Get current alarm date in UNIX notation.
 */
inline uint32_t rtcGetAlarm(void){
  return rtc_lld_get_alarm();
}

#endif /* HAL_USE_RTC */

/** @} */



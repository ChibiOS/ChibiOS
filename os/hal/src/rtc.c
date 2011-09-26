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
 * @brief   RTC Driver code.
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
 * @brief   Enable access to registers and initialize RTC if BKP domain
 *          was previously reset.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void rtcInit(void) {
  rtc_lld_init();
}

#if RTC_SUPPORTS_CALLBACKS || defined(__DOXYGEN__)
/**
 * @brief   Enables or disables callbacks.
 * @details This function enables or disables callbacks, use a @p NULL pointer
 *          in order to disable a callback.
 * @pre     To use this function you must set @p RTC_SUPPORTS_CALLBACKS
 *          to @p TRUE.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] overflowcb overflow callback function
 * @param[in] secondcb  every second callback function
 * @param[in] alarmcb   alarm callback function
 */
void rtcSetCallback(RTCDriver *rtcp, rtccb_t overflowcb,
                    rtccb_t secondcb, rtccb_t alarmcb) {

  chDbgCheck((rtcp != NULL), "rtcSetCallback");

  rtc_lld_set_callback(rtcp, overflowcb, secondcb, alarmcb);
}
#endif /* RTC_SUPPORTS_CALLBACKS */

/**
 * @brief   Set current time.
 *
 * @param[in] timespec  pointer to a @p RTCDateTime structure
 */
void rtcSetTime(RTCDateTime *timespec) {

  chDbgCheck((timespec != NULL), "rtcSetTime");

  rtc_lld_set_time(timespec);
}

/**
 * @brief   Get current time.
 *
 * @param[in] timespec  pointer to a @p RTCDateTime structure
 */
void rtcGetTime(RTCDateTime *timespec) {

  chDbgCheck((timespec != NULL), "rtcGetTime");
  rtc_lld_get_time(timespec);
}

/**
 * @brief   Set alarm time.
 *
 * @param[in] timespec  pointer to a @p RTCDateTime structure
 */
void rtcSetAlarm(RTCDateTime *timespec) {

  chDbgCheck((timespec != NULL), "rtcSetAlarm");

  rtc_lld_set_alarm(timespec);
}

/**
 * @brief   Get current alarm.
 *
 * @param[in] timespec  pointer to a @p RTCDateTime structure
 */
void rtcGetAlarm(RTCDateTime *timespec){

  chDbgCheck((timespec != NULL), "rtcGetAlarm");

  rtc_lld_get_alarm(timespec);
}

#endif /* HAL_USE_RTC */

/** @} */



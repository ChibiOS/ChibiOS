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
 * @brief   RTC Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void rtcInit(void) {

  rtc_lld_init();
}

/**
 * @brief   Set current time.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] timespec  pointer to a @p RTCTime structure
 *
 * @api
 */
void rtcSetTime(RTCDriver *rtcp, const RTCTime *timespec) {

  chDbgCheck((rtcp != NULL) && (timespec != NULL), "rtcSetTime");

  rtc_lld_set_time(rtcp, timespec);
}

/**
 * @brief   Get current time.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[out] timespec pointer to a @p RTCTime structure
 *
 * @api
 */
void rtcGetTime(RTCDriver *rtcp, RTCTime *timespec) {

  chDbgCheck((rtcp != NULL) && (timespec != NULL), "rtcGetTime");

  rtc_lld_get_time(rtcp, timespec);
}

#if (RTC_ALARMS > 0) || defined(__DOXYGEN__)
/**
 * @brief   Set alarm time.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] alarm     alarm identifier
 * @param[in] alarmspec pointer to a @p RTCAlarm structure or @p NULL
 *
 * @api
 */
void rtcSetAlarm(RTCDriver *rtcp,
                 rtcalarm_t alarm,
                 const RTCAlarm *alarmspec) {

  chDbgCheck((rtcp != NULL) && (alarm < RTC_ALARMS), "rtcSetAlarm");

  rtc_lld_set_alarm(rtcp, alarm, alarmspec);
}

/**
 * @brief   Get current alarm.
 * @note    If an alarm has not been set then the returned alarm specification
 *          is not meaningful.
 *
 * @param[in] rtcp       pointer to RTC driver structure
 * @param[in] alarm      alarm identifier
 * @param[out] alarmspec pointer to a @p RTCAlarm structure
 *
 * @api
 */
void rtcGetAlarm(RTCDriver *rtcp,
                 rtcalarm_t alarm,
                 RTCAlarm *alarmspec) {

  chDbgCheck((rtcp != NULL) && (alarm < RTC_ALARMS) && (alarmspec != NULL),
             "rtcGetAlarm");

  rtc_lld_get_alarm(rtcp, alarm, alarmspec);
}
#endif /* RTC_ALARMS > 0 */



/**
 * @brief   Set periodic wakeup period.
 *
 * @param[in] rtcp       pointer to RTC driver structure
 * @param[in] wakeupspec pointer to a @p RTCWakeup structure
 *
 * @api
 */
void rtcSetWakeup(RTCDriver *rtcp, RTCWakeup *wakeupspec) {

  chDbgCheck((rtcp != NULL), "rtcGetAlarm");
  rtc_lld_set_periodic_wakeup(rtcp, wakeupspec);
}

/**
 * @brief   Get periodic wakeup period.
 *
 * @param[in] rtcp        pointer to RTC driver structure
 * @param[out] wakeupspec pointer to a @p RTCWakeup structure
 *
 * @api
 */
void rtcGetWakeup(RTCDriver *rtcp, RTCWakeup *wakeupspec) {

  chDbgCheck((rtcp != NULL), "rtcGetAlarm");
  rtc_lld_get_periodic_wakeup(rtcp, wakeupspec);
}

#if RTC_SUPPORTS_CALLBACKS || defined(__DOXYGEN__)
/**
 * @brief   Enables or disables RTC callbacks.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] cb_cfg    callback configuration struct
 *
 * @api
 */
void rtcSetCallback(RTCDriver *rtcp, RTCCallbackConfig *cb_cfg) {

  chDbgCheck(((rtcp != NULL) && (cb_cfg != NULL)), "rtcSetCallback");

  rtc_lld_set_callback(rtcp, cb_cfg);
}
#endif /* RTC_SUPPORTS_CALLBACKS */

#endif /* HAL_USE_RTC */

/** @} */

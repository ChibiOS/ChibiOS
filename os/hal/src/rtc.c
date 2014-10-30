/*
    ChibiOS/HAL - Copyright (C) 2006,2007,2008,2009,2010,
                  2011,2012,2013,2014 Giovanni Di Sirio.

    This file is part of ChibiOS/HAL 

    ChibiOS/HAL is free software; you can redistribute it and/or modify
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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    rtc.c
 * @brief   RTC Driver code.
 *
 * @addtogroup RTC
 * @{
 */

#include "hal.h"

#if HAL_USE_RTC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
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
 * @note    This function can be called from any context but limitations
 *          could be imposed by the low level implementation. It is
 *          guaranteed that the function can be called from thread
 *          context.
 * @note    The function can be reentrant or not reentrant depending on
 *          the low level implementation.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] timespec  pointer to a @p RTCDateTime structure
 *
 * @special
 */
void rtcSetTime(RTCDriver *rtcp, const RTCDateTime *timespec) {

  osalDbgCheck((rtcp != NULL) && (timespec != NULL));

  rtc_lld_set_time(rtcp, timespec);
}

/**
 * @brief   Get current time.
 * @note    This function can be called from any context but limitations
 *          could be imposed by the low level implementation. It is
 *          guaranteed that the function can be called from thread
 *          context.
 * @note    The function can be reentrant or not reentrant depending on
 *          the low level implementation.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[out] timespec pointer to a @p RTCDateTime structure
 *
 * @special
 */
void rtcGetTime(RTCDriver *rtcp, RTCDateTime *timespec) {

  osalDbgCheck((rtcp != NULL) && (timespec != NULL));

  rtc_lld_get_time(rtcp, timespec);
}

#if (RTC_ALARMS > 0) || defined(__DOXYGEN__)
/**
 * @brief   Set alarm time.
 * @note    This function can be called from any context but limitations
 *          could be imposed by the low level implementation. It is
 *          guaranteed that the function can be called from thread
 *          context.
 * @note    The function can be reentrant or not reentrant depending on
 *          the low level implementation.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] alarm     alarm identifier
 * @param[in] alarmspec pointer to a @p RTCAlarm structure or @p NULL
 *
 * @special
 */
void rtcSetAlarm(RTCDriver *rtcp,
                 rtcalarm_t alarm,
                 const RTCAlarm *alarmspec) {

  osalDbgCheck((rtcp != NULL) && (alarm < RTC_ALARMS));

  rtc_lld_set_alarm(rtcp, alarm, alarmspec);
}

/**
 * @brief   Get current alarm.
 * @note    If an alarm has not been set then the returned alarm specification
 *          is not meaningful.
 * @note    This function can be called from any context but limitations
 *          could be imposed by the low level implementation. It is
 *          guaranteed that the function can be called from thread
 *          context.
 * @note    The function can be reentrant or not reentrant depending on
 *          the low level implementation.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] alarm     alarm identifier
 * @param[out] alarmspec pointer to a @p RTCAlarm structure
 *
 * @special
 */
void rtcGetAlarm(RTCDriver *rtcp,
                 rtcalarm_t alarm,
                 RTCAlarm *alarmspec) {

  osalDbgCheck((rtcp != NULL) && (alarm < RTC_ALARMS) && (alarmspec != NULL));

  rtc_lld_get_alarm(rtcp, alarm, alarmspec);
}
#endif /* RTC_ALARMS > 0 */

#if RTC_SUPPORTS_CALLBACKS || defined(__DOXYGEN__)
/**
 * @brief   Enables or disables RTC callbacks.
 * @details This function enables or disables the callback, use a @p NULL
 *          pointer in order to disable it.
 * @note    This function can be called from any context but limitations
 *          could be imposed by the low level implementation. It is
 *          guaranteed that the function can be called from thread
 *          context.
 * @note    The function can be reentrant or not reentrant depending on
 *          the low level implementation.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] callback  callback function pointer or @p NULL
 *
 * @special
 */
void rtcSetCallback(RTCDriver *rtcp, rtccb_t callback) {

  osalDbgCheck(rtcp != NULL);

  rtc_lld_set_callback(rtcp, callback);
}
#endif /* RTC_SUPPORTS_CALLBACKS */

/**
 * @brief   Get current time in format suitable for usage in FAT file system.
 *
 * @param[out] timespec pointer to a @p RTCDateTime structure
 * @return              FAT date/time value.
 *
 * @api
 */
uint32_t rtcConvertDateTimeToFAT(RTCDateTime *timespec) {

  osalDbgCheck(timespec != NULL);

  /* TODO: Implement.*/
  return 0;
}

#endif /* HAL_USE_RTC */

/** @} */

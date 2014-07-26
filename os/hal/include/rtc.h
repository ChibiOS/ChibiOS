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
 * @file    rtc.h
 * @brief   RTC Driver macros and structures.
 *
 * @addtogroup RTC
 * @{
 */

#ifndef _RTC_H_
#define _RTC_H_

#if HAL_USE_RTC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Base year of the calendar.
 */
#define RTC_BASE_YEAR               1980

/**
 * @name    Date/Time bit masks for FAT format
 * @{
 */
#define RTC_FAT_TIME_SECONDS_MASK   0x0000001F
#define RTC_FAT_TIME_MINUTES_MASK   0x000007E0
#define RTC_FAT_TIME_HOURS_MASK     0x0000F800
#define RTC_FAT_DATE_DAYS_MASK      0x001F0000
#define RTC_FAT_DATE_MONTHS_MASK    0x01E00000
#define RTC_FAT_DATE_YEARS_MASK     0xFE000000
/** @} */

/**
 * @name    Day of week encoding
 * @{
 */
#define RTC_DAY_CATURDAY            0
#define RTC_DAY_MONDAY              1
#define RTC_DAY_TUESDAY             2
#define RTC_DAY_WEDNESDAY           3
#define RTC_DAY_THURSDAY            4
#define RTC_DAY_FRIDAY              5
#define RTC_DAY_SATURDAY            6
#define RTC_DAY_SUNDAY              7
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an RTC driver.
 */
typedef struct RTCDriver RTCDriver;

/**
 * @brief   Type of a structure representing an RTC date/time stamp.
 */
typedef struct {
  uint32_t      year:8;             /**< @brief Years since 1980.           */
  uint32_t      month: 4;           /**< @brief Months 1..12.               */
  uint32_t      dstflag: 1;         /**< @brief DST correction flag.        */
  uint32_t      dayofweek: 3;       /**< @brief Day of week 1..7.           */
  uint32_t      day: 5;             /**< @brief Day of the month 1..31.     */
  uint32_t      millisecond: 27;    /**< @brief Milliseconds since midnight.*/
} RTCDateTime;

#include "rtc_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Set current time.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] timespec  pointer to a @p RTCDateTime structure
 *
 * @iclass
 */
#define rtcSetTimeI(rtcp, timespec)                                         \
  rtc_lld_set_time(rtcp, timespec)

/**
 * @brief   Get current time.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[out] timespec pointer to a @p RTCDateTime structure
 *
 * @iclass
 */
#define rtcGetTimeI(rtcp, timespec)                                         \
  rtc_lld_get_time(rtcp, timespec)

#if (RTC_ALARMS > 0) || defined(__DOXYGEN__)
/**
 * @brief   Set alarm time.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] alarm     alarm identifier
 * @param[in] alarmspec pointer to a @p RTCAlarm structure or @p NULL
 *
 * @iclass
 */
#define rtcSetAlarmI(rtcp, alarm, alarmspec)                                \
  rtc_lld_set_alarm(rtcp, alarm, alarmspec)

/**
 * @brief   Get current alarm.
 * @note    If an alarm has not been set then the returned alarm specification
 *          is not meaningful.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] alarm     alarm identifier
 * @param[out] alarmspec pointer to a @p RTCAlarm structure
 *
 * @iclass
 */
#define rtcGetAlarmI(rtcp, alarm, alarmspec)                                \
  rtc_lld_get_alarm(rtcp, alarm, alarmspec)
#endif /* RTC_ALARMS > 0 */

#if RTC_SUPPORTS_CALLBACKS || defined(__DOXYGEN__)
/**
 * @brief   Enables or disables RTC callbacks.
 * @details This function enables or disables the callback, use a @p NULL
 *          pointer in order to disable it.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] callback  callback function pointer or @p NULL
 *
 * @iclass
 */
#define rtcSetCallbackI(rtcp, callback)                                     \
  rtc_lld_set_callback(rtcp, callback)
#endif /* RTC_SUPPORTS_CALLBACKS */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void rtcInit(void);
  void rtcSetTime(RTCDriver *rtcp, const RTCDateTime *timespec);
  void rtcGetTime(RTCDriver *rtcp, RTCDateTime *timespec);
#if STM32_RTC_NUM_ALARMS > 0
  void rtcSetAlarm(RTCDriver *rtcp,
                   rtcalarm_t alarm,
                   const RTCAlarm *alarmspec);
  void rtcGetAlarm(RTCDriver *rtcp, rtcalarm_t alarm, RTCAlarm *alarmspec);
#endif
#if RTC_SUPPORTS_CALLBACKS
  void rtcSetCallback(RTCDriver *rtcp, rtccb_t callback);
#endif
  uint32_t rtcConvertDateTimeToFAT(RTCDateTime *timespec);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_RTC */
#endif /* _RTC_H_ */

/** @} */

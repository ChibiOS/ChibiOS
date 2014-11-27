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

/*
 * Lookup table with months' length
 */
static const uint8_t month_len[12] = {
  31, 30, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

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
 * @brief   Initializes a generic RTC driver object.
 * @details The HW dependent part of the initialization has to be performed
 *          outside, usually in the hardware initialization code.
 *
 * @param[out] rtcp     pointer to RTC driver structure
 *
 * @init
 */
void rtcObjectInit(RTCDriver *rtcp) {

#if RTC_HAS_STORAGE
  rtcp->vmt = &_rtc_lld_vmt;
#endif
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
 * @brief   Convert @p RTCDateTime to broken-down time structure.
 *
 * @param[in]  timespec   pointer to a @p RTCDateTime structure
 * @param[out] timp       pointer to a broken-down time structure
 *
 * @api
 */
void rtcConvertDateTimeToStructTm(const RTCDateTime *timespec,
                                  struct tm *timp) {
  uint32_t tmp;

  timp->tm_year  = timespec->year + (1980 - 1900);
  timp->tm_mon   = timespec->month - 1;
  timp->tm_mday  = timespec->day;
  timp->tm_isdst = timespec->dstflag;

  tmp = timespec->millisecond / 1000;
  timp->tm_sec  = tmp % 60;
  tmp -= timp->tm_sec;
  timp->tm_min  = (tmp % 3600) / 60;
  tmp -= timp->tm_min * 60;
  timp->tm_hour = tmp / 3600;
}

/**
 * @brief   Convert broken-down time structure to @p RTCDateTime.
 *
 * @param[in] timp        pointer to a broken-down time structure
 * @param[in] tv_msec     milliseconds value
 * @param[out] timespec   pointer to a @p RTCDateTime structure
 *
 * @api
 */
void rtcConvertStructTmToDateTime(const struct tm *timp,
                                  uint32_t tv_msec,
                                  RTCDateTime *timespec) {

  timespec->year      = timp->tm_year - (1980 - 1900);
  timespec->month     = timp->tm_mon + 1;
  timespec->day       = timp->tm_mday;
  timespec->dayofweek = timp->tm_wday + 1;
  if (-1 == timp->tm_isdst)
    timespec->dstflag = 0;  /* set zero if dst is unknown */
  else
    timespec->dstflag = timp->tm_isdst;
  timespec->millisecond = tv_msec +
      (timp->tm_hour * 3600 + timp->tm_min * 60 + timp->tm_sec) * 1000;
}

/**
 * @brief   Get current time in format suitable for usage in FAT file system.
 * @note    The information about day of week and DST is lost in DOS
 *          format, the second field loses its least significant bit.
 *
 * @param[out] timespec pointer to a @p RTCDateTime structure
 * @return              FAT date/time value.
 *
 * @api
 */
uint32_t rtcConvertDateTimeToFAT(const RTCDateTime *timespec) {
  uint32_t fattime;
  uint32_t sec, min, hour, day, month, tmp;

  tmp = timespec->millisecond / 1000;
  sec = tmp % 60;
  min = (tmp - sec) % 3600;
  hour = (tmp - sec - min * 60) / 3600;
  day = timespec->day;
  month = timespec->month;

  /* handle DST flag */
  if (1 == timespec->dstflag) {
    hour += 1;
    if (hour == 24) {
      hour = 0;
      day += 1;
      if (day > month_len[month - 1]) {
        day = 1;
        month += 1;
      }
    }
  }

  fattime  = sec            >> 1;
  fattime |= min            << 5;
  fattime |= hour           << 11;
  fattime |= day            << 16;
  fattime |= month          << 21;
  fattime |= timespec->year << 25;

  return fattime;
}

#endif /* HAL_USE_RTC */

/** @} */

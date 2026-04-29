/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file        hal_rtc.c
 * @brief       Generated RTC Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_RTC
 * @{
 */

#include "hal.h"

#if (HAL_USE_RTC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

#define RTC_SECONDS_PER_DAY                 86400LL

#define RTC_MILLISECONDS_PER_DAY            86400000U

/*===========================================================================*/
/* Module local macros.                                                      */
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
 * @brief       Checks if the specified year is a leap year.
 *
 * @param[in]     year          Year to be checked.
 * @return                      The leap-year check result.
 */
static bool rtc_is_leap_year(unsigned year) {
  return ((((year % 4U) == 0U) && ((year % 100U) != 0U)) ||
          ((year % 400U) == 0U));
}

/**
 * @brief       Returns the length of a month in days.
 *
 * @param[in]     year          Year.
 * @param[in]     month         Month in range 1..12.
 * @return                      The month length in days.
 */
static uint8_t rtc_get_month_length(unsigned year, unsigned month) {
  static const uint8_t month_len[12] = {
    31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U
  };
  uint8_t n;

  n = month_len[month - 1U];
  if ((month == 2U) && rtc_is_leap_year(year)) {
    n++;
  }

  return n;
}

/**
 * @brief       Checks the validity of a broken-down RTC time value.
 *
 * @param[in]     timespec      RTC time value to be checked.
 * @return                      The validity check result.
 */
static bool rtc_datetime_is_valid(const rtc_datetime_t *timespec) {
  unsigned year;

  if ((timespec->month < 1U) || (timespec->month > 12U)) {
    return false;
  }
  if ((timespec->day < 1U) ||
      (timespec->day > rtc_get_month_length((unsigned)timespec->year +
                                            RTC_BASE_YEAR,
                                            timespec->month))) {
    return false;
  }
  if (timespec->dayofweek == RTC_DAY_RESERVED) {
    return false;
  }
  if ((timespec->dayofweek > RTC_DAY_SUNDAY) ||
      (timespec->millisecond >= RTC_MILLISECONDS_PER_DAY)) {
    return false;
  }

  year = (unsigned)timespec->year + RTC_BASE_YEAR;
  (void)year;

  return true;
}

/**
 * @brief       Converts a civil date to days from the Unix epoch.
 *
 * @param[in]     year          Year.
 * @param[in]     month         Month.
 * @param[in]     day           Day.
 * @return                      Days from the Unix epoch.
 */
static int64_t rtc_days_from_civil(int64_t year, unsigned month, unsigned day) {
  int64_t era, yoe, doy, doe;

  year -= month <= 2U ? 1 : 0;
  era = (year >= 0) ? (year / 400) : ((year - 399) / 400);
  yoe = year - era * 400;
  doy = ((153 * ((int64_t)month + (month > 2U ? -3 : 9)) + 2) / 5) +
        (int64_t)day - 1;
  doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;

  return era * 146097 + doe - 719468;
}

/**
 * @brief       Converts days from the Unix epoch to a civil date.
 *
 * @param[in]     z             Days from the Unix epoch.
 * @param[out]    year          Converted year.
 * @param[out]    month         Converted month.
 * @param[out]    day           Converted day.
 */
static void rtc_civil_from_days(int64_t z, int64_t *year, unsigned *month,
                                unsigned *day) {
  int64_t era, doe, yoe, y, doy, mp;

  z += 719468;
  era = (z >= 0) ? (z / 146097) : ((z - 146096) / 146097);
  doe = z - era * 146097;
  yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
  y = yoe + era * 400;
  doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
  mp = (5 * doy + 2) / 153;

  *day = (unsigned)(doy - (153 * mp + 2) / 5 + 1);
  *month = (unsigned)(mp + (mp < 10 ? 3 : -9));
  *year = y + (*month <= 2U ? 1 : 0);
}

/**
 * @brief       Converts epoch days to RTC day-of-week encoding.
 *
 * @param[in]     days          Days from the Unix epoch.
 * @return                      The RTC day-of-week encoding.
 */
static uint8_t rtc_encode_day_of_week(int64_t days) {
  int64_t wday;

  wday = (days + 4) % 7;
  if (wday < 0) {
    wday += 7;
  }

  return wday == 0 ? RTC_DAY_SUNDAY : (uint8_t)wday;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       RTC Driver initialization.
 *
 * @init
 */
void rtcInit(void) {
  rtc_lld_init();
}

/**
 * @brief       Converts broken-down RTC time to a 64-bit UTC time value.
 *
 * @param[in]     src           Broken-down RTC time.
 * @param[out]    dst           Converted 64-bit UTC time.
 * @return                      The operation status.
 *
 * @api
 */
msg_t rtcConvertDateTimeToTime64(const rtc_datetime_t *src, rtc_time64_t *dst) {
  int64_t days, sec;

  osalDbgCheck((src != NULL) && (dst != NULL));

  if (!rtc_datetime_is_valid(src)) {
    return HAL_RET_CONFIG_ERROR;
  }

  days = rtc_days_from_civil((int64_t)src->year + RTC_BASE_YEAR,
                             src->month,
                             src->day);
  sec = days * RTC_SECONDS_PER_DAY + (int64_t)(src->millisecond / 1000U);

  dst->tv_sec = sec;
  dst->tv_nsec = (src->millisecond % 1000U) * 1000000U;

  return HAL_RET_SUCCESS;
}

/**
 * @brief       Converts a 64-bit UTC time value to broken-down RTC time.
 *
 * @param[in]     src           Source 64-bit UTC time.
 * @param[out]    dst           Converted broken-down RTC time.
 * @return                      The operation status.
 *
 * @api
 */
msg_t rtcConvertTime64ToDateTime(const rtc_time64_t *src, rtc_datetime_t *dst) {
  int64_t days, sod;
  int64_t year;
  unsigned month, day;

  osalDbgCheck((src != NULL) && (dst != NULL));

  if ((src->tv_sec < 315532800LL) || (src->tv_nsec >= 1000000000U)) {
    return HAL_RET_CONFIG_ERROR;
  }

  days = src->tv_sec / RTC_SECONDS_PER_DAY;
  sod = src->tv_sec % RTC_SECONDS_PER_DAY;

  if (sod < 0) {
    sod += RTC_SECONDS_PER_DAY;
    days--;
  }

  rtc_civil_from_days(days, &year, &month, &day);
  if (year < (int64_t)RTC_BASE_YEAR) {
    return HAL_RET_CONFIG_ERROR;
  }

  dst->year = (uint16_t)(year - RTC_BASE_YEAR);
  dst->month = (uint8_t)month;
  dst->day = (uint8_t)day;
  dst->dayofweek = rtc_encode_day_of_week(days);
  dst->dstflag = 0U;
  dst->millisecond = (uint32_t)sod * 1000U + src->tv_nsec / 1000000U;

  return HAL_RET_SUCCESS;
}

/*===========================================================================*/
/* Module class "hal_rtc_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_rtc_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_rtc_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__rtc_objinit_impl(void *ip, const void *vmt) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

  /* Initialization code.*/
  self->events = 0U;

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance to be
 *                              disposed.
 */
void __rtc_dispose_impl(void *ip) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 */
msg_t __rtc_start_impl(void *ip, const void *config) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  msg_t msg;

  if (config != NULL) {
    self->config = __rtc_setcfg_impl(self, config);
    if (self->config == NULL) {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  msg = rtc_lld_start(self);
  if (msg != HAL_RET_SUCCESS) {
    self->config = NULL;
  }

  return msg;
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 */
void __rtc_stop_impl(void *ip) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  rtc_lld_stop(self);
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__rtc_setcfg_impl(void *ip, const void *config) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  return (const void *)rtc_lld_setcfg(self, (const hal_rtc_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__rtc_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  return (const void *)rtc_lld_selcfg(self, cfgnum);
}

/**
 * @brief       Override of method @p __cbdrv_on_cb_set().
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 * @param         cb            Callback function to be associated. Passing @p
 *                              NULL disables the existing callback, if any.
 */
void __rtc_oncbset_impl(void *ip, drv_cb_t cb) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  rtc_lld_set_callback(self, cb);
}
/** @} */

/**
 * @brief       VMT structure of RTC driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_rtc_driver_vmt __hal_rtc_driver_vmt = {
  .dispose                  = __rtc_dispose_impl,
  .start                    = __rtc_start_impl,
  .stop                     = __rtc_stop_impl,
  .setcfg                   = __rtc_setcfg_impl,
  .selcfg                   = __rtc_selcfg_impl,
  .oncbset                  = __rtc_oncbset_impl
};

/**
 * @name        Regular methods of hal_rtc_driver_c
 * @{
 */
/**
 * @brief       Sets the RTC broken-down date/time.
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 * @param[in]     timespec      New date/time value.
 * @return                      The operation status.
 *
 * @api
 */
msg_t rtcSetDateTime(void *ip, const rtc_datetime_t *timespec) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  osalDbgCheck((self != NULL) && (timespec != NULL));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  if (!rtc_datetime_is_valid(timespec)) {
    return HAL_RET_CONFIG_ERROR;
  }

  return rtc_lld_set_datetime(self, timespec);
}

/**
 * @brief       Reads the RTC broken-down date/time.
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 * @param[out]    timespec      Current date/time value.
 * @return                      The operation status.
 *
 * @xclass
 */
msg_t rtcGetDateTimeX(void *ip, rtc_datetime_t *timespec) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  osalDbgCheck((self != NULL) && (timespec != NULL));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  return rtc_lld_get_datetime(self, timespec);
}

/**
 * @brief       Reads the RTC broken-down date/time.
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 * @param[out]    timespec      Current date/time value.
 * @return                      The operation status.
 *
 * @api
 */
msg_t rtcGetDateTime(void *ip, rtc_datetime_t *timespec) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  return rtcGetDateTimeX(self, timespec);
}

/**
 * @brief       Sets the RTC from a 64-bit UTC time value.
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 * @param[in]     timespec      New 64-bit time value.
 * @return                      The operation status.
 *
 * @api
 */
msg_t rtcSetTime64(void *ip, const rtc_time64_t *timespec) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  rtc_datetime_t dt;
  msg_t msg;

  osalDbgCheck((ip != NULL) && (timespec != NULL));

  msg = rtcConvertTime64ToDateTime(timespec, &dt);
  if (msg != HAL_RET_SUCCESS) {
    return msg;
  }

  return rtcSetDateTime(self, &dt);
}

/**
 * @brief       Reads the RTC as a 64-bit UTC time value.
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 * @param[out]    timespec      Current 64-bit time value.
 * @return                      The operation status.
 *
 * @xclass
 */
msg_t rtcGetTime64X(void *ip, rtc_time64_t *timespec) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  rtc_datetime_t dt;
  msg_t msg;

  osalDbgCheck((ip != NULL) && (timespec != NULL));

  msg = rtcGetDateTimeX(self, &dt);
  if (msg != HAL_RET_SUCCESS) {
    return msg;
  }

  return rtcConvertDateTimeToTime64(&dt, timespec);
}

/**
 * @brief       Reads the RTC as a 64-bit UTC time value.
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 * @param[out]    timespec      Current 64-bit time value.
 * @return                      The operation status.
 *
 * @api
 */
msg_t rtcGetTime64(void *ip, rtc_time64_t *timespec) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  return rtcGetTime64X(self, timespec);
}

/**
 * @brief       Programs an RTC alarm.
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 * @param[in]     alarm         Alarm selector.
 * @param[in]     alarmspec     Alarm specification.
 * @return                      The operation status.
 *
 * @api
 */
msg_t rtcSetAlarm(void *ip, rtcalarm_t alarm, const rtc_alarm_t *alarmspec) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  if (alarm >= (rtcalarm_t)RTC_ALARMS) {
    return HAL_RET_CONFIG_ERROR;
  }

  return rtc_lld_set_alarm(self, alarm, alarmspec);
}

/**
 * @brief       Reads an RTC alarm configuration.
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 * @param[in]     alarm         Alarm selector.
 * @param[out]    alarmspec     Alarm specification.
 * @return                      The operation status.
 *
 * @api
 */
msg_t rtcGetAlarm(void *ip, rtcalarm_t alarm, rtc_alarm_t *alarmspec) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  osalDbgCheck((self != NULL) && (alarmspec != NULL));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  if (alarm >= (rtcalarm_t)RTC_ALARMS) {
    return HAL_RET_CONFIG_ERROR;
  }

  return rtc_lld_get_alarm(self, alarm, alarmspec);
}
/** @} */

#endif /* HAL_USE_RTC == TRUE */

/** @} */

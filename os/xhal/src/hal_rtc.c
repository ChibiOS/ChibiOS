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
 * @brief       RTC Driver source.
 *
 * @addtogroup  HAL_RTC
 * @{
 */

#include "hal.h"

#if (HAL_USE_RTC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

#define RTC_SECONDS_PER_DAY              86400LL
#define RTC_MILLISECONDS_PER_DAY         86400000U

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

static const uint8_t rtc_month_len[12] = {
  31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U
};

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static bool rtc_is_leap_year(unsigned year) {

  return ((((year % 4U) == 0U) && ((year % 100U) != 0U)) ||
          ((year % 400U) == 0U));
}

static uint8_t rtc_get_month_length(unsigned year, unsigned month) {
  uint8_t n;

  n = rtc_month_len[month - 1U];
  if ((month == 2U) && rtc_is_leap_year(year)) {
    n++;
  }

  return n;
}

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
  if ((timespec->dayofweek > RTC_DAY_SUNDAY) ||
      (timespec->millisecond >= RTC_MILLISECONDS_PER_DAY)) {
    return false;
  }

  year = (unsigned)timespec->year + RTC_BASE_YEAR;

  return year >= RTC_BASE_YEAR;
}

/*
 * Howard Hinnant's days-from-civil algorithm, adapted to the local coding
 * style and integer types.
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

static void rtc_civil_from_days(int64_t z,
                                int64_t *year,
                                unsigned *month,
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

void rtcInit(void) {

  rtc_lld_init();
}

void *__rtc_objinit_impl(void *ip, const void *vmt) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;

  __cbdrv_objinit_impl(self, vmt);
  self->events = 0U;

  return self;
}

void __rtc_dispose_impl(void *ip) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;

  (void)self;
  __cbdrv_dispose_impl(self);
}

msg_t __rtc_start_impl(void *ip) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;

  return rtc_lld_start(self);
}

void __rtc_stop_impl(void *ip) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;

  rtc_lld_stop(self);
}

const void *__rtc_setcfg_impl(void *ip, const void *config) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;

  return (const void *)rtc_lld_setcfg(self, (const hal_rtc_config_t *)config);
}

const void *__rtc_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;

  return (const void *)rtc_lld_selcfg(self, cfgnum);
}

void __rtc_setcb_impl(void *ip, drv_cb_t cb) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;

  __cbdrv_setcb_impl(self, cb);
  rtc_lld_set_callback(self, cb);
}

const struct hal_rtc_driver_vmt __hal_rtc_driver_vmt = {
  .dispose                  = __rtc_dispose_impl,
  .start                    = __rtc_start_impl,
  .stop                     = __rtc_stop_impl,
  .setcfg                   = __rtc_setcfg_impl,
  .selcfg                   = __rtc_selcfg_impl,
  .setcb                    = __rtc_setcb_impl
};

msg_t rtcSetDateTime(void *ip, const rtc_datetime_t *timespec) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;

  osalDbgCheck((self != NULL) && (timespec != NULL));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  if (!rtc_datetime_is_valid(timespec)) {
    return HAL_RET_CONFIG_ERROR;
  }

  return rtc_lld_set_datetime(self, timespec);
}

msg_t rtcGetDateTimeX(void *ip, rtc_datetime_t *timespec) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;

  osalDbgCheck((self != NULL) && (timespec != NULL));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  return rtc_lld_get_datetime(self, timespec);
}

msg_t rtcGetDateTime(void *ip, rtc_datetime_t *timespec) {

  return rtcGetDateTimeX(ip, timespec);
}

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

msg_t rtcSetTime64(void *ip, const rtc_time64_t *timespec) {
  rtc_datetime_t dt;
  msg_t msg;

  osalDbgCheck((ip != NULL) && (timespec != NULL));

  msg = rtcConvertTime64ToDateTime(timespec, &dt);
  if (msg != HAL_RET_SUCCESS) {
    return msg;
  }

  return rtcSetDateTime(ip, &dt);
}

msg_t rtcGetTime64X(void *ip, rtc_time64_t *timespec) {
  rtc_datetime_t dt;
  msg_t msg;

  osalDbgCheck((ip != NULL) && (timespec != NULL));

  msg = rtcGetDateTimeX(ip, &dt);
  if (msg != HAL_RET_SUCCESS) {
    return msg;
  }

  return rtcConvertDateTimeToTime64(&dt, timespec);
}

msg_t rtcGetTime64(void *ip, rtc_time64_t *timespec) {

  return rtcGetTime64X(ip, timespec);
}

msg_t rtcSetAlarm(void *ip, rtcalarm_t alarm, const rtc_alarm_t *alarmspec) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;

  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  if (alarm >= (rtcalarm_t)RTC_ALARMS) {
    return HAL_RET_CONFIG_ERROR;
  }

  return rtc_lld_set_alarm(self, alarm, alarmspec);
}

msg_t rtcGetAlarm(void *ip, rtcalarm_t alarm, rtc_alarm_t *alarmspec) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;

  osalDbgCheck((self != NULL) && (alarmspec != NULL));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  if (alarm >= (rtcalarm_t)RTC_ALARMS) {
    return HAL_RET_CONFIG_ERROR;
  }

  return rtc_lld_get_alarm(self, alarm, alarmspec);
}

#endif /* HAL_USE_RTC == TRUE */

/** @} */

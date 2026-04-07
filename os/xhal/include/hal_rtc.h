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
 * @file        hal_rtc.h
 * @brief       RTC Driver header.
 *
 * @addtogroup  HAL_RTC
 * @brief       RTC Driver macros and structures.
 * @{
 */

#ifndef HAL_RTC_H
#define HAL_RTC_H

#if (HAL_USE_RTC == TRUE) || defined(__DOXYGEN__)

#include "hal_cb_driver.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief       Base year of the RTC broken-down representation.
 */
#define RTC_BASE_YEAR                   1980U

/**
 * @name        Day of week encoding
 * @{
 */
#define RTC_DAY_CATURDAY                0U
#define RTC_DAY_MONDAY                  1U
#define RTC_DAY_TUESDAY                 2U
#define RTC_DAY_WEDNESDAY               3U
#define RTC_DAY_THURSDAY                4U
#define RTC_DAY_FRIDAY                  5U
#define RTC_DAY_SATURDAY                6U
#define RTC_DAY_SUNDAY                  7U
/** @} */

/**
 * @name        RTC event flags
 * @{
 */
#define RTC_FLAGS_ALARM_A               (1U << 0)
#define RTC_FLAGS_ALARM_B               (1U << 1)
#define RTC_FLAGS_TS                    (1U << 2)
#define RTC_FLAGS_TS_OVF                (1U << 3)
#define RTC_FLAGS_TAMP1                 (1U << 4)
#define RTC_FLAGS_TAMP2                 (1U << 5)
#define RTC_FLAGS_TAMP3                 (1U << 6)
#define RTC_FLAGS_WAKEUP                (1U << 7)
/** @} */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of RTC event flags.
 */
typedef uint32_t rtceventflags_t;

/**
 * @brief       Type of structure representing an RTC driver.
 */
typedef struct hal_rtc_driver hal_rtc_driver_c;

/**
 * @brief       Type of structure representing an RTC configuration.
 */
typedef struct hal_rtc_config hal_rtc_config_t;

/**
 * @brief       Type of an RTC alarm number.
 */
typedef unsigned int rtcalarm_t;

/**
 * @brief       Type of a structure representing an RTC date/time stamp.
 */
typedef struct {
  uint16_t                  year;
  uint8_t                   month;
  uint8_t                   day;
  uint8_t                   dayofweek;
  uint8_t                   dstflag;
  uint32_t                  millisecond;
} rtc_datetime_t;

/**
 * @brief       Linux-like 64-bit time representation.
 */
typedef struct {
  int64_t                   tv_sec;
  uint32_t                  tv_nsec;
} rtc_time64_t;

/* Inclusion of LLD header.*/
#include "hal_rtc_lld.h"

/**
 * @brief       Driver configuration structure.
 * @note        Implementations may extend this structure to contain more,
 *              architecture dependent, fields.
 */
struct hal_rtc_config {
  /**
   * @brief       Initial value for the RTC CR register.
   */
  uint32_t                  cr;
  /**
   * @brief       Initial value for the RTC PRER register.
   */
  uint32_t                  prer;
  /* End of the mandatory fields.*/
  rtc_lld_config_fields;
#if defined(RTC_CONFIG_EXT_FIELDS) || defined(__DOXYGEN__)
  RTC_CONFIG_EXT_FIELDS
#endif
};

/**
 * @brief       Type of structure representing an RTC alarm time stamp.
 */
typedef struct {
  uint32_t                  alrmr;
} rtc_alarm_t;

/**
 * @class       hal_rtc_driver_c
 * @extends     hal_cb_driver_c
 *
 * @brief       Class of an RTC driver.
 * @{
 */

/**
 * @brief       Class @p hal_rtc_driver_c virtual methods table.
 */
struct hal_rtc_driver_vmt {
  void (*dispose)(void *ip);
  msg_t (*start)(void *ip);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  void (*setcb)(void *ip, drv_cb_t cb);
};

/**
 * @brief       Structure representing an RTC driver class.
 */
struct hal_rtc_driver {
  const struct hal_rtc_driver_vmt *vmt;
  driver_state_t            state;
  const void                *config;
  void                      *arg;
#if (HAL_USE_MUTUAL_EXCLUSION == TRUE) || defined(__DOXYGEN__)
  mutex_t                   mutex;
#endif
#if (HAL_USE_REGISTRY == TRUE) || defined(__DOXYGEN__)
  unsigned int              id;
  const char                *name;
  hal_regent_t              regent;
#endif
  drv_cb_t                  cb;
  volatile rtceventflags_t  events;
#if defined(RTC_DRIVER_EXT_FIELDS) || defined(__DOXYGEN__)
  RTC_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  rtc_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern hal_rtc_driver_c RTCD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void rtcInit(void);
  void *__rtc_objinit_impl(void *ip, const void *vmt);
  void __rtc_dispose_impl(void *ip);
  msg_t __rtc_start_impl(void *ip);
  void __rtc_stop_impl(void *ip);
  const void *__rtc_setcfg_impl(void *ip, const void *config);
  const void *__rtc_selcfg_impl(void *ip, unsigned cfgnum);
  void __rtc_setcb_impl(void *ip, drv_cb_t cb);
  msg_t rtcSetDateTime(void *ip, const rtc_datetime_t *timespec);
  msg_t rtcGetDateTimeX(void *ip, rtc_datetime_t *timespec);
  msg_t rtcGetDateTime(void *ip, rtc_datetime_t *timespec);
  msg_t rtcSetTime64(void *ip, const rtc_time64_t *timespec);
  msg_t rtcGetTime64X(void *ip, rtc_time64_t *timespec);
  msg_t rtcGetTime64(void *ip, rtc_time64_t *timespec);
  msg_t rtcSetAlarm(void *ip, rtcalarm_t alarm, const rtc_alarm_t *alarmspec);
  msg_t rtcGetAlarm(void *ip, rtcalarm_t alarm, rtc_alarm_t *alarmspec);
  msg_t rtcConvertDateTimeToTime64(const rtc_datetime_t *src,
                                   rtc_time64_t *dst);
  msg_t rtcConvertTime64ToDateTime(const rtc_time64_t *src,
                                   rtc_datetime_t *dst);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_rtc_driver_c
 * @{
 */
CC_FORCE_INLINE
static inline hal_rtc_driver_c *rtcObjectInit(hal_rtc_driver_c *self) {
  extern const struct hal_rtc_driver_vmt __hal_rtc_driver_vmt;

  return __rtc_objinit_impl(self, &__hal_rtc_driver_vmt);
}
/** @} */

/**
 * @name        Inline methods of hal_rtc_driver_c
 * @{
 */
CC_FORCE_INLINE
static inline rtceventflags_t rtcGetEventsX(void *ip) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;

  return self->events;
}

CC_FORCE_INLINE
static inline rtceventflags_t rtcGetAndClearEventsX(void *ip,
                                                    rtceventflags_t mask) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  rtceventflags_t flags;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  flags = self->events & mask;
  self->events &= ~mask;
  osalSysRestoreStatusX(sts);

  return flags;
}
/** @} */

#endif /* HAL_USE_RTC == TRUE */
#endif /* HAL_RTC_H */

/** @} */

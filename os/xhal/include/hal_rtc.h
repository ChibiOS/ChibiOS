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
 * @brief       Generated RTC Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_RTC
 * @brief       RTC Driver macros and structures.
 * @{
 */

#ifndef HAL_RTC_H
#define HAL_RTC_H

#include "hal_cb_driver.h"

#if (HAL_USE_RTC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief       Base year of the RTC broken-down representation.
 */
#define RTC_BASE_YEAR                       1980U

/**
 * @name    Day of week encoding
 * @{
 */
/**
 * @brief       Zero/reserved day encoding.
 */
#define RTC_DAY_RESERVED                    0U

/**
 * @brief       Monday day encoding.
 */
#define RTC_DAY_MONDAY                      1U

/**
 * @brief       Tuesday day encoding.
 */
#define RTC_DAY_TUESDAY                     2U

/**
 * @brief       Wednesday day encoding.
 */
#define RTC_DAY_WEDNESDAY                   3U

/**
 * @brief       Thursday day encoding.
 */
#define RTC_DAY_THURSDAY                    4U

/**
 * @brief       Friday day encoding.
 */
#define RTC_DAY_FRIDAY                      5U

/**
 * @brief       Saturday day encoding.
 */
#define RTC_DAY_SATURDAY                    6U

/**
 * @brief       Sunday day encoding.
 */
#define RTC_DAY_SUNDAY                      7U
/** @} */

/**
 * @name    RTC event flags
 * @{
 */
/**
 * @brief       Alarm A event flag.
 */
#define RTC_FLAGS_ALARM_A                   (1U << 0)

/**
 * @brief       Alarm B event flag.
 */
#define RTC_FLAGS_ALARM_B                   (1U << 1)

/**
 * @brief       Timestamp event flag.
 */
#define RTC_FLAGS_TS                        (1U << 2)

/**
 * @brief       Timestamp overflow event flag.
 */
#define RTC_FLAGS_TS_OVF                    (1U << 3)

/**
 * @brief       Tamper 1 event flag.
 */
#define RTC_FLAGS_TAMP1                     (1U << 4)

/**
 * @brief       Tamper 2 event flag.
 */
#define RTC_FLAGS_TAMP2                     (1U << 5)

/**
 * @brief       Tamper 3 event flag.
 */
#define RTC_FLAGS_TAMP3                     (1U << 6)

/**
 * @brief       Wakeup event flag.
 */
#define RTC_FLAGS_WAKEUP                    (1U << 7)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

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

/**
 * @brief       Type of structure representing an RTC alarm time stamp.
 */
typedef struct {
  uint32_t                  alrmr;
} rtc_alarm_t;

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
#if (defined(RTC_CONFIG_EXT_FIELDS)) || defined (__DOXYGEN__)
  RTC_CONFIG_EXT_FIELDS
#endif /* defined(RTC_CONFIG_EXT_FIELDS) */
};

/**
 * @class       hal_rtc_driver_c
 * @extends     hal_cb_driver_c
 *
 * @brief       Class of an RTC driver.
 *
 * @name        Class @p hal_rtc_driver_c structures
 * @{
 */

/**
 * @brief       Type of a RTC driver class.
 */
typedef struct hal_rtc_driver hal_rtc_driver_c;

/**
 * @brief       Class @p hal_rtc_driver_c virtual methods table.
 */
struct hal_rtc_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip, const void *config);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_cb_driver_c.*/
  void (*oncbset)(void *ip, drv_cb_t cb);
  /* From hal_rtc_driver_c.*/
};

/**
 * @brief       Structure representing a RTC driver class.
 */
struct hal_rtc_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_rtc_driver_vmt *vmt;
  /**
   * @brief       Driver state.
   */
  driver_state_t            state;
  /**
   * @brief       Associated configuration structure.
   */
  const void                *config;
  /**
   * @brief       Driver argument.
   */
  void                      *arg;
#if (HAL_USE_MUTUAL_EXCLUSION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Driver mutex.
   */
  mutex_t                   mutex;
#endif /* HAL_USE_MUTUAL_EXCLUSION == TRUE */
#if (HAL_USE_REGISTRY == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Driver identifier.
   */
  unsigned int              id;
  /**
   * @brief       Driver name.
   */
  const char                *name;
  /**
   * @brief       Registry link structure.
   */
  hal_regent_t              regent;
#endif /* HAL_USE_REGISTRY == TRUE */
  /**
   * @brief       Driver callback.
   * @note        Can be @p NULL.
   */
  drv_cb_t                  cb;
  /**
   * @brief       Pending RTC event flags.
   */
  volatile rtceventflags_t  events;
#if (defined(RTC_DRIVER_EXT_FIELDS)) || defined (__DOXYGEN__)
  RTC_DRIVER_EXT_FIELDS
#endif /* defined(RTC_DRIVER_EXT_FIELDS) */
  /* End of the mandatory fields.*/
  rtc_lld_driver_fields;
};
/** @} */

#if !defined(__DOXYGEN__)
extern hal_rtc_driver_c RTCD1;
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_rtc_driver_c.*/
  void *__rtc_objinit_impl(void *ip, const void *vmt);
  void __rtc_dispose_impl(void *ip);
  msg_t __rtc_start_impl(void *ip, const void *config);
  void __rtc_stop_impl(void *ip);
  const void *__rtc_setcfg_impl(void *ip, const void *config);
  const void *__rtc_selcfg_impl(void *ip, unsigned cfgnum);
  void __rtc_oncbset_impl(void *ip, drv_cb_t cb);
  msg_t rtcSetDateTime(void *ip, const rtc_datetime_t *timespec);
  msg_t rtcGetDateTimeX(void *ip, rtc_datetime_t *timespec);
  msg_t rtcGetDateTime(void *ip, rtc_datetime_t *timespec);
  msg_t rtcSetTime64(void *ip, const rtc_time64_t *timespec);
  msg_t rtcGetTime64X(void *ip, rtc_time64_t *timespec);
  msg_t rtcGetTime64(void *ip, rtc_time64_t *timespec);
  msg_t rtcSetAlarm(void *ip, rtcalarm_t alarm, const rtc_alarm_t *alarmspec);
  msg_t rtcGetAlarm(void *ip, rtcalarm_t alarm, rtc_alarm_t *alarmspec);
  /* Regular functions.*/
  void rtcInit(void);
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
/**
 * @brief       Default initialization function of @p hal_rtc_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_rtc_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
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
/**
 * @brief       Returns the cached RTC event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 * @return                      The current cached event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline rtceventflags_t rtcGetEventsX(void *ip) {
  hal_rtc_driver_c *self = (hal_rtc_driver_c *)ip;
  return self->events;
}

/**
 * @brief       Gets and clears cached RTC event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_rtc_driver_c instance.
 * @param[in]     mask          Mask of events to be returned and cleared.
 * @return                      The selected pending event flags.
 *
 * @xclass
 */
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

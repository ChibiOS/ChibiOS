/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    STM32/RTCv2/rtc_lld.h
 * @brief   STM32L1xx/STM32F2xx/STM32F4xx RTC low level driver header.
 *
 * @addtogroup RTC
 * @{
 */

#ifndef _RTC_LLD_H_
#define _RTC_LLD_H_

#if HAL_USE_RTC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   This RTC implementation doesn't support callbacks.
 */
#define RTC_SUPPORTS_CALLBACKS      FALSE

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Two alarm comparators available on STM32F4x.
 */
#define RTC_ALARMS                  2

/**
 * @brief   Callback enable masks.
 */
#define ALARMA_CB_FLAG    0x1
#define ALARMB_CB_FLAG    0x2
#define WAKEUP_CB_FLAG    0x4
#define TIMESTAMP_CB_FLAG 0x8

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if HAL_USE_RTC && !STM32_HAS_RTC
#error "RTC not present in the selected device"
#endif

#if !(STM32_RTCSEL == STM32_RTCSEL_LSE) &&                                  \
    !(STM32_RTCSEL == STM32_RTCSEL_LSI) &&                                  \
    !(STM32_RTCSEL == STM32_RTCSEL_HSEDIV)
#error "invalid source selected for RTC clock"
#endif

#if RTC_SUPPORTS_CALLBACKS && !(HAL_USE_EXT)
#error "interrupts from RTC works only through EXTI on this platform"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an RTC alarm time stamp.
 */
typedef struct RTCAlarm RTCAlarm;

/**
 * @brief   Type of a structure representing an RTC wakeup period.
 */
typedef struct RTCWakeup RTCWakeup;

/**
 * @brief   Type of a structure representing an RTC callbacks config.
 */
typedef struct RTCCallbackConfig RTCCallbackConfig;

/**
 * @brief   Type of an RTC alarm.
 * @details Meaningful on platforms with more than 1 alarm comparator.
 */
typedef uint32_t rtcalarm_t;

/**
 * @brief   Type of an RTC event.
 */
typedef enum {
  RTC_EVENT_WAKEUP = 0,           /** Triggered every wakeup event.          */
  RTC_EVENT_ALARM_A = 1,          /** Triggered on alarm A.                  */
  RTC_EVENT_ALARM_B = 2,          /** Triggered on alarm B.                  */
  RTC_EVENT_TAMPER = 3,           /** Triggered on Tamper event.             */
  RTC_EVENT_TIMESTAMP = 4,        /** Triggered on TimeStamp event.          */
} rtcevent_t;

/**
 * @brief   Structure representing an RTC time stamp.
 */
struct RTCTime {
  /**
   * @brief RTC date register in STM32 BCD format.
   */
  uint32_t tv_date;
  /**
   * @brief RTC time register in STM32 BCD format.
   */
  uint32_t tv_time;
  /**
   * @brief Fractional part of time.
   */
#if STM32_RTC_HAS_SUBSECONDS
  uint32_t tv_msec;
#endif
};

/**
 * @brief   Structure representing an RTC alarm time stamp.
 */
struct RTCAlarm {
  /**
   * @brief Date and time of alarm in STM32 BCD.
   */
  uint32_t tv_datetime;
};

/**
 * @brief   Structure representing an RTC periodic wakeup period.
 */
struct RTCWakeup {
  /**
   * @brief   RTC WUTR register.
   * @details Bits [15:0] contain value of WUTR register
   *          Bits [18:16] contain value of WUCKSEL bits in CR register
   *
   * @notes   ((WUTR == 0) || (WUCKSEL == 3)) is forbidden combination.
   */
  uint32_t wakeup;
};

/**
 * @brief   Structure representing an RTC callbacks config.
 * @details It is bitmask. Set bit to enable callback, clear bit to disable.
 *          bit0 - alarmA
 *          bit1 - alarmB
 *          bit2 - wakeup
 *          bit3 - timestamp
 */
struct RTCCallbackConfig{
  uint32_t cb_cfg;
};

/**
 * @brief   Structure representing an RTC driver.
 */
struct RTCDriver{
  /**
   * @brief Pointer to the RTC registers block.
   */
  RTC_TypeDef               *id_rtc;
  /**
   * @brief Current callback confuguration.
   */
  const RTCCallbackConfig   *cb_cfg;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern RTCDriver RTCD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void rtc_lld_init(void);
  void rtc_lld_set_time(RTCDriver *rtcp, const RTCTime *timespec);
  void rtc_lld_get_time(RTCDriver *rtcp, RTCTime *timespec);
  void rtc_lld_set_alarm(RTCDriver *rtcp,
                         rtcalarm_t alarm,
                         const RTCAlarm *alarmspec);
  void rtc_lld_get_alarm(RTCDriver *rtcp,
                         rtcalarm_t alarm,
                         RTCAlarm *alarmspec);
  void rtc_lld_set_periodic_wakeup(RTCDriver *rtcp, RTCWakeup *wakeupspec);
  void rtc_lld_get_periodic_wakeup(RTCDriver *rtcp, RTCWakeup *wakeupspec);
  void rtc_lld_set_callback(RTCDriver *rtcp, RTCCallbackConfig *cb_cfg);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_RTC */

#endif /* _RTC_LLD_H_ */

/** @} */

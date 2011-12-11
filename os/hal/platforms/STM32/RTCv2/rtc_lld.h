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
 * @file    STM32/RTCv1/rtc_lld.h
 * @brief   STM32 RTC subsystem low level driver header.
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
 * @brief   This RTC implementation supports callbacks.
 */
#define RTC_SUPPORTS_CALLBACKS      TRUE

/**
 * @brief   Two alarm comparators available on STM32F4x.
 */
#define RTC_ALARMS                  2

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if HAL_USE_RTC && !STM32_HAS_RTC
#error "RTC not present in the selected device"
#endif

#if !(STM32_RTC == STM32_RTC_LSE) && !(STM32_RTC == STM32_RTC_LSI) &&       \
    !(STM32_RTC == STM32_RTC_HSE)
#error "invalid source selected for RTC clock"
#endif

#if RTC_SUPPORTS_CALLBACKS && !(HAL_USE_EXT)
#error "interrupts from STM32 RTC works only through EXTI"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an RTC alarm stamp.
 */
typedef struct RTCAlarm RTCAlarm;

/**
 * @brief   Type of an RTC alarm.
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
 * @brief   Type of a generic RTC callback.
 */
typedef void (*rtccb_t)(RTCDriver *rtcp, rtcevent_t event);

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
   * @note  If platform does not support subseconds than always zero.
   */
  uint16_t tv_msec;
};


/**
 * @brief   Structure representing an RTC alarm specification.
 */
struct RTCAlarm {
  /**
   * @brief Date and time of alarm in STM32 BCD.
   */
  uint32_t tv_datetime;
};


/**
 * @brief   Structure representing an RTC driver.
 */
struct RTCDriver{
  /**
   * @brief Callback pointer.
   */
  rtccb_t           rtc_cb;
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
  void rtc_lld_set_callback(RTCDriver *rtcp, rtccb_t callback);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_RTC */

#endif /* _RTC_LLD_H_ */

/** @} */

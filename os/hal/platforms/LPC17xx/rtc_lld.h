/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC17xx RTC driver - Copyright (C) 2013 Marcin Jokel

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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    LPC17xx/rtc_lld.h
 * @brief   LPC17xx RTC low level driver header.
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

#define ILR_RTCCIF         0x01
#define ILR_RTCALF         0x02

#define CCR_CLKEN          0x01
#define CCR_CTCRST         0x02
#define CCR_CCALEN         0x10

#define CIIR_IMSEC         0x01
#define CIIR_IMMIN         0x02
#define CIIR_IMHOUR        0x04
#define CIIR_IMDOM         0x08
#define CIIR_IMDOW         0x10
#define CIIR_IMDOY         0x20
#define CIIR_IMMON         0x40
#define CIIR_IMYEAR        0x80

#define AMR_AMRSEC         0x01
#define AMR_AMRMIN         0x02
#define AMR_AMRHOUR        0x04
#define AMR_AMRDOM         0x08
#define AMR_AMRDOW         0x10
#define AMR_AMRDOY         0x20
#define AMR_AMRMON         0x40
#define AMR_AMRYEAR        0x80
#define AMR_MASK_ALL       0xFF

#define RTC_AUX_RTC_OSCF        0x10

#define RTC_AUXEN_RTC_OSCFEN    0x10

/**
 * @brief   This RTC implementation supports callbacks.
 */
#define RTC_SUPPORTS_CALLBACKS      TRUE

/**
 * @brief   One alarm comparator available.
 */
#define RTC_ALARMS          		1

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/*
 * RTC driver system settings.
 */

/**
 * @brief   RTC Alarm enable.
 */
#if !defined(LPC17xx_RTC_USE_ALARM) || defined(__DOXYGEN__)
#define LPC17xx_RTC_USE_ALARM         FALSE
#endif

/**
 * @brief   RTC IRQ Priority.
 */
#if !defined(LPC17xx_RTC_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC17xx_RTC_IRQ_PRIORITY      3
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

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
  RTC_EVENT_ALARM = 0                  /** Triggered on alarm. */
} rtcevent_t;

/**
 * @brief   Type of a generic RTC callback.
 */
typedef void (*rtccb_t)(RTCDriver *rtcp, rtcevent_t event);

/**
 * @brief   Structure representing an RTC callbacks config.
 */
struct RTCCallbackConfig{
  /**
   * @brief Generic RTC callback pointer.
   */
  rtccb_t           callback;
};

/**
 * @brief   Structure representing an RTC time stamp.
 */
struct RTCTime {
  /**
   * @brief RTC seconds register.
   */
  uint8_t sec;
  /**
   * @brief RTC minutes register.
   */
  uint8_t min;
  /**
   * @brief RTC hours register.
   */
  uint8_t hour;
  /**
   * @brief RTC day of month register.
   */
  uint8_t dom;
  /**
   * @brief RTC month register.
   */
  uint8_t month;
  /**
   * @brief RTC day of week register.
   */
  uint8_t dow;
  /**
   * @brief RTC year register.
   */
  uint16_t year;
  /**
   * @brief RTC day of year register.
   */
  uint16_t doy;

};

/**
 * @brief   Structure representing an RTC alarm time stamp.
 */
struct RTCAlarm {
  /**
   * @brief RTC alarm seconds register.
   */
  uint8_t alsec;
  /**
   * @brief RTC alarm minutes register.
   */
  uint8_t almin;
  /**
   * @brief RTC alarm hours register.
   */
  uint8_t alhour;
  /**
   * @brief RTC alarm day of month register.
   */
  uint8_t aldom;
  /**
   * @brief RTC alarm month register.
   */
  uint8_t almonth;
  /**
   * @brief RTC alarm day of week register.
   */
  uint8_t aldow;
  /**
   * @brief RTC alarm year register.
   */
  uint16_t alyear;
  /**
   * @brief RTC alarm day of year register.
   */
  uint16_t aldoy;

};

/**
 * @brief   Structure representing an RTC driver.
 */
struct RTCDriver{
  /**
   * @brief Callback pointer.
   */
  rtccb_t           callback;
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
  uint32_t rtc_lld_get_time_fat(RTCDriver *rtcp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_RTC */

#endif /* _RTC_LLD_H_ */

/** @} */

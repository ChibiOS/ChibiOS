/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC122x RTC driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC122x/rtc_lld.h
 * @brief   LPC122x RTC low level driver header.
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

#define CR_RTCSTART         1
#define ICSC_RTCCIC         1
#define RIS_RTCRIS          1
#define MIS_RTCMIS          1
#define ICR_RTCICR          1

/**
 * @brief   This RTC implementation supports callbacks.
 */
#define RTC_SUPPORTS_CALLBACKS      TRUE

/**
 * @brief   One alarm comparator available.
 */
#define RTC_ALARMS              1

/**
 * @brief   RTC Clock source type
 */
#define SYSCFG_RTCCLK_1Hz		    0		  /* 1 Hz clock */
#define SYSCFG_RTCCLK_DEL_1Hz	  1		  /* delayed 1 Hz clock */
#define SYSCFG_RTCCLK_1kHz		  10		/* 1 kHz clock */
#define SYSCFG_RTCCLK_PCLK		  4		  /* Main clock source divided by RTC */
										                  /* clock divider */

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
 * @brief   RTC PCLK divider.
 */
#if !defined(LPC122x_RTC_CLKDIV) || defined(__DOXYGEN__)
#define LPC122x_RTC_CLKDIV          0
#endif

/**
 * @brief   RTC CLK Source
 */
#if !defined(LPC122x_RTCCLK) || defined(__DOXYGEN__)
#define LPC122x_RTCCLK   SYSCFG_RTCCLK_1Hz
#endif

/**
 * @brief   RTC Alarm enable.
 */
#if !defined(LPC122x_RTC_USE_ALARM) || defined(__DOXYGEN__)
#define LPC122x_RTC_USE_ALARM         FALSE
#endif

/**
 * @brief   RTC IRQ Priority.
 */
#if !defined(LPC122x_RTC_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC122x_RTC_IRQ_PRIORITY      3
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !(LPC122x_RTCCLK == SYSCFG_RTCCLK_1Hz) &&                                 \
    !(LPC122x_RTCCLK == SYSCFG_RTCCLK_DEL_1Hz) &&                             \
    !(LPC122x_RTCCLK == SYSCFG_RTCCLK_1kHz) &&                                \
    !(LPC122x_RTCCLK == SYSCFG_RTCCLK_PCLK)
#error "Invalid source selected for RTC clock"
#endif

#if LPC122x_RTCCLK == SYSCFG_RTCCLK_1kHz
#error "1 kHz RTC clock not supported"
#endif

#if LPC122x_RTCCLK == SYSCFG_RTCCLK_PCLK
#if (LPC122x_MAINCLK/LPC122x_RTC_CLKDIV) != 1
#error "RTC clock is not 1 Hz"
#endif
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
   * @brief Seconds since UNIX epoch.
   */
  uint32_t tv_sec;
};

/**
 * @brief   Structure representing an RTC alarm time stamp.
 */
struct RTCAlarm {
  /**
   * @brief Seconds since UNIX epoch.
   */
  uint32_t          tv_sec;
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

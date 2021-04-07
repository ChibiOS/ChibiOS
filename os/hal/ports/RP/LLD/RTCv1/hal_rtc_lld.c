/*
    ChibiOS - Copyright (C) 2006..2021 Giovanni Di Sirio

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
 * @file    hal_rtc_lld.c
 * @brief   RP2040 RTC subsystem low level driver source.
 *
 * @addtogroup RTC
 * @{
 */

#include "hal.h"
#include "hardware/rtc.h"

#if (HAL_USE_RTC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief RTC driver identifier.
 */
RTCDriver RTCD1;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enable access to registers.
 *
 * @notapi
 */
void rtc_lld_init(void) {

  /* RTC object initialization.*/
  rtcObjectInit(&RTCD1);

  /* Callback initially disabled.*/
  RTCD1.callback = NULL;

  /* RTC pointer initialization.*/
  RTCD1.rtc = RTC;

  /* Get clock parameters. */
  clock = hal_lld_get_clock(clk_rtc);
  osalDbgAssert((clock > 0U) || (clock - 1 <= RTC_CLKDIV_M1_BITS), "bad clock");

  /* Take RTC out of reset. */
  hal_lld_peripheral_unreset(RESETS_ALLREG_RTC);

  /* Set divider. */
  RTCD1.rtc.CLKDIVM1 = clock - 1;
}

/**
 * @brief   Set current time.
 * @note    Fractional seconds part will be silently ignored. There is no
 *          possibility to set it on RP2040 platform. The RP2040 handles
 *          leap year for years evenly divisible by 4.
 * @note    The function can be called from any context.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] timespec  pointer to a @p RTCDateTime structure
 *
 * @notapi
 */
void rtc_lld_set_time(RTCDriver *rtcp, const RTCDateTime *timespec) {

  uint32_t sec = (uint32_t)timespec->millisecond / 1000;
  uint32_t hour = sec / 3600;
  sec %= 3600;
  uint32_t min = sec / 60;
  sec %= 60;

  /* Disable RTC. */
  rtcp->rtc->CTRL = 0;

  /* Wait for RTC to go inactive. */
  while (rtccp->rtc->CTRL & RTC_CTRL_RTC_ACTIVE_BITS != 0)
    ;

  /* Write setup to pre-load registers. */
  rtcp->rtc->SETUP0 =
    ((timespec->year + 1980)   << RTC_SETUP_0_YEAR_LSB)   |
    (timespec->month           << RTC_SETUP_0_MONTH_LSB)  |
    (timespec->day             << RTC_SETUP_0_DAY_LSB);
  rtcp->rtc->SETUP1 =
    ((timespec->dayofweek - 1) << RTC_SETUP_1_DOTW_LSB)   |
    (hour                      << RTC_SETUP_1_HOUR_LSB)   |
    (min                       << RTC_SETUP_1_MIN_LSB)    |
    (sec                       << RTC_SETUP_1_SEC_LSB);

  /* Move setup values into RTC clock domain. */
  rtcp->rtc->CTRL = RTC_CTRL_LOAD_BITS;

  /* Enable RTC and wait for active. */
  rtcp->rtc->CTRL = RTC_CTRL_RTC_ENABLE_BITS;
  while (rtccp->rtc->CTRL & RTC_CTRL_RTC_ACTIVE_BITS == 0)
    ;
}

/**
 * @brief   Get current time.
 * @note    The function can be called from any context.
 *
 * @param[in]  rtcp      pointer to RTC driver structure
 * @param[out] timespec pointer to a @p RTCDateTime structure
 *
 * @notapi
 */
void rtc_lld_get_time(RTCDriver *rtcp, RTCDateTime *timespec) {

  /* Read RTC0 first then RTC1. */
  uint32_t rtc_0 = rtcp->rtc->RTC0;
  uint32_t rtc_1 = rtcp->rtc->RTC1;

  /* Calculate and set milliseconds since midnight field. */
  timespec->millisecond =
      ((((rtc_0 & RTC_RTC_0_HOUR_BITS) >> RTC_RTC_0_HOUR_LSB) * 3600)
    + (((rtc_0 & RTC_RTC_0_MIN_BITS)   >> RTC_RTC_0_MIN_LSB) * 60)
    + (((rtc_0 & RTC_RTC_0_SEC_BITS)   >> RTC_RTC_0_SEC_LSB))) * 1000;

  /* Set fields with adjustments. */
  timespec->dayofweek =
      ((rtc_0 & RTC_RTC_0_DOTW_BITS)  >> RTC_RTC_0_DOTW_LSB) + 1;
  timespec->year      =
      ((rtc_1 & RTC_RTC_1_YEAR_BITS)  >> RTC_RTC_1_YEAR_LSB) - 1980;
  timespec->month     =
      ((rtc_1 & RTC_RTC_1_MONTH_BITS) >> RTC_RTC_1_MONTH_LSB);
  timespec->day       =
      ((rtc_1 & RTC_RTC_1_DAY_BITS)   >> RTC_RTC_1_DAY_LSB);
}

#if (RTC_ALARMS > 0) || defined(__DOXYGEN__)
/**
 * @brief   Set alarm time.
 * @note    Default value after BKP domain reset for both comparators is 0.
 * @note    Function does not performs any checks of alarm time validity.
 * @note    The function can be called from any context.
 *
 * @param[in] rtcp      pointer to RTC driver structure.
 * @param[in] alarm     alarm identifier. Can be 1 or 2.
 * @param[in] alarmspec pointer to a @p RTCAlarm structure.
 *
 * @notapi
 */
void rtc_lld_set_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       const RTCAlarm *alarmspec) {

  (void)rtcp;
  (void)alarm;
  (void)alarmspec;
}

/**
 * @brief   Get alarm time.
 * @note    The function can be called from any context.
 *
 * @param[in] rtcp       pointer to RTC driver structure
 * @param[in] alarm      alarm identifier
 * @param[out] alarmspec pointer to a @p RTCAlarm structure
 *
 * @notapi
 */
void rtc_lld_get_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       RTCAlarm *alarmspec) {

  (void)rtcp;
  (void)alarm;
  (void)alarmspec;
}
#endif /* RTC_ALARMS > 0 */

#endif /* HAL_USE_RTC */

/** @} */

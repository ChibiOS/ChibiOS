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

#if (RTC_ALARMS > 0) || defined(__DOXYGEN__)
static void rtc_enable_alarm(RTCDriver *rtcp) {
    /* Enable matching and wait for it to be activated. */
  rtcp->rtc->IRQSETUP0 |= RTC_IRQ_SETUP_0_MATCH_ENA;
  while (!(rtcp->rtc->IRQSETUP0 & RTC_IRQ_SETUP_0_MATCH_ACTIVE))
    ;
}

static void rtc_disable_alarm(RTCDriver *rtcp) {
    /* Disable alarm matching and wait until deactivated. */
  rtcp->rtc->IRQSETUP0 &= ~RTC_IRQ_SETUP_0_MATCH_ENA;
  while (rtcp->rtc->IRQSETUP0 & RTC_IRQ_SETUP_0_MATCH_ACTIVE)
    ;
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/
#if (RTC_ALARMS > 0) || defined(__DOXYGEN__)
/**
 * @brief   RTC alarm interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(RP_RTC_IRQ_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  if (RTCD1.callback != NULL) {
    RTCD1.callback(&RTCD1, 1);
  }

  OSAL_IRQ_EPILOGUE();
}
#endif

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
  uint32_t clock = hal_lld_get_clock(clk_rtc);
  osalDbgAssert((clock > 0U) || (clock - 1 <= RTC_CLKDIV_M1), "bad clock");

  /* Take RTC out of reset. */
  hal_lld_peripheral_unreset(RESETS_ALLREG_RTC);

  /* Set divider. */
  RTCD1.rtc->CLKDIVM1 = clock - 1;

  /* Enable RTC vector. */
  nvicEnableVector(RP_RTC_IRQ_NUMBER, RP_IRQ_RTC_PRIORITY);
}

/**
 * @brief   Set current time.
 * @note    Fractional seconds part will be silently ignored. There is no
 *          possibility to set it on RP2040 platform.
 * @note    The RP2040 handles leap year for years evenly divisible by 4.
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

  /* Entering a reentrant critical zone.*/
  syssts_t sts = osalSysGetStatusAndLockX();

  /* Disable RTC. */
  rtcp->rtc->CTRL &= ~RTC_CTRL_RTC_ENABLE;

  /* Wait for RTC to go inactive. */
  while ((rtcp->rtc->CTRL & RTC_CTRL_RTC_ACTIVE) != 0)
    ;


  /* Write setup to pre-load registers. */
  rtcp->rtc->SETUP0 = (RTC_SETUP_0_YEAR(timespec->year + 1980))     |
                      (RTC_SETUP_0_MONTH(timespec->month))          |
                      (RTC_SETUP_0_DAY(timespec->day));
  rtcp->rtc->SETUP1 = (RTC_SETUP_1_DOTW(timespec->dayofweek - 1))   |
                      (RTC_SETUP_1_HOUR(hour))                      |
                      (RTC_SETUP_1_MIN(min))                        |
                      (RTC_SETUP_1_SEC(sec));

  /* Move setup values into RTC clock domain. */
  rtcp->rtc->CTRL = RTC_CTRL_LOAD;

  /* Enable RTC and wait for active. */
  rtcp->rtc->CTRL |= RTC_CTRL_RTC_ENABLE;

  /* Leaving a reentrant critical zone.*/
  osalSysRestoreStatusX(sts);

  /* Wait for RTC to go active. */
  while ((rtcp->rtc->CTRL & RTC_CTRL_RTC_ACTIVE) == 0)
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

  /* Entering a reentrant critical zone.*/
  syssts_t sts = osalSysGetStatusAndLockX();

  /* Read RTC0 first then RTC1. */
  uint32_t rtc_0 = rtcp->rtc->RTC0;
  uint32_t rtc_1 = rtcp->rtc->RTC1;

  /* Leaving a reentrant critical zone.*/
  osalSysRestoreStatusX(sts);

  /* Calculate and set milliseconds since midnight field. */
  timespec->millisecond = ((RTC_RTC_0_HOUR(rtc_0) * 3600) +
                           (RTC_RTC_0_MIN(rtc_0) * 60) +
                           (RTC_RTC_0_SEC(rtc_0))) * 1000;

  /* Set RTCDateTime fields with adjustments from RTC data. */
  timespec->dayofweek = RTC_RTC_0_DOTW(rtc_0) + 1;
  timespec->year      = RTC_RTC_1_YEAR(rtc_1) - 1980;
  timespec->month     = RTC_RTC_1_MONTH(rtc_1);
  timespec->day       = RTC_RTC_1_DAY(rtc_1);
}

#if (RTC_ALARMS > 0) || defined(__DOXYGEN__)
/**
 * @brief   Set alarm time.
 * @note    The alarm time can be partially specified by leaving fields as zero.
 * @note    The function can be called from any context.
 *
 * @param[in] rtcp      pointer to RTC driver structure.
 * @param[in] alarm     alarm identifier. Can be 1.
 * @param[in] alarmspec pointer to a @p RTCAlarm structure.
 *
 * @notapi
 */
void rtc_lld_set_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       const RTCAlarm *alarmspec) {

  (void)alarm;
  uint32_t sec, min, hour, day, month, year, dotw, setup0, setup1;
  const RTCDateTime *timespec = &alarmspec->alarm;
  sec = (uint32_t)timespec->millisecond / 1000;
  hour = sec / 3600;
  sec %= 3600;
  min = sec / 60;
  sec %= 60;
  day = timespec->day;
  month = timespec->month;

  /* Normalise and setup for non-zero checking. */
  year = timespec->year == 0 ? 0 : timespec->year + 1980;
  dotw = timespec->dayofweek;

  /* Setup register data. */
  setup0 = (RTC_IRQ_SETUP_0_YEAR(year))        |
           (RTC_IRQ_SETUP_0_MONTH(month))      |
           (RTC_IRQ_SETUP_0_DAY(day));
  setup1 = (RTC_IRQ_SETUP_1_DOTW(dotw - 1))    |
           (RTC_IRQ_SETUP_1_HOUR(hour))        |
           (RTC_IRQ_SETUP_1_MIN(min))          |
           (RTC_IRQ_SETUP_1_SEC(sec));

  /* Check and set match enable bits for non-zero */
  if (year  > 0) setup0 |= RTC_IRQ_SETUP_0_YEAR_ENA;
  if (month > 0) setup0 |= RTC_IRQ_SETUP_0_MONTH_ENA;
  if (day   > 0) setup0 |= RTC_IRQ_SETUP_0_DAY_ENA;
  if (dotw  > 0) setup1 |= RTC_IRQ_SETUP_1_DOTW_ENA;
  if (hour  > 0) setup1 |= RTC_IRQ_SETUP_1_HOUR_ENA;
  if (min   > 0) setup1 |= RTC_IRQ_SETUP_1_MIN_ENA;
  if (sec   > 0) setup1 |= RTC_IRQ_SETUP_1_SEC_ENA;

  /* Entering a reentrant critical zone.*/
  syssts_t sts = osalSysGetStatusAndLockX();

  /* Disable RTC and load the alarm time. */
  rtc_disable_alarm(rtcp);
  rtcp->rtc->IRQSETUP0 = setup0;
  rtcp->rtc->IRQSETUP1 = setup1;

  /* Enable the interrupt and re-enable the RTC. */
  rtcp->rtc->INTE = RTC_INTE_RTC;
  rtc_enable_alarm(rtcp);

  /* Save the alarm setting. */
  rtcp->alarm = *timespec;

  /* Leaving a reentrant critical zone.*/
  osalSysRestoreStatusX(sts);
}

/**
 * @brief   Get alarm time.
 * @note    The function can be called from any context.
 *
 * @param[in]  rtcp       pointer to RTC driver structure
 * @param[in]  alarm      alarm identifier
 * @param[out] alarmspec pointer to a @p RTCAlarm structure
 *
 * @notapi
 */
void rtc_lld_get_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       RTCAlarm *alarmspec) {

  (void)alarm;
  alarmspec->alarm = rtcp->alarm;
}
#endif /* RTC_ALARMS > 0 */

/**
 * @brief   Enables or disables RTC callbacks.
 * @details This function enables or disables callbacks. Use a @p NULL pointer
 *          in order to disable a callback.
 * @note    The function can be called from any context.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] callback  callback function pointer or @p NULL
 *
 * @notapi
 */
void rtc_lld_set_callback(RTCDriver *rtcp, rtccb_t callback) {

  rtcp->callback = callback;
}

#endif /* HAL_USE_RTC */

/** @} */

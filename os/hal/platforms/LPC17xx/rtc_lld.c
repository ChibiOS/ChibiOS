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
 * @file    LPC17xx/rtc_lld.c
 * @brief   LPC17xx RTC low level driver.
 *
 * @addtogroup RTC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_RTC || defined(__DOXYGEN__)

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

/**
 * @brief   Enable RTC clock.
 */
#define _rtc_clk_enable()                                                     \
  LPC_RTC->CCR |= CCR_CLKEN

/**
 * @brief   Disable RTC clock.
 */
#define _rtc_clk_disable()                                                    \
    LPC_RTC->CCR &= ~CCR_CLKEN

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   RTC interrupt handler.
 *
 * @isr
 */

#if LPC17xx_RTC_USE_ALARM || defined(__DOXYGEN__)
CH_IRQ_HANDLER(Vector84) {
  uint32_t status;

  CH_IRQ_PROLOGUE();

  status = LPC_RTC->ILR;
  LPC_RTC->ILR = status;    /* Clear interrupt flag */

  if ((status & ILR_RTCALF) && (RTCD1.callback != NULL))
    RTCD1.callback(&RTCD1, RTC_EVENT_ALARM);

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enable access to registers.
 *
 * @api
 */
void rtc_lld_init(void) {

  if(LPC_RTC->RTC_AUX & RTC_AUX_RTC_OSCF) {     /* Set after RTC power was first turn. */

    LPC_RTC->CCR = CCR_CCALEN;                  /* Disable calibration and RTC clock. */
    LPC_RTC->CALIBRATION = 0;
    LPC_RTC->RTC_AUX = RTC_AUX_RTC_OSCF;        /* Clear RTC Oscillator Fail detect flag. */

    LPC_RTC->CIIR = 0;                          /* Disable Counter Increment Interrupt. */
    LPC_RTC->AMR = AMR_MASK_ALL;                /* Mask alarm interrupt. */
    LPC_RTC->ILR = ILR_RTCALF | ILR_RTCCIF;     /* Clear interrupt flags. */

    /* Set date to Saturday 01.01.2000 00:00:00 */
    LPC_RTC->SEC  = 0;
    LPC_RTC->MIN  = 0;
    LPC_RTC->HOUR = 0;
    LPC_RTC->DOM  = 1;
    LPC_RTC->MONTH = 1;
    LPC_RTC->YEAR = 2000;
    LPC_RTC->DOW = 6;
    LPC_RTC->DOY = 1;

    _rtc_clk_enable();                          /* Enable RTC clock. */
  }

#if LPC17xx_RTC_USE_ALARM
  nvicEnableVector(RTC_IRQn, CORTEX_PRIORITY_MASK(LPC17xx_RTC_IRQ_PRIORITY));
#endif
  return;
}

/**
 * @brief   Set current time.
 * @note    Fractional part will be silently ignored. There is no possibility
 *          to set it on STM32 platform.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] timespec  pointer to a @p RTCTime structure
 *
 * @api
 */
void rtc_lld_set_time(RTCDriver *rtcp, const RTCTime *timespec) {
  (void)rtcp;

  _rtc_clk_disable();                  /* Disable RTC clock. */
  LPC_RTC->SEC  = timespec->sec;
  LPC_RTC->MIN  = timespec->min;
  LPC_RTC->HOUR = timespec->hour;
  LPC_RTC->DOM  = timespec->dom;
  LPC_RTC->MONTH = timespec->month;
  LPC_RTC->YEAR = timespec->year;
  LPC_RTC->DOW = timespec->dow;
  LPC_RTC->DOY = timespec->doy;
  _rtc_clk_enable();                  /* Enable RTC clock. */

}

/**
 * @brief   Get current time.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[out] timespec pointer to a @p RTCTime structure
 *
 * @api
 */
void rtc_lld_get_time(RTCDriver *rtcp, RTCTime *timespec) {
  (void)rtcp;

  timespec->sec = LPC_RTC->SEC;
  timespec->min = LPC_RTC->MIN;
  timespec->hour = LPC_RTC->HOUR;
  timespec->dom = LPC_RTC->DOM;
  timespec->month = LPC_RTC->MONTH;
  timespec->year = LPC_RTC->YEAR;
  timespec->dow = LPC_RTC->DOW;
  timespec->doy = LPC_RTC->DOY;
}

/**
 * @brief     Set alarm time.
 *
 * @note      Default value after BKP domain reset for both comparators is 0.
 * @note      Function does not performs any checks of alarm time validity.
 *
 * @param[in] rtcp      Pointer to RTC driver structure.
 * @param[in] alarm     Alarm identifier. Can be 1 or 2.
 * @param[in] alarmspec Pointer to a @p RTCAlarm structure.
 *
 * @api
 */
void rtc_lld_set_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       const RTCAlarm *alarmspec) {
  (void)rtcp;
  (void)alarm;
  LPC_RTC->ALSEC = alarmspec->alsec;
  LPC_RTC->ALMIN = alarmspec->almin;
  LPC_RTC->ALHOUR = alarmspec->alhour;
  LPC_RTC->ALDOM = alarmspec->aldom;
  LPC_RTC->ALMON = alarmspec->almonth;
  LPC_RTC->ALYEAR = alarmspec->alyear;
  LPC_RTC->ALDOW = alarmspec->aldow;
  LPC_RTC->ALDOY = alarmspec->aldoy;

}

/**
 * @brief   Get alarm time.
 *
 * @param[in] rtcp       pointer to RTC driver structure
 * @param[in] alarm      alarm identifier
 * @param[out] alarmspec pointer to a @p RTCAlarm structure
 *
 * @api
 */
void rtc_lld_get_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       RTCAlarm *alarmspec) {
  (void)rtcp;
  (void)alarm;
  alarmspec->alsec = LPC_RTC->ALSEC;
  alarmspec->almin = LPC_RTC->ALMIN;
  alarmspec->alhour = LPC_RTC->ALHOUR;
  alarmspec->aldom = LPC_RTC->ALDOM;
  alarmspec->almonth = LPC_RTC->ALMON;
  alarmspec->alyear = LPC_RTC->ALYEAR;
  alarmspec->aldow = LPC_RTC->ALDOW;
  alarmspec->aldoy = LPC_RTC->ALDOY;

}

/**
 * @brief   Enables or disables RTC callbacks.
 * @details This function enables or disables callbacks, use a @p NULL pointer
 *          in order to disable a callback.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] callback  callback function pointer or @p NULL
 *
 * @notapi
 */
void rtc_lld_set_callback(RTCDriver *rtcp, rtccb_t callback) {

  LPC_RTC->AMR = AMR_MASK_ALL;                /* Mask alarm interrupt. */
  LPC_RTC->ILR = ILR_RTCALF;                  /* Clear interrupt flag. */

  if (callback != NULL) {

    /* IRQ sources enabled only after setting up the callback.*/
    rtcp->callback = callback;
    LPC_RTC->AMR = 0;           /* Enable alarm interrupt. */
  }
  else {

    /* Callback set to NULL only after disabling the IRQ sources.*/
    rtcp->callback = NULL;
  }
}

#include "chrtclib.h"

/**
 * @brief   Get current time in format suitable for usage in FatFS.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @return              FAT time value.
 *
 * @api
 */
uint32_t rtc_lld_get_time_fat(RTCDriver *rtcp) {
  uint32_t fattime;
  struct tm timp;

  rtcGetTimeTm(rtcp, &timp);

  fattime  = (timp.tm_sec)       >> 1;
  fattime |= (timp.tm_min)       << 5;
  fattime |= (timp.tm_hour)      << 11;
  fattime |= (timp.tm_mday)      << 16;
  fattime |= (timp.tm_mon + 1)   << 21;
  fattime |= (timp.tm_year - 80) << 25;

  return fattime;
}

#endif /* HAL_USE_RTC */

/** @} */

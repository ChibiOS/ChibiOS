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
 * @file    STM32/RTCv1/rtc_lld.c
 * @brief   STM32 RTC subsystem low level driver header.
 *
 * @addtogroup RTC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_RTC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief RTC driver identifier.
 */
RTCDriver RTCD1;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] rtcp    pointer to a @p RTCDriver object
 *
 * @notapi
 */
static void rtc_lld_serve_interrupt(RTCDriver *rtcp) {

  chSysLockFromIsr();
  rtcp->rtc_cb(rtcp, RTC_EVENT_SECOND);
  rtcp->rtc_cb(rtcp, RTC_EVENT_ALARM);
  rtcp->rtc_cb(rtcp, RTC_EVENT_OVERFLOW);
  chSysUnlockFromIsr();
}



/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   RTC interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(RTC_IRQHandler) {

  CH_IRQ_PROLOGUE();

  rtc_lld_serve_interrupt(&RTCD1);

  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enable access to registers and initialize RTC if BKP domain
 *          was previously reseted.
 * @note:   Cold start time of LSE oscillator on STM32 platform 
 *          takes about 3 seconds.
 *
 * @notapi
 */
void rtc_lld_init(void){
  uint32_t preload;

  /* Enables access to BKP registers.*/
  PWR->CR |= PWR_CR_DBP;

  /* If the RTC is not enabled then performs a reset of the backup domain.*/
  if (!(RCC->BDCR & RCC_BDCR_RTCEN)) {
    RCC->BDCR = RCC_BDCR_BDRST;
    RCC->BDCR = 0;
  }

#if STM32_RTC == STM32_RTC_LSE
  if (!(RCC->BDCR & RCC_BDCR_LSEON)) {
    RCC->BDCR |= RCC_BDCR_LSEON;
    while (!(RCC->BDCR & RCC_BDCR_LSERDY))
      ;
  }
  preload = STM32_LSECLK - 1;
#elif STM32_RTC == STM32_RTC_LSI
  /* TODO: Move the LSI clock initialization in the HAL low level driver.*/
  RCC->CSR |= RCC_CSR_LSION;
  while (!(RCC->CSR & RCC_CSR_LSIRDY))
    ;
  /* According to errata sheet we must wait additional 100 uS for
     stabilization.
     TODO: Change this code, software loops are not reliable.*/
  uint32_t tmo = (STM32_SYSCLK / 1000000) * 100;
  while (tmo--)
    ;
  preload = STM32_LSICLK - 1;
#elif STM32_RTC == STM32_RTC_HSE
  preload = (STM32_HSICLK / 128) - 1;
#endif

  /* Selects clock source (previously enabled and stabilized).*/
  RCC->BDCR = (RCC->BDCR & ~RCC_BDCR_RTCSEL) | STM32_RTC;

  /* RTC enabled regardless its previous status.*/
  RCC->BDCR |= RCC_BDCR_RTCEN;

  /* Calendar not init yet. */
  if (!(RTC->ISR & RTC_ISR_INITS)){
    /* Disable write protection on RTC registers. */
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    /* Enter in init mode. */
    RTC->ISR |= RTC_ISR_INIT;
    while(!(RTC->ISR & RTC_ISR_INITF))
      ;
    /* Prescaler registers must be written in by two separate writes. */
    RTC->PRER = 0;
    RTC->PRER = 0x007F00FF;
    /* Wait until calendar data will updated. */
    while(!(RTC->ISR & RTC_ISR_RSF))
      ;
  }

  /* Callback initially disabled.*/
  RTCD1.rtc_cb = NULL;
}

/**
 * @brief   Set current time.
 * @note    Fractional part will be silently ignored. There is no possibility
 *          to change it on STM32F1xx platform.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] timespec  pointer to a @p RTCTime structure
 *
 * @notapi
 */
void rtc_lld_set_time(RTCDriver *rtcp, const RTCTime *timespec) {
  (void)rtcp;

  RTC->ISR |= RTC_ISR_INIT;
  while(!(RTC->ISR & RTC_ISR_INITF))
    ;

  RTC->TR = timespec->tv_time;
  RTC->DR = timespec->tv_date;

  /* Wait until calendar data will updated. */
  while(!(RTC->ISR & RTC_ISR_RSF))
    ;
}

/**
 * @brief   Get current time.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[out] timespec pointer to a @p RTCTime structure
 *
 * @notapi
 */
void rtc_lld_get_time(RTCDriver *rtcp, RTCTime *timespec) {
  (void)rtcp;

  /*  TODO: If the frequency of the APB1 clock is less than seven times
   * the frequency of RTCCLK, BYPSHAD must be set to ‘1’ .*/

  /* Wait until calendar data will updated. */
    while(!(RTC->ISR & RTC_ISR_RSF))
      ;

  timespec->tv_time = RTC->TR;
  timespec->tv_date = RTC->DR;
}

/**
 * @brief   Set alarm time.
 *
 * @note      Default value after BKP domain reset is 0xFFFFFFFF
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] alarm     alarm identifier
 * @param[in] alarmspec pointer to a @p RTCAlarm structure
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
 * @brief   Get current alarm.
 * @note    If an alarm has not been set then the returned alarm specification
 *          is not meaningful.
 *
 * @note    Default value after BKP domain reset is 0xFFFFFFFF.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] alarm     alarm identifier
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
  if (callback != NULL) {
    rtcp->rtc_cb = callback;
  }
  return;
}

#endif /* HAL_USE_RTC */

/** @} */

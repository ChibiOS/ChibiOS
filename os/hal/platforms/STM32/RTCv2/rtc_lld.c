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
*/
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    STM32/RTCv2/rtc_lld.c
 * @brief   STM32L1xx/STM32F2xx/STM32F4xx RTC low level driver header.
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

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

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
  RTCD1.id_rtc = RTC;

  /* Asynchronous part of preloader. Set it to maximum value. */
  #define PREDIV_A ((uint32_t)0x7F)

  /* Add async part to preload value. */
  volatile uint32_t preload = PREDIV_A << 16;

  /* Enables access to BKP registers.*/
  PWR->CR |= PWR_CR_DBP;

  /* If the RTC is not enabled then performs a reset of the backup domain.*/
  if (!(RCC->BDCR & RCC_BDCR_RTCEN)) {
    RCC->BDCR = RCC_BDCR_BDRST;
    RCC->BDCR = 0;
  }

#if STM32_RTC == STM32_RTC_LSE
  #define RTC_CLK   STM32_LSECLK
  if (!(RCC->BDCR & RCC_BDCR_LSEON)) {
    RCC->BDCR |= RCC_BDCR_LSEON;
    while (!(RCC->BDCR & RCC_BDCR_LSERDY))
      ;
  }

#elif STM32_RTC == STM32_RTC_LSI
  #define RTC_CLK   STM32_LSICLK
  /* TODO: Move the LSI clock initialization in the HAL low level driver.*/
  RCC->CSR |= RCC_CSR_LSION;
  while (!(RCC->CSR & RCC_CSR_LSIRDY))
    ;

#elif STM32_RTC == STM32_RTC_HSE
  #define RTC_CLK   (STM32_HSICLK / 31)
#endif

  /* Add sync part to preload value. */
  preload |= ((RTC_CLK / (PREDIV_A + 1)) - 1) & 0x7FFF;

  /* Selects clock source (previously enabled and stabilized).*/
  RCC->BDCR = (RCC->BDCR & ~RCC_BDCR_RTCSEL) | STM32_RTCSEL;

  /* RTC enabled regardless its previous status.*/
  RCC->BDCR |= RCC_BDCR_RTCEN;

  /* Disable write protection on RTC registers. */
  RTCD1.id_rtc->WPR = 0xCA;
  RTCD1.id_rtc->WPR = 0x53;

  /* If calendar not init yet. */
  if (!(RTC->ISR & RTC_ISR_INITS)){
    /* Enter in init mode. */
    RTCD1.id_rtc->ISR |= RTC_ISR_INIT;
    while(!(RTC->ISR & RTC_ISR_INITF))
      ;
    /* Prescaler registers must be written in by two separate writes. */
    RTCD1.id_rtc->PRER = preload;
    RTCD1.id_rtc->PRER = preload;
    RTCD1.id_rtc->ISR &= ~RTC_ISR_INIT;
  }
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

  RTCD1.id_rtc->ISR |= RTC_ISR_INIT;
  while(!(RTC->ISR & RTC_ISR_INITF))
    ;
  RTCD1.id_rtc->TR = timespec->tv_time;
  RTCD1.id_rtc->DR = timespec->tv_date;
  RTCD1.id_rtc->ISR &= ~RTC_ISR_INIT;
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

#if STM32_RTC_HAS_SUBSECONDS
  timespec->tv_msec = (1000 * ((RTCD1.id_rtc->PRER & 0x7FFF) - RTCD1.id_rtc->SSR)) /
                      ((RTCD1.id_rtc->PRER & 0x7FFF) + 1);
#endif /* STM32_RTC_HAS_SUBSECONDS */
  timespec->tv_time = RTCD1.id_rtc->TR;
  timespec->tv_date = RTCD1.id_rtc->DR;
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
 * @notapi
 */
void rtc_lld_set_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       const RTCAlarm *alarmspec) {
  if (alarm == 1){
    rtcp->id_rtc->CR &= ~RTC_CR_ALRAE;
    while(!(rtcp->id_rtc->ISR & RTC_ISR_ALRAWF))
      ;
    rtcp->id_rtc->ALRMAR = alarmspec->tv_datetime;
    rtcp->id_rtc->CR |= RTC_CR_ALRAE;
  }
  else{
    rtcp->id_rtc->CR &= ~RTC_CR_ALRBE;
    while(!(rtcp->id_rtc->ISR & RTC_ISR_ALRBWF))
      ;
    rtcp->id_rtc->ALRMAR = alarmspec->tv_datetime;
    rtcp->id_rtc->CR |= RTC_CR_ALRBE;
  }
}

/**
 * @brief   Get alarm time.
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
  if (alarm == 1)
    alarmspec->tv_datetime = rtcp->id_rtc->ALRMAR;
  else
    alarmspec->tv_datetime = rtcp->id_rtc->ALRMBR;
}

/**
 * @brief     Sets time of periodic wakeup.
 *
 * @note      Default value after BKP domain reset is 0x0000FFFF
 *
 * @param[in] rtcp       pointer to RTC driver structure
 * @param[in] wakeupspec pointer to a @p RTCWakeup structure
 *
 * @notapi
 */
void rtc_lld_set_periodic_wakeup(RTCDriver *rtcp, RTCWakeup *wakeupspec){
  chDbgCheck((wakeupspec->wakeup != 0x30000),
              "rtc_lld_set_periodic_wakeup, forbidden combination");

  rtcp->id_rtc->CR &= ~RTC_CR_WUTE;
  while(!(rtcp->id_rtc->ISR & RTC_ISR_WUTWF))
    ;
  rtcp->id_rtc->WUTR = wakeupspec->wakeup & 0xFFFF;
  rtcp->id_rtc->CR   = (wakeupspec->wakeup >> 16) & 0x7;
  rtcp->id_rtc->CR |= RTC_CR_WUTE;
}

/**
 * @brief     Gets time of periodic wakeup.
 *
 * @note      Default value after BKP domain reset is 0x0000FFFF
 *
 * @param[in] rtcp        pointer to RTC driver structure
 * @param[out] wakeupspec pointer to a @p RTCWakeup structure
 *
 * @notapi
 */
void rtc_lld_get_periodic_wakeup(RTCDriver *rtcp, RTCWakeup *wakeupspec){
  wakeupspec->wakeup  = 0;
  wakeupspec->wakeup |= rtcp->id_rtc->WUTR;
  wakeupspec->wakeup |= (((uint32_t)rtcp->id_rtc->CR) & 0x7) << 16;
}

#if RTC_SUPPORTS_CALLBACKS

/**
 * @brief   Enables or disables RTC callbacks.
 * @details To enable interrupt set corresponding bit in @p RTCCallbackConfig
 *          structure. To disable interrupt clear that bit.
 * @note    This function just enable/disable interrupts in RTC CR register.
 *          You must configure callbacks in EXTI driver for corresponding
 *          interrupts. See documentation for you MCU.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] cb_cfg    pointer to configuration structure with callbacks
 *
 * @notapi
 */
void rtc_lld_set_callback(RTCDriver *rtcp, RTCCallbackConfig *cb_cfg) {

  if (cb_cfg->cb_cfg & ALARMA_CB_FLAG)
    rtcp->id_rtc->CR |= RTC_CR_ALRAIE;
  else
    rtcp->id_rtc->CR &= ~RTC_CR_ALRAIE;

  if (cb_cfg->cb_cfg & ALARMB_CB_FLAG)
    rtcp->id_rtc->CR |= RTC_CR_ALRBIE;
  else
    rtcp->id_rtc->CR &= ~RTC_CR_ALRBIE;

  if (cb_cfg->cb_cfg & WAKEUP_CB_FLAG)
    rtcp->id_rtc->CR |= RTC_CR_WUTIE;
  else
    rtcp->id_rtc->CR &= ~RTC_CR_WUTIE;

  if (cb_cfg->cb_cfg & TIMESTAMP_CB_FLAG)
    rtcp->id_rtc->CR |= RTC_CR_TSIE;
  else
    rtcp->id_rtc->CR &= ~RTC_CR_TSIE;
}

/**
 * @brief               Gets current RTC callbacks.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[out] cb_cfg   callback bitmask
 *
 * @notapi
 */
void rtc_lld_get_callback(RTCDriver *rtcp, RTCCallbackConfig *cb_cfg) {
  cb_cfg->cb_cfg = rtcp->cb_cfg->cb_cfg;
}

#endif /* RTC_SUPPORTS_CALLBACKS */

#endif /* HAL_USE_RTC */

/** @} */

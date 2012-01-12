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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
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

  PWR->CR |= PWR_CR_DBP;
  chSysLockFromIsr();

  if ((RTC->CRH & RTC_CRH_SECIE) && (RTC->CRL & RTC_CRL_SECF)) {
    rtcp->callback(rtcp, RTC_EVENT_SECOND);
    RTC->CRL &= ~RTC_CRL_SECF;
  }
  if ((RTC->CRH & RTC_CRH_ALRIE) && (RTC->CRL & RTC_CRL_ALRF)) {
    rtcp->callback(rtcp, RTC_EVENT_ALARM);
    RTC->CRL &= ~RTC_CRL_ALRF;
  }
  if ((RTC->CRH & RTC_CRH_OWIE) && (RTC->CRL & RTC_CRL_OWF)) {
    rtcp->callback(rtcp, RTC_EVENT_OVERFLOW);
    RTC->CRL &= ~RTC_CRL_OWF;
  }

  chSysUnlockFromIsr();
  PWR->CR &= ~PWR_CR_DBP;
}

/**
 * @brief   Acquire atomic write access to RTC registers.
 *
 * @notapi
 */
static void rtc_lld_acquire(void) {

  /* Waits registers write completion.*/
BEGIN:
  while ((RTC->CRL & RTC_CRL_RTOFF) == 0)
    ;
  chSysLock();
  if ((RTC->CRL & RTC_CRL_RTOFF) == 0){
    chSysUnlock();
    goto BEGIN;
  }
}

/**
 * @brief   Release atomic write access to RTC registers.
 *
 * @notapi
 */
#define rtc_lld_release() {chSysUnlock();}

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

  PWR->CR |= PWR_CR_DBP;

  /* Ensure that RTC_CNT and RTC_DIV contain actual values after enabling
     clocking on APB1, because these values only update when APB1
     functioning.*/
  RTC->CRL &= ~(RTC_CRL_OWF | RTC_CRL_ALRF | RTC_CRL_SECF);
  while (!(RTC->CRL & RTC_CRL_RSF))
    ;

  /* Write preload register only if its value is not equal to desired value.*/
  if (STM32_RTCCLK != (((uint32_t)(RTC->PRLH)) << 16) +
                       ((uint32_t)RTC->PRLL) + 1) {

    /* Enters configuration mode and writes PRLx registers then leaves the
       configuration mode.*/
    rtc_lld_acquire();
    RTC->CRL |= RTC_CRL_CNF;
    RTC->PRLH = (uint16_t)((STM32_RTCCLK - 1) >> 16);
    RTC->PRLL = (uint16_t)((STM32_RTCCLK - 1) & 0xFFFF);
    RTC->CRL &= ~RTC_CRL_CNF;
    rtc_lld_release();
  }

  /* All interrupts initially disabled.*/
  if (RTC->CRH != 0){
    rtc_lld_acquire();
    RTC->CRH = 0;
    rtc_lld_release();
  }

  PWR->CR &= ~PWR_CR_DBP;

  /* Callback initially disabled.*/
  RTCD1.callback = NULL;
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

  PWR->CR |= PWR_CR_DBP;
  rtc_lld_acquire();
  RTC->CRL |= RTC_CRL_CNF;
  RTC->CNTH = (uint16_t)(timespec->tv_sec >> 16);
  RTC->CNTL = (uint16_t)(timespec->tv_sec & 0xFFFF);
  RTC->CRL &= ~RTC_CRL_CNF;
  rtc_lld_release();
  PWR->CR &= ~PWR_CR_DBP;
}

/**
 * @brief   Get current time.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] timespec  pointer to a @p RTCTime structure
 *
 * @notapi
 */
void rtc_lld_get_time(RTCDriver *rtcp, RTCTime *timespec) {
  (void)rtcp;

  uint32_t time_frac;

  /* The read is repeated until we are able to do it twice and obtain the
     same result.*/
  do {
    timespec->tv_sec = ((uint32_t)(RTC->CNTH) << 16) + RTC->CNTL;
    time_frac = (((uint32_t)RTC->DIVH) << 16) + (uint32_t)RTC->DIVL;
  } while ((timespec->tv_sec) != (((uint32_t)(RTC->CNTH) << 16) + RTC->CNTL));

  timespec->tv_msec = (uint16_t)(((STM32_RTCCLK - 1 - time_frac) * 1000) /
                                 STM32_RTCCLK);
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

  /* Enters configuration mode and writes ALRHx registers then leaves the
     configuration mode.*/
  PWR->CR |= PWR_CR_DBP;
  rtc_lld_acquire();
  RTC->CRL |= RTC_CRL_CNF;
  if (alarmspec != NULL) {
    RTC->ALRH = (uint16_t)(alarmspec->tv_sec >> 16);
    RTC->ALRL = (uint16_t)(alarmspec->tv_sec & 0xFFFF);
  }
  else {
    RTC->ALRH = 0;
    RTC->ALRL = 0;
  }
  RTC->CRL &= ~RTC_CRL_CNF;
  rtc_lld_release();
  PWR->CR &= ~PWR_CR_DBP;
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

  alarmspec->tv_sec = ((RTC->ALRH << 16) + RTC->ALRL);
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

  PWR->CR |= PWR_CR_DBP;
  rtc_lld_acquire();
  if (callback != NULL) {
    rtcp->callback = callback;

    /* Interrupts are enabled only after setting up the callback, this
       way there is no need to check for the NULL callback pointer inside
       the IRQ handler.*/
    RTC->CRL &= ~(RTC_CRL_OWF | RTC_CRL_ALRF | RTC_CRL_SECF);
    nvicEnableVector(RTC_IRQn, CORTEX_PRIORITY_MASK(STM32_RTC_IRQ_PRIORITY));
    RTC->CRH |= RTC_CRH_OWIE | RTC_CRH_ALRIE | RTC_CRH_SECIE;
  }
  else {
    nvicDisableVector(RTC_IRQn);
    RTC->CRL &= ~(RTC_CRL_OWF | RTC_CRL_ALRF | RTC_CRL_SECF);
    RTC->CRH &= ~(RTC_CRH_OWIE | RTC_CRH_ALRIE | RTC_CRH_SECIE);
  }
  rtc_lld_release();
  PWR->CR &= ~PWR_CR_DBP;
}

#endif /* HAL_USE_RTC */

/** @} */

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

  if ((RTC->CRH & RTC_CRH_SECIE) && (RTC->CRL & RTC_CRL_SECF)) {
	RTC->CRL &= ~RTC_CRL_SECF;
    rtcp->rtc_cb(rtcp, RTC_EVENT_SECOND);
  }
  if ((RTC->CRH & RTC_CRH_ALRIE) && (RTC->CRL & RTC_CRL_ALRF)) {
	RTC->CRL &= ~RTC_CRL_ALRF;
	rtcp->rtc_cb(rtcp, RTC_EVENT_ALARM);
  }
  if ((RTC->CRH & RTC_CRH_OWIE) && (RTC->CRL & RTC_CRL_OWF)) {
	RTC->CRL &= ~RTC_CRL_OWF;
    rtcp->rtc_cb(rtcp, RTC_EVENT_OVERFLOW);
  }

  chSysUnlockFromIsr();
}

/**
 * @brief   Waits for the previous registers write to finish.
 *
 * @notapi
 */
static void rtc_lld_wait_write(void) {

  /* Waits registers write completion.*/
  while (!(RTC->CRL & RTC_CRL_RTOFF))
    ;
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

  rccEnableBKPInterface(FALSE);

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
  /* According to errata sheet we must wait additional 100 uS for
     stabilization.
     TODO: Change this code, software loops are not reliable.*/
  volatile uint32_t tmo = (STM32_SYSCLK / 1000000) * 100;
  while (tmo--)
    ;
#elif STM32_RTC == STM32_RTC_HSE
#define RTC_CLK   (STM32_HSECLK / 128)
#endif

  preload = RTC_CLK - 1;

  /* Selects clock source (previously enabled and stabilized).*/
  RCC->BDCR = (RCC->BDCR & ~RCC_BDCR_RTCSEL) | STM32_RTC;

  /* RTC enabled regardless its previous status.*/
  RCC->BDCR |= RCC_BDCR_RTCEN;

  /* Ensure that RTC_CNT and RTC_DIV contain actual values after enabling
     clocking on APB1, because these values only update when APB1
     functioning.*/
  RTC->CRL = 0;
  while (!(RTC->CRL & RTC_CRL_RSF))
    ;

  /* Write preload register only if its value differs.*/
  if (preload != ((((uint32_t)(RTC->PRLH)) << 16) + (uint32_t)RTC->PRLL)) {

    rtc_lld_wait_write();

    /* Enters configuration mode and writes PRLx registers then leaves the
       configuration mode.*/
    RTC->CRL |= RTC_CRL_CNF;
    RTC->PRLH = (uint16_t)(preload >> 16);
    RTC->PRLL = (uint16_t)(preload & 0xFFFF);
    RTC->CRL &= ~RTC_CRL_CNF;
  }

  /* All interrupts initially disabled.*/
  RTC->CRH = 0;

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

  rtc_lld_wait_write();

  RTC->CRL |= RTC_CRL_CNF;
  RTC->CNTH = (uint16_t)(timespec->tv_sec >> 16);
  RTC->CNTL = (uint16_t)(timespec->tv_sec & 0xFFFF);
  RTC->CRL &= ~RTC_CRL_CNF;
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

READ_REGISTERS:
  timespec->tv_sec = ((uint32_t)(RTC->CNTH) << 16) + RTC->CNTL;
  time_frac = (((uint32_t)RTC->DIVH) << 16) + (uint32_t)RTC->DIVL;

  /* If second counter updated between reading of integer and fractional parts
   * we must reread both values. */
  if((timespec->tv_sec) != (((uint32_t)(RTC->CNTH) << 16) + RTC->CNTL)){
    goto READ_REGISTERS;
  }

  timespec->tv_msec = (uint16_t)(((RTC_CLK - 1 - time_frac) * 1000) / RTC_CLK);
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

  rtc_lld_wait_write();

  /* Enters configuration mode and writes ALRHx registers then leaves the
     configuration mode.*/
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
}

/**
 * @brief   Get current alarm.
 * @note    If an alarm has not been set then the returned alarm specification
 *          is not meaningful.
 *
 * @note    Default value after BKP domain reset is 0xFFFFFFFF.
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
void rtc_lld_set_callback(RTCDriver *rtcp, RTCCallbackConfig *cb_cfg) {

  if (cb_cfg->rtc_cb != NULL) {
    rtcp->rtc_cb = cb_cfg->rtc_cb;
    /* Interrupts are enabled only after setting up the callback, this
	   way there is no need to check for the NULL callback pointer inside
	   the IRQ handler.*/
    rtc_lld_wait_write();
    RTC->CRL &= ~(RTC_CRL_OWF | RTC_CRL_ALRF | RTC_CRL_SECF);
    rtc_lld_wait_write();
    NVICEnableVector(RTC_IRQn, CORTEX_PRIORITY_MASK(STM32_RTC_IRQ_PRIORITY));
    RTC->CRH |= RTC_CRH_OWIE | RTC_CRH_ALRIE | RTC_CRH_SECIE;
  }
  else {
    NVICDisableVector(RTC_IRQn);
    rtc_lld_wait_write();
    RTC->CRL = 0;
    RTC->CRH = 0;
  }
}

#endif /* HAL_USE_RTC */

/** @} */

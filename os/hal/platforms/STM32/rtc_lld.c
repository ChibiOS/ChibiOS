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
 * @file    STM32/rtc_lld.c
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

/** @brief RTC driver identifier.*/
RTCDriver RTCD;


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
#if RTC_SUPPORTS_CALLBACKS

static void rtc_lld_serve_interrupt(RTCDriver *rtcp){
  chSysLockFromIsr();

  if ((RTC->CRH & RTC_CRH_SECIE) && \
      (RTC->CRL & RTC_CRL_SECF) && \
      (rtcp->config->second_cb != NULL)){
    rtcp->config->second_cb(rtcp);
    RTC->CRL &= ~RTC_CRL_SECF;
  }
  if ((RTC->CRH & RTC_CRH_ALRIE) && \
      (RTC->CRL & RTC_CRL_ALRF) && \
      (rtcp->config->alarm_cb != NULL)){
    rtcp->config->alarm_cb(rtcp);
    RTC->CRL &= ~RTC_CRL_ALRF;
  }
  if ((RTC->CRH & RTC_CRH_OWIE) && \
      (RTC->CRL & RTC_CRL_OWF) && \
      (rtcp->config->overflow_cb != NULL)){
    rtcp->config->overflow_cb(rtcp);
    RTC->CRL &= ~RTC_CRL_OWF;
  }

  chSysUnlockFromIsr();
}
#endif /* RTC_SUPPORTS_CALLBACKS */

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   RTC interrupt handler.
 * @isr
 */
#if RTC_SUPPORTS_CALLBACKS

CH_IRQ_HANDLER(RTC_IRQHandler) {
  CH_IRQ_PROLOGUE();
  rtc_lld_serve_interrupt(&RTCD);
  CH_IRQ_EPILOGUE();
}

#endif /* RTC_SUPPORTS_CALLBACKS */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enable access to registers and initialize RTC if BKP domain
 *          was previously reseted.
 *
 * @notapi
 */
void rtc_lld_init(void){
  RCC->APB1ENR |= (RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN);  /* enable clocking */
  PWR->CR |= PWR_CR_DBP;                                    /* enable access */

  if (!(RCC->BDCR & (RCC_BDCR_RTCEN | RCC_BDCR_LSEON))){    /* BKP domain was reseted */
    RCC->BDCR |= RTC_CLOCK_SOURCE;                          /* select clocking from LSE */
    RCC->BDCR |= RCC_BDCR_LSEON;                            /* switch LSE on */
    while(!(RCC->BDCR & RCC_BDCR_LSEON))                    /* wait for stabilization */
      ;
    RCC->BDCR |= RCC_BDCR_RTCEN;                            /* run clock */
  }

  #if defined(RTC_CLOCK_SOURCE) == defined(RCC_BDCR_RTCSEL_LSE)
    uint32_t preload = STM32_LSECLK - 1UL;
  #elif defined(RTC_CLOCK_SOURCE) == defined(RCC_BDCR_RTCSEL_LSI)
    uint32_t preload = STM32_LSICLK - 1UL;
  #elif defined(RTC_CLOCK_SOURCE) == defined(RCC_BDCR_RTCSEL_HSE)
    uint32_t preload = (STM32_HSICLK / 128UL) - 1UL;
  #else
    #error "RTC clock source not selected"
  #endif /* RTC_CLOCK_SOURCE == RCC_BDCR_RTCSEL_LSE */

  /* Write preload register only if value changed */
  if (preload != (((uint32_t)(RTC->PRLH)) << 16) + RTC->PRLH){
    while(!(RTC->CRL & RTC_CRL_RTOFF))
      ;

    RTC->CRL |= RTC_CRL_CNF;                            /* switch on configure mode */
    RTC->PRLH = (uint16_t)((preload >> 16) & 0b1111);   /* write preloader */
    RTC->PRLL = (uint16_t)(preload & 0xFFFF);
    RTC->CRL &= ~RTC_CRL_CNF;                           /* switch off configure mode */

    while(!(RTC->CRL & RTC_CRL_RTOFF))                  /* wait for completion */
      ;
  }

  /* Ensure that RTC_CNT and RTC_DIV contain actual values after enabling
   * clocking on APB1, because these values only update when APB1 functioning.*/
  RTC->CRL &= ~(RTC_CRL_RSF);
  while (!(RTC->CRL & RTC_CRL_RSF))
    ;

  /* disable all interrupts and clear all even flags just to be safe */
  RTC->CRH &= ~(RTC_CRH_OWIE | RTC_CRH_ALRIE | RTC_CRH_SECIE);
  RTC->CRL &= ~(RTC_CRL_SECF | RTC_CRL_ALRF | RTC_CRL_OWF);

  RTCD.config = NULL;
}

/**
 * @brief     Configure and start interrupt servicing routines.
 *            This function do nothing if callbacks disabled.
 *
 * @param[in] rtcp    pointer to a @p RTCDriver object
 * @param[in] rtccfgp pointer to a @p RTCDriver config object
 *
 * @notapi
 */
void rtc_lld_start(RTCDriver *rtcp, const RTCConfig *rtccfgp){
  uint16_t isr_flags = 0;

  NVICEnableVector(RTC_IRQn, CORTEX_PRIORITY_MASK(STM32_RTC_IRQ_PRIORITY));

  rtcp->config = rtccfgp;
  if (rtcp->config->overflow_cb != NULL){
    isr_flags |= RTC_CRH_OWIE;
  }
  if (rtcp->config->alarm_cb != NULL){
    isr_flags |= RTC_CRH_ALRIE;
  }
  if (rtcp->config->second_cb != NULL){
    isr_flags |= RTC_CRH_SECIE;
  }

  /* clear all event flags just to be safe */
  RTC->CRL &= ~(RTC_CRL_SECF | RTC_CRL_ALRF | RTC_CRL_OWF);
  RTC->CRH |= isr_flags;
}

/**
 * @brief   Disable interrupt servicing routines.
 *
 * @notapi
 */
void rtc_lld_stop(void){
  NVICDisableVector(RTC_IRQn);
  RTC->CRH = 0;
}


/**
 * @brief     Set current time.
 *
 * @param[in] tv_sec     time value in UNIX notation.
 *
 * @notapi
 */
void rtc_lld_set_time(uint32_t tv_sec){

  while(!(RTC->CRL & RTC_CRL_RTOFF))
    ;

  RTC->CRL |= RTC_CRL_CNF;                            /* switch on configure mode */
  RTC->CNTH = (uint16_t)((tv_sec >> 16) & 0xFFFF);    /* write time */
  RTC->CNTL = (uint16_t)(tv_sec & 0xFFFF);
  RTC->CRL &= ~RTC_CRL_CNF;                           /* switch off configure mode */

  while(!(RTC->CRL & RTC_CRL_RTOFF))                  /* wait for completion */
    ;
}

/**
 * @brief Return current time in UNIX notation.
 *
 * @notapi
 */
inline uint32_t rtc_lld_get_sec(void){
  return ((RTC->CNTH << 16) + RTC->CNTL);
}

/**
 * @brief Return fractional part of current time (milliseconds).
 *
 * @notapi
 */
inline uint16_t rtc_lld_get_msec(void){
  uint32_t time_frac = 0;
  time_frac = (((uint32_t)RTC->DIVH) << 16) + (RTC->DIVL);
  return(((STM32_LSECLK - time_frac) * 1000) / STM32_LSECLK);
}

/**
 * @brief Set alarm date in UNIX notation.
 * @note  Default value after BKP domain reset is 0xFFFFFFFF
 *
 * @notapi
 */
void rtc_lld_set_alarm(uint32_t tv_alarm){

  while(!(RTC->CRL & RTC_CRL_RTOFF))
    ;

  RTC->CRL |= RTC_CRL_CNF;                            /* switch on configure mode */
  RTC->ALRH = (uint16_t)((tv_alarm >> 16) & 0xFFFF);  /* write time */
  RTC->ALRL = (uint16_t)(tv_alarm & 0xFFFF);
  RTC->CRL &= ~RTC_CRL_CNF;                           /* switch off configure mode */

#if !(RTC_SUPPORTS_CALLBACKS)
  RTC->CRL &= ~RTC_CRL_ALRF;
  RTC->CRH |= RTC_CRH_ALRIE;
#endif /* !(RTC_SUPPORTS_CALLBACKS) */

  while(!(RTC->CRL & RTC_CRL_RTOFF))                  /* wait for completion */
    ;
}

/**
 * @brief Get current alarm date in UNIX notation.
 * @note  Default value after BKP domain reset is 0xFFFFFFFF
 *
 * @notapi
 */
inline uint32_t rtc_lld_get_alarm(void){
  return ((RTC->ALRH << 16) + RTC->ALRL);
}


#endif /* HAL_USE_RTC */

/** @} */

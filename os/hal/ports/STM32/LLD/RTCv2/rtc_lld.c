/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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
 * @file    STM32/RTCv2/rtc_lld.c
 * @brief   STM32L1xx/STM32F2xx/STM32F4xx RTC low level driver.
 *
 * @addtogroup RTC
 * @{
 */

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
 * @brief   Wait for synchronization of RTC registers with shadow registers.
 * @details This function must be invoked before trying to access RTC
 *          registers.
 *
 * @notapi
 */
static void rtc_regs_sync(void) {

  while ((RTCD1.rtc->ISR & RTC_ISR_RSF) == 0)
    ;
}

/**
 * @brief   Beginning of configuration procedure.
 *
 * @notapi
 */
static void rtc_enter_init(void) {

  RTCD1.rtc->ISR |= RTC_ISR_INIT;
  while ((RTCD1.rtc->ISR & RTC_ISR_INITF) == 0)
    ;
}

/**
 * @brief   Finalizing of configuration procedure.
 *
 * @notapi
 */
static inline void rtc_exit_init(void) {

  RTCD1.rtc->ISR &= ~RTC_ISR_INIT;
}

/**
 * @brief   Converts time from TR register encoding to timespec.
 *
 * @param[in] tr        TR register value
 * @param[out] timespec pointer to a @p RTCDateTime structure
 *
 * @notapi
 */
static void rtc_decode_time(uint32_t tr, RTCDateTime *timespec) {

}

/**
 * @brief   Converts date from DR register encoding to timespec.
 *
 * @param[in] dr        DR register value
 * @param[out] timespec pointer to a @p RTCDateTime structure
 *
 * @notapi
 */
static void rtc_decode_date(uint32_t dr, RTCDateTime *timespec) {

}

/**
 * @brief   Converts time from timespec to TR register encoding.
 *
 * @param[in] timespec  pointer to a @p RTCDateTime structure
 * @return              the TR register encoding.
 *
 * @notapi
 */
static uint32_t rtc_encode_time(const RTCDateTime *timespec) {
  uint32_t n, tr = 0;

  /* Subseconds cannot be set.*/
  n = timespec->millisecond / 1000;

  /* Seconds conversion.*/
  tr = tr | ((n % 10) << RTC_TR_SU_OFFSET);
  n /= 10;
  tr = tr | ((n % 6) << RTC_TR_ST_OFFSET);
  n /= 6;

  /* Minutes conversion.*/
  tr = tr | ((n % 10) << RTC_TR_MNU_OFFSET);
  n /= 10;
  tr = tr | ((n % 6) << RTC_TR_MNT_OFFSET);
  n /= 6;

  /* Hours conversion.*/
  tr = tr | ((n % 10) << RTC_TR_HU_OFFSET);
  n /= 10;
  tr = tr | (n << RTC_TR_HT_OFFSET);

  return tr;
}

/**
 * @brief   Converts a date from timespec to DR register encoding.
 *
 * @param[in] timespec  pointer to a @p RTCDateTime structure
 * @return              the DR register encoding.
 *
 * @notapi
 */
static uint32_t rtc_encode_date(const RTCDateTime *timespec) {
  uint32_t n, dr = 0;

  /* Year conversion. Note, only years last two digits are considered.*/
  n = 1980 + timespec->year;
  dr = dr | ((n % 10) << RTC_DR_YU_OFFSET);
  n /= 10;
  dr = dr | ((n % 10) << RTC_DR_YT_OFFSET);

  /* Months conversion.*/
  n = timespec->month;
  dr = dr | ((n % 10) << RTC_DR_MU_OFFSET);
  n /= 10;
  dr = dr | ((n % 10) << RTC_DR_MT_OFFSET);

  /* Days conversion.*/
  n = timespec->day;
  dr = dr | ((n % 10) << RTC_DR_DU_OFFSET);
  n /= 10;
  dr = dr | ((n % 10) << RTC_DR_DT_OFFSET);

  /* Days of week conversion.*/
  dr = dr | (timespec->dayofweek << RTC_DR_WDU_OFFSET);

  return dr;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enable access to registers.
 *
 * @api
 */
void rtc_lld_init(void) {

  /* RTC pointer initialization.*/
  RTCD1.rtc = RTC;

  /* Disable write protection. */
  RTCD1.rtc->WPR = 0xCA;
  RTCD1.rtc->WPR = 0x53;

  /* If calendar has not been initialized yet then proceed with the
     initial setup.*/
  if (!(RTCD1.rtc->ISR & RTC_ISR_INITS)) {
    rtc_enter_init();

    RTCD1.rtc->CR   = 0;
    RTCD1.rtc->PRER = STM32_RTC_PRES_VALUE;
    RTCD1.rtc->PRER = STM32_RTC_PRES_VALUE;

    rtc_exit_init();
  }
}

/**
 * @brief   Set current time.
 * @note    Fractional part will be silently ignored. There is no possibility
 *          to set it on STM32 platform.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] timespec  pointer to a @p RTCDateTime structure
 *
 * @api
 */
void rtc_lld_set_time(RTCDriver *rtcp, const RTCDateTime *timespec) {
  uint32_t dr, tr;

  tr = rtc_encode_time(timespec);
  dr = rtc_encode_date(timespec);

  rtc_regs_sync();

  rtc_enter_init();

  rtcp->rtc->TR = tr;
  rtcp->rtc->DR = dr;

  rtc_exit_init();
}

/**
 * @brief   Get current time.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[out] timespec pointer to a @p RTCDateTime structure
 *
 * @api
 */
void rtc_lld_get_time(RTCDriver *rtcp, RTCDateTime *timespec) {
  uint32_t subs;

  rtc_regs_sync();

#if STM32_RTC_HAS_SUBSECONDS
  subs = (1000 * ((rtcp->rtc->PRER & 0x7FFF) - rtcp->rtc->SSR)) /
         ((rtcp->rtc->PRER & 0x7FFF) + 1);
#else
  subs = 0;
#endif /* STM32_RTC_HAS_SUBSECONDS */
/*  timespec->tv_time = rtcp->rtc->TR;
  timespec->tv_date = rtcp->rtc->DR;*/
}

#if (STM32_RTC_NUM_ALARMS > 0) || defined(__DOXYGEN__)
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
  if (alarm == 1){
    if (alarmspec != NULL){
      rtcp->id_rtc->CR &= ~RTC_CR_ALRAE;
      while(!(rtcp->id_rtc->ISR & RTC_ISR_ALRAWF))
        ;
      rtcp->id_rtc->ALRMAR = alarmspec->tv_datetime;
      rtcp->id_rtc->CR |= RTC_CR_ALRAE;
      rtcp->id_rtc->CR |= RTC_CR_ALRAIE;
    }
    else {
      rtcp->id_rtc->CR &= ~RTC_CR_ALRAIE;
      rtcp->id_rtc->CR &= ~RTC_CR_ALRAE;
    }
  }
  else{
    if (alarmspec != NULL){
      rtcp->id_rtc->CR &= ~RTC_CR_ALRBE;
      while(!(rtcp->id_rtc->ISR & RTC_ISR_ALRBWF))
        ;
      rtcp->id_rtc->ALRMBR = alarmspec->tv_datetime;
      rtcp->id_rtc->CR |= RTC_CR_ALRBE;
      rtcp->id_rtc->CR |= RTC_CR_ALRBIE;
    }
    else {
      rtcp->id_rtc->CR &= ~RTC_CR_ALRBIE;
      rtcp->id_rtc->CR &= ~RTC_CR_ALRBE;
    }
  }
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
  if (alarm == 1)
    alarmspec->tv_datetime = rtcp->id_rtc->ALRMAR;
  else
    alarmspec->tv_datetime = rtcp->id_rtc->ALRMBR;
}
#endif /* STM32_RTC_NUM_ALARMS > 0 */

#endif /* HAL_USE_RTC */

/** @} */

/*
    ChibiOS/HAL - Copyright (C) 2006-2014 Giovanni Di Sirio

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

#define RTC_TR_PM_OFFSET                    22
#define RTC_TR_HT_OFFSET                    20
#define RTC_TR_HU_OFFSET                    16
#define RTC_TR_MNT_OFFSET                   12
#define RTC_TR_MNU_OFFSET                   8
#define RTC_TR_ST_OFFSET                    4
#define RTC_TR_SU_OFFSET                    0

#define RTC_DR_YT_OFFSET                    20
#define RTC_DR_YU_OFFSET                    16
#define RTC_DR_WDU_OFFSET                   13
#define RTC_DR_MT_OFFSET                    12
#define RTC_DR_MU_OFFSET                    8
#define RTC_DR_DT_OFFSET                    4
#define RTC_DR_DU_OFFSET                    0

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
  uint32_t n;

  n  = ((tr >> RTC_TR_HT_OFFSET) & 3)   * 36000000;
  n += ((tr >> RTC_TR_HU_OFFSET) & 15)  * 3600000;
  n += ((tr >> RTC_TR_MNT_OFFSET) & 7)  * 600000;
  n += ((tr >> RTC_TR_MNU_OFFSET) & 15) * 60000;
  n += ((tr >> RTC_TR_ST_OFFSET) & 7)   * 10000;
  n += ((tr >> RTC_TR_SU_OFFSET) & 15)  * 1000;
  timespec->millisecond = n;
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

  timespec->year  = (((dr >> RTC_DR_YT_OFFSET) & 15) * 10) +
                     ((dr >> RTC_DR_YU_OFFSET) & 15);
  timespec->month = (((dr >> RTC_TR_MNT_OFFSET) & 1) * 10) +
                     ((dr >> RTC_TR_MNU_OFFSET) & 15);
  timespec->day   = (((dr >> RTC_DR_DT_OFFSET) & 3) * 10) +
                     ((dr >> RTC_DR_DU_OFFSET) & 15);
  timespec->dayofweek = (dr >> RTC_DR_WDU_OFFSET) & 1;
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
  n = timespec->year;
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
    RTCD1.rtc->ISR  = 0;
    RTCD1.rtc->PRER = STM32_RTC_PRER_BITS;
    RTCD1.rtc->PRER = STM32_RTC_PRER_BITS;

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

  /* Decoding day time, this starts the atomic read sequence, see "Reading
     the calendar" in the RTC documentation.*/
  rtc_decode_time(rtcp->rtc->TR, timespec);

  /* If the RTC is capable of sub-second counting then the value is
     normalized in milliseconds and added to the time.*/
#if STM32_RTC_HAS_SUBSECONDS
  subs = (((rtcp->rtc->SSR << 16) / STM32_RTC_PRESS_VALUE) * 1000) >> 16;
#else
  subs = 0;
#endif /* STM32_RTC_HAS_SUBSECONDS */
  timespec->millisecond += subs;

  /* Decoding date, this concludes the atomic read sequence.*/
  rtc_decode_date(rtcp->rtc->DR, timespec);
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

  if (alarm == 1) {
    if (alarmspec != NULL) {
      rtcp->rtc->CR &= ~RTC_CR_ALRAE;
      while (!(rtcp->rtc->ISR & RTC_ISR_ALRAWF))
        ;
      rtcp->rtc->ALRMAR = alarmspec->alrmr;
      rtcp->rtc->CR |= RTC_CR_ALRAE;
      rtcp->rtc->CR |= RTC_CR_ALRAIE;
    }
    else {
      rtcp->rtc->CR &= ~RTC_CR_ALRAIE;
      rtcp->rtc->CR &= ~RTC_CR_ALRAE;
    }
  }
#if STM32_RTC_NUM_ALARMS > 1
  else {
    if (alarmspec != NULL) {
      rtcp->rtc->CR &= ~RTC_CR_ALRBE;
      while (!(rtcp->rtc->ISR & RTC_ISR_ALRBWF))
        ;
      rtcp->rtc->ALRMBR = alarmspec->alrmr;
      rtcp->rtc->CR |= RTC_CR_ALRBE;
      rtcp->rtc->CR |= RTC_CR_ALRBIE;
    }
    else {
      rtcp->rtc->CR &= ~RTC_CR_ALRBIE;
      rtcp->rtc->CR &= ~RTC_CR_ALRBE;
    }
  }
#endif /* STM32_RTC_NUM_ALARMS > 1 */
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
    alarmspec->alrmr = rtcp->rtc->ALRMAR;
#if STM32_RTC_NUM_ALARMS > 1
  else
    alarmspec->alrmr = rtcp->rtc->ALRMBR;
#endif /* STM32_RTC_NUM_ALARMS > 1 */
}
#endif /* STM32_RTC_NUM_ALARMS > 0 */


#if STM32_RTC_HAS_PERIODIC_WAKEUPS || defined(__DOXYGEN__)
/**
 * @brief     Sets time of periodic wakeup.
 *
 * @note      Default value after BKP domain reset is 0x0000FFFF
 *
 * @param[in] rtcp       pointer to RTC driver structure
 * @param[in] wakeupspec pointer to a @p RTCWakeup structure
 *
 * @api
 */
void rtcSTM32SetPeriodicWakeup(RTCDriver *rtcp, const RTCWakeup *wakeupspec) {

  if (wakeupspec != NULL) {
    osalDbgCheck(wakeupspec->wutr != 0x30000);

    rtcp->rtc->CR &= ~RTC_CR_WUTE;
    while (!(rtcp->rtc->ISR & RTC_ISR_WUTWF))
      ;
    rtcp->rtc->WUTR = wakeupspec->wutr & 0xFFFF;
    rtcp->rtc->CR   = (wakeupspec->wutr >> 16) & 0x7;
    rtcp->rtc->CR |= RTC_CR_WUTIE;
    rtcp->rtc->CR |= RTC_CR_WUTE;
  }
  else {
    rtcp->rtc->CR &= ~RTC_CR_WUTIE;
    rtcp->rtc->CR &= ~RTC_CR_WUTE;
  }
}

/**
 * @brief     Gets time of periodic wakeup.
 *
 * @note      Default value after BKP domain reset is 0x0000FFFF
 *
 * @param[in] rtcp        pointer to RTC driver structure
 * @param[out] wakeupspec pointer to a @p RTCWakeup structure
 *
 * @api
 */
void rtcSTM32GetPeriodicWakeup(RTCDriver *rtcp, RTCWakeup *wakeupspec) {

  wakeupspec->wutr  = 0;
  wakeupspec->wutr |= rtcp->rtc->WUTR;
  wakeupspec->wutr |= (((uint32_t)rtcp->rtc->CR) & 0x7) << 16;
}
#endif /* STM32_RTC_HAS_PERIODIC_WAKEUPS */

#endif /* HAL_USE_RTC */

/** @} */

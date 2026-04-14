/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    RTCv4/hal_rtc_lld.c
 * @brief   STM32 RTCv4 low level driver source.
 *
 * @addtogroup HAL_RTC
 * @{
 */

#include "hal.h"

#if HAL_USE_RTC || defined(__DOXYGEN__)

#define RTC_TR_HT_OFFSET                    RTC_TR_HT_Pos
#define RTC_TR_HU_OFFSET                    RTC_TR_HU_Pos
#define RTC_TR_MNT_OFFSET                   RTC_TR_MNT_Pos
#define RTC_TR_MNU_OFFSET                   RTC_TR_MNU_Pos
#define RTC_TR_ST_OFFSET                    RTC_TR_ST_Pos
#define RTC_TR_SU_OFFSET                    RTC_TR_SU_Pos

#define RTC_DR_YT_OFFSET                    RTC_DR_YT_Pos
#define RTC_DR_YU_OFFSET                    RTC_DR_YU_Pos
#define RTC_DR_WDU_OFFSET                   RTC_DR_WDU_Pos
#define RTC_DR_MT_OFFSET                    RTC_DR_MT_Pos
#define RTC_DR_MU_OFFSET                    RTC_DR_MU_Pos
#define RTC_DR_DT_OFFSET                    RTC_DR_DT_Pos
#define RTC_DR_DU_OFFSET                    RTC_DR_DU_Pos

#define RTC_CR_BKP_OFFSET                   RTC_CR_BKP_Pos

static const hal_rtc_config_t rtc_default_config = {
  .cr                        = STM32_RTC_CR_INIT,
  .prer                      = STM32_RTC_PRER_BITS
};

hal_rtc_driver_c RTCD1;

static void rtc_wpr_unlock(hal_rtc_driver_c *rtcp) {

  rtcp->rtc->WPR = 0xCAU;
  rtcp->rtc->WPR = 0x53U;
}

static void rtc_wpr_lock(hal_rtc_driver_c *rtcp) {

  rtcp->rtc->WPR = 0xFFU;
}

static void rtc_enter_init(hal_rtc_driver_c *rtcp) {

  rtcp->rtc->ICSR |= RTC_ICSR_INIT;
  while ((rtcp->rtc->ICSR & RTC_ICSR_INITF) == 0U) {
  }
}

static void rtc_exit_init(hal_rtc_driver_c *rtcp) {

  rtcp->rtc->ICSR &= ~RTC_ICSR_INIT;
}

static void rtc_disable_interrupt_sources(hal_rtc_driver_c *rtcp) {

#if defined(RTC_CR_ALRAIE)
  rtcp->rtc->CR &= ~RTC_CR_ALRAIE;
#endif
#if defined(RTC_CR_ALRBIE)
  rtcp->rtc->CR &= ~RTC_CR_ALRBIE;
#endif
#if defined(RTC_CR_WUTIE)
  rtcp->rtc->CR &= ~RTC_CR_WUTIE;
#endif
#if defined(RTC_CR_TSIE)
  rtcp->rtc->CR &= ~RTC_CR_TSIE;
#endif
#if defined(TAMP_CR1_TAMP1IE)
  TAMP->CR1 &= ~TAMP_CR1_TAMP1IE;
#endif
#if defined(TAMP_CR1_TAMP2IE)
  TAMP->CR1 &= ~TAMP_CR1_TAMP2IE;
#endif
#if defined(TAMP_CR1_ITAMP3IE)
  TAMP->CR1 &= ~TAMP_CR1_ITAMP3IE;
#endif
}

static void rtc_disable_irqs_exti(void) {

#if defined(STM32_RTC_TAMP_STAMP_NUMBER)
  nvicDisableVector(STM32_RTC_TAMP_STAMP_NUMBER);
#endif
#if defined(STM32_RTC_WKUP_NUMBER)
  nvicDisableVector(STM32_RTC_WKUP_NUMBER);
#endif
#if defined(STM32_RTC_ALARM_NUMBER)
  nvicDisableVector(STM32_RTC_ALARM_NUMBER);
#endif
#if defined(STM32_RTC_GLOBAL_NUMBER)
  nvicDisableVector(STM32_RTC_GLOBAL_NUMBER);
#endif
#if defined(STM32_RTC_TAMP_NUMBER)
  nvicDisableVector(STM32_RTC_TAMP_NUMBER);
#endif
#if defined(STM32_RTC_ALARM_EXTI) && defined(STM32_RTC_TAMP_STAMP_EXTI) && \
    defined(STM32_RTC_WKUP_EXTI)
  extiEnableGroup1(EXTI_MASK1(STM32_RTC_ALARM_EXTI) |
                   EXTI_MASK1(STM32_RTC_TAMP_STAMP_EXTI) |
                   EXTI_MASK1(STM32_RTC_WKUP_EXTI),
                   EXTI_MODE_DISABLED);
#elif defined(STM32_RTC_GLOBAL_EXTI) && defined(STM32_RTC_TAMP_EXTI)
  extiEnableGroup1(EXTI_MASK1(STM32_RTC_GLOBAL_EXTI) |
                   EXTI_MASK1(STM32_RTC_TAMP_EXTI),
                   EXTI_MODE_DISABLED);
#endif
  STM32_RTC_CLEAR_ALL_EXTI();
}

static void rtc_decode_time(uint32_t tr, rtc_datetime_t *timespec) {
  uint32_t n;

  n  = ((tr >> RTC_TR_HT_OFFSET) & 3U)   * 36000000U;
  n += ((tr >> RTC_TR_HU_OFFSET) & 15U)  * 3600000U;
  n += ((tr >> RTC_TR_MNT_OFFSET) & 7U)  * 600000U;
  n += ((tr >> RTC_TR_MNU_OFFSET) & 15U) * 60000U;
  n += ((tr >> RTC_TR_ST_OFFSET) & 7U)   * 10000U;
  n += ((tr >> RTC_TR_SU_OFFSET) & 15U)  * 1000U;
  timespec->millisecond = n;
}

static void rtc_decode_date(uint32_t dr, rtc_datetime_t *timespec) {

  timespec->year = (uint16_t)((((dr >> RTC_DR_YT_OFFSET) & 15U) * 10U) +
                              ((dr >> RTC_DR_YU_OFFSET) & 15U));
  timespec->month = (uint8_t)((((dr >> RTC_DR_MT_OFFSET) & 1U) * 10U) +
                              ((dr >> RTC_DR_MU_OFFSET) & 15U));
  timespec->day = (uint8_t)((((dr >> RTC_DR_DT_OFFSET) & 3U) * 10U) +
                            ((dr >> RTC_DR_DU_OFFSET) & 15U));
  timespec->dayofweek = (uint8_t)((dr >> RTC_DR_WDU_OFFSET) & 7U);
}

static uint32_t rtc_encode_time(const rtc_datetime_t *timespec) {
  uint32_t n, tr = 0U;

  n = timespec->millisecond / 1000U;
  tr |= ((n % 10U) << RTC_TR_SU_OFFSET);
  n /= 10U;
  tr |= ((n % 6U) << RTC_TR_ST_OFFSET);
  n /= 6U;
  tr |= ((n % 10U) << RTC_TR_MNU_OFFSET);
  n /= 10U;
  tr |= ((n % 6U) << RTC_TR_MNT_OFFSET);
  n /= 6U;
  tr |= ((n % 10U) << RTC_TR_HU_OFFSET);
  n /= 10U;
  tr |= (n << RTC_TR_HT_OFFSET);

  return tr;
}

static uint32_t rtc_encode_date(const rtc_datetime_t *timespec) {
  uint32_t n, dr = 0U;

  n = timespec->year;
  dr |= ((n % 10U) << RTC_DR_YU_OFFSET);
  n /= 10U;
  dr |= ((n % 10U) << RTC_DR_YT_OFFSET);

  n = timespec->month;
  dr |= ((n % 10U) << RTC_DR_MU_OFFSET);
  n /= 10U;
  dr |= ((n % 10U) << RTC_DR_MT_OFFSET);

  n = timespec->day;
  dr |= ((n % 10U) << RTC_DR_DU_OFFSET);
  n /= 10U;
  dr |= ((n % 10U) << RTC_DR_DT_OFFSET);
  dr |= ((uint32_t)timespec->dayofweek << RTC_DR_WDU_OFFSET);

  return dr;
}

static rtceventflags_t rtc_update_events(uint32_t cr, uint32_t misr) {
  rtceventflags_t flags = 0U;
  syssts_t sts;

#if defined(RTC_MISR_ALRAMF)
  if (((cr & RTC_CR_ALRAIE) != 0U) && ((misr & RTC_MISR_ALRAMF) != 0U)) {
    flags |= RTC_FLAGS_ALARM_A;
  }
#endif
#if defined(RTC_MISR_ALRBMF)
  if (((cr & RTC_CR_ALRBIE) != 0U) && ((misr & RTC_MISR_ALRBMF) != 0U)) {
    flags |= RTC_FLAGS_ALARM_B;
  }
#endif
#if defined(RTC_MISR_TSMF)
  if (((cr & RTC_CR_TSIE) != 0U) && ((misr & RTC_MISR_TSMF) != 0U)) {
    flags |= RTC_FLAGS_TS;
  }
#endif
#if defined(RTC_MISR_TSOVMF)
  if (((cr & RTC_CR_TSIE) != 0U) && ((misr & RTC_MISR_TSOVMF) != 0U)) {
    flags |= RTC_FLAGS_TS_OVF;
  }
#endif
#if defined(RTC_MISR_TAMP1MF)
  if ((misr & RTC_MISR_TAMP1MF) != 0U) {
    flags |= RTC_FLAGS_TAMP1;
  }
#endif
#if defined(RTC_MISR_TAMP2MF)
  if ((misr & RTC_MISR_TAMP2MF) != 0U) {
    flags |= RTC_FLAGS_TAMP2;
  }
#endif
#if defined(RTC_MISR_TAMP3MF)
  if ((misr & RTC_MISR_TAMP3MF) != 0U) {
    flags |= RTC_FLAGS_TAMP3;
  }
#endif
#if defined(RTC_MISR_WUTMF)
  if ((misr & RTC_MISR_WUTMF) != 0U) {
    flags |= RTC_FLAGS_WAKEUP;
  }
#endif

  if (flags != 0U) {
    sts = osalSysGetStatusAndLockX();
    RTCD1.events |= flags;
    osalSysRestoreStatusX(sts);
  }

  return flags;
}

void rtc_lld_serve_interrupt(void) {
  uint32_t cr, isr;
  rtceventflags_t flags;

  cr = RTCD1.rtc->CR;
  isr = RTCD1.rtc->MISR;
  RTCD1.rtc->SCR = isr;
  STM32_RTC_CLEAR_ALL_EXTI();
  flags = rtc_update_events(cr, isr);

  if ((flags != 0U) && (RTCD1.cb != NULL)) {
    RTCD1.cb(&RTCD1);
  }
}

void rtc_lld_init(void) {

  rtcObjectInit(&RTCD1);
  RTCD1.rtc = RTC;
}

msg_t rtc_lld_start(hal_rtc_driver_c *rtcp) {
  const hal_rtc_config_t *cfg;

  cfg = (const hal_rtc_config_t *)rtcp->config;
  if (cfg == NULL) {
    cfg = rtc_lld_selcfg(rtcp, 0U);
  }
  if (cfg == NULL) {
    return HAL_RET_CONFIG_ERROR;
  }

  rtcp->config = cfg;

  rtc_wpr_unlock(rtcp);

  if ((rtcp->rtc->ICSR & RTC_ICSR_INITS) == 0U) {
    rtc_enter_init(rtcp);
    rtcp->rtc->CR = (cfg->cr & STM32_RTC_CR_MASK) | RTC_CR_BYPSHAD;
    rtcp->rtc->PRER = cfg->prer & 0x7FFFU;
    rtcp->rtc->PRER = cfg->prer;
    rtc_exit_init(rtcp);
  }
  else {
    rtcp->rtc->CR = (rtcp->rtc->CR & ~STM32_RTC_CR_MASK) |
                    (cfg->cr & STM32_RTC_CR_MASK) |
                    RTC_CR_BYPSHAD;
  }
  rtc_wpr_lock(rtcp);

  rtcp->events = 0U;
  STM32_RTC_ENABLE_ALL_EXTI();

  return HAL_RET_SUCCESS;
}

void rtc_lld_stop(hal_rtc_driver_c *rtcp) {

  rtc_wpr_unlock(rtcp);
  rtc_disable_interrupt_sources(rtcp);
  rtc_wpr_lock(rtcp);
  rtc_disable_irqs_exti();
  rtcp->cb = NULL;
  rtcp->events = 0U;
}

const hal_rtc_config_t *rtc_lld_setcfg(hal_rtc_driver_c *rtcp,
                                       const hal_rtc_config_t *config) {
  (void)rtcp;

  if (config == NULL) {
    return rtc_lld_selcfg(rtcp, 0U);
  }

  return config;
}

const hal_rtc_config_t *rtc_lld_selcfg(hal_rtc_driver_c *rtcp, unsigned cfgnum) {
  (void)rtcp;

  if (cfgnum != 0U) {
    return NULL;
  }

  return &rtc_default_config;
}

void rtc_lld_set_callback(hal_rtc_driver_c *rtcp, drv_cb_t cb) {

  (void)rtcp;
  (void)cb;
}

msg_t rtc_lld_set_datetime(hal_rtc_driver_c *rtcp,
                           const rtc_datetime_t *timespec) {
  uint32_t tr, dr;
  syssts_t sts;

  tr = rtc_encode_time(timespec);
  dr = rtc_encode_date(timespec);

  sts = osalSysGetStatusAndLockX();
  rtc_wpr_unlock(rtcp);
  rtc_enter_init(rtcp);
  rtcp->rtc->TR = tr;
  rtcp->rtc->DR = dr;
  rtcp->rtc->CR = (rtcp->rtc->CR & ~(1U << RTC_CR_BKP_OFFSET)) |
                  ((uint32_t)timespec->dstflag << RTC_CR_BKP_OFFSET);
  rtc_exit_init(rtcp);
  rtc_wpr_lock(rtcp);
  osalSysRestoreStatusX(sts);

  return HAL_RET_SUCCESS;
}

msg_t rtc_lld_get_datetime(hal_rtc_driver_c *rtcp, rtc_datetime_t *timespec) {
  uint32_t cr, dr, tr, ssr, prev_dr, prev_tr, prev_ssr;
  uint32_t subs;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  ssr = 0U;
  tr = 0U;
  dr = 0U;
  do {
    prev_ssr = ssr;
    prev_tr = tr;
    prev_dr = dr;
    ssr = rtcp->rtc->SSR;
    tr = rtcp->rtc->TR;
    dr = rtcp->rtc->DR;
  } while ((ssr != prev_ssr) || (tr != prev_tr) || (dr != prev_dr));
  cr = rtcp->rtc->CR;
  osalSysRestoreStatusX(sts);

  rtc_decode_time(tr, timespec);
  subs = ((((rtcp->rtc->PRER & RTC_PRER_PREDIV_S_Msk) >> RTC_PRER_PREDIV_S_Pos) - ssr) * 1000U) /
         ((((rtcp->rtc->PRER & RTC_PRER_PREDIV_S_Msk) >> RTC_PRER_PREDIV_S_Pos) + 1U));
  timespec->millisecond += subs;
  rtc_decode_date(dr, timespec);
  timespec->dstflag = (uint8_t)((cr >> RTC_CR_BKP_OFFSET) & 1U);

  return HAL_RET_SUCCESS;
}

msg_t rtc_lld_set_alarm(hal_rtc_driver_c *rtcp,
                        rtcalarm_t alarm,
                        const rtc_alarm_t *alarmspec) {
  syssts_t sts;

  if (alarm >= (rtcalarm_t)RTC_ALARMS) {
    return HAL_RET_CONFIG_ERROR;
  }

  sts = osalSysGetStatusAndLockX();
  rtc_wpr_unlock(rtcp);
  if (alarm == 0U) {
    if (alarmspec != NULL) {
      rtcp->rtc->CR &= ~RTC_CR_ALRAE;
#if defined(RTC_ICSR_ALRAWF)
      while ((rtcp->rtc->ICSR & RTC_ICSR_ALRAWF) == 0U) {
      }
#endif
      rtcp->rtc->ALRMAR = alarmspec->alrmr;
      rtcp->rtc->CR |= RTC_CR_ALRAE;
      rtcp->rtc->CR |= RTC_CR_ALRAIE;
    }
    else {
      rtcp->rtc->CR &= ~RTC_CR_ALRAIE;
      rtcp->rtc->CR &= ~RTC_CR_ALRAE;
    }
  }
#if RTC_ALARMS > 1
  else {
    if (alarmspec != NULL) {
      rtcp->rtc->CR &= ~RTC_CR_ALRBE;
#if defined(RTC_ICSR_ALRBWF)
      while ((rtcp->rtc->ICSR & RTC_ICSR_ALRBWF) == 0U) {
      }
#endif
      rtcp->rtc->ALRMBR = alarmspec->alrmr;
      rtcp->rtc->CR |= RTC_CR_ALRBE;
      rtcp->rtc->CR |= RTC_CR_ALRBIE;
    }
    else {
      rtcp->rtc->CR &= ~RTC_CR_ALRBIE;
      rtcp->rtc->CR &= ~RTC_CR_ALRBE;
    }
  }
#endif
  rtc_wpr_lock(rtcp);
  osalSysRestoreStatusX(sts);

  return HAL_RET_SUCCESS;
}

msg_t rtc_lld_get_alarm(hal_rtc_driver_c *rtcp,
                        rtcalarm_t alarm,
                        rtc_alarm_t *alarmspec) {

  if (alarm >= (rtcalarm_t)RTC_ALARMS) {
    return HAL_RET_CONFIG_ERROR;
  }

  if (alarm == 0U) {
    alarmspec->alrmr = rtcp->rtc->ALRMAR;
  }
#if RTC_ALARMS > 1
  else {
    alarmspec->alrmr = rtcp->rtc->ALRMBR;
  }
#endif

  return HAL_RET_SUCCESS;
}

#endif /* HAL_USE_RTC */

/** @} */

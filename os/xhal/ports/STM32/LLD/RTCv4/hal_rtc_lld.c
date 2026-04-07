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

static void rtc_enter_init(void) {

  RTCD1.rtc->ICSR |= RTC_ICSR_INIT;
  while ((RTCD1.rtc->ICSR & RTC_ICSR_INITF) == 0U) {
  }
}

static void rtc_exit_init(void) {

  RTCD1.rtc->ICSR &= ~RTC_ICSR_INIT;
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

static void rtc_update_events(uint32_t misr) {
  rtceventflags_t flags = 0U;
  syssts_t sts;

#if defined(RTC_MISR_ALRAMF)
  if ((misr & RTC_MISR_ALRAMF) != 0U) {
    flags |= RTC_FLAGS_ALARM_A;
  }
#endif
#if defined(RTC_MISR_ALRBMF)
  if ((misr & RTC_MISR_ALRBMF) != 0U) {
    flags |= RTC_FLAGS_ALARM_B;
  }
#endif
#if defined(RTC_MISR_TSMF)
  if ((misr & RTC_MISR_TSMF) != 0U) {
    flags |= RTC_FLAGS_TS;
  }
#endif
#if defined(RTC_MISR_TSOVMF)
  if ((misr & RTC_MISR_TSOVMF) != 0U) {
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
}

static void rtc_lld_serve_interrupt(void) {
  uint32_t isr;

  isr = RTCD1.rtc->MISR;
  RTCD1.rtc->SCR = isr;
  STM32_RTC_CLEAR_ALL_EXTI();
  rtc_update_events(isr);

  if (RTCD1.cb != NULL) {
    RTCD1.cb(&RTCD1);
  }
}

#if defined(STM32_RTC_TAMP_STAMP_HANDLER) && defined(STM32_RTC_WKUP_HANDLER) && \
    defined(STM32_RTC_ALARM_HANDLER)
#include "stm32_rtc_g4.inc"
#elif defined(STM32_RTC_GLOBAL_HANDLER) && defined(STM32_RTC_TAMP_HANDLER)
#include "stm32_rtc_h5.inc"
#else
#error "unsupported RTCv4 interrupt topology"
#endif

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

  rtcp->rtc->WPR = 0xCAU;
  rtcp->rtc->WPR = 0x53U;

  if ((rtcp->rtc->ICSR & RTC_ICSR_INITS) == 0U) {
    rtc_enter_init();
    rtcp->rtc->CR = (cfg->cr & STM32_RTC_CR_MASK) | RTC_CR_BYPSHAD;
    rtcp->rtc->PRER = cfg->prer & 0x7FFFU;
    rtcp->rtc->PRER = cfg->prer;
    rtc_exit_init();
  }
  else {
    rtcp->rtc->CR = (rtcp->rtc->CR & ~STM32_RTC_CR_MASK) |
                    (cfg->cr & STM32_RTC_CR_MASK) |
                    RTC_CR_BYPSHAD;
  }

  rtcp->events = 0U;
  STM32_RTC_ENABLE_ALL_EXTI();
  STM32_RTC_IRQ_ENABLE();

  return HAL_RET_SUCCESS;
}

void rtc_lld_stop(hal_rtc_driver_c *rtcp) {

  rtcp->cb = NULL;
  rtcp->events = 0U;
}

const hal_rtc_config_t *rtc_lld_setcfg(hal_rtc_driver_c *rtcp,
                                       const hal_rtc_config_t *config) {

  if (config == NULL) {
    return rtc_lld_selcfg(rtcp, 0U);
  }

  rtcp->config_buf = *config;

  return &rtcp->config_buf;
}

const hal_rtc_config_t *rtc_lld_selcfg(hal_rtc_driver_c *rtcp, unsigned cfgnum) {

  if (cfgnum != 0U) {
    return NULL;
  }

  rtcp->config_buf = rtc_default_config;

  return &rtcp->config_buf;
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
  rtc_enter_init();
  rtcp->rtc->TR = tr;
  rtcp->rtc->DR = dr;
  rtcp->rtc->CR = (rtcp->rtc->CR & ~(1U << RTC_CR_BKP_OFFSET)) |
                  ((uint32_t)timespec->dstflag << RTC_CR_BKP_OFFSET);
  rtc_exit_init();
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
  subs = (((STM32_RTC_PRESS_VALUE - 1U) - ssr) * 1000U) /
         STM32_RTC_PRESS_VALUE;
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
    rtcp->rtc->CR &= ~RTC_CR_ALRBE;
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

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
 * @file    sandbox/hal_rtc_lld.c
 * @brief   SandBox RTC subsystem low level driver source.
 *
 * @addtogroup HAL_RTC
 * @{
 */

#include "hal.h"

#if HAL_USE_RTC || defined(__DOXYGEN__)

hal_rtc_driver_c RTCD1;

void rtc_lld_init(void) {

  rtcObjectInit(&RTCD1);
  RTCD1.nvrtc = 1U;
}

msg_t rtc_lld_start(hal_rtc_driver_c *rtcp) {

  (void)rtcp;

  return HAL_RET_HW_FAILURE;
}

void rtc_lld_stop(hal_rtc_driver_c *rtcp) {

  (void)rtcp;
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
  (void)cfgnum;

  return NULL;
}

void rtc_lld_set_callback(hal_rtc_driver_c *rtcp, drv_cb_t cb) {

  (void)rtcp;
  (void)cb;
}

msg_t rtc_lld_set_datetime(hal_rtc_driver_c *rtcp,
                           const rtc_datetime_t *timespec) {

  (void)rtcp;
  (void)timespec;

  return HAL_RET_HW_FAILURE;
}

msg_t rtc_lld_get_datetime(hal_rtc_driver_c *rtcp, rtc_datetime_t *timespec) {

  (void)rtcp;
  (void)timespec;

  return HAL_RET_HW_FAILURE;
}

msg_t rtc_lld_set_alarm(hal_rtc_driver_c *rtcp,
                        rtcalarm_t alarm,
                        const rtc_alarm_t *alarmspec) {

  (void)rtcp;
  (void)alarm;
  (void)alarmspec;

  return HAL_RET_HW_FAILURE;
}

msg_t rtc_lld_get_alarm(hal_rtc_driver_c *rtcp,
                        rtcalarm_t alarm,
                        rtc_alarm_t *alarmspec) {

  (void)rtcp;
  (void)alarm;
  (void)alarmspec;

  return HAL_RET_HW_FAILURE;
}

#endif /* HAL_USE_RTC */

/** @} */

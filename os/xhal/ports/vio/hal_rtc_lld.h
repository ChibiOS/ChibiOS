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
 * @file    sandbox/hal_rtc_lld.h
 * @brief   SandBox RTC subsystem low level driver header.
 *
 * @addtogroup HAL_RTC
 * @{
 */

#ifndef HAL_RTC_LLD_H
#define HAL_RTC_LLD_H

#if HAL_USE_RTC || defined(__DOXYGEN__)

#if !defined(VIO_RTC_USE_VRTC1) || defined(__DOXYGEN__)
#define VIO_RTC_USE_VRTC1                   FALSE
#endif

#if !VIO_RTC_USE_VRTC1
#error "RTC driver activated but no VRTC peripheral assigned"
#endif

#define RTC_ALARMS                          1U

#define rtc_lld_driver_fields                                               \
  uint32_t                  nvrtc

#define rtc_lld_config_fields

#ifdef __cplusplus
extern "C" {
#endif
  void rtc_lld_init(void);
  msg_t rtc_lld_start(hal_rtc_driver_c *rtcp);
  void rtc_lld_stop(hal_rtc_driver_c *rtcp);
  const hal_rtc_config_t *rtc_lld_setcfg(hal_rtc_driver_c *rtcp,
                                         const hal_rtc_config_t *config);
  const hal_rtc_config_t *rtc_lld_selcfg(hal_rtc_driver_c *rtcp,
                                         unsigned cfgnum);
  void rtc_lld_set_callback(hal_rtc_driver_c *rtcp, drv_cb_t cb);
  msg_t rtc_lld_set_datetime(hal_rtc_driver_c *rtcp,
                             const rtc_datetime_t *timespec);
  msg_t rtc_lld_get_datetime(hal_rtc_driver_c *rtcp,
                             rtc_datetime_t *timespec);
  msg_t rtc_lld_set_alarm(hal_rtc_driver_c *rtcp,
                          rtcalarm_t alarm,
                          const rtc_alarm_t *alarmspec);
  msg_t rtc_lld_get_alarm(hal_rtc_driver_c *rtcp,
                          rtcalarm_t alarm,
                          rtc_alarm_t *alarmspec);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_RTC */

#endif /* HAL_RTC_LLD_H */

/** @} */

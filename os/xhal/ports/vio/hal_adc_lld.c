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
 * @file    sandbox/hal_adc_lld.c
 * @brief   SandBox ADC subsystem low level driver source.
 *
 * @addtogroup HAL_ADC
 * @{
 */

#include "hal.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

ADCDriver ADCD1;

void adc_lld_init(void) {

  adcObjectInit(&ADCD1);
  ADCD1.nvadc = 1U;
}

msg_t adc_lld_start(ADCDriver *adcp) {

  (void)adcp;

  return HAL_RET_HW_FAILURE;
}

void adc_lld_stop(ADCDriver *adcp) {

  (void)adcp;
}

const hal_adc_config_t *adc_lld_setcfg(ADCDriver *adcp,
                                       const hal_adc_config_t *config) {

  if (config == NULL) {
    return adc_lld_selcfg(adcp, 0U);
  }

  adcp->config_buf = *config;

  return &adcp->config_buf;
}

const hal_adc_config_t *adc_lld_selcfg(ADCDriver *adcp, unsigned cfgnum) {

  (void)adcp;
  (void)cfgnum;

  return NULL;
}

void adc_lld_set_callback(ADCDriver *adcp, drv_cb_t cb) {

  (void)adcp;
  (void)cb;
}

msg_t adc_lld_start_conversion(ADCDriver *adcp,
                               const ADCConversionGroup *grpp,
                               adcsample_t *samples,
                               size_t depth) {

  (void)adcp;
  (void)grpp;
  (void)samples;
  (void)depth;

  return HAL_RET_HW_FAILURE;
}

void adc_lld_stop_conversion(ADCDriver *adcp) {

  (void)adcp;
}

#endif /* HAL_USE_ADC */

/** @} */

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

hal_adc_driver_c ADCD1;

static const hal_adc_config_t default_config = {
  .grps = NULL
};

void adc_lld_init(void) {

  adcObjectInit(&ADCD1);
  ADCD1.nvadc = 1U;
}

msg_t adc_lld_start(hal_adc_driver_c *adcp) {

  (void)adcp;

  return HAL_RET_HW_FAILURE;
}

void adc_lld_stop(hal_adc_driver_c *adcp) {

  (void)adcp;
}

const hal_adc_config_t *adc_lld_setcfg(hal_adc_driver_c *adcp,
                                       const hal_adc_config_t *config) {
  (void)adcp;

  if (config == NULL) {
    return adc_lld_selcfg(adcp, 0U);
  }

  return config;
}

const hal_adc_config_t *adc_lld_selcfg(hal_adc_driver_c *adcp,
                                       unsigned cfgnum) {

  (void)adcp;

  if (cfgnum != 0U) {
    return NULL;
  }

  return &default_config;
}

void adc_lld_set_callback(hal_adc_driver_c *adcp, drv_cb_t cb) {

  (void)adcp;
  (void)cb;
}

msg_t adc_lld_start_conversion(hal_adc_driver_c *adcp, unsigned grpnum,
                               adcsample_t *samples,
                               size_t depth) {
  const hal_adc_config_t *config = (const hal_adc_config_t *)adcp->config;

  (void)samples;
  (void)depth;

  if ((config == NULL) || (config->grps == NULL) ||
      (grpnum >= config->grps->grpsnum)) {
    return HAL_RET_CONFIG_ERROR;
  }

  adcp->grpp = &config->grps->grps[grpnum];

  return HAL_RET_HW_FAILURE;
}

void adc_lld_stop_conversion(hal_adc_driver_c *adcp) {

  (void)adcp;
}

#endif /* HAL_USE_ADC */

/** @} */

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
 * @file    sandbox/hal_adc_lld.h
 * @brief   SandBox ADC subsystem low level driver header.
 *
 * @addtogroup HAL_ADC
 * @{
 */

#ifndef HAL_ADC_LLD_H
#define HAL_ADC_LLD_H

#if HAL_USE_ADC || defined(__DOXYGEN__)

#if !defined(VIO_ADC_USE_VADC1) || defined(__DOXYGEN__)
#define VIO_ADC_USE_VADC1                   FALSE
#endif

#if !VIO_ADC_USE_VADC1
#error "ADC driver activated but no VADC peripheral assigned"
#endif

typedef uint16_t adcsample_t;
typedef uint16_t adc_channels_num_t;
typedef uint32_t adcerror_t;

#define ADC_ERR_DMAFAILURE                  1U
#define ADC_ERR_OVERFLOW                    2U
#define ADC_ERR_AWD1                        4U
#define ADC_ERR_AWD2                        8U
#define ADC_ERR_AWD3                        16U

#define adc_lld_driver_fields                                               \
  uint32_t                  nvadc

#define adc_lld_config_fields

#define adc_lld_configuration_group_fields

#ifdef __cplusplus
extern "C" {
#endif
  extern hal_adc_driver_c ADCD1;
  void adc_lld_init(void);
  msg_t adc_lld_start(hal_adc_driver_c *adcp);
  void adc_lld_stop(hal_adc_driver_c *adcp);
  const hal_adc_config_t *adc_lld_setcfg(hal_adc_driver_c *adcp,
                                         const hal_adc_config_t *config);
  const hal_adc_config_t *adc_lld_selcfg(hal_adc_driver_c *adcp,
                                         unsigned cfgnum);
  void adc_lld_set_callback(hal_adc_driver_c *adcp, drv_cb_t cb);
  msg_t adc_lld_start_conversion(hal_adc_driver_c *adcp, unsigned grpnum,
                                 adcsample_t *samples,
                                 size_t depth);
  void adc_lld_stop_conversion(hal_adc_driver_c *adcp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_ADC */

#endif /* HAL_ADC_LLD_H */

/** @} */

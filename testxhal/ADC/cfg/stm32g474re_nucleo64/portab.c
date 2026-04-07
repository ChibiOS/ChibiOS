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
 * @file    portab.c
 * @brief   Application portability module code.
 *
 * @addtogroup application_portability
 * @{
 */

#include "hal.h"

#include "portab.h"

const hal_adc_config_t portab_adccfg1 = {
  .difsel                      = 0U
};

const adc_conversion_group_t portab_adcgrpcfg1 = {
  .circular                    = false,
  .num_channels                = ADC_GRP1_NUM_CHANNELS,
  .cfgr                        = 0U,
  .cfgr2                       = 0U,
  .tr1                         = ADC_TR_DISABLED,
  .tr2                         = ADC_TR_DISABLED,
  .tr3                         = ADC_TR_DISABLED,
  .awd2cr                      = 0U,
  .awd3cr                      = 0U,
  .smpr                        = {
    ADC_SMPR1_SMP_AN1(ADC_SMPR_SMP_247P5) |
    ADC_SMPR1_SMP_AN2(ADC_SMPR_SMP_247P5),
    0U
  },
  .sqr                         = {
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN1) |
    ADC_SQR1_SQ2_N(ADC_CHANNEL_IN2),
    0U,
    0U,
    0U
  }
};

const adc_conversion_group_t portab_adcgrpcfg2 = {
  .circular                    = true,
  .num_channels                = ADC_GRP2_NUM_CHANNELS,
  .cfgr                        = ADC_CFGR_CONT,
  .cfgr2                       = 0U,
  .tr1                         = ADC_TR_DISABLED,
  .tr2                         = ADC_TR_DISABLED,
  .tr3                         = ADC_TR_DISABLED,
  .awd2cr                      = 0U,
  .awd3cr                      = 0U,
  .smpr                        = {
    ADC_SMPR1_SMP_AN1(ADC_SMPR_SMP_247P5) |
    ADC_SMPR1_SMP_AN2(ADC_SMPR_SMP_247P5),
    0U
  },
  .sqr                         = {
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN1) |
    ADC_SQR1_SQ2_N(ADC_CHANNEL_IN2),
    0U,
    0U,
    0U
  }
};

void portab_setup(void) {

  palSetPadMode(GPIOA, 0U, PAL_MODE_INPUT_ANALOG);
  palSetPadMode(GPIOA, 1U, PAL_MODE_INPUT_ANALOG);
}

/** @} */

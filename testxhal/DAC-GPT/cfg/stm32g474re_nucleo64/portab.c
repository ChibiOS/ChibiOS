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

const hal_gpt_config_t portab_gptcfg1 = {
  .frequency                   = 1000000U,
  .cr2                         = TIM_CR2_MMS_1,
  .dier                        = 0U
};

const DACConfig portab_daccfg1 = {
  .init                        = 2047U,
  .datamode                    = DAC_DHRM_12BIT_RIGHT,
  .cr                          = 0U
};

const DACConversionGroup portab_dacgrpcfg1 = {
  .num_channels                = 1U,
  .trigger                     = DAC_TRG(PORTAB_DAC_TRIG)
};

void portab_setup(void) {

  /* Setting up the output pin as analog as suggested
     by the Reference Manual.*/
  palSetPadMode(GPIOA, 4U, PAL_MODE_INPUT_ANALOG);
}

/** @} */

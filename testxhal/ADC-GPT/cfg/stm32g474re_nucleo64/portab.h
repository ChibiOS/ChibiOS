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
 * @file    portab.h
 * @brief   Application portability macros and structures.
 *
 * @addtogroup application_portability
 * @{
 */

#ifndef PORTAB_H
#define PORTAB_H

#define PORTAB_LINE_LED1                    LINE_LED_GREEN
#define PORTAB_LED_OFF                      PAL_LOW
#define PORTAB_LED_ON                       PAL_HIGH

#define PORTAB_LINE_BUTTON                  LINE_BUTTON
#define PORTAB_BUTTON_PRESSED               PAL_HIGH

#define PORTAB_ADC1                         ADCD1
#define PORTAB_GPT1                         GPTD4
#define PORTAB_GPT1_PERIOD                  100U

#define ADC_GRP1_NUM_CHANNELS               2U
#define ADC_GRP2_NUM_CHANNELS               2U
#define ADC_GRP3_NUM_CHANNELS               2U
#define ADC_GRP1                            0U
#define ADC_GRP2                            1U
#define ADC_GRP3                            2U

extern const hal_adc_config_t portab_adccfg1;
extern const hal_gpt_config_t portab_gptcfg1;

#ifdef __cplusplus
extern "C" {
#endif
  void portab_setup(void);
#ifdef __cplusplus
}
#endif

#endif /* PORTAB_H */

/** @} */

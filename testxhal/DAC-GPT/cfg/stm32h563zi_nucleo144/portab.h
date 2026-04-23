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

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

#define PORTAB_LINE_LED1                    LINE_LED1
#define PORTAB_LINE_LED2                    LINE_LED2
#define PORTAB_LED_OFF                      PAL_LOW
#define PORTAB_LED_ON                       PAL_HIGH
#define PORTAB_LINE_BUTTON                  LINE_BUTTON
#define PORTAB_BUTTON_PRESSED               PAL_HIGH

#define PORTAB_DAC1                         DACD1
#define PORTAB_GPT1                         GPTD6
#define PORTAB_GPT1_PERIOD                  2U
#define PORTAB_DAC_TRIG                     5U

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  extern const DACConfig portab_daccfg1;
  extern const DACConversionGroup portab_dacgrpcfg1;
  extern const hal_gpt_config_t portab_gptcfg1;
  void portab_setup(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* PORTAB_H */

/** @} */

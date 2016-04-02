/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    TIMv1/stm32_tim.h
 * @brief   STM32 TIM units common code.
 * @note    This file requires definitions from the ST STM32 header file.
 *
 * @addtogroup STM32_TIMv1
 * @{
 */

#include "hal.h"

#if HAL_USE_GPT || HAL_USE_ICU || HAL_USE_PWM || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if defined(STM32_TIM2_IS_USED) || defined(__DOXYGEN__)
#if !defined(STM32_TIM2_SUPPRESS_ISR)
#if !defined(STM32_TIM2_HANDLER)
#error "STM32_TIM2_HANDLER not defined"
#endif
/**
 * @brief   TIM2 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_TIM2_HANDLER) {
  uint32_t sr;

  OSAL_IRQ_PROLOGUE();

  sr  = TIM2->SR;
  sr &= TIM2->DIER & STM32_TIM_DIER_IRQ_MASK;
  TIM2->SR = ~sr;

#if STM32_GPT_USE_TIM2
  gpt_lld_serve_interrupt(&GPTD2);
#endif

#if STM32_ICU_USE_TIM2
  icu_lld_serve_interrupt(&ICUD2, sr);
#endif

#if STM32_PWM_USE_TIM2
  gpt_lld_serve_interrupt(&PWMD2, sr);
#endif

#if STM32_ST_USE_TIM2
  st_lld_serve_interrupt();
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(STM32_TIM2_SUPPRESS_ISR) */
#endif /* defined(STM32_TIM2_IS_USED) */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

#endif /* HAL_USE_GPT || HAL_USE_ICU || HAL_USE_PWM */

/** @} */

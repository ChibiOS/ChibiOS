/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    TIMv1/stm32_tim16.inc
 * @brief   Shared TIM16 handler.
 *
 * @addtogroup STM32_TIM16_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(STM32_HAS_TIM16)
#error "STM32_HAS_TIM16 not defined in registry"
#endif

#if STM32_HAS_TIM16 == FALSE
#error "TIM16 not present"
#endif

/* Priority settings checks.*/
#if !defined(STM32_IRQ_TIM16_PRIORITY)
#error "STM32_IRQ_TIM16_PRIORITY not defined in mcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM16_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM16_PRIORITY"
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void tim16_irq_init(void) {
#if defined(STM32_TIM16_IS_USED)
  nvicEnableVector(STM32_TIM16_NUMBER, STM32_IRQ_TIM16_PRIORITY);
#endif
}

static inline void tim16_irq_deinit(void) {
#if defined(STM32_TIM16_IS_USED)
  nvicDisableVector(STM32_TIM16_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (HAL_USE_GPT && STM32_GPT_USE_TIM16) ||                                  \
    (HAL_USE_PWM && STM32_PWM_USE_TIM16) ||                                  \
    defined(__DOXYGEN__)
/**
 * @brief   TIM16 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_TIM16_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_GPT
#if STM32_GPT_USE_TIM16
  gpt_lld_serve_interrupt(&GPTD16);
#endif
#endif
#if HAL_USE_PWM
#if STM32_PWM_USE_TIM16
  pwm_lld_serve_interrupt(&PWMD16);
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/** @} */

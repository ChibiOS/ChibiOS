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
 * @file    TIMv1/stm32_tim1_tim15_tim16_tim17.inc
 * @brief   Shared TIM1 handler.
 *
 * @addtogroup STM32_TIM1_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(STM32_HAS_TIM1)
#error "STM32_HAS_TIM1 not defined in registry"
#endif

#if STM32_HAS_TIM1 == FALSE
#error "TIM1 not present"
#endif

/* Priority settings checks.*/
#if !defined(STM32_IRQ_TIM1_UP_PRIORITY)
#error "STM32_IRQ_TIM1_UP_PRIORITY not defined in mcuconf.h"
#endif

#if !defined(STM32_IRQ_TIM1_CC_PRIORITY)
#error "STM32_IRQ_TIM1_CC_PRIORITY not defined in mcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM1_UP_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM1_UP_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM1_CC_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM1_CC_PRIORITY"
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

static inline void tim1_irq_init(void) {
#if (HAL_USE_GPT && STM32_GPT_USE_TIM1) ||                                  \
    (HAL_USE_ICU && STM32_ICU_USE_TIM1) ||                                  \
    (HAL_USE_PWM && STM32_PWM_USE_TIM1)
  nvicEnableVector(STM32_TIM1_UP_NUMBER, STM32_IRQ_TIM1_UP_PRIORITY);
#endif
#if (HAL_USE_ICU && STM32_ICU_USE_TIM1) ||                                  \
    (HAL_USE_PWM && STM32_PWM_USE_TIM1)
  nvicEnableVector(STM32_TIM1_CC_NUMBER, STM32_IRQ_TIM1_CC_PRIORITY);
#endif
}

static inline void tim1_irq_deinit(void) {
#if (HAL_USE_GPT && STM32_GPT_USE_TIM1) ||                                  \
    (HAL_USE_ICU && STM32_ICU_USE_TIM1) ||                                  \
    (HAL_USE_PWM && STM32_PWM_USE_TIM1)
  nvicDisableVector(STM32_TIM1_UP_TIM16_NUMBER);
#endif
#if (HAL_USE_ICU && STM32_ICU_USE_TIM1) ||                                  \
    (HAL_USE_PWM && STM32_PWM_USE_TIM1)
  nvicDisableVector(STM32_TIM1_CC_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (HAL_USE_GPT && STM32_GPT_USE_TIM1) ||                                  \
    (HAL_USE_ICU && STM32_ICU_USE_TIM1) ||                                  \
    (HAL_USE_PWM && STM32_PWM_USE_TIM1) ||                                  \
    defined(__DOXYGEN__)
/**
 * @brief   TIM1-UP interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_TIM1_UP_TIM16_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_GPT
#if STM32_GPT_USE_TIM1
  gpt_lld_serve_interrupt(&GPTD1);
#endif
#endif
#if HAL_USE_ICU
#if STM32_ICU_USE_TIM1
  icu_lld_serve_interrupt(&ICUD1);
#endif
#endif
#if HAL_USE_PWM
#if STM32_PWM_USE_TIM1
  pwm_lld_serve_interrupt(&PWMD1);
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

#if (HAL_USE_ICU && STM32_ICU_USE_TIM1) ||                                  \
    (HAL_USE_PWM && STM32_PWM_USE_TIM1) ||                                  \
    defined(__DOXYGEN__)
/**
 * @brief   TIM1-CC interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_TIM1_CC_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_GPT
  /* Not used by GPT.*/
#endif
#if HAL_USE_ICU
#if STM32_ICU_USE_TIM1
  icu_lld_serve_interrupt(&ICUD1);
#endif
#endif
#if HAL_USE_PWM
#if STM32_PWM_USE_TIM1
  pwm_lld_serve_interrupt(&PWMD1);
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/** @} */

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
 * @file    TIMv1/stm32_tim20.inc
 * @brief   Shared TIM20 handler.
 *
 * @addtogroup STM32_TIM20_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#if defined(STM32_TIM20_IS_USED) || defined(__DOXYGEN__)
/**
 * @brief   IRQ vectors initialization.
 */
#define STM32_TIM20_TIM205_INIT() do {                                       \
  nvicEnableVector(STM32_TIM20_UP_NUMBER,                                    \
                   STM32_IRQ_TIM20_UP_PRIORITY);                             \
  nvicEnableVector(STM32_TIM20_CC_NUMBER,                                    \
                   STM32_IRQ_TIM20_CC_PRIORITY);                             \
} while (0)

/**
 * @brief   IRQ vectors de-initialization.
 */
#define STM32_TIM20_TIM205_DEINIT() do {                                     \
  nvicDisableVector(STM32_TIM20_UP_NUMBER);                                  \
  nvicDisableVector(STM32_TIM20_CC_NUMBER);                                  \
} while (0)
#else
#define STM32_TIM8_TIM20_INIT()
#define STM32_TIM8_TIM20_DEINIT()
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(STM32_IRQ_TIM20_UP_PRIORITY)
#error "STM32_IRQ_TIM20_UP_PRIORITY not defined in mcuconf.h"
#endif

#if !defined(STM32_IRQ_TIM20_CC_PRIORITY)
#error "STM32_IRQ_TIM20_CC_PRIORITY not defined in mcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM20_UP_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM20_UP_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM20_CC_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM20_CC_PRIORITY"
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

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if HAL_USE_GPT || HAL_USE_ICU || HAL_USE_PWM || defined(__DOXYGEN__)
/**
 * @brief   TIM20-UP interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_TIM20_UP_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_GPT
#if STM32_GPT_USE_TIM20
  gpt_lld_serve_interrupt(&GPTD20);
#endif
#endif
#if HAL_USE_ICU
#if STM32_ICU_USE_TIM20
  icu_lld_serve_interrupt(&ICUD20);
#endif
#endif
#if HAL_USE_PWM
#if STM32_PWM_USE_TIM20
  pwm_lld_serve_interrupt(&PWMD20);
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   TIM20-CC interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_TIM20_CC_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_GPT
  /* Not used by GPT.*/
#endif
#if HAL_USE_ICU
#if STM32_ICU_USE_TIM20
  icu_lld_serve_interrupt(&ICUD20);
#endif
#endif
#if HAL_USE_PWM
#if STM32_PWM_USE_TIM20
  pwm_lld_serve_interrupt(&PWMD20);
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif /* HAL_USE_GPT || HAL_USE_ICU || HAL_USE_PWM */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/** @} */

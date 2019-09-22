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
 * @file    STM32G4xx/stm32_isr.h
 * @brief   STM32G4xx ISR handler code.
 *
 * @addtogroup STM32G4xx_ISR
 * @{
 */

#include "hal.h"

#include "stm32_tim1_tim15_tim16_tim17.inc"
#include "stm32_tim2.inc"
#include "stm32_tim2.inc"
#include "stm32_tim3.inc"
#include "stm32_tim4.inc"
#include "stm32_tim5.inc"
#include "stm32_tim6.inc"
#include "stm32_tim7.inc"
#include "stm32_tim20.inc"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

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

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enables IRQ sources.
 *
 * @notapi
 */
void irqInit(void) {

#if HAL_USE_PAL
  nvicEnableVector(STM32_EXTI_LINE0_NUMBER,      STM32_IRQ_EXTI0_PRIORITY);
  nvicEnableVector(STM32_EXTI_LINE1_NUMBER,      STM32_IRQ_EXTI1_PRIORITY);
  nvicEnableVector(STM32_EXTI_LINE2_NUMBER,      STM32_IRQ_EXTI2_PRIORITY);
  nvicEnableVector(STM32_EXTI_LINE3_NUMBER,      STM32_IRQ_EXTI3_PRIORITY);
  nvicEnableVector(STM32_EXTI_LINE4_NUMBER,      STM32_IRQ_EXTI4_PRIORITY);
  nvicEnableVector(STM32_EXTI_LINE5_9_HANDLER,   STM32_IRQ_EXTI5_9_PRIORITY);
  nvicEnableVector(STM32_EXTI_LINE10_15_HANDLER, STM32_IRQ_EXTI10_15_PRIORITY);
#endif
#if HAL_USE_GPT || HAL_USE_ICU || HAL_USE_PWM || defined(__DOXYGEN__)
  STM32_TIM1_TIM15_TIM16_TIM17_INIT();
  STM32_TIM2_INIT();
  STM32_TIM3_INIT();
  STM32_TIM4_INIT();
  STM32_TIM5_INIT();
  STM32_TIM6_INIT();
  STM32_TIM7_INIT();
  STM32_TIM8_INIT();
  STM32_TIM20_INIT();
#endif
}

/**
 * @brief   Disables IRQ sources.
 *
 * @notapi
 */
void irqDeinit(void) {

#if HAL_USE_PAL
  nvicDisableVector(STM32_EXTI_LINE0_NUMBER);
  nvicDisableVector(STM32_EXTI_LINE1_NUMBER);
  nvicDisableVector(STM32_EXTI_LINE2_NUMBER);
  nvicDisableVector(STM32_EXTI_LINE3_NUMBER);
  nvicDisableVector(STM32_EXTI_LINE4_NUMBER);
  nvicDisableVector(STM32_EXTI_LINE5_9_HANDLER);
  nvicDisableVector(STM32_EXTI_LINE10_15_HANDLER);
#endif
#if HAL_USE_GPT || HAL_USE_ICU || HAL_USE_PWM || defined(__DOXYGEN__)
  STM32_TIM1_TIM15_TIM16_TIM17_DEINIT();
  STM32_TIM2_DEINIT();
  STM32_TIM3_DEINIT();
  STM32_TIM4_DEINIT();
  STM32_TIM5_DEINIT();
  STM32_TIM6_DEINIT();
  STM32_TIM7_DEINIT();
  STM32_TIM8_DEINIT();
  STM32_TIM20_DEINIT();
#endif
}

/** @} */

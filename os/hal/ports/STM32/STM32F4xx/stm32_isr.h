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
 * @file    STM32F4xx/stm32_isr.h
 * @brief   STM32F4xx ISR handler header.
 *
 * @addtogroup STM32F4xx_ISR
 * @{
 */

#ifndef STM32_ISR_H
#define STM32_ISR_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    ISR names and numbers remapping
 * @{
 */
#define STM32_TIM1_SUPPRESS_ISR
#define STM32_TIM9_SUPPRESS_ISR
#define STM32_TIM10_SUPPRESS_ISR
#define STM32_TIM11_SUPPRESS_ISR
#define STM32_TIM8_SUPPRESS_ISR
#define STM32_TIM12_SUPPRESS_ISR
#define STM32_TIM13_SUPPRESS_ISR
#define STM32_TIM14_SUPPRESS_ISR
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   EXTI0 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI0_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI0_PRIORITY            6
#endif

/**
 * @brief   EXTI1 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI1_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI1_PRIORITY            6
#endif

/**
 * @brief   EXTI2 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI2_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI2_PRIORITY            6
#endif

/**
 * @brief   EXTI3 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI3_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI3_PRIORITY            6
#endif

/**
 * @brief   EXTI4 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI4_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI4_PRIORITY            6
#endif

/**
 * @brief   EXTI5..9 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI5_9_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI5_9_PRIORITY          6
#endif

/**
 * @brief   EXTI10..15 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI10_15_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI10_15_PRIORITY        6
#endif

/**
 * @brief   EXTI16 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI16_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI16_PRIORITY           6
#endif

/**
 * @brief   EXTI17 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI17_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI17_PRIORITY           6
#endif

/**
 * @brief   EXTI18 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI18_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI18_PRIORITY           6
#endif

/**
 * @brief   EXTI21 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI21_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI21_PRIORITY           6
#endif

/**
 * @brief   EXTI22 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI22_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI22_PRIORITY           6
#endif

/**
 * @brief   EXTI23 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI23_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI23_PRIORITY           6
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* IRQ priority checks.*/
#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI0_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI0_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI1_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI1_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI2_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI2_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI3_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI3_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI4_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI4_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI5_9_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI5_9_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI10_15_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI10_15_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI16_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI16_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI17_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI17_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI18_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI18_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI21_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI21_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI22_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI22_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI23_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI23_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM1_BRK_TIM9_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM1_BRK_TIM9_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM1_UP_TIM10_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM1_UP_TIM10_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM1_TRGCO_TIM11_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM1_TRGCO_TIM11_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM1_CC_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM1_CC_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM8_BRK_TIM12_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM1_BRK_TIM12_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM8_UP_TIM13_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM1_UP_TIM13_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM8_TRGCO_TIM14_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM1_TRGCO_TIM14_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_TIM8_CC_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_TIM8_CC_PRIORITY"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void irqInit(void);
  void irqDeinit(void);
#ifdef __cplusplus
}
#endif

#endif /* STM32_ISR_H */

/** @} */

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
 * @file    STM32G0xx/stm32_isr.h
 * @brief   STM32G0xx ISR handler header.
 *
 * @addtogroup STM32G0xx_ISR
 * @{
 */

#ifndef STM32_ISR_H
#define STM32_ISR_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    ISRs suppressed in standard drivers
 * @{
 */
#define STM32_USART1_SUPPRESS_ISR
#define STM32_USART2_SUPPRESS_ISR
#define STM32_USART3_SUPPRESS_ISR
#define STM32_UART4_SUPPRESS_ISR
#define STM32_LPUART1_SUPPRESS_ISR
/** @} */

/**
 * @name    ISR names and numbers remapping
 * @{
 */
/*
 * EXTI unit.
 */
#define STM32_EXTI_LINE01_HANDLER           Vector54
#define STM32_EXTI_LINE23_HANDLER           Vector58
#define STM32_EXTI_LINE4_15_HANDLER         Vector5C
#define STM32_EXTI_LINE16_HANDLER           Vector44
#define STM32_EXTI_LINE1921_HANDLER         Vector48

#define STM32_EXTI_LINE01_NUMBER            5
#define STM32_EXTI_LINE23_NUMBER            6
#define STM32_EXTI_LINE4_15_NUMBER          7
#define STM32_EXTI_LINE16_NUMBER            1
#define STM32_EXTI_LINE1921_NUMBER          2

/*
 * I2C units.
 */
#define STM32_I2C1_GLOBAL_HANDLER           Vector9C
#define STM32_I2C2_GLOBAL_HANDLER           VectorA0

#define STM32_I2C1_GLOBAL_NUMBER            23
#define STM32_I2C2_GLOBAL_NUMBER            24

/*
 * TIM units.
 */
#define STM32_TIM1_UP_HANDLER               Vector74
#define STM32_TIM1_CC_HANDLER               Vector78
#define STM32_TIM2_HANDLER                  Vector7C
#define STM32_TIM3_HANDLER                  VectorB4
#define STM32_TIM6_HANDLER                  Vector84
#define STM32_TIM7_HANDLER                  Vector88
#define STM32_TIM14_HANDLER                 Vector8C
#define STM32_TIM15_HANDLER                 Vector90
#define STM32_TIM16_HANDLER                 Vector94
#define STM32_TIM17_HANDLER                 Vector98

#define STM32_TIM1_UP_NUMBER                13
#define STM32_TIM1_CC_NUMBER                14
#define STM32_TIM2_NUMBER                   15
#define STM32_TIM3_NUMBER                   29
#define STM32_TIM6_NUMBER                   17
#define STM32_TIM7_NUMBER                   18
#define STM32_TIM14_NUMBER                  19
#define STM32_TIM15_NUMBER                  20
#define STM32_TIM16_NUMBER                  21
#define STM32_TIM17_NUMBER                  22

/*
 * USART/UART units.
 */
#define STM32_USART1_HANDLER                VectorAC
#define STM32_USART2_HANDLER                VectorB0
#define STM32_USART3_4_LP1_HANDLER          VectorB4

#define STM32_USART1_NUMBER                 27
#define STM32_USART2_NUMBER                 28
#define STM32_USART3_4_LP1_NUMBER           29
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   EXTI0..1 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI0_1_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI0_1_PRIORITY          3
#endif

/**
 * @brief   EXTI2..3 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI2_3_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI2_3_PRIORITY          3
#endif

/**
 * @brief   EXTI4..15 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI4_15_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI4_15_PRIORITY         3
#endif

/**
 * @brief   EXTI16 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI16_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI16_PRIORITY           3
#endif

/**
 * @brief   EXTI17..18 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI17_18_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI17_18_PRIORITY        3
#endif

/**
 * @brief   USART1 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_USART1_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_USART1_PRIORITY           3
#endif

/**
 * @brief   USART2 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_USART2_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_USART2_PRIORITY           3
#endif

/**
 * @brief   USART3, 4 and LP1 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_USART3_4_LP1_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_USART3_4_LP1_PRIORITY     3
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* IRQ priority checks.*/
#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI0_1_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI0_1_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI2_3_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI2_3_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI4_15_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI4_15_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI16_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI16_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI17_18_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI17_18_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_USART1_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_USART1_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_USART2_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_USART2_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_USART3_4_LP1_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_USART3_4_LP1_PRIORITY"
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

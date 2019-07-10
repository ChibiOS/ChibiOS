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
 * @file    STM32L0xx/stm32_isr.h
 * @brief   STM32L0xx ISR handler header.
 *
 * @addtogroup STM32L0xx_ISR
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
#define STM32_UART4_SUPPRESS_ISR
#define STM32_UART5_SUPPRESS_ISR
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
#define STM32_EXTI_LINE171920_HANDLER       Vector48
#define STM32_EXTI_LINE2122_HANDLER         Vector70

#define STM32_EXTI_LINE01_NUMBER            5
#define STM32_EXTI_LINE23_NUMBER            6
#define STM32_EXTI_LINE4_15_NUMBER          7
#define STM32_EXTI_LINE16_NUMBER            1
#define STM32_EXTI_LINE171920_NUMBER        2
#define STM32_EXTI_LINE2122_NUMBER          12

/*
 * USART/UART units.
 */
#define STM32_USART1_HANDLER                VectorAC
#define STM32_USART2_HANDLER                VectorB0
#define STM32_UART4_5_HANDLER               Vector78
#define STM32_LPUART1_HANDLER               VectorB4
#define STM32_USART1_NUMBER                 27
#define STM32_USART2_NUMBER                 28
#define STM32_UART4_5_NUMBER                14
#define STM32_LPUART1_NUMBER                29
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
 * @brief   EXTI16 (PVD) interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI16_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI16_PRIORITY           3
#endif

/**
 * @brief   EXTI17,19,20 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI17_20_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI17_20_PRIORITY        3
#endif

/**
 * @brief   EXTI21,22 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI21_22_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI21_22_PRIORITY        3
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
 * @brief   UART4 and 5 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_UART4_5_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_UART4_5_PRIORITY          3
#endif

/**
 * @brief   LPUART1 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_LPUART1_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_LPUART1_PRIORITY          3
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

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI17_20_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI17_20_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI21_22_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI21_22_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_USART1_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_USART1_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_USART2_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_USART2_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_UART4_5_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_UART4_5_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_LPUART1_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_LPUART1_PRIORITY"
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

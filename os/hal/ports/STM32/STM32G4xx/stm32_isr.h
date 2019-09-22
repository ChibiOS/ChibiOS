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
 * @brief   STM32G4xx ISR handler header.
 *
 * @addtogroup STM32G4xx_ISR
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
#define STM32_TIM1_SUPPRESS_ISR
#define STM32_TIM2_SUPPRESS_ISR
#define STM32_TIM3_SUPPRESS_ISR
#define STM32_TIM4_SUPPRESS_ISR
#define STM32_TIM5_SUPPRESS_ISR
#define STM32_TIM6_SUPPRESS_ISR
#define STM32_TIM7_SUPPRESS_ISR
#define STM32_TIM8_SUPPRESS_ISR
#define STM32_TIM15_SUPPRESS_ISR
#define STM32_TIM16_SUPPRESS_ISR
#define STM32_TIM17_SUPPRESS_ISR
#define STM32_TIM20_SUPPRESS_ISR

#define STM32_USART1_SUPPRESS_ISR
#define STM32_USART2_SUPPRESS_ISR
#define STM32_USART3_SUPPRESS_ISR
#define STM32_UART4_SUPPRESS_ISR
#define STM32_UART5_SUPPRESS_ISR
#define STM32_LPUART1_SUPPRESS_ISR
/** @} */

/**
 * @name    ISR names and numbers remapping
 * @{
 */
/*
 * ADC unit.
 */
#define STM32_ADC1_HANDLER                  Vector88
#define STM32_ADC2_HANDLER                  Vector88
#define STM32_ADC3_HANDLER                  VectorFC
#define STM32_ADC4_HANDLER                  Vector134
#define STM32_ADC5_HANDLER                  Vector138

#define STM32_ADC1_NUMBER                   18
#define STM32_ADC2_NUMBER                   18
#define STM32_ADC3_NUMBER                   47
#define STM32_ADC4_NUMBER                   61
#define STM32_ADC5_NUMBER                   62

/*
 * DMA unit.
 */
#define STM32_DMA1_CH1_HANDLER              Vector6C
#define STM32_DMA1_CH2_HANDLER              Vector70
#define STM32_DMA1_CH3_HANDLER              Vector74
#define STM32_DMA1_CH4_HANDLER              Vector78
#define STM32_DMA1_CH5_HANDLER              Vector7C
#define STM32_DMA1_CH6_HANDLER              Vector80
#define STM32_DMA1_CH7_HANDLER              Vector84
#define STM32_DMA1_CH8_HANDLER              Vector1B0
#define STM32_DMA2_CH1_HANDLER              Vector120
#define STM32_DMA2_CH2_HANDLER              Vector124
#define STM32_DMA2_CH3_HANDLER              Vector128
#define STM32_DMA2_CH4_HANDLER              Vector12C
#define STM32_DMA2_CH5_HANDLER              Vector130
#define STM32_DMA2_CH6_HANDLER              Vector1B4
#define STM32_DMA2_CH7_HANDLER              Vector1B8
#define STM32_DMA2_CH8_HANDLER              Vector1BC

#define STM32_DMA1_CH1_NUMBER               11
#define STM32_DMA1_CH2_NUMBER               12
#define STM32_DMA1_CH3_NUMBER               13
#define STM32_DMA1_CH4_NUMBER               14
#define STM32_DMA1_CH5_NUMBER               15
#define STM32_DMA1_CH6_NUMBER               16
#define STM32_DMA1_CH7_NUMBER               17
#define STM32_DMA1_CH8_NUMBER               96
#define STM32_DMA2_CH1_NUMBER               56
#define STM32_DMA2_CH2_NUMBER               57
#define STM32_DMA2_CH3_NUMBER               58
#define STM32_DMA2_CH4_NUMBER               59
#define STM32_DMA2_CH5_NUMBER               60
#define STM32_DMA2_CH6_NUMBER               97
#define STM32_DMA2_CH7_NUMBER               98
#define STM32_DMA2_CH8_NUMBER               99

/*
 * EXTI unit.
 */
#define STM32_EXTI_LINE0_HANDLER            Vector58
#define STM32_EXTI_LINE1_HANDLER            Vector5C
#define STM32_EXTI_LINE2_HANDLER            Vector60
#define STM32_EXTI_LINE3_HANDLER            Vector64
#define STM32_EXTI_LINE4_HANDLER            Vector68
#define STM32_EXTI_LINE5_9_HANDLER          Vector9C
#define STM32_EXTI_LINE10_15_HANDLER        VectorE0
#define STM32_EXTI_LINE164041HANDLER        Vector44
#define STM32_EXTI_LINE17_HANDLER           VectorE4
#define STM32_EXTI_LINE19_HANDLER           Vector48
#define STM32_EXTI_LINE20_HANDLER           Vector4C
#define STM32_EXTI_LINE212229_HANDLER       Vector140
#define STM32_EXTI_LINE303132_HANDLER       Vector144
#define STM32_EXTI_LINE33_HANDLER           Vector148

#define STM32_EXTI_LINE0_NUMBER             6
#define STM32_EXTI_LINE1_NUMBER             7
#define STM32_EXTI_LINE2_NUMBER             8
#define STM32_EXTI_LINE3_NUMBER             9
#define STM32_EXTI_LINE4_NUMBER             10
#define STM32_EXTI_LINE5_9_NUMBER           23
#define STM32_EXTI_LINE10_15_NUMBER         40
#define STM32_EXTI_LINE164041_NUMBER        1
#define STM32_EXTI_LINE17_NUMBER            41
#define STM32_EXTI_LINE19_NUMBER            2
#define STM32_EXTI_LINE20_NUMBER            3
#define STM32_EXTI_LINE212229_NUMBER        64
#define STM32_EXTI_LINE303132_NUMBER        65
#define STM32_EXTI_LINE33_NUMBER            66

/*
 * I2C units.
 */
#define STM32_I2C1_EVENT_HANDLER            VectorBC
#define STM32_I2C1_ERROR_HANDLER            VectorC0
#define STM32_I2C2_EVENT_HANDLER            VectorC4
#define STM32_I2C2_ERROR_HANDLER            VectorC8
#define STM32_I2C3_EVENT_HANDLER            Vector1A0
#define STM32_I2C3_ERROR_HANDLER            Vector1A4
#define STM32_I2C4_EVENT_HANDLER            Vector188
#define STM32_I2C4_ERROR_HANDLER            Vector18C

#define STM32_I2C1_EVENT_NUMBER             31
#define STM32_I2C1_ERROR_NUMBER             32
#define STM32_I2C2_EVENT_NUMBER             33
#define STM32_I2C2_ERROR_NUMBER             34
#define STM32_I2C3_EVENT_NUMBER             92
#define STM32_I2C3_ERROR_NUMBER             93
#define STM32_I2C4_EVENT_NUMBER             82
#define STM32_I2C4_ERROR_NUMBER             83

/*
 * TIM units.
 */
#define STM32_TIM1_BRK_TIM15_HANDLER        VectorA0
#define STM32_TIM1_UP_TIM16_HANDLER         VectorA4
#define STM32_TIM1_TRGCO_TIM17_HANDLER      VectorA8
#define STM32_TIM1_CC_HANDLER               VectorAC
#define STM32_TIM2_HANDLER                  VectorB0
#define STM32_TIM3_HANDLER                  VectorB4
#define STM32_TIM4_HANDLER                  VectorB8
#define STM32_TIM5_HANDLER                  Vector108
#define STM32_TIM6_HANDLER                  Vector118
#define STM32_TIM7_HANDLER                  Vector11C
#define STM32_TIM8_BRK_HANDLER              VectorEC
#define STM32_TIM8_UP_HANDLER               VectorF0
#define STM32_TIM8_TRGCO_HANDLER            VectorF4
#define STM32_TIM8_CC_HANDLER               VectorF8
#define STM32_TIM20_BRK_HANDLER             Vector174
#define STM32_TIM20_UP_HANDLER              Vector178
#define STM32_TIM20_TRGCO_HANDLER           Vector17C
#define STM32_TIM20_CC_HANDLER              Vector180

#define STM32_TIM1_BRK_TIM15_NUMBER         24
#define STM32_TIM1_UP_TIM16_NUMBER          25
#define STM32_TIM1_TRGCO_TIM17_NUMBER       26
#define STM32_TIM1_CC_NUMBER                27
#define STM32_TIM2_NUMBER                   28
#define STM32_TIM3_NUMBER                   29
#define STM32_TIM4_NUMBER                   30
#define STM32_TIM5_NUMBER                   50
#define STM32_TIM6_NUMBER                   54
#define STM32_TIM7_NUMBER                   55
#define STM32_TIM8_BRK_NUMBER               43
#define STM32_TIM8_UP_NUMBER                44
#define STM32_TIM8_TRGCO_NUMBER             45
#define STM32_TIM8_CC_NUMBER                46
#define STM32_TIM20_BRK_NUMBER              77
#define STM32_TIM20_UP_NUMBER               78
#define STM32_TIM20_TRGCO_NUMBER            79
#define STM32_TIM20_CC_NUMBER               80

/*
 * USART/UART units.
 */
#define STM32_USART1_HANDLER                VectorD4
#define STM32_USART2_HANDLER                VectorD8
#define STM32_USART3_HANDLER                VectorDC
#define STM32_UART4_HANDLER                 Vector110
#define STM32_UART5_HANDLER                 Vector114
#define STM32_LPUART1_HANDLER               Vector19C

#define STM32_USART1_NUMBER                 37
#define STM32_USART2_NUMBER                 38
#define STM32_USART3_NUMBER                 39
#define STM32_UART4_NUMBER                  52
#define STM32_UART5_NUMBER                  53
#define STM32_LPUART1_NUMBER                91
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
 * @brief   EXTI9..5 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI5_9_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI5_9_PRIORITY          6
#endif

/**
 * @brief   EXTI15..10 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI10_15_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI10_15_PRIORITY        6
#endif

/**
 * @brief   EXTI16 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI16_40_41_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI16_40_41_PRIORITY     6
#endif

/**
 * @brief   EXTI17 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI17_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI17_PRIORITY           6
#endif

/**
 * @brief   EXTI19 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI19_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI19_PRIORITY           6
#endif

/**
 * @brief   EXTI20 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI20_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI20_PRIORITY           6
#endif

/**
 * @brief   EXTI21, 22, 29 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI21_22_29_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI21_22_29_PRIORITY     6
#endif

/**
 * @brief   EXTI30, 31, 32 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_EXTI30_31_32_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_EXTI30_31_32_PRIORITY     6
#endif

/**
 * @brief   TIM1-BRK, TIM15 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM1_BRK_TIM15_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM1_BRK_TIM15_PRIORITY   7
#endif

/**
 * @brief   TIM1-UP, TIM16 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM1_UP_TIM16_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM1_UP_TIM16_PRIORITY    7
#endif

/**
 * @brief   TIM1-TRG-COM, TIM17 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM1_TRGCO_TIM17_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM1_TRGCO_TIM17_PRIORITY 7
#endif

/**
 * @brief   TIM1-CC interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM1_CC_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM1_CC_PRIORITY          7
#endif

/**
 * @brief   TIM2 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM2_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM2_PRIORITY             7
#endif

/**
 * @brief   TIM3 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM3_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM3_PRIORITY             7
#endif

/**
 * @brief   TIM4 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM4_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM4_PRIORITY             7
#endif

/**
 * @brief   TIM5 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM5_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM5_PRIORITY             7
#endif

/**
 * @brief   TIM6 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM6_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM6_PRIORITY             7
#endif

/**
 * @brief   TIM7 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM7_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM7_PRIORITY             7
#endif

/**
 * @brief   TIM8-BRK interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM8_BRK_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM8_BRK_PRIORITY         7
#endif

/**
 * @brief   TIM8-UP interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM8_UP_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM8_UP_PRIORITY          7
#endif

/**
 * @brief   TIM8-TRG-COM interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM8_TRGCO_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM8_TRGCO_PRIORITY       7
#endif

/**
 * @brief   TIM8-CC interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM8_CC_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM8_CC_PRIORITY          7
#endif

/**
 * @brief   TIM20-BRK interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM20_BRK_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM20_BRK_PRIORITY        7
#endif

/**
 * @brief   TIM20-UP interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM20_UP_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM20_UP_PRIORITY         7
#endif

/**
 * @brief   TIM20-TRG-COM interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM20_TRGCO_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM20_TRGCO_PRIORITY      7
#endif

/**
 * @brief   TIM20-CC interrupt priority level setting.
 */
#if !defined(STM32_IRQ_TIM20_CC_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_TIM20_CC_PRIORITY         7
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
 * @brief   USART3 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_USART3_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_USART3_PRIORITY           3
#endif

/**
 * @brief   UART4 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_UART4_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_UART4_PRIORITY            3
#endif

/**
 * @brief   UART5 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_UART5_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_UART5_PRIORITY            3
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

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI16_40_41_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI16_40_41_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI17_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI17_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI19_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI19_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI20_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI20_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI21_22_29_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI21_22_29_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_EXTI16_40_41_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_EXTI16_40_41_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_USART1_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_USART1_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_USART2_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_USART2_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_USART3_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_USART3_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_UART4_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_UART4_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_UART5_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_UART5_PRIORITY"
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

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
 * @name    ISR names and numbers
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
#if !defined(STM32G431xx) && !defined(STM32G441xx)
#define STM32_DMA1_CH7_HANDLER              Vector84
#define STM32_DMA1_CH8_HANDLER              Vector1C0
#endif
#define STM32_DMA2_CH1_HANDLER              Vector120
#define STM32_DMA2_CH2_HANDLER              Vector124
#define STM32_DMA2_CH3_HANDLER              Vector128
#define STM32_DMA2_CH4_HANDLER              Vector12C
#define STM32_DMA2_CH5_HANDLER              Vector130
#define STM32_DMA2_CH6_HANDLER              Vector1C4
#if !defined(STM32G431xx) && !defined(STM32G441xx)
#define STM32_DMA2_CH7_HANDLER              Vector1C8
#define STM32_DMA2_CH8_HANDLER              Vector1CC
#endif

#define STM32_DMA1_CH1_NUMBER               11
#define STM32_DMA1_CH2_NUMBER               12
#define STM32_DMA1_CH3_NUMBER               13
#define STM32_DMA1_CH4_NUMBER               14
#define STM32_DMA1_CH5_NUMBER               15
#define STM32_DMA1_CH6_NUMBER               16
#if !defined(STM32G431xx) && !defined(STM32G441xx)
#define STM32_DMA1_CH7_NUMBER               17
#define STM32_DMA1_CH8_NUMBER               96
#endif
#define STM32_DMA2_CH1_NUMBER               56
#define STM32_DMA2_CH2_NUMBER               57
#define STM32_DMA2_CH3_NUMBER               58
#define STM32_DMA2_CH4_NUMBER               59
#define STM32_DMA2_CH5_NUMBER               60
#define STM32_DMA2_CH6_NUMBER               97
#if !defined(STM32G431xx) && !defined(STM32G441xx)
#define STM32_DMA2_CH7_NUMBER               98
#define STM32_DMA2_CH8_NUMBER               99
#endif

/*
 * EXTI unit.
 */
#define STM32_EXTI0_HANDLER                 Vector58
#define STM32_EXTI1_HANDLER                 Vector5C
#define STM32_EXTI2_HANDLER                 Vector60
#define STM32_EXTI3_HANDLER                 Vector64
#define STM32_EXTI4_HANDLER                 Vector68
#define STM32_EXTI5_9_HANDLER               Vector9C
#define STM32_EXTI10_15_HANDLER             VectorE0
#define STM32_EXTI164041_HANDLER            Vector44    /* PVD PVM          */
#define STM32_EXTI17_HANDLER                VectorE4    /* RTC ALARM        */
#define STM32_EXTI18_HANDLER                VectorE8    /* USB WAKEUP       */
#define STM32_EXTI19_HANDLER                Vector48    /* RTC TAMP CSS     */
#define STM32_EXTI20_HANDLER                Vector4C    /* RTC WAKEUP       */
#define STM32_EXTI212229_HANDLER            Vector140   /* COMP1..3         */
#define STM32_EXTI30_32_HANDLER             Vector144   /* COMP4..6         */
#define STM32_EXTI33_HANDLER                Vector148   /* COMP7            */

#define STM32_EXTI0_NUMBER                  6
#define STM32_EXTI1_NUMBER                  7
#define STM32_EXTI2_NUMBER                  8
#define STM32_EXTI3_NUMBER                  9
#define STM32_EXTI4_NUMBER                  10
#define STM32_EXTI5_9_NUMBER                23
#define STM32_EXTI10_15_NUMBER              40
#define STM32_EXTI164041_NUMBER             1
#define STM32_EXTI17_NUMBER                 41
#define STM32_EXTI18_NUMBER                 42
#define STM32_EXTI19_NUMBER                 2
#define STM32_EXTI20_NUMBER                 3
#define STM32_EXTI212229_NUMBER             64
#define STM32_EXTI30_32_NUMBER              65
#define STM32_EXTI33_NUMBER                 66

/*
 * FDCAN units.
 */
#define STM32_FDCAN1_IT0_HANDLER            Vector94
#define STM32_FDCAN1_IT1_HANDLER            Vector98
#define STM32_FDCAN2_IT0_HANDLER            Vector198
#define STM32_FDCAN2_IT1_HANDLER            Vector19C
#define STM32_FDCAN3_IT0_HANDLER            Vector1A0
#define STM32_FDCAN3_IT1_HANDLER            Vector1A4

#define STM32_FDCAN1_IT0_NUMBER             21
#define STM32_FDCAN1_IT1_NUMBER             22
#define STM32_FDCAN2_IT0_NUMBER             86
#define STM32_FDCAN2_IT1_NUMBER             87
#define STM32_FDCAN3_IT0_NUMBER             88
#define STM32_FDCAN3_IT1_NUMBER             89

/*
 * I2C units.
 */
#define STM32_I2C1_EV_HANDLER               VectorBC
#define STM32_I2C1_ER_HANDLER               VectorC0
#define STM32_I2C2_EV_HANDLER               VectorC4
#define STM32_I2C2_ER_HANDLER               VectorC8
#define STM32_I2C3_EV_HANDLER               Vector1B0
#define STM32_I2C3_ER_HANDLER               Vector1B4
#define STM32_I2C4_EV_HANDLER               Vector188
#define STM32_I2C4_ER_HANDLER               Vector18C

#define STM32_I2C1_EV_NUMBER                31
#define STM32_I2C1_ER_NUMBER                32
#define STM32_I2C2_EV_NUMBER                33
#define STM32_I2C2_ER_NUMBER                34
#define STM32_I2C3_EV_NUMBER                92
#define STM32_I2C3_ER_NUMBER                93
#define STM32_I2C4_EV_NUMBER                82
#define STM32_I2C4_ER_NUMBER                83

/*
 * QUADSPI unit.
 */
#define STM32_QUADSPI1_HANDLER              Vector1BC
#define STM32_QUADSPI1_NUMBER               95

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
#define STM32_LPUART1_HANDLER               Vector1AC

#define STM32_USART1_NUMBER                 37
#define STM32_USART2_NUMBER                 38
#define STM32_USART3_NUMBER                 39
#define STM32_UART4_NUMBER                  52
#define STM32_UART5_NUMBER                  53
#define STM32_LPUART1_NUMBER                91

/*
 * USB units.
 */
#define STM32_USB1_HP_HANDLER               Vector8C
#define STM32_USB1_LP_HANDLER               Vector90
#define STM32_USB1_HP_NUMBER                19
#define STM32_USB1_LP_NUMBER                20
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

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

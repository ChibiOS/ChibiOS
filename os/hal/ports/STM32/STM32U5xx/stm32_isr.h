/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    STM32U5xx/stm32_isr.h
 * @brief   STM32U5xx ISR handler header.
 *
 * @addtogroup STM32U5xx_ISR
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
#define STM32_ADC1_HANDLER                  VectorD4
#define STM32_ADC4_HANDLER                  Vector204

#define STM32_ADC1_NUMBER                   37
#define STM32_ADC4_NUMBER                   113

/*
 * DAC unit.
 */
#define STM32_DAC1_HANDLER                  VectorD8

#define STM32_DAC1_NUMBER                   38

/*
 * DMA unit.
 */
#define STM32_DMA31_CH0_HANDLER             VectorB4
#define STM32_DMA31_CH1_HANDLER             VectorB8
#define STM32_DMA31_CH2_HANDLER             VectorBC
#define STM32_DMA31_CH3_HANDLER             VectorC0
#define STM32_DMA31_CH4_HANDLER             VectorC4
#define STM32_DMA31_CH5_HANDLER             VectorC8
#define STM32_DMA31_CH6_HANDLER             VectorCC
#define STM32_DMA31_CH7_HANDLER             VectorD0
#define STM32_DMA32_CH0_HANDLER             Vector180
#define STM32_DMA32_CH1_HANDLER             Vector184
#define STM32_DMA32_CH2_HANDLER             Vector188
#define STM32_DMA32_CH3_HANDLER             Vector18C
#define STM32_DMA32_CH4_HANDLER             Vector190
#define STM32_DMA32_CH5_HANDLER             Vector194
#define STM32_DMA32_CH6_HANDLER             Vector198
#define STM32_DMA32_CH7_HANDLER             Vector19C

#define STM32_DMA31_CH0_NUMBER              29
#define STM32_DMA31_CH1_NUMBER              30
#define STM32_DMA31_CH2_NUMBER              31
#define STM32_DMA31_CH3_NUMBER              32
#define STM32_DMA31_CH4_NUMBER              33
#define STM32_DMA31_CH5_NUMBER              34
#define STM32_DMA31_CH6_NUMBER              35
#define STM32_DMA31_CH7_NUMBER              36
#define STM32_DMA32_CH0_NUMBER              80
#define STM32_DMA32_CH1_NUMBER              81
#define STM32_DMA32_CH2_NUMBER              82
#define STM32_DMA32_CH3_NUMBER              83
#define STM32_DMA32_CH4_NUMBER              84
#define STM32_DMA32_CH5_NUMBER              85
#define STM32_DMA32_CH6_NUMBER              86
#define STM32_DMA32_CH7_NUMBER              87

/*
 * EXTI unit.
 */
#define STM32_EXTI0_HANDLER                 Vector6C
#define STM32_EXTI1_HANDLER                 Vector70
#define STM32_EXTI2_HANDLER                 Vector74
#define STM32_EXTI3_HANDLER                 Vector78
#define STM32_EXTI4_HANDLER                 Vector7C
#define STM32_EXTI5_HANDLER                 Vector80
#define STM32_EXTI6_HANDLER                 Vector84
#define STM32_EXTI7_HANDLER                 Vector88
#define STM32_EXTI8_HANDLER                 Vector8C
#define STM32_EXTI9_HANDLER                 Vector90
#define STM32_EXTI10_HANDLER                Vector94
#define STM32_EXTI11_HANDLER                Vector98
#define STM32_EXTI12_HANDLER                Vector9C
#define STM32_EXTI13_HANDLER                VectorA0
#define STM32_EXTI14_HANDLER                VectorA4
#define STM32_EXTI15_HANDLER                VectorA8

#define STM32_EXTI0_NUMBER                  11
#define STM32_EXTI1_NUMBER                  12
#define STM32_EXTI2_NUMBER                  13
#define STM32_EXTI3_NUMBER                  14
#define STM32_EXTI4_NUMBER                  15
#define STM32_EXTI5_NUMBER                  16
#define STM32_EXTI6_NUMBER                  17
#define STM32_EXTI7_NUMBER                  18
#define STM32_EXTI8_NUMBER                  19
#define STM32_EXTI9_NUMBER                  20
#define STM32_EXTI10_NUMBER                 21
#define STM32_EXTI11_NUMBER                 22
#define STM32_EXTI12_NUMBER                 23
#define STM32_EXTI13_NUMBER                 24
#define STM32_EXTI14_NUMBER                 25
#define STM32_EXTI15_NUMBER                 26

/*
 * FDCAN units.
 */
#define STM32_FDCAN1_IT0_HANDLER            VectorDC
#define STM32_FDCAN1_IT1_HANDLER            VectorE0

#define STM32_FDCAN1_IT0_NUMBER             39
#define STM32_FDCAN1_IT1_NUMBER             40

/*
 * I2C units.
 */
#define STM32_I2C1_EV_HANDLER               Vector11C
#define STM32_I2C1_ER_HANDLER               Vector120
#define STM32_I2C2_EV_HANDLER               Vector124
#define STM32_I2C2_ER_HANDLER               Vector128
#define STM32_I2C3_EV_HANDLER               Vector1A0
#define STM32_I2C3_ER_HANDLER               Vector1A4
#define STM32_I2C4_ER_HANDLER               Vector1D0
#define STM32_I2C4_EV_HANDLER               Vector1D4

#define STM32_I2C1_EV_NUMBER                55
#define STM32_I2C1_ER_NUMBER                56
#define STM32_I2C2_EV_NUMBER                57
#define STM32_I2C2_ER_NUMBER                58
#define STM32_I2C3_EV_NUMBER                88
#define STM32_I2C3_ER_NUMBER                89
#define STM32_I2C4_ER_NUMBER                100
#define STM32_I2C4_EV_NUMBER                101

/*
 * Low-power timer units.
 */
#define STM32_LPTIM1_HANDLER                Vector14C
#define STM32_LPTIM2_HANDLER                Vector150
#define STM32_LPTIM3_HANDLER                Vector1C8
#define STM32_LPTIM4_HANDLER                Vector1F8

#define STM32_LPTIM1_NUMBER                 67
#define STM32_LPTIM2_NUMBER                 68
#define STM32_LPTIM3_NUMBER                 98
#define STM32_LPTIM4_NUMBER                 110

/*
 * OCTOSPI units.
 */
#define STM32_OCTOSPI1_HANDLER              Vector170
#define STM32_OCTOSPI2_HANDLER              Vector220

#define STM32_OCTOSPI1_NUMBER               76
#define STM32_OCTOSPI2_NUMBER               120

/*
 * SAI units.
 */
#define STM32_SAI1_HANDLER                  Vector1A8
#define STM32_SAI2_HANDLER                  Vector1AC

#define STM32_SAI1_NUMBER                   90
#define STM32_SAI2_NUMBER                   91

/*
 * SDMMC units.
 */
#define STM32_SDMMC1_HANDLER                Vector178
#define STM32_SDMMC2_HANDLER                Vector17C

#define STM32_SDMMC1_NUMBER                 78
#define STM32_SDMMC2_NUMBER                 79

/*
 * SPI units.
 */
#define STM32_SPI1_HANDLER                  Vector12C
#define STM32_SPI2_HANDLER                  Vector130
#define STM32_SPI3_HANDLER                  Vector1CC

#define STM32_SPI1_NUMBER                   59
#define STM32_SPI2_NUMBER                   60
#define STM32_SPI3_NUMBER                   99

/*
 * System helper units.
 */
#define STM32_TSC_HANDLER                   Vector1B0
#define STM32_RNG_HANDLER                   Vector1B8
#define STM32_HASH_HANDLER                  Vector1C0
#define STM32_UCPD1_HANDLER                 Vector1E8
#define STM32_DMA2D_HANDLER                 Vector218
#define STM32_DCMI_PSSI_HANDLER             Vector21C

#define STM32_TSC_NUMBER                    92
#define STM32_RNG_NUMBER                    94
#define STM32_HASH_NUMBER                   96
#define STM32_UCPD1_NUMBER                  106
#define STM32_DMA2D_NUMBER                  118
#define STM32_DCMI_PSSI_NUMBER              119

/*
 * TIM units.
 */
#define STM32_TIM1_BRK_TERR_IERR_HANDLER    VectorE4
#define STM32_TIM1_UP_HANDLER               VectorE8
#define STM32_TIM1_TRGCO_DIR_IDX_HANDLER    VectorEC
#define STM32_TIM1_CC_HANDLER               VectorF0
#define STM32_TIM2_HANDLER                  VectorF4
#define STM32_TIM3_HANDLER                  VectorF8
#define STM32_TIM4_HANDLER                  VectorFC
#define STM32_TIM5_HANDLER                  Vector100
#define STM32_TIM6_HANDLER                  Vector104
#define STM32_TIM7_HANDLER                  Vector108
#define STM32_TIM8_BRK_TERR_IERR_HANDLER    Vector10C
#define STM32_TIM8_UP_HANDLER               Vector110
#define STM32_TIM8_TRGCO_DIR_IDX_HANDLER    Vector114
#define STM32_TIM8_CC_HANDLER               Vector118
#define STM32_TIM15_HANDLER                 Vector154
#define STM32_TIM16_HANDLER                 Vector158
#define STM32_TIM17_HANDLER                 Vector15C

#define STM32_TIM1_BRK_HANDLER              STM32_TIM1_BRK_TERR_IERR_HANDLER
#define STM32_TIM1_TRGCO_HANDLER            STM32_TIM1_TRGCO_DIR_IDX_HANDLER
#define STM32_TIM8_BRK_HANDLER              STM32_TIM8_BRK_TERR_IERR_HANDLER
#define STM32_TIM8_TRGCO_HANDLER            STM32_TIM8_TRGCO_DIR_IDX_HANDLER

#define STM32_TIM1_BRK_TERR_IERR_NUMBER     41
#define STM32_TIM1_UP_NUMBER                42
#define STM32_TIM1_TRGCO_DIR_IDX_NUMBER     43
#define STM32_TIM1_CC_NUMBER                44
#define STM32_TIM2_NUMBER                   45
#define STM32_TIM3_NUMBER                   46
#define STM32_TIM4_NUMBER                   47
#define STM32_TIM5_NUMBER                   48
#define STM32_TIM6_NUMBER                   49
#define STM32_TIM7_NUMBER                   50
#define STM32_TIM8_BRK_TERR_IERR_NUMBER     51
#define STM32_TIM8_UP_NUMBER                52
#define STM32_TIM8_TRGCO_DIR_IDX_NUMBER     53
#define STM32_TIM8_CC_NUMBER                54
#define STM32_TIM15_NUMBER                  69
#define STM32_TIM16_NUMBER                  70
#define STM32_TIM17_NUMBER                  71

#define STM32_TIM1_BRK_NUMBER               STM32_TIM1_BRK_TERR_IERR_NUMBER
#define STM32_TIM1_TRGCO_NUMBER             STM32_TIM1_TRGCO_DIR_IDX_NUMBER
#define STM32_TIM8_BRK_NUMBER               STM32_TIM8_BRK_TERR_IERR_NUMBER
#define STM32_TIM8_TRGCO_NUMBER             STM32_TIM8_TRGCO_DIR_IDX_NUMBER

/*
 * USART/UART units.
 */
#define STM32_USART1_HANDLER                Vector134
#define STM32_USART2_HANDLER                Vector138
#define STM32_USART3_HANDLER                Vector13C
#define STM32_UART4_HANDLER                 Vector140
#define STM32_UART5_HANDLER                 Vector144
#define STM32_LPUART1_HANDLER               Vector148

#define STM32_USART1_NUMBER                 61
#define STM32_USART2_NUMBER                 62
#define STM32_USART3_NUMBER                 63
#define STM32_UART4_NUMBER                  64
#define STM32_UART5_NUMBER                  65
#define STM32_LPUART1_NUMBER                66

/*
 * USB units.
 */
#define STM32_USB1_HANDLER                  Vector164

#define STM32_USB1_NUMBER                   73
/** @} */

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

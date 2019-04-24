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
 * @file    STM32G0xx/stm32_registry.h
 * @brief   STM32G0xx capabilities registry.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef STM32_REGISTRY_H
#define STM32_REGISTRY_H

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/**
 * @name    STM32G0xx capabilities
 * @{
 */

/*===========================================================================*/
/* Common.                                                                   */
/*===========================================================================*/

/* RTC attributes.*/
#define STM32_HAS_RTC                       TRUE
#define STM32_RTC_HAS_SUBSECONDS            TRUE
#define STM32_RTC_HAS_PERIODIC_WAKEUPS      TRUE
#define STM32_RTC_NUM_ALARMS                2
#define STM32_RTC_STORAGE_SIZE              20
#define STM32_RTC_COMMON_HANDLER            Vector48
#define STM32_RTC_COMMON_NUMBER             2
#define STM32_RTC_ALARM_EXTI                18
#define STM32_RTC_TAMP_STAMP_EXTI           19
#define STM32_RTC_WKUP_EXTI                 20
#define STM32_RTC_IRQ_ENABLE() do {                                         \
  nvicEnableVector(STM32_RTC_COMMON_NUMBER,                                 \
                   STM32_IRQ_EXTI19_21_IRQ_PRIORITY);                       \
} while (false)

#if defined(STM32G081xx) || defined(__DOXYGEN__)
#define STM32_HAS_RNG1                      TRUE
#define STM32_HAS_HASH1                     FALSE
#define STM32_HAS_AES1                      TRUE
#else
#define STM32_HAS_RNG1                      FALSE
#define STM32_HAS_HASH1                     FALSE
#define STM32_HAS_AES1                      FALSE
#endif

/*===========================================================================*/
/* STM32G070xx.                                                              */
/*===========================================================================*/

#if defined(STM32G070xx) || defined(__DOXYGEN__)

/* ADC attributes.*/
#define STM32_HAS_ADC1                      TRUE
#define STM32_ADC1_HANDLER                  Vector70
#define STM32_ADC1_NUMBER                   12

#define STM32_HAS_ADC2                      FALSE
#define STM32_HAS_ADC3                      FALSE
#define STM32_HAS_ADC4                      FALSE

/* CAN attributes.*/
#define STM32_HAS_CAN1                      FALSE
#define STM32_HAS_CAN2                      FALSE
#define STM32_HAS_CAN3                      FALSE

/* DAC attributes.*/
#define STM32_HAS_DAC1_CH1                  FALSE
#define STM32_HAS_DAC1_CH2                  FALSE
#define STM32_HAS_DAC2_CH1                  FALSE
#define STM32_HAS_DAC2_CH2                  FALSE

/* DMA attributes.*/
#define STM32_ADVANCED_DMA                  TRUE
#define STM32_DMA_SUPPORTS_DMAMUX           TRUE
#define STM32_DMA_SUPPORTS_CSELR            FALSE

#define STM32_DMA1_NUM_CHANNELS             7
#define STM32_DMA1_CH1_HANDLER              Vector64
#define STM32_DMA1_CH23_HANDLER             Vector68
#define STM32_DMA1_CH4567_HANDLER           Vector6C
#define STM32_DMA1_CH1_NUMBER               9
#define STM32_DMA1_CH23_NUMBER              10
#define STM32_DMA1_CH4567_NUMBER            11

#define STM32_DMA2_NUM_CHANNELS             0

/* ETH attributes.*/
#define STM32_HAS_ETH                       FALSE

/* EXTI attributes.*/
#define STM32_EXTI_NUM_LINES                16
#define STM32_EXTI_IMR1_MASK                0xFFF80000U

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

/* GPIO attributes.*/
#define STM32_HAS_GPIOA                     TRUE
#define STM32_HAS_GPIOB                     TRUE
#define STM32_HAS_GPIOC                     TRUE
#define STM32_HAS_GPIOD                     TRUE
#define STM32_HAS_GPIOE                     FALSE
#define STM32_HAS_GPIOF                     TRUE
#define STM32_HAS_GPIOG                     FALSE
#define STM32_HAS_GPIOH                     FALSE
#define STM32_HAS_GPIOI                     FALSE
#define STM32_HAS_GPIOJ                     FALSE
#define STM32_HAS_GPIOK                     FALSE
#define STM32_GPIO_EN_MASK                  (RCC_IOPENR_GPIOAEN |           \
                                             RCC_IOPENR_GPIOBEN |           \
                                             RCC_IOPENR_GPIOCEN |           \
                                             RCC_IOPENR_GPIODEN |           \
                                             RCC_IOPENR_GPIOFEN)

/* I2C attributes.*/
#define STM32_HAS_I2C1                      TRUE
#define STM32_I2C1_GLOBAL_HANDLER           Vector9C
#define STM32_I2C1_GLOBAL_NUMBER            23

#define STM32_HAS_I2C2                      TRUE
#define STM32_I2C1_GLOBAL_HANDLER           VectorA0
#define STM32_I2C1_GLOBAL_NUMBER            24

#define STM32_HAS_I2C3                      FALSE
#define STM32_HAS_I2C4                      FALSE

/* OCTOSPI attributes.*/
#define STM32_HAS_OCTOSPI1                  FALSE
#define STM32_HAS_OCTOSPI2                  FALSE

/* QUADSPI attributes.*/
#define STM32_HAS_QUADSPI1                  FALSE

/* SDMMC attributes.*/
#define STM32_HAS_SDMMC1                    FALSE
#define STM32_HAS_SDMMC2                    FALSE

/* SPI attributes.*/
#define STM32_HAS_SPI1                      TRUE
#define STM32_SPI1_SUPPORTS_I2S             TRUE

#define STM32_HAS_SPI2                      TRUE
#define STM32_SPI2_SUPPORTS_I2S             FALSE

#define STM32_HAS_SPI3                      FALSE
#define STM32_HAS_SPI4                      FALSE
#define STM32_HAS_SPI5                      FALSE
#define STM32_HAS_SPI6                      FALSE

/* TIM attributes.*/
#define STM32_TIM_MAX_CHANNELS              6

#define STM32_HAS_TIM1                      TRUE
#define STM32_TIM1_IS_32BITS                FALSE
#define STM32_TIM1_CHANNELS                 6
#define STM32_TIM1_UP_HANDLER               Vector74
#define STM32_TIM1_CC_HANDLER               Vector78
#define STM32_TIM1_UP_NUMBER                13
#define STM32_TIM1_CC_NUMBER                14

#define STM32_HAS_TIM3                      TRUE
#define STM32_TIM3_IS_32BITS                FALSE
#define STM32_TIM3_CHANNELS                 4
#define STM32_TIM3_HANDLER                  VectorB4
#define STM32_TIM3_NUMBER                   29

#define STM32_HAS_TIM6                      TRUE
#define STM32_TIM6_IS_32BITS                FALSE
#define STM32_TIM6_CHANNELS                 0
#define STM32_TIM6_HANDLER                  Vector84
#define STM32_TIM6_NUMBER                   17

#define STM32_HAS_TIM7                      TRUE
#define STM32_TIM7_IS_32BITS                FALSE
#define STM32_TIM7_CHANNELS                 0
#define STM32_TIM7_HANDLER                  Vector88
#define STM32_TIM7_NUMBER                   18

#define STM32_HAS_TIM14                     TRUE
#define STM32_TIM14_IS_32BITS               FALSE
#define STM32_TIM14_CHANNELS                1
#define STM32_TIM14_HANDLER                 Vector8C
#define STM32_TIM14_NUMBER                  19

#define STM32_HAS_TIM15                     TRUE
#define STM32_TIM15_IS_32BITS               FALSE
#define STM32_TIM15_CHANNELS                2
#define STM32_TIM15_HANDLER                 Vector90
#define STM32_TIM15_NUMBER                  20

#define STM32_HAS_TIM16                     TRUE
#define STM32_TIM16_IS_32BITS               FALSE
#define STM32_TIM16_CHANNELS                2
#define STM32_TIM16_HANDLER                 Vector94
#define STM32_TIM16_NUMBER                  21

#define STM32_HAS_TIM17                     TRUE
#define STM32_TIM17_IS_32BITS               FALSE
#define STM32_TIM17_CHANNELS                2
#define STM32_TIM17_HANDLER                 Vector98
#define STM32_TIM17_NUMBER                  22

#define STM32_HAS_TIM2                      FALSE
#define STM32_HAS_TIM4                      FALSE
#define STM32_HAS_TIM5                      FALSE
#define STM32_HAS_TIM8                      FALSE
#define STM32_HAS_TIM9                      FALSE
#define STM32_HAS_TIM10                     FALSE
#define STM32_HAS_TIM11                     FALSE
#define STM32_HAS_TIM12                     FALSE
#define STM32_HAS_TIM13                     FALSE
#define STM32_HAS_TIM18                     FALSE
#define STM32_HAS_TIM19                     FALSE
#define STM32_HAS_TIM20                     FALSE
#define STM32_HAS_TIM21                     FALSE
#define STM32_HAS_TIM22                     FALSE

/* USART attributes.*/
#define STM32_HAS_USART1                    TRUE
#define STM32_HAS_USART2                    TRUE
#define STM32_HAS_USART3                    TRUE
#define STM32_HAS_UART4                     TRUE
#define STM32_HAS_LPUART1                   TRUE
#define STM32_HAS_UART5                     FALSE
#define STM32_HAS_USART6                    FALSE
#define STM32_HAS_UART7                     FALSE
#define STM32_HAS_UART8                     FALSE

/* USB attributes.*/
#define STM32_HAS_OTG1                      FALSE
#define STM32_HAS_OTG2                      FALSE
#define STM32_HAS_USB                       FALSE

/* IWDG attributes.*/
#define STM32_HAS_IWDG                      TRUE
#define STM32_IWDG_IS_WINDOWED              TRUE

/* LTDC attributes.*/
#define STM32_HAS_LTDC                      FALSE

/* DMA2D attributes.*/
#define STM32_HAS_DMA2D                     FALSE

/* FSMC attributes.*/
#define STM32_HAS_FSMC                      FALSE

/* CRC attributes.*/
#define STM32_HAS_CRC                       FALSE

/* DCMI attributes.*/
#define STM32_HAS_DCMI                      FALSE

#endif /* defined(STM32G070xx) */

/** @} */

#endif /* STM32_REGISTRY_H */

/** @} */

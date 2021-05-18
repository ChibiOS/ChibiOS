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
 * @file    STM32WLxx/stm32_registry.h
 * @brief   STM32WLxx capabilities registry.
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
 * @name    STM32WLxx capabilities
 * @{
 */

/* ADC attributes.*/
#define STM32_HAS_ADC1                      TRUE
#define STM32_HAS_ADC2                      FALSE
#define STM32_HAS_ADC3                      FALSE
#define STM32_HAS_ADC4                      FALSE

/* CAN attributes.*/
#define STM32_HAS_CAN1                      FALSE
#define STM32_HAS_CAN2                      FALSE
#define STM32_HAS_CAN3                      FALSE

/* CPU2 attributes.*/
#if defined(STM32WL55xx) || defined(STM32WL54xx) || defined(__DOXYGEN__)
#define STM32_HAS_CPU2                      TRUE
#else
#define STM32_HAS_CPU2                      FALSE
#endif /* defined(STM32WL55xx) || defined(STM32WL54xx) */

/* CRC attributes.*/
#define STM32_HAS_CRC                       TRUE
#define STM32_CRC_PROGRAMMABLE              TRUE

/* CRYP attributes.*/
#define STM32_HAS_CRYP1                     TRUE

/* DAC attributes.*/
#define STM32_HAS_DAC1_CH1                  TRUE
#define STM32_HAS_DAC1_CH2                  FALSE
#define STM32_HAS_DAC2_CH1                  FALSE
#define STM32_HAS_DAC2_CH2                  FALSE

/* DMA attributes.*/
#define STM32_ADVANCED_DMA                  TRUE
#define STM32_DMA_SUPPORTS_DMAMUX           TRUE
#define STM32_DMA_SUPPORTS_CSELR            FALSE
#define STM32_DMA1_NUM_CHANNELS             7
#define STM32_DMA2_NUM_CHANNELS             7

/* DMA2D attributes.*/
#define STM32_HAS_DMA2D                     FALSE

/* ETH attributes.*/
#define STM32_HAS_ETH                       FALSE

/* EXTI attributes.*/
#define STM32_EXTI_NUM_LINES                47
#define STM32_EXTI_IMR1_MASK                0xFF9E0000U
#define STM32_EXTI_IMR2_MASK                0xFFFFDFFBU

/* Flash attributes.*/
#define STM32_FLASH_NUMBER_OF_BANKS         1
#if !defined(STM32_FLASH_SECTORS_PER_BANK) || defined(__DOXYGEN__)
#define STM32_FLASH_SECTORS_PER_BANK        256 /* Maximum, can be redefined.*/
#endif

/* FSMC attributes.*/
#define STM32_HAS_FSMC                      FALSE

/* GPIO attributes.*/
#define STM32_HAS_GPIOA                     TRUE
#define STM32_HAS_GPIOB                     TRUE
#define STM32_HAS_GPIOC                     TRUE
#define STM32_HAS_GPIOD                     FALSE
#define STM32_HAS_GPIOE                     FALSE
#define STM32_HAS_GPIOF                     FALSE
#define STM32_HAS_GPIOG                     FALSE
#define STM32_HAS_GPIOH                     FALSE
#define STM32_HAS_GPIOI                     FALSE
#define STM32_HAS_GPIOJ                     FALSE
#define STM32_HAS_GPIOK                     FALSE
#define STM32_GPIO_EN_MASK                  (RCC_AHB2ENR_GPIOAEN |          \
                                             RCC_AHB2ENR_GPIOBEN |          \
                                             RCC_AHB2ENR_GPIOCEN)

/* GTZC attributes.*/
#if defined(STM32WL55xx) || defined(STM32WL54xx) || defined(__DOXYGEN__)
#define STM32_HAS_GTZC_TZSC                 TRUE
#define STM32_HAS_GTZC_TZIC                 TRUE
#else
#define STM32_HAS_GTZC_TZSC                 FALSE
#define STM32_HAS_GTZC_TZIC                 FALSE
#endif /* defined(STM32WL55xx) || defined(STM32WL54xx) */

#define STM32_HAS_HASH1                     FALSE

/* HSEM attributes.*/
#define STM32_HAS_HSEM                      TRUE
#define STM32_HSEM_SEMAPHORES               16

/* I2C attributes.*/
#define STM32_HAS_I2C1                      TRUE
#define STM32_HAS_I2C3                      TRUE
#define STM32_HAS_I2C2                      FALSE
#define STM32_HAS_I2C4                      FALSE

/* IWDG attributes.*/
#define STM32_HAS_IWDG                      TRUE
#define STM32_IWDG_IS_WINDOWED              TRUE

/* LTDC attributes.*/
#define STM32_HAS_LTDC                      FALSE

/* PKA attributes.*/
#define STM32_HAS_PKA                       TRUE

/* QUADSPI attributes.*/
#define STM32_HAS_QUADSPI1                  FALSE

/* Radio Transceiver (RTR) attributes.*/
#define STM32_HAS_RTR                       TRUE
#if defined(STM32WLE5xx) || defined(STM32WL55xx) || defined(__DOXYGEN__)
#define STM32_RTR_HAS_LORA_MODEM            TRUE
#else
#define STM32_RTR_HAS_LORA_MODEM            FALSE
#endif /* defined(STM32WLE5xx) || defined(STM32WL55xx) */
#define STM32_RTR_HAS_FSK_MODEM             TRUE
#define STM32_RTR_HAS_MSK_MODEM             TRUE
#define STM32_RTR_HAS_BPSK_MODEM            TRUE
#define STM32_RTR_IRQ_EXTI                  44
#define STM32_RTR_BUSY_EXTI                 45
#define STM32_RTR_IRQ_HANDLER               Vector108

/* RNG attributes.*/
#define STM32_HAS_RNG1                      TRUE

/* RTC attributes.*/
#define STM32_HAS_RTC                       TRUE
#define STM32_RTC_HAS_SUBSECONDS            TRUE
#define STM32_RTC_HAS_PERIODIC_WAKEUPS      TRUE
#define STM32_RTC_NUM_ALARMS                2
#define STM32_RTC_STORAGE_SIZE              32
#define STM32_RTC_TAMP_STAMP_HANDLER        Vector48
#define STM32_RTC_WKUP_HANDLER              Vector4C
#define STM32_RTC_ALARM_HANDLER             VectorE4
#define STM32_RTC_TAMP_STAMP_NUMBER         2
#define STM32_RTC_WKUP_NUMBER               3
#define STM32_RTC_ALARM_NUMBER              42
#define STM32_RTC_ALARM_EXTI                17
#define STM32_RTC_TAMP_STAMP_EXTI           19
#define STM32_RTC_WKUP_EXTI                 20
#define STM32_RTC_IRQ_ENABLE() do {                                         \
  nvicEnableVector(STM32_RTC_TAMP_STAMP_NUMBER, STM32_IRQ_EXTI19_PRIORITY); \
  nvicEnableVector(STM32_RTC_WKUP_NUMBER, STM32_IRQ_EXTI20_PRIORITY);       \
  nvicEnableVector(STM32_RTC_ALARM_NUMBER, STM32_IRQ_EXTI18_PRIORITY);      \
} while (false)

/* SDMMC attributes.*/
#define STM32_HAS_SDMMC1                    FALSE
#define STM32_HAS_SDMMC2                    FALSE

/* SPI attributes.*/
#define STM32_HAS_SPI1                      TRUE
#define STM32_SPI1_SUPPORTS_I2S             FALSE

#define STM32_HAS_SPI2                      TRUE
#define STM32_SPI2_SUPPORTS_I2S             TRUE

#define STM32_HAS_SPIR                      TRUE
#define STM32_SPIR_SUPPORTS_I2S             FALSE

#define STM32_HAS_SPI3                      FALSE
#define STM32_HAS_SPI4                      FALSE
#define STM32_HAS_SPI5                      FALSE
#define STM32_HAS_SPI6                      FALSE

/* TIM attributes.*/
#define STM32_TIM_MAX_CHANNELS              6

#define STM32_HAS_TIM1                      TRUE
#define STM32_TIM1_IS_32BITS                FALSE
#define STM32_TIM1_CHANNELS                 6

#define STM32_HAS_TIM2                      TRUE
#define STM32_TIM2_IS_32BITS                TRUE
#define STM32_TIM2_CHANNELS                 4

#define STM32_HAS_TIM16                     TRUE
#define STM32_TIM16_IS_32BITS               FALSE
#define STM32_TIM16_CHANNELS                1

#define STM32_HAS_TIM17                     TRUE
#define STM32_TIM17_IS_32BITS               FALSE
#define STM32_TIM17_CHANNELS                1

#define STM32_HAS_TIM3                      FALSE
#define STM32_HAS_TIM4                      FALSE
#define STM32_HAS_TIM5                      FALSE
#define STM32_HAS_TIM6                      FALSE
#define STM32_HAS_TIM7                      FALSE
#define STM32_HAS_TIM8                      FALSE
#define STM32_HAS_TIM9                      FALSE
#define STM32_HAS_TIM10                     FALSE
#define STM32_HAS_TIM11                     FALSE
#define STM32_HAS_TIM12                     FALSE
#define STM32_HAS_TIM13                     FALSE
#define STM32_HAS_TIM14                     FALSE
#define STM32_HAS_TIM18                     FALSE
#define STM32_HAS_TIM19                     FALSE
#define STM32_HAS_TIM20                     FALSE
#define STM32_HAS_TIM21                     FALSE
#define STM32_HAS_TIM22                     FALSE

/* LPTIM attributes.*/
#define STM32_HAS_LPTIM1                    TRUE
#define STM32_HAS_LPTIM2                    TRUE
#define STM32_HAS_LPTIM3                    TRUE

/* USART attributes.*/
#define STM32_HAS_USART1                    TRUE
#define STM32_HAS_USART2                    TRUE
#define STM32_HAS_LPUART1                   TRUE
#define STM32_HAS_USART3                    FALSE
#define STM32_HAS_UART4                     FALSE
#define STM32_HAS_UART5                     FALSE
#define STM32_HAS_USART6                    FALSE
#define STM32_HAS_UART7                     FALSE
#define STM32_HAS_UART8                     FALSE

/* USB attributes.*/
#define STM32_HAS_USB                       FALSE

#define STM32_HAS_OTG1                      FALSE
#define STM32_HAS_OTG2                      FALSE

/** @} */

#endif /* STM32_REGISTRY_H */

/** @} */

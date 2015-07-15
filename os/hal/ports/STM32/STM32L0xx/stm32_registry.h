/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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
 * @file    STM32L0xx/stm32_registry.h
 * @brief   STM32L0xx capabilities registry.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _STM32_REGISTRY_H_
#define _STM32_REGISTRY_H_

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/**
 * @name    STM32L0xx capabilities
 * @{
 */
/*===========================================================================*/
/* STM32L051xx, STM32L061xx                                                  */
/*===========================================================================*/
#if defined(STM32L051xx) || defined(STM32L061xx) ||                         \
    defined(__DOXYGEN__)

/* ADC attributes.*/
#define STM32_HAS_ADC1                      TRUE
#define STM32_HAS_ADC2                      FALSE
#define STM32_HAS_ADC3                      FALSE
#define STM32_HAS_ADC4                      FALSE

/* CAN attributes.*/
#define STM32_HAS_CAN1                      FALSE
#define STM32_HAS_CAN2                      FALSE

/* DAC attributes.*/
#define STM32_HAS_DAC1_CH1                  FALSE
#define STM32_HAS_DAC1_CH2                  FALSE
#define STM32_HAS_DAC2_CH1                  FALSE
#define STM32_HAS_DAC2_CH2                  FALSE

/* DMA attributes.*/
#define STM32_ADVANCED_DMA                  FALSE
#define STM32_HAS_DMA1                      TRUE
#define STM32_HAS_DMA2                      FALSE
#define STM32_DMA_STREAMS                   5

/* ETH attributes.*/
#define STM32_HAS_ETH                       FALSE

/* EXTI attributes.*/
#define STM32_EXTI_NUM_CHANNELS             28

/* GPIO attributes.*/
#define STM32_HAS_GPIOA                     TRUE
#define STM32_HAS_GPIOB                     TRUE
#define STM32_HAS_GPIOC                     TRUE
#define STM32_HAS_GPIOD                     TRUE
#define STM32_HAS_GPIOE                     FALSE
#define STM32_HAS_GPIOF                     FALSE
#define STM32_HAS_GPIOG                     FALSE
#define STM32_HAS_GPIOH                     TRUE
#define STM32_HAS_GPIOI                     FALSE
#define STM32_GPIO_EN_MASK                  (RCC_AHBENR_GPIOAEN |           \
                                             RCC_AHBENR_GPIOBEN |           \
                                             RCC_AHBENR_GPIOCEN |           \
                                             RCC_AHBENR_GPIODEN |           \
                                             RCC_AHBENR_GPIOHEN)

/* I2C attributes.*/
#define STM32_HAS_I2C1                      TRUE
#define STM32_I2C_I2C1_RX_DMA_STREAM        0
#define STM32_I2C_I2C1_TX_DMA_STREAM        0

#define STM32_HAS_I2C2                      TRUE
#define STM32_I2C_I2C2_RX_DMA_STREAM        0
#define STM32_I2C_I2C2_TX_DMA_STREAM        0

#define STM32_HAS_I2C3                      FALSE

/* RTC attributes.*/
#define STM32_HAS_RTC                       TRUE
#define STM32_RTC_HAS_SUBSECONDS            TRUE
#define STM32_RTC_HAS_PERIODIC_WAKEUPS      FALSE
#define STM32_RTC_NUM_ALARMS                1
#define STM32_RTC_HAS_INTERRUPTS            FALSE

/* SDIO attributes.*/
#define STM32_HAS_SDIO                      FALSE

/* SPI attributes.*/
#define STM32_HAS_SPI1                      TRUE
#define STM32_SPI_SPI1_RX_DMA_STREAM        0
#define STM32_SPI_SPI1_TX_DMA_STREAM        0

#define STM32_HAS_SPI2                      TRUE
#define STM32_SPI_SPI2_RX_DMA_STREAM        0
#define STM32_SPI_SPI2_TX_DMA_STREAM        0

#define STM32_HAS_SPI3                      FALSE
#define STM32_HAS_SPI4                      FALSE
#define STM32_HAS_SPI5                      FALSE
#define STM32_HAS_SPI6                      FALSE

/* TIM attributes.*/
#define STM32_TIM_MAX_CHANNELS              4

#define STM32_HAS_TIM2                      TRUE
#define STM32_TIM2_IS_32BITS                FALSE
#define STM32_TIM2_CHANNELS                 4

#define STM32_HAS_TIM6                      TRUE
#define STM32_TIM6_IS_32BITS                FALSE
#define STM32_TIM6_CHANNELS                 0

#define STM32_HAS_TIM20                     TRUE
#define STM32_TIM20_IS_32BITS               FALSE
#define STM32_TIM20_CHANNELS                4

#define STM32_HAS_TIM21                     TRUE
#define STM32_TIM21_IS_32BITS               FALSE
#define STM32_TIM21_CHANNELS                4

#define STM32_HAS_TIM1                      FALSE
#define STM32_HAS_TIM3                      FALSE
#define STM32_HAS_TIM4                      FALSE
#define STM32_HAS_TIM5                      FALSE
#define STM32_HAS_TIM7                      FALSE
#define STM32_HAS_TIM8                      FALSE
#define STM32_HAS_TIM9                      FALSE
#define STM32_HAS_TIM10                     FALSE
#define STM32_HAS_TIM11                     FALSE
#define STM32_HAS_TIM12                     FALSE
#define STM32_HAS_TIM13                     FALSE
#define STM32_HAS_TIM14                     FALSE
#define STM32_HAS_TIM15                     FALSE
#define STM32_HAS_TIM16                     FALSE
#define STM32_HAS_TIM17                     FALSE
#define STM32_HAS_TIM18                     FALSE
#define STM32_HAS_TIM19                     FALSE

/* USART attributes.*/
#define STM32_HAS_USART1                    TRUE
#define STM32_UART_USART1_RX_DMA_STREAM     0
#define STM32_UART_USART1_TX_DMA_STREAM     0

#define STM32_HAS_USART2                    TRUE
#define STM32_UART_USART2_RX_DMA_STREAM     0
#define STM32_UART_USART2_TX_DMA_STREAM     0

#define STM32_HAS_USART3                    FALSE
#define STM32_HAS_UART4                     FALSE
#define STM32_HAS_UART5                     FALSE
#define STM32_HAS_USART6                    FALSE

/* USB attributes.*/
#define STM32_HAS_USB                       FALSE
#define STM32_HAS_OTG1                      FALSE
#define STM32_HAS_OTG2                      FALSE

/* LTDC attributes.*/
#define STM32_HAS_LTDC                      FALSE

/* DMA2D attributes.*/
#define STM32_HAS_DMA2D                     FALSE

/* FSMC attributes.*/
#define STM32_HAS_FSMC                      FALSE

/* CRC attributes.*/
#define STM32_HAS_CRC                       TRUE
#define STM32_CRC_PROGRAMMABLE              TRUE

/*===========================================================================*/
/* STM32L052xx, STM32L062xx.                                                 */
/*===========================================================================*/
#elif defined(STM32L052xx) || defined(STM32L062xx) ||                       \
    defined(__DOXYGEN__)

/* ADC attributes.*/
#define STM32_HAS_ADC1                      TRUE
#define STM32_HAS_ADC2                      FALSE
#define STM32_HAS_ADC3                      FALSE
#define STM32_HAS_ADC4                      FALSE

/* CAN attributes.*/
#define STM32_HAS_CAN1                      FALSE
#define STM32_HAS_CAN2                      FALSE

/* DAC attributes.*/
#define STM32_HAS_DAC1_CH1                  TRUE
#define STM32_DAC_DAC1_CH1_DMA_STREAM       0

#define STM32_HAS_DAC1_CH2                  FALSE
#define STM32_HAS_DAC2_CH1                  FALSE
#define STM32_HAS_DAC2_CH2                  FALSE

/* DMA attributes.*/
#define STM32_ADVANCED_DMA                  FALSE
#define STM32_HAS_DMA1                      TRUE
#define STM32_HAS_DMA2                      FALSE
#define STM32_DMA_STREAMS                   5

/* ETH attributes.*/
#define STM32_HAS_ETH                       FALSE

/* EXTI attributes.*/
#define STM32_EXTI_NUM_CHANNELS             28

/* GPIO attributes.*/
#define STM32_HAS_GPIOA                     TRUE
#define STM32_HAS_GPIOB                     TRUE
#define STM32_HAS_GPIOC                     TRUE
#define STM32_HAS_GPIOD                     TRUE
#define STM32_HAS_GPIOE                     FALSE
#define STM32_HAS_GPIOF                     FALSE
#define STM32_HAS_GPIOG                     FALSE
#define STM32_HAS_GPIOH                     TRUE
#define STM32_HAS_GPIOI                     FALSE
#define STM32_GPIO_EN_MASK                  (RCC_AHBENR_GPIOAEN |           \
                                             RCC_AHBENR_GPIOBEN |           \
                                             RCC_AHBENR_GPIOCEN |           \
                                             RCC_AHBENR_GPIODEN |           \
                                             RCC_AHBENR_GPIOHEN)

/* I2C attributes.*/
#define STM32_HAS_I2C1                      TRUE
#define STM32_I2C_I2C1_RX_DMA_STREAM        0
#define STM32_I2C_I2C1_TX_DMA_STREAM        0

#define STM32_HAS_I2C2                      TRUE
#define STM32_I2C_I2C2_RX_DMA_STREAM        0
#define STM32_I2C_I2C2_TX_DMA_STREAM        0

#define STM32_HAS_I2C3                      FALSE

/* RTC attributes.*/
#define STM32_HAS_RTC                       TRUE
#define STM32_RTC_HAS_SUBSECONDS            TRUE
#define STM32_RTC_HAS_PERIODIC_WAKEUPS      FALSE
#define STM32_RTC_NUM_ALARMS                1
#define STM32_RTC_HAS_INTERRUPTS            FALSE

/* SDIO attributes.*/
#define STM32_HAS_SDIO                      FALSE

/* SPI attributes.*/
#define STM32_HAS_SPI1                      TRUE
#define STM32_SPI_SPI1_RX_DMA_STREAM        0
#define STM32_SPI_SPI1_TX_DMA_STREAM        0

#define STM32_HAS_SPI2                      TRUE
#define STM32_SPI_SPI2_RX_DMA_STREAM        0
#define STM32_SPI_SPI2_TX_DMA_STREAM        0

#define STM32_HAS_SPI3                      FALSE
#define STM32_HAS_SPI4                      FALSE
#define STM32_HAS_SPI5                      FALSE
#define STM32_HAS_SPI6                      FALSE

/* TIM attributes.*/
#define STM32_TIM_MAX_CHANNELS              4

#define STM32_HAS_TIM2                      TRUE
#define STM32_TIM2_IS_32BITS                FALSE
#define STM32_TIM2_CHANNELS                 4

#define STM32_HAS_TIM6                      TRUE
#define STM32_TIM6_IS_32BITS                FALSE
#define STM32_TIM6_CHANNELS                 0

#define STM32_HAS_TIM20                     TRUE
#define STM32_TIM20_IS_32BITS               FALSE
#define STM32_TIM20_CHANNELS                4

#define STM32_HAS_TIM21                     TRUE
#define STM32_TIM21_IS_32BITS               FALSE
#define STM32_TIM21_CHANNELS                4

#define STM32_HAS_TIM1                      FALSE
#define STM32_HAS_TIM3                      FALSE
#define STM32_HAS_TIM4                      FALSE
#define STM32_HAS_TIM5                      FALSE
#define STM32_HAS_TIM7                      FALSE
#define STM32_HAS_TIM8                      FALSE
#define STM32_HAS_TIM9                      FALSE
#define STM32_HAS_TIM10                     FALSE
#define STM32_HAS_TIM11                     FALSE
#define STM32_HAS_TIM12                     FALSE
#define STM32_HAS_TIM13                     FALSE
#define STM32_HAS_TIM14                     FALSE
#define STM32_HAS_TIM15                     FALSE
#define STM32_HAS_TIM16                     FALSE
#define STM32_HAS_TIM17                     FALSE
#define STM32_HAS_TIM18                     FALSE
#define STM32_HAS_TIM19                     FALSE

/* USART attributes.*/
#define STM32_HAS_USART1                    TRUE
#define STM32_UART_USART1_RX_DMA_STREAM     0
#define STM32_UART_USART1_TX_DMA_STREAM     0

#define STM32_HAS_USART2                    TRUE
#define STM32_UART_USART2_RX_DMA_STREAM     0
#define STM32_UART_USART2_TX_DMA_STREAM     0

#define STM32_HAS_USART3                    FALSE
#define STM32_HAS_UART4                     FALSE
#define STM32_HAS_UART5                     FALSE
#define STM32_HAS_USART6                    FALSE

/* USB attributes.*/
#define STM32_HAS_USB                       TRUE
#define STM32_USB_ACCESS_SCHEME_2x16        TRUE
#define STM32_USB_PMA_SIZE                  1024
#define STM32_USB_HAS_BCDR                  TRUE

#define STM32_HAS_OTG1                      FALSE
#define STM32_HAS_OTG2                      FALSE

/* LTDC attributes.*/
#define STM32_HAS_LTDC                      FALSE

/* DMA2D attributes.*/
#define STM32_HAS_DMA2D                     FALSE

/* FSMC attributes.*/
#define STM32_HAS_FSMC                      FALSE

/* CRC attributes.*/
#define STM32_HAS_CRC                       TRUE
#define STM32_CRC_PROGRAMMABLE              TRUE

/*===========================================================================*/
/* STM32L053xx, STM32L063xx.                                                 */
/*===========================================================================*/
#elif defined(STM32L053xx) || defined(STM32L063xx) ||                       \
    defined(__DOXYGEN__)

/* ADC attributes.*/
#define STM32_HAS_ADC1                      TRUE
#define STM32_HAS_ADC2                      FALSE
#define STM32_HAS_ADC3                      FALSE
#define STM32_HAS_ADC4                      FALSE

/* CAN attributes.*/
#define STM32_HAS_CAN1                      FALSE
#define STM32_HAS_CAN2                      FALSE

/* DAC attributes.*/
#define STM32_HAS_DAC1_CH1                  FALSE
#define STM32_DAC_DAC1_CH1_DMA_STREAM       0

#define STM32_HAS_DAC1_CH2                  FALSE
#define STM32_HAS_DAC2_CH1                  FALSE
#define STM32_HAS_DAC2_CH2                  FALSE

/* DMA attributes.*/
#define STM32_ADVANCED_DMA                  FALSE
#define STM32_HAS_DMA1                      TRUE
#define STM32_HAS_DMA2                      FALSE
#define STM32_DMA_STREAMS                   5

/* ETH attributes.*/
#define STM32_HAS_ETH                       FALSE

/* EXTI attributes.*/
#define STM32_EXTI_NUM_CHANNELS             28

/* GPIO attributes.*/
#define STM32_HAS_GPIOA                     TRUE
#define STM32_HAS_GPIOB                     TRUE
#define STM32_HAS_GPIOC                     TRUE
#define STM32_HAS_GPIOD                     TRUE
#define STM32_HAS_GPIOE                     FALSE
#define STM32_HAS_GPIOF                     FALSE
#define STM32_HAS_GPIOG                     FALSE
#define STM32_HAS_GPIOH                     TRUE
#define STM32_HAS_GPIOI                     FALSE
#define STM32_GPIO_EN_MASK                  (RCC_AHBENR_GPIOAEN |           \
                                             RCC_AHBENR_GPIOBEN |           \
                                             RCC_AHBENR_GPIOCEN |           \
                                             RCC_AHBENR_GPIODEN |           \
                                             RCC_AHBENR_GPIOHEN)

/* I2C attributes.*/
#define STM32_HAS_I2C1                      TRUE
#define STM32_I2C_I2C1_RX_DMA_STREAM        0
#define STM32_I2C_I2C1_TX_DMA_STREAM        0

#define STM32_HAS_I2C2                      TRUE
#define STM32_I2C_I2C2_RX_DMA_STREAM        0
#define STM32_I2C_I2C2_TX_DMA_STREAM        0

#define STM32_HAS_I2C3                      FALSE

/* RTC attributes.*/
#define STM32_HAS_RTC                       TRUE
#define STM32_RTC_HAS_SUBSECONDS            TRUE
#define STM32_RTC_HAS_PERIODIC_WAKEUPS      FALSE
#define STM32_RTC_NUM_ALARMS                1
#define STM32_RTC_HAS_INTERRUPTS            FALSE

/* SDIO attributes.*/
#define STM32_HAS_SDIO                      FALSE

/* SPI attributes.*/
#define STM32_HAS_SPI1                      TRUE
#define STM32_SPI_SPI1_RX_DMA_STREAM        0
#define STM32_SPI_SPI1_TX_DMA_STREAM        0

#define STM32_HAS_SPI2                      TRUE
#define STM32_SPI_SPI2_RX_DMA_STREAM        0
#define STM32_SPI_SPI2_TX_DMA_STREAM        0

#define STM32_HAS_SPI3                      FALSE
#define STM32_HAS_SPI4                      FALSE
#define STM32_HAS_SPI5                      FALSE
#define STM32_HAS_SPI6                      FALSE

/* TIM attributes.*/
#define STM32_TIM_MAX_CHANNELS              4

#define STM32_HAS_TIM2                      TRUE
#define STM32_TIM2_IS_32BITS                FALSE
#define STM32_TIM2_CHANNELS                 4

#define STM32_HAS_TIM6                      TRUE
#define STM32_TIM6_IS_32BITS                FALSE
#define STM32_TIM6_CHANNELS                 0

#define STM32_HAS_TIM20                     TRUE
#define STM32_TIM20_IS_32BITS               FALSE
#define STM32_TIM20_CHANNELS                4

#define STM32_HAS_TIM21                     TRUE
#define STM32_TIM21_IS_32BITS               FALSE
#define STM32_TIM21_CHANNELS                4

#define STM32_HAS_TIM1                      FALSE
#define STM32_HAS_TIM3                      FALSE
#define STM32_HAS_TIM4                      FALSE
#define STM32_HAS_TIM5                      FALSE
#define STM32_HAS_TIM7                      FALSE
#define STM32_HAS_TIM8                      FALSE
#define STM32_HAS_TIM9                      FALSE
#define STM32_HAS_TIM10                     FALSE
#define STM32_HAS_TIM11                     FALSE
#define STM32_HAS_TIM12                     FALSE
#define STM32_HAS_TIM13                     FALSE
#define STM32_HAS_TIM14                     FALSE
#define STM32_HAS_TIM15                     FALSE
#define STM32_HAS_TIM16                     FALSE
#define STM32_HAS_TIM17                     FALSE
#define STM32_HAS_TIM18                     FALSE
#define STM32_HAS_TIM19                     FALSE

/* USART attributes.*/
#define STM32_HAS_USART1                    TRUE
#define STM32_UART_USART1_RX_DMA_STREAM     0
#define STM32_UART_USART1_TX_DMA_STREAM     0

#define STM32_HAS_USART2                    TRUE
#define STM32_UART_USART2_RX_DMA_STREAM     0
#define STM32_UART_USART2_TX_DMA_STREAM     0

#define STM32_HAS_USART3                    FALSE
#define STM32_HAS_UART4                     FALSE
#define STM32_HAS_UART5                     FALSE
#define STM32_HAS_USART6                    FALSE

/* USB attributes.*/
#define STM32_HAS_USB                       TRUE
#define STM32_USB_ACCESS_SCHEME_2x16        TRUE
#define STM32_USB_PMA_SIZE                  1024
#define STM32_USB_HAS_BCDR                  TRUE

#define STM32_HAS_OTG1                      FALSE
#define STM32_HAS_OTG2                      FALSE

/* LTDC attributes.*/
#define STM32_HAS_LTDC                      FALSE

/* DMA2D attributes.*/
#define STM32_HAS_DMA2D                     FALSE

/* FSMC attributes.*/
#define STM32_HAS_FSMC                      FALSE

/* CRC attributes.*/
#define STM32_HAS_CRC                       TRUE
#define STM32_CRC_PROGRAMMABLE              TRUE

#else
#error "STM32L0xx device not specified"
#endif

/** @} */

#endif /* _STM32_REGISTRY_H_ */

/** @} */

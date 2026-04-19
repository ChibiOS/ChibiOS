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
 * @file    STM32U5xx/stm32_registry.h
 * @brief   STM32U5xx capabilities registry.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef STM32_REGISTRY_H
#define STM32_REGISTRY_H

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

#if defined(STM32U575xx) || defined(__DOXYGEN__)

/* Cache attributes.*/
#define STM32_HAS_ICACHE                    TRUE
#define STM32_ICACHE_HAS_REGIONS            FALSE

/* EXTI attributes.*/
#define STM32_EXTI_HAS_CR                   TRUE
#define STM32_EXTI_SEPARATE_RF              TRUE
#define STM32_EXTI_NUM_LINES                24
#define STM32_EXTI_IMR1_MASK                0xFF000000U

/* GPIO attributes.*/
#define STM32_HAS_GPIOA                     TRUE
#define STM32_HAS_GPIOB                     TRUE
#define STM32_HAS_GPIOC                     TRUE
#define STM32_HAS_GPIOD                     TRUE
#define STM32_HAS_GPIOE                     TRUE
#define STM32_HAS_GPIOF                     TRUE
#define STM32_HAS_GPIOG                     TRUE
#define STM32_HAS_GPIOH                     TRUE
#define STM32_HAS_GPIOI                     FALSE
#define STM32_HAS_GPIOJ                     FALSE
#define STM32_HAS_GPIOK                     FALSE
#define STM32_GPIO_EN_MASK                  (RCC_AHB2ENR1_GPIOAEN |         \
                                             RCC_AHB2ENR1_GPIOBEN |         \
                                             RCC_AHB2ENR1_GPIOCEN |         \
                                             RCC_AHB2ENR1_GPIODEN |         \
                                             RCC_AHB2ENR1_GPIOEEN |         \
                                             RCC_AHB2ENR1_GPIOFEN |         \
                                             RCC_AHB2ENR1_GPIOGEN |         \
                                             RCC_AHB2ENR1_GPIOHEN)

/* Serial-capable peripherals.*/
#define STM32_HAS_USART1                    TRUE
#define STM32_HAS_USART2                    TRUE
#define STM32_HAS_USART3                    TRUE
#define STM32_HAS_UART4                     TRUE
#define STM32_HAS_UART5                     TRUE
#define STM32_HAS_USART6                    TRUE
#define STM32_HAS_UART7                     FALSE
#define STM32_HAS_UART8                     FALSE
#define STM32_HAS_UART9                     FALSE
#define STM32_HAS_USART10                   FALSE
#define STM32_HAS_LPUART1                   TRUE

/* RTC attributes.*/
#define STM32_HAS_RTC                       TRUE

#else
#error "STM32U5 device not supported yet by this registry"
#endif

#endif /* STM32_REGISTRY_H */

/** @} */

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

#ifndef MCUCONF_H
#define MCUCONF_H

/*
 * STM32U5xx drivers configuration.
 *
 * This first demo-oriented configuration keeps the device on its reset clock
 * setup and only enables the pieces required by RT-STM32-MULTI: PAL plus the
 * STLINK VCOM through LPUART1.
 */

#define STM32U5xx_MCUCONF
#define STM32U575_MCUCONF

/*
 * HAL driver general settings.
 */
#define STM32_NO_INIT                       FALSE
#define STM32_CLOCK_DYNAMIC                 FALSE

/*
 * Clock settings.
 */
#define STM32_HSI16_ENABLED                 FALSE
#define STM32_HSI48_ENABLED                 FALSE
#define STM32_HSE_ENABLED                   FALSE
#define STM32_LSI_ENABLED                   FALSE
#define STM32_LSE_ENABLED                   FALSE
#define STM32_LPUART1SEL                    RCC_CCIPR3_LPUART1SEL_PCLK3

/*
 * IRQ system settings.
 */
#define STM32_IRQ_EXTI0_PRIORITY            6
#define STM32_IRQ_EXTI1_PRIORITY            6
#define STM32_IRQ_EXTI2_PRIORITY            6
#define STM32_IRQ_EXTI3_PRIORITY            6
#define STM32_IRQ_EXTI4_PRIORITY            6
#define STM32_IRQ_EXTI5_PRIORITY            6
#define STM32_IRQ_EXTI6_PRIORITY            6
#define STM32_IRQ_EXTI7_PRIORITY            6
#define STM32_IRQ_EXTI8_PRIORITY            6
#define STM32_IRQ_EXTI9_PRIORITY            6
#define STM32_IRQ_EXTI10_PRIORITY           6
#define STM32_IRQ_EXTI11_PRIORITY           6
#define STM32_IRQ_EXTI12_PRIORITY           6
#define STM32_IRQ_EXTI13_PRIORITY           6
#define STM32_IRQ_EXTI14_PRIORITY           6
#define STM32_IRQ_EXTI15_PRIORITY           6

#define STM32_IRQ_LPUART1_PRIORITY          12

/*
 * SERIAL driver system settings.
 */
#define STM32_SERIAL_USE_USART1             FALSE
#define STM32_SERIAL_USE_USART2             FALSE
#define STM32_SERIAL_USE_USART3             FALSE
#define STM32_SERIAL_USE_UART4              FALSE
#define STM32_SERIAL_USE_UART5              FALSE
#define STM32_SERIAL_USE_USART6             FALSE
#define STM32_SERIAL_USE_UART7              FALSE
#define STM32_SERIAL_USE_UART8              FALSE
#define STM32_SERIAL_USE_UART9              FALSE
#define STM32_SERIAL_USE_USART10            FALSE
#define STM32_SERIAL_USE_LPUART1            TRUE

#endif /* MCUCONF_H */

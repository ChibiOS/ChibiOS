/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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

/*
 * STM8 drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the driver
 * is enabled in halconf.h.
 */

/*
 * HAL general settings.
 */
#define STM8L_NO_CLOCK_INIT             FALSE
#define STM8L_HSI_ENABLED               TRUE
#define STM8L_LSI_ENABLED               TRUE
#define STM8L_HSE_ENABLED               FALSE
#define STM8L_LSE_ENABLED               TRUE
#define STM8L_SYSCLK_SOURCE             CLK_SYSSEL_HSI
#define STM8L_SYSCLK_DIVIDER            CLK_SYSCLK_DIV1
#define STM8L_RTCCLK_SOURCE             CLK_RTCSEL_LSE
#define STM8L_RTCCLK_DIVIDER            CLK_RTCCLK_DIV1

/*
 * SERIAL driver system settings.
 */
#define STM8L_SERIAL_USE_USART1         TRUE
#define STM8L_SERIAL_USE_USART2         FALSE
#define STM8L_SERIAL_USE_USART3         FALSE

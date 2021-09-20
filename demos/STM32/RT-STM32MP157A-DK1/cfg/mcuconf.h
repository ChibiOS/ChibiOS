/*
    ChibiOS - Copyright (C) 2006..2020 Giovanni Di Sirio

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
 * STM32L4xx drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the whole
 * driver is enabled in halconf.h.
 *
 * IRQ priorities:
 * 15...0       Lowest...Highest.
 *
 * DMA priorities:
 * 0...3        Lowest...Highest.
 */

#ifndef MCUCONF_H
#define MCUCONF_H

#define STM32MP1xx_MCUCONF
#define STM32MP151A_MCUCONF
#define STM32MP151C_MCUCONF
#define STM32MP151D_MCUCONF
#define STM32MP151F_MCUCONF
#define STM32MP153A_MCUCONF
#define STM32MP153C_MCUCONF
#define STM32MP153D_MCUCONF
#define STM32MP153F_MCUCONF
#define STM32MP157A_MCUCONF
#define STM32MP157C_MCUCONF
#define STM32MP157D_MCUCONF
#define STM32MP157F_MCUCONF

/*
 * HAL driver system settings.
 */
#define STM32_NO_INIT                       FALSE

/*
 * Peripherals clock sources.
 */

/*
 * IRQ system settings.
 */
#define STM32_IRQ_TIM1_UP_PRIORITY          7
#define STM32_IRQ_TIM1_CC_PRIORITY          7
#define STM32_IRQ_TIM2_PRIORITY             7
#define STM32_IRQ_TIM3_PRIORITY             7
#define STM32_IRQ_TIM4_PRIORITY             7
#define STM32_IRQ_TIM5_PRIORITY             7
#define STM32_IRQ_TIM6_PRIORITY             7
#define STM32_IRQ_TIM7_PRIORITY             7
#define STM32_IRQ_TIM8_UP_PRIORITY          7
#define STM32_IRQ_TIM8_CC_PRIORITY          7
#define STM32_IRQ_TIM12_PRIORITY            7
#define STM32_IRQ_TIM13_PRIORITY            7
#define STM32_IRQ_TIM14_PRIORITY            7
#define STM32_IRQ_TIM15_PRIORITY            7
#define STM32_IRQ_TIM16_PRIORITY            7
#define STM32_IRQ_TIM17_PRIORITY            7

/*
 * ADC driver system settings.
 */

/*
 * DAC driver system settings.
 */

/*
 * GPT driver system settings.
 */

/*
 * I2C driver system settings.
 */

/*
 * ICU driver system settings.
 */

/*
 * PWM driver system settings.
 */

/*
 * RTC driver system settings.
 */

/*
 * SDC driver system settings.
 */

/*
 * SERIAL driver system settings.
 */

/*
 * SIO driver system settings.
 */

/*
 * SPI driver system settings.
 */

/*
 * ST driver system settings.
 */
#define STM32_ST_IRQ_PRIORITY               8
#define STM32_ST_USE_TIMER                  2

/*
 * UART driver system settings.
 */

/*
 * WDG driver system settings.
 */
#define STM32_WDG_USE_IWDG                  FALSE

#endif /* MCUCONF_H */

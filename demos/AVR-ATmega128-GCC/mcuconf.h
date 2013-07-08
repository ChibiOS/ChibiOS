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
 * AVR drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the driver
 * is enabled in halconf.h.
 */

/*
 * ADC driver system settings.
 */
#define AVR_ADC_USE_ADC1                   FALSE

/*
 * CAN driver system settings.
 */

/*
 * MAC driver system settings.
 */

/*
 * PWM driver system settings.
 */
#define AVR_PWM_USE_PWM1                   FALSE
#define AVR_PWM_USE_PWM2                   FALSE
#define AVR_PWM_USE_PWM3                   FALSE
#define AVR_PWM_USE_PWM4                   FALSE
#define AVR_PWM_USE_PWM5                   FALSE

/*
 * ICU driver system settings.
 */
#define AVR_ICU_USE_ICU1                   FALSE
#define AVR_ICU_USE_ICU3                   FALSE
#define AVR_ICU_USE_ICU4                   FALSE
#define AVR_ICU_USE_ICU5                   FALSE

/*
 * GPT driver system settings.
 */
#define AVR_GPT_USE_GPT1                   FALSE
#define AVR_GPT_USE_GPT2                   FALSE
#define AVR_GPT_USE_GPT3                   FALSE
#define AVR_GPT_USE_GPT4                   FALSE
#define AVR_GPT_USE_GPT5                   FALSE

/*
 * SERIAL driver system settings.
 */
#define AVR_SERIAL_USE_USART0              FALSE
#define AVR_SERIAL_USE_USART1              TRUE

/*
 * I2C driver system settings.
 */
#define AVR_I2C_USE_I2C1                   FALSE

/*
 * SPI driver system settings.
 */
#define AVR_SPI_USE_SPI1                   FALSE
#define AVR_SPI_USE_16BIT_POLLED_EXCHANGE  FALSE

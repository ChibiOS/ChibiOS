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
 * RX62N drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the driver
 * is enabled in halconf.h.
 */

/*
 * HAL driver system settings.
 */
#define RX62N_ICLK_MUL                     8
#define RX62N_PCLK_MUL                     4
#define RX62N_BCLK_MUL                     1
#define RX62N_SDCLK_OUTPUT_ENABLED         FALSE
#define RX62N_BCLK_OUTPUT_ENABLED          FALSE

/*
 * ADC driver system settings.
 */

/*
 * CAN driver system settings.
 */

/*
 * MAC driver system settings.
 */
#define RX62N_MAC_ETH1_IRQ_PRIORITY        5

/*
 * PWM driver system settings.
 */

/*
 * SERIAL driver system settings.
 */
#define RX62N_SERIAL_USE_UART0             TRUE
#define RX62N_SERIAL_UART0_IRQ_PRIORITY    3
#define RX62N_SERIAL_USE_UART1             FALSE

/*
 * SPI driver system settings.
 */

 /*
 * USB driver system settings.
 */
#define RX62N_USE_USB0                     FALSE
#define RX62N_USB0_IRQ_PRIORITY            7

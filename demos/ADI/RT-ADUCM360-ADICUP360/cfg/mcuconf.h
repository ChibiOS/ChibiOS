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

#ifndef MCUCONF_H
#define MCUCONF_H

/*
 * ADUCM36x drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the whole
 * driver is enabled in halconf.h.
 *
 * IRQ priorities:
 * 3...0       Lowest...Highest.
 *
 * DMA priorities:
 * 0...3        Lowest...Highest.
 */

#define ADUCM36x_MCUCONF

/*
 * HAL driver system settings.
 */
#define ADUCM_NO_INIT                       FALSE
#define ADUCM_XOSC_ENABLED                  FALSE
#define ADUCM_CLKMUX                        ADUCM_CLKMUX_HFOSC
#define ADUCM_HFOSC_PREDIV                  ADUCM_HFOSC_DIV1
#define ADUCM_XOSC_PREDIV                   ADUCM_XOSC_DIV1
#define ADUCM_CD_DIV                        ADUCM_CD_DIV1
#define ADUCM_SPI0CD_DIV                    ADUCM_SPI0CD_DIV1
#define ADUCM_SPI1CD_DIV                    ADUCM_SPI1CD_DIV1
#define ADUCM_I2CCD_DIV                     ADUCM_I2CCD_DIV1
#define ADUCM_UARTCD_DIV                    ADUCM_UARTCD_DIV1
#define ADUCM_PWMCD_DIV                     ADUCM_PWMCD_DIV1

/*
 * IRQ system settings.
 */
#define ADUCM_IRQ_UART1_PRIORITY            3

/*
 * SERIAL driver system settings.
 */
#define ADUCM_SERIAL_USE_UART0              TRUE
#define ADUCM_SERIAL_UART0_PRIORITY         12

/** @} */
#endif /* MCUCONF_H */

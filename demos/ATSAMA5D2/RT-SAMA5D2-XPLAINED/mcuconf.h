/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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

#define SAMA5D2x_MCUCONF

/*
 * HAL driver system settings.
 */
#define SAMA_NO_INIT                        FALSE
#define SAMA_MOSCRC_ENABLED                 TRUE
#define SAMA_MOSCXT_ENABLED                 FALSE
#define SAMA_MOSC_SEL                       SAMA_MOSC_MOSCRC
#define SAMA_OSC_SEL                        SAMA_OSC_OSCXT
#define SAMA_MCK_SEL                        SAMA_MCK_PLLA_CLK
#define SAMA_MCK_PRES_VALUE                 1
#define SAMA_MCK_MDIV_VALUE                 3
#define SAMA_PLLA_MUL_VALUE                 83
#define SAMA_PLLADIV2_EN                    TRUE
#define SAMA_H64MX_H32MX_RATIO              2

/*
 * SPI driver system settings.
 */
#define SAMA_SPI_USE_SPI0                   FALSE
#define SAMA_SPI_USE_SPI1                   FALSE
#define SAMA_SPI_SPI0_IRQ_PRIORITY          10
#define SAMA_SPI_SPI1_IRQ_PRIORITY          10
#define SAMA_SPI_DMA_ERROR_HOOK(spip)       osalSysHalt("DMA failure")

#endif /* MCUCONF_H */

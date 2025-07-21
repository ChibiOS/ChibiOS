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
 * @file    portab.c
 * @brief   Application portability module code.
 *
 * @addtogroup application_portability
 * @{
 */

#include "hal.h"

#include "portab.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

void spi_circular_cb(SPIDriver *spip);
void spi_error_cb(SPIDriver *spip);

/*
 * Circular SPI configuration (21.25MHz, CPHA=0, CPOL=0, MSb first).
 */
const SPIConfig c_spicfg = {
  .circular         = true,
  .slave            = false,
  .data_cb          = spi_circular_cb,
  .error_cb         = spi_error_cb,
  .ssport           = GPIOB,
  .sspad            = 12U,
  .cr1              = SPI_CR1_BR_0,
  .cr2              = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

/*
 * Maximum speed SPI configuration (21.25MHz, CPHA=0, CPOL=0, MSb first).
 */
const SPIConfig hs_spicfg = {
  .circular         = false,
  .slave            = false,
  .data_cb          = NULL,
  .error_cb         = spi_error_cb,
  .ssport           = GPIOB,
  .sspad            = 12U,
  .cr1              = SPI_CR1_BR_0,
  .cr2              = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

/*
 * Low speed SPI configuration (664,062kHz, CPHA=0, CPOL=0, MSb first).
 */
const SPIConfig ls_spicfg = {
  .circular         = false,
  .slave            = false,
  .data_cb          = NULL,
  .error_cb         = spi_error_cb,
  .ssport           = GPIOB,
  .sspad            = 12U,
  .cr1              = SPI_CR1_BR_2 | SPI_CR1_BR_1,
  .cr2              = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

/*
 * Slave SPI configuration (CPHA=0, CPOL=0, MSb first).
 */
const SPIConfig sl_spicfg = {
  .circular         = false,
  .slave            = true,
  .data_cb          = NULL,
  .error_cb         = spi_error_cb,
  .cr1              = 0U,
  .cr2              = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void portab_setup(void) {

  /*
   * SPI2 I/O pins setup.
   */
  palSetPadMode(GPIOB, 13, PAL_MODE_ALTERNATE(5) |
                           PAL_STM32_OSPEED_HIGHEST);       /* SPI2 SCK.    */
  palSetPadMode(GPIOB, 14, PAL_MODE_ALTERNATE(5) |
                           PAL_STM32_OSPEED_HIGHEST);       /* SPI2 MISO.   */
  palSetPadMode(GPIOB, 15, PAL_MODE_ALTERNATE(5) |
                           PAL_STM32_OSPEED_HIGHEST);       /* SPI2 MOSI.   */
  palSetPadMode(GPIOB, 12, PAL_MODE_OUTPUT_PUSHPULL |
                           PAL_STM32_OSPEED_HIGHEST);       /* SPI2 CS.     */
  palSetPad(GPIOB, 12);

  /*
   * SPI3 I/O pins setup.
   */
  palSetPadMode(GPIOB, 3,  PAL_MODE_ALTERNATE(6) |
                           PAL_STM32_OSPEED_HIGHEST);       /* SPI3 SCK.    */
  palSetPadMode(GPIOB, 4,  PAL_MODE_ALTERNATE(6) |
                           PAL_STM32_OSPEED_HIGHEST);       /* SPI3 MISO.   */
  palSetPadMode(GPIOB, 5,  PAL_MODE_ALTERNATE(6) |
                           PAL_STM32_OSPEED_HIGHEST);       /* SPI3 MOSI.   */
  palSetPadMode(GPIOA, 4,  PAL_MODE_ALTERNATE(6) |
                           PAL_STM32_OSPEED_HIGHEST);       /* SPI3 NSSS.   */
}

/** @} */

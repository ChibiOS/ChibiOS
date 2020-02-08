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

/*
 * Circular SPI configuration (25MHz, CPHA=0, CPOL=0, MSb first).
 */
const SPIConfig c_spicfg = {
  true,
  spi_circular_cb,
  GPIOD,
  14,
  SPI_CFG1_MBR_DIV8 | SPI_CFG1_DSIZE_VALUE(7),
  0
};

/*
 * Maximum speed SPI configuration (25MHz, CPHA=0, CPOL=0, MSb first).
 */
const SPIConfig hs_spicfg = {
  false,
  NULL,
  GPIOD,
  14,
  SPI_CFG1_MBR_DIV8 | SPI_CFG1_DSIZE_VALUE(7),
  0
};

/*
 * Low speed SPI configuration (1.5625MHz, CPHA=0, CPOL=0, MSb first).
 */
const SPIConfig ls_spicfg = {
  false,
  NULL,
  GPIOD,
  14,
  SPI_CFG1_MBR_DIV128 | SPI_CFG1_DSIZE_VALUE(7),
  0
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
   * SPI1 I/O pins setup.
   */
  palSetPadMode(GPIOA, 5, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);
  palSetPadMode(GPIOA, 6, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);
  palSetPadMode(GPIOB, 5, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);
  palSetPadMode(GPIOD, 14, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
  palSetPad(GPIOD, 14);
}

/** @} */

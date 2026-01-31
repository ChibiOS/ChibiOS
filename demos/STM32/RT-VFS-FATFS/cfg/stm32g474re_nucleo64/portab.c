/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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

/* Making sure mcuconf.h setup is as expected.*/
#if STM32_PCLK1 != 85000000
#error "unexpected PCLK1 frequency"
#endif

/*
 * High speed SPI configuration (PCLK1/4=21.25MHz, CPHA=0, CPOL=0, MSb first).
 */
const SPIConfig hs_spicfg = {
  .circular         = false,
  .slave            = false,
  .data_cb          = NULL,
  .error_cb         = NULL,
  .ssport           = GPIOB,
  .sspad            = 12U,
  .cr1              = SPI_CR1_BR_0,
  .cr2              = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

/*
 * Low speed SPI configuration (PCLK1/256=332.03125kHz, CPHA=0, CPOL=0, MSb first).
 */
const SPIConfig ls_spicfg = {
  .circular         = false,
  .slave            = false,
  .data_cb          = NULL,
  .error_cb         = NULL,
  .ssport           = GPIOB,
  .sspad            = 12U,
  .cr1              = SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0,
  .cr2              = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

void portab_setup(void) {

  /*
   * SPI2 I/O pins setup.
   */
  palSetPad(GPIOB, 12);
  palSetPadMode(GPIOB, 11, PAL_MODE_INPUT_PULLUP |
                           PAL_STM32_OSPEED_HIGHEST);       /* Card Detect. */
  palSetPadMode(GPIOB, 12, PAL_MODE_OUTPUT_PUSHPULL |
                           PAL_STM32_OSPEED_HIGHEST);       /* SPI2 CS.     */
  palSetPadMode(GPIOB, 13, PAL_MODE_ALTERNATE(5) |
                           PAL_STM32_OSPEED_HIGHEST);       /* SPI2 SCK.    */
  palSetPadMode(GPIOB, 14, PAL_MODE_ALTERNATE(5) |
                           PAL_STM32_OSPEED_HIGHEST);       /* SPI2 MISO.   */
  palSetPadMode(GPIOB, 15, PAL_MODE_ALTERNATE(5) |
                           PAL_STM32_OSPEED_HIGHEST);       /* SPI2 MOSI.   */
}

/** @} */

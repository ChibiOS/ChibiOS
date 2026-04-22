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

#include "hal.h"

#include "portab.h"

const SPIConfig hs_spicfg = {
  .mode         = SPI_MODE_FSIZE_8,
  .ssline       = PAL_LINE(GPIOB, 12U),
  .cr1          = SPI_CR1_BR_0,
  .cr2          = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

const SPIConfig ls_spicfg = {
  .mode         = SPI_MODE_FSIZE_8,
  .ssline       = PAL_LINE(GPIOB, 12U),
  .cr1          = SPI_CR1_BR_2 | SPI_CR1_BR_1,
  .cr2          = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

void portab_setup(void) {

  palSetPadMode(GPIOB, 13U, PAL_MODE_ALTERNATE(5) |
                            PAL_STM32_OSPEED_HIGHEST);
  palSetPadMode(GPIOB, 14U, PAL_MODE_ALTERNATE(5) |
                            PAL_STM32_OSPEED_HIGHEST);
  palSetPadMode(GPIOB, 15U, PAL_MODE_ALTERNATE(5) |
                            PAL_STM32_OSPEED_HIGHEST);
  palSetPadMode(GPIOB, 12U, PAL_MODE_OUTPUT_PUSHPULL |
                            PAL_STM32_OSPEED_HIGHEST);
  palSetPad(GPIOB, 12U);
}

bool portab_mmc_is_inserted(void *arg) {

  (void)arg;

  return true;
}

bool portab_mmc_is_write_protected(void *arg) {

  (void)arg;

  return false;
}

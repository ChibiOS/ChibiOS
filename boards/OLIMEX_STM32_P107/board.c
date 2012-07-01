/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "ch.h"
#include "hal.h"

/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
#if HAL_USE_PAL || defined(__DOXYGEN__)
const PALConfig pal_default_config =
{
  {VAL_GPIOAODR, VAL_GPIOACRL, VAL_GPIOACRH},
  {VAL_GPIOBODR, VAL_GPIOBCRL, VAL_GPIOBCRH},
  {VAL_GPIOCODR, VAL_GPIOCCRL, VAL_GPIOCCRH},
  {VAL_GPIODODR, VAL_GPIODCRL, VAL_GPIODCRH},
  {VAL_GPIOEODR, VAL_GPIOECRL, VAL_GPIOECRH},
};
#endif

/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
void __early_init(void) {

  stm32_clock_init();
}

#if HAL_USE_MMC_SPI
/*
 * Card detection through the card internal pull-up on D3.
 */
bool_t mmc_lld_is_card_inserted(MMCDriver *mmcp) {
  static bool_t last_status = FALSE;

  (void)mmcp;
  if ((palReadLatch(GPIOA) & PAL_PORT_BIT(GPIOA_SPI3_CS_MMC)) == 0)
    return last_status;
  return last_status = (bool_t)palReadPad(GPIOA, GPIOA_SPI3_CS_MMC);
}

/*
 * Card write protection detection is not possible, the card is always
 * reported as not protected.
 */
bool_t mmc_lld_is_write_protected(MMCDriver *mmcp) {

  (void)mmcp;
  return FALSE;
}
#endif

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

  /*
   * Several I/O pins are re-mapped:
   *   USART3 to the PD8/PD9 pins.
   *   I2C1 to the PB8/PB9 pins.
   *   SPI3 to the PC10/PC11/PC12 pins.
   */
  AFIO->MAPR |= AFIO_MAPR_USART3_REMAP_FULLREMAP |
                AFIO_MAPR_I2C1_REMAP |
                AFIO_MAPR_SPI3_REMAP;
}

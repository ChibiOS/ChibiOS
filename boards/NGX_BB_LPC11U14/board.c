/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
*/

#include "ch.h"
#include "hal.h"

/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
#if HAL_USE_PAL || defined(__DOXYGEN__)
const PALConfig pal_default_config = {
  {VAL_GPIO0DATA, VAL_GPIO0DIR},
  {VAL_GPIO1DATA, VAL_GPIO1DIR}
};
#endif

/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
__inline void __early_init(void) {

  lpc_clock_init();
}

/*
 * Board-specific initialization code.
 */
__inline void boardInit(void) {

  /* LCD */
  LPC_IOCON->TMS_PIO0_12  = 0x91;       /* LCD_EN: GPIO - pull-up     */
  LPC_IOCON->TDO_PIO0_13  = 0x81;       /* LCD_RW: GPIO - No pull-up  */
  LPC_IOCON->TRST_PIO0_14 = 0x81;       /* LCD_RS: GPIO - No pull-up  */

  /* USART */
  LPC_IOCON->PIO0_18 = 0x81;            /* RDX: RXD - No pull-up      */
  LPC_IOCON->PIO0_19 = 0x81;            /* TDX: TXD - No pull-up      */

  /* Test LEDs */
  LPC_IOCON->PIO0_22 = 0x80;            /* LED_TEST1: GPIO - No pull-up */
  LPC_IOCON->PIO0_23 = 0x80;            /* LED_TEST2: GPIO - No pull-up */

}


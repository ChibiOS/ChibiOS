/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "ch.h"
#include "hal.h"

/*
 * Early initialization code.
 * This initialization is performed just after reset before BSS and DATA
 * segments initialization.
 */
void hwinit0(void) {

  LPC13xx_clock_init();
}

/*
 * Late initialization code.
 * This initialization is performed after BSS and DATA segments initialization
 * and before invoking the main() function.
 */
void hwinit1(void) {

  /*
   * HAL initialization.
   */
  halInit();

  /*
   * Extra, board-specific, initializations.
   * NOTE: PIO1_2 is associated also to the JTAG, if you need to use JTAG
   *       you must comment that line first.
   */
  LPC_IOCON->PIO0_7 = 0xC0;             /* Disables pull-up on LED2 output. */
  LPC_IOCON->JTAG_nTRST_PIO1_2 = 0xC1;  /* Disables pull-up on LED3B output
                                           and makes it GPIO1_2.            */
  LPC_IOCON->PIO1_9 = 0xC0;             /* Disables pull-up on LED3R output.*/
  LPC_IOCON->PIO1_10 = 0xC0;            /* Disables pull-up on LED3G output.*/

  /*
   * ChibiOS/RT initialization.
   */
  chSysInit();
}

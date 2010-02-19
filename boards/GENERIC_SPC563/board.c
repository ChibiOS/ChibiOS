/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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

/*
 * Early initialization code.
 * This initialization is performed just after reset before BSS and DATA
 * segments initialization.
 */
void hwinit0(void) {

  spc563_clock_init();
}

/*
 * Late initialization code.
 * This initialization is performed after BSS and DATA segments initialization
 * and before invoking the main() function.
 */
void hwinit1(void) {

  /*
   * Various initialization (temporary code).
   */
  SIU.PCR[GPIO_LED1].R     = 0x0300;                /* OBE | IBE.           */
  SIU.PCR[GPIO_LED2].R     = 0x0300;                /* OBE | IBE.           */
  SIU.PCR[GPIO_LED3].R     = 0x0300;                /* OBE | IBE.           */
  SIU.PCR[GPIO_LED4].R     = 0x0300;                /* OBE | IBE.           */
  SIU.PCR[GPIO_BUTTON1].R  = 0x0100;                /* IBE.                 */
  SIU.PCR[GPIO_BUTTON2].R  = 0x0100;                /* IBE.                 */
  SIU.PCR[GPIO_BUTTON3].R  = 0x0100;                /* IBE.                 */
  SIU.PCR[GPIO_BUTTON4].R  = 0x0100;                /* IBE.                 */
  SIU.PCR[GPIO_SCI_A_TX].R = 0x0500;                /* Primary | IBE.       */

  /*
   * HAL initialization.
   */
  halInit();

  /*
   * ChibiOS/RT initialization.
   */
  chSysInit();
}

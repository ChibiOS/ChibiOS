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
*/

#include "ch.h"
#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)
/* Initial setup of all defined pads, the list is terminated by a {0, 0, 0}.*/
static const spc_siu_init_t spc_siu_init[] = {
  {PCR(PORT4,  P4_ESCI_A_TX), PAL_HIGH, PAL_MODE_OUTPUT_ALTERNATE(1)},
  {PCR(PORT4,  P4_ESCI_A_RX), PAL_HIGH, PAL_MODE_OUTPUT_ALTERNATE(1)},
  {PCR(PORT11, P11_BUTTON1),  PAL_LOW,  PAL_MODE_INPUT},
  {PCR(PORT11, P11_BUTTON2),  PAL_LOW,  PAL_MODE_INPUT},
  {PCR(PORT11, P11_BUTTON3),  PAL_LOW,  PAL_MODE_INPUT},
  {PCR(PORT11, P11_BUTTON4),  PAL_LOW,  PAL_MODE_INPUT},
  {PCR(PORT11, P11_LED1),     PAL_HIGH, PAL_MODE_OUTPUT_PUSHPULL},
  {PCR(PORT11, P11_LED2),     PAL_HIGH, PAL_MODE_OUTPUT_PUSHPULL},
  {PCR(PORT11, P11_LED3),     PAL_HIGH, PAL_MODE_OUTPUT_PUSHPULL},
  {PCR(PORT11, P11_LED4),     PAL_HIGH, PAL_MODE_OUTPUT_PUSHPULL},
  {0, 0, 0}
};

/**
 * @brief   PAL setup.
 */
const PALConfig pal_default_config = {
  spc_siu_init
};
#endif

/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
void __early_init(void) {

  spc_clock_init();
}

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

#if 0
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
  SIU.PCR[GPIO_SCI_A_RX].R = 0x0500;                /* Primary | IBE.       */
#endif
}

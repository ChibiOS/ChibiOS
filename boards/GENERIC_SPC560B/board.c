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
/* Initial setup of all defined pads, the list is terminated by a {0, 0}.*/
static const spc_siu_init_t spc_siu_init[] = {
  {PCR(PB, PB_LIN0_TDX), PAL_HIGH, PAL_MODE_OUTPUT_ALTERNATE(1)},
  {PCR(PB, PB_LIN0_RDX), PAL_HIGH, PAL_MODE_INPUT},
  {PCR(PE, PE_BUTTON1),  PAL_LOW,  PAL_MODE_INPUT},
  {PCR(PE, PE_BUTTON2),  PAL_LOW,  PAL_MODE_INPUT},
  {PCR(PE, PE_BUTTON3),  PAL_LOW,  PAL_MODE_INPUT},
  {PCR(PE, PE_BUTTON4),  PAL_LOW,  PAL_MODE_INPUT},
  {PCR(PE, PE_LED1),     PAL_HIGH, PAL_MODE_OUTPUT_PUSHPULL},
  {PCR(PE, PE_LED2),     PAL_HIGH, PAL_MODE_OUTPUT_PUSHPULL},
  {PCR(PE, PE_LED3),     PAL_HIGH, PAL_MODE_OUTPUT_PUSHPULL},
  {PCR(PE, PE_LED4),     PAL_HIGH, PAL_MODE_OUTPUT_PUSHPULL},
  {0, 0, 0}
};

/* Initialization array for the PSMI registers.*/
static const uint8_t spc_padsels_init[SPC5_SIU_NUM_PADSELS] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/**
 * @brief   PAL setup.
 */
const PALConfig pal_default_config = {
  PAL_MODE_UNCONNECTED,             /* Default mode for all undefined pads. */
  spc_siu_init,
  spc_padsels_init
};
#endif

/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
void __early_init(void) {

  spc_clock_init();

  /* SWT disabled.*/
  SWT.SR.R = 0xC520;
  SWT.SR.R = 0xD928;
  SWT.CR.R = 0xFF00000A;
}

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

}

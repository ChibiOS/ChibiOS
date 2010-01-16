/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

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

/**
 * @file MSP430/hal_lld.c
 * @brief MSP430 HAL subsystem low level driver source.
 * @addtogroup MSP430_HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 */
const MSP430DIOConfig pal_default_config =
{
#if defined(__MSP430_HAS_PORT1__) || defined(__MSP430_HAS_PORT1_R__)
  {VAL_P1OUT, VAL_P1DIR},
#endif
#if defined(__MSP430_HAS_PORT2__) || defined(__MSP430_HAS_PORT2_R__)
  {VAL_P2OUT, VAL_P2DIR},
#endif
#if defined(__MSP430_HAS_PORT3__) || defined(__MSP430_HAS_PORT3_R__)
  {VAL_P3OUT, VAL_P3DIR},
#endif
#if defined(__MSP430_HAS_PORT4__) || defined(__MSP430_HAS_PORT4_R__)
  {VAL_P4OUT, VAL_P4DIR},
#endif
#if defined(__MSP430_HAS_PORT5__) || defined(__MSP430_HAS_PORT5_R__)
  {VAL_P5OUT, VAL_P5DIR},
#endif
#if defined(__MSP430_HAS_PORT6__) || defined(__MSP430_HAS_PORT6_R__)
  {VAL_P6OUT, VAL_P6DIR},
#endif
};

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Low level HAL driver initialization.
 */
void hal_lld_init(void) {

  /*
   * Clock sources setup.
   */
  DCOCTL  = VAL_DCOCTL;
  BCSCTL1 = VAL_BCSCTL1;
#if MSP430_USE_CLOCK == MSP430_CLOCK_SOURCE_XT2CLK
  do {
    int i;
    IFG1 &= ~OFIFG;
     for (i = 255; i > 0; i--)
       asm("nop");
  } while (IFG1 & OFIFG);
#endif
  BCSCTL2 = VAL_BCSCTL2;
}

/** @} */

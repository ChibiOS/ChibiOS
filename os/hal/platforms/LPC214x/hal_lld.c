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
 * @file LPC214x/hal_lld.c
 * @brief LPC214x HAL subsystem low level driver source.
 * @addtogroup LPC214x_HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 */
const LPC214xFIOConfig pal_default_config =
{
  VAL_PINSEL0,
  VAL_PINSEL1,
  VAL_PINSEL2,
  {VAL_FIO0PIN, VAL_FIO0DIR},
  {VAL_FIO1PIN, VAL_FIO1DIR}
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*
 * Non-vectored IRQs handler, the default action can be overridden by
 * redefining the @p LPC214x_NON_VECTORED_IRQ_HOOK() hook macro.
 */
static CH_IRQ_HANDLER(irq_handler) {

  CH_IRQ_PROLOGUE();

  LPC214x_NON_VECTORED_IRQ_HOOK();

  VICVectAddr = 0;
  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Low level HAL driver initialization.
 */
void hal_lld_init(void) {

  vic_init();
  VICDefVectAddr = (IOREG32)irq_handler;

}

/**
 * @brief LPC214x clocks and PLL initialization.
 * @note All the involved constants come from the file @p board.h.
 */
void lpc214x_clock_init(void) {

  /*
   * All peripherals clock disabled by default in order to save power.
   */
  PCONP = PCRTC | PCTIM0;

  /*
   * MAM setup.
   */
  MAMTIM = 0x3;                 /* 3 cycles for flash accesses. */
  MAMCR  = 0x2;                 /* MAM fully enabled. */

  /*
   * PLL setup for Fosc=12MHz and CCLK=48MHz.
   * P=2 M=3.
   */
  PLL *pll = PLL0Base;
  pll->PLL_CFG  = 0x23;         /* P and M values. */
  pll->PLL_CON  = 0x1;          /* Enables the PLL 0. */
  pll->PLL_FEED = 0xAA;
  pll->PLL_FEED = 0x55;
  while (!(pll->PLL_STAT & 0x400))
    ;                           /* Wait for PLL lock. */

  pll->PLL_CON  = 0x3;          /* Connects the PLL. */
  pll->PLL_FEED = 0xAA;
  pll->PLL_FEED = 0x55;

  /*
   * VPB setup.
   * PCLK = CCLK / 4.
   */
  VPBDIV = VPD_D4;
}

/** @} */

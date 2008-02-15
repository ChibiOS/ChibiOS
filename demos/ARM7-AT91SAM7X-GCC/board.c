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

#include <ch.h>

#include "at91lib/AT91SAM7X256.h"

extern void FiqHandler(void);

__attribute__((interrupt("IRQ")))
static void SpuriousHandler(void) {

  AT91C_BASE_AIC->AIC_EOICR = (AT91_REG)AT91C_BASE_AIC;
}

void hwinit(void) {
  int i;
  AT91PS_PMC pmcp = AT91C_BASE_PMC;
  AT91PS_AIC aicp = AT91C_BASE_AIC;

  /*
   * Flash Memory: 1 wait state, about 50 cycles in a microsecond.
   */
  AT91C_BASE_MC->MC_FMR = (AT91C_MC_FMCN & (50 << 16)) | AT91C_MC_FWS_1FWS;

  /*
   * Watchdog disabled.
   */
  AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;

  /*
   * Enables the main oscillator and waits 56 slow cycles as startup time.
   */
  pmcp->PMC_MOR = (AT91C_CKGR_OSCOUNT & (7 << 8)) | AT91C_CKGR_MOSCEN;
  while (!(pmcp->PMC_SR & AT91C_PMC_MOSCS))
    ;

  /*
   * PLL setup: DIV = 14, MUL = 72, PLLCOUNT = 10
   * PLLfreq = 96109714 Hz (rounded)
   */
  pmcp->PMC_PLLR = (AT91C_CKGR_DIV & 14) |
                   (AT91C_CKGR_PLLCOUNT & (10 << 8)) |
                   (AT91C_CKGR_MUL & (72 << 16));
  while (!(pmcp->PMC_SR & (AT91C_PMC_LOCK)))
    ;

  /*
   * Master clock = PLLfreq / 2 = 48054858 Hz (rounded)
   */
  pmcp->PMC_MCKR = AT91C_PMC_CSS_PLL_CLK | AT91C_PMC_PRES_CLK_2;
  while (!(pmcp->PMC_SR & (AT91C_PMC_MCKRDY)))
    ;

  /*
   * Default AIC setup, the device drivers will modify it as needed.
   */
  aicp->AIC_SVR[0] = (AT91_REG)FiqHandler;
  for (i = 1; i < 31; i++) {
    aicp->AIC_SVR[i] = (AT91_REG)NULL;
    aicp->AIC_EOICR = (AT91_REG)i;
  }
  aicp->AIC_SPU  = (AT91_REG)SpuriousHandler;

  /*
   * I/O setup, enable clocks, initially all pins are inputs with pullups.
   */
  pmcp->PMC_PCER = (1 << AT91C_ID_PIOA) | (1 << AT91C_ID_PIOB);
  AT91C_BASE_PIOA->PIO_PER = 0xFFFFFFFF;
}

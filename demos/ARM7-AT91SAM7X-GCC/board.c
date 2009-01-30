/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

#include <ch.h>

#include "board.h"
#include "at91lib/aic.h"

#include <sam7x_serial.h>

extern void FiqHandler(void);

__attribute__((naked))
static void SpuriousHandler(void) {

  chSysIRQEnterI();

  AT91C_BASE_AIC->AIC_EOICR = 0;

  chSysIRQExitI();
}

/*
 * SYS IRQ handling here.
 */
__attribute__((naked))
static void SYSIrqHandler(void) {

  chSysIRQEnterI();

  if (AT91C_BASE_PITC->PITC_PISR & AT91C_PITC_PITS) {
    (void) AT91C_BASE_PITC->PITC_PIVR;
    chSysTimerHandlerI();
  }
  AT91C_BASE_AIC->AIC_EOICR = 0;

  chSysIRQExitI();
}

/*
 * Early initialization code.
 * This initialization is performed just after reset before BSS and DATA
 * segments initialization.
 */
void hwinit0(void) {
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
  AT91C_BASE_PMC->PMC_MOR = (AT91C_CKGR_OSCOUNT & (7 << 8)) | AT91C_CKGR_MOSCEN;
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS))
    ;

  /*
   * PLL setup: DIV = 14, MUL = 72, PLLCOUNT = 10
   * PLLfreq = 96109714 Hz (rounded)
   */
  AT91C_BASE_PMC->PMC_PLLR = (AT91C_CKGR_DIV & 14) |
                             (AT91C_CKGR_PLLCOUNT & (10 << 8)) |
                             (AT91C_CKGR_MUL & (72 << 16));
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK))
    ;

  /*
   * Master clock = PLLfreq / 2 = 48054858 Hz (rounded)
   */
  AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_PLL_CLK | AT91C_PMC_PRES_CLK_2;
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY))
    ;

  /*
   * I/O setup, enable clocks, initially all pins are inputs with pullups.
   */
  AT91C_BASE_PMC->PMC_PCER   = (1 << AT91C_ID_PIOA) | (1 << AT91C_ID_PIOB);
  AT91C_BASE_PIOA->PIO_PER   = 0xFFFFFFFF;
  AT91C_BASE_PIOB->PIO_PER   = 0xFFFFFFFF;
}

/*
 * Late initialization code.
 * This initialization is performed after BSS and DATA segments initialization
 * and before invoking the main() function.
 */
void hwinit1(void) {
  int i;

  /*
   * Default AIC setup, the device drivers will modify it as needed.
   */
  AT91C_BASE_AIC->AIC_ICCR = 0xFFFFFFFF;
  AT91C_BASE_AIC->AIC_SVR[0] = (AT91_REG)FiqHandler;
  for (i = 1; i < 31; i++) {
    AT91C_BASE_AIC->AIC_SVR[i] = (AT91_REG)NULL;
    AT91C_BASE_AIC->AIC_EOICR = (AT91_REG)i;
  }
  AT91C_BASE_AIC->AIC_SPU  = (AT91_REG)SpuriousHandler;

  /*
   * LCD pins setup.
   */
  AT91C_BASE_PIOB->PIO_CODR   = PIOB_LCD_BL;    // Set to low.
  AT91C_BASE_PIOB->PIO_OER    = PIOB_LCD_BL;    // Configure as output.
  AT91C_BASE_PIOB->PIO_PPUDR  = PIOB_LCD_BL;    // Disable internal pullup resistor.

  AT91C_BASE_PIOA->PIO_SODR   = PIOA_LCD_RESET; // Set to high.
  AT91C_BASE_PIOA->PIO_OER    = PIOA_LCD_RESET; // Configure as output.
  AT91C_BASE_PIOA->PIO_PPUDR  = PIOA_LCD_RESET; // Disable internal pullup resistor.

  /*
   * Joystick and buttons, disable pullups, already inputs.
   */
  AT91C_BASE_PIOA->PIO_PPUDR = PIOA_B1 | PIOA_B2 | PIOA_B3 | PIOA_B4 | PIOA_B5;
  AT91C_BASE_PIOB->PIO_PPUDR = PIOB_SW1 | PIOB_SW2;

  /*
   * MMC/SD slot, disable pullups, already inputs.
   */
  AT91C_BASE_SYS->PIOB_PPUDR = PIOB_MMC_WP | PIOB_MMC_CP;

  /*
   * PIT Initialization.
   */
  AIC_ConfigureIT(AT91C_ID_SYS,
                  AT91C_AIC_SRCTYPE_HIGH_LEVEL | (AT91C_AIC_PRIOR_HIGHEST - 1),
                  SYSIrqHandler);
  AIC_EnableIT(AT91C_ID_SYS);
  AT91C_BASE_PITC->PITC_PIMR = (MCK / 16 / CH_FREQUENCY) - 1;
  AT91C_BASE_PITC->PITC_PIMR |= AT91C_PITC_PITEN | AT91C_PITC_PITIEN;

  /*
   * Serial driver initialization, RTS/CTS pins enabled for USART0 only.
   */
  InitSerial(AT91C_AIC_PRIOR_HIGHEST - 2, AT91C_AIC_PRIOR_HIGHEST - 2);
  AT91C_BASE_PIOA->PIO_PDR   = AT91C_PA3_RTS0 | AT91C_PA4_CTS0;
  AT91C_BASE_PIOA->PIO_ASR   = AT91C_PIO_PA3 | AT91C_PIO_PA4;
  AT91C_BASE_PIOA->PIO_PPUDR = AT91C_PIO_PA3 | AT91C_PIO_PA4;

  /*
   * ChibiOS/RT initialization.
   */
  chSysInit();
}

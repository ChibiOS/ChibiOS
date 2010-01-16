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

#include "ch.h"
#include "hal.h"

/*
 * SYS IRQ handling here.
 */
static CH_IRQ_HANDLER(SYSIrqHandler) {

  CH_IRQ_PROLOGUE();

  if (AT91C_BASE_PITC->PITC_PISR & AT91C_PITC_PITS) {
    (void) AT91C_BASE_PITC->PITC_PIVR;
    chSysLockFromIsr();
    chSysTimerHandlerI();
    chSysUnlockFromIsr();
  }
  AT91C_BASE_AIC->AIC_EOICR = 0;

  CH_IRQ_EPILOGUE();
}

/*
 * Early initialization code.
 * This initialization is performed just after reset before BSS and DATA
 * segments initialization.
 */
void hwinit0(void) {
  
  /* Watchdog disabled.*/
  AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;
  
  at91sam7_clock_init();
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
   * LCD pins setup.
   */
  palClearPad(IOPORT2, PIOB_LCD_BL);
  palSetPadMode(IOPORT2, PIOB_LCD_BL, PAL_MODE_OUTPUT_PUSHPULL);

  palSetPad(IOPORT1, PIOA_LCD_RESET);
  palSetPadMode(IOPORT1, PIOA_LCD_RESET, PAL_MODE_OUTPUT_PUSHPULL);

  /*
   * Joystick and buttons setup.
   */
  palSetGroupMode(IOPORT1,
                  PIOA_B1_MASK | PIOA_B2_MASK | PIOA_B3_MASK |
                  PIOA_B4_MASK | PIOA_B5_MASK,
                  PAL_MODE_INPUT);
  palSetGroupMode(IOPORT2, PIOB_SW1_MASK | PIOB_SW2_MASK, PAL_MODE_INPUT);

  /*
   * MMC/SD slot setup.
   */
  palSetGroupMode(IOPORT2,
                  PIOB_MMC_WP_MASK | PIOB_MMC_CP_MASK,
                  PAL_MODE_INPUT);

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
   * RTS/CTS pins enabled for USART0 only.
   */
  AT91C_BASE_PIOA->PIO_PDR   = AT91C_PA3_RTS0 | AT91C_PA4_CTS0;
  AT91C_BASE_PIOA->PIO_ASR   = AT91C_PIO_PA3 | AT91C_PIO_PA4;
  AT91C_BASE_PIOA->PIO_PPUDR = AT91C_PIO_PA3 | AT91C_PIO_PA4;

  /*
   * ChibiOS/RT initialization.
   */
  chSysInit();
}

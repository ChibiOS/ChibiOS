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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "ch.h"
#include "hal.h"

/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
#if HAL_USE_PAL || defined(__DOXYGEN__)
const PALConfig pal_default_config =
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
#endif

CH_IRQ_HANDLER(TIMERA0) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

#if USE_MSP430_USART0
  P3SEL |= (1 << 4) | (1 << 5);
#endif

#if USE_MSP430_USART1
  P3SEL |= (1 << 6) | (1 << 7);
#endif

  /*
   * Timer 0 setup, uses SMCLK as source.
   */
  TACCR0 = SMCLK / 4 / CH_FREQUENCY - 1;/* Counter limit.               */
  TACTL = TACLR;                        /* Clean start.                 */
  TACTL = TASSEL_2 | ID_2 | MC_1;       /* Src=SMCLK, ID=4, cmp=TACCR0. */
  TACCTL0 = CCIE;                       /* Interrupt on compare.        */
}

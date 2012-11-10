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

/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
#if HAL_USE_PAL || defined(__DOXYGEN__)
const PALConfig pal_default_config =
{
#if defined(__MSP430_HAS_PORTA_R__) || defined(__MSP430_HAS_PORT1_R__)
  {VAL_P1OUT, VAL_P1DIR},
#endif
#if defined(__MSP430_HAS_PORTA_R__) || defined(__MSP430_HAS_PORT2_R__)
  {VAL_P2OUT, VAL_P2DIR},
#endif
#if defined(__MSP430_HAS_PORTB_R__) || defined(__MSP430_HAS_PORT3_R__)
  {VAL_P3OUT, VAL_P3DIR},
#endif
#if defined(__MSP430_HAS_PORTB_R__) || defined(__MSP430_HAS_PORT4_R__)
  {VAL_P4OUT, VAL_P4DIR},
#endif
#if defined(__MSP430_HAS_PORTC_R__) || defined(__MSP430_HAS_PORT5_R__)
  {VAL_P5OUT, VAL_P5DIR},
#endif
#if defined(__MSP430_HAS_PORTC_R__) || defined(__MSP430_HAS_PORT6_R__)
  {VAL_P6OUT, VAL_P6DIR},
#endif
#if defined(__MSP430_HAS_PORTD_R__) || defined(__MSP430_HAS_PORT7_R__)
  {VAL_P7OUT, VAL_P7DIR},
#endif
#if defined(__MSP430_HAS_PORTD_R__) || defined(__MSP430_HAS_PORT8_R__)
  {VAL_P8OUT, VAL_P8DIR},
#endif
#if defined(__MSP430_HAS_PORTE_R__) || defined(__MSP430_HAS_PORT9_R__)
  {VAL_P9OUT, VAL_P9DIR},
#endif
#if defined(__MSP430_HAS_PORTE_R__) || defined(__MSP430_HAS_PORT10_R__)
  {VAL_P10OUT, VAL_P10DIR},
#endif
#if defined(__MSP430_HAS_PORTF_R__) || defined(__MSP430_HAS_PORT11_R__)
  {VAL_P11OUT, VAL_P11DIR},
#endif
#if defined(__MSP430_HAS_PORTJ_R__)
  {VAL_P12OUT, VAL_P12DIR},
#endif
};
#endif

CH_IRQ_HANDLER(TIMER0_A0) {

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
  /*
   * Timer 0 setup, uses SMCLK as source.
   */
  TA0CCTL0 = CCIE;                         /* Interrupt on compare enabled. */
  TA0CTL = TACLR;                          /* Clean start. */
  TA0CCR0 = DCOCLK / 4 / CH_FREQUENCY - 1; /* Counter upper limit. */
  TA0CTL = TASSEL_2 | ID_2 | MC_1;         /* SMCLK, clock divider 4, contionous mode. */
}

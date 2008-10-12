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
#include <signal.h>

#include "board.h"
#include "msp430_serial.h"

static void SetError(uint8_t urctl, FullDuplexDriver *com) {
  dflags_t sts = 0;

  if (urctl & OE)
    sts |= SD_OVERRUN_ERROR;
  if (urctl & PE)
    sts |= SD_PARITY_ERROR;
  if (urctl & FE)
    sts |= SD_FRAMING_ERROR;
  if (urctl & BRK)
    sts |= SD_BREAK_DETECTED;
  chFDDAddFlagsI(com, sts);
}

#ifdef USE_MSP430_USART0
FullDuplexDriver COM1;
static uint8_t ib1[SERIAL_BUFFERS_SIZE];
static uint8_t ob1[SERIAL_BUFFERS_SIZE];

interrupt(USART0TX_VECTOR) u0txirq(void) {
  msg_t b;

  chSysIRQEnterI();

  b = chFDDRequestDataI(&COM1);
  if (b < Q_OK)
    U0IE &= ~UTXIE0;
  else
    U0TXBUF = b;

  chSysIRQExitI();
}

interrupt(USART0RX_VECTOR) u0rxirq(void) {
  uint8_t urctl;

  chSysIRQEnterI();

  if ((urctl = U0RCTL) & RXERR)
    SetError(urctl, &COM1);
  chFDDIncomingDataI(&COM1, U0RXBUF);

  chSysIRQExitI();
}

/*
 * Invoked by the high driver when one or more bytes are inserted in the
 * output queue.
 */
static void OutNotify1(void) {

  U0IE |= UTXIE0;
}

/*
 * USART setup, must be invoked with interrupts disabled.
 * NOTE: Does not reset I/O queues.
 */
void SetUSART0I(uint16_t div, uint8_t mod, uint8_t ctl) {

  U0CTL = SWRST;                /* Resets the USART, it should already be */
  /* USART init */
  U0TCTL = SSEL0 | SSEL1;       /* SMCLK as clock source */
  U0MCTL = mod;                 /* Modulator */
  U0BR1 = (uint8_t)(div >> 8);  /* Divider high */
  U0BR0 = (uint8_t)(div >> 0);  /* Divider low */
  /* Clear USART status */
  (void)U0RXBUF;
  U0RCTL = 0;
  /* USART enable */
  P3SEL |= BV(4) + BV(5);       /* I/O pins for USART */
  U0ME |= UTXE0 + URXE0;        /* Enables the USART */
  U0CTL = ctl & ~SWRST;         /* Various settings, clears reset state */
  U0IE |= URXIE0;               /* Enables RX interrupt */
}
#endif /* USE_MSP430_USART0 */

void InitSerial(void) {

  /* I/O queues setup.*/
#ifdef USE_MSP430_USART0
  chFDDInit(&COM1, ib1, sizeof ib1, NULL, ob1, sizeof ob1, OutNotify1);
  SetUSART0I(UBR(38400), 0, CHAR);
#endif

#ifdef USE_MSP430_USART1
#endif
}

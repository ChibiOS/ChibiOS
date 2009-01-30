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
#include "sam7x_serial.h"
#include "at91lib/aic.h"

FullDuplexDriver COM1;
static uint8_t ib1[SERIAL_BUFFERS_SIZE];
static uint8_t ob1[SERIAL_BUFFERS_SIZE];

FullDuplexDriver COM2;
static uint8_t ib2[SERIAL_BUFFERS_SIZE];
static uint8_t ob2[SERIAL_BUFFERS_SIZE];

static void SetError(AT91_REG csr, FullDuplexDriver *com) {
  dflags_t sts = 0;

  if (csr & AT91C_US_OVRE)
    sts |= SD_OVERRUN_ERROR;
  if (csr & AT91C_US_PARE)
    sts |= SD_PARITY_ERROR;
  if (csr & AT91C_US_FRAME)
    sts |= SD_FRAMING_ERROR;
  if (csr & AT91C_US_RXBRK)
    sts |= SD_BREAK_DETECTED;
  chFDDAddFlagsI(com, sts);
}

/*
 * Serves the pending sources on the USART.
 */
static void ServeInterrupt(AT91PS_USART u, FullDuplexDriver *com) {

  if (u->US_CSR & AT91C_US_RXRDY)
    chFDDIncomingDataI(com, u->US_RHR);
  if (u->US_CSR & AT91C_US_TXRDY) {
    msg_t b = chFDDRequestDataI(com);
    if (b < Q_OK)
      u->US_IDR = AT91C_US_TXRDY;
    else
      u->US_THR = b;
  }
  if (u->US_CSR & (AT91C_US_OVRE | AT91C_US_FRAME | AT91C_US_PARE | AT91C_US_RXBRK)) {
    SetError(u->US_CSR, com);
    u->US_CR = AT91C_US_RSTSTA;
  }
  AT91C_BASE_AIC->AIC_EOICR = 0;
}

__attribute__((naked, weak))
void USART0IrqHandler(void) {

  chSysIRQEnterI();

  ServeInterrupt(AT91C_BASE_US0, &COM1);

  chSysIRQExitI();
}

__attribute__((naked, weak))
void USART1IrqHandler(void) {

  chSysIRQEnterI();

  ServeInterrupt(AT91C_BASE_US1, &COM2);

  chSysIRQExitI();
}

/*
 * Invoked by the high driver when one or more bytes are inserted in the
 * output queue.
 */
static void OutNotify1(void) {

  AT91C_BASE_US0->US_IER = AT91C_US_TXRDY;
}

/*
 * Invoked by the high driver when one or more bytes are inserted in the
 * output queue.
 */
static void OutNotify2(void) {

  AT91C_BASE_US1->US_IER = AT91C_US_TXRDY;
}

/*
 * USART setup, must be invoked with interrupts disabled.
 * NOTE: Does not reset I/O queues.
 */
void SetUSARTI(AT91PS_USART u, int speed, int mode) {

  /* Disables IRQ sources and stop operations.*/
  u->US_IDR = 0xFFFFFFFF;
  u->US_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RSTSTA;

  /* New parameters setup.*/
  if (mode & AT91C_US_OVER)
    u->US_BRGR = MCK / (speed * 8);
  else
    u->US_BRGR = MCK / (speed * 16);
  u->US_MR = mode;
  u->US_RTOR = 0;
  u->US_TTGR = 0;

  /* Enables operations and IRQ sources.*/
  u->US_CR = AT91C_US_RXEN | AT91C_US_TXEN | AT91C_US_DTREN | AT91C_US_RTSEN;
  u->US_IER = AT91C_US_RXRDY | AT91C_US_OVRE | AT91C_US_FRAME | AT91C_US_PARE |
              AT91C_US_RXBRK;
}

/*
 * Serial subsystem initialization.
 * NOTE: Handshake pins are not switched to their function because they may have
 *       another use. Enable them externally if needed.
 */
void InitSerial(int prio0, int prio1) {

  /* I/O queues setup.*/
  chFDDInit(&COM1, ib1, sizeof ib1, NULL, ob1, sizeof ob1, OutNotify1);
  chFDDInit(&COM2, ib2, sizeof ib2, NULL, ob2, sizeof ob2, OutNotify2);

  /* Switches the I/O pins to the peripheral function A, disables pullups.*/
  AT91C_BASE_PIOA->PIO_PDR   = AT91C_PA0_RXD0 | AT91C_PA1_TXD0 |
                               AT91C_PA5_RXD1 | AT91C_PA6_TXD1;
  AT91C_BASE_PIOA->PIO_ASR   = AT91C_PIO_PA0 | AT91C_PIO_PA1 |
                               AT91C_PIO_PA5 | AT91C_PIO_PA6;
  AT91C_BASE_PIOA->PIO_PPUDR = AT91C_PIO_PA0 | AT91C_PIO_PA1 |
                               AT91C_PIO_PA5 | AT91C_PIO_PA6;

  /* Starts the clock and clears possible sources of immediate interrupts.*/
  AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_US0) | (1 << AT91C_ID_US1);
  AT91C_BASE_US0->US_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RSTSTA;
  AT91C_BASE_US1->US_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RSTSTA;

  /* Interrupts setup.*/
  AIC_ConfigureIT(AT91C_ID_US0,
                  AT91C_AIC_SRCTYPE_HIGH_LEVEL | prio0,
                  USART0IrqHandler);
  AIC_EnableIT(AT91C_ID_US0);
  AIC_ConfigureIT(AT91C_ID_US1,
                  AT91C_AIC_SRCTYPE_HIGH_LEVEL | prio1,
                  USART1IrqHandler);
  AIC_EnableIT(AT91C_ID_US1);

  SetUSARTI(AT91C_BASE_US0, 38400, AT91C_US_USMODE_NORMAL |
                                   AT91C_US_CLKS_CLOCK |
                                   AT91C_US_CHRL_8_BITS |
                                   AT91C_US_PAR_NONE |
                                   AT91C_US_NBSTOP_1_BIT);
  SetUSARTI(AT91C_BASE_US1, 38400, AT91C_US_USMODE_NORMAL |
                                   AT91C_US_CLKS_CLOCK |
                                   AT91C_US_CHRL_8_BITS |
                                   AT91C_US_PAR_NONE |
                                   AT91C_US_NBSTOP_1_BIT);
}

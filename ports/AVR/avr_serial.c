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

#include "avr_serial.h"

static void SetError(uint8_t sra, FullDuplexDriver *com) {
  dflags_t sts = 0;

  if (sra & (1 << DOR))
    sts |= SD_OVERRUN_ERROR;
  if (sra & (1 << UPE))
    sts |= SD_PARITY_ERROR;
  if (sra & (1 << FE))
    sts |= SD_FRAMING_ERROR;
  chSysLockFromIsr();
  chFDDAddFlagsI(com, sts);
  chSysUnlockFromIsr();
}

#ifdef USE_AVR_USART0
FullDuplexDriver SER1;
static uint8_t ib1[SERIAL_BUFFERS_SIZE];
static uint8_t ob1[SERIAL_BUFFERS_SIZE];

CH_IRQ_HANDLER(USART0_RX_vect) {
  uint8_t sra;

  CH_IRQ_PROLOGUE();

  sra = UCSR0A;
  if (sra & ((1 << DOR) | (1 << UPE) | (1 << FE)))
    SetError(sra, &SER1);
  chSysLockFromIsr();
  chFDDIncomingDataI(&SER1, UDR0);
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(USART0_UDRE_vect) {
  msg_t b;

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  b = chFDDRequestDataI(&SER1);
  chSysUnlockFromIsr();
  if (b < Q_OK)
    UCSR0B &= ~(1 << UDRIE);
  else
    UDR0 = b;

  CH_IRQ_EPILOGUE();
}

/*
 * Invoked by the high driver when one or more bytes are inserted in the
 * output queue.
 */
static void OutNotify1(void) {

  UCSR0B |= (1 << UDRIE);
}

/*
 * USART setup, must be invoked with interrupts disabled.
 * NOTE: Does not reset I/O queues.
 */
void SetUSART0I(uint16_t brr, uint8_t csrc) {

  UBRR0L = brr;
  UBRR0H = brr >> 8;
  UCSR0A = 0;
  UCSR0B = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
  UCSR0C = csrc;
}
#endif /* USE_AVR_USART0 */

#ifdef USE_AVR_USART1
FullDuplexDriver SER2;
static uint8_t ib2[SERIAL_BUFFERS_SIZE];
static uint8_t ob2[SERIAL_BUFFERS_SIZE];

CH_IRQ_HANDLER(USART1_RX_vect) {
  uint8_t sra;

  CH_IRQ_PROLOGUE();

  sra = UCSR1A;
  if (sra & ((1 << DOR) | (1 << UPE) | (1 << FE)))
    SetError(sra, &SER2);
  chSysLockFromIsr();
  chFDDIncomingDataI(&SER2, UDR1);
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(USART1_UDRE_vect) {
  msg_t b;

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  b = chFDDRequestDataI(&SER2);
  chSysUnlockFromIsr();
  if (b < Q_OK)
    UCSR1B &= ~(1 << UDRIE);
  else
    UDR1 = b;

  CH_IRQ_EPILOGUE();
}

/*
 * Invoked by the high driver when one or more bytes are inserted in the
 * output queue.
 */
static void OutNotify2(void) {

  UCSR1B |= (1 << UDRIE);
}

/*
 * USART setup, must be invoked with interrupts disabled.
 * NOTE: Does not reset I/O queues.
 */
void SetUSART1(uint16_t brr, uint8_t csrc) {

  UBRR1L = brr;
  UBRR1H = brr >> 8;
  UCSR1A = 0;
  UCSR1B = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
  UCSR1C = csrc;
}
#endif /* USE_AVR_USART1 */

void InitSerial(void) {

#ifdef USE_AVR_USART0
  /* I/O queues setup.*/
  chFDDInit(&SER1, ib1, sizeof ib1, NULL, ob1, sizeof ob1, OutNotify1);
  SetUSART0(UBRR(38400), (1 << UCSZ1) | (1 << UCSZ0));
#endif

#ifdef USE_AVR_USART1
  chFDDInit(&SER2, ib2, sizeof ib2, NULL, ob2, sizeof ob2, OutNotify2);
  SetUSART1(UBRR(38400), (1 << UCSZ1) | (1 << UCSZ0));
#endif
}

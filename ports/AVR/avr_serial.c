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

/**
 * @file ports/AVR/avr_serial.c
 * @brief AVR Serial driver code.
 * @addtogroup AVR_SERIAL
 * @{
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

#if USE_AVR_USART0 || defined(__DOXYGEN__)
/** @brief USART0 serial driver identifier.*/
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

static void OutNotify1(void) {

  UCSR0B |= (1 << UDRIE);
}

/**
 * @brief USART0 setup.
 * @details This function must be invoked with interrupts disabled.
 * @param[in] brr the divider value as calculated by the @p UBRR() macro
 * @param[in] csrc the value for the @p UCSR0C register
 * @note Must be invoked with interrupts disabled.
 * @note Does not reset the I/O queues.
 */
void usart0_setup(uint16_t brr, uint8_t csrc) {

  UBRR0L = brr;
  UBRR0H = brr >> 8;
  UCSR0A = 0;
  UCSR0B = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
  UCSR0C = csrc;
}
#endif /* USE_AVR_USART0 */

#if USE_AVR_USART1 || defined(__DOXYGEN__)
/** @brief USART1 serial driver identifier.*/
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

static void OutNotify2(void) {

  UCSR1B |= (1 << UDRIE);
}

/**
 * @brief USART1 setup.
 * @details This function must be invoked with interrupts disabled.
 * @param[in] brr the divider value as calculated by the @p UBRR() macro
 * @param[in] csrc the value for the @p UCSR1C register
 * @note Must be invoked with interrupts disabled.
 * @note Does not reset the I/O queues.
 */
void usart1_setup(uint16_t brr, uint8_t csrc) {

  UBRR1L = brr;
  UBRR1H = brr >> 8;
  UCSR1A = 0;
  UCSR1B = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
  UCSR1C = csrc;
}
#endif /* USE_AVR_USART1 */

/**
 * @brief Serial driver initialization.
 * @note The serial ports are initialized at @p 38400-8-N-1 by default.
 */
void serial_init(void) {

#if USE_AVR_USART0
  /* I/O queues setup.*/
  chFDDInit(&SER1, ib1, sizeof ib1, NULL, ob1, sizeof ob1, OutNotify1);
  usart0_setup(UBRR(DEFAULT_USART_BITRATE), (1 << UCSZ1) | (1 << UCSZ0));
#endif

#if USE_AVR_USART1
  chFDDInit(&SER2, ib2, sizeof ib2, NULL, ob2, sizeof ob2, OutNotify2);
  usart1_setup(UBRR(DEFAULT_USART_BITRATE), (1 << UCSZ1) | (1 << UCSZ0));
#endif
}

/** @} */

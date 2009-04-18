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
 * @file ports/ARM7-AT91SAM7X/sam7x_serial.h
 * @brief AT91SAM7X Serial driver macros and structures.
 * @addtogroup AT91SAM7X_SERIAL
 * @{
 */

#include <ch.h>

#include "board.h"
#include "sam7x_serial.h"
#include "at91lib/aic.h"

#if USE_SAM7X_USART0 || defined(__DOXYGEN__)
/** @brief USART0 serial driver identifier.*/
FullDuplexDriver COM1;

static uint8_t ib1[SERIAL_BUFFERS_SIZE];
static uint8_t ob1[SERIAL_BUFFERS_SIZE];
#endif

#if USE_SAM7X_USART1 || defined(__DOXYGEN__)
/** @brief USART1 serial driver identifier.*/
FullDuplexDriver COM2;

static uint8_t ib2[SERIAL_BUFFERS_SIZE];
static uint8_t ob2[SERIAL_BUFFERS_SIZE];
#endif

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
  chSysLockFromIsr();
  chFDDAddFlagsI(com, sts);
  chSysUnlockFromIsr();
}

/** @cond never*/
__attribute__((noinline))
/** @endcond*/
/**
 * @brief Common IRQ handler.
 * @param[in] u pointer to an USART I/O block
 * @param[in] com communication channel associated to the USART
 */
static void ServeInterrupt(AT91PS_USART u, FullDuplexDriver *com) {

  if (u->US_CSR & AT91C_US_RXRDY) {
    chSysLockFromIsr();
    chFDDIncomingDataI(com, u->US_RHR);
    chSysUnlockFromIsr();
  }
  if (u->US_CSR & AT91C_US_TXRDY) {
    chSysLockFromIsr();
    msg_t b = chFDDRequestDataI(com);
    chSysUnlockFromIsr();
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

#if USE_SAM7X_USART0 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(USART0IrqHandler) {

  CH_IRQ_PROLOGUE();

  ServeInterrupt(AT91C_BASE_US0, &COM1);

  CH_IRQ_EPILOGUE();
}

static void OutNotify1(void) {

  AT91C_BASE_US0->US_IER = AT91C_US_TXRDY;
}
#endif

#if USE_SAM7X_USART1 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(USART1IrqHandler) {

  CH_IRQ_PROLOGUE();

  ServeInterrupt(AT91C_BASE_US1, &COM2);

  CH_IRQ_EPILOGUE();
}

static void OutNotify2(void) {

  AT91C_BASE_US1->US_IER = AT91C_US_TXRDY;
}
#endif

/**
 * @brief USART setup.
 * @param[in] u pointer to an UART I/O block
 * @param[in] speed serial port speed in bits per second
 * @param[in] mr the value for the @p MR register
 * @note Must be invoked with interrupts disabled.
 * @note Does not reset the I/O queues.
 */
void usart_setup(AT91PS_USART u, int speed, int mr) {

  /* Disables IRQ sources and stop operations.*/
  u->US_IDR = 0xFFFFFFFF;
  u->US_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RSTSTA;

  /* New parameters setup.*/
  if (mr & AT91C_US_OVER)
    u->US_BRGR = MCK / (speed * 8);
  else
    u->US_BRGR = MCK / (speed * 16);
  u->US_MR = mr;
  u->US_RTOR = 0;
  u->US_TTGR = 0;

  /* Enables operations and IRQ sources.*/
  u->US_CR = AT91C_US_RXEN | AT91C_US_TXEN | AT91C_US_DTREN | AT91C_US_RTSEN;
  u->US_IER = AT91C_US_RXRDY | AT91C_US_OVRE | AT91C_US_FRAME | AT91C_US_PARE |
              AT91C_US_RXBRK;
}

/**
 * @brief Serial driver initialization.
 * @param[in] prio0 priority to be assigned to the USART1 IRQ
 * @param[in] prio1 priority to be assigned to the USART2 IRQ
 * @note Handshake pads are not enabled inside this function because they
 *       may have another use, enable them externally if needed.
 *       RX and TX pads are handled inside.
 */
void serial_init(int prio0, int prio1) {

#if USE_SAM7X_USART0 || defined(__DOXYGEN__)
  /* I/O queue setup.*/
  chFDDInit(&COM1, ib1, sizeof ib1, NULL, ob1, sizeof ob1, OutNotify1);

  /* Switches the I/O pins to the peripheral function A, disables pullups.*/
  AT91C_BASE_PIOA->PIO_PDR   = AT91C_PA0_RXD0 | AT91C_PA1_TXD0;
  AT91C_BASE_PIOA->PIO_ASR   = AT91C_PIO_PA0 | AT91C_PIO_PA1;
  AT91C_BASE_PIOA->PIO_PPUDR = AT91C_PIO_PA0 | AT91C_PIO_PA1;

  /* Starts the clock and clears possible sources of immediate interrupts.*/
  AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_US0);
  AT91C_BASE_US0->US_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RSTSTA;

  /* Interrupts setup.*/
  AIC_ConfigureIT(AT91C_ID_US0,
                  AT91C_AIC_SRCTYPE_HIGH_LEVEL | prio0,
                  USART0IrqHandler);
  AIC_EnableIT(AT91C_ID_US0);

  /* Default parameters.*/
  usart_setup(AT91C_BASE_US0, DEFAULT_USART_BITRATE,
              AT91C_US_USMODE_NORMAL |
              AT91C_US_CLKS_CLOCK |
              AT91C_US_CHRL_8_BITS |
              AT91C_US_PAR_NONE |
              AT91C_US_NBSTOP_1_BIT);
#endif

#if USE_SAM7X_USART1 || defined(__DOXYGEN__)
  /* I/O queues setup.*/
  chFDDInit(&COM2, ib2, sizeof ib2, NULL, ob2, sizeof ob2, OutNotify2);

  /* Switches the I/O pins to the peripheral function A, disables pullups.*/
  AT91C_BASE_PIOA->PIO_PDR   = AT91C_PA5_RXD1 | AT91C_PA6_TXD1;
  AT91C_BASE_PIOA->PIO_ASR   = AT91C_PIO_PA5 | AT91C_PIO_PA6;
  AT91C_BASE_PIOA->PIO_PPUDR = AT91C_PIO_PA5 | AT91C_PIO_PA6;

  /* Starts the clock and clears possible sources of immediate interrupts.*/
  AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_US1);
  AT91C_BASE_US1->US_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RSTSTA;

  /* Interrupts setup.*/
  AIC_ConfigureIT(AT91C_ID_US1,
                  AT91C_AIC_SRCTYPE_HIGH_LEVEL | prio1,
                  USART1IrqHandler);
  AIC_EnableIT(AT91C_ID_US1);

  /* Default parameters.*/
  usart_setup(AT91C_BASE_US1, DEFAULT_USART_BITRATE,
              AT91C_US_USMODE_NORMAL |
              AT91C_US_CLKS_CLOCK |
              AT91C_US_CHRL_8_BITS |
              AT91C_US_PAR_NONE |
              AT91C_US_NBSTOP_1_BIT);
#endif
}

/** @} */

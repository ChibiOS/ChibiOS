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
 * @file ports/ARM7-LPC214x/lpc214x_serial.c
 * @brief LPC214x Serial driver code.
 * @addtogroup LPC214x_SERIAL
 * @{
 */

#include <ch.h>

#include "lpc214x.h"
#include "vic.h"
#include "lpc214x_serial.h"
#include "board.h"

#if USE_LPC214x_UART0 || defined(__DOXYGEN__)
/** @brief UART0 serial driver identifier.*/
FullDuplexDriver COM1;

static uint8_t ib1[SERIAL_BUFFERS_SIZE];
static uint8_t ob1[SERIAL_BUFFERS_SIZE];
#endif

#if USE_LPC214x_UART1 || defined(__DOXYGEN__)
/** @brief UART1 serial driver identifier.*/
FullDuplexDriver COM2;

static uint8_t ib2[SERIAL_BUFFERS_SIZE];
static uint8_t ob2[SERIAL_BUFFERS_SIZE];
#endif

/**
 * @brief Error handling routine.
 * @param[in] err UART LSR register value
 * @param[in] com communication channel associated to the USART
 */
static void SetError(IOREG32 err, FullDuplexDriver *com) {
  dflags_t sts = 0;

  if (err & LSR_OVERRUN)
    sts |= SD_OVERRUN_ERROR;
  if (err & LSR_PARITY)
    sts |= SD_PARITY_ERROR;
  if (err & LSR_FRAMING)
    sts |= SD_FRAMING_ERROR;
  if (err & LSR_BREAK)
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
 * @param[in] u pointer to an UART I/O block
 * @param[in] com communication channel associated to the UART
 * @note Tries hard to clear all the pending interrupt sources, we dont want to
 *       go through the whole ISR and have another interrupt soon after.
 */
static void ServeInterrupt(UART *u, FullDuplexDriver *com) {

  while (TRUE) {

    switch (u->UART_IIR & IIR_SRC_MASK) {
    case IIR_SRC_NONE:
      return;
    case IIR_SRC_ERROR:
      SetError(u->UART_LSR, com);
      break;
    case IIR_SRC_TIMEOUT:
    case IIR_SRC_RX:
      while (u->UART_LSR & LSR_RBR_FULL) {
        chSysLockFromIsr();
        if (chIQPutI(&com->sd_iqueue, u->UART_RBR) < Q_OK)
           chFDDAddFlagsI(com, SD_OVERRUN_ERROR);
        chSysUnlockFromIsr();
      }
      chSysLockFromIsr();
      chEvtBroadcastI(&com->sd_ievent);
      chSysUnlockFromIsr();
      break;
    case IIR_SRC_TX:
      {
#if UART_FIFO_PRELOAD > 0
        int i = UART_FIFO_PRELOAD;
        do {
          chSysLockFromIsr();
          msg_t b = chOQGetI(&com->sd_oqueue);
          chSysUnlockFromIsr();
          if (b < Q_OK) {
            u->UART_IER &= ~IER_THRE;
            chSysLockFromIsr();
            chEvtBroadcastI(&com->sd_oevent);
            chSysUnlockFromIsr();
            break;
          }
          u->UART_THR = b;
        } while (--i);
#else
        chSysLockFromIsr();
        msg_t b = chFDDRequestDataI(com);
        chSysUnlockFromIsr();
        if (b < Q_OK)
          u->UART_IER &= ~IER_THRE;
        else
          u->UART_THR = b;
#endif
      }
    default:
      u->UART_THR;
      u->UART_RBR;
    }
  }
}

#if UART_FIFO_PRELOAD > 0
static void preload(UART *u, FullDuplexDriver *com) {

  if (u->UART_LSR & LSR_THRE) {
    int i = UART_FIFO_PRELOAD;
    do {
      chSysLockFromIsr();
      msg_t b = chOQGetI(&com->sd_oqueue);
      chSysUnlockFromIsr();
      if (b < Q_OK) {
        chSysLockFromIsr();
        chEvtBroadcastI(&com->sd_oevent);
        chSysUnlockFromIsr();
        return;
      }
      u->UART_THR = b;
    } while (--i);
  }
  u->UART_IER |= IER_THRE;
}
#endif

#if USE_LPC214x_UART0 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(UART0IrqHandler) {

  CH_IRQ_PROLOGUE();

  ServeInterrupt(U0Base, &COM1);
  VICVectAddr = 0;

  CH_IRQ_EPILOGUE();
}

static void OutNotify1(void) {
#if UART_FIFO_PRELOAD > 0

  preload(U0Base, &COM1);
#else
  UART *u = U0Base;

  if (u->UART_LSR & LSR_THRE) {
    chSysLockFromIsr();
    u->UART_THR = chOQGetI(&COM1.sd_oqueue);
    chSysUnlockFromIsr();
  }
  u->UART_IER |= IER_THRE;
#endif
}
#endif

#if USE_LPC214x_UART1 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(UART1IrqHandler) {

  CH_IRQ_PROLOGUE();

  ServeInterrupt(U1Base, &COM2);
  VICVectAddr = 0;

  CH_IRQ_EPILOGUE();
}

static void OutNotify2(void) {
#if UART_FIFO_PRELOAD > 0

  preload(U1Base, &COM2);
#else
  UART *u = U1Base;

  if (u->UART_LSR & LSR_THRE)
    u->UART_THR = chOQGetI(&COM2.sd_oqueue);
  u->UART_IER |= IER_THRE;
#endif
}
#endif

/**
 * @brief UART setup.
 * @param[in] u pointer to an UART I/O block
 * @param[in] speed serial port speed in bits per second
 * @param[in] lcr the value for the @p LCR register
 * @param[in] fcr the value for the @p FCR register
 * @note Must be invoked with interrupts disabled.
 * @note Does not reset the I/O queues.
 */
void uart_setup(UART *u, int speed, int lcr, int fcr) {

  int div = PCLK / (speed << 4);
  u->UART_LCR = lcr | LCR_DLAB;
  u->UART_DLL = div;
  u->UART_DLM = div >> 8;
  u->UART_LCR = lcr;
  u->UART_FCR = FCR_ENABLE | FCR_RXRESET | FCR_TXRESET | fcr;
  u->UART_ACR = 0;
  u->UART_FDR = 0x10;
  u->UART_TER = TER_ENABLE;
  u->UART_IER = IER_RBR | IER_STATUS;
}

/**
 * @brief Serial driver initialization.
 * @param[in] vector1 IRC vector to be used for UART0
 * @param[in] vector2 IRC vector to be used for UART1
 * @note Handshake pads are not enabled inside this function because they
 *       may have another use, enable them externally if needed.
 *       RX and TX pads are handled inside.
 */
void serial_init(int vector1, int vector2) {

#if USE_LPC214x_UART0
  SetVICVector(UART0IrqHandler, vector1, SOURCE_UART0);
  PCONP = (PCONP & PCALL) | PCUART0;
  chFDDInit(&COM1, ib1, sizeof ib1, NULL, ob1, sizeof ob1, OutNotify1);
  uart_setup(U0Base,
             DEFAULT_UART_BITRATE,
             LCR_WL8 | LCR_STOP1 | LCR_NOPARITY,
             FCR_TRIGGER0);
  VICIntEnable = INTMASK(SOURCE_UART0);
#endif

#if USE_LPC214x_UART1
  SetVICVector(UART1IrqHandler, vector2, SOURCE_UART1);
  PCONP = (PCONP & PCALL) | PCUART1;
  chFDDInit(&COM2, ib2, sizeof ib2, NULL, ob2, sizeof ob2, OutNotify2);
  uart_setup(U1Base,
             DEFAULT_UART_BITRATE,
             LCR_WL8 | LCR_STOP1 | LCR_NOPARITY,
             FCR_TRIGGER0);
  VICIntEnable = INTMASK(SOURCE_UART0) | INTMASK(SOURCE_UART1);
#endif
}

/** @} */

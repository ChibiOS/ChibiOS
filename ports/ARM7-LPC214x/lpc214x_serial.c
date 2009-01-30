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

#include "lpc214x.h"
#include "vic.h"
#include "lpc214x_serial.h"
#include "board.h"

FullDuplexDriver COM1;
static uint8_t ib1[SERIAL_BUFFERS_SIZE];
static uint8_t ob1[SERIAL_BUFFERS_SIZE];

FullDuplexDriver COM2;
static uint8_t ib2[SERIAL_BUFFERS_SIZE];
static uint8_t ob2[SERIAL_BUFFERS_SIZE];

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
  chFDDAddFlagsI(com, sts);
}

/*
 * Tries hard to clear all the pending interrupt sources, we dont want to
 * go through the whole ISR and have another interrupt soon after.
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
      while (u->UART_LSR & LSR_RBR_FULL)
        if (chIQPutI(&com->sd_iqueue, u->UART_RBR) < Q_OK)
           chFDDAddFlagsI(com, SD_OVERRUN_ERROR);
      chEvtBroadcastI(&com->sd_ievent);
      break;
    case IIR_SRC_TX:
      {
#ifdef FIFO_PRELOAD
        int i = FIFO_PRELOAD;
        do {
          msg_t b = chOQGetI(&com->sd_oqueue);
          if (b < Q_OK) {
            u->UART_IER &= ~IER_THRE;
            chEvtBroadcastI(&com->sd_oevent);
            break;
          }
          u->UART_THR = b;
        } while (--i);
#else
        msg_t b = chFDDRequestDataI(com);
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

__attribute__((naked, weak))
void UART0IrqHandler(void) {

  chSysIRQEnterI();

  ServeInterrupt(U0Base, &COM1);
  VICVectAddr = 0;

  chSysIRQExitI();
}

__attribute__((naked, weak))
void UART1IrqHandler(void) {

  chSysIRQEnterI();

  ServeInterrupt(U1Base, &COM2);
  VICVectAddr = 0;

  chSysIRQExitI();
}

#ifdef FIFO_PRELOAD
static void preload(UART *u, FullDuplexDriver *com) {

  if (u->UART_LSR & LSR_THRE) {
    int i = FIFO_PRELOAD;
    do {
      msg_t b = chOQGetI(&com->sd_oqueue);
      if (b < Q_OK) {
        chEvtBroadcastI(&com->sd_oevent);
        return;
      }
      u->UART_THR = b;
    } while (--i);
  }
  u->UART_IER |= IER_THRE;
}
#endif

/*
 * Invoked by the high driver when one or more bytes are inserted in the
 * output queue.
 */
static void OutNotify1(void) {
#ifdef FIFO_PRELOAD

  preload(U0Base, &COM1);
#else
  UART *u = U0Base;

  if (u->UART_LSR & LSR_THRE)
    u->UART_THR = chOQGetI(&COM1.sd_oqueue);
  u->UART_IER |= IER_THRE;
#endif
}

/*
 * Invoked by the high driver when one or more bytes are inserted in the
 * output queue.
 */
static void OutNotify2(void) {
#ifdef FIFO_PRELOAD

  preload(U1Base, &COM2);
#else
  UART *u = U1Base;

  if (u->UART_LSR & LSR_THRE)
    u->UART_THR = chOQGetI(&COM2.sd_oqueue);
  u->UART_IER |= IER_THRE;
#endif
}

/*
 * UART setup, must be invoked with interrupts disabled.
 */
void SetUARTI(UART *u, int speed, int lcr, int fcr) {

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

/*
 * Serial subsystem initialization.
 */
void InitSerial(int vector1, int vector2) {

  SetVICVector(UART0IrqHandler, vector1, SOURCE_UART0);
  SetVICVector(UART1IrqHandler, vector2, SOURCE_UART1);

  PCONP = (PCONP & PCALL) | PCUART0 | PCUART1;

  chFDDInit(&COM1, ib1, sizeof ib1, NULL, ob1, sizeof ob1, OutNotify1);
  SetUARTI(U0Base, 38400, LCR_WL8 | LCR_STOP1 | LCR_NOPARITY, FCR_TRIGGER0);

  chFDDInit(&COM2, ib2, sizeof ib2, NULL, ob2, sizeof ob2, OutNotify2);
  SetUARTI(U1Base, 38400, LCR_WL8 | LCR_STOP1 | LCR_NOPARITY, FCR_TRIGGER0);

  VICIntEnable = INTMASK(SOURCE_UART0) | INTMASK(SOURCE_UART1);
}

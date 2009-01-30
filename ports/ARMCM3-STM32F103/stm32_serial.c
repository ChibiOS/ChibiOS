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
#include "nvic.h"
#include "stm32_serial.h"
#include "stm32lib/stm32f10x_nvic.h"

#define USART_BITRATE (38400)

#ifdef USE_USART1
FullDuplexDriver COM1;
static uint8_t ib1[SERIAL_BUFFERS_SIZE];
static uint8_t ob1[SERIAL_BUFFERS_SIZE];
#endif

#ifdef USE_USART2
FullDuplexDriver COM2;
static uint8_t ib2[SERIAL_BUFFERS_SIZE];
static uint8_t ob2[SERIAL_BUFFERS_SIZE];
#endif

#ifdef USE_USART3
FullDuplexDriver COM3;
static uint8_t ib3[SERIAL_BUFFERS_SIZE];
static uint8_t ob3[SERIAL_BUFFERS_SIZE];
#endif

static void SetError(uint16_t sr, FullDuplexDriver *com) {
  dflags_t sts = 0;

  if (sr & SR_ORE)
    sts |= SD_OVERRUN_ERROR;
  if (sr & SR_PE)
    sts |= SD_PARITY_ERROR;
  if (sr & SR_FE)
    sts |= SD_FRAMING_ERROR;
  if (sr & SR_LBD)
    sts |= SD_BREAK_DETECTED;
  chSysLock();
  chFDDAddFlagsI(com, sts);
  chSysUnlock();
}

static void ServeInterrupt(USART_TypeDef *u, FullDuplexDriver *com) {
  uint16_t sr = u->SR;

  if (sr & (SR_ORE | SR_FE | SR_PE | SR_LBD))
    SetError(sr, com);
  if (sr & SR_RXNE) {
    chSysLock();
    chFDDIncomingDataI(com, u->DR);
    chSysUnlock();
  }
  if (sr & SR_TXE) {
    chSysLock();
    msg_t b = chFDDRequestDataI(com);
    chSysUnlock();
    if (b < Q_OK)
      u->CR1 &= ~CR1_TXEIE;
    else
      u->DR = b;
  }
}

#ifdef USE_USART1
/*
 * USART1 IRQ service routine.
 */
void VectorD4(void) {

  chSysIRQEnterI();
  ServeInterrupt(USART1, &COM1);
  chSysIRQExitI();
}

/*
 * Invoked by the high driver when one or more bytes are inserted in the
 * output queue.
 */
static void OutNotify1(void) {

  USART1->CR1 |= CR1_TXEIE;
}
#endif

#ifdef USE_USART2
/*
 * USART2 IRQ service routine.
 */
void VectorD8(void) {

  chSysIRQEnterI();
  ServeInterrupt(USART2, &COM2);
  chSysIRQExitI();
}

/*
 * Invoked by the high driver when one or more bytes are inserted in the
 * output queue.
 */
static void OutNotify2(void) {

  USART2->CR1 |= CR1_TXEIE;
}
#endif

#ifdef USE_USART3
/*
 * USART3 IRQ service routine.
 */
void VectorDC(void) {

  chSysIRQEnterI();
  ServeInterrupt(USART3, &COM3);
  chSysIRQExitI();
}

/*
 * Invoked by the high driver when one or more bytes are inserted in the
 * output queue.
 */
static void OutNotify3(void) {

  USART3->CR1 |= CR1_TXEIE;
}
#endif

/*
 * USART setup, must be invoked with interrupts disabled.
 * NOTE: Does not reset I/O queues.
 */
void SetUSARTI(USART_TypeDef *u, uint32_t speed, uint16_t cr1,
               uint16_t cr2, uint16_t cr3) {

  /*
   * Baud rate setting.
   */
  if (u == USART1)
    u->BRR = APB2CLK / speed;
  else
    u->BRR = APB1CLK / speed;

  /*
   * Note that some bits are enforced.
   */
  u->CR1 = cr1 | CR1_UE | CR1_PEIE | CR1_RXNEIE | CR1_TE | CR1_RE;
  u->CR2 = cr2;
  u->CR3 = cr3 | CR3_EIE;
}

/*
 * Serial subsystem initialization.
 * NOTE: Handshake pins are not switched to their function because they may have
 *       another use. Enable them externally if needed.
 */
void InitSerial(uint32_t prio1, uint32_t prio2, uint32_t prio3) {

#ifdef USE_USART1
  chFDDInit(&COM1, ib1, sizeof ib1, NULL, ob1, sizeof ob1, OutNotify1);
  RCC->APB2ENR |= 0x00004000;
  SetUSARTI(USART1, USART_BITRATE, 0, CR2_STOP1_BITS | CR2_LINEN, 0);
  GPIOA->CRH = (GPIOA->CRH & 0xFFFFF00F) | 0x000004B0;
  NVICEnableVector(USART1_IRQChannel, prio1);
#endif

#ifdef USE_USART2
  chFDDInit(&COM2, ib2, sizeof ib2, NULL, ob2, sizeof ob2, OutNotify2);
  RCC->APB1ENR |= 0x00020000;
  SetUSARTI(USART2, USART_BITRATE, 0, CR2_STOP1_BITS | CR2_LINEN, 0);
  GPIOA->CRL = (GPIOA->CRL & 0xFFFF00FF) | 0x00004B00;
  NVICEnableVector(USART2_IRQChannel, prio2);
#endif

#ifdef USE_USART3
  chFDDInit(&COM3, ib3, sizeof ib3, NULL, ob3, sizeof ob3, OutNotify3);
  RCC->APB1ENR |= 0x00040000;
  SetUSARTI(USART3, USART_BITRATE, 0, CR2_STOP1_BITS | CR2_LINEN, 0);
  GPIOB->CRH = (GPIOB->CRH & 0xFFFF00FF) | 0x00004B00;
  NVICEnableVector(USART3_IRQChannel, prio3);
#endif
}

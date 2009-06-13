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
 * @file ports/ARMCM3-STM32F103/stm32_serial.c
 * @brief STM32F103 Serial driver code.
 * @addtogroup STM32F103_SERIAL
 * @{
 */

#include <ch.h>

#include "board.h"
#include "nvic.h"
#include "stm32_serial.h"
#include "stm32f10x_nvic.h"

#if USE_STM32_USART1 || defined(__DOXYGEN__)
/** @brief USART1 serial driver identifier.*/
FullDuplexDriver COM1;

static uint8_t ib1[SERIAL_BUFFERS_SIZE];
static uint8_t ob1[SERIAL_BUFFERS_SIZE];
#endif

#if USE_STM32_USART2 || defined(__DOXYGEN__)
/** @brief USART2 serial driver identifier.*/
FullDuplexDriver COM2;

static uint8_t ib2[SERIAL_BUFFERS_SIZE];
static uint8_t ob2[SERIAL_BUFFERS_SIZE];
#endif

#if USE_STM32_USART3 || defined(__DOXYGEN__)
/** @brief USART3 serial driver identifier.*/
FullDuplexDriver COM3;

static uint8_t ib3[SERIAL_BUFFERS_SIZE];
static uint8_t ob3[SERIAL_BUFFERS_SIZE];
#endif

/**
 * @brief Error handling routine.
 * @param[in] sr USART SR register value
 * @param[in] com communication channel associated to the USART
 */
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
  chSysLockFromIsr();
  chFDDAddFlagsI(com, sts);
  chSysUnlockFromIsr();
}

/**
 * @brief Common IRQ handler.
 * @param[in] u pointer to an USART I/O block
 * @param[in] com communication channel associated to the USART
 */
static void ServeInterrupt(USART_TypeDef *u, FullDuplexDriver *com) {
  uint16_t sr = u->SR;

  if (sr & (SR_ORE | SR_FE | SR_PE | SR_LBD))
    SetError(sr, com);
  if (sr & SR_RXNE) {
    chSysLockFromIsr();
    chFDDIncomingDataI(com, u->DR);
    chSysUnlockFromIsr();
  }
  if (sr & SR_TXE) {
    chSysLockFromIsr();
    msg_t b = chFDDRequestDataI(com);
    chSysUnlockFromIsr();
    if (b < Q_OK)
      u->CR1 &= ~CR1_TXEIE;
    else
      u->DR = b;
  }
}

#if USE_STM32_USART1 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(VectorD4) {

  CH_IRQ_PROLOGUE();

  ServeInterrupt(USART1, &COM1);

  CH_IRQ_EPILOGUE();
}

static void OutNotify1(void) {

  USART1->CR1 |= CR1_TXEIE;
}
#endif

#if USE_STM32_USART2 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(VectorD8) {

  CH_IRQ_PROLOGUE();

  ServeInterrupt(USART2, &COM2);

  CH_IRQ_EPILOGUE();
}

static void OutNotify2(void) {

  USART2->CR1 |= CR1_TXEIE;
}
#endif

#if USE_STM32_USART3 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(VectorDC) {

  CH_IRQ_PROLOGUE();

  ServeInterrupt(USART3, &COM3);

  CH_IRQ_EPILOGUE();
}

static void OutNotify3(void) {

  USART3->CR1 |= CR1_TXEIE;
}
#endif

/**
 * @brief USART1 setup.
 * @details This function must be invoked with interrupts disabled.
 * @param[in] u pointer to an USART I/O block
 * @param[in] speed serial port speed in bits per second
 * @param[in] cr1 the value for the @p CR1 register
 * @param[in] cr2 the value for the @p CR2 register
 * @param[in] cr3 the value for the @p CR3 register
 * @note Must be invoked with interrupts disabled.
 * @note Does not reset the I/O queues.
 */
void usart_setup(USART_TypeDef *u, uint32_t speed, uint16_t cr1,
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

/**
 * @brief Serial driver initialization.
 * @param[in] prio1 priority to be assigned to the USART1 IRQ
 * @param[in] prio2 priority to be assigned to the USART2 IRQ
 * @param[in] prio3 priority to be assigned to the USART3 IRQ
 * @note Handshake pads are not enabled inside this function because they
 *       may have another use, enable them externally if needed.
 *       RX and TX pads are handled inside.
 */
void serial_init(uint32_t prio1, uint32_t prio2, uint32_t prio3) {

#if USE_STM32_USART1
  chFDDInit(&COM1, ib1, sizeof ib1, NULL, ob1, sizeof ob1, OutNotify1);
  RCC->APB2ENR |= 0x00004000;
  usart_setup(USART1, DEFAULT_USART_BITRATE, 0,
              CR2_STOP1_BITS | CR2_LINEN, 0);
  GPIOA->CRH = (GPIOA->CRH & 0xFFFFF00F) | 0x000004B0;
  NVICEnableVector(USART1_IRQChannel, prio1);
#endif

#if USE_STM32_USART2
  chFDDInit(&COM2, ib2, sizeof ib2, NULL, ob2, sizeof ob2, OutNotify2);
  RCC->APB1ENR |= 0x00020000;
  usart_setup(USART2, DEFAULT_USART_BITRATE, 0,
              CR2_STOP1_BITS | CR2_LINEN, 0);
  GPIOA->CRL = (GPIOA->CRL & 0xFFFF00FF) | 0x00004B00;
  NVICEnableVector(USART2_IRQChannel, prio2);
#endif

#if USE_STM32_USART3
  chFDDInit(&COM3, ib3, sizeof ib3, NULL, ob3, sizeof ob3, OutNotify3);
  RCC->APB1ENR |= 0x00040000;
  usart_setup(USART3, DEFAULT_USART_BITRATE, 0,
              CR2_STOP1_BITS | CR2_LINEN, 0);
  GPIOB->CRH = (GPIOB->CRH & 0xFFFF00FF) | 0x00004B00;
  NVICEnableVector(USART3_IRQChannel, prio3);
#endif
}

/** @} */

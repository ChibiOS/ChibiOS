/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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

/**
 * @file    uart.c
 * @brief   UART Driver code.
 *
 * @addtogroup UART
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_UART || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   UART Driver initialization.
 */
void uartInit(void) {

  uart_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p UARTDriver structure.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
void uartObjectInit(UARTDriver *uartp) {

  uartp->ud_state   = UART_STOP;
  uartp->ud_txstate = UART_TX_IDLE;
  uartp->ud_rxstate = UART_RX_IDLE;
  uartp->ud_config  = NULL;
}

/**
 * @brief   Configures and activates the UART peripheral.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] config    pointer to the @p UARTConfig object
 */
void uartStart(UARTDriver *uartp, const UARTConfig *config) {

  chDbgCheck((uartp != NULL) && (config != NULL), "uartStart");

  chSysLock();
  chDbgAssert((uartp->ud_state == UART_STOP) ||
              (uartp->ud_state == UART_READY),
              "uartStart(), #1",
              "invalid state");

  uartp->ud_config = config;
  uart_lld_start(uartp);
  uartp->ud_state = UART_READY;
  chSysUnlock();
}

/**
 * @brief   Deactivates the UART peripheral.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
void uartStop(UARTDriver *uartp) {

  chDbgCheck(uartp != NULL, "uartStop");

  chSysLock();
  chDbgAssert((uartp->ud_state == UART_STOP) ||
              (uartp->ud_state == UART_READY),
              "uartStop(), #1",
              "invalid state");

  uart_lld_stop(uartp);
  uartp->ud_state = UART_STOP;
  uartp->ud_txstate = UART_TX_IDLE;
  uartp->ud_rxstate = UART_RX_IDLE;
  chSysUnlock();
}

/**
 * @brief   Starts a transmission on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[in] txbuf     the pointer to the transmit buffer
 */
void uartStartSend(UARTDriver *uartp, size_t n, const void *txbuf) {

  chDbgCheck((uartp != NULL) && (n > 0) && (txbuf != NULL),
             "uartStartSend");
             
  chSysLock();
  chDbgAssert((uartp->ud_state == UART_READY) &&
              (uartp->ud_txstate == UART_TX_IDLE),
              "uartStartSend(), #1",
              "not active");

  uart_lld_start_send(uartp, n, txbuf);
  uartp->ud_txstate = UART_TX_ACTIVE;
  chSysUnlock();
}

/**
 * @brief   Starts a transmission on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 * @note    This function has to be invoked from a lock zone.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[in] txbuf     the pointer to the transmit buffer
 */
void uartStartSendI(UARTDriver *uartp, size_t n, const void *txbuf) {

  chDbgCheck((uartp != NULL) && (n > 0) && (txbuf != NULL),
             "uartStartSendI");

  chDbgAssert((uartp->ud_state == UART_READY) &&
              (uartp->ud_txstate != UART_TX_ACTIVE),
              "uartStartSendI(), #1",
              "not active");
  uart_lld_start_send(uartp, n, txbuf);
  uartp->ud_txstate = UART_TX_ACTIVE;
}

/**
 * @brief   Stops any ongoing transmission.
 * @note    Stopping a transmission also suppresses the transmission callbacks.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
void uartStopSend(UARTDriver *uartp) {

  chDbgCheck(uartp != NULL, "uartStopSend");

  chSysLock();
  chDbgAssert(uartp->ud_state == UART_READY,
              "uartStopSend(), #1",
              "not active");

  if (uartp->ud_txstate == UART_TX_ACTIVE) {
    uart_lld_stop_send(uartp);
    uartp->ud_txstate = UART_TX_IDLE;
  }
  chSysUnlock();
}

/**
 * @brief   Stops any ongoing transmission.
 * @note    Stopping a transmission also suppresses the transmission callbacks.
 * @note    This function has to be invoked from a lock zone.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
void uartStopSendI(UARTDriver *uartp) {

  chDbgCheck(uartp != NULL, "uartStopSendI");

  chDbgAssert(uartp->ud_state == UART_READY,
              "uartStopSendI(), #1",
              "not active");

  if (uartp->ud_txstate == UART_TX_ACTIVE) {
    uart_lld_stop_send(uartp);
    uartp->ud_txstate = UART_TX_IDLE;
  }
}

/**
 * @brief   Starts a receive operation on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[in] rxbuf     the pointer to the receive buffer
 */
void uartStartReceive(UARTDriver *uartp, size_t n, void *rxbuf) {

  chDbgCheck((uartp != NULL) && (n > 0) && (rxbuf != NULL),
             "uartStartReceive");

  chSysLock();
  chDbgAssert((uartp->ud_state == UART_READY) &&
              (uartp->ud_rxstate == UART_RX_IDLE),
              "uartStartReceive(), #1",
              "not active");

  uart_lld_start_receive(uartp, n, rxbuf);
  uartp->ud_rxstate = UART_RX_ACTIVE;
  chSysUnlock();
}

/**
 * @brief   Starts a receive operation on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 * @note    This function has to be invoked from a lock zone.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[in] rxbuf     the pointer to the receive buffer
 */
void uartStartReceiveI(UARTDriver *uartp, size_t n, void *rxbuf) {

  chDbgCheck((uartp != NULL) && (n > 0) && (rxbuf != NULL),
             "uartStartReceiveI");

  chDbgAssert((uartp->ud_state == UART_READY) &&
              (uartp->ud_rxstate == UART_RX_IDLE),
              "uartStartReceiveI(), #1",
              "not active");

  uart_lld_start_receive(uartp, n, rxbuf);
  uartp->ud_rxstate = UART_RX_ACTIVE;
}

/**
 * @brief   Stops any ongoing receive operation.
 * @note    Stopping a receive operation also suppresses the receive callbacks.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
void uartStopReceive(UARTDriver *uartp) {

  chDbgCheck(uartp != NULL, "uartStopReceive");

  chSysLock();
  chDbgAssert(uartp->ud_state == UART_READY,
              "uartStopReceive(), #1",
              "not active");

  if (uartp->ud_rxstate == UART_RX_ACTIVE) {
    uart_lld_stop_receive(uartp);
    uartp->ud_rxstate = UART_RX_IDLE;
  }
  chSysUnlock();
}

/**
 * @brief   Stops any ongoing receive operation.
 * @note    Stopping a receive operation also suppresses the receive callbacks.
 * @note    This function has to be invoked from a lock zone.
 *
 * @param[in] uartp      pointer to the @p UARTDriver object
 */
void uartStopReceiveI(UARTDriver *uartp) {

  chDbgCheck(uartp != NULL, "uartStopReceiveI");

  chDbgAssert(uartp->ud_state == UART_READY,
              "uartStopReceiveI(), #1",
              "not active");

  if (uartp->ud_rxstate == UART_RX_ACTIVE) {
    uart_lld_stop_receive(uartp);
    uartp->ud_rxstate = UART_RX_IDLE;
  }
}

#endif /* CH_HAL_USE_UART */

/** @} */

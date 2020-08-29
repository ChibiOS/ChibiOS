/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    hal_sio_lld.c
 * @brief   PLATFORM SIO subsystem low level driver source.
 *
 * @addtogroup SIO
 * @{
 */

#include "hal.h"

#if (HAL_USE_SIO == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   SIO1 driver identifier.
 */
#if (STM32_SIO_USE_USART1 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SIO driver initialization.
 *
 * @notapi
 */
void sio_lld_init(void) {

#if STM32_SIO_USE_USART1 == TRUE
  /* Driver initialization.*/
  sioObjectInit(&SIOD1);
#endif
}

/**
 * @brief   Configures and activates the SIO peripheral.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The operation status.
 * @retval false        if the driver has been correctly started.
 * @retval true         if an error occurred.
 *
 * @notapi
 */
bool sio_lld_start(SIODriver *siop) {

  if (siop->state == SIO_STOP) {
    /* Enables the peripheral.*/
#if STM32_SIO_USE_USART1 == TRUE
    if (&SIOD1 == siop) {

    }
#endif
  }
  /* Configures the peripheral.*/

  return false;
}
/**
 * @brief   Starts a SIO operation.
 *
 * @param[in] siop          pointer to an @p SIODriver structure
 *
 * @api
 */
void sio_lld_start_operation(SIODriver *siop) {
}

/**
 * @brief   Stops an ongoing SIO operation, if any.
 *
 * @param[in] siop      pointer to an @p SIODriver structure
 *
 * @api
 */
void sio_lld_stop_operation(SIODriver *siop) {
}

/**
 * @brief   Deactivates the SIO peripheral.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 *
 * @notapi
 */
void sio_lld_stop(SIODriver *siop) {

  if (siop->state == SIO_READY) {
    /* Resets the peripheral.*/

    /* Disables the peripheral.*/
#if STM32_SIO_USE_USART1 == TRUE
    if (&SIOD1 == siop) {

    }
#endif
  }
}
/**
 * @brief   Reads data from the RX FIFO.
 * @details The function is not blocking, it writes frames until there
 *          is space available without waiting.
 *
 * @param[in] siop          pointer to an @p SIODriver structure
 * @param[in] n             maximum number of frames to be read
 * @param[in] buffer        pointer to the buffer for read frames
 * @return                  The number of frames copied from the buffer.
 * @retval 0                if the TX FIFO is full.
 */
size_t sio_lld_read(SIODriver *siop, size_t n, uint8_t *buffer) {
  size_t rd;

  rd = 0U;
  while (true) {

#if USART_ENABLE_INTERRUPTS == TRUE
  /* If the RX FIFO has been emptied then the interrupt is enabled again.*/
  if (sio_lld_is_rx_empty(siop)) {
    siop->usart->CR3 |= USART_CR3_RXFTIE;
    break;
  }
#endif

    /* Buffer filled condition.*/
    if (rd > n) {
      break;
    }

    *buffer++ = (uint8_t)siop->usart->RDR;
    rd++;
  }

  return n - rd;
}

/**
 * @brief   Writes data into the TX FIFO.
 * @details The function is not blocking, it writes frames until there
 *          is space available without waiting.
 *
 * @param[in] siop          pointer to an @p SIODriver structure
 * @param[in] n             maximum number of frames to be written
 * @param[in] buffer        pointer to the buffer for read frames
 * @return                  The number of frames copied from the buffer.
 * @retval 0                if the TX FIFO is full.
 */
size_t sio_lld_write(SIODriver *siop, size_t n, const uint8_t *buffer) {
  size_t wr;

  wr = 0U;
  while (true) {

#if USART_ENABLE_INTERRUPTS == TRUE
    /* If the TX FIFO has been filled then the interrupt is enabled again.*/
    if (sio_lld_is_tx_full(siop)) {
      siop->usart->CR3 |= USART_CR3_TXFTIE;
      break;
    }
#endif

    /* Buffer emptied condition.*/
    if (wr >= n) {
      break;
    }

    siop->usart->TDR = (uint32_t)*buffer++;
    wr++;
  }

#if USART_ENABLE_INTERRUPTS == TRUE
  /* The transmit complete interrupt is always re-enabled on write.*/
  siop->usart->CR1 |= USART_CR1_TCIE;
#endif

  return n - wr;
}

/**
 * @brief   Control operation on a serial port.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] operation control operation code
 * @param[in,out] arg   operation argument
 *
 * @return              The control operation status.
 * @retval MSG_OK       in case of success.
 * @retval MSG_TIMEOUT  in case of operation timeout.
 * @retval MSG_RESET    in case of operation reset.
 *
 * @notapi
 */
msg_t sio_lld_control(SIODriver *siop, unsigned int operation, void *arg) {

  (void)siop;
  (void)operation;
  (void)arg;

  return MSG_OK;
}

#endif /* HAL_USE_SIO == TRUE */

/** @} */

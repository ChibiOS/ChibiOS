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
 * @file    sandbox/hal_sio_lld.c
 * @brief   Sandbox SIO subsystem low level driver source.
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
 * @brief   VUART1 SIO driver identifier.
 */
#if (SB_SIO_USE_VUART1 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD1;
#endif

/**
 * @brief   VUART2 SIO driver identifier.
 */
#if (SB_SIO_USE_VUART2 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static const SIOConfig default_config = {
  .ncfg  = 0U
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if SB_SIO_USE_VUART1 || defined(__DOXYGEN__)
#if !defined(SB_VUART1_SUPPRESS_ISR)
OSAL_IRQ_HANDLER(MK_VECTOR(SB_SIO_VUART1_IRQ)) {

}
#endif
#endif

#if SB_SIO_USE_VUART2 || defined(__DOXYGEN__)
#if !defined(SB_VUART2_SUPPRESS_ISR)
OSAL_IRQ_HANDLER(MK_VECTOR(SB_SIO_VUART2_IRQ)) {

}
#endif
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SIO driver initialization.
 *
 * @notapi
 */
void sio_lld_init(void) {

  /* Driver instances initialization.*/
#if SB_SIO_USE_VUART1 == TRUE
  sioObjectInit(&SIOD1);
  SIOD1.nvuart = 0U;
  __sb_vrq_seten(1U << SB_SIO_VUART1_IRQ);
#endif
#if SB_SIO_USE_VUART2 == TRUE
  sioObjectInit(&SIOD2);
  SIOD2.nvuart = 1U;
  __sb_vrq_seten(1U << SB_SIO_VUART2_IRQ);
#endif
}

/**
 * @brief   Configures and activates the SIO peripheral.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t sio_lld_start(SIODriver *siop) {

  /* Using the default configuration if the application passed a
     NULL pointer.*/
  if (siop->config == NULL) {
    siop->config = &default_config;
  }

  if (siop->state == SIO_STOP) {

  /* Enables the peripheral.*/
    if (false) {
    }
#if SB_SIO_USE_VUART1 == TRUE
    else if (&SIOD1 == siop) {
    }
#endif
#if SB_SIO_USE_VUART2 == TRUE
    else if (&SIOD2 == siop) {
    }
#endif
    else {
      osalDbgAssert(false, "invalid USART instance");
    }

    /* Driver object low level initializations.*/
#if SIO_USE_SYNCHRONIZATION
    siop->sync_rx      = NULL;
    siop->sync_tx      = NULL;
    siop->sync_txend   = NULL;
#endif
  }

  return HAL_RET_SUCCESS;
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
    if (false) {
    }
#if SB_SIO_USE_VUART1 == TRUE
    else if (&SIOD1 == siop) {
    }
#endif
#if SB_SIO_USE_VUART2 == TRUE
    else if (&SIOD2 == siop) {
    }
#endif
    else {
      osalDbgAssert(false, "invalid USART instance");
    }
  }
}

/**
 * @brief   Starts a SIO operation.
 *
 * @param[in] siop          pointer to an @p SIODriver structure
 *
 * @api
 */
void sio_lld_start_operation(SIODriver *siop) {

  (void)siop;
}

/**
 * @brief   Stops an ongoing SIO operation, if any.
 *
 * @param[in] siop      pointer to an @p SIODriver structure
 *
 * @api
 */
void sio_lld_stop_operation(SIODriver *siop) {

  (void)siop;
}

/**
 * @brief   Return the pending SIO events flags.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The pending event flags.
 *
 * @notapi
 */
sio_events_mask_t sio_lld_get_and_clear_events(SIODriver *siop) {
  sio_events_mask_t evtmask = 0U;

  (void)siop;

  return evtmask;
}

/**
 * @brief   Reads data from the RX FIFO.
 * @details The function is not blocking, it writes frames until there
 *          is space available without waiting.
 *
 * @param[in] siop          pointer to an @p SIODriver structure
 * @param[in] buffer        pointer to the buffer for read frames
 * @param[in] n             maximum number of frames to be read
 * @return                  The number of frames copied from the buffer.
 * @retval 0                if the TX FIFO is full.
 */
size_t sio_lld_read(SIODriver *siop, uint8_t *buffer, size_t n) {
//  size_t rd;

  (void)siop;
  (void)buffer;
  (void)n;

  return 0;//rd;
}

/**
 * @brief   Writes data into the TX FIFO.
 * @details The function is not blocking, it writes frames until there
 *          is space available without waiting.
 *
 * @param[in] siop          pointer to an @p SIODriver structure
 * @param[in] buffer        pointer to the buffer for read frames
 * @param[in] n             maximum number of frames to be written
 * @return                  The number of frames copied from the buffer.
 * @retval 0                if the TX FIFO is full.
 */
size_t sio_lld_write(SIODriver *siop, const uint8_t *buffer, size_t n) {
//  size_t wr;

  (void)siop;
  (void)buffer;
  (void)n;

  return 0;//wr;
}

/**
 * @brief   Returns one frame from the RX FIFO.
 * @note    If the FIFO is empty then the returned value is unpredictable.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The frame from RX FIFO.
 *
 * @notapi
 */
msg_t sio_lld_get(SIODriver *siop) {
//  msg_t msg;

  (void)siop;

  return 0;//msg;
}

/**
 * @brief   Pushes one frame into the TX FIFO.
 * @note    If the FIFO is full then the behavior is unpredictable.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] data      frame to be written
 *
 * @notapi
 */
void sio_lld_put(SIODriver *siop, uint_fast16_t data) {

  (void)siop;
  (void)data;
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

/**
 * @brief   Serves an USART interrupt.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 *
 * @notapi
 */
void sio_lld_serve_interrupt(SIODriver *siop) {

  (void)siop;
}

#endif /* HAL_USE_SIO == TRUE */

/** @} */

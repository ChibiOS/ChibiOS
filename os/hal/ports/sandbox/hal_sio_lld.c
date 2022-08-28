/*
    ChibiOS - Copyright (C) 2006..2021 Giovanni Di Sirio

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

__attribute__((always_inline))
static inline uint32_t __sio_vuart_init(uint32_t nvuart, uint32_t ncfg) {

  __syscall3r(201, SB_VUART_INIT, nvuart, ncfg);
  return (uint32_t)r0;
}

__attribute__((always_inline))
static inline uint32_t __sio_vuart_deinit(uint32_t nvuart) {

  __syscall2r(201, SB_VUART_INIT, nvuart);
  return (uint32_t)r0;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if SB_SIO_USE_VUART1 || defined(__DOXYGEN__)
#if !defined(SB_VUART1_SUPPRESS_ISR)
OSAL_IRQ_HANDLER(MK_VECTOR(SB_SIO_VUART1_IRQ)) {

  sio_lld_serve_interrupt(&SIOD1);
}
#endif
#endif

#if SB_SIO_USE_VUART2 || defined(__DOXYGEN__)
#if !defined(SB_VUART2_SUPPRESS_ISR)
OSAL_IRQ_HANDLER(MK_VECTOR(SB_SIO_VUART2_IRQ)) {

  sio_lld_serve_interrupt(&SIOD2);
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
  msg_t msg = HAL_RET_SUCCESS;

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
      msg = (msg_t)__sio_vuart_init(siop->nvuart, siop->config->ncfg);
    }
#endif
#if SB_SIO_USE_VUART2 == TRUE
    else if (&SIOD2 == siop) {
      msg = (msg_t)__sio_vuart_init(siop->nvuart, siop->config->ncfg);
    }
#endif
    else {
      osalDbgAssert(false, "invalid SIO instance");
    }
  }

  /* Configures the peripheral.*/

  return msg;
}

/**
 * @brief   Deactivates the SIO peripheral.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 *
 * @notapi
 */
void sio_lld_stop(SIODriver *siop) {
  msg_t msg = HAL_RET_SUCCESS;

  if (siop->state == SIO_READY) {
    /* Disables the peripheral.*/
    if (false) {
    }
#if SB_SIO_USE_VUART1 == TRUE
    else if (&SIOD1 == siop) {
      msg = __sio_vuart_deinit(siop->nvuart);
    }
#endif
#if SB_SIO_USE_VUART2 == TRUE
    else if (&SIOD2 == siop) {
      msg = __sio_vuart_deinit(siop->nvuart);
    }
#endif
    else {
      osalDbgAssert(false, "invalid SIO instance");
    }
  }

  osalDbgAssert(msg = HAL_RET_SUCCESS, "unexpected failure");
}

/**
 * @brief   Determines the state of the RX FIFO.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The RX FIFO state.
 * @retval false        if RX FIFO is not empty
 * @retval true         if RX FIFO is empty
 *
 * @notapi
 */
bool sio_lld_is_rx_empty(SIODriver *siop) {

  __syscall2rr(201, SB_VUART_ISRXE, siop->nvuart);
  osalDbgAssert((msg_t)r0 == HAL_RET_SUCCESS, "unexpected failure");

  return (bool)r1;
}

/**
 * @brief   Determines the activity state of the receiver.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The RX activity state.
 * @retval false        if RX is in active state.
 * @retval true         if RX is in idle state.
 *
 * @notapi
 */
bool sio_lld_is_rx_idle(SIODriver *siop) {

  __syscall2rr(201, SB_VUART_ISRXI, siop->nvuart);
  osalDbgAssert((msg_t)r0 == HAL_RET_SUCCESS, "unexpected failure");

  return (bool)r1;
}

/**
 * @brief   Determines if RX has pending error events to be read and cleared.
 * @note    Only error and protocol errors are handled, data events are not
 *          considered.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The RX error events.
 * @retval false        if RX has no pending events
 * @retval true         if RX has pending events
 *
 * @notapi
 */
bool sio_lld_has_rx_errors(SIODriver *siop) {

  __syscall2rr(201, SB_VUART_HASERR, siop->nvuart);
  osalDbgAssert((msg_t)r0 == HAL_RET_SUCCESS, "unexpected failure");

  return (bool)r1;
}

/**
 * @brief   Determines the state of the TX FIFO.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The TX FIFO state.
 * @retval false        if TX FIFO is not full
 * @retval true         if TX FIFO is full
 *
 * @notapi
 */
bool sio_lld_is_tx_full(SIODriver *siop) {

  __syscall2rr(201, SB_VUART_ISTXF, siop->nvuart);
  osalDbgAssert((msg_t)r0 == HAL_RET_SUCCESS, "unexpected failure");

  return (bool)r1;
}

/**
 * @brief   Determines the transmission state.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The TX FIFO state.
 * @retval false        if transmission is idle
 * @retval true         if transmission is ongoing
 *
 * @notapi
 */
bool sio_lld_is_tx_ongoing(SIODriver *siop) {

  __syscall2rr(201, SB_VUART_ISTXO, siop->nvuart);
  osalDbgAssert((msg_t)r0 == HAL_RET_SUCCESS, "unexpected failure");

  return (bool)r1;
}

/**
 * @brief   Enable flags change notification.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 */
void sio_lld_update_enable_flags(SIODriver *siop) {

  __syscall3r(201, SB_VUART_WREN, siop->nvuart, siop->enabled);
  osalDbgAssert((msg_t)r0 == HAL_RET_SUCCESS, "unexpected failure");
}

/**
 * @brief   Get and clears SIO error event flags.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The pending event flags.
 *
 * @notapi
 */
sioevents_t sio_lld_get_and_clear_errors(SIODriver *siop) {

  __syscall2r(201, SB_VUART_GCERR, siop->nvuart);
  osalDbgAssert((msg_t)r0 == HAL_RET_SUCCESS, "unexpected failure");

  return (sioevents_t)r1;
}

/**
 * @brief   Get and clears SIO event flags.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The pending event flags.
 *
 * @notapi
 */
sioevents_t sio_lld_get_and_clear_events(SIODriver *siop) {

  __syscall2r(201, SB_VUART_GCEVT, siop->nvuart);
  osalDbgAssert((msg_t)r0 == HAL_RET_SUCCESS, "unexpected failure");

  return (sioevents_t)r1;
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

  __syscall4rr(201, SB_VUART_READ, siop->nvuart, buffer, n);
  osalDbgAssert((msg_t)r0 == HAL_RET_SUCCESS, "unexpected failure");

  return (size_t)r1;
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

  __syscall4rr(201, SB_VUART_WRITE, siop->nvuart, buffer, n);
  osalDbgAssert((msg_t)r0 == HAL_RET_SUCCESS, "unexpected failure");

  return (size_t)r1;
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

  __syscall2rr(201, SB_VUART_GET, siop->nvuart);
  osalDbgAssert((msg_t)r0 == HAL_RET_SUCCESS, "unexpected failure");

  return (size_t)r1;
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

  __syscall3r(201, SB_VUART_PUT, siop->nvuart, data);
  osalDbgAssert((msg_t)r0 == HAL_RET_SUCCESS, "unexpected failure");
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

  __syscall4rr(201, SB_VUART_CTL, siop->nvuart, operation, arg);
  osalDbgAssert((msg_t)r0 == HAL_RET_SUCCESS, "unexpected failure");

  return (size_t)r1;
}

/**
 * @brief   Serves an VUART interrupt.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 *
 * @notapi
 */
void sio_lld_serve_interrupt(SIODriver *siop) {

  /* The callback is simply invoked. Letting the driver on the host side
     decide how to manipulate IRQ sources.*/
  __sio_callback(siop);
}

#endif /* HAL_USE_SIO == TRUE */

/** @} */

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
#endif
#if SB_SIO_USE_VUART2 == TRUE
  sioObjectInit(&SIOD2);
  SIOD2.nvuart = 1U;
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
  sio_events_mask_t evtmask;
  uint32_t isr;

  /* Getting and clearing all relevant ISR flags (and only those).*/
  isr = siop->usart->ISR & (USART_ISR_PE  | USART_ISR_LBDF | USART_ISR_FE    |
                            USART_ISR_ORE | USART_ISR_NE);
  siop->usart->ICR = isr;

  /* Status flags cleared, now the related interrupts can be enabled again.*/
  usart_enable_rx_evt_irq(siop);

  /* Translating the status flags in SIO events.*/
  evtmask = 0U;
  if ((isr & USART_ISR_LBDF) != 0U) {
    evtmask |= SIO_BREAK_DETECTED;
  }
  if ((isr & USART_ISR_ORE) != 0U) {
    evtmask |= SIO_OVERRUN_ERROR;
  }
  if ((isr & USART_ISR_NE) != 0U) {
    evtmask |= SIO_NOISE_ERROR;
  }
  if ((isr & USART_ISR_FE) != 0U) {
    evtmask |= SIO_FRAMING_ERROR;
  }
  if ((isr & USART_ISR_PE) != 0U) {
    evtmask |= SIO_PARITY_ERROR;
  }

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
  size_t rd;

  rd = 0U;
  while (true) {

    /* If the RX FIFO has been emptied then the interrupt is enabled again.*/
    if (sio_lld_is_rx_empty(siop)) {
      usart_enable_rx_irq(siop);
      break;
    }

    /* Buffer filled condition.*/
    if (rd >= n) {
      break;
    }

    *buffer++ = (uint8_t)siop->usart->RDR;
    rd++;
  }

  return rd;
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
  size_t wr;

  wr = 0U;
  while (true) {

    /* If the TX FIFO has been filled then the interrupt is enabled again.*/
    if (sio_lld_is_tx_full(siop)) {
      usart_enable_tx_irq(siop);
      break;
    }

    /* Buffer emptied condition.*/
    if (wr >= n) {
      break;
    }

    siop->usart->TDR = (uint32_t)*buffer++;
    wr++;
  }

  /* The transmit complete interrupt is always re-enabled on write.*/
  usart_enable_tx_end_irq(siop);

  return wr;
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
  msg_t msg;

  msg = (msg_t)siop->usart->RDR;

  /* If the RX FIFO has been emptied then the interrupt is enabled again.*/
  if (sio_lld_is_rx_empty(siop)) {
    usart_enable_rx_irq(siop);
  }

  return msg;
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

  siop->usart->TDR = data;

  /* If the TX FIFO has been filled then the interrupt is enabled again.*/
  if (sio_lld_is_tx_full(siop)) {
    usart_enable_tx_irq(siop);
  }

  /* The transmit complete interrupt is always re-enabled on write.*/
  usart_enable_tx_end_irq(siop);
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
  USART_TypeDef *u = siop->usart;
  uint32_t isr, cr1, cr2, cr3, evtmask, irqmask;

  osalDbgAssert(siop->state == SIO_ACTIVE, "invalid state");

  /* Note, ISR flags are just read but not cleared, ISR sources are
     disabled instead.*/
  isr = u->ISR;

  /* Read on control registers.*/
  cr1 = u->CR1;
  cr2 = u->CR2;
  cr3 = u->CR3;

  /* Enabled errors/events handling.*/
  irqmask = ((cr1 & USART_CR1_PEIE)   != 0U ? USART_ISR_PE   : 0U) |
            ((cr1 & USART_CR1_RXNEIE) != 0U ? USART_ISR_ORE  : 0U) |
            ((cr2 & USART_CR2_LBDIE)  != 0U ? USART_ISR_LBDF : 0U) |
            ((cr3 & USART_CR3_EIE)    != 0U ? USART_ISR_FE  |
                                              USART_ISR_ORE |
                                              USART_ISR_NE   : 0U);
  evtmask = isr & irqmask;
  if (evtmask != 0U) {

    /* Disabling event sources until errors are recognized by the
       application.*/
    u->CR1 = cr1 & ~USART_CR1_PEIE;
    u->CR2 = cr2 & ~USART_CR2_LBDIE;
    u->CR3 = cr3 & ~USART_CR3_EIE;

    /* The callback is invoked if defined.*/
    __sio_callback_rx_evt(siop);

    /* Waiting thread woken, if any.*/
    __sio_wakeup_rx(siop, SIO_MSG_ERRORS);

    /* Values could have been changed by the callback, CR2-CR3 no more needed.*/
    cr1 = u->CR1;
  }

  /* RX FIFO is non-empty.*/
  if (((cr1 & USART_CR1_RXNEIE) != 0U) &&
      (isr & USART_ISR_RXNE) != 0U) {

    /* Called once then the interrupt source is disabled.*/
    u->CR1 = cr1 & ~USART_CR1_RXNEIE;

    /* The callback is invoked if defined.*/
    __sio_callback_rx(siop);

    /* Waiting thread woken, if any.*/
    __sio_wakeup_rx(siop, MSG_OK);

    /* Values could have been changed by the callback, CR2-CR3 no more needed.*/
    cr1 = u->CR1;
  }

  /* TX FIFO is non-full.*/
  if (((cr1 & USART_CR1_TXEIE) != 0U) &&
      (isr & USART_ISR_TXE) != 0U) {

    /* Called once then the interrupt is disabled.*/
    u->CR1 = cr1 & ~USART_CR1_TXEIE;

    /* The callback is invoked if defined.*/
    __sio_callback_tx(siop);

    /* Waiting thread woken, if any.*/
    __sio_wakeup_tx(siop, MSG_OK);

    /* Values could have been changed by the callback, CR2-CR3 no more needed.*/
    cr1 = u->CR1;
  }

  /* RX idle condition.*/
  if (((cr1 & USART_CR1_IDLEIE) != 0U) &&
      (isr & USART_ISR_IDLE) != 0U) {

    /* The idle flag requires clearing, it stays enabled.*/
    u->ICR = USART_ISR_IDLE;

    /* The callback is invoked if defined.*/
    __sio_callback_rx_idle(siop);

    /* Waiting thread woken, if any.*/
    __sio_wakeup_rx(siop, SIO_MSG_IDLE);

    /* Values could have been changed by the callback, CR2-CR3 no more needed.*/
    cr1 = u->CR1;
  }

  /* Physical transmission end.*/
  if (((cr1 & USART_CR1_TCIE) != 0U) &&
      (isr & USART_ISR_TC) != 0U) {

    /* Called once then the interrupt is disabled.*/
    u->CR1 = cr1 & ~USART_CR1_TCIE;

    /* The callback is invoked if defined.*/
    __sio_callback_tx_end(siop);

    /* Waiting thread woken, if any.*/
    __sio_wakeup_txend(siop, MSG_OK);
  }
}

#endif /* HAL_USE_SIO == TRUE */

/** @} */

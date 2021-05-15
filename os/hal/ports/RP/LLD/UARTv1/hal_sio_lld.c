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
 * @file    UARTv1/hal_sio_lld.c
 * @brief   RP SIO subsystem low level driver source.
 *
 * @addtogroup SIO
 * @{
 */

#include "hal.h"

#if (HAL_USE_SIO == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define UART_LCRH_CFG_FORBIDDEN     (UART_UARTLCR_H_BRK)
#define UART_CR_CFG_FORBIDDEN       (UART_UARTCR_RXE    |                   \
                                     UART_UARTCR_TXE    |                   \
                                     UART_UARTCR_SIRLP  |                   \
                                     UART_UARTCR_SIREN  |                   \
                                     UART_UARTCR_UARTEN)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   UART0 SIO driver identifier.
 */
#if (RP_SIO_USE_UART0 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD0;
#endif

/**
 * @brief   UART1 SIO driver identifier.
 */
#if (RP_SIO_USE_UART1 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 * @note    In this implementation it is: 38400-8-N-1.
 */
static const SIOConfig default_config = {
  .baud         = SIO_DEFAULT_BITRATE,
  .UARTLCR_H    = UART_UARTLCR_H_WLEN_8BITS | UART_UARTLCR_H_FEN,
  .UARTCR       = 0U,
  .UARTIFLS     = UART_UARTIFLS_RXIFLSEL_1_2F | UART_UARTIFLS_TXIFLSEL_1_2E,
  .UARTDMACR    = 0U
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

__STATIC_INLINE void uart_enable_rx_irq(SIODriver *siop) {

#if SIO_USE_SYNCHRONIZATION == TRUE
  siop->uart->UARTIMSC |= UART_UARTIMSC_RXIM;
#else
  if (siop->operation->rx_cb != NULL) {
    siop->uart->UARTIMSC |= UART_UARTIMSC_RXIM;
  }
#endif
}

__STATIC_INLINE void uart_enable_rx_evt_irq(SIODriver *siop) {

#if SIO_USE_SYNCHRONIZATION == TRUE
  siop->uart->UARTIMSC |= UART_UARTIMSC_OEIM | UART_UARTIMSC_BEIM |
                          UART_UARTIMSC_PEIM | UART_UARTIMSC_FEIM;
#else
  if (siop->operation->rx_evt_cb != NULL) {
    siop->uart->UARTIMSC |= UART_UARTIMSC_OEIM | UART_UARTIMSC_BEIM |
                            UART_UARTIMSC_PEIM | UART_UARTIMSC_FEIM;
  }
#endif
}

__STATIC_INLINE void uart_enable_tx_irq(SIODriver *siop) {

#if SIO_USE_SYNCHRONIZATION == TRUE
  siop->uart->UARTIMSC |= UART_UARTIMSC_TXIM;
#else
  if (siop->operation->tx_cb != NULL) {
    siop->uart->UARTIMSC |= UART_UARTIMSC_TXIM;
  }
#endif
}

/**
 * @brief   UART initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] siop       pointer to a @p SIODriver object
 */
__STATIC_INLINE void uart_init(SIODriver *siop) {
  uint32_t div, idiv, fdiv;
  halfreq_t clock;

  clock = halClockGetPointX(clk_peri);

  osalDbgAssert(clock > 0U, "no clock");

  div = (8U * (uint32_t)clock) / siop->config->baud;
  idiv = div >> 7;
  fdiv = ((div & 0x7FU) + 1U) / 2U;

  osalDbgAssert((idiv > 0U) && (idiv <= 0xFFFFU), "invalid baud rate");

  siop->uart->UARTIBRD = idiv;
  siop->uart->UARTFBRD = fdiv;

  /* Registers settings, the LCR_H write also latches dividers values.*/
  siop->uart->UARTLCR_H = siop->config->UARTLCR_H & ~UART_LCRH_CFG_FORBIDDEN;
  siop->uart->UARTCR    = siop->config->UARTCR    & ~UART_CR_CFG_FORBIDDEN;
}

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
#if RP_SIO_USE_UART0 == TRUE
  sioObjectInit(&SIOD0);
  SIOD0.uart = UART0;
  hal_lld_peripheral_reset(RESETS_ALLREG_UART0);
#endif
#if RP_SIO_USE_UART1 == TRUE
  sioObjectInit(&SIOD1);
  SIOD1.uart = UART1;
  hal_lld_peripheral_reset(RESETS_ALLREG_UART1);
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

  /* Using the default configuration if the application passed a
     NULL pointer.*/
  if (siop->config == NULL) {
    siop->config = &default_config;
  }

  if (siop->state == SIO_STOP) {

  /* Enables the peripheral.*/
    if (false) {
    }
#if RP_SIO_USE_UART0 == TRUE
    else if (&SIOD0 == siop) {
      hal_lld_peripheral_unreset(RESETS_ALLREG_UART0);
      nvicEnableVector(RP_UART0_IRQ_NUMBER, RP_IRQ_UART0_PRIORITY);
    }
#endif
#if RP_SIO_USE_UART1 == TRUE
    else if (&SIOD1 == siop) {
      hal_lld_peripheral_unreset(RESETS_ALLREG_UART1);
      nvicEnableVector(RP_UART1_IRQ_NUMBER, RP_IRQ_UART1_PRIORITY);
    }
#endif
    else {
      osalDbgAssert(false, "invalid SIO instance");
    }

    /* Driver object low level initializations.*/
#if SIO_USE_SYNCHRONIZATION
    siop->sync_rx      = NULL;
    siop->sync_tx      = NULL;
    siop->sync_txend   = NULL;
#endif
  }

  /* Configures the peripheral.*/
  uart_init(siop);

  return false;
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
#if RP_SIO_USE_UART0 == TRUE
    else if (&SIOD0 == siop) {
      nvicDisableVector(RP_UART0_IRQ_NUMBER);
      hal_lld_peripheral_reset(RESETS_ALLREG_UART0);
    }
#endif
#if RP_SIO_USE_UART1 == TRUE
    else if (&SIOD1 == siop) {
      nvicDisableVector(RP_UART1_IRQ_NUMBER);
      hal_lld_peripheral_reset(RESETS_ALLREG_UART1);
    }
#endif
    else {
      osalDbgAssert(false, "invalid SIO instance");
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
  uint32_t imsc;

#if SIO_USE_SYNCHRONIZATION == TRUE
  /* With synchronization all interrupts are required.*/
  imsc = UART_UARTIMSC_OEIM | UART_UARTIMSC_BEIM |
         UART_UARTIMSC_PEIM | UART_UARTIMSC_FEIM |
         UART_UARTIMSC_RXIM | UART_UARTIMSC_RTIM |
         UART_UARTIMSC_TXIM;
#else
  /* When using just callbacks we can select only those really required.*/
  imsc = 0U;
  if (siop->operation->rx_cb != NULL) {
    imsc |= UART_UARTIMSC_RXIM;
  }
  if (siop->operation->rx_idle_cb != NULL) {
    imsc |= UART_UARTIMSC_RTIM;
  }
  if (siop->operation->tx_cb != NULL) {
    imsc |= UART_UARTIMSC_TXIM;
  }
  if (siop->operation->tx_end_cb != NULL) {
    osalDbgAssert(false, "unsupported callback");
  }
  if (siop->operation->rx_evt_cb != NULL) {
    imsc |= UART_UARTIMSC_OEIM | UART_UARTIMSC_BEIM |
            UART_UARTIMSC_PEIM | UART_UARTIMSC_FEIM;
  }
#endif

  /* Setting up the operation.*/
  siop->uart->UARTICR   = siop->uart->UARTRIS;
  siop->uart->UARTIMSC |= imsc;
  siop->uart->UARTCR    = siop->config->UARTCR |
                          UART_UARTCR_RXE | UART_UARTCR_TXE | UART_UARTCR_UARTEN;
}

/**
 * @brief   Stops an ongoing SIO operation, if any.
 *
 * @param[in] siop      pointer to an @p SIODriver structure
 *
 * @api
 */
void sio_lld_stop_operation(SIODriver *siop) {

  /* Stop operation.*/
  siop->uart->UARTIMSC = 0U;
  siop->uart->UARTCR   = siop->config->UARTCR & ~UART_CR_CFG_FORBIDDEN;
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
  uint32_t ris;

  /* Getting and clearing all relevant ISR flags (and only those).*/
  ris = siop->uart->UARTRIS & (UART_UARTRIS_OERIS | UART_UARTRIS_BERIS |
                               UART_UARTRIS_PERIS | UART_UARTRIS_FERIS);
  siop->uart->UARTICR = ris;

  /* Status flags cleared, now the related interrupts can be enabled again.*/
  uart_enable_rx_evt_irq(siop);

  /* Translating the status flags in SIO events.*/
  evtmask = 0U;
  if ((ris & UART_UARTRIS_BERIS) != 0U) {
    evtmask |= SIO_BREAK_DETECTED;
  }
  if ((ris & UART_UARTRIS_OERIS) != 0U) {
    evtmask |= SIO_OVERRUN_ERROR;
  }
  if ((ris & UART_UARTRIS_FERIS) != 0U) {
    evtmask |= SIO_FRAMING_ERROR;
  }
  if ((ris & UART_UARTRIS_PERIS) != 0U) {
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
      uart_enable_rx_irq(siop);
      break;
    }

    /* Buffer filled condition.*/
    if (rd > n) {
      break;
    }

    *buffer++ = (uint8_t)siop->uart->UARTDR;
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
      uart_enable_tx_irq(siop);
      break;
    }

    /* Buffer emptied condition.*/
    if (wr >= n) {
      break;
    }

    siop->uart->UARTDR = (uint32_t)*buffer++;
    wr++;
  }

  /* The transmit complete interrupt is always re-enabled on write.*/
  /* none */

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

  msg = (msg_t)(siop->uart->UARTDR & 0xFFU);

  /* If the RX FIFO has been emptied then the interrupt is enabled again.*/
  if (sio_lld_is_rx_empty(siop)) {
    uart_enable_rx_irq(siop);
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

  siop->uart->UARTDR = data;

  /* If the TX FIFO has been filled then the interrupt is enabled again.*/
  if (sio_lld_is_tx_full(siop)) {
    uart_enable_tx_irq(siop);
  }

  /* The transmit complete interrupt is always re-enabled on write.*/
  /* none */
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
  UART_TypeDef *u = siop->uart;
  uint32_t mis, imsc, evtmask;

  osalDbgAssert(siop->state == SIO_ACTIVE, "invalid state");

  /* Note, ISR flags are just read but not cleared, ISR sources are
     disabled instead.*/
  mis = u->UARTMIS;

  /* One read on control registers.*/
  imsc = u->UARTIMSC;

  /* Enabled errors/events handling.*/
  evtmask = mis & (UART_UARTMIS_OEMIS | UART_UARTMIS_BEMIS |
                   UART_UARTMIS_PEMIS | UART_UARTMIS_FEMIS);
  if (evtmask != 0U) {

    /* Disabling event sources.*/
    imsc &= ~(UART_UARTIMSC_OEIM | UART_UARTIMSC_BEIM |
             UART_UARTIMSC_PEIM | UART_UARTIMSC_FEIM);

    /* The callback is invoked if defined.*/
    __sio_callback_rx_evt(siop);

    /* Waiting thread woken, if any.*/
    __sio_wakeup_rx(siop, SIO_MSG_ERRORS);
  }

  /* RX FIFO is non-empty.*/
  if ((mis & UART_UARTMIS_RXMIS) != 0U) {

    /* Called once then the interrupt source is disabled.*/
    imsc &= ~UART_UARTIMSC_RXIM;

    /* The callback is invoked if defined.*/
    __sio_callback_rx(siop);

    /* Waiting thread woken, if any.*/
    __sio_wakeup_rx(siop, MSG_OK);
  }

  /* RX idle condition.*/
  if ((mis & UART_UARTMIS_RTMIS) != 0U) {

    /* The callback is invoked if defined.*/
    __sio_callback_rx_idle(siop);

    /* Waiting thread woken, if any.*/
    __sio_wakeup_rx(siop, SIO_MSG_IDLE);

    /* The idle flag requires clearing, it stays enabled.*/
    u->UARTICR = UART_UARTICR_RTIC;
  }

  /* TX FIFO is non-full.*/
  if ((mis & UART_UARTMIS_TXMIS) != 0U) {

    /* Called once then the interrupt source is disabled.*/
    imsc &= ~UART_UARTIMSC_TXIM;

    /* The callback is invoked if defined.*/
    __sio_callback_tx(siop);

    /* Waiting thread woken, if any.*/
    __sio_wakeup_tx(siop, MSG_OK);
  }

  /* One write on control registers.*/
  u->UARTIMSC = imsc;
}

#endif /* HAL_USE_SIO == TRUE */

/** @} */

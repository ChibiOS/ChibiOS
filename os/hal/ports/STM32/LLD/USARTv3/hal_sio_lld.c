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
 * @file    USARTv3/hal_sio_lld.c
 * @brief   STM32 SIO subsystem low level driver source.
 *
 * @addtogroup SIO
 * @{
 */

#include "hal.h"

#if (HAL_USE_SIO == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define USART_CR1_CFG_FORBIDDEN             (USART_CR1_RXFFIE           |   \
                                             USART_CR1_TXFEIE           |   \
                                             USART_CR1_FIFOEN           |   \
                                             USART_CR1_EOBIE            |   \
                                             USART_CR1_RTOIE            |   \
                                             USART_CR1_CMIE             |   \
                                             USART_CR1_PEIE             |   \
                                             USART_CR1_TXEIE_TXFNFIE    |   \
                                             USART_CR1_TCIE             |   \
                                             USART_CR1_RXNEIE_RXFNEIE   |   \
                                             USART_CR1_IDLEIE           |   \
                                             USART_CR1_TE               |   \
                                             USART_CR1_RE               |   \
                                             USART_CR1_UE)
#define USART_CR2_CFG_FORBIDDEN             (USART_CR2_LBDIE)
#define USART_CR3_CFG_FORBIDDEN             (USART_CR3_RXFTIE           |   \
                                             USART_CR3_TCBGTIE          |   \
                                             USART_CR3_TXFTIE           |   \
                                             USART_CR3_WUFIE            |   \
                                             USART_CR3_CTSIE            |   \
                                             USART_CR3_EIE)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   USART1 SIO driver identifier.
 */
#if (STM32_SIO_USE_USART1 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD1;
#endif

/**
 * @brief   USART2 SIO driver identifier.
 */
#if (STM32_SIO_USE_USART2 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD2;
#endif

/**
 * @brief   USART3 SIO driver identifier.
 */
#if (STM32_SIO_USE_USART3 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD3;
#endif

/**
 * @brief   UART4 SIO driver identifier.
 */
#if (STM32_SIO_USE_UART4 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD4;
#endif

/**
 * @brief   UART5 SIO driver identifier.
 */
#if (STM32_SIO_USE_UART5 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD5;
#endif

/**
 * @brief   USART6 SIO driver identifier.
 */
#if (STM32_SIO_USE_USART6 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD6;
#endif

/**
 * @brief   UART7 SIO driver identifier.
 */
#if (STM32_SIO_USE_UART7 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD7;
#endif

/**
 * @brief   UART8 SIO driver identifier.
 */
#if (STM32_SIO_USE_UART8 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD8;
#endif

/**
 * @brief   LPUART1 SIO driver identifier.
 */
#if (STM32_SIO_USE_LPUART1 == TRUE) || defined(__DOXYGEN__)
SIODriver LPSIOD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 * @note    In this implementation it is: 38400-8-N-1, RX and TX FIFO
 *          thresholds set to 50%.
 */
static const SIOConfig default_config = {
  .baud  = SIO_DEFAULT_BITRATE,
  .presc = USART_PRESC1,
  .cr1   = USART_CR1_DATA8 | USART_CR1_OVER16,
  .cr2   = USART_CR2_STOP1_BITS,
  .cr3   = USART_CR3_TXFTCFG_1H | USART_CR3_RXFTCFG_1H
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

__STATIC_INLINE void usart_enable_rx_irq(SIODriver *siop) {

#if HAL_SIO_USE_SYNCHRONIZATION == TRUE
  siop->usart->CR3 |= USART_CR3_RXFTIE;
#else
  if (siop->operation->rx_cb != NULL) {
    siop->usart->CR3 |= USART_CR3_RXFTIE;
  }
#endif
}

__STATIC_INLINE void usart_enable_rx_evt_irq(SIODriver *siop) {

#if HAL_SIO_USE_SYNCHRONIZATION == TRUE
  siop->usart->CR1 |= USART_CR1_PEIE;
  siop->usart->CR2 |= USART_CR2_LBDIE;
  siop->usart->CR3 |= USART_CR3_EIE;
#else
  if (siop->operation->rx_evt_cb != NULL) {
    siop->usart->CR1 |= USART_CR1_PEIE;
    siop->usart->CR2 |= USART_CR2_LBDIE;
    siop->usart->CR3 |= USART_CR3_EIE;
  }
#endif
}

__STATIC_INLINE void usart_enable_tx_irq(SIODriver *siop) {

#if HAL_SIO_USE_SYNCHRONIZATION == TRUE
  siop->usart->CR3 |= USART_CR3_TXFTIE;
#else
  if (siop->operation->tx_cb != NULL) {
    siop->usart->CR3 |= USART_CR3_TXFTIE;
  }
#endif
}

__STATIC_INLINE void usart_enable_tx_end_irq(SIODriver *siop) {

#if HAL_SIO_USE_SYNCHRONIZATION == TRUE
  siop->usart->CR1 |= USART_CR1_TCIE;
#else
  if (siop->operation->tx_end_cb != NULL) {
    siop->usart->CR1 |= USART_CR1_TCIE;
  }
#endif
}

/**
 * @brief   USART initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] siop       pointer to a @p SIODriver object
 */
__STATIC_INLINE void usart_init(SIODriver *siop) {
  USART_TypeDef *u = siop->usart;
  uint32_t presc, brr;

  /* Prescaler calculation.*/
  static const uint32_t prescvals[] = {1, 2, 4, 6, 8, 10, 12, 16, 32, 64, 128, 256};
  presc = prescvals[siop->config->presc];

 /* Baud rate setting.*/
#if STM32_SIO_USE_LPUART1
  if (siop == &LPSIOD1) {
    osalDbgAssert((siop->clock >= siop->config->baud * 3U) &&
                  (siop->clock <= siop->config->baud * 4096U),
                  "invalid baud rate vs input clock");

    brr = (uint32_t)(((uint64_t)(siop->clock / presc) * (uint64_t)256) / siop->config->baud);

    osalDbgAssert((brr >= 0x300) && (brr < 0x100000), "invalid BRR value");
  }
 else
#endif
  {
    brr = (uint32_t)((siop->clock / presc) / siop->config->baud);

    /* Correcting BRR value when oversampling by 8 instead of 16.
       Fraction is still 4 bits wide, but only lower 3 bits used.
       Mantissa is doubled, but Fraction is left the same.*/
    if ((siop->config->cr1 & USART_CR1_OVER8) != 0U) {
      brr = ((brr & ~7U) * 2U) | (brr & 7U);
    }

    osalDbgAssert(brr < 0x10000, "invalid BRR value");
  }

  /* Setting up USART.*/
  u->PRESC = siop->config->presc;
  u->BRR   = brr;
  u->CR1   = (siop->config->cr1 & ~USART_CR1_CFG_FORBIDDEN) | USART_CR1_FIFOEN;
  u->CR2   = siop->config->cr2 & ~USART_CR2_CFG_FORBIDDEN;
  u->CR3   = siop->config->cr3 & ~USART_CR3_CFG_FORBIDDEN;
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
#if STM32_SIO_USE_USART1 == TRUE
  sioObjectInit(&SIOD1);
  SIOD1.usart = USART1;
  SIOD1.clock = STM32_USART1CLK;
#endif
#if STM32_SIO_USE_USART2 == TRUE
  sioObjectInit(&SIOD2);
  SIOD2.usart = USART2;
  SIOD2.clock = STM32_USART2CLK;
#endif
#if STM32_SIO_USE_USART3 == TRUE
  sioObjectInit(&SIOD3);
  SIOD3.usart = USART3;
  SIOD3.clock = STM32_USART3CLK;
#endif
#if STM32_SIO_USE_UART4 == TRUE
  sioObjectInit(&SIOD4);
  SIOD4.usart = UART4;
  SIOD4.clock = STM32_UART4CLK;
#endif
#if STM32_SIO_USE_UART5 == TRUE
  sioObjectInit(&SIOD5);
  SIOD5.usart = UART5;
  SIOD5.clock = STM32_UART5CLK;
#endif
#if STM32_SIO_USE_USART6 == TRUE
  sioObjectInit(&SIOD6);
  SIOD6.usart = USART6;
  SIOD6.clock = STM32_USART6CLK;
#endif
#if STM32_SIO_USE_UART7 == TRUE
  sioObjectInit(&SIOD7);
  SIOD7.usart = UART7;
  SIOD7.clock = STM32_UART7CLK;
#endif
#if STM32_SIO_USE_UART8 == TRUE
  sioObjectInit(&SIOD8);
  SIOD8.usart = UART8;
  SIOD8.clock = STM32_UART8CLK;
#endif
#if STM32_SIO_USE_LPUART1 == TRUE
  sioObjectInit(&LPSIOD1);
  LPSIOD1.usart = LPUART1;
  LPSIOD1.clock = STM32_LPUART1CLK;
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
#if STM32_SIO_USE_USART1 == TRUE
    else if (&SIOD1 == siop) {
      rccResetUSART1();
      rccEnableUSART1(true);
    }
#endif
#if STM32_SIO_USE_USART2 == TRUE
    else if (&SIOD2 == siop) {
      rccResetUSART2();
      rccEnableUSART2(true);
    }
#endif
#if STM32_SIO_USE_USART3 == TRUE
    else if (&SIOD3 == siop) {
      rccResetUSART3();
      rccEnableUSART3(true);
    }
#endif
#if STM32_SIO_USE_UART4 == TRUE
    else if (&SIOD4 == siop) {
      rccResetUART4();
      rccEnableUART4(true);
    }
#endif
#if STM32_SIO_USE_UART5 == TRUE
    else if (&SIOD5 == siop) {
      rccResetUART5();
      rccEnableUART5(true);
    }
#endif
#if STM32_SIO_USE_USART6 == TRUE
    else if (&SIOD6 == siop) {
      rccResetUSART6();
      rccEnableUSART6(true);
    }
#endif
#if STM32_SIO_USE_UART7 == TRUE
    else if (&SIOD7 == siop) {
      rccResetUART7();
      rccEnableUART7(true);
    }
#endif
#if STM32_SIO_USE_UART8 == TRUE
    else if (&SIOD8 == siop) {
      rccResetUART8();
      rccEnableUART8(true);
    }
#endif
#if STM32_SIO_USE_LPUART1 == TRUE
    else if (&LPSIOD1 == siop) {
      rccResetLPUART1();
      rccEnableLPUART1(true);
    }
#endif
    else {
      osalDbgAssert(false, "invalid USART instance");
    }

    /* Driver object low level initializations.*/
#if HAL_SIO_USE_SYNCHRONIZATION
    siop->sync_rx      = NULL;
    siop->sync_tx      = NULL;
    siop->sync_txend   = NULL;
//    siop->events       = 0U;
#endif
  }

  /* Configures the peripheral.*/
  usart_init(siop);

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
#if STM32_SIO_USE_USART1 == TRUE
    else if (&SIOD1 == siop) {
     rccResetUSART1();
     rccDisableUSART1();
    }
#endif
#if STM32_SIO_USE_USART2 == TRUE
    else if (&SIOD2 == siop) {
      rccResetUSART2();
      rccDisableUSART2();
    }
#endif
#if STM32_SIO_USE_USART3 == TRUE
    else if (&SIOD3 == siop) {
      rccResetUSART3();
      rccDisableUSART3();
    }
#endif
#if STM32_SIO_USE_UART4 == TRUE
    else if (&SIOD4 == siop) {
      rccResetUART4();
      rccDisableUART4();
    }
#endif
#if STM32_SIO_USE_UART5 == TRUE
    else if (&SIOD5 == siop) {
      rccResetUART5();
      rccDisableUART5();
    }
#endif
#if STM32_SIO_USE_USART6 == TRUE
    else if (&SIOD6 == siop) {
      rccResetUSART6();
      rccDisableUSART6();
    }
#endif
#if STM32_SIO_USE_UART7 == TRUE
    else if (&SIOD7 == siop) {
      rccResetUART7();
      rccDisableUART7();
    }
#endif
#if STM32_SIO_USE_UART8 == TRUE
    else if (&SIOD8 == siop) {
      rccResetUART8();
      rccDisableUART8();
    }
#endif
#if STM32_SIO_USE_LPUART1 == TRUE
    else if (&LPSIOD1 == siop) {
      rccResetLPUART1();
      rccDisableLPUART1();
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
  uint32_t cr1irq, cr2irq, cr3irq;

#if HAL_SIO_USE_SYNCHRONIZATION == TRUE
  /* With synchronization all interrupts are required.*/
  cr1irq  = USART_CR1_PEIE   | USART_CR1_TCIE   | USART_CR1_IDLEIE;
  cr2irq  = USART_CR2_LBDIE;
  cr3irq  = USART_CR3_RXFTIE | USART_CR3_TXFTIE | USART_CR3_EIE;
#else
  /* When using just callbacks we can select only those really required.*/
  cr1irq  = 0U;
  cr2irq  = 0U;
  cr3irq  = 0U;
  if (siop->operation->rx_cb != NULL) {
    cr3irq |= USART_CR3_RXFTIE;
  }
  if (siop->operation->rx_idle_cb != NULL) {
    cr1irq |= USART_CR1_IDLEIE;
  }
  if (siop->operation->tx_cb != NULL) {
    cr3irq |= USART_CR3_TXFTIE;
  }
  if (siop->operation->tx_end_cb != NULL) {
    cr1irq |= USART_CR1_TCIE;
  }
  if (siop->operation->rx_evt_cb != NULL) {
    cr1irq |= USART_CR1_PEIE;
    cr2irq |= USART_CR2_LBDIE;
    cr3irq |= USART_CR3_EIE;
  }
#endif

  /* Setting up the operation.*/
  siop->usart->ICR  = siop->usart->ISR;
  siop->usart->CR2 |= cr2irq;
  siop->usart->CR3 |= cr3irq;
  siop->usart->CR1 |= cr1irq | USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
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
  siop->usart->CR1 &= USART_CR1_CFG_FORBIDDEN;
  siop->usart->CR2 &= USART_CR2_CFG_FORBIDDEN;
  siop->usart->CR3 &= USART_CR3_CFG_FORBIDDEN;
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
    if (rd > n) {
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
  uint32_t isr, cr1, cr3, evtmask;

  osalDbgAssert(siop->state == SIO_ACTIVE, "invalid state");

  /* Note, ISR flags are just read but not cleared, ISR sources are
     disabled instead.*/
  isr = u->ISR;

  /* One read on control registers.*/
  cr1 = u->CR1;
  cr3 = u->CR3;

  /* Enabled errors/events handling.*/
  evtmask = isr & (USART_ISR_PE  | USART_ISR_LBDF | USART_ISR_FE    |
                   USART_ISR_ORE | USART_ISR_NE);
  if (evtmask != 0U) {
    uint32_t cr2;

    /* One read on control registers.*/
    cr2 = u->CR2;

    /* The callback is invoked if defined.*/
    __sio_callback_rx_evt(siop);

    /* Waiting thread woken, if any.*/
    __sio_wakeup_rx(siop, SIO_MSG_ERRORS);

    /* Disabling event sources until errors are recognized by the
       application.*/
    cr1 &= ~USART_CR1_PEIE;
    cr2 &= ~USART_CR2_LBDIE;
    cr3 &= ~USART_CR3_EIE;

    /* One write on control registers.*/
    u->CR2 = cr2;
  }

  /* RX FIFO is non-empty.*/
  if (((cr3 & USART_CR3_RXFTIE) != 0U) &&
      (isr & USART_ISR_RXFT) != 0U) {

    /* The callback is invoked if defined.*/
    __sio_callback_rx(siop);

    /* Waiting thread woken, if any.*/
    __sio_wakeup_rx(siop, MSG_OK);

    /* Called once then the interrupt source is disabled.*/
    cr3 &= ~USART_CR3_RXFTIE;
  }

  /* RX idle condition.*/
  if (((cr1 & USART_CR1_IDLEIE) != 0U) &&
      (isr & USART_ISR_IDLE) != 0U) {

    /* The callback is invoked if defined.*/
    __sio_callback_rx_idle(siop);

    /* Waiting thread woken, if any.*/
    __sio_wakeup_rx(siop, SIO_MSG_IDLE);

    /* The idle flag requires clearing, it stays enabled.*/
    u->ICR = USART_ISR_IDLE;
  }

  /* TX FIFO is non-full.*/
  if (((cr3 & USART_CR3_TXFTIE) != 0U) &&
      (isr & USART_ISR_TXFT) != 0U) {

    /* The callback is invoked if defined.*/
    __sio_callback_tx(siop);

    /* Waiting thread woken, if any.*/
    __sio_wakeup_tx(siop, MSG_OK);

    /* Called once then the interrupt is disabled.*/
    cr3 &= ~USART_CR3_TXFTIE;
  }

  /* Physical transmission end.*/
  if (((cr1 & USART_CR1_TCIE) != 0U) &&
      (isr & USART_ISR_TC) != 0U) {

    /* The callback is invoked if defined.*/
    __sio_callback_tx_end(siop);

    /* Waiting thread woken, if any.*/
    __sio_wakeup_txend(siop, MSG_OK);

    /* Called once then the interrupt is disabled.*/
    cr1 &= ~USART_CR1_TCIE;
  }

  /* One write on control registers.*/
  u->CR1 = cr1;
  u->CR3 = cr3;
}

#endif /* HAL_USE_SIO == TRUE */

/** @} */

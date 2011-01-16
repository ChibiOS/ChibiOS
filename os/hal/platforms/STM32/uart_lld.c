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
 * @file    STM32/uart_lld.c
 * @brief   STM32 low level UART driver code.
 *
 * @addtogroup UART
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_UART || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief USART1 UART driver identifier.*/
#if STM32_UART_USE_USART1 || defined(__DOXYGEN__)
UARTDriver UARTD1;
#endif

/** @brief USART2 UART driver identifier.*/
#if STM32_UART_USE_USART2 || defined(__DOXYGEN__)
UARTDriver UARTD2;
#endif

/** @brief USART3 UART driver identifier.*/
#if STM32_UART_USE_USART3 || defined(__DOXYGEN__)
UARTDriver UARTD3;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Status bits translation.
 *
 * @param[in] sr        USART SR register value
 *
 * @return  The error flags.
 */
static uartflags_t translate_errors(uint16_t sr) {
  uartflags_t sts = 0;

  if (sr & USART_SR_ORE)
    sts |= UART_OVERRUN_ERROR;
  if (sr & USART_SR_PE)
    sts |= UART_PARITY_ERROR;
  if (sr & USART_SR_FE)
    sts |= UART_FRAMING_ERROR;
  if (sr & USART_SR_NE)
    sts |= UART_NOISE_ERROR;
  if (sr & USART_SR_LBD)
    sts |= UART_BREAK_DETECTED;
  return sts;
}

/**
 * @brief   Puts the receiver in the UART_RX_IDLE state.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
static void set_rx_idle_loop(UARTDriver *uartp) {
  uint32_t ccr;
  
  /* RX DMA channel preparation, if the char callback is defined then the
     TCIE interrupt is enabled too.*/
  if (uartp->ud_config->uc_rxchar == NULL)
    ccr = DMA_CCR1_CIRC | DMA_CCR1_TEIE;
  else
    ccr = DMA_CCR1_CIRC | DMA_CCR1_TEIE | DMA_CCR1_TCIE;
  dmaSetupChannel(uartp->ud_dmap, uartp->ud_dmarx, 1,
                  &uartp->ud_rxbuf, uartp->ud_dmaccr | ccr);
  dmaEnableChannel(uartp->ud_dmap, uartp->ud_dmarx);
}

/**
 * @brief   USART de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
static void usart_stop(UARTDriver *uartp) {

  /* Stops RX and TX DMA channels.*/
  dmaDisableChannel(uartp->ud_dmap, uartp->ud_dmarx);
  dmaDisableChannel(uartp->ud_dmap, uartp->ud_dmatx);
  dmaClearChannel(uartp->ud_dmap, uartp->ud_dmarx);
  dmaClearChannel(uartp->ud_dmap, uartp->ud_dmatx);
  
  /* Stops USART operations.*/
  uartp->ud_usart->CR1 = 0;
  uartp->ud_usart->CR2 = 0;
  uartp->ud_usart->CR3 = 0;
}

/**
 * @brief   USART initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
static void usart_start(UARTDriver *uartp) {
  uint16_t cr1;
  USART_TypeDef *u = uartp->ud_usart;

  /* Defensive programming, starting from a clean state.*/
  usart_stop(uartp);

  /* Baud rate setting.*/
  if (uartp->ud_usart == USART1)
    u->BRR = STM32_PCLK2 / uartp->ud_config->uc_speed;
  else
    u->BRR = STM32_PCLK1 / uartp->ud_config->uc_speed;

  /* Resetting eventual pending status flags.*/
  (void)u->SR;  /* SR reset step 1.*/
  (void)u->DR;  /* SR reset step 2.*/
  u->SR = 0;

  /* Note that some bits are enforced because required for correct driver
     operations.*/
  if (uartp->ud_config->uc_txend2 == NULL)
    cr1 = USART_CR1_UE | USART_CR1_PEIE | USART_CR1_TE | USART_CR1_RE;
  else
    cr1 = USART_CR1_UE | USART_CR1_PEIE | USART_CR1_TE | USART_CR1_RE |
          USART_CR1_TCIE;
  u->CR1 = uartp->ud_config->uc_cr1 | cr1;
  u->CR2 = uartp->ud_config->uc_cr2 | USART_CR2_LBDIE;
  u->CR3 = uartp->ud_config->uc_cr3 | USART_CR3_DMAT | USART_CR3_DMAR |
                                      USART_CR3_EIE;

  /* Starting the receiver idle loop.*/
  set_rx_idle_loop(uartp);
}

/**
 * @brief   RX DMA common service routine.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
static void serve_rx_end_irq(UARTDriver *uartp) {

  uartp->ud_rxstate = UART_RX_COMPLETE;
  if (uartp->ud_config->uc_rxend != NULL)
    uartp->ud_config->uc_rxend(uartp);
  /* If the callback didn't explicitly change state then the receiver
     automatically returns to the idle state.*/
  if (uartp->ud_rxstate == UART_RX_COMPLETE) {
    uartp->ud_rxstate = UART_RX_IDLE;
    set_rx_idle_loop(uartp);
  }
}

/**
 * @brief   TX DMA common service routine.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
static void serve_tx_end_irq(UARTDriver *uartp) {

  /* A callback is generated, if enabled, after a completed transfer.*/
  uartp->ud_txstate = UART_TX_COMPLETE;
  if (uartp->ud_config->uc_txend1 != NULL)
    uartp->ud_config->uc_txend1(uartp);
  /* If the callback didn't explicitly change state then the transmitter
     automatically returns to the idle state.*/
  if (uartp->ud_txstate == UART_TX_COMPLETE)
    uartp->ud_txstate = UART_TX_IDLE;
}
/**
 * @brief   USART common service routine.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
static void serve_usart_irq(UARTDriver *uartp) {
  uint16_t sr;
  USART_TypeDef *u = uartp->ud_usart;
  
  sr = u->SR;   /* SR reset step 1.*/
  (void)u->DR;  /* SR reset step 2.*/
  if (sr & (USART_SR_LBD | USART_SR_ORE | USART_SR_NE |
            USART_SR_FE  | USART_SR_PE)) {
    u->SR = ~USART_SR_LBD;
    if (uartp->ud_config->uc_rxerr != NULL)
      uartp->ud_config->uc_rxerr(uartp, translate_errors(sr));
  }
  if (sr & USART_SR_TC) {
    u->SR = ~USART_SR_TC;
    /* End of transmission, a callback is generated.*/
    if (uartp->ud_config->uc_txend2 != NULL)
      uartp->ud_config->uc_txend2(uartp);
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_UART_USE_USART1 || defined(__DOXYGEN__)
/**
 * @brief   USART1 RX DMA interrupt handler (channel 5).
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch5_IRQHandler) {
  UARTDriver *uartp;

  CH_IRQ_PROLOGUE();

  uartp = &UARTD1;
  if ((STM32_DMA1->ISR & DMA_ISR_TEIF5) != 0) {
    STM32_UART_USART1_DMA_ERROR_HOOK();
  }
  if (uartp->ud_rxstate == UART_RX_IDLE) {
    dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_5);
    /* Fast IRQ path, this is why it is not centralized in serve_rx_end_irq().*/
    /* Receiver in idle state, a callback is generated, if enabled, for each
       received character and then the driver stays in the same state.*/
    if (uartp->ud_config->uc_rxchar != NULL)
      uartp->ud_config->uc_rxchar(uartp, uartp->ud_rxbuf);
  }
  else {
    /* Receiver in active state, a callback is generated, if enabled, after
       a completed transfer.*/
    dmaDisableChannel(STM32_DMA1, STM32_DMA_CHANNEL_5);
    dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_5);
    serve_rx_end_irq(uartp);
  }

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   USART1 TX DMA interrupt handler (channel 4).
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch4_IRQHandler) {

  CH_IRQ_PROLOGUE();

  if ((STM32_DMA1->ISR & DMA_ISR_TEIF4) != 0) {
    STM32_UART_USART1_DMA_ERROR_HOOK();
  }
  dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_4);
  dmaDisableChannel(STM32_DMA1, STM32_DMA_CHANNEL_4);
  serve_tx_end_irq(&UARTD1);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   USART1 IRQ handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USART1_IRQHandler) {

  CH_IRQ_PROLOGUE();

  serve_usart_irq(&UARTD1);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_UART_USE_USART1 */

#if STM32_UART_USE_USART2 || defined(__DOXYGEN__)
/**
 * @brief   USART2 RX DMA interrupt handler (channel 6).
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch6_IRQHandler) {
  UARTDriver *uartp;

  CH_IRQ_PROLOGUE();

  uartp = &UARTD2;
  if ((STM32_DMA1->ISR & DMA_ISR_TEIF6) != 0) {
    STM32_UART_USART2_DMA_ERROR_HOOK();
  }
  if (uartp->ud_rxstate == UART_RX_IDLE) {
    dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_6);
    /* Fast IRQ path, this is why it is not centralized in serve_rx_end_irq().*/
    /* Receiver in idle state, a callback is generated, if enabled, for each
       received character and then the driver stays in the same state.*/
    if (uartp->ud_config->uc_rxchar != NULL)
      uartp->ud_config->uc_rxchar(uartp, uartp->ud_rxbuf);
  }
  else {
    /* Receiver in active state, a callback is generated, if enabled, after
       a completed transfer.*/
    dmaDisableChannel(STM32_DMA1, STM32_DMA_CHANNEL_6);
    dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_6);
    serve_rx_end_irq(uartp);
  }

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   USART2 TX DMA interrupt handler (channel 7).
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch7_IRQHandler) {

  CH_IRQ_PROLOGUE();

  if ((STM32_DMA1->ISR & DMA_ISR_TEIF7) != 0) {
    STM32_UART_USART2_DMA_ERROR_HOOK();
  }
  dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_7);
  dmaDisableChannel(STM32_DMA1, STM32_DMA_CHANNEL_7);
  serve_tx_end_irq(&UARTD2);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   USART2 IRQ handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USART2_IRQHandler) {

  CH_IRQ_PROLOGUE();

  serve_usart_irq(&UARTD2);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_UART_USE_USART2 */

#if STM32_UART_USE_USART3 || defined(__DOXYGEN__)
/**
 * @brief   USART3 RX DMA interrupt handler (channel 3).
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch3_IRQHandler) {
  UARTDriver *uartp;

  CH_IRQ_PROLOGUE();

  uartp = &UARTD3;
  if ((STM32_DMA1->ISR & DMA_ISR_TEIF3) != 0) {
    STM32_UART_USART1_DMA_ERROR_HOOK();
  }
  if (uartp->ud_rxstate == UART_RX_IDLE) {
    dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_3);
    /* Fast IRQ path, this is why it is not centralized in serve_rx_end_irq().*/
    /* Receiver in idle state, a callback is generated, if enabled, for each
       received character and then the driver stays in the same state.*/
    if (uartp->ud_config->uc_rxchar != NULL)
      uartp->ud_config->uc_rxchar(uartp, uartp->ud_rxbuf);
  }
  else {
    /* Receiver in active state, a callback is generated, if enabled, after
       a completed transfer.*/
    dmaDisableChannel(STM32_DMA1, STM32_DMA_CHANNEL_3);
    dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_3);
    serve_rx_end_irq(uartp);
  }

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   USART3 TX DMA interrupt handler (channel 2).
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch2_IRQHandler) {

  CH_IRQ_PROLOGUE();

  if ((STM32_DMA1->ISR & DMA_ISR_TEIF2) != 0) {
    STM32_UART_USART1_DMA_ERROR_HOOK();
  }
  dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_2);
  dmaDisableChannel(STM32_DMA1, STM32_DMA_CHANNEL_2);
  serve_tx_end_irq(&UARTD3);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   USART3 IRQ handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USART3_IRQHandler) {

  CH_IRQ_PROLOGUE();

  serve_usart_irq(&UARTD3);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_UART_USE_USART3 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level UART driver initialization.
 *
 * @notapi
 */
void uart_lld_init(void) {

#if STM32_UART_USE_USART1
  RCC->APB2RSTR     = RCC_APB2RSTR_USART1RST;
  RCC->APB2RSTR     = 0;
  uartObjectInit(&UARTD1);
  UARTD1.ud_usart   = USART1;
  UARTD1.ud_dmap    = STM32_DMA1;
  UARTD1.ud_dmarx   = STM32_DMA_CHANNEL_5;
  UARTD1.ud_dmatx   = STM32_DMA_CHANNEL_4;
  UARTD1.ud_dmaccr  = 0;
#endif

#if STM32_UART_USE_USART2
  RCC->APB1RSTR     = RCC_APB1RSTR_USART2RST;
  RCC->APB1RSTR     = 0;
  uartObjectInit(&UARTD2);
  UARTD2.ud_usart   = USART2;
  UARTD2.ud_dmap    = STM32_DMA1;
  UARTD2.ud_dmarx   = STM32_DMA_CHANNEL_6;
  UARTD2.ud_dmatx   = STM32_DMA_CHANNEL_7;
  UARTD2.ud_dmaccr  = 0;
#endif

#if STM32_UART_USE_USART3
  RCC->APB1RSTR     = RCC_APB1RSTR_USART3RST;
  RCC->APB1RSTR     = 0;
  uartObjectInit(&UARTD3);
  UARTD3.ud_usart   = USART3;
  UARTD3.ud_dmap    = STM32_DMA1;
  UARTD3.ud_dmarx   = STM32_DMA_CHANNEL_3;
  UARTD3.ud_dmatx   = STM32_DMA_CHANNEL_2;
  UARTD3.ud_dmaccr  = 0;
#endif
}

/**
 * @brief   Configures and activates the UART peripheral.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 *
 * @notapi
 */
void uart_lld_start(UARTDriver *uartp) {

  if (uartp->ud_state == UART_STOP) {
#if STM32_UART_USE_USART1
    if (&UARTD1 == uartp) {
      dmaEnable(DMA1_ID);   /* NOTE: Must be enabled before the IRQs.*/
      NVICEnableVector(USART1_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_UART_USART1_IRQ_PRIORITY));
      NVICEnableVector(DMA1_Channel4_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_UART_USART1_IRQ_PRIORITY));
      NVICEnableVector(DMA1_Channel5_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_UART_USART1_IRQ_PRIORITY));
      RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    }
#endif

#if STM32_UART_USE_USART2
    if (&UARTD2 == uartp) {
      dmaEnable(DMA1_ID);   /* NOTE: Must be enabled before the IRQs.*/
      NVICEnableVector(USART2_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_UART_USART2_IRQ_PRIORITY));
      NVICEnableVector(DMA1_Channel6_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_UART_USART2_IRQ_PRIORITY));
      NVICEnableVector(DMA1_Channel7_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_UART_USART2_IRQ_PRIORITY));
      RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    }
#endif

#if STM32_UART_USE_USART3
    if (&UARTD3 == uartp) {
      dmaEnable(DMA1_ID);   /* NOTE: Must be enabled before the IRQs.*/
      NVICEnableVector(USART3_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_UART_USART3_IRQ_PRIORITY));
      NVICEnableVector(DMA1_Channel2_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_UART_USART3_IRQ_PRIORITY));
      NVICEnableVector(DMA1_Channel3_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_UART_USART3_IRQ_PRIORITY));
      RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    }
#endif

    /* Static DMA setup, the transfer size depends on the USART settings,
       it is 16 bits if M=1 and PCE=0 else it is 8 bits.*/
    uartp->ud_dmaccr = STM32_UART_USART1_DMA_PRIORITY << 12;
    if ((uartp->ud_config->uc_cr1 & (USART_CR1_M | USART_CR1_PCE)) == USART_CR1_M)
      uartp->ud_dmaccr |= DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_0;
    dmaChannelSetPeripheral(&uartp->ud_dmap->channels[uartp->ud_dmarx],
                            &uartp->ud_usart->DR);
    dmaChannelSetPeripheral(&uartp->ud_dmap->channels[uartp->ud_dmatx],
                            &uartp->ud_usart->DR);
    uartp->ud_rxbuf = 0;
  }

  uartp->ud_rxstate = UART_RX_IDLE;
  uartp->ud_txstate = UART_TX_IDLE;
  usart_start(uartp);
}

/**
 * @brief   Deactivates the UART peripheral.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 *
 * @notapi
 */
void uart_lld_stop(UARTDriver *uartp) {

  if (uartp->ud_state == UART_READY) {
    usart_stop(uartp);

#if STM32_UART_USE_USART1
    if (&UARTD1 == uartp) {
      NVICDisableVector(USART1_IRQn);
      NVICDisableVector(DMA1_Channel4_IRQn);
      NVICDisableVector(DMA1_Channel5_IRQn);
      dmaDisable(DMA1_ID);
      RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
      return;
    }
#endif

#if STM32_UART_USE_USART2
    if (&UARTD2 == uartp) {
      NVICDisableVector(USART2_IRQn);
      NVICDisableVector(DMA1_Channel6_IRQn);
      NVICDisableVector(DMA1_Channel7_IRQn);
      dmaDisable(DMA1_ID);
      RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN;
      return;
    }
#endif

#if STM32_UART_USE_USART3
    if (&UARTD3 == uartp) {
      NVICDisableVector(USART3_IRQn);
      NVICDisableVector(DMA1_Channel2_IRQn);
      NVICDisableVector(DMA1_Channel3_IRQn);
      dmaDisable(DMA1_ID);
      RCC->APB1ENR &= ~RCC_APB1ENR_USART3EN;
      return;
    }
#endif
  }
}

/**
 * @brief   Starts a transmission on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void uart_lld_start_send(UARTDriver *uartp, size_t n, const void *txbuf) {

  /* TX DMA channel preparation and start.*/
  dmaSetupChannel(uartp->ud_dmap, uartp->ud_dmatx, n, txbuf,
                  uartp->ud_dmaccr | DMA_CCR1_DIR | DMA_CCR1_MINC |
                  DMA_CCR1_TEIE | DMA_CCR1_TCIE);
  dmaEnableChannel(uartp->ud_dmap, uartp->ud_dmatx);
}

/**
 * @brief   Stops any ongoing transmission.
 * @note    Stopping a transmission also suppresses the transmission callbacks.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 *
 * @return              The number of data frames not transmitted by the
 *                      stopped transmit operation.
 *
 * @notapi
 */
size_t uart_lld_stop_send(UARTDriver *uartp) {

  dmaDisableChannel(uartp->ud_dmap, uartp->ud_dmatx);
  dmaClearChannel(uartp->ud_dmap, uartp->ud_dmatx);
  return (size_t)uartp->ud_dmap->channels[uartp->ud_dmatx].CNDTR;
}

/**
 * @brief   Starts a receive operation on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void uart_lld_start_receive(UARTDriver *uartp, size_t n, void *rxbuf) {

  /* Stopping previous activity (idle state).*/
  dmaDisableChannel(uartp->ud_dmap, uartp->ud_dmarx);
  dmaClearChannel(uartp->ud_dmap, uartp->ud_dmarx);

  /* RX DMA channel preparation and start.*/
  dmaSetupChannel(uartp->ud_dmap, uartp->ud_dmarx, n, rxbuf,
                  uartp->ud_dmaccr | DMA_CCR1_MINC |
                  DMA_CCR1_TEIE | DMA_CCR1_TCIE);
  dmaEnableChannel(uartp->ud_dmap, uartp->ud_dmarx);
}

/**
 * @brief   Stops any ongoing receive operation.
 * @note    Stopping a receive operation also suppresses the receive callbacks.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 *
 * @return              The number of data frames not received by the
 *                      stopped receive operation.
 *
 * @notapi
 */
size_t uart_lld_stop_receive(UARTDriver *uartp) {
  size_t n;

  dmaDisableChannel(uartp->ud_dmap, uartp->ud_dmarx);
  dmaClearChannel(uartp->ud_dmap, uartp->ud_dmarx);
  n = (size_t)uartp->ud_dmap->channels[uartp->ud_dmarx].CNDTR;
  set_rx_idle_loop(uartp);
  return n;
}

#endif /* HAL_USE_UART */

/** @} */

/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    STM32/serial_lld.c
 * @brief   STM32 low level serial driver code.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief USART1 serial driver identifier.*/
#if STM32_SERIAL_USE_USART1 || defined(__DOXYGEN__)
SerialDriver SD1;
#endif

/** @brief USART2 serial driver identifier.*/
#if STM32_SERIAL_USE_USART2 || defined(__DOXYGEN__)
SerialDriver SD2;
#endif

/** @brief USART3 serial driver identifier.*/
#if STM32_SERIAL_USE_USART3 || defined(__DOXYGEN__)
SerialDriver SD3;
#endif

/** @brief UART4 serial driver identifier.*/
#if STM32_SERIAL_USE_UART4 || defined(__DOXYGEN__)
SerialDriver SD4;
#endif

/** @brief UART5 serial driver identifier.*/
#if STM32_SERIAL_USE_UART5 || defined(__DOXYGEN__)
SerialDriver SD5;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/** @brief Driver default configuration.*/
static const SerialConfig default_config =
{
  SERIAL_DEFAULT_BITRATE,
  0,
  USART_CR2_STOP1_BITS | USART_CR2_LINEN,
  0
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   USART initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void usart_init(SerialDriver *sdp, const SerialConfig *config) {
  USART_TypeDef *u = sdp->usart;

  /*
   * Baud rate setting.
   */
  if (sdp->usart == USART1)
    u->BRR = STM32_PCLK2 / config->sc_speed;
  else
    u->BRR = STM32_PCLK1 / config->sc_speed;

  /*
   * Note that some bits are enforced.
   */
  u->CR1 = config->sc_cr1 | USART_CR1_UE | USART_CR1_PEIE |
                            USART_CR1_RXNEIE | USART_CR1_TE |
                            USART_CR1_RE;
  u->CR2 = config->sc_cr2 | USART_CR2_LBDIE;
  u->CR3 = config->sc_cr3 | USART_CR3_EIE;
  u->SR = 0;
  (void)u->SR;  /* SR reset step 1.*/
  (void)u->DR;  /* SR reset step 2.*/
}

/**
 * @brief   USART de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] u         pointer to an USART I/O block
 */
static void usart_deinit(USART_TypeDef *u) {

  u->CR1 = 0;
  u->CR2 = 0;
  u->CR3 = 0;
}

#if STM32_SERIAL_USE_USART1 || STM32_SERIAL_USE_USART2 ||                   \
    STM32_SERIAL_USE_USART3 || STM32_SERIAL_USE_UART4  ||                   \
    STM32_SERIAL_USE_UART5
/**
 * @brief   Error handling routine.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] sr        USART SR register value
 */
static void set_error(SerialDriver *sdp, uint16_t sr) {
  ioflags_t sts = 0;

  if (sr & USART_SR_ORE)
    sts |= SD_OVERRUN_ERROR;
  if (sr & USART_SR_PE)
    sts |= SD_PARITY_ERROR;
  if (sr & USART_SR_FE)
    sts |= SD_FRAMING_ERROR;
  if (sr & USART_SR_NE)
    sts |= SD_NOISE_ERROR;
  if (sr & USART_SR_LBD)
    sts |= SD_BREAK_DETECTED;
  chSysLockFromIsr();
  chIOAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();
}

/**
 * @brief   Common IRQ handler.
 *
 * @param[in] sdp       communication channel associated to the USART
 */
static void serve_interrupt(SerialDriver *sdp) {
  USART_TypeDef *u = sdp->usart;
  uint16_t cr1 = u->CR1;
  uint16_t sr = u->SR;  /* SR reset step 1.*/
  uint16_t dr = u->DR;  /* SR reset step 2.*/

  if (sr & (USART_SR_LBD | USART_SR_ORE | USART_SR_NE |
            USART_SR_FE  | USART_SR_PE)) {
    set_error(sdp, sr);
    u->SR = 0;    /* Clears the LBD bit in the SR.*/
  }
  if (sr & USART_SR_RXNE) {
    chSysLockFromIsr();
    sdIncomingDataI(sdp, (uint8_t)dr);
    chSysUnlockFromIsr();
  }
  if ((cr1 & USART_CR1_TXEIE) && (sr & USART_SR_TXE)) {
    msg_t b;
    chSysLockFromIsr();
    b = chOQGetI(&sdp->oqueue);
    if (b < Q_OK) {
      chIOAddFlagsI(sdp, IO_OUTPUT_EMPTY);
      u->CR1 = cr1 & ~USART_CR1_TXEIE;
    }
    else
      u->DR = b;
    chSysUnlockFromIsr();
  }
}
#endif

#if STM32_SERIAL_USE_USART1 || defined(__DOXYGEN__)
static void notify1(GenericQueue *qp) {

  (void)qp;
  USART1->CR1 |= USART_CR1_TXEIE;
}
#endif

#if STM32_SERIAL_USE_USART2 || defined(__DOXYGEN__)
static void notify2(GenericQueue *qp) {

  (void)qp;
  USART2->CR1 |= USART_CR1_TXEIE;
}
#endif

#if STM32_SERIAL_USE_USART3 || defined(__DOXYGEN__)
static void notify3(GenericQueue *qp) {

  (void)qp;
  USART3->CR1 |= USART_CR1_TXEIE;
}
#endif

#if STM32_SERIAL_USE_UART4 || defined(__DOXYGEN__)
static void notify4(GenericQueue *qp) {

  (void)qp;
  UART4->CR1 |= USART_CR1_TXEIE;
}
#endif

#if STM32_SERIAL_USE_UART5 || defined(__DOXYGEN__)
static void notify5(GenericQueue *qp) {

  (void)qp;
  UART5->CR1 |= USART_CR1_TXEIE;
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_SERIAL_USE_USART1 || defined(__DOXYGEN__)
/**
 * @brief   USART1 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USART1_IRQHandler) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD1);

  CH_IRQ_EPILOGUE();
}
#endif

#if STM32_SERIAL_USE_USART2 || defined(__DOXYGEN__)
/**
 * @brief   USART2 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USART2_IRQHandler) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD2);

  CH_IRQ_EPILOGUE();
}
#endif

#if STM32_SERIAL_USE_USART3 || defined(__DOXYGEN__)
/**
 * @brief   USART3 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USART3_IRQHandler) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD3);

  CH_IRQ_EPILOGUE();
}
#endif

#if STM32_SERIAL_USE_UART4 || defined(__DOXYGEN__)
/**
 * @brief   UART4 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(UART4_IRQHandler) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD4);

  CH_IRQ_EPILOGUE();
}
#endif

#if STM32_SERIAL_USE_UART5 || defined(__DOXYGEN__)
/**
 * @brief   UART5 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(UART5_IRQHandler) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD5);

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 *
 * @notapi
 */
void sd_lld_init(void) {

#if STM32_SERIAL_USE_USART1
  sdObjectInit(&SD1, NULL, notify1);
  SD1.usart = USART1;
#endif

#if STM32_SERIAL_USE_USART2
  sdObjectInit(&SD2, NULL, notify2);
  SD2.usart = USART2;
#endif

#if STM32_SERIAL_USE_USART3
  sdObjectInit(&SD3, NULL, notify3);
  SD3.usart = USART3;
#endif

#if STM32_SERIAL_USE_UART4
  sdObjectInit(&SD4, NULL, notify4);
  SD4.usart = UART4;
#endif

#if STM32_SERIAL_USE_UART5
  sdObjectInit(&SD5, NULL, notify5);
  SD5.usart = UART5;
#endif
}

/**
 * @brief   Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 *
 * @notapi
 */
void sd_lld_start(SerialDriver *sdp, const SerialConfig *config) {

  if (config == NULL)
    config = &default_config;

  if (sdp->state == SD_STOP) {
#if STM32_SERIAL_USE_USART1
    if (&SD1 == sdp) {
      RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
      NVICEnableVector(USART1_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SERIAL_USART1_PRIORITY));
    }
#endif
#if STM32_SERIAL_USE_USART2
    if (&SD2 == sdp) {
      RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
      NVICEnableVector(USART2_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SERIAL_USART2_PRIORITY));
    }
#endif
#if STM32_SERIAL_USE_USART3
    if (&SD3 == sdp) {
      RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
      NVICEnableVector(USART3_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SERIAL_USART3_PRIORITY));
    }
#endif
#if STM32_SERIAL_USE_UART4
    if (&SD4 == sdp) {
      RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
      NVICEnableVector(UART4_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SERIAL_UART4_PRIORITY));
    }
#endif
#if STM32_SERIAL_USE_UART5
    if (&SD5 == sdp) {
      RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
      NVICEnableVector(UART5_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SERIAL_UART5_PRIORITY));
    }
#endif
  }
  usart_init(sdp, config);
}

/**
 * @brief   Low level serial driver stop.
 * @details De-initializes the USART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @notapi
 */
void sd_lld_stop(SerialDriver *sdp) {

  if (sdp->state == SD_READY) {
    usart_deinit(sdp->usart);
#if STM32_SERIAL_USE_USART1
    if (&SD1 == sdp) {
      RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
      NVICDisableVector(USART1_IRQn);
      return;
    }
#endif
#if STM32_SERIAL_USE_USART2
    if (&SD2 == sdp) {
      RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN;
      NVICDisableVector(USART2_IRQn);
      return;
    }
#endif
#if STM32_SERIAL_USE_USART3
    if (&SD3 == sdp) {
      RCC->APB1ENR &= ~RCC_APB1ENR_USART3EN;
      NVICDisableVector(USART3_IRQn);
      return;
    }
#endif
#if STM32_SERIAL_USE_UART4
    if (&SD4 == sdp) {
      RCC->APB1ENR &= ~RCC_APB1ENR_UART4EN;
      NVICDisableVector(UART4_IRQn);
      return;
    }
#endif
#if STM32_SERIAL_USE_UART5
    if (&SD5 == sdp) {
      RCC->APB1ENR &= ~RCC_APB1ENR_UART5EN;
      NVICDisableVector(UART5_IRQn);
      return;
    }
#endif
  }
}

#endif /* HAL_USE_SERIAL */

/** @} */

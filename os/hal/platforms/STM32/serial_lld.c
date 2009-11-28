/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @file STM32/serial_lld.c
 * @brief STM32 low level serial driver code
 * @addtogroup STM32_SERIAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_SERIAL

#if USE_STM32_USART1 || defined(__DOXYGEN__)
/** @brief USART1 serial driver identifier.*/
SerialDriver SD1;
#endif

#if USE_STM32_USART2 || defined(__DOXYGEN__)
/** @brief USART2 serial driver identifier.*/
SerialDriver SD2;
#endif

#if USE_STM32_USART3 || defined(__DOXYGEN__)
/** @brief USART3 serial driver identifier.*/
SerialDriver SD3;
#endif

/** @brief Driver default configuration.*/
static const SerialDriverConfig default_config =
{
  38400,
  0,
  USART_CR2_STOP1_BITS | USART_CR2_LINEN,
  0
};

/*===========================================================================*/
/* Low Level Driver local functions.                                         */
/*===========================================================================*/

/**
 * @brief USART initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] u pointer to an USART I/O block
 * @param[in] config the architecture-dependent serial driver configuration
 */
static void usart_init(USART_TypeDef *u, const SerialDriverConfig *config) {

  /*
   * Baud rate setting.
   */
  if (u == USART1)
    u->BRR = APB2CLK / config->speed;
  else
    u->BRR = APB1CLK / config->speed;

  /*
   * Note that some bits are enforced.
   */
  u->CR1 = config->cr1 | USART_CR1_UE | USART_CR1_PEIE | USART_CR1_RXNEIE |
                         USART_CR1_TE | USART_CR1_RE;
  u->CR2 = config->cr2;
  u->CR3 = config->cr3 | USART_CR3_EIE;
}

/**
 * @brief USART de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] u pointer to an USART I/O block
 */
static void usart_deinit(USART_TypeDef *u) {

  u->CR1 = 0;
  u->CR2 = 0;
  u->CR3 = 0;
}

/**
 * @brief Error handling routine.
 * @param[in] sr USART SR register value
 * @param[in] com communication channel associated to the USART
 */
static void set_error(uint16_t sr, SerialDriver *sdp) {
  sdflags_t sts = 0;

  if (sr & USART_SR_ORE)
    sts |= SD_OVERRUN_ERROR;
  if (sr & USART_SR_PE)
    sts |= SD_PARITY_ERROR;
  if (sr & USART_SR_FE)
    sts |= SD_FRAMING_ERROR;
  if (sr & USART_SR_LBD)
    sts |= SD_BREAK_DETECTED;
  chSysLockFromIsr();
  sdAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();
}

/**
 * @brief Common IRQ handler.
 * @param[in] u pointer to an USART I/O block
 * @param[in] com communication channel associated to the USART
 */
static void serve_interrupt(USART_TypeDef *u, SerialDriver *sdp) {
  uint16_t sr = u->SR;

  if (sr & (USART_SR_ORE | USART_SR_FE | USART_SR_PE | USART_SR_LBD))
    set_error(sr, sdp);
  if (sr & USART_SR_RXNE) {
    chSysLockFromIsr();
    sdIncomingDataI(sdp, u->DR);
    chSysUnlockFromIsr();
  }
  if (sr & USART_SR_TXE) {
    chSysLockFromIsr();
    msg_t b = sdRequestDataI(sdp);
    chSysUnlockFromIsr();
    if (b < Q_OK)
      u->CR1 &= ~USART_CR1_TXEIE;
    else
      u->DR = b;
  }
}

#if USE_STM32_USART1 || defined(__DOXYGEN__)
static void notify1(void) {

  USART1->CR1 |= USART_CR1_TXEIE;
}
#endif

#if USE_STM32_USART2 || defined(__DOXYGEN__)
static void notify2(void) {

  USART2->CR1 |= USART_CR1_TXEIE;
}
#endif

#if USE_STM32_USART3 || defined(__DOXYGEN__)
static void notify3(void) {

  USART3->CR1 |= USART_CR1_TXEIE;
}
#endif

/*===========================================================================*/
/* Low Level Driver interrupt handlers.                                      */
/*===========================================================================*/

#if USE_STM32_USART1 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(VectorD4) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(USART1, &SD1);

  CH_IRQ_EPILOGUE();
}
#endif

#if USE_STM32_USART2 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(VectorD8) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(USART2, &SD2);

  CH_IRQ_EPILOGUE();
}
#endif

#if USE_STM32_USART3 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(VectorDC) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(USART3, &SD3);

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Low Level Driver exported functions.                                      */
/*===========================================================================*/

/**
 * Low level serial driver initialization.
 */
void sd_lld_init(void) {

#if USE_STM32_USART1
  sdObjectInit(&SD1, NULL, notify1);
  GPIOA->CRH = (GPIOA->CRH & 0xFFFFF00F) | 0x000004B0;
#endif

#if USE_STM32_USART2
  sdObjectInit(&SD2, NULL, notify2);
  GPIOA->CRL = (GPIOA->CRL & 0xFFFF00FF) | 0x00004B00;
#endif

#if USE_STM32_USART3
  sdObjectInit(&SD3, NULL, notify3);
  GPIOB->CRH = (GPIOB->CRH & 0xFFFF00FF) | 0x00004B00;
#endif
}

/**
 * @brief Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp pointer to a @p SerialDriver object
 * @param[in] config the architecture-dependent serial driver configuration.
 *                   If this parameter is set to @p NULL then a default
 *                   configuration is used.
 */
void sd_lld_start(SerialDriver *sdp, const SerialDriverConfig *config) {

  if (config == NULL)
    config = &default_config;

#if USE_STM32_USART1
  if (&SD1 == sdp) {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    usart_init(USART1, config);
    NVICEnableVector(USART1_IRQn, STM32_USART1_PRIORITY);
    return;
  }
#endif
#if USE_STM32_USART2
  if (&SD2 == sdp) {
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    usart_init(USART2, config);
    NVICEnableVector(USART2_IRQn, STM32_USART2_PRIORITY);
    return;
  }
#endif
#if USE_STM32_USART3
  if (&SD3 == sdp) {
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    usart_init(USART3, config);
    NVICEnableVector(USART3_IRQn, STM32_USART3_PRIORITY);
    return;
  }
#endif
}

/**
 * @brief Low level serial driver stop.
 * @details De-initializes the USART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp pointer to a @p SerialDriver object
 */
void sd_lld_stop(SerialDriver *sdp) {

#if USE_STM32_USART1
  if (&SD1 == sdp) {
    usart_deinit(USART1);
    RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
    NVICDisableVector(USART1_IRQn);
    return;
  }
#endif
#if USE_STM32_USART2
  if (&SD2 == sdp) {
    usart_deinit(USART2);
    RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN;
    NVICDisableVector(USART2_IRQn);
    return;
  }
#endif
#if USE_STM32_USART3
  if (&SD3 == sdp) {
    usart_deinit(USART3);
    RCC->APB1ENR &= ~RCC_APB1ENR_USART3EN;
    NVICDisableVector(USART3_IRQn);
    return;
  }
#endif
}

#endif /* CH_HAL_USE_SERIAL */

/** @} */

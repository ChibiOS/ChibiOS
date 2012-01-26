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
*/

/**
 * @file    STM8L/serial_lld.c
 * @brief   STM8L low level serial driver code.
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

/**
 * @brief   USART1 serial driver identifier.
 */
#if STM8L_SERIAL_USE_USART1 || defined(__DOXYGEN__)
SerialDriver SD1;
#endif

/**
 * @brief   USART2 serial driver identifier.
 */
#if STM8L_SERIAL_USE_USART2 || defined(__DOXYGEN__)
SerialDriver SD2;
#endif

/**
 * @brief   USART3 serial driver identifier.
 */
#if STM8L_SERIAL_USE_USART3 || defined(__DOXYGEN__)
SerialDriver SD3;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static ROMCONST SerialConfig default_config = {
  BRR(SERIAL_DEFAULT_BITRATE),
  SD_MODE_PARITY_NONE | SD_MODE_STOP_1
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if STM8L_SERIAL_USE_USART1 || defined(__DOXYGEN__)
static void notify1(GenericQueue *qp) {

  (void)qp;
  USART1->CR2 |= USART_CR2_TIEN;
}
#endif /* STM8L_SERIAL_USE_USART1 */

#if STM8L_SERIAL_USE_USART2 || defined(__DOXYGEN__)
static void notify2(GenericQueue *qp) {

  (void)qp;
  USART2->CR2 |= USART_CR2_TIEN;
}
#endif /* STM8L_SERIAL_USE_USART1 */

#if STM8L_SERIAL_USE_USART3 || defined(__DOXYGEN__)
static void notify3(GenericQueue *qp) {

  (void)qp;
  USART3->CR2 |= USART_CR2_TIEN;
}
#endif /* STM8L_SERIAL_USE_USART3 */

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/* See in serial_lld.h.*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Error handling routine.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] sr        USART SR register value
 *
 * @notapi
 */
void sd_lld_set_error(SerialDriver *sdp, uint8_t sr) {
  ioflags_t sts = 0;

  if (sr & USART_SR_OR)
    sts |= SD_OVERRUN_ERROR;
  if (sr & USART_SR_NF)
    sts |= SD_NOISE_ERROR;
  if (sr & USART_SR_FE)
    sts |= SD_FRAMING_ERROR;
  if (sr & USART_SR_PE)
    sts |= SD_PARITY_ERROR;
  chSysLockFromIsr();
  chIOAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();
}

/**
 * @brief   Low level serial driver initialization.
 *
 * @notapi
 */
void sd_lld_init(void) {

#if STM8L_SERIAL_USE_USART1
  sdObjectInit(&SD1, NULL, notify1);
  CLK->PCKENR1 |= CLK_PCKENR1_USART1;       /* PCKEN12, clock source.       */
  USART1->CR1 = USART_CR1_USARTD;           /* USARTD (low power).          */
  SD1.usart = USART1;
#endif

#if STM8L_SERIAL_USE_USART2
  sdObjectInit(&SD2, NULL, notify2);
  CLK->PCKENR3 |= CLK_PCKENR3_USART2;       /* PCKEN13, clock source.       */
  USART2->CR1 = USART_CR1_USARTD;           /* USARTD (low power).          */
  SD2.usart = USART2;
#endif

#if STM8L_SERIAL_USE_USART3
  sdObjectInit(&SD3, NULL, notify3);
  CLK->PCKENR3 |= CLK_PCKENR3_USART3;       /* PCKEN13, clock source.       */
  USART3->CR1 = USART_CR1_USARTD;           /* USARTD (low power).          */
  SD3.usart = USART3;
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
  USART_TypeDef *u = sdp->usart;

  if (config == NULL)
    config = &default_config;

  u->BRR2 = (uint8_t)(((uint8_t)(config->sc_brr >> 8) & (uint8_t)0xF0) |
                      ((uint8_t)config->sc_brr & (uint8_t)0x0F));
  u->BRR1 = (uint8_t)(config->sc_brr >> 4);
  u->CR1  = (uint8_t)(config->sc_mode & SD_MODE_PARITY);
  u->CR2  = USART_CR2_RIEN | USART_CR2_TEN | USART_CR2_REN;
  u->CR3  = (uint8_t)(config->sc_mode & SD_MODE_STOP);
  u->CR4  = 0;
  u->CR5  = 0;
  u->PSCR = 1;
  (void)u->SR;
  (void)u->DR;
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
  USART_TypeDef *u = sdp->usart;

  u->CR1  = USART_CR1_USARTD;
  u->CR2  = 0;
  u->CR3  = 0;
  u->CR4  = 0;
  u->CR5  = 0;
  u->PSCR = 0;
}

#endif /* HAL_USE_SERIAL */

/** @} */

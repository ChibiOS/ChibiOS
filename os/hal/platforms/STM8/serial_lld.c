/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    STM8/serial_lld.c
 * @brief   STM8 low level serial driver code.
 *
 * @addtogroup STM8_SERIAL
 * @{
 */

#include "ch.h"
#include "hal.h"

/* Because someone somewhere couldn't use the same name for the same thing.*/
#if STM8_PLATFORM == PLATFORM_STM8AF51AA
#define UART1_BRR1  USART_BRR1
#define UART1_BRR2  USART_BRR2
#define UART1_SR    USART_SR
#define UART1_DR    USART_DR
#define UART1_CR1   USART_CR1
#define UART1_CR2   USART_CR2
#define UART1_CR3   USART_CR3
#define UART1_CR4   USART_CR4
#define UART1_CR5   USART_CR5

#define UART3_BRR1  LINUART_BRR1
#define UART3_BRR2  LINUART_BRR2
#define UART3_SR    LINUART_SR
#define UART3_DR    LINUART_DR
#define UART3_CR1   LINUART_CR1
#define UART3_CR2   LINUART_CR2
#define UART3_CR3   LINUART_CR3
#define UART3_CR4   LINUART_CR4
#define UART3_CR5   LINUART_CR5
#define UART3_CR6   LINUART_CR6
#endif

#if CH_HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   UART1 serial driver identifier.
 */
#if USE_STM8_UART1 || defined(__DOXYGEN__)
SerialDriver SD1;
#endif

/**
 * @brief   UART3 serial driver identifier.
 */
#if USE_STM8_UART3 || defined(__DOXYGEN__)
SerialDriver SD3;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static ROMCONST SerialConfig default_config = {
  BBR(SERIAL_DEFAULT_BITRATE),
  SD_MODE_PARITY_NONE | SD_MODE_STOP_1
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void set_error(SerialDriver *sdp, uint8_t sr) {
  sdflags_t sts = 0;

  if (sr & 0x08)                            /* OR bit.                      */
    sts |= SD_OVERRUN_ERROR;
  if (sr & 0x04)                            /* NF bit.                      */
    sts |= SD_NOISE_ERROR;
  if (sr & 0x02)                            /* FE bit.                      */
    sts |= SD_FRAMING_ERROR;
  if (sr & 0x01)                            /* PE bit.                      */
    sts |= SD_PARITY_ERROR;
  chSysLockFromIsr();
  sdAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();
}

#if USE_STM8_UART1 || defined(__DOXYGEN__)
static void notify1(void) {

  UART1_CR2 |= 0x80;                        /* TIEN bit.                    */
}

/**
 * @brief   UART1 initialization.
 *
 * @param[in] config    architecture-dependent serial driver configuration
 */
static void uart1_init(const SerialConfig *config) {

  UART1_BRR2 = ((uint8_t)(config->sc_brr >> 8) & (uint8_t)0xF0) |
               ((uint8_t)config->sc_brr & (uint8_t)0x0F);
  UART1_BRR1 = (uint8_t)(config->sc_brr >> 4);
  UART1_CR1  = config->sc_mode &
               SD_MODE_PARITY;              /* PIEN included.               */
  UART1_CR2  = 0x2C;                        /* RIEN | TEN | REN.            */
  UART1_CR3  = config->sc_mode & SD_MODE_STOP;
  UART1_CR4  = 0;
  UART1_CR5  = 0;
  UART1_PSCR = 1;
  (void)UART1_SR;
  (void)UART1_DR;
}

/**
 * @brief   UART1 de-initialization.
 */
static void uart1_deinit(void) {

  UART1_CR1  = 0x20;                        /* UARTD (low power).           */
  UART1_CR2  = 0;
  UART1_CR3  = 0;
  UART1_CR4  = 0;
  UART1_CR5  = 0;
  UART1_PSCR = 0;
}
#endif /* USE_STM8_UART1 */

#if USE_STM8_UART3 || defined(__DOXYGEN__)
static void notify3(void) {

  UART3_CR2 |= 0x80;                        /* TIEN bit.                    */
}

/**
 * @brief   UART3 initialization.
 *
 * @param[in] config    architecture-dependent serial driver configuration
 */
static void uart3_init(const SerialConfig *config) {

  UART3_BRR2 = ((uint8_t)(config->sc_brr >> 8) & (uint8_t)0xF0) |
               ((uint8_t)config->sc_brr & (uint8_t)0x0F);
  UART3_BRR1 = (uint8_t)(config->sc_brr >> 4);
  UART3_CR1  = config->sc_mode &
               SD_MODE_PARITY;              /* PIEN included.               */
  UART3_CR2  = 0x2C;                        /* RIEN | TEN | REN.            */
  UART3_CR3  = config->sc_mode & SD_MODE_STOP;
  UART3_CR4  = 0;
  UART3_CR6  = 0;
  (void)UART3_SR;
  (void)UART3_DR;
}

/**
 * @brief   UART3 de-initialization.
 */
static void uart3_deinit(void) {

  UART3_CR1  = 0x20;                        /* UARTD (low power).           */
  UART3_CR2  = 0;
  UART3_CR3  = 0;
  UART3_CR4  = 0;
  UART3_CR6  = 0;
}
#endif /* USE_STM8_UART3 */

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if USE_STM8_UART1 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(17) {
  msg_t b;

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  b = sdRequestDataI(&SD1);
  chSysUnlockFromIsr();
  if (b < Q_OK)
    UART1_CR2 &= ~0x80;                         /* TIEN.                    */
  else
    UART1_DR = b;

  CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(18) {
  uint8_t sr = UART1_SR;

  CH_IRQ_PROLOGUE();

  if ((sr = UART1_SR) & 0x0F)                   /* OR | BF | FE | PE.       */
    set_error(&SD1, sr);
  chSysLockFromIsr();
  sdIncomingDataI(&SD1, UART1_DR);
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}
#endif /* USE_STM8_UART1 */

#if USE_STM8_UART3 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(20) {
  msg_t b;

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  b = sdRequestDataI(&SD3);
  chSysUnlockFromIsr();
  if (b < Q_OK)
    UART3_CR2 &= ~0x80;                         /* TIEN.                    */
  else
    UART3_DR = b;

  CH_IRQ_EPILOGUE();
}

CH_IRQ_HANDLER(21) {
  uint8_t sr = UART3_SR;

  CH_IRQ_PROLOGUE();

  if ((sr = UART3_SR) & 0x0F)                   /* OR | BF | FE | PE.       */
    set_error(&SD3, sr);
  chSysLockFromIsr();
  sdIncomingDataI(&SD3, UART3_DR);
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}
#endif /* USE_STM8_UART3 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 */
void sd_lld_init(void) {

#if USE_STM8_UART1
  sdObjectInit(&SD1, NULL, notify1);
  CLK_PCKENR1 |= 4;                         /* PCKEN12, clock source.       */
  UART1_CR1 = 0x20;                         /* UARTD (low power).           */
#endif

#if USE_STM8_UART3
  sdObjectInit(&SD3, NULL, notify3);
  CLK_PCKENR1 |= 8;                         /* PCKEN13, clock source.       */
  UART3_CR1 = 0x20;                         /* UARTD (low power).           */
#endif
}

/**
 * @brief   Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 */
void sd_lld_start(SerialDriver *sdp, const SerialConfig *config) {

  if (config == NULL)
    config = &default_config;

#if USE_STM8_UART1
  if (&SD1 == sdp) {
    uart1_init(config);
    return;
  }
#endif
#if USE_STM8_UART3
  if (&SD3 == sdp) {
    uart3_init(config);
    return;
  }
#endif
}

/**
 * @brief   Low level serial driver stop.
 * @details De-initializes the USART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
void sd_lld_stop(SerialDriver *sdp) {

#if USE_STM8_UART1
  if (&SD1 == sdp) {
    uart1_deinit();
    return;
  }
#endif
#if USE_STM8_UART3
  if (&SD3 == sdp) {
    uart3_deinit();
    return;
  }
#endif
}

#endif /* CH_HAL_USE_SERIAL */

/** @} */

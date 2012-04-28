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
 * @file    MSP430/serial_lld.c
 * @brief   MSP430 low level serial driver code.
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

#if USE_MSP430_USART0 || defined(__DOXYGEN__)
/** @brief USART0 serial driver identifier.*/
SerialDriver SD1;
#endif
#if USE_MSP430_USART1 || defined(__DOXYGEN__)
/** @brief USART1 serial driver identifier.*/
SerialDriver SD2;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/** @brief Driver default configuration.*/
static const SerialConfig default_config = {
  UBR(SERIAL_DEFAULT_BITRATE),
  0,
  CHAR
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void set_error(SerialDriver *sdp, uint8_t urctl) {
  ioflags_t sts = 0;

  if (urctl & OE)
    sts |= SD_OVERRUN_ERROR;
  if (urctl & PE)
    sts |= SD_PARITY_ERROR;
  if (urctl & FE)
    sts |= SD_FRAMING_ERROR;
  if (urctl & BRK)
    sts |= SD_BREAK_DETECTED;
  chSysLockFromIsr();
  chIOAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();
}

#if USE_MSP430_USART0 || defined(__DOXYGEN__)
static void notify1(GenericQueue *qp) {

  (void)qp;
  if (!(U0IE & UTXIE0)) {
    msg_t b = sdRequestDataI(&SD1);
    if (b != Q_EMPTY) {
      U0IE |= UTXIE0;
      U0TXBUF = (uint8_t)b;
    }
  }
}

/**
 * @brief   USART0 initialization.
 *
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void usart0_init(const SerialConfig *config) {

  U0CTL = SWRST;                            /* Resets the USART.            */
  /* USART init */
  U0TCTL = SSEL0 | SSEL1;                   /* SMCLK as clock source.       */
  U0MCTL = config->sc_mod;                  /* Modulator.                   */
  U0BR1 = (uint8_t)(config->sc_div >> 8);   /* Divider high.                */
  U0BR0 = (uint8_t)(config->sc_div >> 0);   /* Divider low.                 */
  /* Clear USART status.*/
  (void)U0RXBUF;
  U0RCTL = 0;
  /* USART enable.*/
  U0ME |= UTXE0 + URXE0;                    /* Enables the USART.           */
  U0CTL = config->sc_ctl & ~SWRST;          /* Various settings.            */
  U0IE |= URXIE0;                           /* Enables RX interrupt.        */
}

/**
 * @brief   USART0 de-initialization.
 */
static void usart0_deinit(void) {

  U0IE &= ~URXIE0;
  U0CTL = SWRST;
}
#endif /* USE_MSP430_USART0 */

#if USE_MSP430_USART1 || defined(__DOXYGEN__)
static void notify2(GenericQueue *qp) {

  (void)qp;
  if (!(U1IE & UTXIE1)) {
    msg_t b = sdRequestDataI(&SD2);
    if (b != Q_EMPTY) {
      U1IE |= UTXIE1;
      U1TXBUF = (uint8_t)b;
    }
  }
}

/**
 * @brief   USART1 initialization.
 *
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void usart1_init(const SerialConfig *config) {

  U1CTL = SWRST;                            /* Resets the USART.            */
  /* USART init.*/
  U1TCTL = SSEL0 | SSEL1;                   /* SMCLK as clock source.       */
  U1MCTL = config->sc_mod;                  /* Modulator.                   */
  U1BR1 = (uint8_t)(config->sc_div >> 8);   /* Divider high.                */
  U1BR0 = (uint8_t)(config->sc_div >> 0);   /* Divider low.                 */
  /* Clear USART status.*/
  (void)U0RXBUF;
  U1RCTL = 0;
  /* USART enable.*/
  U1ME |= UTXE0 + URXE0;                    /* Enables the USART.           */
  U1CTL = config->sc_ctl & ~SWRST;          /* Various settings.            */
  U1IE |= URXIE0;                           /* Enables RX interrupt.        */
}

/**
 * @brief   USART1 de-initialization.
 */
static void usart1_deinit(void) {

  U1IE &= ~URXIE0;
  U1CTL = SWRST;
}
#endif /* USE_MSP430_USART1 */

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if USE_MSP430_USART0 || defined(__DOXYGEN__)
/**
 * @brief   USART0 TX interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USART0TX) {
  msg_t b;

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  b = sdRequestDataI(&SD1);
  chSysUnlockFromIsr();
  if (b < Q_OK)
    U0IE &= ~UTXIE0;
  else
    U0TXBUF = b;

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   USART0 RX interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USART0RX) {
  uint8_t urctl;

  CH_IRQ_PROLOGUE();

  if ((urctl = U0RCTL) & RXERR)
    set_error(&SD1, urctl);
  chSysLockFromIsr();
  sdIncomingDataI(&SD1, U0RXBUF);
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}
#endif /* USE_MSP430_USART0 */

#if USE_MSP430_USART1 || defined(__DOXYGEN__)
/**
 * @brief   USART1 TX interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USART1TX_VECTOR) {
  msg_t b;

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  b = sdRequestDataI(&SD2);
  chSysUnlockFromIsr();
  if (b < Q_OK)
    U1IE &= ~UTXIE1;
  else
    U1TXBUF = b;

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   USART1 RX interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USART1RX_VECTOR) {
  uint8_t urctl;

  CH_IRQ_PROLOGUE();

  if ((urctl = U1RCTL) & RXERR)
    set_error(&SD2, urctl);
  chSysLockFromIsr();
  sdIncomingDataI(&SD2, U1RXBUF);
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}
#endif /* USE_MSP430_USART1 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 *
 * @notapi
 */
void sd_lld_init(void) {

#if USE_MSP430_USART0
  sdObjectInit(&SD1, NULL, notify1);
#endif

#if USE_MSP430_USART1
  sdObjectInit(&SD2, NULL, notify2);
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

#if USE_MSP430_USART0
  if (&SD1 == sdp) {
    usart0_init(config);
    return;
  }
#endif
#if USE_MSP430_USART1
  if (&SD2 == sdp) {
    usart1_init(config);
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
 *
 * @notapi
 */
void sd_lld_stop(SerialDriver *sdp) {

#if USE_MSP430_USART0
  if (&SD1 == sdp) {
    usart0_deinit();
    return;
  }
#endif
#if USE_MSP430_USART1
  if (&SD2 == sdp) {
    usart1_deinit();
    return;
  }
#endif
}

#endif /* HAL_USE_SERIAL */

/** @} */

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
 * @file AT91SAM7X/serial_lld.c
 * @brief AT91SAM7X low level serial driver code
 * @addtogroup AT91SAM7X_SERIAL
 * @{
 */

#include <ch.h>
#include <serial.h>

#include "at91lib/aic.h"
#include "at91sam7.h"

#if SAM7_PLATFORM == SAM7S256

#define SAM7_USART0_RX    AT91C_PA5_RXD0
#define SAM7_USART0_TX    AT91C_PA6_TXD0
#define SAM7_USART1_RX    AT91C_PA21_RXD1
#define SAM7_USART1_TX    AT91C_PA22_TXD1

#elif SAM7_PLATFORM == SAM7X256

#define SAM7_USART0_RX    AT91C_PA0_RXD0
#define SAM7_USART0_TX    AT91C_PA1_TXD0
#define SAM7_USART1_RX    AT91C_PA5_RXD1
#define SAM7_USART1_TX    AT91C_PA6_TXD1

#else
#error "serial lines not defined for this SAM7 version"
#endif

#if USE_SAM7_USART0 || defined(__DOXYGEN__)
/** @brief USART0 serial driver identifier.*/
SerialDriver SD1;
#endif

#if USE_SAM7_USART1 || defined(__DOXYGEN__)
/** @brief USART1 serial driver identifier.*/
SerialDriver SD2;
#endif

/** @brief Driver default configuration.*/
static const SerialDriverConfig default_config = {
  38400,
  AT91C_US_USMODE_NORMAL | AT91C_US_CLKS_CLOCK |
  AT91C_US_CHRL_8_BITS | AT91C_US_PAR_NONE | AT91C_US_NBSTOP_1_BIT
};

/*===========================================================================*/
/* Low Level Driver local functions.                                         */
/*===========================================================================*/

/**
 * @brief USART initialization.
 * @param[in] u pointer to an USART I/O block
 * @param[in] config the architecture-dependent serial driver configuration
 */
static void usart_init(AT91PS_USART u, const SerialDriverConfig *config) {

  /* Disables IRQ sources and stop operations.*/
  u->US_IDR = 0xFFFFFFFF;
  u->US_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RSTSTA;

  /* New parameters setup.*/
  if (config->mr & AT91C_US_OVER)
    u->US_BRGR = MCK / (config->speed * 8);
  else
    u->US_BRGR = MCK / (config->speed * 16);
  u->US_MR = config->mr;
  u->US_RTOR = 0;
  u->US_TTGR = 0;

  /* Enables operations and IRQ sources.*/
  u->US_CR = AT91C_US_RXEN | AT91C_US_TXEN | AT91C_US_DTREN | AT91C_US_RTSEN;
  u->US_IER = AT91C_US_RXRDY | AT91C_US_OVRE | AT91C_US_FRAME | AT91C_US_PARE |
              AT91C_US_RXBRK;
}

/**
 * @brief USART de-initialization.
 * @param[in] u pointer to an USART I/O block
 */
static void usart_deinit(AT91PS_USART u) {

  /* Disables IRQ sources and stop operations.*/
  u->US_IDR = 0xFFFFFFFF;
  u->US_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RSTSTA;
  u->US_MR = 0;
  u->US_RTOR = 0;
  u->US_TTGR = 0;
}

/**
 * @brief Error handling routine.
 * @param[in] err USART CSR register value
 * @param[in] sdp communication channel associated to the USART
 */
static void set_error(AT91_REG csr, SerialDriver *sdp) {
  sdflags_t sts = 0;

  if (csr & AT91C_US_OVRE)
    sts |= SD_OVERRUN_ERROR;
  if (csr & AT91C_US_PARE)
    sts |= SD_PARITY_ERROR;
  if (csr & AT91C_US_FRAME)
    sts |= SD_FRAMING_ERROR;
  if (csr & AT91C_US_RXBRK)
    sts |= SD_BREAK_DETECTED;
  chSysLockFromIsr();
  sdAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();
}

#if defined(__GNU__)
__attribute__((noinline))
#endif
/**
 * @brief Common IRQ handler.
 * @param[in] u pointer to an USART I/O block
 * @param[in] com communication channel associated to the USART
 */
static void serve_interrupt(AT91PS_USART u, SerialDriver *sdp) {

  if (u->US_CSR & AT91C_US_RXRDY) {
    chSysLockFromIsr();
    sdIncomingDataI(sdp, u->US_RHR);
    chSysUnlockFromIsr();
  }
  if (u->US_CSR & AT91C_US_TXRDY) {
    chSysLockFromIsr();
    msg_t b = sdRequestDataI(sdp);
    chSysUnlockFromIsr();
    if (b < Q_OK)
      u->US_IDR = AT91C_US_TXRDY;
    else
      u->US_THR = b;
  }
  if (u->US_CSR & (AT91C_US_OVRE | AT91C_US_FRAME | AT91C_US_PARE | AT91C_US_RXBRK)) {
    set_error(u->US_CSR, sdp);
    u->US_CR = AT91C_US_RSTSTA;
  }
  AT91C_BASE_AIC->AIC_EOICR = 0;
}

#if USE_SAM7_USART0 || defined(__DOXYGEN__)
static void notify1(void) {

  AT91C_BASE_US0->US_IER = AT91C_US_TXRDY;
}
#endif

#if USE_SAM7_USART1 || defined(__DOXYGEN__)
static void notify2(void) {

  AT91C_BASE_US1->US_IER = AT91C_US_TXRDY;
}
#endif

/*===========================================================================*/
/* Low Level Driver interrupt handlers.                                      */
/*===========================================================================*/

#if USE_SAM7_USART0 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(USART0IrqHandler) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(AT91C_BASE_US0, &SD1);

  CH_IRQ_EPILOGUE();
}
#endif

#if USE_SAM7_USART1 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(USART1IrqHandler) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(AT91C_BASE_US1, &SD2);

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

#if USE_SAM7_USART0
  sdObjectInit(&SD1, NULL, notify1);
  AT91C_BASE_PIOA->PIO_PDR   = SAM7_USART0_RX | SAM7_USART0_TX;
  AT91C_BASE_PIOA->PIO_ASR   = SAM7_USART0_RX | SAM7_USART0_TX;
  AT91C_BASE_PIOA->PIO_PPUDR = SAM7_USART0_RX | SAM7_USART0_TX;
  AIC_ConfigureIT(AT91C_ID_US0,
                  AT91C_AIC_SRCTYPE_HIGH_LEVEL | SAM7_USART0_PRIORITY,
                  USART0IrqHandler);
#endif

#if USE_SAM7_USART1
  sdObjectInit(&SD2, NULL, notify2);
  AT91C_BASE_PIOA->PIO_PDR   = SAM7_USART1_RX | SAM7_USART1_TX;
  AT91C_BASE_PIOA->PIO_ASR   = SAM7_USART1_RX | SAM7_USART1_TX;
  AT91C_BASE_PIOA->PIO_PPUDR = SAM7_USART1_RX | SAM7_USART1_TX;
  AIC_ConfigureIT(AT91C_ID_US1,
                  AT91C_AIC_SRCTYPE_HIGH_LEVEL | SAM7_USART1_PRIORITY,
                  USART1IrqHandler);
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

#if USE_SAM7_USART0
  if (&SD1 == sdp) {
    /* Starts the clock and clears possible sources of immediate interrupts.*/
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_US0);
    /* USART initialization.*/
    usart_init(AT91C_BASE_US0, config);
    /* Enables associated interrupt vector.*/
    AIC_EnableIT(AT91C_ID_US0);
    return;
  }
#endif
#if USE_SAM7_USART1
  if (&SD2 == sdp) {
    /* Starts the clock and clears possible sources of immediate interrupts.*/
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_US1);
    /* USART initialization.*/
    usart_init(AT91C_BASE_US1, config);
    /* Enables associated interrupt vector.*/
    AIC_EnableIT(AT91C_ID_US1);
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

#if USE_SAM7_USART0
  if (&SD1 == sdp) {
    usart_deinit(AT91C_BASE_US0);
    AT91C_BASE_PMC->PMC_PCDR = (1 << AT91C_ID_US0);
    AIC_DisableIT(AT91C_ID_US0);
    return;
  }
#endif
#if USE_SAM7_USART1
  if (&SD2 == sdp) {
    usart_deinit(AT91C_BASE_US1);
    AT91C_BASE_PMC->PMC_PCDR = (1 << AT91C_ID_US1);
    AIC_DisableIT(AT91C_ID_US1);
    return;
  }
#endif
}

/** @} */

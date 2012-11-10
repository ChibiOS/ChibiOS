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
  UCBRS_0 + UCBRF_13 + UCOS16, /* Modln UCBRSx=0, UCBRFx=0, over sampling */
  0x00 /* parity disabled,  8-bit data, one stop bit, UART mode, asynchronous mode */
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void set_error(SerialDriver *sdp, uint8_t urctl) {
  ioflags_t sts = 0;

  if (urctl & UCOE)
    sts |= SD_OVERRUN_ERROR;
  if (urctl & UCPE)
    sts |= SD_PARITY_ERROR;
  if (urctl & UCFE)
    sts |= SD_FRAMING_ERROR;
  if (urctl & UCBRK)
    sts |= SD_BREAK_DETECTED;
  chSysLockFromIsr();
  chIOAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();
}

#if USE_MSP430_USART0 || defined(__DOXYGEN__)
static void notify1(GenericQueue *qp) {

  (void)qp;
  if (!(UCA0IE & UCTXIE)) {
    msg_t b = sdRequestDataI(&SD1);
    if (b != Q_EMPTY) {
      UCA0IE |= UCTXIE;
      UCA0TXBUF = (uint8_t)b;
    }
  }
}

/**
 * @brief   USART0 initialization.
 *
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void usart0_init(const SerialConfig *config) { 
  UCA0CTL1 = UCSWRST;                      /* Put state machine in reset. */
  /* USART init */
  P3SEL |= (P3_O_TXD0_MASK | P3_I_RXD0_MASK);
  UCA0CTL1 |= UCSSEL_2;                     /* SMCLK as clock source.       */
  UCA0MCTL = config->sc_mod;   				      /* Modulator.                   */
  UCA0BR1 = (uint8_t)(config->sc_div >> 8); /* Divider high.                 */
  UCA0BR0 = (uint8_t)(config->sc_div >> 0); /* Divider low.                */
  /* Clear USART status.*/
  (void)UCA0RXBUF;
  /* USART enable.*/
  UCA0CTL0 = config->sc_ctl;               /* Various settings.            */
  UCA0CTL1 &= ~UCSWRST;                    /* Initialize USCI state machine. */
  UCA0IE |= UCRXIE;                        /* Enables RX interrupt.        */
}

/**
 * @brief   USART0 de-initialization.
 */
static void usart0_deinit(void) {

  UCA0IE &= ~UCRXIE;						/* Disables RX interrupt.      */
  UCA0CTL1 |= UCSWRST;                      /* Put state machine in reset. */
}
#endif /* USE_MSP430_USART0 */

#if USE_MSP430_USART1 || defined(__DOXYGEN__)
static void notify2(GenericQueue *qp) {

  (void)qp;
  if (!(UCA1IE & UCTXIE)) {
    msg_t b = sdRequestDataI(&SD2);
    if (b != Q_EMPTY) {
      UCA1IE |= UCTXIE;
      UCA1TXBUF = (uint8_t)b;
    }
  }
}

/**
 * @brief   USART1 initialization.
 *
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void usart1_init(const SerialConfig *config) {
  UCA1CTL1 = UCSWRST;                      /* Put state machine in reset. */
  /* USART init */    
  P5SEL |= (P5_O_TXD1_MASK | P5_I_RXD1_MASK);
  UCA1CTL1 |= UCSSEL_2;                     /* SMCLK as clock source.       */
  UCA1MCTL = config->sc_mod;   				/* Modulator.                   */
  UCA1BR1 = (uint8_t)(config->sc_div >> 8); /* Divider high.                 */
  UCA1BR0 = (uint8_t)(config->sc_div >> 0); /* Divider low.                */
  /* Clear USART status.*/
  (void)UCA1RXBUF;
  /* USART enable.*/
  UCA1CTL0 = config->sc_ctl;               /* Various settings.            */
  UCA1CTL1 &= ~UCSWRST;                    /* Initialize USCI state machine. */
  UCA1IE |= UCRXIE;                        /* Enables RX interrupt.        */  
}

/**
 * @brief   USART1 de-initialization.
 */
static void usart1_deinit(void) {

  UCA1IE &= ~UCRXIE;						/* Disables RX interrupt.      */
  UCA1CTL1 |= UCSWRST;                      /* Put state machine in reset. */
}
#endif /* USE_MSP430_USART1 */

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if USE_MSP430_USART0 || defined(__DOXYGEN__)
/**
 * @brief USART0 TX interrupt handler segment.
 */
#define _USART0_TRANSMIT_ISR() {                                            \
  msg_t b;                                                                  \
  chSysLockFromIsr();                                                       \
  b = sdRequestDataI(&SD1);                                                 \
  chSysUnlockFromIsr();                                                     \
  if (b < Q_OK)                                                             \
    UCA0IE &= ~UCTXIE;                                                      \
  else                                                                      \
    UCA0TXBUF = b;                                                          \
}

/**
 * @brief USART0 RX interrupt handler segment.
 */
#define _USART0_RECEIVE_ISR() {                                             \
  uint8_t urctl;                                                            \
  if ((urctl = UCA0STAT) & UCRXERR)                                         \
    set_error(&SD1, urctl);                                                 \
  chSysLockFromIsr();                                                       \
  sdIncomingDataI(&SD1, UCA0RXBUF);                                         \
  chSysUnlockFromIsr();                                                     \
}

/**
 * @brief   USART0 TX/RX interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USCI_A0) {

  CH_IRQ_PROLOGUE();

  #if defined(__IAR_SYSTEMS_ICC__)
  switch(__even_in_range(UCA0IV,4))
  #else
  switch(*(volatile uint16_t*)UCA0IV)
  #endif
  {
  case 2: /* Vector 2 - RXIFG */
    _USART0_RECEIVE_ISR();
    break;
	
  case 4: /* Vector 4 - TXIFG */
    _USART0_TRANSMIT_ISR();
    break;
	
  default:
      break;      
  }

  CH_IRQ_EPILOGUE();
}
#endif /* USE_MSP430_USART0 */

#if USE_MSP430_USART1 || defined(__DOXYGEN__)
/**
 * @brief USART1 TX interrupt handler segment.
 */
#define _USART1_TRANSMIT_ISR() {                                            \
  msg_t b;                                                                  \
  chSysLockFromIsr();                                                       \
  b = sdRequestDataI(&SD2);                                                 \
  chSysUnlockFromIsr();                                                     \
  if (b < Q_OK)                                                             \
    UCA1IE &= ~UCTXIE;                                                      \
  else                                                                      \
    UCA1TXBUF = b;                                                          \
}

/**
 * @brief USART0 RX interrupt handler segment.
 */
#define _USART1_RECEIVE_ISR() {                                             \
  uint8_t urctl;                                                            \
  if ((urctl = UCA1STAT) & UCRXERR)                                         \
    set_error(&SD2, urctl);                                                 \
  chSysLockFromIsr();                                                       \
  sdIncomingDataI(&SD2, UCA1RXBUF);                                         \
  chSysUnlockFromIsr();                                                     \
}

/**
 * @brief   USART1 TX/RX interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USCI_A1) {

  CH_IRQ_PROLOGUE();

  #if defined(__IAR_SYSTEMS_ICC__)
  switch(__even_in_range(UCA1IV,4))
  #else
  switch(*(volatile uint16_t*)UCA1IV)
  #endif
  {
  case 2: /* Vector 2 - RXIFG */
    _USART1_RECEIVE_ISR();
    break;
	
  case 4: /* Vector 4 - TXIFG */
    _USART1_TRANSMIT_ISR();
    break;
	
  default:
      break;      
  }

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

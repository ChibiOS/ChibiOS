/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
 * @file    LPC8xx/serial_lld.c
 * @brief   LPC8xx low level serial driver code.
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

#if LPC8xx_SERIAL_USE_UART0 || defined(__DOXYGEN__)
/** @brief UART0 serial driver identifier.*/
SerialDriver SD1;
#endif

#if LPC8xx_SERIAL_USE_UART1 || defined(__DOXYGEN__)
/** @brief UART1 serial driver identifier.*/
SerialDriver SD2;
#endif

#if LPC8xx_SERIAL_USE_UART2 || defined(__DOXYGEN__)
/** @brief UART2 serial driver identifier.*/
SerialDriver SD3;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/** @brief Driver default configuration.*/
static const SerialConfig default_config = {
  SERIAL_DEFAULT_BITRATE,
  (CFG_DL8 | CFG_NOPARITY | CFG_STOP1)
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Error handling routine.
 *
 * @param[in] sdp       communication channel associated to the UART
 * @param[in] err       UART STAT register value
 */
static void set_error(SerialDriver *sdp, IOREG32 err) {
  flagsmask_t sts = 0;

  if (err & STAT_OVERRUN)
    sts |= SD_OVERRUN_ERROR;
  if (err & STAT_PARITYERR)
    sts |= SD_PARITY_ERROR;
  if (err & STAT_FRAMERR)
    sts |= SD_FRAMING_ERROR;
  if (err & STAT_RXBRK)
    sts |= SD_BREAK_DETECTED;
    
  chSysLockFromIsr();
  chnAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();  
}

/**
 * @brief   Common IRQ handler.
 * @note    Tries hard to clear all the pending interrupt sources, we don't
 *          want to go through the whole ISR and have another interrupt soon
 *          after.
 *
 * @param[in] u         pointer to an UART I/O block
 * @param[in] sdp       communication channel associated to the UART
 */
static void serve_interrupt(SerialDriver *sdp) {
  LPC_USART_TypeDef *u = sdp->uart;

  while (u->INTSTAT) {
  
    if (u->INTSTAT & STAT_RXRDY) {
      chSysLockFromIsr();
      if (chIQIsEmptyI(&sdp->iqueue))
        chnAddFlagsI(sdp, CHN_INPUT_AVAILABLE);
      if (chIQPutI(&sdp->iqueue, u->RXDATA) < Q_OK)
        chnAddFlagsI(sdp, SD_OVERRUN_ERROR);
      chSysUnlockFromIsr();
    }
    
    if (u->INTSTAT & STAT_TXRDY) {
      msg_t b;

      chSysLockFromIsr();
      b = chOQGetI(&sdp->oqueue);
      chSysUnlockFromIsr();

      if (b < Q_OK) {
        u->INTENCLR = STAT_TXRDY;
        chSysLockFromIsr();
        chnAddFlagsI(sdp, CHN_OUTPUT_EMPTY);
        chSysUnlockFromIsr();
        break;
      }
      else {
         u->TXDATA = b;
      }
    }
    
    if (u->INTSTAT & (STAT_OVERRUN | STAT_DELTARXBRK |
                      STAT_FRAMERR | STAT_PARITYERR) ) {
      set_error(sdp, u->STAT);
    }

  }
}

/**
 * @brief   Attempts a TX preload.
 */
static void preload(SerialDriver *sdp) {
  LPC_USART_TypeDef *u = sdp->uart;

  if (u->STAT & STAT_TXIDLE) {
    msg_t b = chOQGetI(&sdp->oqueue);
    if (b < Q_OK) {
      chnAddFlagsI(sdp, CHN_OUTPUT_EMPTY);
      return;
    }
    u->TXDATA = b;
  }
  u->INTENSET = STAT_TXRDY;
}

/**
 * @brief   Driver output notification.
 */
#if LPC8xx_SERIAL_USE_UART0 || defined(__DOXYGEN__)
static void notify1(GenericQueue *qp) {

  (void)qp;
  preload(&SD1);
}
#endif

#if LPC8xx_SERIAL_USE_UART1 || defined(__DOXYGEN__)
static void notify2(GenericQueue *qp) {

  (void)qp;
  preload(&SD2);
}
#endif

#if LPC8xx_SERIAL_USE_UART2 || defined(__DOXYGEN__)
static void notify3(GenericQueue *qp) {

  (void)qp;
  preload(&SD3);
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   UARTn IRQ handlers.
 *
 * @isr
 */
#if LPC8xx_SERIAL_USE_UART0 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(Vector4C) {

  CH_IRQ_PROLOGUE();
  serve_interrupt( &SD1 );
  CH_IRQ_EPILOGUE();
}
#endif
#if LPC8xx_SERIAL_USE_UART1 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(Vector50) {

  CH_IRQ_PROLOGUE();
  serve_interrupt( &SD2 );
  CH_IRQ_EPILOGUE();
}
#endif
#if LPC8xx_SERIAL_USE_UART2 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(Vector54) {

  CH_IRQ_PROLOGUE();
  serve_interrupt( &SD3 );
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

#if LPC8xx_SERIAL_USE_UART0
  sdObjectInit(&SD1, NULL, notify1);
  SD1.uart = LPC_USART0;
#endif

#if LPC8xx_SERIAL_USE_UART1
  sdObjectInit(&SD2, NULL, notify2);
  SD2.uart = LPC_USART1;
#endif

#if LPC8xx_SERIAL_USE_UART2
  sdObjectInit(&SD3, NULL, notify3);
  SD3.uart = LPC_USART2;
#endif

  /* Reset fractional baudrate generator */
  LPC_SYSCON->PRESETCTRL &= ~(1<<2);
  LPC_SYSCON->PRESETCTRL |=  (1<<2);

  LPC_SYSCON->UARTCLKDIV  = LPC8xx_SERIAL_UARTCLKDIV;
  LPC_SYSCON->UARTFRGDIV  = LPC8xx_SERIAL_UARTFRGDIV;
  LPC_SYSCON->UARTFRGMULT = LPC8xx_SERIAL_UARTFRGMULT;

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
void sd_lld_start( SerialDriver *sdp, const SerialConfig *config ) {

  if (config == NULL)
    config = &default_config;

  if (sdp->state == SD_STOP) {
  
#if LPC8xx_SERIAL_USE_UART0
    if (&SD1 == sdp) {
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<14);           // Enable Clk
      LPC_SYSCON->PRESETCTRL &= ~(1<<3);              // Reset peripheral
      LPC_SYSCON->PRESETCTRL |=  (1<<3);
      nvicEnableVector(UART0_IRQn,
                       CORTEX_PRIORITY_MASK(LPC8xx_SERIAL_UART0_IRQ_PRIORITY));
    }
#endif

#if LPC8xx_SERIAL_USE_UART1
    if (&SD2 == sdp) {
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<15);           // Enable Clk
      LPC_SYSCON->PRESETCTRL &= ~(1<<4);              // Reset peripheral
      LPC_SYSCON->PRESETCTRL |=  (1<<4);
      nvicEnableVector(UART1_IRQn,
                       CORTEX_PRIORITY_MASK(LPC8xx_SERIAL_UART1_IRQ_PRIORITY));
    }
#endif

#if LPC8xx_SERIAL_USE_UART2
    if (&SD3 == sdp) {
      LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);           // Enable Clk
      LPC_SYSCON->PRESETCTRL &= ~(1<<5);              // Reset peripheral
      LPC_SYSCON->PRESETCTRL |=  (1<<5);
      nvicEnableVector(UART2_IRQn,
                       CORTEX_PRIORITY_MASK(LPC8xx_SERIAL_UART2_IRQ_PRIORITY));
    }
#endif

  }

  sdp->uart->BRG = (LPC8xx_SERIAL_U_PCLK / (config->sc_speed << 4)) -1;
  sdp->uart->INTENSET = STAT_FRAMERR | STAT_OVERRUN |
                        STAT_PARITYERR | STAT_DELTARXBRK |
                        STAT_RXRDY; 
  sdp->uart->CFG = config->sc_cfg | CFG_ENA;
}

/**
 * @brief   Low level serial driver stop.
 * @details De-initializes the UART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @notapi
 */
void sd_lld_stop( SerialDriver *sdp ) {

  if (sdp->state == SD_READY) {
    sdp->uart->INTENCLR = STAT_TXRDY | STAT_RXRDY; 
    sdp->uart->CFG = 0;

#if LPC8xx_SERIAL_USE_UART0
    if (&SD1 == sdp) {
      LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<12);
      nvicDisableVector(UART0_IRQn);
      return;
    }
#endif

#if LPC8xx_SERIAL_USE_UART1
    if (&SD2 == sdp) {
      LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<13);
      nvicDisableVector(UART1_IRQn);
      return;
    }
#endif

#if LPC8xx_SERIAL_USE_UART2
    if (&SD3 == sdp) {
      LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<14);
      nvicDisableVector(UART2_IRQn);
      return;
    }
#endif

  }
}

#endif /* HAL_USE_SERIAL */

/** @} */

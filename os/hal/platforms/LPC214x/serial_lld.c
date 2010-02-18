/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

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
 * @file LPC214x/serial_lld.c
 * @brief LPC214x low level serial driver code.
 * @addtogroup LPC214x_SERIAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if USE_LPC214x_UART0 || defined(__DOXYGEN__)
/** @brief UART0 serial driver identifier.*/
SerialDriver SD1;
#endif

#if USE_LPC214x_UART1 || defined(__DOXYGEN__)
/** @brief UART1 serial driver identifier.*/
SerialDriver SD2;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/** @brief Driver default configuration.*/
static const SerialConfig default_config = {
  SERIAL_DEFAULT_BITRATE,
  LCR_WL8 | LCR_STOP1 | LCR_NOPARITY,
  FCR_TRIGGER0
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief UART initialization.
 *
 * @param[in] sdp communication channel associated to the UART
 */
static void uart_init(SerialDriver *sdp) {
  UART *u = sdp->sd.uart;

  uint32_t div = PCLK / (sdp->sd.config->sc_speed << 4);
  u->UART_LCR = sdp->sd.config->sc_lcr | LCR_DLAB;
  u->UART_DLL = div;
  u->UART_DLM = div >> 8;
  u->UART_LCR = sdp->sd.config->sc_lcr;
  u->UART_FCR = FCR_ENABLE | FCR_RXRESET | FCR_TXRESET | sdp->sd.config->sc_fcr;
  u->UART_ACR = 0;
  u->UART_FDR = 0x10;
  u->UART_TER = TER_ENABLE;
  u->UART_IER = IER_RBR | IER_STATUS;
}

/**
 * @brief UART de-initialization.
 *
 * @param[in] u pointer to an UART I/O block
 */
static void uart_deinit(UART *u) {

  u->UART_LCR = LCR_DLAB;
  u->UART_DLL = 1;
  u->UART_DLM = 0;
  u->UART_LCR = 0;
  u->UART_FDR = 0x10;
  u->UART_IER = 0;
  u->UART_FCR = FCR_RXRESET | FCR_TXRESET;
  u->UART_ACR = 0;
  u->UART_TER = TER_ENABLE;
}

/**
 * @brief Error handling routine.
 *
 * @param[in] sdp communication channel associated to the UART
 * @param[in] err UART LSR register value
 */
static void set_error(SerialDriver *sdp, IOREG32 err) {
  sdflags_t sts = 0;

  if (err & LSR_OVERRUN)
    sts |= SD_OVERRUN_ERROR;
  if (err & LSR_PARITY)
    sts |= SD_PARITY_ERROR;
  if (err & LSR_FRAMING)
    sts |= SD_FRAMING_ERROR;
  if (err & LSR_BREAK)
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
 * @param[in] u pointer to an UART I/O block
 * @param[in] sdp communication channel associated to the UART
 * @note Tries hard to clear all the pending interrupt sources, we dont want to
 *       go through the whole ISR and have another interrupt soon after.
 */
static void serve_interrupt(SerialDriver *sdp) {
  UART *u = sdp->sd.uart;

  while (TRUE) {
    switch (u->UART_IIR & IIR_SRC_MASK) {
    case IIR_SRC_NONE:
      return;
    case IIR_SRC_ERROR:
      set_error(sdp, u->UART_LSR);
      break;
    case IIR_SRC_TIMEOUT:
    case IIR_SRC_RX:
      chSysLockFromIsr();
      if (chIQIsEmpty(&sdp->sd.iqueue))
        chEvtBroadcastI(&sdp->bac.ievent);
      chSysUnlockFromIsr();
      while (u->UART_LSR & LSR_RBR_FULL) {
        chSysLockFromIsr();
        if (chIQPutI(&sdp->sd.iqueue, u->UART_RBR) < Q_OK)
           sdAddFlagsI(sdp, SD_OVERRUN_ERROR);
        chSysUnlockFromIsr();
      }
      break;
    case IIR_SRC_TX:
      {
        int i = LPC214x_UART_FIFO_PRELOAD;
        do {
          msg_t b;

          chSysLockFromIsr();
          b = chOQGetI(&sdp->sd.oqueue);
          chSysUnlockFromIsr();
          if (b < Q_OK) {
            u->UART_IER &= ~IER_THRE;
            chSysLockFromIsr();
            chEvtBroadcastI(&sdp->bac.oevent);
            chSysUnlockFromIsr();
            break;
          }
          u->UART_THR = b;
        } while (--i);
      }
      break;
    default:
      (void) u->UART_THR;
      (void) u->UART_RBR;
    }
  }
}

/**
 * @brief Attempts a TX FIFO preload.
 */
static void preload(SerialDriver *sdp) {
  UART *u = sdp->sd.uart;

  if (u->UART_LSR & LSR_THRE) {
    int i = LPC214x_UART_FIFO_PRELOAD;
    do {
      msg_t b = chOQGetI(&sdp->sd.oqueue);
      if (b < Q_OK) {
        chEvtBroadcastI(&sdp->bac.oevent);
        return;
      }
      u->UART_THR = b;
    } while (--i);
  }
  u->UART_IER |= IER_THRE;
}

/**
 * @brief Driver SD1 output notification.
 */
#if USE_LPC214x_UART0 || defined(__DOXYGEN__)
static void notify1(void) {

  preload(&SD1);
}
#endif


/**
 * @brief Driver SD2 output notification.
 */
#if USE_LPC214x_UART1 || defined(__DOXYGEN__)
static void notify2(void) {

  preload(&SD2);
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief UART0 IRQ handler.
 */
#if USE_LPC214x_UART0 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(UART0IrqHandler) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD1);
  VICVectAddr = 0;

  CH_IRQ_EPILOGUE();
}
#endif


/**
 * @brief UART1 IRQ handler.
 */
#if USE_LPC214x_UART1 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(UART1IrqHandler) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD2);
  VICVectAddr = 0;

  CH_IRQ_EPILOGUE();
}
#endif


/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * Low level serial driver initialization.
 */
void sd_lld_init(void) {

#if USE_LPC214x_UART0
  sdObjectInit(&SD1, NULL, notify1);
  SD1.sd.uart = U0Base;
  SetVICVector(UART0IrqHandler, LPC214x_UART0_PRIORITY, SOURCE_UART0);
#endif
#if USE_LPC214x_UART1
  sdObjectInit(&SD2, NULL, notify2);
  SD2.sd.uart = U1Base;
  SetVICVector(UART1IrqHandler, LPC214x_UART1_PRIORITY, SOURCE_UART1);
#endif
}

/**
 * @brief Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp pointer to a @p SerialDriver object
 */
void sd_lld_start(SerialDriver *sdp) {

  if (sdp->sd.config == NULL)
    sdp->sd.config = &default_config;

  if (sdp->sd.state == SD_STOP) {
#if USE_LPC214x_UART0
    if (&SD1 == sdp) {
      PCONP = (PCONP & PCALL) | PCUART0;
      VICIntEnable = INTMASK(SOURCE_UART0);
    }
#endif
#if USE_LPC214x_UART1
    if (&SD2 == sdp) {
      PCONP = (PCONP & PCALL) | PCUART1;
      VICIntEnable = INTMASK(SOURCE_UART1);
    }
#endif
  }
  uart_init(sdp);
}

/**
 * @brief Low level serial driver stop.
 * @details De-initializes the UART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp pointer to a @p SerialDriver object
 */
void sd_lld_stop(SerialDriver *sdp) {

  if (sdp->sd.state == SD_READY) {
    uart_deinit(sdp->sd.uart);
#if USE_LPC214x_UART0
    if (&SD1 == sdp) {
      PCONP = (PCONP & PCALL) & ~PCUART0;
      VICIntEnClear = INTMASK(SOURCE_UART0);
      return;
    }
#endif
#if USE_LPC214x_UART1
    if (&SD2 == sdp) {
      PCONP = (PCONP & PCALL) & ~PCUART1;
      VICIntEnClear = INTMASK(SOURCE_UART1);
      return;
    }
#endif
  }
}

#endif /* CH_HAL_USE_SERIAL */

/** @} */

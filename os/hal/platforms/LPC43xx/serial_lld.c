/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    LPC43xx/serial_lld.c
 * @brief   LPC43xx low level serial driver code.
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

#if LPC_SERIAL_USE_UART0 || defined(__DOXYGEN__)
/** @brief UART0 serial driver identifier.*/
SerialDriver SD1;
#endif

#if LPC_SERIAL_USE_UART1 || defined(__DOXYGEN__)
/** @brief UART1 serial driver identifier.*/
SerialDriver SD2;
#endif

#if LPC_SERIAL_USE_UART2 || defined(__DOXYGEN__)
/** @brief UART2 serial driver identifier.*/
SerialDriver SD3;
#endif

#if LPC_SERIAL_USE_UART3 || defined(__DOXYGEN__)
/** @brief UART3 serial driver identifier.*/
SerialDriver SD4;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
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
 * @brief   UART initialization.
 *
 * @param[in] sdp       communication channel associated to the UART
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void uart_init(SerialDriver *sdp, const SerialConfig *config) {
  LPC_USARTn_Type *u = sdp->uart;
  uint32_t div = 0;

#if LPC_SERIAL_USE_UART0
  if (&SD1 == sdp) {
    div = LPC_SERIAL_UART0_PCLK / (config->sc_speed << 4);
  }
#endif

#if LPC_SERIAL_USE_UART1
  if (&SD2 == sdp) {
    div = LPC_SERIAL_UART1_PCLK / (config->sc_speed << 4);
  }
#endif

#if LPC_SERIAL_USE_UART2
  if (&SD3 == sdp) {
    div = LPC_SERIAL_UART2_PCLK / (config->sc_speed << 4);
  }
#endif

#if LPC_SERIAL_USE_UART3
  if (&SD4 == sdp) {
    div = LPC_SERIAL_UART3_PCLK / (config->sc_speed << 4);
  }
#endif

  u->LCR = config->sc_lcr | LCR_DLAB;
  u->DLL = div;
  u->DLM = div >> 8;
  u->LCR = config->sc_lcr;
  u->FCR = FCR_ENABLE | FCR_RXRESET | FCR_TXRESET | config->sc_fcr;
  u->ACR = 0;
  u->FDR = 0x10;
  u->TER = TER_ENABLE;
  u->IER = IER_RBR | IER_STATUS;
}

/**
 * @brief   UART de-initialization.
 *
 * @param[in] u         pointer to an UART I/O block
 */
static void uart_deinit(LPC_USARTn_Type *u) {

  u->LCR = LCR_DLAB;
  u->DLL = 1;
  u->DLM = 0;
  u->LCR = 0;
  u->FDR = 0x10;
  u->IER = 0;
  u->FCR = FCR_RXRESET | FCR_TXRESET;
  u->ACR = 0;
  u->TER = TER_ENABLE;
}

/**
 * @brief   Error handling routine.
 *
 * @param[in] sdp       communication channel associated to the UART
 * @param[in] err       UART LSR register value
 */
static void set_error(SerialDriver *sdp, IOREG32 err) {
  flagsmask_t sts = 0;

  if (err & LSR_OVERRUN)
    sts |= SD_OVERRUN_ERROR;
  if (err & LSR_PARITY)
    sts |= SD_PARITY_ERROR;
  if (err & LSR_FRAMING)
    sts |= SD_FRAMING_ERROR;
  if (err & LSR_BREAK)
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
  LPC_USARTn_Type *u = sdp->uart;

  while (TRUE) {
    switch (u->IIR & IIR_SRC_MASK) {
    case IIR_SRC_NONE:
      return;
    case IIR_SRC_ERROR:
      set_error(sdp, u->LSR);
      break;
    case IIR_SRC_TIMEOUT:
    case IIR_SRC_RX:
      chSysLockFromIsr();
      if (chIQIsEmptyI(&sdp->iqueue))
        chnAddFlagsI(sdp, CHN_INPUT_AVAILABLE);
      chSysUnlockFromIsr();
      while (u->LSR & LSR_RBR_FULL) {
        chSysLockFromIsr();
        if (chIQPutI(&sdp->iqueue, u->RBR) < Q_OK)
          chnAddFlagsI(sdp, SD_OVERRUN_ERROR);
        chSysUnlockFromIsr();
      }
      break;
    case IIR_SRC_TX:
      {
        int i = LPC_SERIAL_FIFO_PRELOAD;
        do {
          msg_t b;

          chSysLockFromIsr();
          b = chOQGetI(&sdp->oqueue);
          chSysUnlockFromIsr();
          if (b < Q_OK) {
            u->IER &= ~IER_THRE;
            chSysLockFromIsr();
            chnAddFlagsI(sdp, CHN_OUTPUT_EMPTY);
            chSysUnlockFromIsr();
            break;
          }
          u->THR = b;
        } while (--i);
      }
      break;
    default:
      (void) u->THR;
      (void) u->RBR;
    }
  }
}

/**
 * @brief   Attempts a TX FIFO preload.
 */
static void preload(SerialDriver *sdp) {
  LPC_USARTn_Type *u = sdp->uart;

  if (u->LSR & LSR_THRE) {
    int i = LPC_SERIAL_FIFO_PRELOAD;
    do {
      msg_t b = chOQGetI(&sdp->oqueue);
      if (b < Q_OK) {
        chnAddFlagsI(sdp, CHN_OUTPUT_EMPTY);
        return;
      }
      u->THR = b;
    } while (--i);
  }
  u->IER |= IER_THRE;
}

/**
 * @brief   Driver SD1 output notification.
 */
#if LPC_SERIAL_USE_UART0 || defined(__DOXYGEN__)
static void notify1(GenericQueue *qp) {

  (void)qp;
  preload(&SD1);
}
#endif

/**
 * @brief   Driver SD2 output notification.
 */
#if LPC_SERIAL_USE_UART1 || defined(__DOXYGEN__)
static void notify2(GenericQueue *qp) {

  (void)qp;
  preload(&SD2);
}
#endif

/**
 * @brief   Driver SD3 output notification.
 */
#if LPC_SERIAL_USE_UART2 || defined(__DOXYGEN__)
static void notify3(GenericQueue *qp) {

  (void)qp;
  preload(&SD3);
}
#endif

/**
 * @brief   Driver SD4 output notification.
 */
#if LPC_SERIAL_USE_UART3 || defined(__DOXYGEN__)
static void notify4(GenericQueue *qp) {

  (void)qp;
  preload(&SD4);
}
#endif
/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   UART0 IRQ handler.
 *
 * @isr
 */
#if LPC_SERIAL_USE_UART0 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(VectorA0) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD1);

  CH_IRQ_EPILOGUE();
}
#endif

/**
 * @brief   UART1 IRQ handler.
 *
 * @isr
 */
#if LPC_SERIAL_USE_UART1 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(VectorA4) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD2);

  CH_IRQ_EPILOGUE();
}
#endif

/**
 * @brief   UART2 IRQ handler.
 *
 * @isr
 */
#if LPC_SERIAL_USE_UART2 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(VectorA8) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD3);

  CH_IRQ_EPILOGUE();
}
#endif

/**
 * @brief   UART3 IRQ handler.
 *
 * @isr
 */
#if LPC_SERIAL_USE_UART3 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(VectorAC) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD4);

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

#if LPC_SERIAL_USE_UART0
  sdObjectInit(&SD1, NULL, notify1);
  SD1.uart = ( LPC_USARTn_Type*) LPC_USART0;
#endif

#if LPC_SERIAL_USE_UART1
  sdObjectInit(&SD2, NULL, notify2);
  SD2.uart = ( LPC_USARTn_Type*) LPC_UART1;
#endif

#if LPC_SERIAL_USE_UART2
  sdObjectInit(&SD3, NULL, notify3);
  SD3.uart = ( LPC_USARTn_Type*) LPC_USART2;
#endif

#if LPC_SERIAL_USE_UART3
  sdObjectInit(&SD4, NULL, notify4);
  SD4.uart = ( LPC_USARTn_Type*) LPC_USART3;
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
#if LPC_SERIAL_USE_UART0
    if (&SD1 == sdp) {
      LPC_CCU2->CLK_APB0_USART0_CFG = 1;
      nvicEnableVector(USART0_IRQn,
                       CORTEX_PRIORITY_MASK(LPC_SERIAL_UART0_IRQ_PRIORITY));
    }
#endif

#if LPC_SERIAL_USE_UART1
    if (&SD2 == sdp) {
      LPC_CCU2->CLK_APB0_UART1_CFG = 1;
      nvicEnableVector(UART1_IRQn,
                       CORTEX_PRIORITY_MASK(LPC_SERIAL_UART1_IRQ_PRIORITY));
    }
#endif

#if LPC_SERIAL_USE_UART2
    if (&SD3 == sdp) {
      LPC_CCU2->CLK_APB2_USART2_CFG = 1;
      nvicEnableVector(USART2_IRQn,
                       CORTEX_PRIORITY_MASK(LPC_SERIAL_UART2_IRQ_PRIORITY));
    }
#endif

#if LPC_SERIAL_USE_UART3
    if (&SD4 == sdp) {
      LPC_CCU2->CLK_APB2_USART3_CFG = 1;
      nvicEnableVector(USART3_IRQn,
                       CORTEX_PRIORITY_MASK(LPC_SERIAL_UART3_IRQ_PRIORITY));
    }
#endif
  }
  uart_init(sdp, config);
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
void sd_lld_stop(SerialDriver *sdp) {

  if (sdp->state == SD_READY) {
    uart_deinit(sdp->uart);
#if LPC_SERIAL_USE_UART0
    if (&SD1 == sdp) {
      LPC_CCU2->CLK_APB0_USART0_CFG = 0;
      nvicDisableVector(USART0_IRQn);
      return;
    }
#endif

#if LPC_SERIAL_USE_UART1
    if (&SD2 == sdp) {
      LPC_CCU2->CLK_APB0_UART1_CFG = 0;
      nvicDisableVector(UART1_IRQn);
      return;
    }
#endif

#if LPC_SERIAL_USE_UART2
    if (&SD3 == sdp) {
      LPC_CCU2->CLK_APB2_USART2_CFG = 0;
      nvicDisableVector(USART2_IRQn);
      return;
    }
#endif

#if LPC_SERIAL_USE_UART3
    if (&SD4 == sdp) {
      LPC_CCU2->CLK_APB2_USART3_CFG = 0;
      nvicDisableVector(USART3_IRQn);
      return;
    }
#endif
  }
}

#endif /* HAL_USE_SERIAL */

/** @} */

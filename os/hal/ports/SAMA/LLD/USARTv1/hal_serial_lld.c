/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    USARTv1/hal_serial_lld.c
 * @brief   SAMA low level serial driver code.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "hal.h"

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief UART0 serial driver identifier.*/
#if SAMA_SERIAL_USE_UART0 || defined(__DOXYGEN__)
SerialDriver SD0;
#endif

/** @brief UART1 serial driver identifier.*/
#if SAMA_SERIAL_USE_UART1 || defined(__DOXYGEN__)
SerialDriver SD1;
#endif

/** @brief UART2 serial driver identifier.*/
#if SAMA_SERIAL_USE_UART2 || defined(__DOXYGEN__)
SerialDriver SD3;
#endif

/** @brief UART3 serial driver identifier.*/
#if SAMA_SERIAL_USE_UART3 || defined(__DOXYGEN__)
SerialDriver SD3;
#endif

/** @brief UART4 serial driver identifier.*/
#if SAMA_SERIAL_USE_UART4 || defined(__DOXYGEN__)
SerialDriver SD4;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/** @brief Driver default configuration.*/
static const SerialConfig default_config =
{
  SERIAL_DEFAULT_BITRATE,
  0,
  UART_MR_PAR_NO
};

#if SAMA_SERIAL_USE_UART0 || defined(__DOXYGEN__)
/** @brief Input buffer for SD0.*/
static uint8_t sd_in_buf0[SAMA_SERIAL_UART0_IN_BUF_SIZE];

/** @brief Output buffer for SD0.*/
static uint8_t sd_out_buf0[SAMA_SERIAL_UART0_OUT_BUF_SIZE];
#endif

#if SAMA_SERIAL_USE_UART1 || defined(__DOXYGEN__)
/** @brief Input buffer for SD1.*/
static uint8_t sd_in_buf1[SAMA_SERIAL_UART1_IN_BUF_SIZE];

/** @brief Output buffer for SD1.*/
static uint8_t sd_out_buf1[SAMA_SERIAL_UART1_OUT_BUF_SIZE];
#endif

#if SAMA_SERIAL_USE_UART2 || defined(__DOXYGEN__)
/** @brief Input buffer for SD2.*/
static uint8_t sd_in_buf2[SAMA_SERIAL_UART2_IN_BUF_SIZE];

/** @brief Output buffer for SD2.*/
static uint8_t sd_out_buf2[SAMA_SERIAL_UART2_OUT_BUF_SIZE];
#endif

#if SAMA_SERIAL_USE_UART3 || defined(__DOXYGEN__)
/** @brief Input buffer for SD3.*/
static uint8_t sd_in_buf3[SAMA_SERIAL_UART3_IN_BUF_SIZE];

/** @brief Output buffer for SD3.*/
static uint8_t sd_out_buf3[SAMA_SERIAL_UART3_IN_BUF_SIZE];
#endif

#if SAMA_SERIAL_USE_UART4 || defined(__DOXYGEN__)
/** @brief Input buffer for SD4.*/
static uint8_t sd_in_buf4[SAMA_SERIAL_UART4_IN_BUF_SIZE];

/** @brief Output buffer for SD4.*/
static uint8_t sd_out_buf4[SAMA_SERIAL_UART4_IN_BUF_SIZE];
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   UART initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void uart_init(SerialDriver *sdp, const SerialConfig *config) {
  Uart *u = sdp->uart;

  /* Baud rate setting.*/
  u->UART_BRGR = UART_BRGR_CD(sdp->clock / (16 * config->speed));

  u->UART_CR = config->cr;
  u->UART_MR = config->mr;
  u->UART_IER = UART_IER_RXRDY;

  /* Clearing error status bit */
  u->UART_CR |= UART_CR_RSTSTA;
  /* Enabling Tx and Rx */
  u->UART_CR |= UART_CR_RXEN | UART_CR_TXEN;

}

/**
 * @brief   USART de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] u         pointer to an UART I/O block
 */
static void uart_deinit(Uart *u) {

  u->UART_CR = 0;
  u->UART_MR = 0;

}

/**
 * @brief   Error handling routine.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] isr       USART ISR register value
 */
static void set_error(SerialDriver *sdp, uint32_t isr) {
  eventflags_t sts = 0;

  if (isr & UART_SR_OVRE)
    sts |= SD_OVERRUN_ERROR;
  if (isr & UART_SR_PARE)
    sts |= SD_PARITY_ERROR;
  if (isr & UART_SR_FRAME)
    sts |= UART_SR_FRAME;
  osalSysLockFromISR();
  chnAddFlagsI(sdp, sts);
  osalSysUnlockFromISR();
}

/**
 * @brief   Common IRQ handler.
 *
 * @param[in] sdp       communication channel associated to the UART
 */
static void serve_interrupt(SerialDriver *sdp) {
  Uart *u = sdp->uart;
  uint32_t imr = u->UART_IMR;
  uint32_t sr;

  /* Reading and clearing status.*/
  sr = u->UART_SR;
  u->UART_CR |= UART_CR_RSTSTA;

  /* Error condition detection.*/
  if (sr & (UART_SR_OVRE | UART_SR_FRAME  | UART_SR_PARE)){
    set_error(sdp, sr);
  }

  /* Data available.*/
  if (sr & UART_SR_RXRDY) {
    osalSysLockFromISR();
    sdIncomingDataI(sdp, (uint8_t)u->UART_RHR);
    osalSysUnlockFromISR();
  }

  /* Transmission buffer empty.*/
  if ((imr & UART_IMR_TXRDY) && (sr & UART_SR_TXRDY)) {
    msg_t b;
    osalSysLockFromISR();
    b = oqGetI(&sdp->oqueue);
    if (b < MSG_OK) {
      chnAddFlagsI(sdp, CHN_OUTPUT_EMPTY);
      u->UART_IDR |= UART_IDR_TXRDY;
      u->UART_IER = UART_IER_TXEMPTY;
    }
    else
      u->UART_THR = b;
    osalSysUnlockFromISR();
  }

  /* Physical transmission end.*/
  if ((imr & UART_SR_TXEMPTY) && (sr & (UART_SR_TXRDY | UART_SR_TXEMPTY))) {
    osalSysLockFromISR();
    if (oqIsEmptyI(&sdp->oqueue))
      chnAddFlagsI(sdp, CHN_TRANSMISSION_END);
    u->UART_IDR |= UART_IDR_TXRDY | UART_IDR_TXEMPTY;
    osalSysUnlockFromISR();
  }
}

#if SAMA_SERIAL_USE_UART0 || defined(__DOXYGEN__)
static void notify0(io_queue_t *qp) {

  (void)qp;
  UART0->UART_IER |= UART_IER_TXRDY;
}
#endif

#if SAMA_SERIAL_USE_UART1 || defined(__DOXYGEN__)
static void notify1(io_queue_t *qp) {

  (void)qp;
  UART1->UART_IER |= UART_IER_TXRDY;
}
#endif

#if SAMA_SERIAL_USE_UART2 || defined(__DOXYGEN__)
static void notify2(io_queue_t *qp) {

  (void)qp;
  UART2->UART_IER |= UART_IER_TXRDY;
}
#endif

#if SAMA_SERIAL_USE_UART3 || defined(__DOXYGEN__)
static void notify3(io_queue_t *qp) {

  (void)qp;
  UART3->UART_IER |= UART_IER_TXRDY;
}
#endif

#if SAMA_SERIAL_USE_UART4 || defined(__DOXYGEN__)
static void notify4(io_queue_t *qp) {

  (void)qp;
  UART4->UART_IER |= UART_IER_TXRDY;
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if SAMA_SERIAL_USE_UART0 || defined(__DOXYGEN__)
/**
 * @brief   UART0 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(SAMA_UART0_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  serve_interrupt(&SD0);
  aicAckInt();
  OSAL_IRQ_EPILOGUE();
}
#endif

#if SAMA_SERIAL_USE_UART1 || defined(__DOXYGEN__)
/**
 * @brief   UART1 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(SAMA_UART1_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  serve_interrupt(&SD1);
  aicAckInt();
  OSAL_IRQ_EPILOGUE();
}
#endif

#if SAMA_SERIAL_USE_UART2 || defined(__DOXYGEN__)
/**
 * @brief   UART2 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(SAMA_UART2_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  serve_interrupt(&SD2);
  aicAckInt();
  OSAL_IRQ_EPILOGUE();
}
#endif

#if SAMA_SERIAL_USE_UART3 || defined(__DOXYGEN__)
/**
 * @brief   UART3 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(SAMA_UART3_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  serve_interrupt(&SD3);
  aicAckInt();
  OSAL_IRQ_EPILOGUE();
}
#endif

#if SAMA_SERIAL_USE_UART4 || defined(__DOXYGEN__)
/**
 * @brief   UART4 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(SAMA_UART4_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  serve_interrupt(&SD4);
  aicAckInt();
  OSAL_IRQ_EPILOGUE();
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

#if SAMA_SERIAL_USE_UART0
  sdObjectInit(&SD0);
  iqObjectInit(&SD0.iqueue, sd_in_buf0, sizeof sd_in_buf0, NULL, &SD0);
  oqObjectInit(&SD0.oqueue, sd_out_buf0, sizeof sd_out_buf0, notify0, &SD0);
  SD0.uart = UART0;
  SD0.clock = SAMA_UART0CLK;

  aicSetSourcePriority(ID_UART0, SAMA_SERIAL_UART0_PRIORITY);
  aicSetSourceHandler(ID_UART0, SAMA_UART0_HANDLER);
  aicEnableInt(ID_UART0);
#endif

#if SAMA_SERIAL_USE_UART1
  sdObjectInit(&SD1);
  iqObjectInit(&SD1.iqueue, sd_in_buf1, sizeof sd_in_buf1, NULL, &SD1);
  oqObjectInit(&SD1.oqueue, sd_out_buf1, sizeof sd_out_buf1, notify1, &SD1);
  SD1.uart = UART1;
  SD1.clock = SAMA_UART1CLK;

  aicSetSourcePriority(ID_UART1, SAMA_SERIAL_UART1_PRIORITY);
  aicSetSourceHandler(ID_UART1, SAMA_UART1_HANDLER);
  aicEnableInt(ID_UART1);
#endif

#if SAMA_SERIAL_USE_UART2
  sdObjectInit(&SD2);
  iqObjectInit(&SD2.iqueue, sd_in_buf2, sizeof sd_in_buf2, NULL, &SD2);
  oqObjectInit(&SD2.oqueue, sd_out_buf2, sizeof sd_out_buf2, notify2, &SD2);
  SD2.uart = UART2;
  SD2.clock = SAMA_UART2CLK;

  aicSetSourcePriority(ID_UART2, SAMA_SERIAL_UART2_PRIORITY);
  aicSetSourceHandler(ID_UART2, SAMA_UART2_HANDLER);
  aicEnableInt(ID_UART2);
#endif

#if SAMA_SERIAL_USE_UART3
  sdObjectInit(&SD3);
  iqObjectInit(&SD3.iqueue, sd_in_buf3, sizeof sd_in_buf3, NULL, &SD3);
  oqObjectInit(&SD3.oqueue, sd_out_buf3, sizeof sd_out_buf3, notify3, &SD3);
  SD3.uart = UART3;
  SD3.clock = SAMA_UART3CLK;

  aicSetSourcePriority(ID_UART3, SAMA_SERIAL_UART3_PRIORITY);
  aicSetSourceHandler(ID_UART3, SAMA_UART3_HANDLER);
  aicEnableInt(ID_UART3);
#endif

#if SAMA_SERIAL_USE_UART4
  sdObjectInit(&SD4);
  iqObjectInit(&SD4.iqueue, sd_in_buf4, sizeof sd_in_buf4, NULL, &SD4);
  oqObjectInit(&SD4.oqueue, sd_out_buf4, sizeof sd_out_buf4, notify4, &SD4);
  SD4.uart = UART4;
  SD4.clock = SAMA_UART4CLK;

  aicSetSourcePriority(ID_UART4, SAMA_SERIAL_UART4_PRIORITY);
  aicSetSourceHandler(ID_UART4, SAMA_UART4_HANDLER);
  aicEnableInt(ID_UART4);
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
#if SAMA_SERIAL_USE_UART0
    if (&SD0 == sdp) {
      pmcEnableUART0();
    }
#endif
#if SAMA_SERIAL_USE_UART1
    if (&SD1 == sdp) {
      pmcEnableUART1();
    }
#endif
#if SAMA_SERIAL_USE_UART2
    if (&SD2 == sdp) {
      pmcEnableUART2();
    }
#endif
#if SAMA_SERIAL_USE_UART3
    if (&SD3 == sdp) {
      pmcEnableUART3();
    }
#endif
#if SAMA_SERIAL_USE_UART4
    if (&SD4 == sdp) {
      pmcEnableUART4();
    }
#endif
  }
  uart_init(sdp, config);
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
    /* UART is de-initialized then clocks are disabled.*/
    uart_deinit(sdp->uart);

#if SAMA_SERIAL_USE_UART0
    if (&SD0 == sdp) {
      pmcDisableUART0();
      return;
    }
#endif
#if SAMA_SERIAL_USE_UART1
    if (&SD1 == sdp) {
      pmcDisableUART1();
      return;
    }
#endif
#if SAMA_SERIAL_USE_UART2
    if (&SD2 == sdp) {
      pmcDisableUART2();
      return;
    }
#endif
#if SAMA_SERIAL_USE_UART3
    if (&SD3 == sdp) {
      pmcDisableUART3();
      return;
    }
#endif
#if SAMA_SERIAL_USE_UART4
    if (&SD4 == sdp) {
      pmcDisableUART4();
      return;
    }
#endif
  }
}

#endif /* HAL_USE_SERIAL */

/** @} */

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
 * @file    RX62N/serial_lld.c
 * @brief   RX62N low level serial driver code.
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

#if RX62N_SERIAL_USE_UART0 || defined(__DOXYGEN__)
/** @brief UART0 serial driver identifier.*/
SerialDriver SD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/** @brief Driver default configuration.*/
static const SerialConfig default_config = {
  .sc_speed = SERIAL_DEFAULT_BITRATE,
  .sc_scr = 0, /* only bits 0-1 used */
  .sc_smr = 0,
  .sc_semr = 0,
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   UART initialization.
 *
 * @param[in] sdp       communication channel associated to the UART
 * @param[in] config    the architecture-dependent serial driver configuration
 * @todo Test all possible SMR and SEMR clock settings
 */
static void uart_init(SerialDriver *sdp, const SerialConfig *config) {
  volatile struct st_sci *u = sdp->uart;

  uint32_t brr;

  u->SCR.BYTE = config->sc_scr & 0x03;
  u->SMR.BYTE = config->sc_smr;
  u->SEMR.BYTE = config->sc_semr;
  brr = ( 32 << ( 2 * ( config->sc_smr & 3 ) ) ) >> ( ( config->sc_semr & 0x10 ) ? 1 : 0 );
  brr = RX62N_PERCLK / ( brr * config->sc_speed ) - 1;
  u->BRR = brr;
  /* TODO: delay 1-bit interval */
  u->SCR.BYTE |= 0x70;
}

/**
 * @brief   UART de-initialization.
 *
 * @param[in] u         pointer to an UART I/O block
 */
static void uart_deinit(volatile struct st_sci *u) {

  u->SCR.BYTE = 0;
  u->SMR.BYTE = 0;
  u->SEMR.BYTE = 0;
  u->BRR = 0;
}

/**
 * @brief   Error handling routine.
 *
 * @param[in] sdp       communication channel associated to the UART
 * @param[in] err       UART LSR register value
 */
static void set_error(SerialDriver *sdp, uint32_t err) {

  flagsmask_t sts = 0;

  if (err & SSR_ORER)
    sts |= SD_OVERRUN_ERROR;
  if (err & SSR_PER)
    sts |= SD_PARITY_ERROR;
  if (err & SSR_FER)
    sts |= SD_FRAMING_ERROR;
  chSysLockFromIsr();
  chnAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();

}

#if RX62N_SERIAL_USE_UART0 || defined(__DOXYGEN__)
/**
 * @brief   Error IRQ handler.
 *
 * @param[in] sdp       communication channel associated to the UART
 */
static void serve_interrupt_eri(SerialDriver *sdp) {
  volatile struct st_sci *u = sdp->uart;

  set_error(sdp, u->SSR.BYTE);
  u->SSR.BYTE &= ~((1<<3)|(1<<4)|(1<<5)); /* clear error flags */
}

/**
 * @brief   Receive IRQ handler.
 *
 * @param[in] sdp       communication channel associated to the UART
 */
static void serve_interrupt_rxi(SerialDriver *sdp) {
  volatile struct st_sci *u = sdp->uart;

  chSysLockFromIsr();
  sdIncomingDataI(sdp, u->RDR);
  chSysUnlockFromIsr();
}

/**
 * @brief   Transmit IRQ handler.
 *
 * @param[in] sdp       communication channel associated to the UART
 */
static void serve_interrupt_txi(SerialDriver *sdp) {
  volatile struct st_sci *u = sdp->uart;
  msg_t b;

  chSysLockFromIsr();
  if (!u->SSR.BIT.TDRE) {
    /* forced by notify */
    chSysUnlockFromIsr();
    return;
  }
  b = chOQGetI(&sdp->oqueue);
  chSysUnlockFromIsr();
  if (b < Q_OK) {
    u->SCR.BIT.TIE = 0;
    chSysLockFromIsr();
    chnAddFlagsI(sdp, CHN_OUTPUT_EMPTY);
    chSysUnlockFromIsr();
    return;
  }
  u->TDR = b;
}

/**
 * @brief   Transmission end IRQ handler.
 *
 * @param[in] sdp       communication channel associated to the UART
 */
static void serve_interrupt_tei(SerialDriver *sdp) {

  chSysLockFromIsr();
  chnAddFlagsI(sdp, CHN_TRANSMISSION_END);
  chSysUnlockFromIsr();
}

/**
 * @brief   Driver SD1 output notification.
 */
static void notify1(GenericQueue *qp) {

  (void)qp;
  SD1.uart->SCR.BIT.TIE = 1;
  IR(SCI0,TXI0) = 1;  /* Set Interrupt Enable Register */
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
#if RX62N_SERIAL_USE_UART0 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(Excep_SCI0_ERI0) {

  CH_IRQ_PROLOGUE();

  serve_interrupt_eri(&SD1);

  CH_IRQ_EPILOGUE();
}
CH_IRQ_HANDLER(Excep_SCI0_RXI0) {

  CH_IRQ_PROLOGUE();

  serve_interrupt_rxi(&SD1);

  CH_IRQ_EPILOGUE();
}
CH_IRQ_HANDLER(Excep_SCI0_TXI0) {

  CH_IRQ_PROLOGUE();

  serve_interrupt_txi(&SD1);

  CH_IRQ_EPILOGUE();
}
CH_IRQ_HANDLER(Excep_SCI0_TEI0) {

  CH_IRQ_PROLOGUE();

  serve_interrupt_tei(&SD1);

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

#if RX62N_SERIAL_USE_UART0
  sdObjectInit(&SD1, NULL, notify1);
  SD1.uart = &SCI0;
  PORT2.ICR.BIT.B1 = 1;
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
#if RX62N_SERIAL_USE_UART0
    if (&SD1 == sdp) {
      MSTP(SCI0) = 0; /* Enable SCI0 (cancel module stop state) */
      IEN(SCI0,RXI0) = 1;
      IEN(SCI0,TXI0) = 1;
      IEN(SCI0,ERI0) = 1;
      IPR(SCI0,TXI0) = RX62N_SERIAL_UART0_IRQ_PRIORITY;
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
#if RX62N_SERIAL_USE_UART0
    if (&SD1 == sdp) {
      IEN(SCI0,RXI0) = 0;
      IEN(SCI0,TXI0) = 0;
      IEN(SCI0,ERI0) = 0;
      MSTP(SCI0) = 1; /* Disable SCI0 (enter module stop state) */
      return;
    }
#endif
  }
}

#endif /* HAL_USE_SERIAL */

/** @} */

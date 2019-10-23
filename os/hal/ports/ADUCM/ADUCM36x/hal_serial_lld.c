/*
    ChibiOS - Copyright (C) 2019 Rocco Marco Guglielmi

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
 * @file    ADUCM36x/hal_serial_lld.c
 * @brief   ADUCM serial subsystem low level driver source.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "hal.h"

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @name    ADUCM UART Fractional BR Divider register's bitfields.
 * @{
 */
#define ADUCM_COMFBR_ENABLE                 COMFBR_ENABLE_EN
#define ADUCM_COMFBR_DIVM(m)                ((m) << 11U)
#define ADUCM_COMFBR_DIVN(n)                ((n) << 0U)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief USART1 serial driver identifier.*/
#if (ADUCM_SERIAL_USE_UART0 == TRUE) || defined(__DOXYGEN__)
SerialDriver SD0;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static const SerialConfig default_config = {
  SERIAL_DEFAULT_BITRATE,
  COMLCR_WLS_8BITS
};

#if ADUCM_SERIAL_USE_UART0 || defined(__DOXYGEN__)
/** @brief Input buffer for SD0.*/
static uint8_t sd_in_buf0[ADUCM_SERIAL_UART0_IN_BUF_SIZE];

/** @brief Output buffer for SD0.*/
static uint8_t sd_out_buf0[ADUCM_SERIAL_UART0_OUT_BUF_SIZE];
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   USART initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void uart_init(SerialDriver *sdp, const SerialConfig *config) {
  
  uint32_t comdiv, divn, divm;
  
  /* Reseting the UART state machine. */
  sdp->uart->COMCON = COMCON_DISABLE_EN;

  /* Baud rate setting.*/
  osalDbgAssert(((sdp->clock / 32U) > config->speed),
                "invalid baud rate vs input clock");
  
  /* The Baudrate comes from the formula:
     BR = (UDIV/DIV) / (32 * comdiv) / (M + N/2048)
     Whereas:
      - 0 < comdiv < 65536
      - 0 <= M < 4
      - 0 <= N < 2048
     The following computation is deduced from the fact the the maximum value 
     of M is 3 and that N/2048 is always smaller than 1.
     
     Thus, is it possible to find M and comdiv in an interative way increasing
     comdiv until M does not reaches an allowed value.

     Once founded comdiv and M it is possible to find N inverting the equation.
     Note that N only refines the value introducing a decimal part to the last 
     dividend.    
  */     
  comdiv = 1U;
  divm = sdp->clock / config->speed / 32U / comdiv;
  while(divm > 3U) {
    comdiv++;
    divm = sdp->clock / config->speed / 32U / comdiv;
  }
  
  osalDbgAssert((divm <= 3), "invalid divm value");
  osalDbgAssert((comdiv <= 65535U) && (comdiv >= 1), "invalid comdiv value");
  
  divn = (uint32_t)((((uint64_t)sdp->clock * 2048U) / config->speed / 32U / 
                      comdiv) - (divm * 2048U));
                      
  osalDbgAssert((divn <= 2047), "invalid divn value");
  
  sdp->uart->COMFBR = ADUCM_COMFBR_ENABLE | ADUCM_COMFBR_DIVM(divm) | 
                      ADUCM_COMFBR_DIVN(divn);
                      
  sdp->uart->COMDIV = comdiv;
  
  /* Line and modem configurations*/
  sdp->uart->COMLCR = config->lcr;
  
  sdp->uart->COMIEN = COMIEN_EDMAR_DIS | COMIEN_EDMAT_DIS | COMIEN_EDSSI_EN |
                      COMIEN_ELSI_EN | COMIEN_ETBEI_EN | COMIEN_ERBFI_EN;
  sdp->uart->COMCON = COMCON_DISABLE_DIS;
}

/**
 * @brief   USART de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void uart_deinit(SerialDriver *sdp) {

  sdp->uart->COMIEN = 0;
  sdp->uart->COMCON = COMCON_DISABLE_EN;
}

/**
 * @brief   Error handling routine.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] lsr       UART LSR register value
 */
static void set_error(SerialDriver *sdp, uint32_t lsr) {
  eventflags_t sts = 0;

  if (lsr & COMLSR_OE)
    sts |= SD_OVERRUN_ERROR;
  if (lsr & COMLSR_PE)
    sts |= SD_PARITY_ERROR;
  if (lsr & COMLSR_FE)
    sts |= SD_FRAMING_ERROR;
  osalSysLockFromISR();
  chnAddFlagsI(sdp, sts);
  osalSysUnlockFromISR();
}

#if ADUCM_SERIAL_USE_UART0 || defined(__DOXYGEN__)
static void notify0(io_queue_t *qp) {

  (void)qp;
  pADI_UART->COMIEN |= COMIEN_ETBEI_EN;
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if ADUCM_SERIAL_USE_UART0 || defined(__DOXYGEN__)
#if !defined(ADUCM_UART0_HANDLER)
#error "ADUCM_UART0_HANDLER not defined"
#endif
/**
 * @brief   UART0 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(ADUCM_UART0_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  sd_lld_serve_interrupt(&SD0);

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

#if ADUCM_SERIAL_USE_UART0 == TRUE
  sdObjectInit(&SD0);
  iqObjectInit(&SD0.iqueue, sd_in_buf0, sizeof sd_in_buf0, NULL, &SD0);
  oqObjectInit(&SD0.oqueue, sd_out_buf0, sizeof sd_out_buf0, notify0, &SD0);
  SD0.uart = pADI_UART;
  SD0.clock = ADUCM_UARTCLK;

  nvicEnableVector(ADUCM_UART0_NUMBER, ADUCM_SERIAL_UART0_PRIORITY); 
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

  if (config == NULL) {
    config = &default_config;
  }

  if (sdp->state == SD_STOP) {
#if ADUCM_SERIAL_USE_UART0 == TRUE
    if (&SD0 == sdp) {
      ccEnableUART0();
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
    uart_deinit(sdp);

#if ADUCM_SERIAL_USE_UART0 == TRUE
    if (&SD0 == sdp) {
      ccDisableUART0();
      return;
    }
#endif
  }
}

/**
 * @brief   Common IRQ handler.
 *
 * @param[in] sdp       communication channel associated to the USART
 */
void sd_lld_serve_interrupt(SerialDriver *sdp) {
  uint32_t irr = sdp->uart->COMIIR;
  uint32_t ien = sdp->uart->COMIEN;
  
  if(!(irr & COMIIR_NINT_MSK)) {
    if(irr == COMIIR_STA_MODEMSTATUS) {
      volatile uint32_t msr;

      /* Clearing the interrupt. */
      msr = sdp->uart->COMMSR;

     (void) msr;
    }
    
    /* Data available, note it is a while in order to handle two situations:
       1) Another byte arrived after removing the previous one, this would cause
          an extra interrupt to serve.
       2) FIFO mode is enabled on devices that support it, we need to empty
          the FIFO.*/
    while (irr & COMIIR_STA_RXBUFFULL) {
      osalSysLockFromISR();
      sdIncomingDataI(sdp, (uint8_t)sdp->uart->COMRX);
      osalSysUnlockFromISR();

      irr = sdp->uart->COMIIR;
    }

    /* Transmission buffer empty, note it is a while in order to handle two
       situations:
       1) The data registers has been emptied immediately after writing it, this
          would cause an extra interrupt to serve.
       2) FIFO mode is enabled on devices that support it, we need to fill
          the FIFO.*/
    if (ien & COMIEN_ETBEI) {
      while (irr & COMIIR_STA_TXBUFEMPTY) {
        msg_t b;

        osalSysLockFromISR();
        b = oqGetI(&sdp->oqueue);
        if (b < MSG_OK) {
          chnAddFlagsI(sdp, CHN_OUTPUT_EMPTY);
          sdp->uart->COMIEN = ien & ~COMIEN_ETBEI;
          osalSysUnlockFromISR();
          break;
        }
        sdp->uart->COMTX = b;
        osalSysUnlockFromISR();
  
        irr = sdp->uart->COMIIR;
      }
    }
    
    if(irr == COMIIR_STA_RXLINESTATUS) {
      uint32_t lsr;

      /* Clearing the interrupt. */
      lsr = sdp->uart->COMLSR;

      /* Error handling. */
      if(lsr & 0x0EU) {
        set_error(sdp, lsr);
      }

      /* Error handling. */
      if(lsr & COMLSR_TEMT) {
        set_error(sdp, lsr);
      }

      /* Physical transmission end.*/
      if (lsr & COMLSR_TEMT) {
        osalSysLockFromISR();
        if (oqIsEmptyI(&sdp->oqueue)) {
          chnAddFlagsI(sdp, CHN_TRANSMISSION_END);
        }
        osalSysUnlockFromISR();
      }
    }
  } 
}

#endif /* HAL_USE_SERIAL == TRUE */

/** @} */

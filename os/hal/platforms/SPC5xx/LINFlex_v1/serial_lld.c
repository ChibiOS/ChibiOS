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
 * @file    SPC5xx/serial_lld.c
 * @brief   SPC5xx low level serial driver code.
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

/**
 * @brief   LIINFlex-0 serial driver identifier.
 */
#if SPC5_SERIAL_USE_LINFLEX0 || defined(__DOXYGEN__)
SerialDriver SD1;
#endif

/**
 * @brief   LIINFlex-1 serial driver identifier.
 */
#if SPC5_SERIAL_USE_LINFLEX1 || defined(__DOXYGEN__)
SerialDriver SD2;
#endif

/**
 * @brief   LIINFlex-2 serial driver identifier.
 */
#if SPC5_SERIAL_USE_LINFLEX2 || defined(__DOXYGEN__)
SerialDriver SD3;
#endif

/**
 * @brief   LIINFlex-3 serial driver identifier.
 */
#if SPC5_SERIAL_USE_LINFLEX3 || defined(__DOXYGEN__)
SerialDriver SD4;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static const SerialConfig default_config = {
  SERIAL_DEFAULT_BITRATE,
  0
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   LINFlex initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void spc5_linflex_init(SerialDriver *sdp, const SerialConfig *config) {
  volatile struct LINFLEX_tag *linflexp = sdp->linflexp;

  linflexp->LINFBRR.R = 0;              /* Fractional divider.              */
  linflexp->LINIBRR.R = 0;              /* Integer divider.                 */
  linflexp->UARTSR.R = 0xFFFF;          /* Clearing UART status register.   */
  linflexp->UARTCR.R = config->mode | 1;/* Enforced UART mode.              */
  linflexp->LINIER.R = 0;               /* Interrupts enabled.              */
}

/**
 * @brief   LINFlex de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] linflexp  pointer to a LINFlex I/O block
 */
static void spc5_linflex_deinit(volatile struct LINFLEX_tag *linflexp) {

#if 0
  escip->LPR.R  = 0;
  escip->SR.R   = 0xFFFFFFFF;
  escip->CR1.R  = 0;
  escip->CR2.R  = 0x8000;           /* MDIS on.                             */
#endif
}

/**
 * @brief   Error handling routine.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] sr        eSCI SR register value
 */
static void set_error(SerialDriver *sdp, uint32_t sr) {
  flagsmask_t sts = 0;

#if 0
  if (sr & 0x08000000)
    sts |= SD_OVERRUN_ERROR;
  if (sr & 0x04000000)
    sts |= SD_NOISE_ERROR;
  if (sr & 0x02000000)
    sts |= SD_FRAMING_ERROR;
  if (sr & 0x01000000)
    sts |= SD_PARITY_ERROR;
/*  if (sr & 0x00000000)
    sts |= SD_BREAK_DETECTED;*/
#endif
  chSysLockFromIsr();
  chnAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();
}

/**
 * @brief   Common RXI IRQ handler.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void spc5xx_serve_rxi_interrupt(SerialDriver *sdp) {
}

/**
 * @brief   Common TXI IRQ handler.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void spc5xx_serve_txi_interrupt(SerialDriver *sdp) {
}

/**
 * @brief   Common ERR IRQ handler.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void spc5xx_serve_err_interrupt(SerialDriver *sdp) {
}

#if SPC5_SERIAL_USE_LINFLEX0 || defined(__DOXYGEN__)
static void notify1(GenericQueue *qp) {

  (void)qp;
}
#endif

#if SPC5_SERIAL_USE_LINFLEX1 || defined(__DOXYGEN__)
static void notify2(GenericQueue *qp) {

  (void)qp;
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if SPC5_SERIAL_USE_LINFLEX0 || defined(__DOXYGEN__)
/**
 * @brief   LINFlex-0 RXI interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_LINFLEX0_RXI_HANDLER) {

  CH_IRQ_PROLOGUE();

  spc5xx_serve_rxi_interrupt(&SD1);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   LINFlex-0 TXI interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_LINFLEX0_TXI_HANDLER) {

  CH_IRQ_PROLOGUE();

  spc5xx_serve_txi_interrupt(&SD1);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   LINFlex-0 ERR interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_LINFLEX0_ERR_HANDLER) {

  CH_IRQ_PROLOGUE();

  spc5xx_serve_err_interrupt(&SD1);

  CH_IRQ_EPILOGUE();
}
#endif

#if SPC5_SERIAL_USE_LINFLEX1 || defined(__DOXYGEN__)
/**
 * @brief   LINFlex-1 RXI interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_LINFLEX1_RXI_HANDLER) {

  CH_IRQ_PROLOGUE();

  spc5xx_serve_rxi_interrupt(&SD2);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   LINFlex-1 TXI interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_LINFLEX1_TXI_HANDLER) {

  CH_IRQ_PROLOGUE();

  spc5xx_serve_txi_interrupt(&SD2);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   LINFlex-1 ERR interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_LINFLEX1_ERR_HANDLER) {

  CH_IRQ_PROLOGUE();

  spc5xx_serve_err_interrupt(&SD2);

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

#if SPC5_SERIAL_USE_LINFLEX0
  sdObjectInit(&SD1, NULL, notify1);
  SD1.linflexp = &LINFLEX_0;
//  ESCI_A.CR2.R    = 0x8000;                 /* MDIS ON.                     */
  INTC.PSR[SPC5_LINFLEX0_RXI_NUMBER].R = SPC5_SERIAL_LINFLEX0_PRIORITY;
  INTC.PSR[SPC5_LINFLEX0_TXI_NUMBER].R = SPC5_SERIAL_LINFLEX0_PRIORITY;
  INTC.PSR[SPC5_LINFLEX0_ERR_NUMBER].R = SPC5_SERIAL_LINFLEX0_PRIORITY;
#endif

#if SPC5_SERIAL_USE_LINFLEX1
  sdObjectInit(&SD2, NULL, notify2);
  SD2.linflexp = &LINFLEX_1;
//  ESCI_B.CR2.R    = 0x8000;                 /* MDIS ON.                     */
  INTC.PSR[SPC5_LINFLEX1_RXI_NUMBER].R = SPC5_SERIAL_LINFLEX1_PRIORITY;
  INTC.PSR[SPC5_LINFLEX1_TXI_NUMBER].R = SPC5_SERIAL_LINFLEX1_PRIORITY;
  INTC.PSR[SPC5_LINFLEX1_ERR_NUMBER].R = SPC5_SERIAL_LINFLEX1_PRIORITY;
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
  spc5_linflex_init(sdp, config);
}

/**
 * @brief   Low level serial driver stop.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @notapi
 */
void sd_lld_stop(SerialDriver *sdp) {

  if (sdp->state == SD_READY)
    spc5_linflex_deinit(sdp->linflexp);
}

#endif /* HAL_USE_SERIAL */

/** @} */

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
 * @file    SPC563/serial_lld.c
 * @brief   SPC563 low level serial driver code.
 *
 * @addtogroup SPC563_SERIAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   eSCI-A serial driver identifier.
 */
#if USE_SPC563_ESCIA || defined(__DOXYGEN__)
SerialDriver SD1;
#endif

/**
 * @brief   eSCI-B serial driver identifier.
 */
#if USE_SPC563_ESCIB || defined(__DOXYGEN__)
SerialDriver SD2;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static const SerialConfig default_config = {
  SERIAL_DEFAULT_BITRATE,
  SC_MODE_NORMAL | SC_MODE_PARITY_NONE
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   eSCI initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void esci_init(SerialDriver *sdp) {
  volatile struct ESCI_tag *escip = sdp->escip;
  uint8_t mode = sdp->config->sc_mode;

  escip->CR2.R  = 0;                /* MDIS off.                            */
  escip->CR1.R  = 0;
  escip->LCR.R  = 0;
  escip->CR1.B.SBR = SPC563_SYSCLK / (16 * sdp->config->sc_speed);
  if (mode & SC_MODE_LOOPBACK)
    escip->CR1.B.LOOPS = 1;
  switch (mode & SC_MODE_PARITY) {
  case SC_MODE_PARITY_ODD:
    escip->CR1.B.PT = 1;
  case SC_MODE_PARITY_EVEN:
    escip->CR1.B.PE = 1;
    escip->CR1.B.M  = 1;            /* Makes it 8 bits data + 1 bit parity. */
  default:
    ;
  }
  escip->LPR.R  = 0;
  escip->CR1.R |= 0x0000002C;       /* RIE, TE, RE to 1.                    */
  escip->CR2.R |= 0x000F;           /* ORIE, NFIE, FEIE, PFIE to 1.         */
}

/**
 * @brief   eSCI de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] escip     pointer to an eSCI I/O block
 */
static void esci_deinit(volatile struct ESCI_tag *escip) {

  escip->LPR.R  = 0;
  escip->SR.R   = 0xFFFFFFFF;
  escip->CR1.R  = 0;
  escip->CR2.R  = 0x8000;           /* MDIS on.                             */
}

/**
 * @brief   Error handling routine.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] sr        eSCI SR register value
 */
static void set_error(SerialDriver *sdp, uint32_t sr) {
  sdflags_t sts = 0;

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
  chSysLockFromIsr();
  sdAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();
}

/**
 * @brief   Common IRQ handler.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void serve_interrupt(SerialDriver *sdp) {
  volatile struct ESCI_tag *escip = sdp->escip;

  uint32_t sr = escip->SR.R;
  escip->SR.R = 0x3FFFFFFF;                     /* Does not clear TDRE | TC.*/
  if (sr & 0x0F000000)                          /* OR | NF | FE | PF.       */
    set_error(sdp, sr);
  if (sr & 0x20000000) {                        /* RDRF.                    */
    chSysLockFromIsr();
    sdIncomingDataI(sdp, escip->DR.B.D);
    chSysUnlockFromIsr();
  }
  if (escip->CR1.B.TIE && (sr & 0x80000000)) {  /* TDRE.                    */
    msg_t b;
    chSysLockFromIsr();
    b = chOQGetI(&sdp->oqueue);
    if (b < Q_OK) {
      chEvtBroadcastI(&sdp->oevent);
      escip->CR1.B.TIE = 0;
    }
    else {
      ESCI_A.SR.B.TDRE = 1;
      escip->DR.R = (uint16_t)b;
    }
    chSysUnlockFromIsr();
  }
}

#if USE_SPC563_ESCIA || defined(__DOXYGEN__)
static void notify1(void) {

  if (ESCI_A.SR.B.TDRE) {
    msg_t b = sdRequestDataI(&SD1);
    if (b != Q_EMPTY) {
      ESCI_A.SR.B.TDRE = 1;
      ESCI_A.CR1.B.TIE = 1;
      ESCI_A.DR.R = (uint16_t)b;
    }
  }
/*  if (!ESCI_A.CR1.B.TIE) {
    msg_t b = sdRequestDataI(&SD1);
    if (b != Q_EMPTY) {
      ESCI_A.CR1.B.TIE = 1;
      ESCI_A.DR.R = (uint16_t)b;
    }
  }*/
}
#endif

#if USE_SPC563_ESCIB || defined(__DOXYGEN__)
static void notify2(void) {

  if (ESCI_B.SR.B.TDRE) {
    msg_t b = sdRequestDataI(&SD2);
    if (b != Q_EMPTY) {
      ESCI_B.SR.B.TDRE = 1;
      ESCI_B.CR1.B.TIE = 1;
      ESCI_B.DR.R = (uint16_t)b;
    }
  }
/*  if (!ESCI_B.CR1.B.TIE) {
    msg_t b = sdRequestDataI(&SD2);
    if (b != Q_EMPTY) {
      ESCI_B.CR1.B.TIE = 1;
      ESCI_B.DR.R = (uint16_t)b;
    }
  }*/
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if USE_SPC563_ESCIA || defined(__DOXYGEN__)
/**
 * @brief   eSCI-A interrupt handler.
 */
CH_IRQ_HANDLER(vector146) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD1);

  CH_IRQ_EPILOGUE();
}
#endif

#if USE_SPC563_ESCIB || defined(__DOXYGEN__)
/**
 * @brief   eSCI-B interrupt handler.
 */
CH_IRQ_HANDLER(vector149) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&SD2);

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 */
void sd_lld_init(void) {

#if USE_SPC563_ESCIA
  sdObjectInit(&SD1, NULL, notify1);
  SD1.escip       = &ESCI_A;
  ESCI_A.CR2.R    = 0x8000;                 /* MDIS ON.                     */
  INTC.PSR[146].R = SPC563_ESCIA_PRIORITY;
#endif

#if USE_SPC563_ESCIB
  sdObjectInit(&SD2, NULL, notify2);
  SD2.escip       = &ESCI_B;
  ESCI_B.CR2.R    = 0x8000;                 /* MDIS ON.                     */
  INTC.PSR[149].R = SPC563_ESCIB_PRIORITY;
#endif
}

/**
 * @brief   Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
void sd_lld_start(SerialDriver *sdp) {

  if (sdp->config == NULL)
    sdp->config = &default_config;
  esci_init(sdp);
}

/**
 * @brief   Low level serial driver stop.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
void sd_lld_stop(SerialDriver *sdp) {

  if (sdp->state == SD_READY)
    esci_deinit(sdp->escip);
}

#endif /* CH_HAL_USE_SERIAL */

/** @} */

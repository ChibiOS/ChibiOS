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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    STM32/can_lld.c
 * @brief   STM32 CAN subsystem low level driver source.
 *
 * @addtogroup CAN
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_CAN || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief ADC1 driver identifier.*/
#if STM32_CAN_USE_CAN1 || defined(__DOXYGEN__)
CANDriver CAND1;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   CAN1 TX interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(CAN1_TX_IRQHandler) {

  CH_IRQ_PROLOGUE();

  /* No more events until a message is transmitted.*/
  CAN1->TSR = CAN_TSR_RQCP0 | CAN_TSR_RQCP1 | CAN_TSR_RQCP2;
  chSysLockFromIsr();
  while (chSemGetCounterI(&CAND1.cd_txsem) < 0)
    chSemSignalI(&CAND1.cd_txsem);
  chEvtBroadcastI(&CAND1.cd_txempty_event);
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}

/*
 * @brief   CAN1 RX0 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(CAN1_RX0_IRQHandler) {
  uint32_t rf0r;

  CH_IRQ_PROLOGUE();

  rf0r = CAN1->RF0R;
  if ((rf0r & CAN_RF0R_FMP0) > 0) {
    /* No more receive events until the queue 0 has been emptied.*/
    CAN1->IER &= ~CAN_IER_FMPIE0;
    chSysLockFromIsr();
    while (chSemGetCounterI(&CAND1.cd_rxsem) < 0)
      chSemSignalI(&CAND1.cd_rxsem);
    chEvtBroadcastI(&CAND1.cd_rxfull_event);
    chSysUnlockFromIsr();
  }
  if ((rf0r & CAN_RF0R_FOVR0) > 0) {
    /* Overflow events handling.*/
    CAN1->RF0R = CAN_RF0R_FOVR0;
    canAddFlagsI(&CAND1, CAN_OVERFLOW_ERROR);
    chSysLockFromIsr();
    chEvtBroadcastI(&CAND1.cd_error_event);
    chSysUnlockFromIsr();
  }

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 RX1 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(CAN1_RX1_IRQHandler) {

  CH_IRQ_PROLOGUE();

  chSysHalt(); /* Not supported (yet).*/

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 SCE interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(CAN1_SCE_IRQHandler) {
  uint32_t msr;

  CH_IRQ_PROLOGUE();

  msr = CAN1->MSR;
  CAN1->MSR = CAN_MSR_ERRI | CAN_MSR_WKUI | CAN_MSR_SLAKI;
  /* Wakeup event.*/
  if (msr & CAN_MSR_WKUI) {
    chSysLockFromIsr();
    chEvtBroadcastI(&CAND1.cd_wakeup_event);
    chSysUnlockFromIsr();
  }
  /* Error event.*/
  if (msr & CAN_MSR_ERRI) {
    canstatus_t flags;
    uint32_t esr = CAN1->ESR;

    CAN1->ESR &= ~CAN_ESR_LEC;
    flags = (canstatus_t)(esr & 7);
    if ((esr & CAN_ESR_LEC) > 0)
      flags |= CAN_FRAMING_ERROR;
    chSysLockFromIsr();
    canAddFlagsI(&CAND1, flags | (canstatus_t)(flags < 16));
    chEvtBroadcastI(&CAND1.cd_error_event);
    chSysUnlockFromIsr();
  }

  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level CAN driver initialization.
 *
 * @notapi
 */
void can_lld_init(void) {

#if STM32_CAN_USE_CAN1
  /* CAN reset, ensures reset state in order to avoid trouble with JTAGs.*/
  RCC->APB1RSTR = RCC_APB1RSTR_CAN1RST;
  RCC->APB1RSTR = 0;

  /* Driver initialization.*/
  canObjectInit(&CAND1);
  CAND1.cd_can = CAN1;
#endif
}

/**
 * @brief   Configures and activates the CAN peripheral.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_start(CANDriver *canp) {

  /* Clock activation.*/
#if STM32_CAN_USE_CAN1
  if (&CAND1 == canp) {
    NVICEnableVector(USB_HP_CAN1_TX_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_CAN_CAN1_IRQ_PRIORITY));
    NVICEnableVector(USB_LP_CAN1_RX0_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_CAN_CAN1_IRQ_PRIORITY));
    NVICEnableVector(CAN1_RX1_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_CAN_CAN1_IRQ_PRIORITY));
    NVICEnableVector(CAN1_SCE_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_CAN_CAN1_IRQ_PRIORITY));
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
  }
#endif

  /* Entering initialization mode. */
  canp->cd_state = CAN_STARTING;
  canp->cd_can->MCR = CAN_MCR_INRQ;
  while ((canp->cd_can->MSR & CAN_MSR_INAK) == 0)
    chThdSleepS(1);
  /* BTR initialization.*/
  canp->cd_can->BTR = canp->cd_config->cc_btr;
  /* MCR initialization.*/
  canp->cd_can->MCR = canp->cd_config->cc_mcr;
  /* Filters initialization.*/
  canp->cd_can->FMR |= CAN_FMR_FINIT;
  if (canp->cd_config->cc_num > 0) {
    uint32_t i, fmask;
    CAN_FilterRegister_TypeDef *cfp;

    canp->cd_can->FA1R = 0;
    canp->cd_can->FM1R = 0;
    canp->cd_can->FS1R = 0;
    canp->cd_can->FFA1R = 0;
    cfp = canp->cd_can->sFilterRegister;
    fmask = 1;
    for (i = 0; i < CAN_MAX_FILTERS; i++) {
      if (i < canp->cd_config->cc_num) {
        if (canp->cd_config->cc_filters[i].cf_mode)
          canp->cd_can->FM1R |= fmask;
        if (canp->cd_config->cc_filters[i].cf_scale)
          canp->cd_can->FS1R |= fmask;
        if (canp->cd_config->cc_filters[i].cf_assignment)
          canp->cd_can->FFA1R |= fmask;
        cfp->FR1 = canp->cd_config->cc_filters[i].cf_register1;
        cfp->FR2 = canp->cd_config->cc_filters[i].cf_register2;
        canp->cd_can->FA1R |= fmask;
      }
      else {
        cfp->FR1 = 0;
        cfp->FR2 = 0;
      }
      /* Gives a chance for preemption since this is a rather long loop.*/
      chSysUnlock();
      cfp++;
      fmask <<= 1;
      chSysLock();
    }
  }
  else {
    /* Setup a default filter.*/
    canp->cd_can->sFilterRegister[0].FR1 = 0;
    canp->cd_can->sFilterRegister[0].FR2 = 0;
    canp->cd_can->FM1R = 0;
    canp->cd_can->FFA1R = 0;
    canp->cd_can->FS1R = 1;
    canp->cd_can->FA1R = 1;
  }
  canp->cd_can->FMR &= ~CAN_FMR_FINIT;
  /* Interrupt sources initialization.*/
  canp->cd_can->IER = CAN_IER_TMEIE  | CAN_IER_FMPIE0 | CAN_IER_FMPIE1 |
                      CAN_IER_WKUIE  | CAN_IER_ERRIE  | CAN_IER_LECIE  |
                      CAN_IER_BOFIE  | CAN_IER_EPVIE  | CAN_IER_EWGIE  |
                      CAN_IER_FOVIE0 | CAN_IER_FOVIE1;
}

/**
 * @brief   Deactivates the CAN peripheral.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_stop(CANDriver *canp) {

  /* If in ready state then disables the CAN peripheral.*/
  if (canp->cd_state == CAN_READY) {
#if STM32_CAN_USE_CAN1
    if (&CAND1 == canp) {
      CAN1->MCR = 0x00010002;                   /* Register reset value.    */
      CAN1->IER = 0x00000000;                   /* All sources disabled.    */
      NVICDisableVector(USB_HP_CAN1_TX_IRQn);
      NVICDisableVector(USB_LP_CAN1_RX0_IRQn);
      NVICDisableVector(CAN1_RX1_IRQn);
      NVICDisableVector(CAN1_SCE_IRQn);
      RCC->APB1ENR &= ~RCC_APB1ENR_CAN1EN;
    }
#endif
  }
}

/**
 * @brief   Determines whether a frame can be transmitted.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @return The queue space availability.
 * @retval FALSE        no space in the transmit queue.
 * @retval TRUE         transmit slot available.
 *
 * @notapi
 */
bool_t can_lld_can_transmit(CANDriver *canp) {

  return (canp->cd_can->TSR & CAN_TSR_TME) != 0;
}

/**
 * @brief   Inserts a frame into the transmit queue.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] ctfp      pointer to the CAN frame to be transmitted
 *
 * @notapi
 */
void can_lld_transmit(CANDriver *canp, const CANTxFrame *ctfp) {
  uint32_t tir;
  CAN_TxMailBox_TypeDef *tmbp;

  /* Pointer to a free transmission mailbox.*/
  tmbp = &canp->cd_can->sTxMailBox[(canp->cd_can->TSR & CAN_TSR_CODE) >> 24];

  /* Preparing the message.*/
  if (ctfp->cf_IDE)
    tir = ((uint32_t)ctfp->cf_EID << 3) | ((uint32_t)ctfp->cf_RTR << 1) |
          CAN_TI0R_IDE;
  else
    tir = ((uint32_t)ctfp->cf_SID << 21) | ((uint32_t)ctfp->cf_RTR << 1);
  tmbp->TDTR = ctfp->cf_DLC;
  tmbp->TDLR = ctfp->cf_data32[0];
  tmbp->TDHR = ctfp->cf_data32[1];
  tmbp->TIR = tir | CAN_TI0R_TXRQ;
}

/**
 * @brief   Determines whether a frame has been received.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @return The queue space availability.
 * @retval FALSE        no space in the transmit queue.
 * @retval TRUE         transmit slot available.
 *
 * @notapi
 */
bool_t can_lld_can_receive(CANDriver *canp) {

  return (canp->cd_can->RF0R & CAN_RF0R_FMP0) > 0;
}

/**
 * @brief   Receives a frame from the input queue.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[out] crfp     pointer to the buffer where the CAN frame is copied
 *
 * @notapi
 */
void can_lld_receive(CANDriver *canp, CANRxFrame *crfp) {
  uint32_t r;

  /* Fetches the message.*/
  r = canp->cd_can->sFIFOMailBox[0].RIR;
  crfp->cf_RTR = (r & CAN_RI0R_RTR) >> 1;
  crfp->cf_IDE = (r & CAN_RI0R_IDE) >> 2;
  if (crfp->cf_IDE)
    crfp->cf_EID = r >> 3;
  else
    crfp->cf_SID = r >> 21;
  r = canp->cd_can->sFIFOMailBox[0].RDTR;
  crfp->cf_DLC = r & CAN_RDT0R_DLC;
  crfp->cf_FMI = (uint8_t)(r >> 8);
  crfp->cf_TIME = (uint16_t)(r >> 16);
  crfp->cf_data32[0] = canp->cd_can->sFIFOMailBox[0].RDLR;
  crfp->cf_data32[1] = canp->cd_can->sFIFOMailBox[0].RDHR;

  /* Releases the mailbox.*/
  canp->cd_can->RF0R = CAN_RF0R_RFOM0;

  /* If the queue is empty re-enables the interrupt in order to generate
     events again.*/
  if ((canp->cd_can->RF0R & CAN_RF0R_FMP0) == 0)
    canp->cd_can->IER |= CAN_IER_FMPIE0;
}

#if CAN_USE_SLEEP_MODE || defined(__DOXYGEN__)
/**
 * @brief   Enters the sleep mode.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_sleep(CANDriver *canp) {

  canp->cd_can->MCR |= CAN_MCR_SLEEP;
}

/**
 * @brief   Enforces leaving the sleep mode.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_wakeup(CANDriver *canp) {

  canp->cd_can->MCR &= ~CAN_MCR_SLEEP;
}
#endif /* CAN_USE_SLEEP_MODE */

#endif /* HAL_USE_CAN */

/** @} */

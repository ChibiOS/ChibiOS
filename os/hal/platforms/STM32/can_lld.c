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
 * @file STM32/can_lld.c
 * @brief STM32 CAN subsystem low level driver source
 * @addtogroup STM32_CAN
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_CAN || defined(__DOXYGEN__)

/*===========================================================================*/
/* Low Level Driver exported variables.                                      */
/*===========================================================================*/

/** @brief ADC1 driver identifier.*/
#if USE_STM32_CAN1 || defined(__DOXYGEN__)
CANDriver CAND1;
#endif

/*===========================================================================*/
/* Low Level Driver local variables.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver local functions.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver interrupt handlers.                                      */
/*===========================================================================*/

/*
 * CAN1 TX interrupt handler.
 */
CH_IRQ_HANDLER(Vector8C) {

  CH_IRQ_PROLOGUE();

  /* No more events until a message is transmitted.*/
  CAN1->IER &= ~CAN_IER_TMEIE;
  chEvtBroadcastI(&CAND1.cd_txempty_event);

  CH_IRQ_EPILOGUE();
}

/*
 * CAN1 RX0 interrupt handler.
 */
CH_IRQ_HANDLER(Vector90) {
  uint32_t rf0r;

  CH_IRQ_PROLOGUE();

  rf0r = CAN1->RF0R;
  chSysLockFromIsr();
  if ((rf0r & CAN_RF0R_FMP0) > 0) {
    /* No more receive events until the queue 0 has been emptied.*/
    CAN1->IER &= ~CAN_IER_FMPIE0;
    chEvtBroadcastI(&CAND1.cd_rxfull_event);
  }
  if ((rf0r & CAN_RF0R_FOVR0) > 0) {
    /* Overflow events handling.*/
    CAN1->RF0R = CAN_RF0R_FOVR0;
    canAddFlagsI(&CAND1, CAN_OVERFLOW_ERROR);
    chEvtBroadcastI(&CAND1.cd_error_event);
  }
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}

/*
 * CAN1 RX1 interrupt handler.
 */
CH_IRQ_HANDLER(Vector94) {

  CH_IRQ_PROLOGUE();

  chSysHalt(); /* Not supported (yet).*/

  CH_IRQ_EPILOGUE();
}

/*
 * CAN1 SCE interrupt handler.
 */
CH_IRQ_HANDLER(Vector98) {
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
    canAddFlagsI(&CAND1, flags);
    chEvtBroadcastI(&CAND1.cd_error_event);
    chSysUnlockFromIsr();
  }

  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Low Level Driver exported functions.                                      */
/*===========================================================================*/

/**
 * @brief Low level CAN driver initialization.
 */
void can_lld_init(void) {

#if USE_STM32_CAN1
  /* CAN reset, ensures reset state in order to avoid trouble with JTAGs.*/
  RCC->APB1RSTR = RCC_APB1RSTR_CAN1RST;
  RCC->APB1RSTR = 0;

  /* Driver initialization.*/
  canObjectInit(&CAND1);
  CAND1.cd_can = CAN1;
#endif
}

/**
 * @brief Configures and activates the CAN peripheral.
 *
 * @param[in] canp pointer to the @p CANDriver object
 */
void can_lld_start(CANDriver *canp) {

  /* Clock activation.*/
#if USE_STM32_CAN1
  if (&CAND1 == canp) {
    NVICEnableVector(USB_HP_CAN1_TX_IRQn, STM32_CAN1_IRQ_PRIORITY);
    NVICEnableVector(USB_LP_CAN1_RX0_IRQn, STM32_CAN1_IRQ_PRIORITY);
    NVICEnableVector(CAN1_RX1_IRQn, STM32_CAN1_IRQ_PRIORITY);
    NVICEnableVector(CAN1_SCE_IRQn, STM32_CAN1_IRQ_PRIORITY);
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
  }
#endif

  /* Entering initialization mode. */
  canp->cd_state = CAN_STARTING;
  canp->cd_can->MCR = CAN_MCR_INRQ;
  while ((canp->cd_can->MSR & CAN_MSR_INAK) == 0)
    chThdSleepS(1);
  /* Initialization.*/
  canp->cd_can->BTR = canp->cd_config->cc_btr;
  canp->cd_can->MCR = canp->cd_config->cc_mcr;
  canp->cd_can->IER = CAN_IER_TMEIE  | CAN_IER_FMPIE0 | CAN_IER_FMPIE1 |
                      CAN_IER_WKUIE  | CAN_IER_ERRIE  | CAN_IER_LECIE  |
                      CAN_IER_BOFIE  | CAN_IER_EPVIE  | CAN_IER_EWGIE  |
                      CAN_IER_FOVIE0 | CAN_IER_FOVIE1;
}

/**
 * @brief Deactivates the CAN peripheral.
 *
 * @param[in] canp pointer to the @p CANDriver object
 */
void can_lld_stop(CANDriver *canp) {

  /* If in ready state then disables the CAN clock.*/
  if (canp->cd_state == CAN_READY) {
#if USE_STM32_CAN1
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
 * @brief Determines whether a frame can be transmitted.
 *
 * @param[in] canp pointer to the @p CANDriver object
 *
 * @return The queue space availability.
 * @retval FALSE no space in the transmit queue.
 * @retval TRUE transmit slot available.
 */
bool_t can_lld_can_transmit(CANDriver *canp) {

  return FALSE;
}

/**
 * @brief Inserts a frame into the transmit queue.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] cfp       pointer to the CAN frame to be transmitted
 *
 * @return The operation status.
 * @retval RDY_OK frame transmitted.
 */
msg_t can_lld_transmit(CANDriver *canp, const CANFrame *cfp) {

  return RDY_OK;
}

/**
 * @brief Determines whether a frame has been received.
 *
 * @param[in] canp pointer to the @p CANDriver object
 *
 * @return The queue space availability.
 * @retval FALSE no space in the transmit queue.
 * @retval TRUE transmit slot available.
 */
bool_t can_lld_can_receive(CANDriver *canp) {

  return FALSE;
}

/**
 * @brief Receives a frame from the input queue.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[out] cfp      pointer to the buffer where the CAN frame is copied
 *
 * @return The operation status.
 * @retval RDY_OK frame received.
 */
msg_t can_lld_receive(CANDriver *canp, CANFrame *cfp) {

  return RDY_OK;
}

#if CAN_USE_SLEEP_MODE || defined(__DOXYGEN__)
/**
 * @brief Enters the sleep mode.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 */
void can_lld_sleep(CANDriver *canp) {

}

/**
 * @brief Enforces leaving the sleep mode.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 */
void can_lld_wakeup(CANDriver *canp) {

}
#endif /* CAN_USE_SLEEP_MODE */

#endif /* CH_HAL_USE_CAN */

/** @} */

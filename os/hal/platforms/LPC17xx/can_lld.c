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
 * @file    LPC17xx/can_lld.c
 * @brief   LPC17xx CAN subsystem low level driver source.
 *
 * @addtogroup CAN
 * @{
 */

/*
  This file has been contributed by:
                Marcin Jokel.
*/

#include "ch.h"
#include "hal.h"

#if HAL_USE_CAN || defined(__DOXYGEN__)
/* TODO: FullCAN mode. */
/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief CAN1 driver identifier.*/
#if LPC17xx_CAN_USE_CAN1 || defined(__DOXYGEN__)
CANDriver CAND1;
#endif

/** @brief CAN2 driver identifier.*/
#if LPC17xx_CAN_USE_CAN2 || defined(__DOXYGEN__)
CANDriver CAND2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if LPC17xx_CAN_USE_FILTER
/**
 * @brief   Copy table to filter ram.
 *
 * @param[in] reg
 * @param[in] ptable
 * @param[in] addr_start
 * @param[in] addr_end
 *
 * @notapi
 */

static void can_lld_copy_table_to_ram(volatile uint32_t * reg,
                                  const uint32_t * ptable,
                                  uint32_t addr_start,
                                  uint32_t addr_end) {

  uint32_t i;

  if (reg != NULL) {
  *reg = ((addr_start * 4) << 2);
  }
   for (i = addr_start; i < addr_end; i++) {
     LPC_CANAF_RAM->mask[i] = ptable[i];
   }
}

/**
 * @brief   Programs the filters.
 *
 * @param[in] cfc       pointer to the can filter configuration structure
 *
 * @notapi
 */
static void can_lld_set_filter( const CANFilterConfig *cfc) {

  uint32_t addr_start;
  uint32_t addr_end;

  LPC_CANAF->AFMR = AFMR_ACC_OFF;           /* Acceptance filter off mode. */

  addr_start = cfc->fc_id_table_n;

  if (addr_start > 0) {
    can_lld_copy_table_to_ram(NULL, cfc->fc_id_table, 0, addr_start);
  }

  addr_start = cfc->fc_id_table_n;
  addr_end = cfc->std_id_table_n;

  can_lld_copy_table_to_ram(&LPC_CANAF->SFF_sa, cfc->std_id_table, addr_start, addr_end);

  addr_start = addr_end;
  addr_end += cfc->std_range_id_table_n;

  can_lld_copy_table_to_ram(&LPC_CANAF->SFF_GRP_sa, cfc->std_range_id_table, addr_start, addr_end);

  addr_start = addr_end;
  addr_end += cfc->ext_id_table_n;

  can_lld_copy_table_to_ram(&LPC_CANAF->EFF_sa, cfc->ext_id_table, addr_start, addr_end);

  addr_start = addr_end;
  addr_end += cfc->ext_range_id_table_n;

  can_lld_copy_table_to_ram(&LPC_CANAF->EFF_GRP_sa, cfc->ext_range_id_table, addr_start, addr_end);

  LPC_CANAF->ENDofTable = (addr_end * 4) << 2;

  LPC_CANAF->AFMR = cfc->afmr;
}
#endif

/**
 * @brief   Common TX ISR handler.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
static void can_lld_tx_handler(CANDriver *canp) {

  /* All transmit buffers are available.*/
  if (canp->can->GSR & CANGSR_TBS) {
    chSysLockFromIsr();
    while (chSemGetCounterI(&canp->txsem) < 0)
      chSemSignalI(&canp->txsem);
    chEvtBroadcastFlagsI(&canp->txempty_event, CAN_MAILBOX_TO_MASK(1));
    chSysUnlockFromIsr();
  }
}

/**
 * @brief   Common RX ISR handler.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] status
 * @notapi
 */
static void can_lld_rx_handler(CANDriver *canp, uint32_t status) {

  /* No more receive events until the queue 0 has been emptied.*/
  canp->can->IER &= ~CANIER_RIE;
  chSysLockFromIsr();
  while (chSemGetCounterI(&canp->rxsem) < 0)
    chSemSignalI(&canp->rxsem);
  chEvtBroadcastFlagsI(&canp->rxfull_event, CAN_MAILBOX_TO_MASK(1));
  chSysUnlockFromIsr();

  if (( status & CANICR_DOI) > 0) {
    /* Overflow events handling.*/
    canp->can->CMR = CANCMR_CDO;
    chSysLockFromIsr();
    chEvtBroadcastFlagsI(&canp->error_event, CAN_OVERFLOW_ERROR);
    chSysUnlockFromIsr();
  }
}

/**
 * @brief   Error ISR handler.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] flags
 *
 * @notapi
 */
static void can_lld_error_handler(CANDriver *canp, uint32_t status) {

  uint32_t flags;

  /* Error event.*/
  /* The high 16-bits of the ICR register (error codes for bus error)
     is copied unchanged in the upper half word of the listener flags mask.*/
  flags = status & 0xFFFF0000;
  if (status & CANICR_EI)
    flags |= CAN_WARNING_ERROR;
  if (status & CANICR_EPI)
    flags |= CAN_PASSIVE_ERROR;
  if (status & CANICR_BEI)
      flags |= CAN_BUS_ERROR;

  chSysLockFromIsr();
  chEvtBroadcastFlagsI(&canp->error_event, flags);
  chSysUnlockFromIsr();
}


#if CAN_USE_SLEEP_MODE
/**
 * @brief   Wake up ISR handler.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
static void can_lld_wakeup_handler(CANDriver *canp) {

  /* Wakeup event.*/
  canp->state = CAN_READY;
  can_lld_wakeup(canp);
  chSysLockFromIsr();
  chEvtBroadcastI(&canp->wakeup_event);
  chSysUnlockFromIsr();

}
#endif /* CAN_USE_SLEEP_MODE */

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if LPC17xx_CAN_USE_CAN1 || LPC17xx_CAN_USE_CAN2 ||defined(__DOXYGEN__)
/**
 * @brief   CAN Common, CAN 1 Tx, CAN 1 Rx, CAN 2 Tx, CAN 2 Rx.
 *
 * @isr
 */
CH_IRQ_HANDLER(VectorA4) {

#if LPC17xx_CAN_USE_CAN1
  volatile uint32_t can1_status;
#endif
#if LPC17xx_CAN_USE_CAN2
  volatile uint32_t can2_status;
#endif

  CH_IRQ_PROLOGUE();

#if LPC17xx_CAN_USE_CAN1
  can1_status = LPC_CAN1->ICR;
#endif
#if LPC17xx_CAN_USE_CAN2
  can2_status = LPC_CAN2->ICR;
#endif

#if LPC17xx_CAN_USE_CAN1
  if (can1_status & 0x000007FF) {

    if (can1_status & (CANICR_EI | CANICR_EPI | CANICR_BEI)) {
      can_lld_error_handler(&CAND1, can1_status);
    }

    if (can1_status & (CANICR_RI | CANICR_DOI)) {
      can_lld_rx_handler(&CAND1, can1_status);
    }

    if (can1_status & (CANICR_TI1 | CANICR_TI2 | CANICR_TI3)){
      can_lld_tx_handler(&CAND1);
    }
  }
#endif

#if LPC17xx_CAN_USE_CAN2
  if (can2_status & 0x000007FF) {

    if (can2_status & CANICR_EI) {
      can_lld_error_handler(&CAND2, can2_status);
    }

    if (can2_status & (CANICR_RI | CANICR_DOI)) {
      can_lld_rx_handler(&CAND2, can2_status);
    }

    if (can2_status & (CANICR_TI1 | CANICR_TI2 | CANICR_TI3)){
      can_lld_tx_handler(&CAND2);
    }
  }
#endif
  CH_IRQ_EPILOGUE();
}

#if CAN_USE_SLEEP_MODE
/**
 * @brief   CAN Common, CAN 1 Tx, CAN 1 Rx, CAN 2 Tx, CAN 2 Rx.
 *
 * @isr
 */
CH_IRQ_HANDLER(VectorC8) {

#if LPC17xx_CAN_USE_CAN1
 uint32_t can1_status;
#endif
#if LPC17xx_CAN_USE_CAN2
 uint32_t can2_status;
#endif

 CH_IRQ_PROLOGUE();

#if LPC17xx_CAN_USE_CAN1
 can1_status = LPC_CAN1->ICR;
#endif
#if LPC17xx_CAN_USE_CAN2
 can2_status = LPC_CAN2->ICR;
#endif

#if LPC17xx_CAN_USE_CAN1
 if (can1_status & CANICR_WUI) {
   can_lld_wakeup_handler(&CAND1);
 }
#endif

#if LPC17xx_CAN_USE_CAN2
 if (can2_status & CANICR_WUI) {
   can_lld_wakeup_handler(&CAND2);
 }
#endif

 CH_IRQ_EPILOGUE();
}
#endif /* CAN_USE_SLEEP_MODE */

#endif /* LPC17xx_CAN_USE_CAN1 || LPC17xx_CAN_USE_CAN2 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level CAN driver initialization.
 *
 * @notapi
 */
void can_lld_init(void) {

#if LPC17xx_CAN_USE_CAN1
  /* Driver initialization.*/
  canObjectInit(&CAND1);
  CAND1.can = LPC_CAN1;
#endif
#if LPC17xx_CAN_USE_CAN2
  /* Driver initialization.*/
  canObjectInit(&CAND2);
  CAND2.can = LPC_CAN2;
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

#if LPC17xx_CAN_USE_CAN1
  if (&CAND1 == canp) {
    LPC_SC->PCONP |= (1UL << 13);
  }
#endif
#if LPC17xx_CAN_USE_CAN2
  if (&CAND2 == canp) {
    LPC_SC->PCONP |= (1UL << 14);
  }
#endif

  canp->can->MOD = CANMOD_RM;           /* Reset mode. */

#if !LPC17xx_CAN_USE_FILTER
  LPC_CANAF->AFMR = AFMR_ACC_BP;        /* Bypass can filter. */
#endif

  /* Entering initialization mode. */
  canp->state = CAN_STARTING;

  canp->can->BTR = canp->config->btr;   /* BTR initialization.*/
  canp->can->CMR = CANCMR_RRB;

  /* Interrupt sources initialization.*/
  canp->can->IER = CANIER_TIE3 | CANIER_TIE2 | CANIER_BEIE |
                   CANIER_EPIE | CANIER_DOIE | CANIER_EIE  |
                   CANIER_TIE1 | CANIER_RIE;

#if CAN_USE_SLEEP_MODE
  canp->can->IER |= CANIER_WUIE;
  nvicEnableVector(CANActivity_IRQn,
                           CORTEX_PRIORITY_MASK(LPC17xx_CAN_IRQ_PRIORITY));
#endif /* CAN_USE_SLEEP_MODE */

  canp->can->MOD = canp->config->mod;       /* Operating mode. */

  nvicEnableVector(CAN_IRQn,
                         CORTEX_PRIORITY_MASK(LPC17xx_CAN_IRQ_PRIORITY));
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
  if (canp->state == CAN_READY) {

    canp->can->MOD = CANMOD_RM;                 /* Reset mode. */
    canp->can->IER = 0;
#if (LPC17xx_CAN_USE_CAN1 && LPC17xx_CAN_USE_CAN2) == 0
    nvicDisableVector(CAN_IRQn);                /* Interrupt disabled only if one CAN is in use. */
#endif

#if LPC17xx_CAN_USE_CAN1
    if (&CAND1 == canp) {
      LPC_SC->PCONP &= ~(1UL << 13);
    }
#endif
#if LPC17xx_CAN_USE_CAN2
    if (&CAND2 == canp) {
      LPC_SC->PCONP &= ~(1UL << 14);
    }
#endif

#if CAN_USE_SLEEP_MODE
    nvicDisableVector(CANActivity_IRQn);
#endif /* CAN_USE_SLEEP_MODE */
  }
}

/**
 * @brief   Determines whether a frame can be transmitted.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 *
 * @return              The queue space availability.
 * @retval FALSE        no space in the transmit queue.
 * @retval TRUE         transmit slot available.
 *
 * @notapi
 */
bool_t can_lld_is_tx_empty(CANDriver *canp, canmbx_t mailbox) {

  switch (mailbox) {
  case CAN_ANY_MAILBOX:
    return ((canp->can->SR & (CANSR_TBS1 | CANSR_TBS2 | CANSR_TBS3)) != 0);
  case 1:
    return (canp->can->SR & CANSR_TBS1) != 0;
  case 2:
    return (canp->can->SR & CANSR_TBS2) != 0;
  case 3:
    return (canp->can->SR & CANSR_TBS3) != 0;
  default:
    return FALSE;
  }
}

/**
 * @brief   Inserts a frame into the transmit queue.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] ctfp      pointer to the CAN frame to be transmitted
 * @param[in] mailbox   mailbox number,  @p CAN_ANY_MAILBOX for any mailbox
 *
 * @notapi
 */
void can_lld_transmit(CANDriver *canp,
                      canmbx_t mailbox,
                      const CANTxFrame *ctfp) {
  volatile uint32_t * tfi;
  uint32_t tbs_n;
  uint32_t sr;

  tbs_n = mailbox;

  if (mailbox == CAN_ANY_MAILBOX) {
    sr = canp->can->SR;
    if (sr & CANSR_TBS1) {
      tbs_n = 1;
    }
    else if (sr & CANSR_TBS2) {
      tbs_n = 2;
    }
    else if (sr & CANSR_TBS3) {
      tbs_n = 3;
    }
    else {
      return;
    }
  }

  /* Pointer to a free transmission mailbox.*/
  switch (tbs_n) {
  case 1:
    tfi = &canp->can->TFI1;
    break;
  case 2:
    tfi = &canp->can->TFI2;
    break;
  case 3:
    tfi = &canp->can->TFI3;
    break;
  default:
    return;
  }

  /* Preparing the message.*/
  * tfi = ((uint32_t)ctfp->IDE << 31) | ((uint32_t)ctfp->RTR << 30) |
      (((uint32_t)ctfp->DLC) << 16);    /* CAN Transmit Frame Information Register. */
  if (ctfp->IDE)
    *(tfi + 1) = ctfp->EID;             /* CAN Transmit Identifier register 29-bit. */
  else
    *(tfi + 1) = ctfp->SID;             /* CAN Transmit Identifier register 11-bit. */

  *(tfi + 2) = ctfp->data32[0];         /* CAN Transmit Data Register A. */
  *(tfi + 3) = ctfp->data32[1];         /* CAN Transmit Data Register B. */

#if LPC17xx_CAN_USE_LOCAL_SELF_TEST
  canp->can->CMR = (1UL << (4 + tbs_n)) | CANCMR_SRR;
#else
  canp->can->CMR = (1UL << (4 + tbs_n)) | CANCMR_TR;
#endif
}

/**
 * @brief   Determines whether a frame has been received.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 *
 * @return              The queue space availability.
 * @retval FALSE        no space in the transmit queue.
 * @retval TRUE         transmit slot available.
 *
 * @notapi
 */
bool_t can_lld_is_rx_nonempty(CANDriver *canp, canmbx_t mailbox) {

  switch (mailbox) {
  case CAN_ANY_MAILBOX:
  case 1:
    return ((canp->can->GSR & CANGSR_RBS) != 0);
  default:
    return FALSE;
  }
}

/**
 * @brief   Receives a frame from the input queue.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 * @param[out] crfp     pointer to the buffer where the CAN frame is copied
 *
 * @notapi
 */
void can_lld_receive(CANDriver *canp,
                     canmbx_t mailbox,
                     CANRxFrame *crfp) {
  uint32_t rfs, rid;

  switch (mailbox) {
  case CAN_ANY_MAILBOX:
  case 1:
    /* Fetches the message.*/
    rfs = canp->can->RFS;
    rid = canp->can->RID;
    crfp->data32[0] = canp->can->RDA;
    crfp->data32[1] = canp->can->RDB;

    /* Releases the mailbox.*/
    canp->can->CMR = CANCMR_RRB;

    /* Re-enables the interrupt in order to generate
       events again. */
    canp->can->IER |= CANIER_RIE;
    break;
  default:
    /* Should not happen, do nothing.*/
    return;
  }

  /* Decodes the various fields in the RX frame.*/
  crfp->RTR = (rfs & CANRFS_RTR) >> 30;
  crfp->IDE = (rfs & CANRFS_FF) >> 31;
  if (crfp->IDE) {
    crfp->EID = rid;
  }
  else {
    crfp->SID = rid;
  }
  crfp->DLC = (rfs & CANRFS_DLC) >> 16;
  crfp->IDF = (uint16_t)(rfs & CANRFS_ID);
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

  canp->can->MOD |= CANMOD_SM;
}

/**
 * @brief   Enforces leaving the sleep mode.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_wakeup(CANDriver *canp) {

  uint32_t reg_val;

#if LPC17xx_CAN_USE_CAN1
  if (&CAND1 == canp) {
    reg_val = (1UL << 1);
  }
#endif
#if LPC17xx_CAN_USE_CAN2
  if (&CAND2 == canp) {
    reg_val = (1UL << 2);
  }
#endif

  LPC_SC->CANSLEEPCLR = reg_val;
  canp->can->MOD &= ~CANMOD_SM;
  LPC_SC->CANWAKEFLAGS = reg_val;
}
#endif /* CAN_USE_SLEEP_MODE */

#if LPC17xx_CAN_USE_FILTER
void canSetFilter(const CANFilterConfig *cfc) {
  can_lld_set_filter(cfc);
}
#endif

#endif /* HAL_USE_CAN */

/** @} */

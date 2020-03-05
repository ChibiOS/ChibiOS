/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    FDCANv1/hal_can_lld.c
 * @brief   STM32 CAN subsystem low level driver source.
 *
 * @addtogroup CAN
 * @{
 */

#include "hal.h"

#if HAL_USE_CAN || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* Filter Standard Element Size in bytes.*/
#define SRAMCAN_FLS_SIZE                (1U * 4U)

/* Filter Extended Element Size in bytes.*/
#define SRAMCAN_FLE_SIZE                (2U * 4U)

/* RX FIFO 0 Elements Size in bytes.*/
#define SRAMCAN_RF0_SIZE                (18U * 4U)

/* RX FIFO 1 Elements Size in bytes.*/
#define SRAMCAN_RF1_SIZE                (18U * 4U)

/* RX Buffer Size in bytes.*/
#define SRAMCAN_RB_SIZE                 (18U * 4U)

/* TX Event FIFO Elements Size in bytes.*/
#define SRAMCAN_TEF_SIZE                (2U * 4U)

/* TX FIFO/Queue Elements Size in bytes.*/
#define SRAMCAN_TB_SIZE                 (18U * 4U)

/* Filter List Standard Start Address.*/
#define SRAMCAN_FLSSA ((uint32_t)0)

/* Filter List Extended Start Address.*/
#define SRAMCAN_FLESA ((uint32_t)(SRAMCAN_FLSSA +                           \
                                  (STM32_FDCAN_FLS_NBR * SRAMCAN_FLS_SIZE)))

/* RX FIFO 0 Start Address.*/
#define SRAMCAN_RF0SA ((uint32_t)(SRAMCAN_FLESA +                           \
                                  (STM32_FDCAN_FLE_NBR * SRAMCAN_FLE_SIZE)))

/* RX FIFO 1 Start Address.*/
#define SRAMCAN_RF1SA ((uint32_t)(SRAMCAN_RF0SA +                           \
                                  (STM32_FDCAN_RF0_NBR * SRAMCAN_RF0_SIZE)))

/* RX Buffer Start Address.*/
#define SRAMCAN_RBSA  ((uint32_t)(SRAMCAN_RF1SA +                           \
                                  (STM32_FDCAN_RF1_NBR * SRAMCAN_RF1_SIZE)))

/* TX Event FIFO Start Address.*/
#define SRAMCAN_TEFSA ((uint32_t)(SRAMCAN_RBSA +                            \
                                  (STM32_FDCAN_TEF_NBR * SRAMCAN_RB_SIZE)))

/* TX Buffers Start Address.*/
#define SRAMCAN_TBSA  ((uint32_t)(SRAMCAN_TEFSA +                           \
                                  (STM32_FDCAN_TEF_NBR * SRAMCAN_TEF_SIZE)))

#define SRAMCAN_TMSA  ((uint32_t)(SRAMCAN_TBSA +                            \
                                  (STM32_FDCAN_TB_NBR * SRAMCAN_TB_SIZE)))

/* Message RAM size.*/
#define SRAMCAN_SIZE  ((uint32_t)(SRAMCAN_TMSA +                            \
                                  (STM32_FDCAN_TM_NBR * SRAMCAN_TM_SIZE)))


/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief CAN1 driver identifier.*/
#if STM32_CAN_USE_FDCAN1 || defined(__DOXYGEN__)
CANDriver CAND1;
#endif

/** @brief CAN2 driver identifier.*/
#if STM32_CAN_USE_FDCAN2 || defined(__DOXYGEN__)
CANDriver CAND2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const uint8_t dlc_to_bytes[] = {
  0U,  1U,  2U,  3U,  4U,  5U,  6U,  7U,
  8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level CAN driver initialization.
 *
 * @notapi
 */
void can_lld_init(void) {

#if STM32_CAN_USE_FDCAN1
  /* Driver initialization.*/
  canObjectInit(&CAND1);
  CAND1.fdcan = FDCAN1;
#endif

#if STM32_CAN_USE_FDCAN2
  /* Driver initialization.*/
  canObjectInit(&CAND2);
  CAND2.fdcan = FDCAN2;
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
#if STM32_CAN_USE_FDCAN1
  if (&CAND1 == canp) {
    rccEnableFDCAN1(true);
  }
#endif

#if STM32_CAN_USE_FDCAN2
  if (&CAND2 == canp) {
    rccEnableFDCAN2(true);
  }
#endif
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
#if STM32_CAN_USE_FDCAN1
    if (&CAND1 == canp) {
      rccDisableFDCAN1();
    }
#endif

#if STM32_CAN_USE_FDCAN2
    if (&CAND2 == canp) {
      rccDisableFDCAN2();
    }
#endif
  }
}

/**
 * @brief   Determines whether a frame can be transmitted.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 *
 * @return              The queue space availability.
 * @retval false        no space in the transmit queue.
 * @retval true         transmit slot available.
 *
 * @notapi
 */
bool can_lld_is_tx_empty(CANDriver *canp, canmbx_t mailbox) {

  (void)mailbox;

  return (bool)((canp->fdcan->TXFQS & FDCAN_TXFQS_TFQF) == 0U);
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
  uint32_t *tx_address;

  (void)mailbox;

  osalDbgCheck(dlc_to_bytes[ctfp->DLC] <= CAN_MAX_DLC_BYTES);

  /* Writing frame.*/
  tx_address = canp->ram_base + (SRAMCAN_TBSA / sizeof (uint32_t));
  *tx_address++ = ctfp->header32[0];
  *tx_address++ = ctfp->header32[1];
  for (unsigned i = 0U; i < dlc_to_bytes[ctfp->DLC]; i += 4U) {
    *tx_address++ = ctfp->data32[i / 4U];
  }
}

/**
 * @brief   Determines whether a frame has been received.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 *
 * @return              The queue space availability.
 * @retval false        no space in the transmit queue.
 * @retval true         transmit slot available.
 *
 * @notapi
 */
bool can_lld_is_rx_nonempty(CANDriver *canp, canmbx_t mailbox) {

  switch (mailbox) {
    case CAN_ANY_MAILBOX:
      return can_lld_is_rx_nonempty(canp, 1U) ||
             can_lld_is_rx_nonempty(canp, 2U);
    case 1:
      return (bool)((canp->fdcan->RXF0S & FDCAN_RXF0S_F0FL) != 0U);
    case 2:
      return (bool)((canp->fdcan->RXF1S & FDCAN_RXF1S_F1FL) != 0U);
    default:
      return false;
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
void can_lld_receive(CANDriver *canp, canmbx_t mailbox, CANRxFrame *crfp) {
  uint32_t get_index;
  uint32_t *rx_address;

  if (mailbox == CAN_ANY_MAILBOX) {
    if (can_lld_is_rx_nonempty(canp, 1U)) {
      mailbox = 1U;
    }
    else if (can_lld_is_rx_nonempty(canp, 2U)) {
      mailbox = 2U;
    }
    else {
      return;
    }
  }

  /* GET index, add it and the length to the rx_address.*/
  get_index = (canp->fdcan->RXF0S & FDCAN_RXF0S_F0GI_Msk) >> FDCAN_RXF0S_F0GI_Pos;
  rx_address = canp->ram_base + (SRAMCAN_RF0SA +
                                 (get_index * SRAMCAN_RF0_SIZE)) / sizeof (uint32_t);
  crfp->header32[0] = *rx_address++;
  crfp->header32[1] = *rx_address++;

  /* Copy message from FDCAN peripheral's SRAM to structure. RAM is restricted
     to word aligned accesses, so up to 3 extra bytes may be copied.*/
  for (unsigned i = 0U; i < dlc_to_bytes[crfp->DLC]; i += 4U) {
    crfp->data32[i / 4U] = *rx_address++;
  }

  /* Acknowledge receipt by writing the get-index to the acknowledge
     register RXFxA then re-enable RX FIFO message arrived interrupt once
     the FIFO is emptied.*/
  if (mailbox == 1U) {
    uint32_t rxf0a = canp->fdcan->RXF0A;
    rxf0a &= ~FDCAN_RXF0A_F0AI_Msk;
    rxf0a |= get_index << FDCAN_RXF0A_F0AI_Pos;
    canp->fdcan->RXF0A = rxf0a;

    if (!can_lld_is_rx_nonempty(canp, mailbox)) {
//      canp->fdcan->IR  = FDCAN_IR_RF0N;
      canp->fdcan->IE |= FDCAN_IE_RF0NE;
    }
  }
  else {
    uint32_t rxf1a = canp->fdcan->RXF1A;
    rxf1a &= ~FDCAN_RXF1A_F1AI_Msk;
    rxf1a |= get_index << FDCAN_RXF1A_F1AI_Pos;
    canp->fdcan->RXF1A = rxf1a;

    if (!can_lld_is_rx_nonempty(canp, mailbox)) {
//      canp->fdcan->IR  = FDCAN_IR_RF1N;
      canp->fdcan->IE |= FDCAN_IE_RF1NE;
    }
  }
}

/**
 * @brief   Tries to abort an ongoing transmission.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number
 *
 * @notapi
 */
void can_lld_abort(CANDriver *canp, canmbx_t mailbox) {

  (void)canp;
  (void)mailbox;
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

}

/**
 * @brief   Enforces leaving the sleep mode.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_wakeup(CANDriver *canp) {

}
#endif /* CAN_USE_SLEEP_MODE */

#endif /* HAL_USE_CAN */

/**
 * @brief   FDCAN IRQ0 service routine.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_serve_interrupt0(CANDriver *canp) {

}

/**
 * @brief   FDCAN IRQ1 service routine.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
void can_lld_serve_interrupt1(CANDriver *canp) {

}

/** @} */

/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    can.c
 * @brief   CAN Driver code.
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

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   CAN Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void canInit(void) {

  can_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p CANDriver structure.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @init
 */
void canObjectInit(CANDriver *canp) {

  canp->cd_state    = CAN_STOP;
  canp->cd_config   = NULL;
  chSemInit(&canp->cd_txsem, 0);
  chSemInit(&canp->cd_rxsem, 0);
  chEvtInit(&canp->cd_rxfull_event);
  chEvtInit(&canp->cd_txempty_event);
  chEvtInit(&canp->cd_error_event);
  canp->cd_status = 0;
#if CAN_USE_SLEEP_MODE
  chEvtInit(&canp->cd_sleep_event);
  chEvtInit(&canp->cd_wakeup_event);
#endif /* CAN_USE_SLEEP_MODE */
}

/**
 * @brief   Configures and activates the CAN peripheral.
 * @note    Activating the CAN bus can be a slow operation this this function
 *          is not atomic, it waits internally for the initialization to
 *          complete.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] config    pointer to the @p CANConfig object
 *
 * @api
 */
void canStart(CANDriver *canp, const CANConfig *config) {

  chDbgCheck((canp != NULL) && (config != NULL), "canStart");

  chSysLock();
  chDbgAssert((canp->cd_state == CAN_STOP) ||
              (canp->cd_state == CAN_STARTING) ||
              (canp->cd_state == CAN_READY),
              "canStart(), #1", "invalid state");
  while (canp->cd_state == CAN_STARTING)
    chThdSleepS(1);
  if (canp->cd_state == CAN_STOP) {
    canp->cd_config = config;
    can_lld_start(canp);
    canp->cd_state = CAN_READY;
  }
  chSysUnlock();
}

/**
 * @brief   Deactivates the CAN peripheral.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @api
 */
void canStop(CANDriver *canp) {

  chDbgCheck(canp != NULL, "canStop");

  chSysLock();
  chDbgAssert((canp->cd_state == CAN_STOP) || (canp->cd_state == CAN_READY),
              "canStop(), #1", "invalid state");
  can_lld_stop(canp);
  chSemResetI(&canp->cd_rxsem, 0);
  chSemResetI(&canp->cd_txsem, 0);
  chSchRescheduleS();
  canp->cd_state  = CAN_STOP;
  canp->cd_status = 0;
  chSysUnlock();
}

/**
 * @brief   Can frame transmission.
 * @details The specified frame is queued for transmission, if the hardware
 *          queue is full then the invoking thread is queued.
 * @note    Trying to transmit while in sleep mode simply enqueues the thread.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] ctfp       pointer to the CAN frame to be transmitted
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation result.
 * @retval RDY_OK       the frame has been queued for transmission.
 * @retval RDY_TIMEOUT  The operation has timed out.
 * @retval RDY_RESET    The driver has been stopped while waiting.
 *
 * @api
 */
msg_t canTransmit(CANDriver *canp, const CANTxFrame *ctfp, systime_t timeout) {

  chDbgCheck((canp != NULL) && (ctfp != NULL), "canTransmit");

  chSysLock();
  chDbgAssert((canp->cd_state == CAN_READY) || (canp->cd_state == CAN_SLEEP),
              "canTransmit(), #1", "invalid state");
  while ((canp->cd_state == CAN_SLEEP) || !can_lld_can_transmit(canp)) {
    msg_t msg = chSemWaitTimeoutS(&canp->cd_txsem, timeout);
    if (msg != RDY_OK) {
      chSysUnlock();
      return msg;
    }
  }
  can_lld_transmit(canp, ctfp);
  chSysUnlock();
  return RDY_OK;
}

/**
 * @brief   Can frame receive.
 * @details The function waits until a frame is received.
 * @note    Trying to receive while in sleep mode simply enqueues the thread.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[out] crfp     pointer to the buffer where the CAN frame is copied
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout (useful in an
 *                        event driven scenario where a thread never blocks
 *                        for I/O).
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation result.
 * @retval RDY_OK       a frame has been received and placed in the buffer.
 * @retval RDY_TIMEOUT  The operation has timed out.
 * @retval RDY_RESET    The driver has been stopped while waiting.
 *
 * @api
 */
msg_t canReceive(CANDriver *canp, CANRxFrame *crfp, systime_t timeout) {

  chDbgCheck((canp != NULL) && (crfp != NULL), "canReceive");

  chSysLock();
  chDbgAssert((canp->cd_state == CAN_READY) || (canp->cd_state == CAN_SLEEP),
              "canReceive(), #1", "invalid state");
  while ((canp->cd_state == CAN_SLEEP) || !can_lld_can_receive(canp)) {
    msg_t msg = chSemWaitTimeoutS(&canp->cd_rxsem, timeout);
    if (msg != RDY_OK) {
      chSysUnlock();
      return msg;
    }
  }
  can_lld_receive(canp, crfp);
  chSysUnlock();
  return RDY_OK;
}

/**
 * @brief   Returns the current status mask and clears it.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @return              The status flags mask.
 *
 * @api
 */
canstatus_t canGetAndClearFlags(CANDriver *canp) {
  canstatus_t status;

  chSysLock();
  status = canp->cd_status;
  canp->cd_status = 0;
  chSysUnlock();
  return status;
}

#if CAN_USE_SLEEP_MODE || defined(__DOXYGEN__)
/**
 * @brief   Enters the sleep mode.
 * @details This function puts the CAN driver in sleep mode and broadcasts
 *          the @p cd_sleep_event event source.
 * @pre     In order to use this function the option @p CAN_USE_SLEEP_MODE must
 *          be enabled and the @p CAN_SUPPORTS_SLEEP mode must be supported
 *          by the low level driver.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @api
 */
void canSleep(CANDriver *canp) {

  chDbgCheck(canp != NULL, "canSleep");

  chSysLock();
  chDbgAssert((canp->cd_state == CAN_READY) || (canp->cd_state == CAN_SLEEP),
              "canSleep(), #1", "invalid state");
  if (canp->cd_state == CAN_READY) {
    can_lld_sleep(canp);
    canp->cd_state = CAN_SLEEP;
    chEvtBroadcastI(&canp->cd_sleep_event);
    chSchRescheduleS();
  }
  chSysUnlock();
}

/**
 * @brief   Enforces leaving the sleep mode.
 * @note    The sleep mode is supposed to be usually exited automatically by
 *          an hardware event.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 */
void canWakeup(CANDriver *canp) {

  chDbgCheck(canp != NULL, "canWakeup");

  chSysLock();
  chDbgAssert((canp->cd_state == CAN_READY) || (canp->cd_state == CAN_SLEEP),
              "canWakeup(), #1", "invalid state");
  if (canp->cd_state == CAN_SLEEP) {
    can_lld_wakeup(canp);
    canp->cd_state = CAN_READY;
    chEvtBroadcastI(&canp->cd_wakeup_event);
    chSchRescheduleS();
  }
  chSysUnlock();
}
#endif /* CAN_USE_SLEEP_MODE */

#endif /* HAL_USE_CAN */

/** @} */

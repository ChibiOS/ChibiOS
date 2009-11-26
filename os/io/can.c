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
 * @file CAN.c
 * @brief CAN Driver code.
 * @addtogroup CAN
 * @{
 */

#include <ch.h>
#include <can.h>

/**
 * @brief CAN Driver initialization.
 */
void canInit(void) {

  can_lld_init();
}

/**
 * @brief Initializes the standard part of a @p CANDriver structure.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 */
void canObjectInit(CANDriver *canp) {

  canp->can_state    = CAN_STOP;
  canp->can_config   = NULL;
  chSemInit(&canp->can_txsem);
  chSemInit(&canp->can_rxsem);
  chEvtInit(&canp->can_rxfull_event);
  chEvtInit(&canp->can_txempty_event);
#if CAN_USE_SLEEP_MODE
  chEvtInit(&canp->can_sleep_event);
  chEvtInit(&canp->can_wakeup_event);
#endif /* CAN_USE_SLEEP_MODE */
}

/**
 * @brief Configures and activates the CAN peripheral.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] config    pointer to the @p CANConfig object
 */
void canStart(CANDriver *canp, const CANConfig *config) {

  chDbgCheck((canp != NULL) && (config != NULL), "canStart");

  chSysLock();
  chDbgAssert((canp->can_state == CAN_STOP) || (canp->can_state == CAN_READY),
              "canStart(), #1",
              "invalid state");
  canp->can_config = config;
  can_lld_start(canp);
  canp->can_state = CAN_READY;
  chSysUnlock();
}

/**
 * @brief Deactivates the CAN peripheral.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 */
void canStop(CANDriver *canp) {

  chDbgCheck(canp != NULL, "canStop");

  chSysLock();
  chDbgAssert((canp->can_state == CAN_STOP) || (canp->can_state == CAN_READY),
              "canStop(), #1",
              "invalid state");
  can_lld_stop(canp);
  canp->can_state = CAN_STOP;
  chSysUnlock();
}

/**
 * @brief Can frame transmission.
 * @details The specified frame is queued for transmission, if the hardware
 *          queue is full then the invoking thread is queued.
 * @note Trying to transmit while in sleep mode simply enqueues the thread.
 *
 * @param canp[in]      pointer to the @p CANDriver object
 * @param cfp[in]       pointer to the CAN frame to be transmitted
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return The operation result.
 * @retval RDY_OK the frame has been queued for transmission.
 * @retval RDY_TIMEOUT operation not finished within the specified time.
 * @retval RDY_RESET driver stopped while waiting.
 */
msg_t canTransmit(CANDriver *canp, const CANFrame *cfp, systime_t timeout) {
  msg_t msg;

  chDbgCheck((canp != NULL) && (cfp != NULL), "canTransmit");

  chSysLock();
  chDbgAssert((canp->can_state == CAN_READY) || (canp->can_state == CAN_SLEEP),
              "canTransmit(), #1",
              "invalid state");
  if ((canp->can_state == CAN_SLEEP) || !can_lld_can_transmit(canp)) {
    msg = chSemWaitTimeoutS(&canp->can_txsem, timeout);
    if (msg != RDY_OK) {
      chSysUnlock();
      return msg;
    }
  }
  msg = can_lld_transmit(canp, cfp);
  chSysUnlock();
  return msg;
}

/**
 * @brief Can frame receive.
 * @details The function waits until a frame is received.
 * @note Trying to receive while in sleep mode simply enqueues the thread.
 *
 * @param canp[in]      pointer to the @p CANDriver object
 * @param cfp[out]      pointer to the buffer where the CAN frame is copied
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return The operation result.
 * @retval RDY_OK a frame has been received and placed in the buffer.
 * @retval RDY_TIMEOUT operation not finished within the specified time.
 * @retval RDY_RESET driver stopped while waiting.
 */
msg_t canReceive(CANDriver *canp, CANFrame *cfp, systime_t timeout) {
  msg_t msg;

  chDbgCheck((canp != NULL) && (cfp != NULL), "canReceive");

  chSysLock();
  chDbgAssert((canp->can_state == CAN_READY) || (canp->can_state == CAN_SLEEP),
              "canReceive(), #1",
              "invalid state");
  if ((canp->can_state == CAN_SLEEP) || !can_lld_can_receive(canp)) {
    msg = chSemWaitTimeoutS(&canp->can_rxsem, timeout);
    if (msg != RDY_OK) {
      chSysUnlock();
      return msg;
    }
  }
  msg = can_lld_receive(canp, cfp);
  chSysUnlock();
  return msg;
}

#if CAN_USE_SLEEP_MODE || defined(__DOXYGEN__)
/**
 * @brief Enters the sleep mode.
 *
 * @param canp[in]      pointer to the @p CANDriver object
 */
void canSleep(CANDriver *canp) {

  chDbgCheck(canp != NULL, "canSleep");

  chSysLock();
  chDbgAssert((canp->can_state == CAN_READY) || (canp->can_state == CAN_SLEEP),
              "canSleep(), #1",
              "invalid state");
  if (canp->can_state = CAN_READY) {
    can_lld_sleep(canp);
    canp->can_state = CAN_SLEEP;
    chEvtBroadcastI(&canp->can_sleep_event);
    chSchRescheduleS();
  }
  chSysUnlock();
}

/**
 * @brief Enforces leaving the sleep mode.
 * @note The sleep mode is supposed to be usually exited automatically by an
 *       hardware event.
 *
 * @param canp[in]      pointer to the @p CANDriver object
 */
void canWakeup(CANDriver *canp) {

  chDbgCheck(canp != NULL, "canWakeup");

  chSysLock();
  chDbgAssert((canp->can_state == CAN_READY) || (canp->can_state == CAN_SLEEP),
              "canWakeup(), #1",
              "invalid state");
  if (canp->can_state = CAN_SLEEP) {
    can_lld_wakeup(canp);
    canp->can_state = CAN_READY;
    chEvtBroadcastI(&canp->can_wakeup_event);
    chSchRescheduleS();
  }
  chSysUnlock();
}
#endif /* CAN_USE_SLEEP_MODE */

/** @} */

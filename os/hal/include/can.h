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
 * @file    can.h
 * @brief   CAN Driver macros and structures.
 *
 * @addtogroup CAN
 * @{
 */

#ifndef _CAN_H_
#define _CAN_H_

#if HAL_USE_CAN || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Errors rate warning.
 */
#define CAN_LIMIT_WARNING           1
/**
 * @brief   Errors rate error.
 */
#define CAN_LIMIT_ERROR             2
/**
 * @brief   Bus off condition reached.
 */
#define CAN_BUS_OFF_ERROR           4
/**
 * @brief   Framing error of some kind on the CAN bus.
 */
#define CAN_FRAMING_ERROR           8
/**
 * @brief   Overflow in receive queue.
 */
#define CAN_OVERFLOW_ERROR          16

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Sleep mode related APIs inclusion switch.
 */
#if !defined(CAN_USE_SLEEP_MODE) || defined(__DOXYGEN__)
#define CAN_USE_SLEEP_MODE          TRUE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !CH_USE_SEMAPHORES || !CH_USE_EVENTS
#error "CAN driver requires CH_USE_SEMAPHORES and CH_USE_EVENTS"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  CAN_UNINIT = 0,                           /**< Not initialized.           */
  CAN_STOP = 1,                             /**< Stopped.                   */
  CAN_STARTING = 2,                         /**< Starting.                  */
  CAN_READY = 3,                            /**< Ready.                     */
  CAN_SLEEP = 4                             /**< Sleep state.               */
} canstate_t;

#include "can_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Adds some flags to the CAN status mask.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mask      flags to be added to the status mask
 *
 * @iclass
 */
#define canAddFlagsI(canp, mask) ((canp)->cd_status |= (mask))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void canInit(void);
  void canObjectInit(CANDriver *canp);
  void canStart(CANDriver *canp, const CANConfig *config);
  void canStop(CANDriver *canp);
  msg_t canTransmit(CANDriver *canp, const CANTxFrame *ctfp, systime_t timeout);
  msg_t canReceive(CANDriver *canp, CANRxFrame *crfp, systime_t timeout);
  canstatus_t canGetAndClearFlags(CANDriver *canp);
#if CAN_USE_SLEEP_MODE
  void canSleep(CANDriver *canp);
  void canWakeup(CANDriver *canp);
#endif /* CAN_USE_SLEEP_MODE */
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_CAN */

#endif /* _CAN_H_ */

/** @} */

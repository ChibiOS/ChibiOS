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
 * @file can.h
 * @brief CAN Driver macros and structures.
 * @addtogroup CAN
 * @{
 */

#ifndef _CAN_H_
#define _CAN_H_

/**
 * @brief Driver state machine possible states.
 */
typedef enum {
  CAN_UNINIT = 0,                           /**< @brief Not initialized.    */
  CAN_STOP = 1,                             /**< @brief Stopped.            */
  CAN_READY = 2,                            /**< @brief Ready.              */
  CAN_SLEEP = 3                             /**< @brief Sleep state.        */
} canstate_t;

#include "can_lld.h"

#ifdef __cplusplus
extern "C" {
#endif
  void canInit(void);
  void canObjectInit(CANDriver *canp);
  void canStart(CANDriver *canp, const CANConfig *config);
  void canStop(CANDriver *canp);
  msg_t canTransmit(CANDriver *canp, const CANFrame *cfp, systime_t timeout);
  msg_t canReceive(CANDriver *canp, CANFrame *cfp, systime_t timeout);
#if CAN_USE_SLEEP_MODE
  void canSleep(CANDriver *canp);
  void canWakeup(CANDriver *canp);
#endif /* CAN_USE_SLEEP_MODE */
#ifdef __cplusplus
}
#endif

#endif /* _CAN_H_ */

/** @} */

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
 * @file STM32/stm32_can.h
 * @brief STM32 CAN driver header file
 * @addtogroup STM32_CAN
 * @{
 */

#ifndef _STM32_CAN_H_
#define _STM32_CAN_H_

typedef struct {
} CANConfig;

typedef struct {
} CANMessage;

#ifdef __cplusplus
extern "C" {
#endif
  void canInit(CANConfig *config);
  bool_t canReceive(CANMessage *canmsg);
  bool_t canTransmit(CANMessage *canmsg);
#ifdef __cplusplus
}
#endif

#endif /* _STM32_CAN_H_ */

/** @} */

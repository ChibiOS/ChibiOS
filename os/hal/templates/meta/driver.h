/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file xxx.h
 * @brief XXX Driver macros and structures.
 * @addtogroup XXX
 * @{
 */

#ifndef _XXX_H_
#define _XXX_H_

#if CH_HAL_USE_XXX || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief Driver state machine possible states.
 */
typedef enum {
  XXX_UNINIT = 0,                           /**< @brief Not initialized.    */
  XXX_STOP = 1,                             /**< @brief Stopped.            */
  XXX_READY = 2,                            /**< @brief Ready.              */
} xxxstate_t;

#include "xxx_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void xxxInit(void);
  void xxxObjectInit(XXXDriver *xxxp);
  void xxxStart(XXXDriver *xxxp, const XXXConfig *config);
  void xxxStop(XXXDriver *xxxp);
#ifdef __cplusplus
}
#endif

#endif /* CH_HAL_USE_XXX */

#endif /* _XXX_H_ */

/** @} */

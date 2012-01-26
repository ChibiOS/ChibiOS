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
*/

/**
 * @file    gpt.h
 * @brief   GPT Driver macros and structures.
 *
 * @addtogroup GPT
 * @{
 */

#ifndef _GPT_H_
#define _GPT_H_

#if HAL_USE_GPT || defined(__DOXYGEN__)

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
 * @brief   Driver state machine possible states.
 */
typedef enum {
  GPT_UNINIT = 0,                   /**< Not initialized.                   */
  GPT_STOP = 1,                     /**< Stopped.                           */
  GPT_READY = 2,                    /**< Ready.                             */
  GPT_CONTINUOUS = 3,               /**< Active in continuous mode.         */
  GPT_ONESHOT = 4                   /**< Active in one shot mode.           */
} gptstate_t;

/**
 * @brief   Type of a structure representing a GPT driver.
 */
typedef struct GPTDriver GPTDriver;

/**
 * @brief   GPT notification callback type.
 *
 * @param[in] gptp      pointer to a @p GPTDriver object
 */
typedef void (*gptcallback_t)(GPTDriver *gptp);

#include "gpt_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void gptInit(void);
  void gptObjectInit(GPTDriver *gptp);
  void gptStart(GPTDriver *gptp, const GPTConfig *config);
  void gptStop(GPTDriver *gptp);
  void gptStartContinuous(GPTDriver *gptp, gptcnt_t interval);
  void gptStartContinuousI(GPTDriver *gptp, gptcnt_t interval);
  void gptStartOneShot(GPTDriver *gptp, gptcnt_t interval);
  void gptStartOneShotI(GPTDriver *gptp, gptcnt_t interval);
  void gptStopTimer(GPTDriver *gptp);
  void gptStopTimerI(GPTDriver *gptp);
  void gptPolledDelay(GPTDriver *gptp, gptcnt_t interval);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_GPT */

#endif /* _GPT_H_ */

/** @} */

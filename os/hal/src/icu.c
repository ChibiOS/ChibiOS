/*
    ChibiOS/HAL - Copyright (C) 2006,2007,2008,2009,2010,
                  2011,2012,2013,2014 Giovanni Di Sirio.

    This file is part of ChibiOS/HAL 

    ChibiOS/HAL is free software; you can redistribute it and/or modify
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
 * @file    icu.c
 * @brief   ICU Driver code.
 *
 * @addtogroup ICU
 * @{
 */

#include "hal.h"

#if HAL_USE_ICU || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   ICU Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void icuInit(void) {

  icu_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p ICUDriver structure.
 *
 * @param[out] icup     pointer to the @p ICUDriver object
 *
 * @init
 */
void icuObjectInit(ICUDriver *icup) {

  icup->state  = ICU_STOP;
  icup->config = NULL;
}

/**
 * @brief   Configures and activates the ICU peripheral.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @param[in] config    pointer to the @p ICUConfig object
 *
 * @api
 */
void icuStart(ICUDriver *icup, const ICUConfig *config) {

  osalDbgCheck((icup != NULL) && (config != NULL));

  osalSysLock();
  osalDbgAssert((icup->state == ICU_STOP) || (icup->state == ICU_READY),
                "invalid state");
  icup->config = config;
  icu_lld_start(icup);
  icup->state = ICU_READY;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the ICU peripheral.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @api
 */
void icuStop(ICUDriver *icup) {

  osalDbgCheck(icup != NULL);

  osalSysLock();
  osalDbgAssert((icup->state == ICU_STOP) || (icup->state == ICU_READY),
                "invalid state");
  icu_lld_stop(icup);
  icup->state = ICU_STOP;
  osalSysUnlock();
}

/**
 * @brief   Starts the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @api
 */
void icuStartCapture(ICUDriver *icup) {

  osalDbgCheck(icup != NULL);

  osalSysLock();
  osalDbgAssert(icup->state == ICU_READY, "invalid state");
  icuStartCaptureI(icup);
  osalSysUnlock();
}

/**
 * @brief   Waits for a completed capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @api
 */
void icuWaitCapture(ICUDriver *icup) {

  osalDbgCheck(icup != NULL);

  osalSysLock();
  osalDbgAssert((icup->state == ICU_WAITING) || (icup->state == ICU_ACTIVE),
                "invalid state");
  icuWaitCaptureI(icup);
  osalSysUnlock();
}

/**
 * @brief   Stops the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @api
 */
void icuStopCapture(ICUDriver *icup) {

  osalDbgCheck(icup != NULL);

  osalSysLock();
  osalDbgAssert((icup->state == ICU_READY) || (icup->state == ICU_WAITING) ||
                (icup->state == ICU_ACTIVE),
                "invalid state");
  icuStopCaptureI(icup);
  osalSysUnlock();
}

/**
 * @brief   Enables notifications.
 * @pre     The ICU unit must have been activated using @p icuStart().
 * @note    If the notification is already enabled then the call has no effect.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @api
 */
void icuEnableNotifications(ICUDriver *icup) {

  osalDbgCheck(icup != NULL);

  osalSysLock();
  osalDbgAssert((icup->state == ICU_WAITING) || (icup->state == ICU_ACTIVE),
                "invalid state");
  icuEnableNotificationsI(icup);
  osalSysUnlock();
}

/**
 * @brief   Disables notifications.
 * @pre     The ICU unit must have been activated using @p icuStart().
 * @note    If the notification is already disabled then the call has no effect.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @api
 */
void icuDisableNotifications(ICUDriver *icup) {

  osalDbgCheck(icup != NULL);

  osalSysLock();
  osalDbgAssert((icup->state == ICU_WAITING) || (icup->state == ICU_ACTIVE),
                "invalid state");
  icuDisableNotificationsI(icup);
  osalSysUnlock();
}

#endif /* HAL_USE_ICU */

/** @} */

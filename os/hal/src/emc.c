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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    emc.c
 * @brief   EMC Driver code.
 *
 * @addtogroup EMC
 * @{
 */

#include "hal.h"

#if HAL_USE_EMC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local types.                                                       */
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
 * @brief   EMC Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void emcInit(void) {

  emc_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p EMCDriver structure.
 *
 * @param[out] emcp     pointer to the @p EMCDriver object
 *
 * @init
 */
void emcObjectInit(EMCDriver *emcp) {

  emcp->state  = EMC_STOP;
  emcp->config = NULL;
}

/**
 * @brief   Configures and activates the EMC peripheral.
 *
 * @param[in] emcp      pointer to the @p EMCDriver object
 * @param[in] config    pointer to the @p EMCConfig object
 *
 * @api
 */
void emcStart(EMCDriver *emcp, const EMCConfig *config) {

  osalDbgCheck((emcp != NULL) && (config != NULL));

  osalSysLock();
  osalDbgAssert((emcp->state == EMC_STOP) || (emcp->state == EMC_READY),
              "invalid state");
  emcp->config = config;
  emc_lld_start(emcp);
  emcp->state = EMC_READY;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the EMC peripheral.
 *
 * @param[in] emcp      pointer to the @p EMCDriver object
 *
 * @api
 */
void emcStop(EMCDriver *emcp) {

  osalDbgCheck(emcp != NULL);

  osalSysLock();
  osalDbgAssert((emcp->state == EMC_STOP) || (emcp->state == EMC_READY),
              "invalid state");
  emc_lld_stop(emcp);
  emcp->state = EMC_STOP;
  osalSysUnlock();
}

#endif /* HAL_USE_EMC */

/** @} */

/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    i2s.c
 * @brief   I2S Driver code.
 *
 * @addtogroup I2S
 * @{
 */

#include "hal.h"

#if (HAL_USE_I2S == TRUE) || defined(__DOXYGEN__)

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
 * @brief   I2S Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void i2sInit(void) {

  i2s_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p I2SDriver structure.
 *
 * @param[out] i2sp     pointer to the @p I2SDriver object
 *
 * @init
 */
void i2sObjectInit(I2SDriver *i2sp) {

  i2sp->state  = I2S_STOP;
  i2sp->config = NULL;
}

/**
 * @brief   Configures and activates the I2S peripheral.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 * @param[in] config    pointer to the @p I2SConfig object
 *
 * @api
 */
void i2sStart(I2SDriver *i2sp, const I2SConfig *config) {

  osalDbgCheck((i2sp != NULL) && (config != NULL));

  osalSysLock();
  osalDbgAssert((i2sp->state == I2S_STOP) || (i2sp->state == I2S_READY),
                "invalid state");
  i2sp->config = config;
  i2s_lld_start(i2sp);
  i2sp->state = I2S_READY;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the I2S peripheral.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 *
 * @api
 */
void i2sStop(I2SDriver *i2sp) {

  osalDbgCheck(i2sp != NULL);

  osalSysLock();
  osalDbgAssert((i2sp->state == I2S_STOP) || (i2sp->state == I2S_READY),
                "invalid state");
  i2s_lld_stop(i2sp);
  i2sp->state = I2S_STOP;
  osalSysUnlock();
}

/**
 * @brief   Starts a I2S data exchange.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 *
 * @api
 */
void i2sStartExchange(I2SDriver *i2sp) {

  osalDbgCheck(i2sp != NULL);

  osalSysLock();
  osalDbgAssert(i2sp->state == I2S_READY, "not ready");
  i2sStartExchangeI(i2sp);
  osalSysUnlock();
}

/**
 * @brief   Stops the ongoing data exchange.
 * @details The ongoing data exchange, if any, is stopped, if the driver
 *          was not active the function does nothing.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 *
 * @api
 */
void i2sStopExchange(I2SDriver *i2sp) {

  osalDbgCheck((i2sp != NULL));

  osalSysLock();
  osalDbgAssert((i2sp->state == I2S_READY) ||
                (i2sp->state == I2S_ACTIVE) ||
                (i2sp->state == I2S_COMPLETE),
                "invalid state");
  i2sStopExchangeI(i2sp);
  osalSysUnlock();
}

#endif /* HAL_USE_I2S == TRUE */

/** @} */

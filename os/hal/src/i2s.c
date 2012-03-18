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
 * @file    i2s.c
 * @brief   I2S Driver code.
 *
 * @addtogroup I2S
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_I2S || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

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

  chDbgCheck((i2sp != NULL) && (config != NULL), "i2sStart");

  chSysLock();
  chDbgAssert((i2sp->state == I2S_STOP) || (i2sp->state == I2S_READY),
              "i2sStart(), #1", "invalid state");
  i2sp->config = config;
  i2s_lld_start(i2sp);
  i2sp->state = I2S_READY;
  chSysUnlock();
}

/**
 * @brief   Deactivates the I2S peripheral.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 *
 * @api
 */
void i2sStop(I2SDriver *i2sp) {

  chDbgCheck(i2sp != NULL, "i2sStop");

  chSysLock();
  chDbgAssert((i2sp->state == I2S_STOP) || (i2sp->state == I2S_READY),
              "i2sStop(), #1", "invalid state");
  i2s_lld_stop(i2sp);
  i2sp->state = I2S_STOP;
  chSysUnlock();
}

/**
 * @brief   Starts a I2S data exchange.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 *
 * @api
 */
void i2sStartExchange(I2SDriver *i2sp) {

  chDbgCheck(i2sp != NULL "i2sStartExchange");

  chSysLock();
  chDbgAssert(i2sp->state == I2S_READY,
              "i2sStartExchange(), #1", "not ready");
  i2sStartExchangeI(i2sp);
  chSysUnlock();
}

/**
 * @brief   Starts a I2S data exchange in continuous mode.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 *
 * @api
 */
void i2sStartExchangeContinuous(I2SDriver *i2sp) {

  chDbgCheck(i2sp != NULL "i2sStartExchangeContinuous");

  chSysLock();
  chDbgAssert(i2sp->state == I2S_READY,
              "i2sStartExchangeContinuous(), #1", "not ready");
  i2sStartExchangeContinuousI(i2sp);
  chSysUnlock();
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

  chDbgCheck((i2sp != NULL), "i2sStopExchange");

  chSysLock();
  chDbgAssert((i2sp->state == I2S_READY) ||
              (i2sp->state == I2S_ACTIVE) ||
              (i2sp->state == I2S_COMPLETE),
              "i2sStopExchange(), #1", "not ready");
  i2sStopExchangeI(i2sp);
  chSysUnlock();
}

#endif /* HAL_USE_I2S */

/** @} */

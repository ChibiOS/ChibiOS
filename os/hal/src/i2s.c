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
 * @brief   Starts a I2S transmission.
 * @details The transmission is started and it is executes until the specified
 *          buffer has entirely transmitted.
 * @post    A callback is invoked when the buffer has been fully transmitted.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 * @param[in] n         number of samples to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @api
 */
void i2sStartSendOnce(I2SDriver *i2sp, size_t n,
                      const i2ssample_t *txbuf) {

  chDbgCheck((i2sp != NULL) && (n > 0) && (txbuf != NULL),
             "i2sStartSendOnce");

  chSysLock();
  chDbgAssert(i2sp->state == I2S_READY, "i2sStartSendOnce(), #1",
              "not ready");
  i2sStartSendOnceI(i2sp, n, txbuf);
  chSysUnlock();
}

/**
 * @brief   Starts a continuous I2S transmission.
 * @details The transmission is started and it is executes continuously
 *          until @p i2sStopSend() has been explicitly invoked .
 * @post    A callback is invoked when the buffer has been half transmitted
 *          and fully transmitted.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 * @param[in] n         number of samples to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @api
 */
void i2sStartSendContinuous(I2SDriver *i2sp, size_t n,
                            const i2ssample_t *txbuf) {

  chDbgCheck((i2sp != NULL) && (n > 0) && (txbuf != NULL),
             "i2sStartSendContinuous");

  chSysLock();
  chDbgAssert(i2sp->state == I2S_READY, "i2sStartSendContinuous(), #1",
              "not ready");
  i2sStartSendContinuousI(i2sp, n, txbuf);
  chSysUnlock();
}

/**
 * @brief   Stops the ongoing transmission.
 * @details The ongoing transmission, if any, is stopped, if the driver
 *          was not transmitting the function does nothing.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 *
 * @api
 */
void i2sStopSend(I2SDriver *i2sp) {

  chDbgCheck((i2sp != NULL), "i2sStopSend");

  chSysLock();
  chDbgAssert((i2sp->state == I2S_READY) ||
              (i2sp->state == I2S_ACTIVE) ||
              (i2sp->state == I2S_COMPLETE),
              "i2sStopSend(), #1", "not ready");
  i2sStopSendI(i2sp);
  chSysUnlock();
}

#endif /* HAL_USE_I2S */

/** @} */

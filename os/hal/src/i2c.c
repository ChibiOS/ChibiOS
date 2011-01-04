/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    i2c.c
 * @brief   I2C Driver code.
 *
 * @addtogroup I2C
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

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
 * @brief   I2C Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void i2cInit(void) {

  i2c_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p I2CDriver structure.
 *
 * @param[out] i2cp     pointer to the @p I2CDriver object
 *
 * @init
 */
void i2cObjectInit(I2CDriver *i2cp) {

  i2cp->i2c_state  = I2C_STOP;
  i2cp->i2c_config = NULL;
#if defined(I2C_DRIVER_EXT_INIT_HOOK)
  I2C_DRIVER_EXT_INIT_HOOK(i2cp);
#endif
}

/**
 * @brief   Configures and activates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] config    pointer to the @p I2CConfig object
 *
 * @api
 */
void i2cStart(I2CDriver *i2cp, const I2CConfig *config) {

  chDbgCheck((i2cp != NULL) && (config != NULL), "i2cStart");

  chSysLock();
  chDbgAssert((i2cp->i2c_state == I2C_STOP) || (i2cp->i2c_state == I2C_READY),
              "i2cStart(), #1",
              "invalid state");
  i2cp->i2c_config = config;
  i2c_lld_start(i2cp);
  i2cp->i2c_state = I2C_READY;
  chSysUnlock();
}

/**
 * @brief   Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @api
 */
void i2cStop(I2CDriver *i2cp) {

  chDbgCheck(i2cp != NULL, "i2cStop");

  chSysLock();
  chDbgAssert((i2cp->i2c_state == I2C_STOP) || (i2cp->i2c_state == I2C_READY),
              "i2cStop(), #1",
              "invalid state");
  i2c_lld_stop(i2cp);
  i2cp->i2c_state = I2C_STOP;
  chSysUnlock();
}

/**
 * @brief   Initiates a master bus transaction.
 * @details This function sends a start bit followed by an one or two bytes
 *          header.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] header    transaction header
 * @param[in] callback  operation complete callback
 *
 * @iclass
 */
void i2cMasterStartI(I2CDriver *i2cp,
                     uint16_t header,
                     i2ccallback_t callback) {

  chDbgCheck((i2cp != NULL) && (callback != NULL), "i2cMasterStartI");
  chDbgAssert(i2cp->i2c_state == I2C_READY,
              "i2cMasterStartI(), #1", "invalid state");

  i2cp->id_callback = callback;
  i2c_lld_master_start(i2cp, header);
}

/**
 * @brief   Terminates a master bus transaction.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] callback  operation complete callback
 *
 * @iclass
 */
void i2cMasterStopI(I2CDriver *i2cp, i2ccallback_t callback) {

  chDbgCheck((i2cp != NULL) && (callback != NULL), "i2cMasterStopI");
  chDbgAssert(i2cp->i2c_state == I2C_MREADY,
              "i2cMasterStopI(), #1", "invalid state");

  i2cp->id_callback = callback;
  i2c_lld_master_stop(i2cp);
}


/**
 * @brief   Sends a restart bit.
 * @details Restart bits are required by some types of I2C transactions.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] callback  operation complete callback
 *
 * @iclass
 */
void i2cMasterRestartI(I2CDriver *i2cp, i2ccallback_t callback) {

  chDbgCheck((i2cp != NULL) && (callback != NULL), "i2cMasterRestartI");
  chDbgAssert(i2cp->i2c_state == I2C_MREADY,
              "i2cMasterRestartI(), #1", "invalid state");

  i2cp->id_callback = callback;
  i2c_lld_master_restart(i2cp);
}

/**
 * @brief   Master transmission.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] n         number of bytes to be transmitted
 * @param[in] txbuf     transmit data buffer pointer
 * @param[in] callback  operation complete callback
 *
 * @iclass
 */
void i2cMasterTransmitI(I2CDriver *i2cp, size_t n, const uint8_t *txbuf,
                        i2ccallback_t callback) {

  chDbgCheck((i2cp != NULL) && (n > 0) &&
             (txbuf != NULL) && (callback != NULL), "i2cMasterTransmitI");
  chDbgAssert(i2cp->i2c_state == I2C_MREADY,
              "i2cMasterTransmitI(), #1", "invalid state");

  i2cp->id_callback = callback;
  i2c_lld_master_transmit(i2cp, n, txbuf);
}

/**
 * @brief   Master receive.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] n         number of bytes to be transmitted
 * @param[in] rxbuf     receive data buffer pointer
 * @param[in] callback  operation complete callback
 *
 * @iclass
 */
void i2cMasterReceiveI(I2CDriver *i2cp, size_t n, uint8_t *rxbuf,
                       i2ccallback_t callback) {

  chDbgCheck((i2cp != NULL) && (n > 0) &&
             (rxbuf != NULL) && (callback != NULL), "i2cMasterReceiveI");
  chDbgAssert(i2cp->i2c_state == I2C_MREADY,
              "i2cMasterReceiveI(), #1", "invalid state");

  i2cp->id_callback = callback;
  i2c_lld_master_receive(i2cp, n, rxbuf);
}

#if I2C_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
/**
 * @brief   Gains exclusive access to the I2C bus.
 * @details This function tries to gain ownership to the I2C bus, if the bus
 *          is already being used then the invoking thread is queued.
 * @pre     In order to use this function the option @p I2C_USE_MUTUAL_EXCLUSION
 *          must be enabled.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @api
 *
 */
void i2cAcquireBus(I2CDriver *i2cp) {

  chDbgCheck(i2cp != NULL, "i2cAcquireBus");

#if CH_USE_MUTEXES
  chMtxLock(&i2cp->id_mutex);
#elif CH_USE_SEMAPHORES
  chSemWait(&i2cp->id_semaphore);
#endif
}

/**
 * @brief   Releases exclusive access to the I2C bus.
 * @pre     In order to use this function the option @p I2C_USE_MUTUAL_EXCLUSION
 *          must be enabled.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @api
 */
void i2cReleaseBus(I2CDriver *i2cp) {

  chDbgCheck(i2cp != NULL, "i2cReleaseBus");

#if CH_USE_MUTEXES
  (void)i2cp;
  chMtxUnlock();
#elif CH_USE_SEMAPHORES
  chSemSignal(&i2cp->id_semaphore);
#endif
}
#endif /* I2C_USE_MUTUAL_EXCLUSION */

#endif /* HAL_USE_I2C */

/** @} */

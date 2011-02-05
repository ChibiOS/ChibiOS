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

#include "ch.h"
#include "hal.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

/**
 * @brief I2C Driver initialization.
 */
void i2cInit(void) {

  i2c_lld_init();
}

/**
 * @brief Initializes the standard part of a @p I2CDriver structure.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
void i2cObjectInit(I2CDriver *i2cp) {
  chEvtInit(&i2cp->sevent);
  i2cp->errors = I2CD_NO_ERROR;
  i2cp->state = I2C_STOP;
//  i2cp->i2cd_config = NULL;
#if I2C_USE_WAIT
  i2cp->thread = NULL;
#endif /* I2C_USE_WAIT */
#if I2C_USE_MUTUAL_EXCLUSION
#if CH_USE_MUTEXES
  chMtxInit(&i2cp->mutex);
#elif CH_USE_SEMAPHORES
  chSemInit(&i2cp->semaphore, 1);
#endif
#endif /* I2C_USE_MUTUAL_EXCLUSION */
#if defined(I2C_DRIVER_EXT_INIT_HOOK)
  I2C_DRIVER_EXT_INIT_HOOK(i2cp);
#endif
}

/**
 * @brief Configures and activates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] config    pointer to the @p I2CConfig object
 */
void i2cStart(I2CDriver *i2cp, const I2CConfig *config) {

  chDbgCheck((i2cp != NULL) && (config != NULL), "i2cStart");

  chSysLock();
  chDbgAssert((i2cp->state == I2C_STOP)||(i2cp->state == I2C_READY),
              "i2cStart(), #1", "invalid state");

  i2cp->nbit_address = config->nBitAddress;
  i2c_lld_start(i2cp);
  i2c_lld_set_clock(i2cp, config->ClockSpeed, config->FastModeDutyCycle);
  i2c_lld_set_opmode(i2cp, config->opMode);
  i2c_lld_set_own_address(i2cp, config->OwnAddress1, config->nBitAddress);
  i2cp->state = I2C_READY;
  chSysUnlock();
}

/**
 * @brief Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 */
void i2cStop(I2CDriver *i2cp) {

  chDbgCheck(i2cp != NULL, "i2cStop");

  chSysLock();
  chDbgAssert((i2cp->state == I2C_STOP) || (i2cp->state == I2C_READY),
              "i2cStop(), #1", "invalid state");
  i2c_lld_stop(i2cp);
  i2cp->state = I2C_STOP;
  chSysUnlock();
}

/**
 * @brief Sends data ever the I2C bus.
 *
 * @param[in] i2cp           pointer to the @p I2CDriver object
 * @param[in] slave_addr     7-bit or 10-bit address of the slave
 * @param[in] n              number of words to send
 * @param[in] txbuf          the pointer to the transmit buffer
 *
 */
void i2cMasterTransmit(I2CDriver *i2cp, uint16_t slave_addr, size_t n, void *txbuf) {

  chDbgCheck((i2cp != NULL) && (n > 0) && (txbuf != NULL),
             "i2cMasterTransmit");

#if I2C_USE_WAIT
  i2c_lld_wait_bus_free(i2cp);
  if(i2c_lld_bus_is_busy(i2cp)) {
#ifdef PRINTTRACE
    print("I2C Bus busy!\n");
#endif
    return;
  };
#endif

  chSysLock();
  chDbgAssert(i2cp->state == I2C_READY,
              "i2cMasterTransmit(), #1", "not ready");

  i2cp->state = I2C_ACTIVE;
  i2c_lld_master_transmit(i2cp, slave_addr, n, txbuf);
  _i2c_wait_s(i2cp);
#if !I2C_USE_WAIT
  i2c_lld_wait_bus_free(i2cp);
#endif
  if (i2cp->state == I2C_COMPLETE)
    i2cp->state = I2C_READY;
  chSysUnlock();
}

/**
 * @brief Receives data from the I2C bus.
 *
 * @param[in] i2cp          pointer to the @p I2CDriver object
 * @param[in] slave_addr    7-bit or 10-bit address of the slave
 * @param[in] n             number of bytes to receive
 * @param[out] rxbuf        the pointer to the receive buffer
 *
 */
void i2cMasterReceive(I2CDriver *i2cp, uint16_t slave_addr, size_t n, void *rxbuf) {

  chDbgCheck((i2cp != NULL) && (n > 0) && (rxbuf != NULL),
             "i2cMasterReceive");

#if I2C_USE_WAIT
  i2c_lld_wait_bus_free(i2cp);
  if(i2c_lld_bus_is_busy(i2cp)) {
#ifdef PRINTTRACE
    print("I2C Bus busy!\n");
#endif
    return;
  };
#endif

  chSysLock();
  chDbgAssert(i2cp->state == I2C_READY,
              "i2cMasterReceive(), #1", "not ready");

  i2cp->state = I2C_ACTIVE;
  i2c_lld_master_receive(i2cp, slave_addr, n, rxbuf);
  _i2c_wait_s(i2cp);
#if !I2C_USE_WAIT
  i2c_lld_wait_bus_free(i2cp);
#endif
  if (i2cp->state == I2C_COMPLETE)
    i2cp->state = I2C_READY;
  chSysUnlock();
}

uint16_t i2cSMBusAlertResponse(I2CDriver *i2cp) {
  uint16_t slv_addr;

  i2cMasterReceive(i2cp, 0x0C, 2, &slv_addr);
  return slv_addr;
}


/**
 * @brief   Handles communication events/errors.
 * @details Must be called from the I/O interrupt service routine in order to
 *          notify I/O conditions as errors, signals change etc.
 *
 * @param[in] i2cp      pointer to a @p I2CDriver structure
 * @param[in] mask      condition flags to be added to the mask
 *
 * @iclass
 */
void i2cAddFlagsI(I2CDriver *i2cp, i2cflags_t mask) {

  chDbgCheck(i2cp != NULL, "i2cAddFlagsI");

  i2cp->errors |= mask;
  chEvtBroadcastI(&i2cp->sevent);
}

/**
 * @brief   Returns and clears the errors mask associated to the driver.
 *
 * @param[in] i2cp      pointer to a @p I2CDriver structure
 * @return              The condition flags modified since last time this
 *                      function was invoked.
 *
 * @api
 */
i2cflags_t i2cGetAndClearFlags(I2CDriver *i2cp) {
  i2cflags_t mask;

  chDbgCheck(i2cp != NULL, "i2cGetAndClearFlags");

  chSysLock();
  mask = i2cp->errors;
  i2cp->errors = I2CD_NO_ERROR;
  chSysUnlock();
  return mask;
}



#if I2C_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
/**
 * @brief Gains exclusive access to the I2C bus.
 * @details This function tries to gain ownership to the I2C bus, if the bus
 *          is already being used then the invoking thread is queued.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @note This function is only available when the @p I2C_USE_MUTUAL_EXCLUSION
 *       option is set to @p TRUE.
 */
void i2cAcquireBus(I2CDriver *i2cp) {

  chDbgCheck(i2cp != NULL, "i2cAcquireBus");

#if CH_USE_MUTEXES
  chMtxLock(&i2cp->mutex);
#elif CH_USE_SEMAPHORES
  chSemWait(&i2cp->semaphore);
#endif
}

/**
 * @brief Releases exclusive access to the I2C bus.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @note This function is only available when the @p I2C_USE_MUTUAL_EXCLUSION
 *       option is set to @p TRUE.
 */
void i2cReleaseBus(I2CDriver *i2cp) {

  chDbgCheck(i2cp != NULL, "i2cReleaseBus");

#if CH_USE_MUTEXES
  (void)i2cp;
  chMtxUnlock();
#elif CH_USE_SEMAPHORES
  chSemSignal(&i2cp->semaphore);
#endif
}
#endif /* I2C_USE_MUTUAL_EXCLUSION */

#endif /* CH_HAL_USE_I2C */

/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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

  i2cp->id_state  = I2C_STOP;
  i2cp->id_config = NULL;
  i2cp->rxbuff_p = NULL;
  i2cp->txbuff_p = NULL;
  i2cp->rxbuf = NULL;
  i2cp->txbuf = NULL;
  i2cp->id_slave_config = NULL;

#if I2C_USE_WAIT
  i2cp->id_thread   = NULL;
#endif /* I2C_USE_WAIT */

#if I2C_USE_MUTUAL_EXCLUSION
#if CH_USE_MUTEXES
  chMtxInit(&i2cp->id_mutex);
#else
  chSemInit(&i2cp->id_semaphore, 1);
#endif /* CH_USE_MUTEXES */
#endif /* I2C_USE_MUTUAL_EXCLUSION */

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
  chDbgAssert((i2cp->id_state == I2C_STOP) || (i2cp->id_state == I2C_READY),
              "i2cStart(), #1",
              "invalid state");

#if (!(STM32_I2C_I2C2_USE_POLLING_WAIT) && I2C_SUPPORTS_CALLBACKS)
    gptStart(i2cp->timer, i2cp->timer_cfg);
#endif /* !(STM32_I2C_I2C2_USE_POLLING_WAIT) */

  chSysLock();
  i2cp->id_config = config;
  i2c_lld_start(i2cp);
  i2cp->id_state = I2C_READY;
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
  chDbgAssert((i2cp->id_state == I2C_STOP) || (i2cp->id_state == I2C_READY),
              "i2cStop(), #1",
              "invalid state");

#if (!(STM32_I2C_I2C2_USE_POLLING_WAIT) && I2C_SUPPORTS_CALLBACKS)
    gptStop(i2cp->timer);
#endif /* !(STM32_I2C_I2C2_USE_POLLING_WAIT) */

  chSysLock();
  i2c_lld_stop(i2cp);
  i2cp->id_state = I2C_STOP;
  chSysUnlock();
}

/**
 * @brief Sends data via the I2C bus.
 *
 * @details Function designed to realize "read-through-write" transfer
 *          paradigm. If you want transmit data without any further read,
 *          than set @b rxbytes field to 0.
 *
 * @param[in] i2cp        pointer to the @p I2CDriver object
 * @param[in] i2cscfg     pointer to the @p I2C slave config
 * @param[in] slave_addr  Slave device address. Bits 0-9 contain slave
 *                        device address. Bit 15 must be set to 1 if 10-bit
 *                        addressing mode used. Otherwise	keep it cleared.
 *                        Bits 10-14 unused.
 * @param[in] txbuf       pointer to transmit buffer
 * @param[in] txbytes     number of bytes to be transmitted
 * @param[in] rxbuf       pointer to receive buffer
 * @param[in] rxbytes     number of bytes to be received, set it to 0 if
 *                        you want transmit only
 */
void i2cMasterTransmit(I2CDriver *i2cp,
                      const I2CSlaveConfig *i2cscfg,
                      uint16_t slave_addr,
                      uint8_t *txbuf,
                      size_t txbytes,
                      uint8_t *rxbuf,
                      size_t rxbytes) {

  chDbgCheck((i2cp != NULL) && (i2cscfg != NULL) &&\
  		(slave_addr != 0) &&\
  		(txbytes > 0) &&\
  		(txbuf != NULL),
  		"i2cMasterTransmit");

  /* init slave config field in driver */
  i2cp->id_slave_config = i2cscfg;

  i2c_lld_wait_bus_free(i2cp);
  chDbgAssert(!(i2c_lld_bus_is_busy(i2cp)), "i2cMasterReceive(), #1", "time is out");

  chDbgAssert(i2cp->id_state == I2C_READY,
              "i2cMasterTransmit(), #1", "not ready");

  i2cp->id_state = I2C_ACTIVE_TRANSMIT;
  i2c_lld_master_transmit(i2cp, slave_addr, txbuf, txbytes, rxbuf, rxbytes);
#if I2C_SUPPORTS_CALLBACKS
  _i2c_wait_s(i2cp);
#else
  i2cp->id_state = I2C_READY;
#endif /* I2C_SUPPORTS_CALLBACKS */
}

/**
 * @brief Receives data from the I2C bus.
 *
 * @param[in] i2cp        pointer to the @p I2CDriver object
 * @param[in] i2cscfg     pointer to the @p I2C slave config
 * @param[in] slave_addr  Slave device address. Bits 0-9 contain slave
 *                        device address. Bit 15 must be set to 1 if 10-bit
 *                        addressing mode used. Otherwise	keep it cleared.
 *                        Bits 10-14 unused.
 * @param[in] rxbytes     number of bytes to be received
 * @param[in] rxbuf       pointer to receive buffer
 */
void i2cMasterReceive(I2CDriver *i2cp,
                      const I2CSlaveConfig *i2cscfg,
                      uint16_t slave_addr,
                      uint8_t *rxbuf,
                      size_t rxbytes){

  chDbgCheck((i2cp != NULL) && (i2cscfg != NULL) &&\
  		(slave_addr != 0) &&\
  		(rxbytes > 0) && \
  		(rxbuf != NULL),
      "i2cMasterReceive");

  /* init slave config field in driver */
  i2cp->id_slave_config = i2cscfg;

  i2c_lld_wait_bus_free(i2cp);
  chDbgAssert(!(i2c_lld_bus_is_busy(i2cp)), "i2cMasterReceive(), #1", "time is out");

  chDbgAssert(i2cp->id_state == I2C_READY,
              "i2cMasterReceive(), #1", "not ready");

  i2cp->id_state = I2C_ACTIVE_RECEIVE;
  i2c_lld_master_receive(i2cp, slave_addr, rxbuf, rxbytes);
#if I2C_SUPPORTS_CALLBACKS
  _i2c_wait_s(i2cp);
#else
  i2cp->id_state = I2C_READY;
#endif /* I2C_SUPPORTS_CALLBACKS */
}


/* FIXME: I do not know what this function must do. And can not test it
uint16_t i2cSMBusAlertResponse(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg) {
  i2cMasterReceive(i2cp, i2cscfg);
  return i2cp->id_slave_config->slave_addr;
}
*/

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
  chMtxLock(&i2cp->id_mutex);
#elif CH_USE_SEMAPHORES
  chSemWait(&i2cp->id_semaphore);
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
  chSemSignal(&i2cp->id_semaphore);
#endif
}
#endif /* I2C_USE_MUTUAL_EXCLUSION */

#endif /* HAL_USE_I2C */

/** @} */

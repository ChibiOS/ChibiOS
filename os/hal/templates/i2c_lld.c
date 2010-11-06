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
 * @file    templates/i2c_lld.c
 * @brief   I2C Driver subsystem low level driver source template.
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
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level I2C driver initialization.
 *
 * @notapi
 */
void i2c_lld_init(void) {

}

/**
 * @brief   Configures and activates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_start(I2CDriver *i2cp) {

  if (i2cp->i2c_state == I2C_STOP) {
    /* Clock activation.*/
  }
  /* Configuration.*/
}

/**
 * @brief   Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_stop(I2CDriver *i2cp) {

}

/**
 * @brief   Initiates a master bus transaction.
 * @details This function sends a start bit followed by an one or two bytes
 *          header.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] header    transaction header
 *
 * @notapi
 */
void i2c_lld_master_start(I2CDriver *i2cp, uint16_t header) {

}

/**
 * @brief   Terminates a master bus transaction.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_master_stop(I2CDriver *i2cp) {

}

/**
 * @brief   Sends a restart bit.
 * @details Restart bits are required by some types of I2C transactions.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_master_restart(I2CDriver *i2cp) {

}

/**
 * @brief   Master transmission.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] n         number of bytes to be transmitted
 * @param[in] txbuf     transmit data buffer pointer
 *
 * @notapi
 */
void i2c_lld_master_transmit(I2CDriver *i2cp, size_t n,
                             const uint8_t *txbuf) {

}

/**
 * @brief   Master receive.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] n         number of bytes to be transmitted
 * @param[in] rxbuf     receive data buffer pointer
 *
 * @notapi
 */
void i2c_lld_master_receive(I2CDriver *i2cp, size_t n, uint8_t *rxbuf) {

}

#endif /* HAL_USE_I2C */

/** @} */

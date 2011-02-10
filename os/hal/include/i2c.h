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
 * @file    i2c.h
 * @brief   I2C Driver macros and structures.
 *
 * @addtogroup I2C
 * @{
 */

#ifndef _I2C_H_
#define _I2C_H_

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/
#define  I2CD_NO_ERROR                  0
/** @brief Bus Error.*/
#define  I2CD_BUS_ERROR                 0x01
/** @brief Arbitration Lost (master mode).*/
#define  I2CD_ARBITRATION_LOST          0x02
/** @brief Acknowledge Failure.*/
#define  I2CD_ACK_FAILURE               0x04
/** @brief Overrun/Underrun.*/
#define  I2CD_OVERRUN                   0x08
/** @brief PEC Error in reception.*/
#define  I2CD_PEC_ERROR                 0x10
/** @brief Timeout or Tlow Error.*/
#define  I2CD_TIMEOUT                   0x20
/** @brief SMBus Alert.*/
#define  I2CD_SMB_ALERT                 0x40
/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Enables the mutual exclusion APIs on the I2C bus.
 */
#if !defined(I2C_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define I2C_USE_MUTUAL_EXCLUSION    TRUE
#endif

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
  I2C_UNINIT = 0,             /**< Not initialized.                          */
  I2C_STOP = 1,               /**< Stopped.                                  */
  I2C_READY = 2,              /**< Ready. Start condition generated.         */
  I2C_MACTIVE = 3,            /**< I2C configured and waiting start cond.    */
  I2C_10BIT_HANDSHAKE = 4,    /**< 10-bit address sending                    */
  I2C_MWAIT_ADDR_ACK = 5,     /**< Waiting ACK on address sending.           */
  I2C_MTRANSMIT = 6,          /**< Master transmitting.                      */
  I2C_MRECEIVE = 7,           /**< Master receiving.                         */
  I2C_MWAIT_TF = 8,           /**< Master wait Transmission Finished         */
  I2C_MERROR = 9,             /**< Error condition.                          */

  // slave part
  I2C_SACTIVE = 10,
  I2C_STRANSMIT = 11,
  I2C_SRECEIVE = 12,
} i2cstate_t;

#include "i2c_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Read mode.
 */
#define I2C_READ                            1

/**
 * @brief   Write mode.
 */
#define I2C_WRITE                           0

/**
 * @brief   Seven bits addresses header builder.
 *
 * @param[in] addr      seven bits address value
 * @param[in] rw        read/write flag
 *
 * @return              A 16 bit value representing the header, the most
 *                      significant byte is always zero.
 */
#define I2C_ADDR7(addr, rw) (uint16_t)((addr) << 1 | (rw))


/**
 * @brief   Ten bits addresses header builder.
 *
 * @param[in] addr      ten bits address value
 * @param[in] rw        read/write flag
 *
 * @return              A 16 bit value representing the header, the most
 *                      significant byte is the first one to be transmitted.
 */
#define I2C_ADDR10(addr, rw)                                                \
    (uint16_t)(0xF000 |                                                     \
               (((addr) & 0x0300) << 1) |                                   \
               (((rw) << 8)) |                                              \
               ((addr) & 0x00FF))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif
  void i2cInit(void);
  void i2cObjectInit(I2CDriver *i2cp);
  void i2cStart(I2CDriver *i2cp, I2CConfig *config);
  void i2cStop(I2CDriver *i2cp);
  void i2cMasterTransmit(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg);
  void i2cMasterReceive(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg);


  void i2cMasterStartI(I2CDriver *i2cp,uint16_t header);
  void i2cMasterStopI(I2CDriver *i2cp);
  void i2cMasterRestartI(I2CDriver *i2cp);
  void i2cMasterTransmitI(I2CDriver *i2cp, size_t n, const uint8_t *txbuf);
  void i2cMasterReceiveI(I2CDriver *i2cp, size_t n, uint8_t *rxbuf);
#if I2C_USE_MUTUAL_EXCLUSION
  void i2cAcquireBus(I2CDriver *i2cp);
  void i2cReleaseBus(I2CDriver *i2cp);
#endif /* I2C_USE_MUTUAL_EXCLUSION */
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_I2C */

#endif /* _I2C_H_ */

/** @} */

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

/**
 * @brief   I2C notification callback type.
 * @details This callback invoked when byte transfer finish event generated,
 *          No matter sending or reading. This function designed
 *          for sending (re)start or stop events to I2C bus.
 *          Use "restart" boolean flag
 *          in I2CSlaveConfig structure for this needs.
 *          Each slave can (must?) have its own callback function.
 *
 *          If callback function is set to NULL - driver generate stop
 *          condition on the bus after the transfer finish.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object triggering the
 *                      callback
 * @param[in] i2cscfg   pointer to the @p I2CSlaveConfig object triggering the
 *                      callback
 */
typedef void (*i2ccallback_t)(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg);


/**
 * @brief   I2C error notification callback type.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object triggering the
 *                      callback
 * @param[in] i2cscfg   pointer to the @p I2CSlaveConfig object triggering the
 *                      callback
 */
typedef void (*i2cerrorcallback_t)(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg);


/**
 * @brief I2C transmission data block size.
 */
typedef uint8_t i2cblock_t;


/**
 * @brief Structure representing an I2C slave configuration.
 * @details TODO: write about befers
 */
struct I2CSlaveConfig{
  /**
   * @brief Callback pointer.
   * @note  Transfer finished callback. Invoke when all data transferred, or
   *        by DMA buffer events
   *        If set to @p NULL then the callback is disabled.
   */
  i2ccallback_t         id_callback;

  /**
   * @brief Callback pointer.
   * @note  This callback will be invoked when error condition occur.
   *        If set to @p NULL then the callback is disabled.
   */
  i2cerrorcallback_t    id_err_callback;

  /**
   * @brief Pointer to input buffer.
   */
  i2cblock_t            *rxbuf;     // pointer to buffer
  size_t                rxdepth;    // depth of buffer
  size_t                rxbytes;    // count of bytes to sent in one transmission
  size_t                rxbufhead;  // head pointer to current data byte

  i2cblock_t            *txbuf;
  size_t                txdepth;
  size_t                txbytes;
  size_t                txbufhead;

  /**
   * @brief   Contain slave address and some flags.
   * @details Bits 0..9 contain slave address in 10-bit mode.
   *
   *          Bits 0..6 contain slave address in 7-bit mode.
   *
   *          Bits 10..14 are not used in 10-bit mode.
   *          Bits  7..14 are not used in 7-bit mode.
   *
   *          Bit 15 is used to switch between 10-bit and 7-bit modes
   *          (0 denotes 7-bit mode).
   */
  uint16_t              address;

  bool_t                restart;    // send restart if TRUE. Else sent stop event after complete data tx/rx


#if I2C_USE_WAIT
  /**
   * @brief Thread waiting for I/O completion.
   */
  Thread                *thread;
#endif /* I2C_USE_WAIT */
};


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
  void i2cMasterStart(I2CDriver *i2cp);
  void i2cMasterStop(I2CDriver *i2cp);

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

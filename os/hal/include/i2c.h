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
#if I2C_USE_MUTUAL_EXCLUSION && !CH_USE_MUTEXES && !CH_USE_SEMAPHORES
#error "I2C_USE_MUTUAL_EXCLUSION requires CH_USE_MUTEXES and/or CH_USE_SEMAPHORES"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  /* master part */
  I2C_UNINIT = 0,             /**< @brief Not initialized.                    */
  I2C_STOP = 1,               /**< @brief Stopped.                            */
  I2C_READY = 2,              /**< @brief Ready.                              */
  I2C_ACTIVE_TRANSMIT = 3,    /**< @brief Transmit in progress.               */
  I2C_ACTIVE_RECEIVE = 4,     /**< @brief Receive in progress.                */
  I2C_ACTIVE_TRANSCEIVE = 5,  /**< @brief Receive after transmit in progress. */

  /* Slave part. Not realized. */
  I2C_SACTIVE = 10,
  I2C_STRANSMIT = 11,
  I2C_SRECEIVE = 12,
} i2cstate_t;


#include "i2c_lld.h"

/**
 * @brief   I2C notification callback type.
 * @details This callback invoked when byte transfer finish event occurs,
 *          No matter sending or reading.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object triggering the
 *                      callback
 * @param[in] i2cscfg   pointer to the @p I2CSlaveConfig object triggering the
 *                      callback
 */
typedef void (*i2ccallback_t)(I2CDriver *i2cp, const I2CSlaveConfig *i2cscfg);


/**
 * @brief   I2C error notification callback type.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object triggering the
 *                      callback
 * @param[in] i2cscfg   pointer to the @p I2CSlaveConfig object triggering the
 *                      callback
 */
typedef void (*i2cerrorcallback_t)(I2CDriver *i2cp,
                                   const I2CSlaveConfig *i2cscfg);


/**
 * @brief I2C transmission data block size.
 */
typedef uint8_t i2cblock_t;


/**
 * @brief   Structure representing an I2C slave configuration.
 * @details Each slave device has its own config structure with input and
 *          output buffers for temporally storing data.
 */
struct I2CSlaveConfig{
  /**
   * @brief Callback pointer.
   * @note  Transfer finished callback. Invoke when all data transferred.
   *        If set to @p NULL then the callback is disabled.
   */
  i2ccallback_t         id_callback;

  /**
   * @brief Callback pointer.
   * @note  This callback will be invoked when error condition occur.
   *        If set to @p NULL then the callback is disabled.
   */
  i2cerrorcallback_t    id_err_callback;
#if defined(I2C_SLAVECONFIG_EXT_FIELDS)
  I2C_SLAVECONFIG_EXT_FIELDS
#endif
};


/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#if I2C_USE_WAIT || defined(__DOXYGEN__)
/**
 * @brief   Waits for operation completion.
 * @details This function waits for the driver to complete the current
 *          operation.
 * @pre     An operation must be running while the function is invoked.
 * @note    No more than one thread can wait on a I2C driver using
 *          this function.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
#define _i2c_wait_s(i2cp) {                                                 \
  chDbgAssert((i2cp)->id_thread == NULL,                                    \
              "_i2c_wait(), #1", "already waiting");                        \
  chSysLock();                                                              \
  (i2cp)->id_thread = chThdSelf();                                          \
  chSchGoSleepS(THD_STATE_SUSPENDED);                                       \
  chSysUnlock();                                                            \
}

/**
 * @brief   Wakes up the waiting thread.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
#define _i2c_wakeup_isr(i2cp) {                                             \
  if ((i2cp)->id_thread != NULL) {                                          \
    Thread *tp = (i2cp)->id_thread;                                         \
    (i2cp)->id_thread = NULL;                                               \
    chSysLockFromIsr();                                                     \
    chSchReadyI(tp);                                                        \
    chSysUnlockFromIsr();                                                   \
  }                                                                         \
}
#else /* !I2C_USE_WAIT */
#define _i2c_wait_s(i2cp)
#define _i2c_wakeup_isr(i2cp)
#endif /* !I2C_USE_WAIT */

/**
 * @brief   Common ISR code.
 * @details This code handles the portable part of the ISR code:
 *          - Callback invocation.
 *          - Waiting thread wakeup, if any.
 *          - Driver state transitions.
 *
 * @note    This macro is meant to be used in the low level drivers
 *          implementation only.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
#define _i2c_isr_code(i2cp, i2cscfg) {                                 \
  if(((i2cp)->id_slave_config)->id_callback) {                         \
    ((i2cp)->id_slave_config)->id_callback(i2cp, i2cscfg);             \
    (i2cp)->id_state = I2C_READY;                                      \
  }                                                                    \
  else                                                                 \
    (i2cp)->id_state = I2C_READY;                                      \
  _i2c_wakeup_isr(i2cp);                                               \
}

/**
 * @brief   Error ISR code.
 * @details This code handles the portable part of the ISR code:
 *          - Error callback invocation.
 *          - Waiting thread wakeup, if any.
 *          - Driver state transitions.
 *
 * @note    This macro is meant to be used in the low level drivers
 *          implementation only.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
#define _i2c_isr_err_code(i2cp, i2cscfg) {                             \
  if(((i2cp)->id_slave_config)->id_err_callback) {                     \
    ((i2cp)->id_slave_config)->id_err_callback(i2cp, i2cscfg);         \
    (i2cp)->id_state = I2C_READY;                                      \
  }                                                                    \
  else                                                                 \
    (i2cp)->id_state = I2C_READY;                                      \
  _i2c_wakeup_isr(i2cp);                                               \
}

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif
  void i2cInit(void);
  void i2cObjectInit(I2CDriver *i2cp);
  void i2cStart(I2CDriver *i2cp, const I2CConfig *config);
  void i2cStop(I2CDriver *i2cp);
  void i2cMasterTransmit(I2CDriver *i2cp, const I2CSlaveConfig *i2cscfg,
      uint16_t slave_addr,
      uint8_t *txbuf, size_t txbytes,
      uint8_t *rxbuf, size_t rxbytes);
  void i2cMasterReceive(I2CDriver *i2cp, const I2CSlaveConfig *i2cscfg,
      uint16_t slave_addr, uint8_t *rxbuf, size_t rxbytes);
  void i2cMasterStart(I2CDriver *i2cp);
  void i2cMasterStop(I2CDriver *i2cp);
  void i2cAddFlagsI(I2CDriver *i2cp, i2cflags_t mask);

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

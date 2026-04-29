/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file        hal_i2c.h
 * @brief       Generated I2C Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_I2C
 * @brief       I2C Driver macros and structures.
 * @{
 */

#ifndef HAL_I2C_H
#define HAL_I2C_H

#include "hal_cb_driver.h"

#if (HAL_USE_I2C == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    I2C bus error flags
 * @{
 */
/**
 * @brief       No error.
 */
#define I2C_NO_ERROR                        0x00U

/**
 * @brief       Bus error.
 */
#define I2C_BUS_ERROR                       0x01U

/**
 * @brief       Arbitration lost.
 */
#define I2C_ARBITRATION_LOST                0x02U

/**
 * @brief       Acknowledge failure.
 */
#define I2C_ACK_FAILURE                     0x04U

/**
 * @brief       Overrun or underrun.
 */
#define I2C_OVERRUN                         0x08U

/**
 * @brief       PEC error in reception.
 */
#define I2C_PEC_ERROR                       0x10U

/**
 * @brief       Hardware timeout.
 */
#define I2C_TIMEOUT                         0x20U

/**
 * @brief       SMBus alert.
 */
#define I2C_SMB_ALERT                       0x40U
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief       Support for synchronous I2C master APIs.
 */
#if !defined(I2C_USE_SYNCHRONIZATION) || defined(__DOXYGEN__)
#define I2C_USE_SYNCHRONIZATION             TRUE
#endif

/**
 * @brief       Support for I2C user configurations.
 * @note        When enabled the user must provide a variable named @p
 *              i2c_configurations of type @p i2c_configurations_t.
 */
#if !defined(I2C_USE_CONFIGURATIONS) || defined(__DOXYGEN__)
#define I2C_USE_CONFIGURATIONS              FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on I2C_USE_SYNCHRONIZATION configuration.*/
#if (I2C_USE_SYNCHRONIZATION != FALSE) && (I2C_USE_SYNCHRONIZATION != TRUE)
#error "invalid I2C_USE_SYNCHRONIZATION value"
#endif

/* Checks on I2C_USE_CONFIGURATIONS configuration.*/
#if (I2C_USE_CONFIGURATIONS != FALSE) && (I2C_USE_CONFIGURATIONS != TRUE)
#error "invalid I2C_USE_CONFIGURATIONS value"
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro functions
 * @{
 */
/**
 * @brief       Legacy name for @p i2cMasterTransmitTimeout().
 *
 * @param[in]     i2cp          Pointer to the I2C driver instance.
 * @param[in]     addr          Slave address.
 * @param[in]     txbuf         Transmit buffer.
 * @param[in]     txbytes       Number of bytes to transmit.
 * @param[out]    rxbuf         Receive buffer.
 * @param[in]     rxbytes       Number of bytes to receive.
 *
 * @api
 */
#define i2cMasterTransmit(i2cp, addr, txbuf, txbytes, rxbuf, rxbytes)       \
  i2cMasterTransmitTimeout(i2cp, addr, txbuf, txbytes, rxbuf, rxbytes, TIME_INFINITE)

/**
 * @brief       Legacy name for @p i2cMasterReceiveTimeout().
 *
 * @param[in]     i2cp          Pointer to the I2C driver instance.
 * @param[in]     addr          Slave address.
 * @param[out]    rxbuf         Receive buffer.
 * @param[in]     rxbytes       Number of bytes to receive.
 *
 * @api
 */
#define i2cMasterReceive(i2cp, addr, rxbuf, rxbytes)                        \
  i2cMasterReceiveTimeout(i2cp, addr, rxbuf, rxbytes, TIME_INFINITE)
/** @} */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of an I2C address.
 */
typedef uint16_t i2caddr_t;

/**
 * @brief       Type of I2C error flags.
 */
typedef uint32_t i2cflags_t;

/**
 * @brief       Type of I2C driver state.
 */
typedef driver_state_t i2cstate_t;

/**
 * @brief       Type of structure representing an I2C driver.
 */
typedef struct hal_i2c_driver hal_i2c_driver_c;

/**
 * @brief       Type of structure representing an I2C configuration.
 */
typedef struct hal_i2c_config hal_i2c_config_t;

/**
 * @brief       Type of structure representing an I2C driver (legacy).
 */
typedef struct hal_i2c_driver I2CDriver;

/**
 * @brief       Type of structure representing an I2C configuration (legacy).
 */
typedef struct hal_i2c_config I2CConfig;

#define I2C_ACTIVE_RX                      (HAL_DRV_STATE_ERROR + 1U)
#define I2C_LOCKED                         (HAL_DRV_STATE_ERROR + 2U)

/* Inclusion of LLD header.*/
#include "hal_i2c_lld.h"

/**
 * @brief       Driver configuration structure.
 */
struct hal_i2c_config {
  /* End of the mandatory fields.*/
  i2c_lld_config_fields;
#if (defined(I2C_CONFIG_EXT_FIELDS)) || defined (__DOXYGEN__)
  I2C_CONFIG_EXT_FIELDS
#endif /* defined(I2C_CONFIG_EXT_FIELDS) */
};

/**
 * @brief       Type of user-provided I2C configurations.
 */
typedef struct i2c_configurations i2c_configurations_t;

/**
 * @brief       Structure representing user-provided I2C configurations.
 */
struct i2c_configurations {
  /**
   * @brief       Number of configurations in the open array.
   */
  unsigned                  cfgsnum;
  /**
   * @brief       User I2C configurations.
   */
  hal_i2c_config_t          cfgs[];
};

/**
 * @class       hal_i2c_driver_c
 * @extends     hal_cb_driver_c
 *
 * @brief       Class of an I2C driver.
 *
 * @name        Class @p hal_i2c_driver_c structures
 * @{
 */

/**
 * @brief       Type of a I2C driver class.
 */
typedef struct hal_i2c_driver hal_i2c_driver_c;

/**
 * @brief       Class @p hal_i2c_driver_c virtual methods table.
 */
struct hal_i2c_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip, const void *config);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_cb_driver_c.*/
  void (*oncbset)(void *ip, drv_cb_t cb);
  /* From hal_i2c_driver_c.*/
};

/**
 * @brief       Structure representing a I2C driver class.
 */
struct hal_i2c_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_i2c_driver_vmt *vmt;
  /**
   * @brief       Driver state.
   */
  driver_state_t            state;
  /**
   * @brief       Associated configuration structure.
   */
  const void                *config;
  /**
   * @brief       Driver argument.
   */
  void                      *arg;
#if (HAL_USE_MUTUAL_EXCLUSION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Driver mutex.
   */
  mutex_t                   mutex;
#endif /* HAL_USE_MUTUAL_EXCLUSION == TRUE */
#if (HAL_USE_REGISTRY == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Driver identifier.
   */
  unsigned int              id;
  /**
   * @brief       Driver name.
   */
  const char                *name;
  /**
   * @brief       Registry link structure.
   */
  hal_regent_t              regent;
#endif /* HAL_USE_REGISTRY == TRUE */
  /**
   * @brief       Driver callback.
   * @note        Can be @p NULL.
   */
  drv_cb_t                  cb;
  /**
   * @brief       Cached I2C error flags.
   */
  volatile i2cflags_t       errors;
#if (I2C_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Synchronization point for transfer completion.
   */
  thread_reference_t        sync_transfer;
#endif /* I2C_USE_SYNCHRONIZATION == TRUE */
#if (defined(I2C_DRIVER_EXT_FIELDS)) || defined (__DOXYGEN__)
  I2C_DRIVER_EXT_FIELDS
#endif /* defined(I2C_DRIVER_EXT_FIELDS) */
  /* End of the mandatory fields.*/
  i2c_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_i2c_driver_c.*/
  void *__i2c_objinit_impl(void *ip, const void *vmt);
  void __i2c_dispose_impl(void *ip);
  msg_t __i2c_start_impl(void *ip, const void *config);
  void __i2c_stop_impl(void *ip);
  const void *__i2c_setcfg_impl(void *ip, const void *config);
  const void *__i2c_selcfg_impl(void *ip, unsigned cfgnum);
  void __i2c_oncbset_impl(void *ip, drv_cb_t cb);
  msg_t i2cStartMasterTransmitI(void *ip, i2caddr_t addr, const uint8_t *txbuf,
                                size_t txbytes, uint8_t *rxbuf, size_t rxbytes);
  msg_t i2cStartMasterReceiveI(void *ip, i2caddr_t addr, uint8_t *rxbuf,
                               size_t rxbytes);
  msg_t i2cStopTransferI(void *ip);
  msg_t i2cStopTransfer(void *ip);
#if (I2C_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  msg_t i2cSynchronizeS(void *ip, sysinterval_t timeout);
  msg_t i2cSynchronize(void *ip, sysinterval_t timeout);
  msg_t i2cMasterTransmitTimeout(void *ip, i2caddr_t addr,
                                 const uint8_t *txbuf, size_t txbytes,
                                 uint8_t *rxbuf, size_t rxbytes,
                                 sysinterval_t timeout);
  msg_t i2cMasterReceiveTimeout(void *ip, i2caddr_t addr, uint8_t *rxbuf,
                                size_t rxbytes, sysinterval_t timeout);
#endif /* I2C_USE_SYNCHRONIZATION == TRUE */
  /* Regular functions.*/
  void i2cInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_i2c_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_i2c_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_i2c_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_i2c_driver_c *i2cObjectInit(hal_i2c_driver_c *self) {
  extern const struct hal_i2c_driver_vmt __hal_i2c_driver_vmt;

  return __i2c_objinit_impl(self, &__hal_i2c_driver_vmt);
}
/** @} */

/**
 * @name        Inline methods of hal_i2c_driver_c
 * @{
 */
/**
 * @brief       Returns cached I2C errors.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @return                      The cached error flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline i2cflags_t i2cGetErrorsX(void *ip) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  return self->errors;
}

/**
 * @brief       Gets and clears cached I2C errors.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @return                      The pending error flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline i2cflags_t i2cGetAndClearErrorsX(void *ip) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  i2cflags_t errors;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  errors = self->errors;
  self->errors = I2C_NO_ERROR;
  osalSysRestoreStatusX(sts);

  return errors;
}

#if (I2C_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Wakes up the waiting thread.
 * @note        This function is meant to be used in low level drivers.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @param[in]     msg           The wakeup message.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline void __i2c_wakeup_isr(void *ip, msg_t msg) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  osalSysLockFromISR();
  osalThreadResumeI(&self->sync_transfer, msg);
  osalSysUnlockFromISR();
  __cbdrv_invoke_cb(self);
}

#else

CC_FORCE_INLINE
static inline void __i2c_wakeup_isr(void *ip, msg_t msg) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  (void)msg;

  __cbdrv_invoke_cb(self);
}
#endif /* I2C_USE_SYNCHRONIZATION == TRUE */

/**
 * @brief       Signals I2C transfer completion.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline void __i2c_complete_isr(void *ip) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  self->state = HAL_DRV_STATE_READY;
  __i2c_wakeup_isr(self, MSG_OK);
}

/**
 * @brief       Signals I2C transfer error.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline void __i2c_error_isr(void *ip) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  self->state = HAL_DRV_STATE_READY;
  __i2c_wakeup_isr(self, MSG_RESET);
}
/** @} */

#endif /* HAL_USE_I2C == TRUE */

#endif /* HAL_I2C_H */

/** @} */

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
 * @file    I2Cv1/hal_i2c_lld.h
 * @brief   RP I2C subsystem low level driver header.
 *
 * @addtogroup I2C
 * @{
 */

#ifndef HAL_I2C_LLD_H
#define HAL_I2C_LLD_H

#if (HAL_USE_I2C == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    RP configuration options
 * @{
 */

/**
 * @brief   I2C timeout on busy condition in milliseconds.
 */
#if !defined(RP_I2C_BUSY_TIMEOUT) || defined(__DOXYGEN__)
#define RP_I2C_BUSY_TIMEOUT              50
#endif

/**
 * @brief   I2C 10-bit address mode switch.
 * @details If set to @p TRUE 10-bit address mode is enabled.
 * @note    The default is @p FALSE.
 */
#if !defined(RP_I2C_ADDRESS_MODE_10BIT) || defined(__DOXYGEN__)
#define RP_I2C_ADDRESS_MODE_10BIT        FALSE
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness. */
#if !defined(RP_HAS_I2C0)
#error "RP_HAS_I2C0 not defined in registry"
#endif

#if !defined(RP_HAS_I2C1)
#error "RP_HAS_I2C1 not defined in registry"
#endif

/* Mcuconf.h checks. */
#if !defined(RP_I2C_USE_I2C0)
#error "RP_I2C_USE_I2C0 not defined in mcuconf.h"
#endif

#if !defined(RP_I2C_USE_I2C1)
#error "RP_I2C_USE_I2C1 not defined in mcuconf.h"
#endif

#if !defined(RP_IRQ_I2C0_PRIORITY)
#error "RP_IRQ_I2C0_PRIORITY not defined in mcuconf.h"
#endif

#if !defined(RP_IRQ_I2C1_PRIORITY)
#error "RP_IRQ_I2C1_PRIORITY not defined in mcuconf.h"
#endif

/* Device selection checks. */
#if RP_I2C_USE_I2C0 && !RP_HAS_I2C0
#error "I2C0 not present in the selected device"
#endif

#if RP_I2C_USE_I2C1 && !RP_HAS_I2C1
#error "I2C1 not present in the selected device"
#endif

#if !RP_I2C_USE_I2C0 && !RP_I2C_USE_I2C1
#error "I2C driver activated but no I2C peripheral assigned"
#endif

/* IRQ priority checks. */
#if RP_I2C_USE_I2C0 &&                                                     \
    !OSAL_IRQ_IS_VALID_PRIORITY(RP_IRQ_I2C0_PRIORITY)
#error "Invalid IRQ priority assigned to I2C0"
#endif

#if RP_I2C_USE_I2C1 &&                                                     \
    !OSAL_IRQ_IS_VALID_PRIORITY(RP_IRQ_I2C1_PRIORITY)
#error "Invalid IRQ priority assigned to I2C1"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type representing an I2C address.
 */
typedef uint16_t i2caddr_t;

/**
 * @brief   Type of I2C Driver condition flags.
 */
typedef uint32_t i2cflags_t;

/**
 * @brief   Type of I2C driver configuration structure.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
typedef struct {
  /* Baudrate setting. */
  uint32_t                  baudrate;
} I2CConfig;

/**
 * @brief   Type of a structure representing an I2C driver.
 */
typedef struct I2CDriver I2CDriver;

/**
 * @brief   Structure representing an I2C driver.
 */
struct I2CDriver {
  /**
   * @brief   Driver state.
   */
  i2cstate_t                state;
  /**
   * @brief   Current configuration data.
   */
  const I2CConfig           *config;
  /**
   * @brief   Error flags.
   */
  i2cflags_t                errors;
#if (I2C_USE_MUTUAL_EXCLUSION == TRUE) || defined(__DOXYGEN__)
  mutex_t                   mutex;
#endif
#if defined(I2C_DRIVER_EXT_FIELDS)
  I2C_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief     Pointer to the I2Cx registers block.
   */
  I2C_TypeDef               *i2c;
  /**
   * @brief   Thread waiting for I/O completion.
   */
  thread_reference_t        thread;
  /**
   * @brief     Number of bytes in TX phase.
   */
  size_t                    txbytes;
  /**
   * @brief     Number of bytes in RX phase.
   */
  size_t                    rxbytes;
  /**
   * @brief     Send restart on next transmission.
   */
  bool                      send_restart;
  /**
   * @brief     Buffer for TX.
   */
  const uint8_t             *txptr;
  /**
   * @brief     Buffer for RX.
   */
  uint8_t                   *rxptr;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Get errors from I2C driver.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
#define i2c_lld_get_errors(i2cp) ((i2cp)->errors)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (RP_I2C_USE_I2C0 == TRUE) && !defined(__DOXYGEN__)
extern I2CDriver I2CD0;
#endif

#if (RP_I2C_USE_I2C1 == TRUE) && !defined(__DOXYGEN__)
extern I2CDriver I2CD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void i2c_lld_init(void);
  void i2c_lld_start(I2CDriver *i2cp);
  void i2c_lld_stop(I2CDriver *i2cp);
  msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                        const uint8_t *txbuf, size_t txbytes,
                                        uint8_t *rxbuf, size_t rxbytes,
                                        sysinterval_t timeout);
  msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                       uint8_t *rxbuf, size_t rxbytes,
                                       sysinterval_t timeout);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_I2C == TRUE */

#endif /* HAL_I2C_LLD_H */

/** @} */

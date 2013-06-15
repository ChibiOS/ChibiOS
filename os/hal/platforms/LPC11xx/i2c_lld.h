/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC11xx I2C driver - Copyright (C) 2013 Marcin Jokel

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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    LPC11xx/i2c_lld.h
 * @brief   LPC11xx I2C subsystem low level driver header.
 *
 * @addtogroup I2C
 * @{
 */

#ifndef _I2C_LLD_H_
#define _I2C_LLD_H_

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define I2C_CONSET_AA               0x04  /* Assert acknowledge flag. */
#define I2C_CONSET_SI               0x08  /* I2C interrupt flag. */
#define I2C_CONSET_STO              0x10  /* STOP flag. */
#define I2C_CONSET_STA              0x20  /* START flag. */
#define I2C_CONSET_EN               0x40  /* I2C interface enable. */

#define I2C_CONCLR_AAC              0x04  /* Assert acknowledge Clear bit. */
#define I2C_CONCLR_SIC              0x08  /* I2C interrupt Clear bit. */
#define I2C_CONCLR_STAC             0x20  /* START flag Clear bit. */
#define I2C_CONCLR_ENC              0x40  /* I2C interface Disable bit. */

#define I2C_WR_BIT                  0x00
#define I2C_RD_BIT                  0x01

#define I2C_STATE_MS_START          0x08
#define I2C_STATE_MS_RSTART         0x10
#define I2C_STATE_MS_SLAW_ACK       0x18
#define I2C_STATE_MS_SLAW_NACK      0x20
#define I2C_STATE_MS_TDAT_ACK       0x28
#define I2C_STATE_MS_TDAT_NACK      0x30
#define I2C_STATE_ARB_LOST          0x38

#define I2C_STATE_MS_SLAR_ACK       0x40
#define I2C_STATE_MS_SLAR_NACK      0x48
#define I2C_STATE_MS_RDAT_ACK       0x50
#define I2C_STATE_MS_RDAT_NACK      0x58

#define I2C_STATE_BUS_ERROR         0x00

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/
/**
 * @name    Configuration options
 * @{
 */

/**
 * @brief   I2C1 interrupt priority level setting.
 */
#if !defined(LPC11xx_I2C_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC11xx_I2C_IRQ_PRIORITY     3
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type representing I2C address.
 */
typedef uint16_t i2caddr_t;

/**
 * @brief   I2C Driver condition flags type.
 */
typedef uint32_t i2cflags_t;
/**
 * @brief   Supported modes for the I2C bus.
 */
typedef enum {
  I2C_STANDARD_MODE = 1,
  I2C_FAST_MODE = 2,
  I2C_FAST_MODE_PLUS = 3,
} i2cmode_t;

/**
 * @brief Driver configuration structure.
 */
typedef struct {
  i2cmode_t       mode;             /**< @brief Specifies the I2C mode.        */
  uint32_t        clock_timing;     /**< @brief Specifies the clock timing     */
} I2CConfig;

/**
 * @brief   Type of a structure representing an I2C driver.
 */
typedef struct I2CDriver I2CDriver;

/**
 * @brief Structure representing an I2C driver.
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
#if I2C_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
#if CH_USE_MUTEXES || defined(__DOXYGEN__)
  /**
   * @brief   Mutex protecting the bus.
   */
  Mutex                     mutex;
#elif CH_USE_SEMAPHORES
  Semaphore                 semaphore;
#endif
#endif /* I2C_USE_MUTUAL_EXCLUSION */
#if defined(I2C_DRIVER_EXT_FIELDS)
  I2C_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief   Thread waiting for I/O completion.
   */
  Thread                    *thread;
  /**
   * @brief     Current slave address without R/W bit.
   */
  i2caddr_t                 addr;
  /**
   * @brief     Pointer to the transmit buffer.
   */
  const uint8_t             *txbuf;
  /**
    * @brief    Number of bytes to transmit.
    */
  size_t                    txbytes;
  /**
    * @brief     Pointer to the receive buffer.
    */
  uint8_t                   *rxbuf;
  /**
      * @brief    Number of bytes to receive.
      */
  size_t                    rxbytes;
  /**
   * @brief     Pointer to the I2C registers block.
   */
  LPC_I2C_TypeDef           *i2c;
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

#if !defined(__DOXYGEN__)
extern I2CDriver I2CD1;
#endif

#endif /* !defined(__DOXYGEN__) */

#ifdef __cplusplus
extern "C" {
#endif
  void i2c_lld_init(void);
  void i2c_lld_start(I2CDriver *i2cp);
  void i2c_lld_stop(I2CDriver *i2cp);
  msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                        const uint8_t *txbuf, size_t txbytes,
                                        uint8_t *rxbuf, size_t rxbytes,
                                        systime_t timeout);
  msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                       uint8_t *rxbuf, size_t rxbytes,
                                       systime_t timeout);
#ifdef __cplusplus
}
#endif
#endif /* _I2C_LLD_H_ */

/** @} */

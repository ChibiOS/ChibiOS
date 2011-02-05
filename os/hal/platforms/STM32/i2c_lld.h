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
 * @file STM32/i2c_lld.h
 * @brief STM32 I2C subsystem low level driver header.
 * @addtogroup STM32_I2C
 * @{
 */

#ifndef _I2C_LLD_H_
#define _I2C_LLD_H_

#include "ch.h"
#include "hal.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/
#define  I2C_OAR1_ADD0_9                     ((uint16_t)0x03FF)            /*!<Interface Address */
#define  I2C_OAR1_BIT14                      ((uint16_t)0x4000)            /*!<bit 14 should always be kept at 1. */


/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief I2C1 driver enable switch.
 * @details If set to @p TRUE the support for I2C1 is included.
 * @note The default is @p TRUE.
 */
#if !defined(STM32_I2C_USE_I2C1) || defined(__DOXYGEN__)
#define STM32_I2C_USE_I2C1              TRUE
#endif

/**
 * @brief I2C2 driver enable switch.
 * @details If set to @p TRUE the support for I2C2 is included.
 * @note The default is @p TRUE.
 */
#if !defined(STM32_I2C_USE_I2C2) || defined(__DOXYGEN__)
#define STM32_I2C_USE_I2C2              TRUE
#endif

/**
 * @brief I2C1 interrupt priority level setting.
 * @note @p BASEPRI_KERNEL >= @p STM32_I2C1_IRQ_PRIORITY > @p PRIORITY_PENDSV.
 */
#if !defined(STM32_I2C1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2C1_IRQ_PRIORITY     0xA0
#endif

/**
 * @brief I2C2 interrupt priority level setting.
 * @note @p BASEPRI_KERNEL >= @p STM32_I2C2_IRQ_PRIORITY > @p PRIORITY_PENDSV.
 */
#if !defined(STM32_I2C2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2C2_IRQ_PRIORITY     0xA0
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/** @brief  EV5 */
#define I2C_EV5_MASTER_MODE_SELECT          ((uint32_t)(((I2C_SR2_MSL|I2C_SR2_BUSY)<< 16)|I2C_SR1_SB))  /* BUSY, MSL and SB flag */
/** @brief  EV6 */
#define I2C_EV6_MASTER_TRA_MODE_SELECTED    ((uint32_t)(((I2C_SR2_MSL|I2C_SR2_BUSY|I2C_SR2_TRA)<< 16)|I2C_SR1_ADDR|I2C_SR1_TXE))  /* BUSY, MSL, ADDR, TXE and TRA flags */
#define I2C_EV6_MASTER_REC_MODE_SELECTED    ((uint32_t)(((I2C_SR2_MSL|I2C_SR2_BUSY)<< 16)|I2C_SR1_ADDR))  /* BUSY, MSL and ADDR flags */
/** @brief  EV7 */
#define I2C_EV7_MASTER_REC_BYTE_RECEIVED    ((uint32_t)(((I2C_SR2_MSL|I2C_SR2_BUSY)<< 16)|I2C_SR1_RXNE))  /* BUSY, MSL and RXNE flags */
#define I2C_EV7_MASTER_REC_BYTE_QUEUED      ((uint32_t)(((I2C_SR2_MSL|I2C_SR2_BUSY)<< 16)|I2C_SR1_BTF|I2C_SR1_RXNE)) /* BUSY, MSL, RXNE and BTF flags*/
/** @brief  EV8 */
#define I2C_EV8_MASTER_BYTE_TRANSMITTING    ((uint32_t)(((I2C_SR2_MSL|I2C_SR2_BUSY|I2C_SR2_TRA)<< 16)|I2C_SR1_TXE))   /* TRA, BUSY, MSL, TXE flags */
/** @brief  EV8_2 */
#define I2C_EV8_2_MASTER_BYTE_TRANSMITTED   ((uint32_t)(((I2C_SR2_MSL|I2C_SR2_BUSY|I2C_SR2_TRA)<< 16)|I2C_SR1_BTF|I2C_SR1_TXE))  /* TRA, BUSY, MSL, TXE and BTF flags */
/** @brief  EV9 */
#define I2C_EV9_MASTER_ADDR_10BIT           ((uint32_t)(((I2C_SR2_MSL|I2C_SR2_BUSY)<< 16)|I2C_SR1_ADD10))  /* BUSY, MSL and ADD10 flags */
#define I2C_EV_MASK                         0x00FFFFFF

#define I2C_FLG_1BTR            0x01 // Single byte to be received and processed
#define I2C_FLG_2BTR            0x02 // Two bytes to be received and processed
#define I2C_FLG_3BTR            0x04 // Last three received bytes to be processed
#define I2C_FLG_MASTER_RECEIVER 0x10
#define I2C_FLG_HEADER_SENT     0x80

#define EV6_SUBEV_MASK  (I2C_FLG_1BTR|I2C_FLG_2BTR|I2C_FLG_MASTER_RECEIVER)
#define EV7_SUBEV_MASK  (I2C_FLG_2BTR|I2C_FLG_3BTR|I2C_FLG_MASTER_RECEIVER)

#define I2C_EV6_1_MASTER_REC_2BTR_MODE_SELECTED (I2C_FLG_2BTR|I2C_FLG_MASTER_RECEIVER)
#define I2C_EV6_3_MASTER_REC_1BTR_MODE_SELECTED (I2C_FLG_1BTR|I2C_FLG_MASTER_RECEIVER)
#define I2C_EV7_2_MASTER_REC_3BYTES_TO_PROCESS  (I2C_FLG_3BTR|I2C_FLG_MASTER_RECEIVER)
#define I2C_EV7_3_MASTER_REC_2BYTES_TO_PROCESS  (I2C_FLG_2BTR|I2C_FLG_MASTER_RECEIVER)

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/
/**
 * @brief   Serial Driver condition flags type.
 */
typedef uint32_t i2cflags_t;

typedef enum {
  opmodeI2C,
  opmodeSMBusDevice,
  opmodeSMBusHost,
} I2C_opMode_t;

typedef enum {
  stdDutyCycle,
  fastDutyCycle_2,
  fastDutyCycle_16_9,
} I2C_DutyCycle_t;

/**
 * @brief   Type of a structure representing an SPI driver.
 */
typedef struct I2CDriver I2CDriver;

/**
 * @brief   I2C notification callback type.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object triggering the
 *                      callback
 */
typedef void (*i2ccallback_t)(I2CDriver *i2cp);

/**
 * @brief Driver configuration structure.
 */
typedef struct {
  I2C_opMode_t opMode;              	/*!< Specifies the I2C mode.*/

  uint32_t ClockSpeed;          		/*!< Specifies the clock frequency. Must be set to a value lower than 400kHz */

  I2C_DutyCycle_t FastModeDutyCycle;   	/*!< Specifies the I2C fast mode duty cycle */

  uint16_t OwnAddress1;         /*!< Specifies the first device own address. Can be a 7-bit or 10-bit address. */

  uint16_t Ack;                 /*!< Enables or disables the acknowledgement. */

  uint8_t nBitAddress;          /*!< Specifies if 7-bit or 10-bit address is acknowledged */

} I2CConfig;

/**
 * @brief Structure representing an I2C driver.
 */
struct I2CDriver {
  /**
   * @brief Driver state.
   */
  i2cstate_t            state;
  /**
   * @brief Operation complete callback or @p NULL.
   */
  i2ccallback_t         endcb;
#if I2C_USE_WAIT
  /**
   * @brief Thread waiting for I/O completion.
   */
  Thread                *thread;
#endif /* I2C_USE_WAIT */
#if I2C_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
#if CH_USE_MUTEXES || defined(__DOXYGEN__)
  /**
   * @brief Mutex protecting the bus.
   */
  Mutex                 mutex;
#elif CH_USE_SEMAPHORES
  Semaphore             semaphore;
#endif
#endif /* I2C_USE_MUTUAL_EXCLUSION */

  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the I2Cx registers block.
   */
  I2C_TypeDef           *i2c_register;
  size_t                remaining_bytes;
  uint8_t               *rxbuf;
  uint8_t               *txbuf;
  uint8_t               slave_addr1;                  // 7-bit address of the slave
  uint8_t               slave_addr2;                  // used in 10-bit address mode
  uint8_t               nbit_address;
  i2cflags_t            errors;
  i2cflags_t            flags;
  /* Status Change @p EventSource.*/
  EventSource           sevent;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define i2c_lld_bus_is_busy(i2cp)                                           \
  (i2cp->i2c_register->SR2 & I2C_SR2_BUSY)


/* Wait until BUSY flag is reset: a STOP has been generated on the bus
 * signaling the end of transmission
 */
#define i2c_lld_wait_bus_free(i2cp) {                                       \
  uint32_t tmo = 0xffff;                                                    \
  while((i2cp->i2c_register->SR2 & I2C_SR2_BUSY) && tmo--)                  \
    ;                                                                       \
}

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/** @cond never*/
#if STM32_I2C_USE_I2C1
extern I2CDriver I2CD1;
#endif

#if STM32_I2C_USE_I2C2
extern I2CDriver I2CD2;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void i2c_lld_init(void);
void i2c_lld_reset(I2CDriver *i2cp);
void i2c_lld_set_clock(I2CDriver *i2cp, int32_t clock_speed, I2C_DutyCycle_t duty);
void i2c_lld_set_opmode(I2CDriver *i2cp, I2C_opMode_t opmode);
void i2c_lld_set_own_address(I2CDriver *i2cp, int16_t address, int8_t nr_bit);
void i2c_lld_start(I2CDriver *i2cp);
void i2c_lld_stop(I2CDriver *i2cp);
void i2c_lld_master_transmit(I2CDriver *i2cp, uint16_t slave_addr, size_t n, void *txbuf);
void i2c_lld_master_receive(I2CDriver *i2cp, uint16_t slave_addr, size_t n, void *rxbuf);

#ifdef __cplusplus
}
#endif
/** @endcond*/

#endif // CH_HAL_USE_I2C

#endif // _I2C_LLD_H_

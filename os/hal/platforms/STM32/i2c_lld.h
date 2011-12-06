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
 * @file STM32/i2c_lld.h
 * @brief STM32 I2C subsystem low level driver header.
 * @addtogroup I2C
 * @{
 */

#ifndef _I2C_LLD_H_
#define _I2C_LLD_H_

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */

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
 * @brief I2C3 driver enable switch.
 * @details If set to @p TRUE the support for I2C3 is included.
 * @note The default is @p TRUE.
 */
#if !defined(STM32_I2C_USE_I2C3) || defined(__DOXYGEN__)
#define STM32_I2C_USE_I2C3              TRUE
#endif

/**
 * @brief I2C1 interrupt priority level setting.
 * @note @p BASEPRI_KERNEL >= @p STM32_I2C_I2C1_IRQ_PRIORITY > @p PRIORITY_PENDSV.
 */
#if !defined(STM32_I2C_I2C1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2C_I2C1_IRQ_PRIORITY     0xA0
#endif

/**
 * @brief I2C2 interrupt priority level setting.
 * @note @p BASEPRI_KERNEL >= @p STM32_I2C_I2C2_IRQ_PRIORITY > @p PRIORITY_PENDSV.
 */
#if !defined(STM32_I2C_I2C2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2C_I2C2_IRQ_PRIORITY     0xA0
#endif

/**
 * @brief I2C2 interrupt priority level setting.
 * @note @p BASEPRI_KERNEL >= @p STM32_I2C_I2C2_IRQ_PRIORITY > @p PRIORITY_PENDSV.
 */
#if !defined(STM32_I2C_I2C3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2C_I2C3_IRQ_PRIORITY     0xA0
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/** @brief  EV5 */
#define I2C_EV5_MASTER_MODE_SELECT          ((uint32_t)(((I2C_SR2_MSL | I2C_SR2_BUSY) << 16) | I2C_SR1_SB))  /* BUSY, MSL and SB flag */
#define I2C_EV8_2_MASTER_BYTE_TRANSMITTED   ((uint32_t)(((I2C_SR2_MSL | I2C_SR2_BUSY | I2C_SR2_TRA) << 16) | I2C_SR1_BTF | I2C_SR1_TXE))  /* TRA, BUSY, MSL, TXE and BTF flags */
#define I2C_EV_MASK                         0x00FFFFFF  /* First byte zeroed because there is no need of PEC register part from SR2 */

#define I2C_FLG_MASTER_RECEIVER 0x10
#define I2C_FLG_HEADER_SENT     0x80


/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   I2C Driver condition flags type.
 */
typedef uint32_t i2cflags_t;

typedef enum {
  OPMODE_I2C = 1,
  OPMODE_SMBUS_DEVICE = 2,
  OPMODE_SMBUS_HOST = 3,
} i2copmode_t;

typedef enum {
  STD_DUTY_CYCLE = 1,
  FAST_DUTY_CYCLE_2 = 2,
  FAST_DUTY_CYCLE_16_9 = 3,
} i2cdutycycle_t;

/**
 * @brief Driver configuration structure.
 */
typedef struct {
  i2copmode_t     op_mode;       /**< @brief Specifies the I2C mode.*/
  uint32_t        clock_speed;   /**< @brief Specifies the clock frequency. Must be set to a value lower than 400kHz */
  i2cdutycycle_t  duty_cycle;    /**< @brief Specifies the I2C fast mode duty cycle */
} I2CConfig;


/**
 * @brief   Type of a structure representing an I2C driver.
 */
typedef struct I2CDriver I2CDriver;

/**
 * @brief   Type of a structure representing an I2C slave config.
 */
typedef struct I2CSlaveConfig I2CSlaveConfig;

/**
 * @brief Structure representing an I2C driver.
 */
struct I2CDriver{
  /**
   * @brief Driver state.
   */
  i2cstate_t            id_state;

#if I2C_USE_WAIT
  /**
   * @brief Thread waiting for I/O completion.
   */
  Thread                *id_thread;
#endif /* I2C_USE_WAIT */
#if I2C_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
#if CH_USE_MUTEXES || defined(__DOXYGEN__)
  /**
   * @brief Mutex protecting the bus.
   */
  Mutex                 id_mutex;
#elif CH_USE_SEMAPHORES
  Semaphore             id_semaphore;
#endif
#endif /* I2C_USE_MUTUAL_EXCLUSION */

  /**
   * @brief Current configuration data.
   */
  const I2CConfig           *id_config;
  /**
   * @brief Current slave configuration data.
   */
  const I2CSlaveConfig      *id_slave_config;

  __IO size_t               txbytes;    /*!< @brief Number of bytes to be transmitted. */
  __IO size_t               rxbytes;    /*!< @brief Number of bytes to be received. */
  uint8_t                   *rxbuf;     /*!< @brief Pointer to receive buffer. */
  uint8_t                   *txbuf;     /*!< @brief Pointer to transmit buffer.*/

  __IO i2cflags_t           errors;     /*!< @brief Error flags.*/
  __IO i2cflags_t           flags;      /*!< @brief State flags.*/

  uint8_t                   slave_addr; /*!< @brief Current slave address without R/W bit. */

#if CH_USE_EVENTS
  EventSource               sevent;     /*!< @brief Status Change @p EventSource.*/
#endif

  /*********** End of the mandatory fields. **********************************/

  uint32_t                  dmamode;    /*!< @brief DMA mode bit mask.*/
  const stm32_dma_stream_t  *dmarx;     /*!< @brief Receive DMA channel.*/
  const stm32_dma_stream_t  *dmatx;     /*!< @brief Transmit DMA channel.*/

  I2C_TypeDef           *id_i2c;        /*!< @brief Pointer to the I2Cx registers block. */
};


/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define i2c_lld_bus_is_busy(i2cp)                                           \
  (i2cp->id_i2c->SR2 & I2C_SR2_BUSY)


/* Wait until BUSY flag is reset: a STOP has been generated on the bus
 * signaling the end of transmission. Normally this wait function
 * does not block thread, only if slave not response it does.
 */
#define i2c_lld_wait_bus_free(i2cp) {                                       \
  uint32_t tmo = 0xfffff;                                                   \
  while((i2cp->id_i2c->SR2 & I2C_SR2_BUSY) && tmo--)                        \
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

#if STM32_I2C_USE_I2C3
extern I2CDriver I2CD3;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void i2c_lld_init(void);
void i2c_lld_reset(I2CDriver *i2cp);
void i2c_lld_set_clock(I2CDriver *i2cp);
void i2c_lld_set_opmode(I2CDriver *i2cp);
void i2c_lld_start(I2CDriver *i2cp);
void i2c_lld_stop(I2CDriver *i2cp);
void i2c_lld_master_transmit(I2CDriver *i2cp, uint8_t slave_addr,
    uint8_t *txbuf, size_t txbytes, uint8_t *rxbuf, size_t rxbytes);
void i2c_lld_master_receive(I2CDriver *i2cp, uint8_t slave_addr,
    uint8_t *rxbuf, size_t rxbytes);

#ifdef __cplusplus
}
#endif
/** @endcond*/

#endif /* CH_HAL_USE_I2C  */

#endif /* _I2C_LLD_H_ */

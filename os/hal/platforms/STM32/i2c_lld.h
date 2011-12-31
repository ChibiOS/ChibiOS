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
 * @note The default is @p FALSE.
 */
#if !defined(STM32_I2C_USE_I2C1) || defined(__DOXYGEN__)
#define STM32_I2C_USE_I2C1              FALSE
#endif

/**
 * @brief I2C2 driver enable switch.
 * @details If set to @p TRUE the support for I2C2 is included.
 * @note The default is @p FALSE.
 */
#if !defined(STM32_I2C_USE_I2C2) || defined(__DOXYGEN__)
#define STM32_I2C_USE_I2C2              FALSE
#endif

/**
 * @brief I2C3 driver enable switch.
 * @details If set to @p TRUE the support for I2C3 is included.
 * @note The default is @p FALSE.
 */
#if !defined(STM32_I2C_USE_I2C3) || defined(__DOXYGEN__)
#define STM32_I2C_USE_I2C3              FALSE
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

/**
 * @brief   I2C1 DMA error hook.
 * @note    The default action for DMA errors is a system halt because DMA
 *          error can only happen because programming errors.
 */
#if !defined(STM32_I2C_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define STM32_I2C_DMA_ERROR_HOOK(uartp)    chSysHalt()
#endif

#if STM32_ADVANCED_DMA || defined(__DOXYGEN__)

/**
 * @brief   DMA stream used for I2C1 RX operations.
 * @note    This option is only available on platforms with enhanced DMA.
 */
#if !defined(STM32_I2C_I2C1_RX_DMA_STREAM) || defined(__DOXYGEN__)
#define STM32_I2C_I2C1_RX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 0)
#endif

/**
 * @brief   DMA stream used for I2C1 TX operations.
 * @note    This option is only available on platforms with enhanced DMA.
 */
#if !defined(STM32_I2C_I2C1_TX_DMA_STREAM) || defined(__DOXYGEN__)
#define STM32_I2C_I2C1_TX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 6)
#endif

/**
 * @brief   DMA stream used for I2C2 RX operations.
 * @note    This option is only available on platforms with enhanced DMA.
 */
#if !defined(STM32_I2C_I2C2_RX_DMA_STREAM) || defined(__DOXYGEN__)
#define STM32_I2C_I2C2_RX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 2)
#endif

/**
 * @brief   DMA stream used for I2C2 TX operations.
 * @note    This option is only available on platforms with enhanced DMA.
 */
#if !defined(STM32_I2C_I2C2_TX_DMA_STREAM) || defined(__DOXYGEN__)
#define STM32_I2C_I2C2_TX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 7)
#endif

/**
 * @brief   DMA stream used for I2C3 RX operations.
 * @note    This option is only available on platforms with enhanced DMA.
 */
#if !defined(STM32_I2C_I2C3_RX_DMA_STREAM) || defined(__DOXYGEN__)
#define STM32_I2C_I2C3_RX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 2)
#endif

/**
 * @brief   DMA stream used for I2C3 TX operations.
 * @note    This option is only available on platforms with enhanced DMA.
 */
#if !defined(STM32_I2C_I2C3_TX_DMA_STREAM) || defined(__DOXYGEN__)
#define STM32_I2C_I2C3_TX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 4)
#endif

#else /* !STM32_ADVANCED_DMA */

/* Fixed streams for platforms using the old DMA peripheral, the values are
   valid for both STM32F1xx and STM32L1xx.*/
#define STM32_I2C_I2C1_RX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 7)
#define STM32_I2C_I2C1_TX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 6)
#define STM32_I2C_I2C2_RX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 5)
#define STM32_I2C_I2C2_TX_DMA_STREAM     STM32_DMA_STREAM_ID(1, 4)

#endif /* !STM32_ADVANCED_DMA*/
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/** @brief  flags for interrupt handling */
#define I2C_EV5_MASTER_MODE_SELECT          ((uint32_t)(((I2C_SR2_MSL | I2C_SR2_BUSY) << 16) | I2C_SR1_SB))  /* BUSY, MSL and SB flag */
#define I2C_EV6_MASTER_TRA_MODE_SELECTED    ((uint32_t)(((I2C_SR2_MSL|I2C_SR2_BUSY|I2C_SR2_TRA)<< 16)|I2C_SR1_ADDR|I2C_SR1_TXE))  /* BUSY, MSL, ADDR, TXE and TRA flags */
#define I2C_EV6_MASTER_REC_MODE_SELECTED    ((uint32_t)(((I2C_SR2_MSL|I2C_SR2_BUSY)<< 16)|I2C_SR1_ADDR))  /* BUSY, MSL and ADDR flags */
#define I2C_EV8_2_MASTER_BYTE_TRANSMITTED   ((uint32_t)(((I2C_SR2_MSL | I2C_SR2_BUSY | I2C_SR2_TRA) << 16) | I2C_SR1_BTF | I2C_SR1_TXE))  /* TRA, BUSY, MSL, TXE and BTF flags */
#define I2C_EV_MASK                         0x00FFFFFF  /* First byte zeroed because there is no need of PEC register part from SR2 */

/** @brief  error checks */
#if STM32_I2C_USE_I2C1 && !STM32_HAS_I2C1
#error "I2C1 not present in the selected device"
#endif

#if STM32_I2C_USE_I2C2 && !STM32_HAS_I2C2
#error "I2C2 not present in the selected device"
#endif

#if STM32_I2C_USE_I2C3 && !STM32_HAS_I2C3
#error "I2C3 not present in the selected device"
#endif

#if !STM32_I2C_USE_I2C1 && !STM32_I2C_USE_I2C2 &&                     		\
    !STM32_I2C_USE_I2C3
#error "I2C driver activated but no I2C peripheral assigned"
#endif

#if STM32_I2C_USE_I2C1 &&                                                \
    !STM32_DMA_IS_VALID_ID(STM32_I2C_I2C1_RX_DMA_STREAM,                 \
                           STM32_I2C1_RX_DMA_MSK)
#error "invalid DMA stream associated to I2C1 RX"
#endif

#if STM32_I2C_USE_I2C1 &&                                                \
    !STM32_DMA_IS_VALID_ID(STM32_I2C_I2C1_TX_DMA_STREAM,                 \
                           STM32_I2C1_TX_DMA_MSK)
#error "invalid DMA stream associated to I2C1 TX"
#endif

#if STM32_I2C_USE_I2C2 &&                                                \
    !STM32_DMA_IS_VALID_ID(STM32_I2C_I2C2_RX_DMA_STREAM,                 \
                           STM32_I2C2_RX_DMA_MSK)
#error "invalid DMA stream associated to I2C2 RX"
#endif

#if STM32_I2C_USE_I2C2 &&                                                \
    !STM32_DMA_IS_VALID_ID(STM32_I2C_I2C2_TX_DMA_STREAM,                 \
                           STM32_I2C2_TX_DMA_MSK)
#error "invalid DMA stream associated to I2C2 TX"
#endif

#if STM32_I2C_USE_I2C3 &&                                                \
    !STM32_DMA_IS_VALID_ID(STM32_I2C_I2C3_RX_DMA_STREAM,                 \
                           STM32_I2C3_RX_DMA_MSK)
#error "invalid DMA stream associated to I2C3 RX"
#endif

#if STM32_I2C_USE_I2C3 &&                                                \
    !STM32_DMA_IS_VALID_ID(STM32_I2C_I2C3_TX_DMA_STREAM,                 \
                           STM32_I2C3_TX_DMA_MSK)
#error "invalid DMA stream associated to I2C3 TX"
#endif

#if !defined(STM32_DMA_REQUIRED)
#define STM32_DMA_REQUIRED
#endif

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
 * @brief Structure representing an I2C driver.
 */
struct I2CDriver{
  /**
   * @brief Driver state.
   */
  i2cstate_t                id_state;

  /**
   * @brief Thread waiting for I/O completion.
   */
  Thread                    *id_thread;

#if I2C_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
#if CH_USE_MUTEXES || defined(__DOXYGEN__)
  /**
   * @brief Mutex protecting the bus.
   */
  Mutex                     id_mutex;
#elif CH_USE_SEMAPHORES
  Semaphore                 id_semaphore;
#endif
#endif /* I2C_USE_MUTUAL_EXCLUSION */

  /**
   * @brief Current configuration data.
   */
  const I2CConfig           *id_config;

  __IO size_t               txbytes;    /*!< @brief Number of bytes to be transmitted. */
  __IO size_t               rxbytes;    /*!< @brief Number of bytes to be received. */
  uint8_t                   *rxbuf;     /*!< @brief Pointer to receive buffer. */

  __IO i2cflags_t           errors;     /*!< @brief Error flags.*/

  i2caddr_t                 slave_addr; /*!< @brief Current slave address without R/W bit. */

  /*********** End of the mandatory fields. **********************************/

  uint32_t                  dmamode;    /*!< @brief DMA mode bit mask.*/
  const stm32_dma_stream_t  *dmarx;     /*!< @brief Receive DMA channel.*/
  const stm32_dma_stream_t  *dmatx;     /*!< @brief Transmit DMA channel.*/

  I2C_TypeDef               *id_i2c;    /*!< @brief Pointer to the I2Cx registers block. */
};


/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * Wait until BUSY flag is reset.
 */
#define i2c_lld_wait_bus_free(i2cp) {                                       \
  while(i2cp->id_i2c->SR2 & I2C_SR2_BUSY)                                   \
    ;                                                                       \
}
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
#define i2c_lld_wait_s(i2cp, timeout, rdymsg) {                             \
  chDbgAssert((i2cp)->id_thread == NULL,                                    \
              "_i2c_wait(), #1", "already waiting");                        \
  chSysLock(); /* this lock will be disarmed in high level part */          \
  (i2cp)->id_thread = chThdSelf();                                          \
  rdymsg = chSchGoSleepTimeoutS(THD_STATE_SUSPENDED, timeout);              \
}

/**
 * @brief   Wakes up the waiting thread.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
#define i2c_lld_wakeup_isr(i2cp) {                                          \
  if ((i2cp)->id_thread != NULL) {                                          \
    Thread *tp = (i2cp)->id_thread;                                         \
    (i2cp)->id_thread = NULL;                                               \
    chSysLockFromIsr();                                                     \
    chSchReadyI(tp);                                                        \
    chSysUnlockFromIsr();                                                   \
  }                                                                         \
}

/**
 * @brief   Wakes up the waiting thread in case of errors.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
#define i2c_lld_error_wakeup_isr(i2cp) {                                    \
  if ((i2cp)->id_thread != NULL) {                                          \
    Thread *tp = (i2cp)->id_thread;                                         \
    (i2cp)->id_thread = NULL;                                               \
    chSysLockFromIsr();                                                     \
    tp->p_u.rdymsg = RDY_RESET;                                             \
    chSchReadyI(tp);                                                        \
    chSysUnlockFromIsr();                                                   \
  }                                                                         \
}

/**
 * @brief   Common ISR code.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
#define i2c_lld_isr_code(i2cp) {                                            \
  i2c_lld_wakeup_isr(i2cp);                                                 \
}

/**
 * @brief   Error ISR code.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
#define i2c_lld_isr_err_code(i2cp) {                                        \
  i2c_lld_error_wakeup_isr(i2cp);                                           \
}

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
msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, uint8_t slave_addr,
                                      const uint8_t *txbuf, size_t txbytes,
                                      uint8_t *rxbuf, size_t rxbytes,
                                      systime_t timeout);
msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp,
                                    uint8_t slave_addr,
                                    uint8_t *rxbuf,
                                    size_t rxbytes,
                                    systime_t timeout);
void i2c_lld_master_transceive(I2CDriver *i2cp);

#ifdef __cplusplus
}
#endif
/** @endcond*/

#endif /* CH_HAL_USE_I2C  */

#endif /* _I2C_LLD_H_ */

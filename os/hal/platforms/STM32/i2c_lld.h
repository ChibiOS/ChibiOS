/**
 * @file STM32/i2c_lld.h
 * @brief STM32 I2C subsystem low level driver header.
 * @addtogroup STM32_I2C
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

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/** @brief No pending conditions.*/
#define I2C_NO_ERROR          0
/*@brief external Stop or Start condition during an address or a data transfer*/
#define I2C_BUS_ERROR         1
/** @brief */
#define I2C_ARBITRATION_LOSS  2
/** @brief */
#define I2C_ACK_FAIL          4
/** @brief */
#define I2C_OVERRUN_UNDERRUN  8
/** @brief */
#define I2C_PEC_ERROR         16
/** @brief */
#define I2C_TIMEOUT           32
/** @brief */
#define I2C_SMBUS_ALERT       64

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   I2C notification callback type.
 *
 * @param[in] i2cp      FIXME: pointer to the @p I2CDriver object triggering the
 *                      callback
 */
typedef void (*i2ccallback_t)(void);

/**
 * @brief   I2C error notification callback type.
 *
 * @param[in] i2cp      FIXME: pointer to the @p I2CDriver object triggering the
 *                      callback
 */
typedef void (*i2cerrorcallback_t)(void);

/**
 * @brief Driver configuration structure.
 */
typedef struct {
  /**
   * @brief I2C initialization data.
   */
  uint16_t              i2cc_cr1;
  uint16_t              i2cc_cr2;
  uint16_t              i2cc_ccr;
  uint16_t              i2cc_trise;

} I2CConfig;


/**
 * @brief TODO:
 */
typedef uint32_t i2cflags_t;

/**
 * @brief TODO:
 */
typedef uint8_t i2cblock_t;


/**
 * @brief Structure representing an I2C slave configuration.
 * @details Each slave has its own data buffers, adress, and error flags.
 */
typedef struct {
  /**
   * @brief Callback pointer.
   * @note  TODO: I don't know, when this callback is inwoked
   *        @p NULL then the callback is disabled.
   */
  i2ccallback_t         id_callback;
  /**
   * @brief Callback pointer.
   * @note  TODO: I don't know, when this callback is inwoked
   *        @p NULL then the callback is disabled.
   */
  i2cerrorcallback_t    id_errcallback;

  i2cblock_t            *rxbuf; // pointer to buffer
  size_t                rxdepth;// depth of buffer
  size_t                rxbytes;// count of bytes to sent in one sending

  i2cblock_t            *txbuf;
  size_t                txdepth;
  size_t                txbytes;

  uint8_t               slave_addr1; // 7-bit address of the slave
  uint8_t               slave_addr2; // used in 10-bit address mode

  uint16_t              error_flags;

  uint8_t               rw_bit; // this flag contain R/W bit

  bool_t                restart; // send restart or stop event after complete data tx/rx

}I2CSlaveConfig;



/**
 * @brief Structure representing an I2C driver.
 */
typedef struct {
  /**
   * @brief Driver state.
   */
  i2cstate_t            id_state;
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
  I2CConfig             *id_config;
  /**
   * @brief Current slave configuration data.
   */
  I2CSlaveConfig        *id_slave_config;

  /* End of the mandatory fields.*/
  /**
   * @brief Thread waiting for I/O completion.
   */
  Thread                *id_thread;
  /**
   * @brief Pointer to the I2Cx registers block.
   */
  I2C_TypeDef           *id_i2c;

} I2CDriver;


/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

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
void i2c_lld_start(I2CDriver *i2cp);
void i2c_lld_stop(I2CDriver *i2cp);
void i2c_lld_master_start(I2CDriver *i2cp, uint16_t header);
void i2c_lld_master_stop(I2CDriver *i2cp);
void i2c_lld_master_transmit(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg, bool_t restart);
void i2c_lld_master_transmitI(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg, bool_t restart);
void i2c_lld_master_receive(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg);
//static i2cflags_t translate_errors(uint16_t sr);

#ifdef __cplusplus
}
#endif
/** @endcond*/

#endif // CH_HAL_USE_I2C

#endif // _I2C_LLD_H_

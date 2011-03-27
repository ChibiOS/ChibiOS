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
/* Driver data structures and types.                                         */
/*===========================================================================*/

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
 * @brief Driver configuration structure.
 */
typedef struct {
  I2C_opMode_t    opMode;           /*!< Specifies the I2C mode.*/
  uint32_t        ClockSpeed;       /*!< Specifies the clock frequency. Must be set to a value lower than 400kHz */
  I2C_DutyCycle_t FastModeDutyCycle;/*!< Specifies the I2C fast mode duty cycle */
  uint8_t         OwnAddress7;      /*!< Specifies the first device 7-bit own address. */
  uint16_t        OwnAddress10;     /*!< Specifies the second part of device own address in 10-bit mode. Set to NULL if not used. */
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
  Thread                *thread;
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
  I2CConfig             *id_config;
  /**
   * @brief Current slave configuration data.
   */
  I2CSlaveConfig        *id_slave_config;
  /**
   * @brief RW-bit sent to slave.
   */
  uint8_t               rw_bit;

  /*********** End of the mandatory fields. **********************************/

  /**
   * @brief Pointer to the I2Cx registers block.
   */
  I2C_TypeDef           *id_i2c;
} ;


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
void i2c_lld_set_clock(I2CDriver *i2cp);
void i2c_lld_set_opmode(I2CDriver *i2cp);
void i2c_lld_set_own_address(I2CDriver *i2cp);

void i2c_lld_master_start(I2CDriver *i2cp);
void i2c_lld_master_stop(I2CDriver *i2cp);

void i2c_lld_master_transmit(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg);
void i2c_lld_master_receive(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg);

void i2c_lld_master_transmit_NI(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg, bool_t restart);
void i2c_lld_master_receive_NI(I2CDriver *i2cp, I2CSlaveConfig *i2cscfg);

#ifdef __cplusplus
}
#endif
/** @endcond*/

#endif // CH_HAL_USE_I2C

#endif // _I2C_LLD_H_

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

#define I2C_FLG_1BTR            0x01 /* Single byte to be received and processed */
#define I2C_FLG_2BTR            0x02 /* Two bytes to be received and processed */
#define I2C_FLG_3BTR            0x04 /* Last three received bytes to be processed */
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
  i2copmode_t     op_mode;       /*!< Specifies the I2C mode.*/
  uint32_t        clock_speed;   /*!< Specifies the clock frequency. Must be set to a value lower than 400kHz */
  i2cdutycycle_t  duty_cycle;    /*!< Specifies the I2C fast mode duty cycle */
  uint8_t         own_addr_7;    /*!< Specifies the first device 7-bit own address. */
  uint16_t        own_addr_10;   /*!< Specifies the second part of device own address in 10-bit mode. Set to NULL if not used. */
  uint16_t        ack;           /*!< Enables or disables the acknowledgement. */
  uint8_t         nbit_own_addr; /*!< Specifies if 7-bit or 10-bit address is acknowledged */
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
  const I2CConfig       *id_config;
  /**
   * @brief Current slave configuration data.
   */
  const I2CSlaveConfig  *id_slave_config;

  size_t                txbytes;    /*!< Number of bytes to be transmitted. */
  size_t                rxbytes;    /*!< Number of bytes to be received. */
  uint8_t               *rxbuf;     /*!< Pointer to receive buffer. */
  uint8_t               *txbuf;     /*!< Pointer to transmit buffer.*/
  uint8_t               *rxbuff_p;  /*!< Pointer to the current byte in slave rx buffer. */
  uint8_t               *txbuff_p;  /*!< Pointer to the current byte in slave tx buffer. */

  i2cflags_t            errors;     /*!< Error flags.*/
  i2cflags_t            flags;      /*!< State flags.*/

  uint16_t              slave_addr; /*!< Current slave address. */
  uint8_t               slave_addr1;/*!< 7-bit address of the slave with r\w bit.*/
  uint8_t               slave_addr2;/*!< Used in 10-bit address mode. */

  EventSource           sevent;     /*!< Status Change @p EventSource.*/

  /*********** End of the mandatory fields. **********************************/

  /**
   * @brief Pointer to the I2Cx registers block.
   */
  I2C_TypeDef           *id_i2c;
} ;


/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define i2c_lld_bus_is_busy(i2cp)                                           \
  (i2cp->id_i2c->SR2 & I2C_SR2_BUSY)


/* Wait until BUSY flag is reset: a STOP has been generated on the bus
 * signaling the end of transmission
 */
#define i2c_lld_wait_bus_free(i2cp) {                                       \
  uint32_t tmo = 0xffff;                                                    \
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

#ifdef __cplusplus
extern "C" {
#endif

void i2c_lld_init(void);
void i2c_lld_reset(I2CDriver *i2cp);
void i2c_lld_set_clock(I2CDriver *i2cp);
void i2c_lld_set_opmode(I2CDriver *i2cp);
void i2c_lld_set_own_address(I2CDriver *i2cp);
void i2c_lld_start(I2CDriver *i2cp);
void i2c_lld_stop(I2CDriver *i2cp);
void i2c_lld_master_transmit(I2CDriver *i2cp, uint16_t slave_addr,
    uint8_t *txbuf, size_t txbytes, uint8_t *rxbuf, size_t rxbytes);
void i2c_lld_master_receive(I2CDriver *i2cp, uint16_t slave_addr,
    uint8_t *rxbuf, size_t rxbytes);

#ifdef __cplusplus
}
#endif
/** @endcond*/

#endif /* CH_HAL_USE_I2C  */

#endif /* _I2C_LLD_H_ */

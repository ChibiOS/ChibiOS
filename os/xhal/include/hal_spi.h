/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file        hal_spi.h
 * @brief       Generated SPI Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_SPI
 * @brief       SPI Driver macros and structures.
 * @{
 */

#ifndef HAL_SPI_H
#define HAL_SPI_H

#if (HAL_USE_SPI == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    SPI operation modes
 * @{
 */
/**
 * @brief       Memory buffers frame size bit position.
 */
#define SPI_MODE_FSIZE_POS                  0U

/**
 * @brief       Memory buffers frame size mask.
 */
#define SPI_MODE_FSIZE_MASK                 (3U << SPI_MODE_FSIZE_POS)

/**
 * @brief       Memory frame size is 8 bits.
 */
#define SPI_MODE_FSIZE_8                    (0U << SPI_MODE_FSIZE_POS)

/**
 * @brief       Memory frame size is 16 bits.
 */
#define SPI_MODE_FSIZE_16                   (1U << SPI_MODE_FSIZE_POS)

/**
 * @brief       Memory frame size is 32 bits.
 */
#define SPI_MODE_FSIZE_32                   (2U << SPI_MODE_FSIZE_POS)

/**
 * @brief       Memory frame size is 64 bits.
 */
#define SPI_MODE_FSIZE_64                   (3U << SPI_MODE_FSIZE_POS)

/**
 * @brief       Enables the circular buffer mode.
 */
#define SPI_MODE_CIRCULAR                   (1U << 2)

/**
 * @brief       Enables the slave mode.
 */
#define SPI_MODE_SLAVE                      (1U << 3)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief       Support for thread synchronization API.
 */
#if !defined(SPI_USE_SYNCHRONIZATION) || defined(__DOXYGEN__)
#define SPI_USE_SYNCHRONIZATION             TRUE
#endif

/**
 * @brief       Assertions on SPI errors return.
 */
#if !defined(SPI_USE_ASSERT_ON_ERROR) || defined(__DOXYGEN__)
#define SPI_USE_ASSERT_ON_ERROR             FALSE
#endif

/**
 * @brief       Support for SPI user configurations.
 * @note        When enabled the user must provide a variable named @p
 *              spi_configurations of type @p spi_configurations_t.
 */
#if !defined(SPI_USE_CONFIGURATIONS) || defined(__DOXYGEN__)
#define SPI_USE_CONFIGURATIONS              FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on SPI_USE_SYNCHRONIZATION configuration.*/
#if (SPI_USE_SYNCHRONIZATION != FALSE) && (SPI_USE_SYNCHRONIZATION != TRUE)
#error "invalid SPI_USE_SYNCHRONIZATION value"
#endif

/* Checks on SPI_USE_ASSERT_ON_ERROR configuration.*/
#if (SPI_USE_ASSERT_ON_ERROR != FALSE) && (SPI_USE_ASSERT_ON_ERROR != TRUE)
#error "invalid SPI_USE_ASSERT_ON_ERROR value"
#endif

/* Checks on SPI_USE_CONFIGURATIONS configuration.*/
#if (SPI_USE_CONFIGURATIONS != FALSE) && (SPI_USE_CONFIGURATIONS != TRUE)
#error "invalid SPI_USE_CONFIGURATIONS value"
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief       Return a pointer to the configuration structure.
 *
 * @param         ip            Pointer to the @p hal_spi_driver_c object.
 * @return                      A pointer to the configuration structure.
 *
 * @notapi
 */
#define __spi_getconf(ip)                                                   \
  ((const hal_spi_config_t *)((ip)->config))

/**
 * @brief       Retrieves a configuration field.
 *
 * @param         ip            Pointer to the @p hal_spi_driver_c object.
 * @param         field         Configuration field to be retrieved.
 * @return                      The field value.
 *
 * @notapi
 */
#define __spi_getfield(ip, field)                                           \
  (__spi_getconf(ip)->field)

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of SPI transfer mode options.
 */
typedef uint_fast16_t spi_mode_t;

/**
 * @brief       Type of structure representing a SPI driver.
 */
typedef struct hal_spi_driver hal_spi_driver_c;

/**
 * @brief       Type of structure representing a SPI configuration.
 */
typedef struct hal_spi_config hal_spi_config_t;

/**
 * @brief       Type of structure representing a SPI configuration (legacy).
 */
typedef struct hal_spi_config SPIConfig;

/**
 * @brief       Type of structure representing a SPI driver (legacy).
 */
typedef struct hal_spi_driver SPIDriver;

/* Inclusion of LLD header.*/
#include "hal_spi_lld.h"

#if !defined(SPI_SUPPORTS_CIRCULAR)
#error "SPI_SUPPORTS_CIRCULAR not defined in SPI LLD driver"
#endif

#if !defined(SPI_SUPPORTS_SLAVE_MODE)
#error "SPI_SUPPORTS_SLAVE_MODE not defined in SPI LLD driver"
#endif

/**
 * @brief       Driver configuration structure.
 * @note        Implementations may extend this structure to contain more,
 *              architecture dependent, fields.
 */
struct hal_spi_config {
  /**
   * @brief       SPI transfer mode options.
   */
  spi_mode_t                mode;
  /* End of the mandatory fields.*/
  spi_lld_config_fields;
#if (defined(SPI_CONFIG_EXT_FIELDS)) || defined (__DOXYGEN__)
  SPI_CONFIG_EXT_FIELDS
#endif /* defined(SPI_CONFIG_EXT_FIELDS) */
};

/**
 * @brief       Type of user-provided SPI configurations.
 */
typedef struct spi_configurations spi_configurations_t;

/**
 * @brief       Structure representing user-provided SPI configurations.
 */
struct spi_configurations {
  /**
   * @brief       Number of configurations in the open array.
   */
  unsigned                  cfgsnum;
  /**
   * @brief       User SPI configurations.
   */
  hal_spi_config_t          cfgs[];
};

/**
 * @class       hal_spi_driver_c
 * @extends     hal_cb_driver_c
 *
 * @brief       Class of a SPI driver.
 *
 * @name        Class @p hal_spi_driver_c structures
 * @{
 */

/**
 * @brief       Type of a SPI driver class.
 */
typedef struct hal_spi_driver hal_spi_driver_c;

/**
 * @brief       Class @p hal_spi_driver_c virtual methods table.
 */
struct hal_spi_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_cb_driver_c.*/
  void (*setcb)(void *ip, drv_cb_t cb);
  /* From hal_spi_driver_c.*/
};

/**
 * @brief       Structure representing a SPI driver class.
 */
struct hal_spi_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_spi_driver_vmt *vmt;
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
#if (SPI_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Synchronization point for transfer.
   */
  thread_reference_t        sync_transfer;
#endif /* SPI_USE_SYNCHRONIZATION == TRUE */
#if defined(SPI_DRIVER_EXT_FIELDS)
  SPI_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  spi_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_spi_driver_c.*/
  void *__spi_objinit_impl(void *ip, const void *vmt);
  void __spi_dispose_impl(void *ip);
  msg_t __spi_start_impl(void *ip);
  void __spi_stop_impl(void *ip);
  const void *__spi_setcfg_impl(void *ip, const void *config);
  const void *__spi_selcfg_impl(void *ip, unsigned cfgnum);
  msg_t spiStartIgnoreI(void *ip, size_t n);
  msg_t spiStartIgnore(void *ip, size_t n);
  msg_t spiStartExchangeI(void *ip, size_t n, const void *txbuf, void *rxbuf);
  msg_t spiStartExchange(void *ip, size_t n, const void *txbuf, void *rxbuf);
  msg_t spiStartSendI(void *ip, size_t n, const void *txbuf);
  msg_t spiStartSend(void *ip, size_t n, const void *txbuf);
  msg_t spiStartReceiveI(void *ip, size_t n, void *rxbuf);
  msg_t spiStartReceive(void *ip, size_t n, void *rxbuf);
  msg_t spiStopTransferI(void *ip, size_t *np);
  msg_t spiStopTransfer(void *ip, size_t *np);
#if (SPI_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  msg_t spiSynchronizeS(void *ip, sysinterval_t timeout);
  msg_t spiSynchronize(void *ip, sysinterval_t timeout);
  msg_t spiIgnore(void *ip, size_t n);
  msg_t spiExchange(void *ip, size_t n, const void *txbuf, void *rxbuf);
  msg_t spiSend(void *ip, size_t n, const void *txbuf);
  msg_t spiReceive(void *ip, size_t n, void *rxbuf);
#endif /* SPI_USE_SYNCHRONIZATION == TRUE */
  /* Regular functions.*/
  void spiInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_spi_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_spi_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_spi_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_spi_driver_c *spiObjectInit(hal_spi_driver_c *self) {
  extern const struct hal_spi_driver_vmt __hal_spi_driver_vmt;

  return __spi_objinit_impl(self, &__hal_spi_driver_vmt);
}
/** @} */

/**
 * @name        Inline methods of hal_spi_driver_c
 * @{
 */
/**
 * @brief       Returns the configured size, in bytes, of data frames.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 */
CC_FORCE_INLINE
static inline size_t spiGetFrameSizeX(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  return (size_t)(1U << ((__spi_getfield(self, mode) & SPI_MODE_FSIZE_MASK) >> SPI_MODE_FSIZE_POS));
}

/**
 * @brief       Asserts the slave select signal and prepares for transfers.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline void spiSelectX(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  spi_lld_select(self);
}

/**
 * @brief       Deasserts the slave select signal.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline void spiUnselectX(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  spi_lld_unselect(self);
}

#if (SPI_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Wakes up the waiting thread.
 * @note        This function is meant to be used in the low level drivers
 *              implementations only.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     msg           The wakeup message.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline void __spi_wakeup_isr(void *ip, msg_t msg) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  osalSysLockFromISR();
  osalThreadResumeI(&self->sync_transfer, msg);
  osalSysUnlockFromISR();
}

#else

CC_FORCE_INLINE
static inline void __spi_wakeup_isr(void *ip, msg_t msg) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  (void)self;
  (void)msg;
}
#endif /* SPI_USE_SYNCHRONIZATION == TRUE */
/** @} */

#endif /* HAL_USE_SPI == TRUE */

#endif /* HAL_SPI_H */

/** @} */

/*
    ChibiOS - Copyright (C) 2006..2023 Giovanni Di Sirio

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
 * @name    SPI CS modes
 * @{
 */
/**
 * @brief       @p spiSelect() and @p spiUnselect() do nothing.
 */
#define SPI_SELECT_MODE_NONE                0

/**
 * @brief       Selection by PAL port and pad number.
 */
#define SPI_SELECT_MODE_PAD                 1

/**
 * @brief       Selection by port and port mask.
 */
#define SPI_SELECT_MODE_PORT                2

/**
 * @brief       Selection by PAL line identifier.
 */
#define SPI_SELECT_MODE_LINE                3

/**
 * @brief       Selection by LLD-defined mode.
 */
#define SPI_SELECT_MODE_LLD                 4
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
 * @brief       Handling method for SPI CS line.
 */
#if !defined(SPI_SELECT_MODE) || defined(__DOXYGEN__)
#define SPI_SELECT_MODE                     SPI_SELECT_MODE_PAD
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on SPI_USE_SYNCHRONIZATION configuration.*/
#if (SPI_USE_SYNCHRONIZATION != FALSE) && (SPI_USE_SYNCHRONIZATION != TRUE)
#error "invalid SPI_USE_SYNCHRONIZATION value"
#endif

/* Checks on SPI_SELECT_MODE configuration.*/
#if (SPI_SELECT_MODE < SPI_SELECT_MODE_NONE) || (SPI_SELECT_MODE > SPI_SELECT_MODE_LLD)
#error "invalid SPI_SELECT_MODE value"
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

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
#if (SPI_SUPPORTS_CIRCULAR == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Enables the circular buffer mode.
   */
  bool                      circular;
#endif /* SPI_SUPPORTS_CIRCULAR == TRUE */
#if (SPI_SUPPORTS_SLAVE_MODE == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Enables the slave mode.
   */
  bool                      slave;
#endif /* SPI_SUPPORTS_SLAVE_MODE == TRUE */
#if (SPI_SELECT_MODE == SPI_SELECT_MODE_LINE) || defined (__DOXYGEN__)
  /**
   * @brief       The chip select line.
   * @note        Only used in master mode.
   */
  ioline_t                  ssline;
#endif /* SPI_SELECT_MODE == SPI_SELECT_MODE_LINE */
#if (SPI_SELECT_MODE == SPI_SELECT_MODE_PORT) || defined (__DOXYGEN__)
  /**
   * @brief       The chip select port.
   * @note        Only used in master mode.
   */
  ioportid_t                ssport;
  /**
   * @brief       The chip select port mask.
   * @note        Only used in master mode.
   */
  ioportmask_t              ssmask;
#endif /* SPI_SELECT_MODE == SPI_SELECT_MODE_PORT */
#if (SPI_SELECT_MODE == SPI_SELECT_MODE_PAD) || defined (__DOXYGEN__)
  /**
   * @brief       The chip select port.
   * @note        Only used in master mode.
   */
  ioportid_t                ssport;
  /**
   * @brief       The chip select pad number.
   * @note        Only used in master mode.
   */
  ioportmask_t              sspad;
#endif /* SPI_SELECT_MODE == SPI_SELECT_MODE_PAD */
  /* End of the mandatory fields.*/
  spi_lld_config_fields;
#if (defined(SPI_CONFIG_EXT_FIELS)) || defined (__DOXYGEN__)
  SPI_CONFIG_EXT_FIELDS
#endif /* defined(SPI_CONFIG_EXT_FIELS) */
};

/**
 * @class       hal_spi_driver_c
 * @extends     base_object_c, hal_base_driver_c, hal_cb_driver_c.
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
  msg_t (*configure)(void *ip, const void *config);
  /* From hal_cb_driver_c.*/
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
  hal_cb_t                  cb;
#if defined(SPI_DRIVER_EXT_FIELS)
  SPI_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  spi_lld_driver_fields;
};
/** @} */

/**
 * @class       hal_buffered_spi_c
 * @extends     base_object_c, hal_base_driver_c, hal_buffered_serial_c.
 *
 * @brief       This class implements a buffered channel interface on top of
 *              SPI.
 *
 * @name        Class @p hal_buffered_spi_c structures
 * @{
 */

/**
 * @brief       Type of a buffered SPI wrapper class.
 */
typedef struct hal_buffered_spi hal_buffered_spi_c;

/**
 * @brief       Class @p hal_buffered_spi_c virtual methods table.
 */
struct hal_buffered_spi_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip);
  void (*stop)(void *ip);
  msg_t (*configure)(void *ip, const void *config);
  /* From hal_buffered_serial_c.*/
  /* From hal_buffered_spi_c.*/
};

/**
 * @brief       Structure representing a buffered SPI wrapper class.
 */
struct hal_buffered_spi {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_buffered_spi_vmt *vmt;
  /**
   * @brief       Driver state.
   */
  driver_state_t            state;
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
   * @brief       Implemented interface @p asynchronous_channel_i.
   */
  asynchronous_channel_i    chn;
  /**
   * @brief       Input queue.
   */
  input_queue_t             iqueue;
  /**
   * @brief       Output queue.
   */
  output_queue_t            oqueue;
  /**
   * @brief       I/O condition event source.
   */
  event_source_t            event;
  /**
   * @brief       Pointer to the associated @p hal_spi_driver_c instance.
   */
  hal_spi_driver_c          *spip;
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
  /* Methods of hal_buffered_spi_c.*/
  void *__bspi_objinit_impl(void *ip, const void *vmt, hal_spi_driver_c *spip,
                            uint8_t *ib, size_t ibsize, uint8_t *ob,
                            size_t obsize);
  void __bspi_dispose_impl(void *ip);
  msg_t __bspi_start_impl(void *ip);
  void __bspi_stop_impl(void *ip);
  msg_t __bspi_configure_impl(void *ip, const void *config);
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
 * @memberof    hal_spi_driver_c
 *
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
 * @name        Default constructor of hal_buffered_spi_c
 * @{
 */
/**
 * @memberof    hal_buffered_spi_c
 *
 * @brief       Default initialization function of @p hal_buffered_spi_c.
 *
 * @param[out]    self          Pointer to a @p hal_buffered_spi_c instance to
 *                              be initialized.
 * @param[in]     spip          Pointer to the @p hal_spi_driver_c object.
 * @param[in]     ib            Pointer to the input buffer.
 * @param[in]     ibsize        Size of the input buffer.
 * @param[in]     ob            Pointer to the output buffer.
 * @param[in]     obsize        Size of the output buffer.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_buffered_spi_c *bspiObjectInit(hal_buffered_spi_c *self,
                                                 hal_spi_driver_c *spip,
                                                 uint8_t *ib, size_t ibsize,
                                                 uint8_t *ob, size_t obsize) {
  extern const struct hal_buffered_spi_vmt __hal_buffered_spi_vmt;

  return __bspi_objinit_impl(self, &__hal_buffered_spi_vmt, spip, ib, ibsize,
                             ob, obsize);
}
/** @} */

#endif /* HAL_USE_SPI == TRUE */

#endif /* HAL_SPI_H */

/** @} */

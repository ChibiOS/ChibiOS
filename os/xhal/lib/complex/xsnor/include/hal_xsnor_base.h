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
 * @file        hal_xsnor_base.h
 * @brief       Generated SNOR Base Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_XSNOR_BASE
 * @brief       SNOR abstract driver.
 * @details     Base class for SNOR flash devices.
 * @{
 */

#ifndef HAL_XSNOR_BASE_H
#define HAL_XSNOR_BASE_H

#include "hal_flash_base.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Bus type and width options
 * @{
 */
#define XSNOR_BUS_MODE_SPI                  0U
#define XSNOR_BUS_MODE_WSPI_1LINE           1U
#define XSNOR_BUS_MODE_WSPI_2LINES          2U
#define XSNOR_BUS_MODE_WSPI_4LINES          3U
#define XSNOR_BUS_MODE_WSPI_8LINES          4U
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief       Non-cacheable operations buffer.
 */
#if !defined(XSNOR_BUFFER_SIZE) || defined(__DOXYGEN__)
#define XSNOR_BUFFER_SIZE                   32
#endif

/**
 * @brief       SPI support enable switch.
 */
#if !defined(XSNOR_USE_SPI) || defined(__DOXYGEN__)
#define XSNOR_USE_SPI                       TRUE
#endif

/**
 * @brief       WSPI support enable switch.
 */
#if !defined(XSNOR_USE_WSPI) || defined(__DOXYGEN__)
#define XSNOR_USE_WSPI                      TRUE
#endif

/**
 * @brief       Bus share support enable switch.
 */
#if !defined(XSNOR_SHARED_BUS) || defined(__DOXYGEN__)
#define XSNOR_SHARED_BUS                    TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on XSNOR_BUFFER_SIZE configuration.*/
#if XSNOR_BUFFER_SIZE < 32
#error "XSNOR_BUFFER_SIZE minimum is 32"
#endif

#if (XSNOR_BUFFER_SIZE & (XSNOR_BUFFER_SIZE - 1)) != 0
#error "XSNOR_BUFFER_SIZE must be a power of 2"
#endif

/* Checks on XSNOR_USE_SPI configuration.*/
#if (XSNOR_USE_SPI != FALSE) && (XSNOR_USE_SPI != TRUE)
#error "XSNOR_USE_SPI invalid value"
#endif

/* Checks on XSNOR_USE_WSPI configuration.*/
#if (XSNOR_USE_WSPI != FALSE) && (XSNOR_USE_WSPI != TRUE)
#error "XSNOR_USE_WSPI invalid value"
#endif

/* Checks on XSNOR_SHARED_BUS configuration.*/
#if (XSNOR_SHARED_BUS != FALSE) && (XSNOR_SHARED_BUS != TRUE)
#error "XSNOR_SHARED_BUS invalid value"
#endif

/* Other consistency checks.*/
#if (XSNOR_USE_SPI == FALSE) && (XSNOR_USE_WSPI == FALSE)
#error "XSNOR_USE_SPI or XSNOR_USE_WSPI must be enabled"
#endif
#if (XSNOR_USE_SPI == TRUE) && (HAL_USE_SPI == FALSE)
#error "XSNOR_USE_SPI requires HAL_USE_SPI"
#endif
#if (XSNOR_USE_WSPI == TRUE) && (HAL_USE_WSPI == FALSE)
#error "XSNOR_USE_WSPI requires HAL_USE_WSPI"
#endif

/**
 * @brief       This switch is @p TRUE if both SPI and WSPI are in use.
 */
#define XSNOR_USE_BOTH                      ((XSNOR_USE_SPI == TRUE) && (XSNOR_USE_WSPI == TRUE))

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

#if (XSNOR_SHARED_BUS == FALSE) || defined (__DOXYGEN__)
/**
 * @name    Bus mutex macros when sharing is disabled
 * @{
 */
#define __xsnor_bus_acquire(self)
#define __xsnor_bus_select(self)                                            \
  ((void)(self), HAL_RET_SUCCESS)
#define __xsnor_bus_release(self)
/** @} */
#endif /* XSNOR_SHARED_BUS == FALSE */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of a non-cacheable buffer.
 */
typedef struct xsnor_buffers xsnor_buffers_t;

/**
 * @brief       Type of a SNOR configuration structure.
 */
typedef struct xsnor_config xsnor_config_t;

/**
 * @brief       Type of a commands configuration structure.
 */
typedef struct xsnor_commands xsnor_commands_t;

/**
 * @brief       SNOR driver configuration.
 */
struct xsnor_buffers {
  /**
   * @brief       Non-cacheable data buffer.
   */
  uint8_t                   databuf[XSNOR_BUFFER_SIZE];
#if (XSNOR_USE_WSPI == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Non-cacheable WSPI command buffer.
   */
  wspi_command_t            cmdbuf;
#endif /* XSNOR_USE_WSPI == TRUE */
#if (XSNOR_USE_SPI == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Non-cacheable SPI buffer.
   */
  uint8_t                   spibuf[8];
#endif /* XSNOR_USE_SPI == TRUE */
};

/**
 * @brief       SNOR command configuration structure.
 */
struct xsnor_commands {
  /**
   * @brief       Command only.
   */
  uint32_t                  cmd;
  /**
   * @brief       Command and address.
   */
  uint32_t                  cmd_addr;
  /**
   * @brief       Command and data.
   */
  uint32_t                  cmd_data;
  /**
   * @brief       Command, address and data.
   */
  uint32_t                  cmd_addr_data;
};

#if (XSNOR_USE_WSPI == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       WSPI-specific configuration fields.
 */
struct xsnor_bus_wspi {
  /**
   * @brief       WSPI driver to be used for physical communication.
   */
  hal_wspi_driver_c         *drv;
  /**
   * @brief       WSPI driver configuration.
   */
  const hal_wspi_config_t   *cfg;
};
#endif /* XSNOR_USE_WSPI == TRUE */

#if (XSNOR_USE_SPI == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       SPI-specific configuration fields.
 */
struct xsnor_bus_spi {
  /**
   * @brief       SPI driver to be used for physical communication.
   */
  SPIDriver                 *drv;
  /**
   * @brief       SPI driver configuration.
   */
  const SPIConfig           *cfg;
};
#endif /* XSNOR_USE_SPI == TRUE */

/**
 * @brief       Union of possible bus configurations.
 */
union xsnor_bus_configs {
#if (XSNOR_USE_WSPI == TRUE) || defined (__DOXYGEN__)
  struct xsnor_bus_wspi     wspi;
#endif /* XSNOR_USE_WSPI == TRUE */
#if (XSNOR_USE_SPI == TRUE) || defined (__DOXYGEN__)
  struct xsnor_bus_spi      spi;
#endif /* XSNOR_USE_SPI == TRUE */
};

/**
 * @brief       SNOR driver configuration.
 */
struct xsnor_config {
  /**
   * @brief       Bus type selection switch.
   */
  int                       bus_type;
  /**
   * @brief       WSPI driver configuration.
   */
  union xsnor_bus_configs   bus;
  /**
   * @brief       Pointer to the non-cacheable buffers.
   */
  xsnor_buffers_t           *buffers;
  /**
   * @brief       Device-dependent options, used by subclasses only.
   */
  int                       options;
};

/**
 * @class       hal_xsnor_base_c
 * @extends     hal_flash_base_c
 *
 * @brief       Base class of all SNOR drivers.
 *
 * @name        Class @p hal_xsnor_base_c structures
 * @{
 */

/**
 * @brief       Type of a XSNOR base driver class.
 */
typedef struct hal_xsnor_base hal_xsnor_base_c;

/**
 * @brief       Class @p hal_xsnor_base_c virtual methods table.
 */
struct hal_xsnor_base_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip, const void *config);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_flash_base_c.*/
  flash_error_t (*read)(void *ip, flash_offset_t offset, size_t n, uint8_t *rp);
  flash_error_t (*program)(void *ip, flash_offset_t offset, size_t n, const uint8_t *pp);
  flash_error_t (*start_erase_all)(void *ip);
  flash_error_t (*start_erase_sector)(void *ip, flash_sector_t sector);
  flash_error_t (*query_erase)(void *ip, unsigned *msec);
  flash_error_t (*verify_erase)(void *ip, flash_sector_t sector);
  /* From hal_xsnor_base_c.*/
  flash_error_t (*init)(void *ip);
  flash_error_t (*mmap_on)(void *ip, uint8_t **addrp);
  void (*mmap_off)(void *ip);
};

/**
 * @brief       Structure representing a XSNOR base driver class.
 */
struct hal_xsnor_base {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_xsnor_base_vmt *vmt;
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
   * @brief       Implemented interface @p flash_interface_i.
   */
  flash_interface_i         fls;
  /**
   * @brief       Flash descriptor.
   */
  flash_descriptor_t        descriptor;
#if (XSNOR_USE_WSPI == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Current commands configuration.
   * @note        This field is meant to be initialized by subclasses on object
   *              creation.
   */
  const xsnor_commands_t    *commands;
#endif /* XSNOR_USE_WSPI == TRUE */
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_xsnor_base_c.*/
  void *__xsnor_objinit_impl(void *ip, const void *vmt);
  void __xsnor_dispose_impl(void *ip);
  msg_t __xsnor_start_impl(void *ip, const void *config);
  void __xsnor_stop_impl(void *ip);
  const void *__xsnor_setcfg_impl(void *ip, const void *config);
  const void *__xsnor_selcfg_impl(void *ip, unsigned cfgnum);
#if (XSNOR_USE_SPI == TRUE) || defined (__DOXYGEN__)
  void __xsnor_spi_cmd_addr(void *ip, uint32_t cmd, flash_offset_t offset);
#endif /* XSNOR_USE_SPI == TRUE */
#if (XSNOR_SHARED_BUS == TRUE) || defined (__DOXYGEN__)
  void __xsnor_bus_acquire(void *ip);
  msg_t __xsnor_bus_select(void *ip);
  void __xsnor_bus_release(void *ip);
#endif /* XSNOR_SHARED_BUS == TRUE */
  void __xsnor_bus_cmd(void *ip, uint32_t cmd);
  void __xsnor_bus_cmd_send(void *ip, uint32_t cmd, size_t n, const uint8_t *p);
  void __xsnor_bus_cmd_receive(void *ip, uint32_t cmd, size_t n, uint8_t *p);
  void __xsnor_bus_cmd_addr(void *ip, uint32_t cmd, flash_offset_t offset);
  void __xsnor_bus_cmd_addr_send(void *ip, uint32_t cmd, flash_offset_t offset,
                                 size_t n, const uint8_t *p);
  void __xsnor_bus_cmd_addr_receive(void *ip, uint32_t cmd,
                                    flash_offset_t offset, size_t n,
                                    uint8_t *p);
  void __xsnor_bus_cmd_dummy_receive(void *ip, uint32_t cmd, uint32_t dummy,
                                     size_t n, uint8_t *p);
  void __xsnor_bus_cmd_addr_dummy_receive(void *ip, uint32_t cmd,
                                          flash_offset_t offset,
                                          uint32_t dummy, size_t n, uint8_t *p);
#if ((XSNOR_USE_WSPI == TRUE) && defined(WSPI_SUPPORTS_MEMMAP) && (WSPI_SUPPORTS_MEMMAP == TRUE)) || defined (__DOXYGEN__)
  flash_error_t xsnorMemoryMap(void *ip, uint8_t **addrp);
  void xsnorMemoryUnmap(void *ip);
#endif /* (XSNOR_USE_WSPI == TRUE) && defined(WSPI_SUPPORTS_MEMMAP) && (WSPI_SUPPORTS_MEMMAP == TRUE) */
  /* Regular functions.*/
  bool __xsnor_find_id(const uint8_t *set, size_t size, uint8_t element);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Virtual methods of hal_xsnor_base_c
 * @{
 */
CC_FORCE_INLINE
static inline flash_error_t xsnor_device_init(void *ip) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;

  return self->vmt->init(ip);
}

CC_FORCE_INLINE
static inline flash_error_t xsnor_device_mmap_on(void *ip, uint8_t **addrp) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;

  return self->vmt->mmap_on(ip, addrp);
}

CC_FORCE_INLINE
static inline void xsnor_device_mmap_off(void *ip) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;

  self->vmt->mmap_off(ip);
}
/** @} */

#endif /* HAL_XSNOR_BASE_H */

/** @} */

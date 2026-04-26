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
 * @file        hal_flash_base.h
 * @brief       Generated Flash Base Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_FLASH_BASE
 * @brief       Common base class for flash devices.
 * @details     This abstract class implements the generic flash interface and
 *              provides common state handling and helper functions for flash
 *              device drivers.
 * @{
 */

#ifndef HAL_FLASH_BASE_H
#define HAL_FLASH_BASE_H

#include "hal_base_driver.h"
#include "hal_flash_interface.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of a flash base driver class (legacy).
 */
typedef struct hal_flash_base BaseFlash;

/**
 * @brief       Driver state machine possible states.
 */
typedef driver_state_t flash_state_t;

typedef enum {
  FLASH_READ  = HAL_DRV_STATE_ACTIVE + 1U,
  FLASH_PGM   = HAL_DRV_STATE_ACTIVE + 2U,
  FLASH_ERASE = HAL_DRV_STATE_ACTIVE + 3U,
  FLASH_MMAP  = HAL_DRV_STATE_ACTIVE + 4U
} flash_state_ex_t;

/**
 * @class       hal_flash_base_c
 * @extends     hal_base_driver_c
 * @implements  flash_interface_i
 *
 * @brief       Base class of all flash drivers.
 *
 * @name        Class @p hal_flash_base_c structures
 * @{
 */

/**
 * @brief       Type of a flash base driver class.
 */
typedef struct hal_flash_base hal_flash_base_c;

/**
 * @brief       Class @p hal_flash_base_c virtual methods table.
 */
struct hal_flash_base_vmt {
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
};

/**
 * @brief       Structure representing a flash base driver class.
 */
struct hal_flash_base {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_flash_base_vmt *vmt;
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
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_flash_base_c.*/
  void *__flash_objinit_impl(void *ip, const void *vmt);
  void __flash_dispose_impl(void *ip);
  flash_error_t flashWaitErase(void *ip);
  flash_offset_t flashGetSectorOffset(void *ip, flash_sector_t sector);
  uint32_t flashGetSectorSize(void *ip, flash_sector_t sector);
  flash_sector_t flashGetOffsetSector(void *ip, flash_offset_t offset);
  void *flashGetOffsetAddress(void *ip, flash_offset_t offset);
  flash_offset_t flashGetAddressOffset(void *ip, void *addr);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Virtual methods of hal_flash_base_c
 * @{
 */
/**
 * @brief       Read operation.
 *
 * @param[in,out] ip            Pointer to a @p hal_flash_base_c instance.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be read.
 * @param[out]    rp            Pointer to the data buffer.
 * @return                      An error code.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline flash_error_t __flash_read(void *ip, flash_offset_t offset,
                                         size_t n, uint8_t *rp) {
  hal_flash_base_c *self = (hal_flash_base_c *)ip;

  return self->vmt->read(ip, offset, n, rp);
}

/**
 * @brief       Program operation.
 *
 * @param[in,out] ip            Pointer to a @p hal_flash_base_c instance.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be programmed.
 * @param[in]     pp            Pointer to the data buffer.
 * @return                      An error code.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline flash_error_t __flash_program(void *ip, flash_offset_t offset,
                                            size_t n, const uint8_t *pp) {
  hal_flash_base_c *self = (hal_flash_base_c *)ip;

  return self->vmt->program(ip, offset, n, pp);
}

/**
 * @brief       Starts a whole-device erase operation.
 *
 * @param[in,out] ip            Pointer to a @p hal_flash_base_c instance.
 * @return                      An error code.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline flash_error_t __flash_start_erase_all(void *ip) {
  hal_flash_base_c *self = (hal_flash_base_c *)ip;

  return self->vmt->start_erase_all(ip);
}

/**
 * @brief       Starts a sector erase operation.
 *
 * @param[in,out] ip            Pointer to a @p hal_flash_base_c instance.
 * @param[in]     sector        Sector to be erased.
 * @return                      An error code.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline flash_error_t __flash_start_erase_sector(void *ip,
                                                       flash_sector_t sector) {
  hal_flash_base_c *self = (hal_flash_base_c *)ip;

  return self->vmt->start_erase_sector(ip, sector);
}

/**
 * @brief       Queries the driver for erase operation progress.
 *
 * @param[in,out] ip            Pointer to a @p hal_flash_base_c instance.
 * @param[out]    msec          Recommended time, in milliseconds, before
 *                              retrying, can be @p NULL.
 * @return                      An error code.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline flash_error_t __flash_query_erase(void *ip, unsigned *msec) {
  hal_flash_base_c *self = (hal_flash_base_c *)ip;

  return self->vmt->query_erase(ip, msec);
}

/**
 * @brief       Returns the erase state of a sector.
 *
 * @param[in,out] ip            Pointer to a @p hal_flash_base_c instance.
 * @param[in]     sector        Sector to be verified.
 * @return                      An error code.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline flash_error_t __flash_verify_erase(void *ip,
                                                 flash_sector_t sector) {
  hal_flash_base_c *self = (hal_flash_base_c *)ip;

  return self->vmt->verify_erase(ip, sector);
}
/** @} */

#endif /* HAL_FLASH_BASE_H */

/** @} */

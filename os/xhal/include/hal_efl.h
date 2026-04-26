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
 * @file        hal_efl.h
 * @brief       Generated Embedded Flash Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_EFL
 * @brief       Embedded Flash Driver macros and structures.
 * @{
 */

#ifndef HAL_EFL_H
#define HAL_EFL_H

#include "hal_flash_base.h"

#if (HAL_USE_EFL == TRUE) || defined(__DOXYGEN__)

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
 * @brief       Type of structure representing an embedded flash driver.
 */
typedef struct hal_efl_driver hal_efl_driver_c;

/**
 * @brief       Type of structure representing an embedded flash configuration.
 */
typedef struct hal_efl_config hal_efl_config_t;

typedef struct hal_efl_driver EFlashDriver;
typedef struct hal_efl_config EFlashConfig;

/* Inclusion of LLD header.*/
#include "hal_efl_lld.h"

/**
 * @brief       Driver configuration structure.
 */
struct hal_efl_config {
  /* End of the mandatory fields.*/
  efl_lld_config_fields;
};

/**
 * @class       hal_efl_driver_c
 * @extends     hal_flash_base_c
 *
 * @brief       Class of an embedded flash driver.
 *
 * @name        Class @p hal_efl_driver_c structures
 * @{
 */

/**
 * @brief       Type of a Embedded Flash driver class.
 */
typedef struct hal_efl_driver hal_efl_driver_c;

/**
 * @brief       Class @p hal_efl_driver_c virtual methods table.
 */
struct hal_efl_driver_vmt {
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
  /* From hal_efl_driver_c.*/
};

/**
 * @brief       Structure representing a Embedded Flash driver class.
 */
struct hal_efl_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_efl_driver_vmt *vmt;
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
  /* End of the mandatory fields.*/
  efl_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_efl_driver_c.*/
  void *__efl_objinit_impl(void *ip, const void *vmt);
  void __efl_dispose_impl(void *ip);
  msg_t __efl_start_impl(void *ip, const void *config);
  void __efl_stop_impl(void *ip);
  const void *__efl_setcfg_impl(void *ip, const void *config);
  const void *__efl_selcfg_impl(void *ip, unsigned cfgnum);
  flash_error_t __efl_read_impl(void *ip, flash_offset_t offset, size_t n,
                                uint8_t *rp);
  flash_error_t __efl_program_impl(void *ip, flash_offset_t offset, size_t n,
                                   const uint8_t *pp);
  flash_error_t __efl_start_erase_all_impl(void *ip);
  flash_error_t __efl_start_erase_sector_impl(void *ip, flash_sector_t sector);
  flash_error_t __efl_query_erase_impl(void *ip, unsigned *msec);
  flash_error_t __efl_verify_erase_impl(void *ip, flash_sector_t sector);
  /* Regular functions.*/
  void eflInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_efl_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_efl_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_efl_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_efl_driver_c *eflObjectInit(hal_efl_driver_c *self) {
  extern const struct hal_efl_driver_vmt __hal_efl_driver_vmt;

  return __efl_objinit_impl(self, &__hal_efl_driver_vmt);
}
/** @} */

#endif /* HAL_USE_EFL == TRUE */

#endif /* HAL_EFL_H */

/** @} */

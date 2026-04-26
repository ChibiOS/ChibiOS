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
 * @file        hal_trng.h
 * @brief       Generated TRNG Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_TRNG
 * @brief       TRNG Driver macros and structures.
 * @{
 */

#ifndef HAL_TRNG_H
#define HAL_TRNG_H

#include "hal_base_driver.h"

#if (HAL_USE_TRNG == TRUE) || defined(__DOXYGEN__)

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
 * @brief       Type of structure representing a TRNG driver.
 */
typedef struct hal_trng_driver hal_trng_driver_c;

/**
 * @brief       Type of structure representing a TRNG configuration.
 */
typedef struct hal_trng_config hal_trng_config_t;

typedef struct hal_trng_driver TRNGDriver;
typedef struct hal_trng_config TRNGConfig;

/* Inclusion of LLD header.*/
#include "hal_trng_lld.h"

/**
 * @brief       Driver configuration structure.
 */
struct hal_trng_config {
  /* End of the mandatory fields.*/
  trng_lld_config_fields;
};

/**
 * @class       hal_trng_driver_c
 * @extends     hal_base_driver_c
 *
 * @brief       Class of a TRNG driver.
 *
 * @name        Class @p hal_trng_driver_c structures
 * @{
 */

/**
 * @brief       Type of a TRNG driver class.
 */
typedef struct hal_trng_driver hal_trng_driver_c;

/**
 * @brief       Class @p hal_trng_driver_c virtual methods table.
 */
struct hal_trng_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip, const void *config);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_trng_driver_c.*/
};

/**
 * @brief       Structure representing a TRNG driver class.
 */
struct hal_trng_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_trng_driver_vmt *vmt;
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
  /* End of the mandatory fields.*/
  trng_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_trng_driver_c.*/
  void *__trng_objinit_impl(void *ip, const void *vmt);
  void __trng_dispose_impl(void *ip);
  msg_t __trng_start_impl(void *ip, const void *config);
  void __trng_stop_impl(void *ip);
  const void *__trng_setcfg_impl(void *ip, const void *config);
  const void *__trng_selcfg_impl(void *ip, unsigned cfgnum);
  msg_t trngGenerate(void *ip, size_t size, uint8_t *out);
  /* Regular functions.*/
  void trngInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_trng_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_trng_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_trng_driver_c instance to
 *                              be initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_trng_driver_c *trngObjectInit(hal_trng_driver_c *self) {
  extern const struct hal_trng_driver_vmt __hal_trng_driver_vmt;

  return __trng_objinit_impl(self, &__hal_trng_driver_vmt);
}
/** @} */

#endif /* HAL_USE_TRNG == TRUE */

#endif /* HAL_TRNG_H */

/** @} */

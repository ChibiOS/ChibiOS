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
 * @file        hal_cb_driver.h
 * @brief       Generated Callback Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_CB_DRIVER
 * @brief       Common ancestor of callback-based drivers.
 * @{
 */

#ifndef HAL_CB_DRIVER_H
#define HAL_CB_DRIVER_H

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
 * @brief       Generic HAL notification callback type.
 */
typedef void (*hal_cb_t)(void *ip);

/**
 * @class       hal_cb_driver_c
 * @extends     base_object_c, hal_base_driver_c.
 *
 * @brief       Class of a callback-based driver.
 *
 * @name        Class @p hal_cb_driver_c structures
 * @{
 */

/**
 * @brief       Type of a callback driver class.
 */
typedef struct hal_cb_driver hal_cb_driver_c;

/**
 * @brief       Class @p hal_cb_driver_c virtual methods table.
 */
struct hal_cb_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip);
  void (*stop)(void *ip);
  msg_t (*configure)(void *ip, const void *config);
  /* From hal_cb_driver_c.*/
};

/**
 * @brief       Structure representing a callback driver class.
 */
struct hal_cb_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_cb_driver_vmt *vmt;
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
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_cb_driver_c.*/
  void *__cbdrv_objinit_impl(void *ip, const void *vmt);
  void __cbdrv_dispose_impl(void *ip);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Inline methods of hal_cb_driver_c
 * @{
 */
/**
 * @memberof    hal_cb_driver_c
 * @public
 *
 * @brief       Associates a callback to the driver instance.
 *
 * @param[in,out] ip            Pointer to a @p hal_cb_driver_c instance.
 * @param         cb            Callback function to be associated. Passing @p
 *                              NULL disables the existing callback, if any.
 */
CC_FORCE_INLINE
static inline void drvSetCallback(void *ip, hal_cb_t cb) {
  hal_cb_driver_c *self = (hal_cb_driver_c *)ip;

  self->cb = cb;
}

/**
 * @memberof    hal_cb_driver_c
 * @public
 *
 * @brief       Returns the callback associated to the driver instance.
 *
 * @param[in,out] ip            Pointer to a @p hal_cb_driver_c instance.
 */
CC_FORCE_INLINE
static inline hal_cb_t drvGetCallback(void *ip) {
  hal_cb_driver_c *self = (hal_cb_driver_c *)ip;

  return self->cb;
}
/** @} */

#endif /* HAL_CB_DRIVER_H */

/** @} */

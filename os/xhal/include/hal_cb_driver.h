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

/**
 * @name    Callback-related driver states
 * @{
 */
#define HAL_DRV_STATE_COMPLETE              6U
#define HAL_DRV_STATE_ERROR                 7U
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Support macros
 * @{
 */
/**
 * @brief       Callback invocation.
 *
 * @param[in,out] self          Pointer to driver instance.
 *
 * @notapi
 */
#define __cbdrv_invoke_cb(self)                                             \
  do {                                                                      \
    if ((self)->cb != NULL) {                                               \
      (self)->cb(self);                                                     \
    }                                                                       \
  } while (false)

/**
 * @brief       Callback invocation with state transitions management.
 *
 * @param[in,out] self          Pointer to driver instance.
 * @param[in]     cbstate       Driver state during callback execution.
 * @param[in]     endstate      Driver state after callback return.
 *
 * @notapi
 */
#define __cbdrv_invoke_cb_with_transition(self, cbstate, endstate)          \
  do {                                                                      \
    if ((self)->cb != NULL) {                                               \
      (self)->state = (cbstate);                                            \
      (self)->cb(self);                                                     \
      if ((self)->state == (cbstate)) {                                     \
        (self)->state = (endstate);                                         \
      }                                                                     \
    }                                                                       \
    else {                                                                  \
      self->state = (endstate);                                             \
    }                                                                       \
  } while (false)
/** @} */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of driver status.
 */
typedef eventflags_t drv_status_t;

/**
 * @brief       Driver callback type.
 */
typedef void (*drv_cb_t)(void *ip);

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
  const void * (*doconf)(void *ip, const void *config);
  /* From hal_cb_driver_c.*/
  void (*setcb)(void *ip, drv_cb_t cb);
  drv_status_t (*gsts)(void *ip);
  drv_status_t (*gcsts)(void *ip, drv_status_t mask);
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
  void __cbdrv_setcb_impl(void *ip, drv_cb_t cb);
  drv_status_t __cbdrv_gsts_impl(void *ip);
  drv_status_t __cbdrv_gcsts_impl(void *ip, drv_status_t mask);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Virtual methods of hal_cb_driver_c
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
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline void drvSetCallbackX(void *ip, drv_cb_t cb) {
  hal_cb_driver_c *self = (hal_cb_driver_c *)ip;

  self->vmt->setcb(ip, cb);
}

/**
 * @memberof    hal_cb_driver_c
 * @public
 *
 * @brief       Returns all driver status flags without clearing.
 *
 * @param[in,out] ip            Pointer to a @p hal_cb_driver_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline drv_status_t drvGetStatusX(void *ip) {
  hal_cb_driver_c *self = (hal_cb_driver_c *)ip;

  return self->vmt->gsts(ip);
}

/**
 * @memberof    hal_cb_driver_c
 * @public
 *
 * @brief       Returns the specified driver status flags and clears them.
 *
 * @param[in,out] ip            Pointer to a @p hal_cb_driver_c instance.
 * @param[in]     mask          Flags to be returned and cleared.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline drv_status_t drvGetAndClearStatusI(void *ip, drv_status_t mask) {
  hal_cb_driver_c *self = (hal_cb_driver_c *)ip;

  return self->vmt->gcsts(ip, mask);
}
/** @} */

/**
 * @name        Inline methods of hal_cb_driver_c
 * @{
 */
/**
 * @memberof    hal_cb_driver_c
 * @public
 *
 * @brief       Returns the callback associated to the driver instance.
 *
 * @param[in,out] ip            Pointer to a @p hal_cb_driver_c instance.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline drv_cb_t drvGetCallbackX(void *ip) {
  hal_cb_driver_c *self = (hal_cb_driver_c *)ip;

  return self->cb;
}

/**
 * @memberof    hal_cb_driver_c
 * @public
 *
 * @brief       Checks for @p HAL_DRV_STATE_COMPLETE state.
 * @details     The @p HAL_DRV_STATE_COMPLETE state is used by those drivers
 *              triggering multiple callbacks for a single asynchronous
 *              operation, it marks the last callback in the sequence.
 * @note        This function is meant to be called exclusively from the driver
 *              callback.
 *
 * @param[in,out] ip            Pointer to a @p hal_cb_driver_c instance.
 * @return                      The check result.
 * @retval false                If the current state is not @p
 *                              HAL_DRV_STATE_COMPLETE.
 * @retval true                 If the current state is @p
 *                              HAL_DRV_STATE_COMPLETE.
 *
 * @api
 */
CC_FORCE_INLINE
static inline bool drvStateIsCompleteI(void *ip) {
  hal_cb_driver_c *self = (hal_cb_driver_c *)ip;

  return (bool)(self->state == HAL_DRV_STATE_COMPLETE);
}

/**
 * @memberof    hal_cb_driver_c
 * @public
 *
 * @brief       Checks for @p HAL_DRV_STATE_ERROR state.
 * @details     The @p HAL_DRV_STATE_ERROR state during a callback marks an
 *              error in an asynchronous operation, the operation is implicitly
 *              stopped and the driver is switched back to its @p
 *              HAL_DRV_STATE_READY state.
 * @note        This function is meant to be called exclusively from the driver
 *              callback.
 *
 * @param[in,out] ip            Pointer to a @p hal_cb_driver_c instance.
 * @return                      The check result.
 * @retval false                If the current state is not @p
 *                              HAL_DRV_STATE_ERROR.
 * @retval true                 If the current state is @p HAL_DRV_STATE_ERROR.
 *
 * @api
 */
CC_FORCE_INLINE
static inline bool drvStateIsErrorI(void *ip) {
  hal_cb_driver_c *self = (hal_cb_driver_c *)ip;

  return (bool)(self->state == HAL_DRV_STATE_ERROR);
}
/** @} */

#endif /* HAL_CB_DRIVER_H */

/** @} */

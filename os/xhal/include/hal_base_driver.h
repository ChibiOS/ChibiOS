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
 * @file        hal_base_driver.h
 * @brief       Generated Base Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_BASE_DRIVER
 * @brief       Common driver base abstract class.
 * @details     This abstract class is the common ancestor of all HAL stateful
 *              HAL drivers.
 * @{
 */

#ifndef HAL_BASE_DRIVER_H
#define HAL_BASE_DRIVER_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Common driver states
 * @{
 */
#define HAL_DRV_STATE_UNINIT                0U
#define HAL_DRV_STATE_STOP                  1U
#define HAL_DRV_STATE_READY                 2U
#define HAL_DRV_STATE_ACTIVE                3U
#define HAL_DRV_STATE_ERROR                 4U
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief       Enables the mutual exclusion APIs on driver instances.
 * @note        Disabling this option saves both code and data space.
 */
#if !defined(HAL_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define HAL_USE_MUTUAL_EXCLUSION            TRUE
#endif

/**
 * @brief       Enables the HAL registry for drivers.
 * @note        Disabling this option saves both code and data space.
 */
#if !defined(HAL_USE_REGISTRY) || defined(__DOXYGEN__)
#define HAL_USE_REGISTRY                    TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on HAL_USE_MUTUAL_EXCLUSION configuration.*/
#if (HAL_USE_MUTUAL_EXCLUSION != FALSE) && (HAL_USE_MUTUAL_EXCLUSION != TRUE)
#error "invalid HAL_USE_MUTUAL_EXCLUSION value"
#endif

/* Checks on HAL_USE_REGISTRY configuration.*/
#if (HAL_USE_REGISTRY != FALSE) && (HAL_USE_REGISTRY != TRUE)
#error "invalid HAL_USE_REGISTRY value"
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

typedef struct hal_base_driver hal_base_driver_c;

/**
 * @brief       Type of a driver state variable.
 */
typedef unsigned int driver_state_t;

#if (HAL_USE_REGISTRY == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Type of a registry entry structure.
 */
typedef struct hal_regent hal_regent_t;

/**
 * @brief       Structure representing a registry entry.
 */
struct hal_regent {
  /**
   * @brief       Next entry in the drivers registry.
   */
  hal_regent_t              *next;
  /**
   * @brief       Previous entry in the drivers registry.
   */
  hal_regent_t              *prev;
};
#endif /* HAL_USE_REGISTRY == TRUE */

/**
 * @class       hal_base_driver_c
 * @extends     base_object_c.
 *
 * @brief       Ancestor class of stateful HAL drivers.
 *
 * @name        Class @p hal_base_driver_c structures
 * @{
 */

/**
 * @brief       Type of a HAL base driver class.
 */
typedef struct hal_base_driver hal_base_driver_c;

/**
 * @brief       Class @p hal_base_driver_c virtual methods table.
 */
struct hal_base_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip);
  void (*stop)(void *ip);
  msg_t (*configure)(void *ip, const void *config);
};

/**
 * @brief       Structure representing a HAL base driver class.
 */
struct hal_base_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_base_driver_vmt *vmt;
  /**
   * @brief       Driver state.
   */
  driver_state_t            state;
  /**
   * @brief       Driver owner.
   */
  void                      *owner;
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
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (HAL_USE_REGISTRY == TRUE) || defined (__DOXYGEN__)
extern hal_regent_t         hal_registry;
#endif /* HAL_USE_REGISTRY == TRUE */

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_base_driver_c.*/
  void *__drv_objinit_impl(void *ip, const void *vmt);
  void __drv_dispose_impl(void *ip);
  msg_t drvStart(void *ip);
  void drvStop(void *ip);
  /* Regular functions.*/
  void drvInit(void);
#if HAL_USE_REGISTRY == TRUE
  hal_base_driver_c *drvRegGetFirstX(void);
  hal_base_driver_c *drvRegGetNextX(hal_base_driver_c *drvp);
  hal_base_driver_c *drvStartByName(const char *name, msg_t *msgp);
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Virtual methods of hal_base_driver_c
 * @{
 */
/**
 * @memberof    hal_base_driver_c
 * @public
 *
 * @brief       Low level driver start.
 *
 * @param[in,out] ip            Pointer to a @p hal_base_driver_c instance.
 * @return                      The operation status.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline msg_t __drv_start(void *ip) {
  hal_base_driver_c *self = (hal_base_driver_c *)ip;

  return self->vmt->start(ip);
}

/**
 * @memberof    hal_base_driver_c
 * @public
 *
 * @brief       Low level driver stop.
 *
 * @param[in,out] ip            Pointer to a @p hal_base_driver_c instance.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline void __drv_stop(void *ip) {
  hal_base_driver_c *self = (hal_base_driver_c *)ip;

  self->vmt->stop(ip);
}

/**
 * @memberof    hal_base_driver_c
 * @public
 *
 * @brief       Driver configure.
 * @details     Applies a new configuration to the driver. The configuration
 *              structure is architecture-dependent.
 * @note        Applying a configuration should be done while the peripheral is
 *              not actively operating, this function can fail depending on the
 *              driver implementation and current state.
 *
 * @param[in,out] ip            Pointer to a @p hal_base_driver_c instance.
 * @param[in]     config        New driver configuration.
 *
 * @api
 */
CC_FORCE_INLINE
static inline msg_t drvConfigureX(void *ip, const void *config) {
  hal_base_driver_c *self = (hal_base_driver_c *)ip;

  return self->vmt->configure(ip, config);
}
/** @} */

/**
 * @name        Inline methods of hal_base_driver_c
 * @{
 */
/**
 * @memberof    hal_base_driver_c
 * @public
 *
 * @brief       Driver state get.
 *
 * @param[in,out] ip            Pointer to a @p hal_base_driver_c instance.
 * @return                      The driver state.
 *
 * @api
 */
CC_FORCE_INLINE
static inline driver_state_t drvGetStateX(void *ip) {
  hal_base_driver_c *self = (hal_base_driver_c *)ip;

  return self->state;
}

/**
 * @memberof    hal_base_driver_c
 * @public
 *
 * @brief       Driver state set.
 *
 * @param[in,out] ip            Pointer to a @p hal_base_driver_c instance.
 * @param[in]     state         New driver state.
 *
 * @api
 */
CC_FORCE_INLINE
static inline void drvSetStateX(void *ip, driver_state_t state) {
  hal_base_driver_c *self = (hal_base_driver_c *)ip;

  self->state = state;
}

/**
 * @memberof    hal_base_driver_c
 * @public
 *
 * @brief       Driver owner get.
 *
 * @param[in,out] ip            Pointer to a @p hal_base_driver_c instance.
 * @return                      The driver owner.
 *
 * @api
 */
CC_FORCE_INLINE
static inline void *drvGetOwnerX(void *ip) {
  hal_base_driver_c *self = (hal_base_driver_c *)ip;

  return self->owner;
}

/**
 * @memberof    hal_base_driver_c
 * @public
 *
 * @brief       Driver owner set.
 *
 * @param[in,out] ip            Pointer to a @p hal_base_driver_c instance.
 * @param[in]     owner         New driver owner.
 *
 * @api
 */
CC_FORCE_INLINE
static inline void drvSetOwnerX(void *ip, void *owner) {
  hal_base_driver_c *self = (hal_base_driver_c *)ip;

  self->owner = owner;
}

/**
 * @memberof    hal_base_driver_c
 * @public
 *
 * @brief       Driver name get.
 * @note        Returns "unk" if registry is disabled.
 *
 * @param[in,out] ip            Pointer to a @p hal_base_driver_c instance.
 * @return                      The driver name.
 *
 * @api
 */
CC_FORCE_INLINE
static inline const char *drvGetNameX(void *ip) {
  hal_base_driver_c *self = (hal_base_driver_c *)ip;

#if HAL_USE_REGISTRY == TRUE
  return self->name;
#else
  return "unk";
#endif
}

/**
 * @memberof    hal_base_driver_c
 * @public
 *
 * @brief       Driver name set.
 * @note        Does nothing if registry is disabled.
 *
 * @param[in,out] ip            Pointer to a @p hal_base_driver_c instance.
 * @param[in]     name          New driver name.
 *
 * @api
 */
CC_FORCE_INLINE
static inline void drvSetNameX(void *ip, const char *name) {
  hal_base_driver_c *self = (hal_base_driver_c *)ip;

#if HAL_USE_REGISTRY == TRUE
  self->name = name;
#else
  (void)name;
#endif
}

#if (HAL_USE_MUTUAL_EXCLUSION == TRUE) || defined (__DOXYGEN__)
/**
 * @memberof    hal_base_driver_c
 * @public
 *
 * @brief       Driver lock.
 *
 * @param[in,out] ip            Pointer to a @p hal_base_driver_c instance.
 *
 * @api
 */
CC_FORCE_INLINE
static inline void drvLock(void *ip) {
  hal_base_driver_c *self = (hal_base_driver_c *)ip;

  osalMutexLock(&self->mutex);
}

/**
 * @memberof    hal_base_driver_c
 * @public
 *
 * @brief       Driver unlock.
 *
 * @param[in,out] ip            Pointer to a @p hal_base_driver_c instance.
 *
 * @api
 */
CC_FORCE_INLINE
static inline void drvUnlock(void *ip) {
  hal_base_driver_c *self = (hal_base_driver_c *)ip;

  osalMutexUnlock(&self->mutex);
}
#endif /* HAL_USE_MUTUAL_EXCLUSION == TRUE */
/** @} */

#endif /* HAL_BASE_DRIVER_H */

/** @} */

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
 * @file        hal_gpt.h
 * @brief       Generated GPT Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_GPT
 * @brief       GPT Driver macros and structures.
 * @{
 */

#ifndef HAL_GPT_H
#define HAL_GPT_H

#include "hal_cb_driver.h"

#if (HAL_USE_GPT == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief       Default GPT timer frequency.
 */
#if !defined(GPT_DEFAULT_FREQUENCY) || defined(__DOXYGEN__)
#define GPT_DEFAULT_FREQUENCY               1000000U
#endif

/**
 * @brief       Support for GPT user configurations.
 * @note        When enabled the user must provide a variable named @p
 *              gpt_configurations of type @p gpt_configurations_t.
 */
#if !defined(GPT_USE_CONFIGURATIONS) || defined(__DOXYGEN__)
#define GPT_USE_CONFIGURATIONS              FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on GPT_USE_CONFIGURATIONS configuration.*/
#if (GPT_USE_CONFIGURATIONS != FALSE) && (GPT_USE_CONFIGURATIONS != TRUE)
#error "invalid GPT_USE_CONFIGURATIONS value"
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Low level driver helper macros
 * @{
 */
/**
 * @brief       Common ISR code, GPT period event.
 *
 * @param[in,out] gptp          Pointer to the GPT driver instance.
 *
 * @notapi
 */
#define _gpt_isr_invoke_cb(gptp)                                            \
  do {                                                                      \
    if ((gptp)->state == GPT_ONESHOT) {                                     \
      gpt_lld_stop_timer(gptp);                                             \
      __cbdrv_invoke_complete_cb(gptp);                                     \
    }                                                                       \
    else {                                                                  \
      __cbdrv_invoke_cb(gptp);                                              \
    }                                                                       \
  } while (false)
/** @} */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       GPT frequency type.
 */
typedef uint32_t gptfreq_t;

/**
 * @brief       GPT counter type.
 */
typedef uint32_t gptcnt_t;

/**
 * @brief       Type of structure representing a GPT driver.
 */
typedef struct hal_gpt_driver hal_gpt_driver_c;

/**
 * @brief       Type of structure representing a GPT configuration.
 */
typedef struct hal_gpt_config hal_gpt_config_t;

/**
 * @brief       Type of user-provided GPT configurations.
 */
typedef struct gpt_configurations gpt_configurations_t;

/**
 * @brief       GPT driver specific states.
 */
typedef enum {
  GPT_CONTINUOUS = HAL_DRV_STATE_ERROR + 1U,
  GPT_ONESHOT
} gptstate_t;

/* Inclusion of LLD header.*/
#include "hal_gpt_lld.h"

/**
 * @brief       Driver configuration structure.
 */
struct hal_gpt_config {
  /**
   * @brief       Timer clock in Hz.
   */
  gptfreq_t                 frequency;
  /* End of the mandatory fields.*/
  gpt_lld_config_fields;
#if (defined(GPT_CONFIG_EXT_FIELDS)) || defined (__DOXYGEN__)
  GPT_CONFIG_EXT_FIELDS
#endif /* defined(GPT_CONFIG_EXT_FIELDS) */
};

/**
 * @brief       Structure representing user-provided GPT configurations.
 */
struct gpt_configurations {
  /**
   * @brief       Number of configurations in the open array.
   */
  unsigned                  cfgsnum;
  /**
   * @brief       User GPT configurations.
   */
  hal_gpt_config_t          cfgs[];
};

/**
 * @class       hal_gpt_driver_c
 * @extends     hal_cb_driver_c
 *
 * @brief       Class of a GPT driver.
 *
 * @name        Class @p hal_gpt_driver_c structures
 * @{
 */

/**
 * @brief       Type of a GPT driver class.
 */
typedef struct hal_gpt_driver hal_gpt_driver_c;

/**
 * @brief       Class @p hal_gpt_driver_c virtual methods table.
 */
struct hal_gpt_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip, const void *config);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_cb_driver_c.*/
  void (*oncbset)(void *ip, drv_cb_t cb);
  /* From hal_gpt_driver_c.*/
};

/**
 * @brief       Structure representing a GPT driver class.
 */
struct hal_gpt_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_gpt_driver_vmt *vmt;
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
#if (defined(GPT_DRIVER_EXT_FIELDS)) || defined (__DOXYGEN__)
  GPT_DRIVER_EXT_FIELDS
#endif /* defined(GPT_DRIVER_EXT_FIELDS) */
  /* End of the mandatory fields.*/
  gpt_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_gpt_driver_c.*/
  void *__gpt_objinit_impl(void *ip, const void *vmt);
  void __gpt_dispose_impl(void *ip);
  msg_t __gpt_start_impl(void *ip, const void *config);
  void __gpt_stop_impl(void *ip);
  const void *__gpt_setcfg_impl(void *ip, const void *config);
  const void *__gpt_selcfg_impl(void *ip, unsigned cfgnum);
  void __gpt_oncbset_impl(void *ip, drv_cb_t cb);
  void gptChangeIntervalI(void *ip, gptcnt_t interval);
  void gptChangeInterval(void *ip, gptcnt_t interval);
  void gptStartContinuousI(void *ip, gptcnt_t interval);
  void gptStartContinuous(void *ip, gptcnt_t interval);
  void gptStartOneShotI(void *ip, gptcnt_t interval);
  void gptStartOneShot(void *ip, gptcnt_t interval);
  void gptStopTimerI(void *ip);
  void gptStopTimer(void *ip);
  void gptPolledDelay(void *ip, gptcnt_t interval);
  /* Regular functions.*/
  void gptInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_gpt_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_gpt_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_gpt_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_gpt_driver_c *gptObjectInit(hal_gpt_driver_c *self) {
  extern const struct hal_gpt_driver_vmt __hal_gpt_driver_vmt;

  return __gpt_objinit_impl(self, &__hal_gpt_driver_vmt);
}
/** @} */

/**
 * @name        Inline methods of hal_gpt_driver_c
 * @{
 */
/**
 * @brief       Returns the interval of GPT peripheral.
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 * @return                      The current interval.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline gptcnt_t gptGetIntervalX(void *ip) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  return gpt_lld_get_interval(self);
}

/**
 * @brief       Returns the counter value of GPT peripheral.
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 * @return                      The current counter value.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline gptcnt_t gptGetCounterX(void *ip) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  return gpt_lld_get_counter(self);
}
/** @} */

#endif /* HAL_USE_GPT == TRUE */

#endif /* HAL_GPT_H */

/** @} */

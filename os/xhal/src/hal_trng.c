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
 * @file        hal_trng.c
 * @brief       Generated TRNG Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_TRNG
 * @{
 */

#include "hal.h"

#if (HAL_USE_TRNG == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module local macros.                                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       TRNG Driver initialization.
 *
 * @init
 */
void trngInit(void) {
  trng_lld_init();
}

/*===========================================================================*/
/* Module class "hal_trng_driver_c" methods.                                 */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_trng_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_trng_driver_c instance to
 *                              be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__trng_objinit_impl(void *ip, const void *vmt) {
  hal_trng_driver_c *self = (hal_trng_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __drv_objinit_impl(self, vmt);

  /* No initialization code.*/

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_trng_driver_c instance to
 *                              be disposed.
 */
void __trng_dispose_impl(void *ip) {
  hal_trng_driver_c *self = (hal_trng_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __drv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_trng_driver_c instance.
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 */
msg_t __trng_start_impl(void *ip, const void *config) {
  hal_trng_driver_c *self = (hal_trng_driver_c *)ip;
  if (config != NULL) {
    self->config = __trng_setcfg_impl(self, config);
  }
  else {
    self->config = __trng_selcfg_impl(self, 0U);
  }

  if (self->config == NULL) {
    return HAL_RET_CONFIG_ERROR;
  }

  return trng_lld_start(self);
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_trng_driver_c instance.
 */
void __trng_stop_impl(void *ip) {
  hal_trng_driver_c *self = (hal_trng_driver_c *)ip;
  trng_lld_stop(self);
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_trng_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__trng_setcfg_impl(void *ip, const void *config) {
  hal_trng_driver_c *self = (hal_trng_driver_c *)ip;
  return trng_lld_setcfg(self, (const hal_trng_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_trng_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__trng_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_trng_driver_c *self = (hal_trng_driver_c *)ip;
  return trng_lld_selcfg(self, cfgnum);
}
/** @} */

/**
 * @brief       VMT structure of TRNG driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_trng_driver_vmt __hal_trng_driver_vmt = {
  .dispose                  = __trng_dispose_impl,
  .start                    = __trng_start_impl,
  .stop                     = __trng_stop_impl,
  .setcfg                   = __trng_setcfg_impl,
  .selcfg                   = __trng_selcfg_impl
};

/**
 * @name        Regular methods of hal_trng_driver_c
 * @{
 */
/**
 * @brief       Fills a buffer with true random data.
 *
 * @param[in,out] ip            Pointer to a @p hal_trng_driver_c instance.
 * @param[in]     size          Size of output buffer.
 * @param[out]    out           Output buffer.
 * @return                      The operation status.
 * @retval HAL_RET_SUCCESS      Random data generated.
 * @retval HAL_RET_HW_FAILURE   TRNG hardware failure or timeout.
 *
 * @api
 */
msg_t trngGenerate(void *ip, size_t size, uint8_t *out) {
  hal_trng_driver_c *self = (hal_trng_driver_c *)ip;
  msg_t msg;

  osalDbgCheck((self != NULL) && (size > 0U) && (out != NULL));

  osalSysLock();
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");
  self->state = HAL_DRV_STATE_ACTIVE;
  osalSysUnlock();

  msg = trng_lld_generate(self, size, out);

  osalSysLock();
  self->state = HAL_DRV_STATE_READY;
  osalSysUnlock();

  return msg;
}
/** @} */

#endif /* HAL_USE_TRNG == TRUE */

/** @} */

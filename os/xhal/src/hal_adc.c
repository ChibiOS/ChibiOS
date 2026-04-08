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
 * @file        hal_adc.c
 * @brief       Generated ADC Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_ADC
 * @{
 */

#include "hal.h"

#if (HAL_USE_ADC == TRUE) || defined(__DOXYGEN__)

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
 * @brief       ADC Driver initialization.
 *
 * @init
 */
void adcInit(void) {
  adc_lld_init();
}

/*===========================================================================*/
/* Module class "hal_adc_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_adc_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_adc_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__adc_objinit_impl(void *ip, const void *vmt) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

  /* Initialization code.*/
  self->samples = NULL;
  self->depth   = 0U;
  self->grpp    = NULL;
  self->events  = 0U;
  self->errors  = 0U;
#if ADC_USE_WAIT == TRUE
  self->thread  = NULL;
#endif

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance to be
 *                              disposed.
 */
void __adc_dispose_impl(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @return                      The operation status.
 */
msg_t __adc_start_impl(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  self->samples = NULL;
  self->depth   = 0U;
  self->grpp    = NULL;
  self->events  = 0U;
  self->errors  = 0U;

  msg = adc_lld_start(self);
  if (msg != HAL_RET_SUCCESS) {
    self->config = NULL;
  }

  return msg;
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 */
void __adc_stop_impl(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  adc_lld_stop(self);
  self->samples = NULL;
  self->depth   = 0U;
  self->grpp    = NULL;
  self->events  = 0U;
  self->errors  = 0U;
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__adc_setcfg_impl(void *ip, const void *config) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  return (const void *)adc_lld_setcfg(self, (const hal_adc_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__adc_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  return (const void *)adc_lld_selcfg(self, cfgnum);
}

/**
 * @brief       Override of method @p drvSetCallbackX().
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param         cb            Callback function to be associated. Passing @p
 *                              NULL disables the existing callback, if any.
 */
void __adc_setcb_impl(void *ip, drv_cb_t cb) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  __cbdrv_setcb_impl(self, cb);
  adc_lld_set_callback(self, cb);
}
/** @} */

/**
 * @brief       VMT structure of ADC driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_adc_driver_vmt __hal_adc_driver_vmt = {
  .dispose                  = __adc_dispose_impl,
  .start                    = __adc_start_impl,
  .stop                     = __adc_stop_impl,
  .setcfg                   = __adc_setcfg_impl,
  .selcfg                   = __adc_selcfg_impl,
  .setcb                    = __adc_setcb_impl
};

/**
 * @name        Regular methods of hal_adc_driver_c
 * @{
 */
/**
 * @brief       Configures and activates the ADC peripheral.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 *
 * @api
 */
msg_t adcStart(void *ip, const hal_adc_config_t *config) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  if (config != NULL) {
    msg = drvSetCfgX(self, config);
    if (msg != HAL_RET_SUCCESS) {
      return msg;
    }
  }

  return drvStart(self);
}

/**
 * @brief       Deactivates the ADC peripheral.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 *
 * @api
 */
void adcStop(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  osalDbgCheck(self != NULL);

  drvStop(self);
}

/**
 * @brief       Starts an ADC conversion.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     grpp          Conversion group.
 * @param[out]    samples       Samples buffer.
 * @param[in]     depth         Buffer depth.
 * @return                      The operation status.
 *
 * @iclass
 */
msg_t adcStartConversionI(void *ip, const adc_conversion_group_t *grpp,
                          adcsample_t *samples, size_t depth) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (grpp != NULL) && (samples != NULL) &&
               (depth > 0U) && ((depth == 1U) || ((depth & 1U) == 0U)));
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == HAL_DRV_STATE_ERROR),
                "not ready");

  self->samples = samples;
  self->depth   = depth;
  self->grpp    = grpp;
  self->events  = 0U;
  self->errors  = 0U;
  self->state   = HAL_DRV_STATE_ACTIVE;
  msg = adc_lld_start_conversion(self, grpp, samples, depth);
  if (msg != HAL_RET_SUCCESS) {
    self->samples = NULL;
    self->depth   = 0U;
    self->grpp    = NULL;
    self->state   = HAL_DRV_STATE_READY;
  }

  return msg;
}

/**
 * @brief       Starts an ADC conversion.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     grpp          Conversion group.
 * @param[out]    samples       Samples buffer.
 * @param[in]     depth         Buffer depth.
 * @return                      The operation status.
 *
 * @api
 */
msg_t adcStartConversion(void *ip, const adc_conversion_group_t *grpp,
                         adcsample_t *samples, size_t depth) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = adcStartConversionI(self, grpp, samples, depth);
  osalSysUnlock();

  return msg;
}

/**
 * @brief       Stops an ongoing conversion.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 *
 * @iclass
 */
void adcStopConversionI(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == HAL_DRV_STATE_HALF) ||
                (self->state == HAL_DRV_STATE_FULL) ||
                (self->state == HAL_DRV_STATE_COMPLETE) ||
                (self->state == HAL_DRV_STATE_ERROR),
                "invalid state");

  if (self->state != HAL_DRV_STATE_READY) {
    adc_lld_stop_conversion(self);
    self->samples = NULL;
    self->depth   = 0U;
    self->grpp    = NULL;
    self->state   = HAL_DRV_STATE_READY;
    _adc_reset_i(self);
  }
}

/**
 * @brief       Stops an ongoing conversion.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 *
 * @api
 */
void adcStopConversion(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == HAL_DRV_STATE_HALF) ||
                (self->state == HAL_DRV_STATE_FULL) ||
                (self->state == HAL_DRV_STATE_COMPLETE) ||
                (self->state == HAL_DRV_STATE_ERROR),
                "invalid state");
  if (self->state != HAL_DRV_STATE_READY) {
    adc_lld_stop_conversion(self);
    self->samples = NULL;
    self->depth   = 0U;
    self->grpp    = NULL;
    self->state   = HAL_DRV_STATE_READY;
    _adc_reset_s(self);
  }
  osalSysUnlock();
}

#if (ADC_USE_WAIT == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Performs a synchronous ADC conversion.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     grpp          Conversion group.
 * @param[out]    samples       Samples buffer.
 * @param[in]     depth         Buffer depth.
 * @return                      The conversion status.
 *
 * @api
 */
msg_t adcConvert(void *ip, const adc_conversion_group_t *grpp,
                 adcsample_t *samples, size_t depth) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert(self->thread == NULL, "already waiting");
  msg = adcStartConversionI(self, grpp, samples, depth);
  if (msg == HAL_RET_SUCCESS) {
    msg = osalThreadSuspendS(&self->thread);
  }
  osalSysUnlock();

  return msg;
}
#endif /* ADC_USE_WAIT == TRUE */
/** @} */

#endif /* HAL_USE_ADC == TRUE */

/** @} */

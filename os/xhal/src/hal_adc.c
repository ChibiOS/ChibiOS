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
#if ADC_USE_SYNCHRONIZATION == TRUE
  self->thread     = NULL;
  self->sync_state = HAL_DRV_STATE_STOP;
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
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 */
msg_t __adc_start_impl(void *ip, const void *config) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  self->config = __adc_setcfg_impl(self, config);
  if (self->config == NULL) {
    return HAL_RET_CONFIG_ERROR;
  }

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
 * @brief       Override of method @p __cbdrv_on_cb_set().
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param         cb            Callback function to be associated. Passing @p
 *                              NULL disables the existing callback, if any.
 */
void __adc_oncbset_impl(void *ip, drv_cb_t cb) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
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
  .oncbset                  = __adc_oncbset_impl
};

/**
 * @name        Regular methods of hal_adc_driver_c
 * @{
 */
/**
 * @brief       Starts a linear ADC conversion.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     grpnum        Conversion group number.
 * @param[out]    samples       Samples buffer.
 * @param[in]     depth         Buffer depth.
 * @return                      The operation status.
 *
 * @iclass
 */
msg_t adcStartConversionLinearI(void *ip, unsigned grpnum,
                                adcsample_t *samples, size_t depth) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (samples != NULL) &&
               (depth > 0U) && ((depth == 1U) || ((depth & 1U) == 0U)));
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == HAL_DRV_STATE_ERROR),
                "not ready");

  self->samples = samples;
  self->depth   = depth;
  self->grpp    = NULL;
  self->events  = 0U;
  self->errors  = 0U;
  self->state   = ADC_ACTIVE_LINEAR;
  msg = adc_lld_start_conversion(self, grpnum, samples, depth);
  if (msg != HAL_RET_SUCCESS) {
    self->samples = NULL;
    self->depth   = 0U;
    self->grpp    = NULL;
    self->state   = HAL_DRV_STATE_READY;
  }

  return msg;
}

/**
 * @brief       Starts a linear ADC conversion.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     grpnum        Conversion group number.
 * @param[out]    samples       Samples buffer.
 * @param[in]     depth         Buffer depth.
 * @return                      The operation status.
 *
 * @api
 */
msg_t adcStartConversionLinear(void *ip, unsigned grpnum, adcsample_t *samples,
                               size_t depth) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = adcStartConversionLinearI(self, grpnum, samples, depth);
  osalSysUnlock();

  return msg;
}

/**
 * @brief       Starts a circular ADC conversion.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     grpnum        Conversion group number.
 * @param[out]    samples       Samples buffer.
 * @param[in]     depth         Buffer depth.
 * @return                      The operation status.
 *
 * @iclass
 */
msg_t adcStartConversionCircularI(void *ip, unsigned grpnum,
                                  adcsample_t *samples, size_t depth) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (samples != NULL) &&
               (depth > 0U) && ((depth == 1U) || ((depth & 1U) == 0U)));
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == HAL_DRV_STATE_ERROR),
                "not ready");

  self->samples = samples;
  self->depth   = depth;
  self->grpp    = NULL;
  self->events  = 0U;
  self->errors  = 0U;
  self->state   = ADC_ACTIVE_CIRCULAR;
  msg = adc_lld_start_conversion(self, grpnum, samples, depth);
  if (msg != HAL_RET_SUCCESS) {
    self->samples = NULL;
    self->depth   = 0U;
    self->grpp    = NULL;
    self->state   = HAL_DRV_STATE_READY;
  }

  return msg;
}

/**
 * @brief       Starts a circular ADC conversion.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     grpnum        Conversion group number.
 * @param[out]    samples       Samples buffer.
 * @param[in]     depth         Buffer depth.
 * @return                      The operation status.
 *
 * @api
 */
msg_t adcStartConversionCircular(void *ip, unsigned grpnum,
                                 adcsample_t *samples, size_t depth) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = adcStartConversionCircularI(self, grpnum, samples, depth);
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
                (self->state == ADC_ACTIVE_LINEAR) ||
                (self->state == ADC_ACTIVE_CIRCULAR) ||
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
                (self->state == ADC_ACTIVE_LINEAR) ||
                (self->state == ADC_ACTIVE_CIRCULAR) ||
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

#if (ADC_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Performs a synchronous ADC conversion.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     grpnum        Conversion group number.
 * @param[out]    samples       Samples buffer.
 * @param[in]     depth         Buffer depth.
 * @return                      The conversion status.
 *
 * @api
 */
msg_t adcConvert(void *ip, unsigned grpnum, adcsample_t *samples, size_t depth) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert(self->thread == NULL, "already waiting");
  msg = adcStartConversionLinearI(self, grpnum, samples, depth);
  if (msg == HAL_RET_SUCCESS) {
    msg = adcSynchronizeStateS(self, HAL_DRV_STATE_COMPLETE, TIME_INFINITE);
  }
  osalSysUnlock();

  return msg;
}

/**
 * @brief       Synchronizes with an ADC conversion state.
 * @details     This function synchronizes with a future occurrence of the
 *              specified conversion state. State occurrences are not buffered,
 *              the waiting thread must already be waiting when the state is
 *              reached.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     state         State to synchronize with.
 * @param[in]     timeout       Synchronization timeout.
 * @return                      The synchronization result.
 * @retval MSG_OK               If the requested state has been reached.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            If conversion stopped or failed while waiting.
 *
 * @sclass
 */
msg_t adcSynchronizeStateS(void *ip, driver_state_t state,
                           sysinterval_t timeout) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);
  osalDbgCheckClassS();
  osalDbgCheck((state == HAL_DRV_STATE_HALF) ||
               (state == HAL_DRV_STATE_FULL) ||
               (state == HAL_DRV_STATE_COMPLETE));
  osalDbgAssert((self->state == ADC_ACTIVE_LINEAR) ||
                (self->state == ADC_ACTIVE_CIRCULAR),
                "invalid state");
  osalDbgAssert(self->thread == NULL, "already waiting");

  self->sync_state = state;
  msg = osalThreadSuspendTimeoutS(&self->thread, timeout);
  self->sync_state = HAL_DRV_STATE_STOP;

  return msg;
}

/**
 * @brief       Synchronizes with an ADC conversion state.
 * @details     This function synchronizes with a future occurrence of the
 *              specified conversion state. State occurrences are not buffered,
 *              the waiting thread must already be waiting when the state is
 *              reached.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     state         State to synchronize with.
 * @param[in]     timeout       Synchronization timeout.
 * @return                      The synchronization result.
 * @retval MSG_OK               If the requested state has been reached.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            If conversion stopped or failed while waiting.
 *
 * @api
 */
msg_t adcSynchronizeState(void *ip, driver_state_t state,
                          sysinterval_t timeout) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  osalSysLock();
  msg = adcSynchronizeStateS(self, state, timeout);
  osalSysUnlock();

  return msg;
}
#endif /* ADC_USE_SYNCHRONIZATION == TRUE */
/** @} */

#endif /* HAL_USE_ADC == TRUE */

/** @} */

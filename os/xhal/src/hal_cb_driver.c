/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file        hal_cb_driver.c
 * @brief       Generated Callback Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_CB_DRIVER
 * @{
 */

#include "hal.h"

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

/*===========================================================================*/
/* Module class "hal_cb_driver_c" methods.                                   */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_cb_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_cb_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__cbdrv_objinit_impl(void *ip, const void *vmt) {
  hal_cb_driver_c *self = (hal_cb_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __drv_objinit_impl(self, vmt);

  /* Initialization code.*/
  self->cb = NULL;

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_cb_driver_c instance to be
 *                              disposed.
 */
void __cbdrv_dispose_impl(void *ip) {
  hal_cb_driver_c *self = (hal_cb_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __drv_dispose_impl(self);
}

/**
 * @brief       Implementation of method @p drvSetCallbackX().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_cb_driver_c instance.
 * @param         cb            Callback function to be associated. Passing @p
 *                              NULL disables the existing callback, if any.
 */
void __cbdrv_setcb_impl(void *ip, drv_cb_t cb) {
  hal_cb_driver_c *self = (hal_cb_driver_c *)ip;

  self->cb = cb;
}
/** @} */

/** @} */

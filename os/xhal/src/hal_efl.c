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
 * @file        hal_efl.c
 * @brief       Generated Embedded Flash Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_EFL
 * @{
 */

#include "hal.h"

#if (HAL_USE_EFL == TRUE) || defined(__DOXYGEN__)

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
 * @brief       Embedded Flash Driver initialization.
 *
 * @init
 */
void eflInit(void) {
  efl_lld_init();
}

/*===========================================================================*/
/* Module class "hal_efl_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_efl_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_efl_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__efl_objinit_impl(void *ip, const void *vmt) {
  hal_efl_driver_c *self = (hal_efl_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __flash_objinit_impl(self, vmt);

  /* No initialization code.*/

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_efl_driver_c instance to be
 *                              disposed.
 */
void __efl_dispose_impl(void *ip) {
  hal_efl_driver_c *self = (hal_efl_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __flash_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_efl_driver_c instance.
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 */
msg_t __efl_start_impl(void *ip, const void *config) {
  hal_efl_driver_c *self = (hal_efl_driver_c *)ip;
  if (config != NULL) {
    self->config = __efl_setcfg_impl(self, config);
  }
  else {
    self->config = __efl_selcfg_impl(self, 0U);
  }

  if (self->config == NULL) {
    return HAL_RET_CONFIG_ERROR;
  }

  return efl_lld_start(self);
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_efl_driver_c instance.
 */
void __efl_stop_impl(void *ip) {
  hal_efl_driver_c *self = (hal_efl_driver_c *)ip;
  efl_lld_stop(self);
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_efl_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__efl_setcfg_impl(void *ip, const void *config) {
  hal_efl_driver_c *self = (hal_efl_driver_c *)ip;
  static const hal_efl_config_t default_config = {0};

  (void)self;

  if (config == NULL) {
    return &default_config;
  }

  return config;
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_efl_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__efl_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_efl_driver_c *self = (hal_efl_driver_c *)ip;
  static const hal_efl_config_t default_config = {0};

  (void)self;

  if (cfgnum == 0U) {
    return &default_config;
  }

  return NULL;
}

/**
 * @brief       Override of method @p __flash_read().
 *
 * @param[in,out] ip            Pointer to a @p hal_efl_driver_c instance.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be read.
 * @param[out]    rp            Pointer to the data buffer.
 * @return                      An error code.
 */
flash_error_t __efl_read_impl(void *ip, flash_offset_t offset, size_t n,
                              uint8_t *rp) {
  hal_efl_driver_c *self = (hal_efl_driver_c *)ip;
  return efl_lld_read(self, offset, n, rp);
}

/**
 * @brief       Override of method @p __flash_program().
 *
 * @param[in,out] ip            Pointer to a @p hal_efl_driver_c instance.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be programmed.
 * @param[in]     pp            Pointer to the data buffer.
 * @return                      An error code.
 */
flash_error_t __efl_program_impl(void *ip, flash_offset_t offset, size_t n,
                                 const uint8_t *pp) {
  hal_efl_driver_c *self = (hal_efl_driver_c *)ip;
  return efl_lld_program(self, offset, n, pp);
}

/**
 * @brief       Override of method @p __flash_start_erase_all().
 *
 * @param[in,out] ip            Pointer to a @p hal_efl_driver_c instance.
 * @return                      An error code.
 */
flash_error_t __efl_start_erase_all_impl(void *ip) {
  hal_efl_driver_c *self = (hal_efl_driver_c *)ip;
  return efl_lld_start_erase_all(self);
}

/**
 * @brief       Override of method @p __flash_start_erase_sector().
 *
 * @param[in,out] ip            Pointer to a @p hal_efl_driver_c instance.
 * @param[in]     sector        Sector to be erased.
 * @return                      An error code.
 */
flash_error_t __efl_start_erase_sector_impl(void *ip, flash_sector_t sector) {
  hal_efl_driver_c *self = (hal_efl_driver_c *)ip;
  return efl_lld_start_erase_sector(self, sector);
}

/**
 * @brief       Override of method @p __flash_query_erase().
 *
 * @param[in,out] ip            Pointer to a @p hal_efl_driver_c instance.
 * @param[out]    msec          Recommended time, in milliseconds, before
 *                              retrying, can be @p NULL.
 * @return                      An error code.
 */
flash_error_t __efl_query_erase_impl(void *ip, unsigned *msec) {
  hal_efl_driver_c *self = (hal_efl_driver_c *)ip;
  return efl_lld_query_erase(self, msec);
}

/**
 * @brief       Override of method @p __flash_verify_erase().
 *
 * @param[in,out] ip            Pointer to a @p hal_efl_driver_c instance.
 * @param[in]     sector        Sector to be verified.
 * @return                      An error code.
 */
flash_error_t __efl_verify_erase_impl(void *ip, flash_sector_t sector) {
  hal_efl_driver_c *self = (hal_efl_driver_c *)ip;
  return efl_lld_verify_erase(self, sector);
}
/** @} */

/**
 * @brief       VMT structure of Embedded Flash driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_efl_driver_vmt __hal_efl_driver_vmt = {
  .dispose                  = __efl_dispose_impl,
  .start                    = __efl_start_impl,
  .stop                     = __efl_stop_impl,
  .setcfg                   = __efl_setcfg_impl,
  .selcfg                   = __efl_selcfg_impl,
  .read                     = __efl_read_impl,
  .program                  = __efl_program_impl,
  .start_erase_all          = __efl_start_erase_all_impl,
  .start_erase_sector       = __efl_start_erase_sector_impl,
  .query_erase              = __efl_query_erase_impl,
  .verify_erase             = __efl_verify_erase_impl
};

#endif /* HAL_USE_EFL == TRUE */

/** @} */

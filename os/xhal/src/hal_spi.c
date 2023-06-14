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
 * @file        hal_spi.c
 * @brief       Generated SPI Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_SPI
 * @{
 */

#include "hal.h"

#if (HAL_USE_SPI == TRUE) || defined(__DOXYGEN__)

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
 * @brief       SPI Driver initialization.
 * @note        This function is implicitly invoked by @p halInit(), there is
 *              no need to explicitly initialize the driver.
 *
 * @init
 */
void spiInit(void) {

  spi_lld_init();
}

/*===========================================================================*/
/* Module class "hal_spi_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_spi_driver_c
 * @{
 */
/**
 * @memberof    hal_spi_driver_c
 * @protected
 *
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_spi_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__spi_objinit_impl(void *ip, const void *vmt) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

  /* Initialization code.*/

  /* Optional, user-defined initializer.*/
#if defined(SPI_DRIVER_EXT_INIT_HOOK)
  SPI_DRIVER_EXT_INIT_HOOK(self);
#endif

  return self;
}

/**
 * @memberof    hal_spi_driver_c
 * @protected
 *
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance to be
 *                              disposed.
 */
void __spi_dispose_impl(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}
/** @} */

/**
 * @brief       VMT structure of SPI driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_spi_driver_vmt __hal_spi_driver_vmt = {
  .dispose                  = __spi_dispose_impl,
  .start                    = NULL /* Method not found.*/,
  .stop                     = NULL /* Method not found.*/,
  .configure                = NULL /* Method not found.*/
};

#endif /* HAL_USE_SPI == TRUE */

/** @} */

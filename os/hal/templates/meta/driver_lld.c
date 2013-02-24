/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    templates/xxx_lld.c
 * @brief   XXX Driver subsystem low level driver source template.
 *
 * @addtogroup XXX
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_XXX || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief XXX1 driver identifier.*/
#if PLATFORM_XXX_USE_XXX1 || defined(__DOXYGEN__)
XXXDriver XXXD1;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level XXX driver initialization.
 *
 * @notapi
 */
void xxx_lld_init(void) {

#if PLATFORM_XXX_USE_XXX1
  /* Driver initialization.*/
  xxxObjectInit(&XXXD1);
#endif /* PLATFORM_XXX_USE_XXX1 */
}

/**
 * @brief   Configures and activates the XXX peripheral.
 *
 * @param[in] xxxp      pointer to the @p XXXDriver object
 *
 * @notapi
 */
void xxx_lld_start(XXXDriver *xxxp) {

  if (xxxp->state == XXX_STOP) {
    /* Enables the pehipheral.*/
#if PLATFORM_XXX_USE_XXX1
    if (&XXXD1 == xxxp) {

    }
#endif /* PLATFORM_XXX_USE_XXX1 */
  }
  /* Configures the peripheral.*/

}

/**
 * @brief   Deactivates the XXX peripheral.
 *
 * @param[in] xxxp      pointer to the @p XXXDriver object
 *
 * @notapi
 */
void xxx_lld_stop(XXXDriver *xxxp) {

  if (xxxp->state == XXX_READY) {
    /* Resets the peripheral.*/

    /* Disables the peripheral.*/
#if PLATFORM_XXX_USE_XXX1
    if (&XXXD1 == xxxp) {

    }
#endif /* PLATFORM_XXX_USE_XXX1 */
  }
}

#endif /* HAL_USE_XXX */

/** @} */

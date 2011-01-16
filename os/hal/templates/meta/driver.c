/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    xxx.c
 * @brief   XXX Driver code.
 *
 * @addtogroup XXX
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_XXX || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   XXX Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void xxxInit(void) {

  xxx_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p XXXDriver structure.
 *
 * @param[out] xxxp     pointer to the @p XXXDriver object
 *
 * @init
 */
void xxxObjectInit(XXXDriver *xxxp) {

  xxxp->xxx_state    = XXX_STOP;
  xxxp->xxx_config   = NULL;
}

/**
 * @brief   Configures and activates the XXX peripheral.
 *
 * @param[in] xxxp      pointer to the @p XXXDriver object
 * @param[in] config    pointer to the @p XXXConfig object
 *
 * @api
 */
void xxxStart(XXXDriver *xxxp, const XXXConfig *config) {

  chDbgCheck((xxxp != NULL) && (config != NULL), "xxxStart");

  chSysLock();
  chDbgAssert((xxxp->xxx_state == XXX_STOP) || (xxxp->xxx_state == XXX_READY),
              "xxxStart(), #1",
              "invalid state");
  xxxp->xxx_config = config;
  xxx_lld_start(xxxp);
  xxxp->xxx_state = XXX_READY;
  chSysUnlock();
}

/**
 * @brief   Deactivates the XXX peripheral.
 *
 * @param[in] xxxp      pointer to the @p XXXDriver object
 *
 * @api
 */
void xxxStop(XXXDriver *xxxp) {

  chDbgCheck(xxxp != NULL, "xxxStop");

  chSysLock();
  chDbgAssert((xxxp->xxx_state == XXX_STOP) || (xxxp->xxx_state == XXX_READY),
              "xxxStop(), #1",
              "invalid state");
  xxx_lld_stop(xxxp);
  xxxp->xxx_state = XXX_STOP;
  chSysUnlock();
}

#endif /* HAL_USE_XXX */

/** @} */

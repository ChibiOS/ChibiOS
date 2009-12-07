/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @file XXX.c
 * @brief XXX Driver code.
 * @addtogroup XXX
 * @{
 */

#include "ch.h"
#include "hal.h"

/**
 * @brief XXX Driver initialization.
 */
void xxxInit(void) {

  xxx_lld_init();
}

/**
 * @brief Initializes the standard part of a @p XXXDriver structure.
 *
 * @param[in] xxxp      pointer to the @p XXXDriver object
 */
void xxxObjectInit(XXXDriver *xxxp) {

  xxxp->xxx_state    = XXX_STOP;
  xxxp->xxx_config   = NULL;
}

/**
 * @brief Configures and activates the XXX peripheral.
 *
 * @param[in] xxxp      pointer to the @p XXXDriver object
 * @param[in] config    pointer to the @p XXXConfig object
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
 * @brief Deactivates the XXX peripheral.
 *
 * @param[in] xxxp      pointer to the @p XXXDriver object
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

/** @} */

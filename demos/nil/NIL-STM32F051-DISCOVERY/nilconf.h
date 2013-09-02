/*
    Nil RTOS - Copyright (C) 2012 Giovanni Di Sirio.

    This file is part of Nil RTOS.

    Nil RTOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Nil RTOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    nilconf.h
 * @brief   Configuration file template.
 * @details A copy of this file must be placed in each project directory, it
 *          contains the application specific kernel settings.
 *
 * @addtogroup config
 * @details Kernel related settings and hooks.
 * @{
 */

#ifndef _NILCONF_H_
#define _NILCONF_H_

/**
 * @brief   Number of user threads in the application.
 * @note    This number is not inclusive of the idle thread which is
 *          Implicitly handled.
 */
#define NIL_CFG_NUM_THREADS                 2

/**
 * @brief   System tick frequency.
 */
#define NIL_CFG_FREQUENCY                   50000

/**
 * @brief   Time delta constant for the tick-less mode.
 * @note    If this value is zero then the system uses the classic
 *          periodic tick. This value represents the minimum number
 *          of ticks that is safe to specify in a timeout directive.
 *          The value one is not valid, timeouts are rounded up to
 *          this value.
 */
#define NIL_CFG_TIMEDELTA                   2

/**
 * @brief   System assertions.
 */
#define NIL_CFG_ENABLE_ASSERTS              FALSE

/**
 * @brief   Threads descriptor structure extension.
 * @details User fields added to the end of the @p thread_t structure.
 */
#define NIL_CFG_THREAD_EXT_FIELDS                                           \
  /* Add threads custom fields here.*/

/**
 * @brief   Threads initialization hook.
 */
#define NIL_CFG_THREAD_EXT_INIT_HOOK(tr) {                                  \
  /* Add custom threads initialization code here.*/                         \
}

/**
 * @brief   Idle thread enter hook.
 * @note    This hook is invoked within a critical zone, no OS functions
 *          should be invoked from here.
 * @note    This macro can be used to activate a power saving mode.
 */
#define NIL_CFG_IDLE_ENTER_HOOK() {                                         \
}

/**
 * @brief   Idle thread leave hook.
 * @note    This hook is invoked within a critical zone, no OS functions
 *          should be invoked from here.
 * @note    This macro can be used to deactivate a power saving mode.
 */
#define NIL_CFG_IDLE_LEAVE_HOOK() {                                         \
}

/*===========================================================================*/
/* Port-specific settings (override port settings defaulted in nilcore.h).   */
/*===========================================================================*/

#endif  /* _NILCONF_H_ */

/** @} */

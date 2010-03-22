/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    ARMCM3/STM32F103/cmparams.h
 * @brief   ARM Cortex-M3 STM32 specific parameters.
 *
 * @addtogroup ARMCM3_STM32F103
 * @{
 */

#ifndef _CMPARAMS_H_
#define _CMPARAMS_H_

/*===========================================================================*/
/* Constants parameters.                                                     */
/*===========================================================================*/

/**
 * @brief   Cortex core model.
 */
#define CORTEX_MODEL            CORTEX_M3

/**
 * @brief   Systick unit presence.
 */
#define CORTEX_HAS_ST           TRUE

/**
 * @brief   Memory Protection unit presence.
 */
#define CORTEX_HAS_MPU          FALSE

/**
 * @brief   Number of bits in priority masks.
 * @details The available number of priority levels is equal to
 *          (1 << @p CORTEX_PRIORITY_BITS).
 */
#define CORTEX_PRIORITY_BITS    4

/**
 * @brief   Priority to priority mask conversion macro.
 */
#define CORTEX_PRIORITY(n)      ((n) << (8 - CORTEX_PRIORITY_BITS))

/*===========================================================================*/
/* Configurable parameters.                                                  */
/*===========================================================================*/

/**
 * @brief   BASEPRI user level, 0 = disabled.
 */
#ifndef CORTEX_BASEPRI_USER
#define CORTEX_BASEPRI_USER     CORTEX_PRIORITY(0)
#endif

/**
 * @brief   BASEPRI level within kernel lock.
 * @details Priority levels higher than this one (lower values) are unaffected
 *          by the OS activity and can be classified as fast interrupt sources,
 *          see @ref interrupt_classes.
 */
#ifndef CORTEX_BASEPRI_KERNEL
#define CORTEX_BASEPRI_KERNEL   CORTEX_PRIORITY(4)
#endif

/**
 * @brief   SVCALL handler priority.
 * @note    This priority must always be one level above the @p BASEPRI_KERNEL
 *          value.
 * @note    It is recommended, but not mandatory, to leave this priority level
 *          for this handler alone.
 */
#ifndef CORTEX_PRIORITY_SVCALL
#define CORTEX_PRIORITY_SVCALL  CORTEX_PRIORITY(3)
#endif

/**
 * @brief   SYSTICK handler priority.
 */
#ifndef CORTEX_PRIORITY_SYSTICK
#define CORTEX_PRIORITY_SYSTICK CORTEX_PRIORITY(8)
#endif

/**
 * @brief   PENDSV handler priority.
 * @note    It is recommended to leave this priority level for this handler
 *          alone.
 * @note    This is a reserved handler and its priority must always be the
 *          lowest priority in the system in order to be always executed last
 *          in the interrupt servicing chain.
 */
#ifndef CORTEX_PRIORITY_PENDSV
#define CORTEX_PRIORITY_PENDSV  CORTEX_PRIORITY(15)
#endif

#endif /* _CMPARAMS_H_ */

/** @} */

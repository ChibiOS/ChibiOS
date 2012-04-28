/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    MSP430/hal_lld.h
 * @brief   MSP430 HAL subsystem low level driver header.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "msp430.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Defines the support for realtime counters in the HAL.
 */
#define HAL_IMPLEMENTS_COUNTERS FALSE

/**
 * @brief   Platform name.
 */
#define PLATFORM_NAME   "MSP430"

#define MSP430_CLOCK_SOURCE_XT2CLK  0   /**< @brief XT2CLK clock selector.  */
#define MSP430_CLOCK_SOURCE_DCOCLK  1   /**< @brief DCOCLK clock selector.  */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Clock source.
 * @details The clock source can be selected from:
 *          - @p MSP430_CLOCK_SOURCE_XT2CLK.
 *          - @p MSP430_CLOCK_SOURCE_DCOCLK.
 *          .
 */
#if !defined(MSP430_USE_CLOCK) || defined(__DOXYGEN__)
#define MSP430_USE_CLOCK            MSP430_CLOCK_SOURCE_XT2CLK
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*
 * Calculating the derived clock constants.
 */
#define ACLK                        LFXT1CLK
#if MSP430_USE_CLOCK == MSP430_CLOCK_SOURCE_XT2CLK
#define MCLK                        XT2CLK
#define SMCLK                       (XT2CLK / 8)
#elif MSP430_USE_CLOCK == MSP430_CLOCK_SOURCE_DCOCLK
#define MCLK                        DCOCLK
#define SMCLK                       DCOCLK
#else
#error "unknown clock source specified"
#endif

/*
 * Calculating the initialization values.
 */
#define VAL_DCOCTL                  (DCO0 | DCO1)
#if MSP430_USE_CLOCK == MSP430_CLOCK_SOURCE_XT2CLK
#define VAL_BCSCTL1                 (RSEL2)
#define VAL_BCSCTL2                 (SELM_2 | DIVM_0 | DIVS_3 | SELS)
#endif
#if MSP430_USE_CLOCK == MSP430_CLOCK_SOURCE_DCOCLK
#define VAL_BCSCTL1                 (XT2OFF | RSEL2)
#define VAL_BCSCTL2                 (SELM_0 | DIVM_0 | DIVS_0)
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */

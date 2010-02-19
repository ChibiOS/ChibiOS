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
 * @file    SPC563/hal_lld.h
 * @brief   SPC563 HAL subsystem low level driver header.
 *
 * @addtogroup SPC563_HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "mpc563m.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define RFD_DIV2                    0       /**< Divide VCO frequency by 2. */
#define RFD_DIV4                    1       /**< Divide VCO frequency by 4. */
#define RFD_DIV8                    2       /**< Divide VCO frequency by 8. */
#define RFD_DIV16                   3       /**< Divide VCO frequency by 16.*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Clock bypass.
 * @note    If set to @p TRUE then the PLL is not started and initialized, the
 *          external clock is used as-is and the other clock-related settings
 *          are ignored.
 */
#if !defined(SPC563_CLK_BYPASS) || defined(__DOXYGEN__)
#define SPC563_CLK_BYPASS           FALSE
#endif

/**
 * @brief   Disables the overclock checks.
 */
#if !defined(SPC563_ALLOW_OVERCLOCK) || defined(__DOXYGEN__)
#define SPC563_ALLOW_OVERCLOCK      FALSE
#endif

/**
 * @brief   External clock pre-divider.
 * @note    Must be in range 0...14.
 * @note    The effective divider factor is this value plus one.
 */
#if !defined(SPC563_CLK_PREDIV) || defined(__DOXYGEN__)
#define SPC563_CLK_PREDIV           0
#endif

/**
 * @brief   Multiplication factor divider.
 * @note    Must be in range 32...96.
 */
#if !defined(SPC563_CLK_MFD) || defined(__DOXYGEN__)
#define SPC563_CLK_MFD              40
#endif

/**
 * @brief   Reduced frequency divider.
 */
#if !defined(SPC563_CLK_RFD) || defined(__DOXYGEN__)
#define SPC563_CLK_RFD              RFD_DIV4
#endif

/**
 * @brief   Flash buffer and prefetching settings.
 * @note    Please refer to the SPC563M64 reference manual about the meaning
 *          of the following bits, if in doubt DO NOT MODIFY IT.
 * @note    Do not specify the APC, WWSC, RWSC bits in this value because
 *          those are calculated from the system clock and ORed with this
 *          value.
 */
#if !defined(SPC563_FLASH_BIUCR) || defined(__DOXYGEN__)
#define SPC563_FLASH_BIUCR          (0x01110053 & 0xFFFF00FF)
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (SPC563_CLK_PREDIV < 0) || (SPC563_CLK_PREDIV > 14)
#error "invalid SPC563_CLK_PREDIV value specified"
#endif

#if (SPC563_CLK_MFD < 32) || (SPC563_CLK_MFD > 96)
#error "invalid SPC563_CLK_MFD value specified"
#endif

#if (SPC563_CLK_RFD != RFD_DIV2) && (SPC563_CLK_RFD != RFD_DIV4) &&         \
    (SPC563_CLK_RFD != RFD_DIV8) && (SPC563_CLK_RFD != RFD_DIV16)
#error "invalid SPC563_CLK_RFD value specified"
#endif

/**
 * @brief   PLL output clock.
 */
#define SPC563_PLLCLK   ((EXTCLK / (SPC563_CLK_PREDIV + 1)) * SPC563_CLK_MFD)

#if (SPC563_PLLCLK < 256000000) || (SPC563_PLLCLK > 512000000)
#error "VCO frequency out of the acceptable range (256...512)"
#endif

/**
 * @brief   PLL output clock.
 */
#if !SPC563_CLK_BYPASS || defined(__DOXYGEN__)
#define SPC563_SYSCLK   (SPC563_PLLCLK / (1 << (SPC563_CLK_RFD + 1)))
#else
#define SPC563_SYSCLK   EXTCLK
#endif

#if (SPC563_SYSCLK > 80000000) && !SPC563_ALLOW_OVERCLOCK
#error "System clock above maximum rated frequency (80MHz)"
#endif

/**
 * @brief   Flash wait states are a function of the system clock.
 */
#if (SPC563_SYSCLK <= 30000000) || defined(__DOXYGEN__)
#define SPC563_FLASH_WS     0x00000800      /* APC=0, RWSC=0, WWSC=1.       */
#elif SPC563_SYSCLK <= 60000000
#define SPC563_FLASH_WS     0x00002900      /* APC=1, RWSC=1, WWSC=1.       */
#else
#define SPC563_FLASH_WS     0x00004A00      /* APC=2, RWSC=2, WWSC=1.       */
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
  void spc563_clock_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */

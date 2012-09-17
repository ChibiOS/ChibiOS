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
*/

/**
 * @file    SPC560Pxx/hal_lld.h
 * @brief   SPC560Pxx HAL subsystem low level driver header.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - SPC560P_XOSC_CLK.
 *          .
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "xpc560p.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Defines the support for realtime counters in the HAL.
 */
#define HAL_IMPLEMENTS_COUNTERS FALSE

/**
 * @name    Platform identification
 * @{
 */
#define PLATFORM_NAME           "SPC560Pxx Chassis and Safety"
/** @} */

/**
 * @name    Absolute Maximum Ratings
 * @{
 */
/**
 * @brief   Maximum XOSC clock frequency.
 */
#define SPC560P_XOSC_CLK_MAX        40000000

/**
 * @brief   Minimum XOSC clock frequency.
 */
#define SPC560P_XOSC_CLK_MIN        4000000

/**
 * @brief   Maximum FMPLLs input clock frequency.
 */
#define SPC560P_FMPLLIN_MIN         4000000

/**
 * @brief   Maximum FMPLLs input clock frequency.
 */
#define SPC560P_FMPLLIN_MAX         16000000

/**
 * @brief   Maximum FMPLLs VCO clock frequency.
 */
#define SPC560P_FMPLLVCO_MAX        512000000

/**
 * @brief   Maximum FMPLLs VCO clock frequency.
 */
#define SPC560P_FMPLLVCO_MIN        256000000

/**
 * @brief   Maximum FMPLL0 output clock frequency.
 */
#define SPC560P_FMPLL0_CLK_MAX      64000000

/**
 * @brief   Maximum FMPLL1 output clock frequency.
 */
#define SPC560P_FMPLL1_CLK_MAX      120000000

/**
 * @brief   Maximum FMPLL1 1D1 output clock frequency.
 */
#define SPC560P_FMPLL1_1D1_CLK_MAX  80000000
/** @} */

/**
 * @name    Internal clock sources
 * @{
 */
#define SPC560P_IRC_CLK             16000000    /**< Internal RC oscillator.*/
/** @} */

/**
 * @name    FMPLL_CR register bits definitions
 * @{
 */
#define SPC560P_FMPLL_ODF_DIV2      (0 << 24)
#define SPC560P_FMPLL_ODF_DIV4      (1 << 24)
#define SPC560P_FMPLL_ODF_DIV8      (2 << 24)
#define SPC560P_FMPLL_ODF_DIV16     (3 << 24)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Clock bypass.
 * @note    If set to @p TRUE then the PLL is not started and initialized, the
 *          external clock is used as-is and the other clock-related settings
 *          are ignored.
 */
#if !defined(SPC560P_CLK_BYPASS) || defined(__DOXYGEN__)
#define SPC560P_CLK_BYPASS          FALSE
#endif

/**
 * @brief   Disables the overclock checks.
 */
#if !defined(SPC560P_ALLOW_OVERCLOCK) || defined(__DOXYGEN__)
#define SPC560P_ALLOW_OVERCLOCK     FALSE
#endif

/**
 * @brief   FMPLL0 IDF divider value.
 * @note    The default value is calculated for XOSC=40MHz and PHI=64MHz.
 */
#if !defined(SPC560P_FMPLL0_IDF_VALUE) || defined(__DOXYGEN__)
#define SPC560P_FMPLL0_IDF_VALUE    5
#endif

/**
 * @brief   FMPLL0 NDIV divider value.
 * @note    The default value is calculated for XOSC=40MHz and PHI=64MHz.
 */
#if !defined(SPC560P_FMPLL0_NDIV_VALUE) || defined(__DOXYGEN__)
#define SPC560P_FMPLL0_NDIV_VALUE   32
#endif

/**
 * @brief   FMPLL0 ODF divider value.
 * @note    The default value is calculated for XOSC=40MHz and PHI=64MHz.
 */
#if !defined(SPC560P_FMPLL0_ODF) || defined(__DOXYGEN__)
#define SPC560P_FMPLL0_ODF          SPC560P_FMPLL_ODF_DIV4
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Check on the XOSC frequency.*/
#if (SPC560P_XOSC_CLK < SPC560P_XOSC_CLK_MIN) ||                            \
    (SPC560P_XOSC_CLK > SPC560P_XOSC_CLK_MAX)
#error "invalid SPC560P_XOSC_CLK value specified"
#endif

/* Check on SPC560P_FMPLL0_IDF_VALUE.*/
#if (SPC560P_FMPLL0_IDF_VALUE < 1) || (SPC560P_FMPLL0_IDF_VALUE > 15)
#error "invalid SPC560P_FMPLL0_IDF_VALUE value specified"
#endif

/* Check on SPC560P_FMPLL0_NDIV_VALUE.*/
#if (SPC560P_FMPLL0_NDIV_VALUE < 32) || (SPC560P_FMPLL0_NDIV_VALUE > 96)
#error "invalid SPC560P_FMPLL0_NDIV_VALUE value specified"
#endif

/* Check on SPC560P_FMPLL0_ODF.*/
#if (SPC560P_FMPLL0_ODF == SPC560P_FMPLL_ODF_DIV2)
#define SPC560P_FMPLL0_ODF_VALUE    2
#elif (SPC560P_FMPLL0_ODF == SPC560P_FMPLL_ODF_DIV4)
#define SPC560P_FMPLL0_ODF_VALUE    4
#elif (SPC560P_FMPLL0_ODF == SPC560P_FMPLL_ODF_DIV8)
#define SPC560P_FMPLL0_ODF_VALUE    8
#elif (SPC560P_FMPLL0_ODF == SPC560P_FMPLL_ODF_DIV16)
#define SPC560P_FMPLL0_ODF_VALUE    16
#else
#error "invalid SPC560P_FMPLL0_ODF value specified"
#endif


/**
 * @brief   SPC560P_FMPLL0_VCO_CLK clock point.
 */
#define SPC560P_FMPLL0_VCO_CLK                                              \
  ((SPC560P_XOSC_CLK / SPC560P_FMPLL0_IDF_VALUE) * SPC560P_FMPLL0_NDIV_VALUE)

/* Check on FMPLL0 VCO output.*/
#if (SPC560P_FMPLL0_VCO_CLK < SPC560P_FMPLLVCO_MIN) ||                      \
    (SPC560P_FMPLL0_VCO_CLK > SPC560P_FMPLLVCO_MAX)
#error "SPC560P_FMPLL0_CLK outside acceptable range (SPC560P_FMPLLVCO_MIN...SPC560P_FMPLLVCO_MAX)"
#endif

/**
 * @brief   SPC560P_XOSC_CLK clock point.
 */
#define SPC560P_FMPLL0_CLK                                                  \
  (SPC560P_FMPLL0_VCO_CLK / SPC560P_FMPLL0_ODF_VALUE)

/* Check on SPC560P_FMPLL0_CLK.*/
#if SPC560P_FMPLL0_CLK > SPC560P_FMPLL0_CLK_MAX
#error "SPC560P_FMPLL0_CLK outside acceptable range (0...SPC560P_FMPLL0_CLK_MAX)"
#endif

/* FMPLL0 activation conditions.*/
#if 1 || defined(__DOXYGEN__)
/**
 * @brief   FMPLL0 activation flag.
 */
#define SPC560P_ACTIVATE_FMPLL0          TRUE
#else
#define SPC560P_ACTIVATE_FMPLL0          FALSE
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
  void spc560p_clock_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */

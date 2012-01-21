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
 * @file    SPC56x/hal_lld.h
 * @brief   SPC563 HAL subsystem low level driver header.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "mpc563m.h"

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
#define PLATFORM_NAME           "SPC563M64"

#define RFD_DIV2                0           /**< Divide VCO frequency by 2. */
#define RFD_DIV4                1           /**< Divide VCO frequency by 4. */
#define RFD_DIV8                2           /**< Divide VCO frequency by 8. */
#define RFD_DIV16               3           /**< Divide VCO frequency by 16.*/

/* The following settings are related to the FLASH controller, performance
   and stability depends on them, be careful.*/
#define BIUCR_BANK1_TOO         0x01000000  /**< Use settings for bank1 too.*/
#define BIUCR_MASTER7_PREFETCH  0x00800000  /**< Enable master 7 prefetch.  */
#define BIUCR_MASTER6_PREFETCH  0x00400000  /**< Enable master 6 prefetch.  */
#define BIUCR_MASTER5_PREFETCH  0x00200000  /**< Enable master 5 prefetch.  */
#define BIUCR_MASTER4_PREFETCH  0x00100000  /**< Enable master 4 prefetch.  */
#define BIUCR_MASTER3_PREFETCH  0x00080000  /**< Enable master 3 prefetch.  */
#define BIUCR_MASTER2_PREFETCH  0x00040000  /**< Enable master 2 prefetch.  */
#define BIUCR_MASTER1_PREFETCH  0x00020000  /**< Enable master 1 prefetch.  */
#define BIUCR_MASTER0_PREFETCH  0x00010000  /**< Enable master 0 prefetch.  */
#define BIUCR_APC_MASK          0x0000E000  /**< APC field mask.            */
#define BIUCR_APC_0             (0 << 13)   /**< No additional hold cycles. */
#define BIUCR_APC_1             (1 << 13)   /**< 1 additional hold cycle.   */
#define BIUCR_APC_2             (2 << 13)   /**< 2 additional hold cycles.  */
#define BIUCR_APC_3             (3 << 13)   /**< 3 additional hold cycles.  */
#define BIUCR_APC_4             (4 << 13)   /**< 4 additional hold cycles.  */
#define BIUCR_APC_5             (5 << 13)   /**< 5 additional hold cycles.  */
#define BIUCR_APC_6             (6 << 13)   /**< 6 additional hold cycles.  */
#define BIUCR_WWSC_MASK         0x00001800  /**< WWSC field mask.           */
#define BIUCR_WWSC_0            (0 << 11)   /**< No write wait states.      */
#define BIUCR_WWSC_1            (1 << 11)   /**< 1 write wait state.        */
#define BIUCR_WWSC_2            (2 << 11)   /**< 2 write wait states.       */
#define BIUCR_WWSC_3            (3 << 11)   /**< 3 write wait states.       */
#define BIUCR_RWSC_MASK         0x00001800  /**< RWSC field mask.           */
#define BIUCR_RWSC_0            (0 << 8)    /**< No read wait states.       */
#define BIUCR_RWSC_1            (1 << 8)    /**< 1 read wait state.         */
#define BIUCR_RWSC_2            (2 << 8)    /**< 2 read wait states.        */
#define BIUCR_RWSC_3            (3 << 8)    /**< 3 read wait states.        */
#define BIUCR_RWSC_4            (4 << 8)    /**< 4 read wait states.        */
#define BIUCR_RWSC_5            (5 << 8)    /**< 5 read wait states.        */
#define BIUCR_RWSC_6            (6 << 8)    /**< 6 read wait states.        */
#define BIUCR_RWSC_7            (7 << 8)    /**< 7 read wait states.        */
#define BIUCR_DPFEN             0x00000040  /**< Data prefetch enable.      */
#define BIUCR_IPFEN             0x00000010  /**< Instr. prefetch enable.    */
#define BIUCR_PFLIM_MASK        0x00000060  /**< PFLIM field mask.          */
#define BIUCR_PFLIM_NO          (0 << 1)    /**< No prefetching.            */
#define BIUCR_PFLIM_ON_MISS     (1 << 1)    /**< Prefetch on miss.          */
#define BIUCR_PFLIM_ON_HITMISS  (2 << 1)    /**< Prefetch on hit and miss.  */
#define BIUCR_BFEN              0x00000001  /**< Flash buffering enable.    */

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
#define SPC563_CLK_PREDIV           1
#endif

/**
 * @brief   Multiplication factor divider.
 * @note    Must be in range 32...96.
 */
#if !defined(SPC563_CLK_MFD) || defined(__DOXYGEN__)
#define SPC563_CLK_MFD              80
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
#define SPC563_FLASH_BIUCR          (BIUCR_BANK1_TOO |                      \
                                     BIUCR_MASTER4_PREFETCH |               \
                                     BIUCR_MASTER0_PREFETCH |               \
                                     BIUCR_DPFEN |                          \
                                     BIUCR_IPFEN |                          \
                                     BIUCR_PFLIM_ON_MISS |                  \
                                     BIUCR_BFEN)
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
#define SPC563_FLASH_WS     (BIUCR_APC_0 | BIUCR_RWSC_0 | BIUCR_WWSC_1)
#elif SPC563_SYSCLK <= 60000000
#define SPC563_FLASH_WS     (BIUCR_APC_1 | BIUCR_RWSC_1 | BIUCR_WWSC_1)
#else
#define SPC563_FLASH_WS     (BIUCR_APC_2 | BIUCR_RWSC_2 | BIUCR_WWSC_1)
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

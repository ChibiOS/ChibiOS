/*
    ChibiOS - Copyright (C) 2019 Rocco Marco Guglielmi

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
 * @file    ADUCM41x/hal_lld.h
 * @brief   ADUCM41x HAL subsystem low level driver header.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - ADUCM_LFXTAL.
 *          .
 *          One of the following macros must also be defined:
 *          - ADUCM410.
 *          .
 *
 * @addtogroup HAL
 * @{
 */

#ifndef HAL_LLD_H
#define HAL_LLD_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Platform identification macros
 * @{
 */
#if defined(ADUCM410) || defined(__DOXYGEN__)
#define PLATFORM_NAME           "ADUCM410 Precision Analog MCU with 16-bit MDIO interface"

#else
#error "ADUCM41x device unsupported or not specified"
#endif
/** @} */

/**
 * @name    Absolute Maximum Ratings
 * @{
 */

#if defined(ADUCM410) || defined(__DOXYGEN__)
/**
 * @brief   Absolute maximum external clock.
 */
#define ADUCM_EXTCLK_MAX        32000000

/**
 * @brief   Absolute minimum external clock.
 */
#define ADUCM_EXTCLK_MIN        16000000

/**
 * @brief   Absolute maximum external clock.
 */
#define ADUCM_AFE_CLK_MAX       32000000

/**
 * @brief   Absolute minimum external clock.
 */
#define ADUCM_AFE_CLK_MIN       32000000
#endif
/** @} */

/**
 * @name    Internal clock sources
 * @{
 */
#define ADUCM_HFOSC             16000000    /**< High speed internal clock. */
#define ADUCM_SPLL              160000000   /**< System PLL clock.          */
#define ADUCM_LFOSC             32768       /**< Low speed internal clock.  */
/** @} */

/**
 * @name    CLKCON0 register bits definitions
 * @{
 */
#define ADUCM_CLKMUX_HFOSC      (0 << 0)    /**< Clock source is HFOSC.     */
#define ADUCM_CLKMUX_SPLL       (1 << 0)    /**< Clock source is SPLL.      */
#define ADUCM_CLKMUX_EXTCLK     (3 << 0)    /**< Clock source is EXTCLK.    */

#define ADUCM_CLKOUT_HFOSC      (0 << 2)    /**< Clock out 16 MHz.          */
#define ADUCM_CLKOUT_ROOTCLK    (1 << 2)    /**< Clock out Root Clock.      */
#define ADUCM_CLKOUT_LFOSC      (3 << 2)    /**< Clock out 32 KHz.          */
#define ADUCM_CLKOUT_CORECLK    (4 << 2)    /**< Clock out Core Clock.      */
#define ADUCM_CLKOUT_PCLK0      (5 << 2)    /**< Clock out Periph. Clock 0. */
#define ADUCM_CLKOUT_PCLK1      (6 << 2)    /**< Clock out Periph. Clock 1. */
#define ADUCM_CLKOUT_PCLK2      (7 << 2)    /**< Clock out Periph. Clock 2. */
#define ADUCM_CLKOUT_ATS        (8 << 2)    /**< Clock out Analog Test Sign.*/
#define ADUCM_CLKOUT_T0         (9 << 2)    /**< Clock out Timer 0.         */
#define ADUCM_CLKOUT_WUT        (10 << 2)   /**< Clock out Wake-up Timer.   */
#define ADUCM_CLKOUT_T3         (11 << 2)   /**< Clock out Timer 3.         */
#define ADUCM_CLKOUT_HCLK       (12 << 2)   /**< Clock out HCLK.            */
#define ADUCM_CLKOUT_SPLL       (13 << 2)   /**< Clock out System PLL.      */
#define ADUCM_CLKOUT_DISABLED   (15 << 2)   /**< Clock out Disabled.        */

#define ADUCM_AMUX_HFOSC        (0 << 7)    /**< AMux is 16 MHz.            */
#define ADUCM_AMUX_EXTCLK       (2 << 7)    /**< AMux is EXTCLK.            */

#define ADUCM_SPLLIE_ENABLED    (1 << 9)    /**< PLL Lock/Unlock IRQ en.    */
#define ADUCM_SPLLIE_DISABLED   (0 << 9)    /**< PLL Lock/Unlock IRQ dis.   */

#define ADUCM_ANAROOT_AMUX      (0 << 10)   /**< Analog clock is AMux.      */
#define ADUCM_ANAROOT_AMUX_X2   (1 << 10)   /**< Analog clock is AMux x 2.  */
#define ADUCM_ANAROOT_SPLL      (2 << 10)   /**< Analog clock is System PLL.*/
#define ADUCM_ANAROOT_EXTCLK    (3 << 10)   /**< Analog clock is EXTCLK.    */
/** @} */

/**
 * @name    CLKCON1 register bits definitions
 * @{
 */
#define ADUCM_CDHCLK_DIV1       (0 << 0)    /**< HCLK divided by 1.         */
#define ADUCM_CDHCLK_DIV2       (1 << 0)    /**< HCLK divided by 2.         */
#define ADUCM_CDHCLK_DIV4       (2 << 0)    /**< HCLK divided by 4.         */
#define ADUCM_CDHCLK_DIV8       (3 << 0)    /**< HCLK divided by 8.         */
#define ADUCM_CDHCLK_DIV16      (4 << 0)    /**< HCLK divided by 16.        */
#define ADUCM_CDHCLK_DIV32      (5 << 0)    /**< HCLK divided by 32.        */
#define ADUCM_CDHCLK_DIV64      (6 << 0)    /**< HCLK divided by 64.        */
#define ADUCM_CDHCLK_DIV128     (7 << 0)    /**< HCLK divided by 128.       */

#define ADUCM_CDPCLK0_DIV1      (0 << 3)    /**< PCLK0 divided by 1.        */
#define ADUCM_CDPCLK0_DIV2      (1 << 3)    /**< PCLK0 divided by 2.        */
#define ADUCM_CDPCLK0_DIV4      (2 << 3)    /**< PCLK0 divided by 4.        */
#define ADUCM_CDPCLK0_DIV8      (3 << 3)    /**< PCLK0 divided by 8.        */
#define ADUCM_CDPCLK0_DIV16     (4 << 3)    /**< PCLK0 divided by 16.       */
#define ADUCM_CDPCLK0_DIV32     (5 << 3)    /**< PCLK0 divided by 32.       */
#define ADUCM_CDPCLK0_DIV64     (6 << 3)    /**< PCLK0 divided by 64.       */
#define ADUCM_CDPCLK0_DIV128    (7 << 3)    /**< PCLK0 divided by 128.      */

#define ADUCM_CDPCLK1_DIV1      (0 << 6)    /**< PCLK1 divided by 1.        */
#define ADUCM_CDPCLK1_DIV2      (1 << 6)    /**< PCLK1 divided by 2.        */
#define ADUCM_CDPCLK1_DIV4      (2 << 6)    /**< PCLK1 divided by 4.        */
#define ADUCM_CDPCLK1_DIV8      (3 << 6)    /**< PCLK1 divided by 8.        */
#define ADUCM_CDPCLK1_DIV16     (4 << 6)    /**< PCLK1 divided by 16.       */
#define ADUCM_CDPCLK1_DIV32     (5 << 6)    /**< PCLK1 divided by 32.       */
#define ADUCM_CDPCLK1_DIV64     (6 << 6)    /**< PCLK1 divided by 64.       */
#define ADUCM_CDPCLK1_DIV128    (7 << 6)    /**< PCLK1 divided by 128.      */

#define ADUCM_CDANACLK_DIV1     (0 << 9)    /**< Analog FE divided by 1.    */
#define ADUCM_CDANACLK_DIV2     (1 << 9)    /**< Analog FE divided by 2.    */
#define ADUCM_CDANACLK_DIV4     (2 << 9)    /**< Analog FE divided by 4.    */
#define ADUCM_CDANACLK_DIV8     (3 << 9)    /**< Analog FE divided by 8.    */
#define ADUCM_CDANACLK_DIV16    (4 << 9)    /**< Analog FE divided by 16.   */
#define ADUCM_CDANACLK_DIV32    (5 << 9)    /**< Analog FE divided by 32.   */
#define ADUCM_CDANACLK_DIV64    (6 << 9)    /**< Analog FE divided by 64.   */
#define ADUCM_CDANACLK_DIV128   (7 << 9)    /**< Analog FE divided by 128.  */
/** @} */

/**
 * @name    CLKSTAT0 register bits definitions
 * @{
 */
#define ADUCM_SPLL_STA_MASK     0x0001      /**< System PLL status mask.    */
#define ADUCM_SPLL_STA_UNLOCKED (0 << 0)    /**< System PLL locked.         */
#define ADUCM_SPLL_STA_LOCKED   (1 << 0)    /**< System PLL unlocked.       */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Disables the CA initialization in the HAL.
 */
#if !defined(ADUCM_NO_INIT) || defined(__DOXYGEN__)
#define ADUCM_NO_INIT                       FALSE
#endif

/**
 * @brief   Main clock source selection.
 */
#if !defined(ADUCM_CLKMUX) || defined(__DOXYGEN__)
#define ADUCM_CLKMUX                        ADUCM_CLKMUX_SPLL
#endif

/**
 * @brief   Clock out source selection.
 */
#if !defined(ADUCM_CLKOUT) || defined(__DOXYGEN__)
#define ADUCM_CLKOUT                        ADUCM_CLKOUT_DISABLED
#endif

/**
 * @brief   Analog clock multiplexer selection.
 */
#if !defined(ADUCM_AMUX) || defined(__DOXYGEN__)
#define ADUCM_AMUX                          ADUCM_AMUX_HFOSC
#endif
/** @} */

/**
 * @brief   Analog clock root source.
 */
#if !defined(ADUCM_ANAROOT) || defined(__DOXYGEN__)
#define ADUCM_ANAROOT                       ADUCM_ANAROOT_AMUX_X2
#endif
/** @} */

/**
 * @brief   High Speed Bus clock divider.
 */
#if !defined(ADUCM_CDHCLK_DIV) || defined(__DOXYGEN__)
#define ADUCM_CDHCLK_DIV                    ADUCM_CDHCLK_DIV1
#endif
/** @} */

/**
 * @brief   Peripheral Bus 0 clock divider.
 */
#if !defined(ADUCM_CDPCLK0_DIV) || defined(__DOXYGEN__)
#define ADUCM_CDPCLK0_DIV                   ADUCM_CDPCLK0_DIV1
#endif
/** @} */

/**
 * @brief   Peripheral Bus 1 clock divider.
 */
#if !defined(ADUCM_CDPCLK1_DIV) || defined(__DOXYGEN__)
#define ADUCM_CDPCLK1_DIV                   ADUCM_CDPCLK1_DIV1
#endif
/** @} */

/**
 * @brief   ADC clock divider.
 */
#if !defined(ADUCM_CDANACLK_DIV) || defined(__DOXYGEN__)
#define ADUCM_CDANACLK_DIV                  ADUCM_CDANACLK_DIV1
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*
 * Configuration-related checks.
 */
#if !defined(ADUCM41x_MCUCONF)
#error "Using a wrong mcuconf.h file, ADUCM41x_MCUCONF not defined"
#endif

/**
 * @brief   SYSCLK source.
 */
#if (ADUCM_CLKMUX == ADUCM_CLKMUX_HFOSC) || defined(__DOXYGEN__)
#define ADUCM_SYSCLK                ADUCM_HFOSC
#elif ADUCM_CLKMUX == ADUCM_CLKMUX_SPLL
#define ADUCM_SYSCLK                ADUCM_SPLL
#elif ADUCM_CLKMUX == ADUCM_CLKMUX_EXTCLK
#define ADUCM_SYSCLK                ADUCM_EXTCLK
#else
#error "invalid ADUCM_CLKMUX value specified"
#endif

/**
 * @brief   AMUX source.
 */
#if (ADUCM_AMUX == ADUCM_AMUX_HFOSC) || defined(__DOXYGEN__)
#define ADUCM_AMUX_CLK              ADUCM_HFOSC
#elif ADUCM_ANAROOT == ADUCM_ANAROOT_EXTCLK
#define ADUCM_AMUX_CLK              ADUCM_EXTCLK
#else
#error "invalid ADUCM_AMUX value specified"
#endif

#if (ADUCM_CLKMUX == ADUCM_CLKMUX_EXTCLK) || (ADUCM_ANAROOT == ADUCM_ANAROOT_EXTCLK)

/* External clock frequency check.*/
#if (ADUCM_EXTCLK < ADUCM_EXTCLK_MIN) || (ADUCM_EXTCLK > ADUCM_EXTCLK_MAX)
#error "ADUCM_EXTCLK outside acceptable range (ADUCM_EXTCLK_MIN...ADUCM_EXTCLK_MAX)"
#endif

#endif

/**
 * @brief   ANAROOT source.
 */
#if (ADUCM_ANAROOT == ADUCM_ANAROOT_AMUX) || defined(__DOXYGEN__)
#define ADUCM_ANAROOT_CLK           ADUCM_AMUX_CLK
#elif ADUCM_ANAROOT == ADUCM_ANAROOT_AMUX_X2
#define ADUCM_ANAROOT_CLK           (ADUCM_AMUX_CLK * 2)
#elif ADUCM_ANAROOT == ADUCM_ANAROOT_SPLL
#define ADUCM_ANAROOT_CLK           ADUCM_SPLL
#elif ADUCM_ANAROOT == ADUCM_CLKMUX_EXTCLK
#define ADUCM_ANAROOT_CLK           ADUCM_EXTCLK
#else
#error "invalid ADUCM_ANAROOT value specified"
#endif

/**
 * @brief   HCLK frequency.
 */
#if (ADUCM_CDHCLK_DIV == ADUCM_CDHCLK_DIV1) || defined(__DOXYGEN__)
#define ADUCM_HCLK                  (ADUCM_SYSCLK / 1)
#elif ADUCM_CDHCLK_DIV == ADUCM_CDHCLK_DIV2
#define ADUCM_HCLK                  (ADUCM_SYSCLK / 2)
#elif ADUCM_CDHCLK_DIV == ADUCM_CDHCLK_DIV4
#define ADUCM_HCLK                  (ADUCM_SYSCLK / 4)
#elif ADUCM_CDHCLK_DIV == ADUCM_CDHCLK_DIV8
#define ADUCM_HCLK                  (ADUCM_SYSCLK / 8)
#elif ADUCM_CDHCLK_DIV == ADUCM_CDHCLK_DIV16
#define ADUCM_HCLK                  (ADUCM_SYSCLK / 16)
#elif ADUCM_CDHCLK_DIV == ADUCM_CDHCLK_DIV32
#define ADUCM_HCLK                  (ADUCM_SYSCLK / 32)
#elif ADUCM_CDHCLK_DIV == ADUCM_CDHCLK_DIV64
#define ADUCM_HCLK                  (ADUCM_SYSCLK / 64)
#elif ADUCM_CDHCLK_DIV == ADUCM_CDHCLK_DIV128
#define ADUCM_HCLK                  (ADUCM_SYSCLK / 128)
#else
#error "invalid ADUCM_CDHCLK_DIV value specified"
#endif

/**
 * @brief   PCLK0 frequency.
 */
#if (ADUCM_CDPCLK0_DIV == ADUCM_CDPCLK0_DIV1) || defined(__DOXYGEN__)
#define ADUCM_PCLK0                 (ADUCM_SYSCLK / 1)
#elif ADUCM_CDPCLK0_DIV == ADUCM_CDPCLK0_DIV2
#define ADUCM_PCLK0                 (ADUCM_SYSCLK / 2)
#elif ADUCM_CDPCLK0_DIV == ADUCM_CDPCLK0_DIV4
#define ADUCM_PCLK0                 (ADUCM_SYSCLK / 4)
#elif ADUCM_CDPCLK0_DIV == ADUCM_CDPCLK0_DIV8
#define ADUCM_PCLK0                 (ADUCM_SYSCLK / 8)
#elif ADUCM_CDPCLK0_DIV == ADUCM_CDPCLK0_DIV16
#define ADUCM_PCLK0                 (ADUCM_SYSCLK / 16)
#elif ADUCM_CDPCLK0_DIV == ADUCM_CDPCLK0_DIV32
#define ADUCM_PCLK0                 (ADUCM_SYSCLK / 32)
#elif ADUCM_CDPCLK0_DIV == ADUCM_CDPCLK0_DIV64
#define ADUCM_PCLK0                 (ADUCM_SYSCLK / 64)
#elif ADUCM_CDPCLK0_DIV == ADUCM_CDPCLK0_DIV128
#define ADUCM_PCLK0                 (ADUCM_SYSCLK / 128)
#else
#error "invalid ADUCM_CDPCLK0_DIV value specified"
#endif

/**
 * @brief   PCLK1 frequency.
 */
#if (ADUCM_CDPCLK1_DIV == ADUCM_CDPCLK1_DIV1) || defined(__DOXYGEN__)
#define ADUCM_PCLK1                 (ADUCM_SYSCLK / 1)
#elif ADUCM_CDPCLK1_DIV == ADUCM_CDPCLK1_DIV2
#define ADUCM_PCLK1                 (ADUCM_SYSCLK / 2)
#elif ADUCM_CDPCLK1_DIV == ADUCM_CDPCLK1_DIV4
#define ADUCM_PCLK1                 (ADUCM_SYSCLK / 4)
#elif ADUCM_CDPCLK1_DIV == ADUCM_CDPCLK1_DIV8
#define ADUCM_PCLK1                 (ADUCM_SYSCLK / 8)
#elif ADUCM_CDPCLK1_DIV == ADUCM_CDPCLK1_DIV16
#define ADUCM_PCLK1                 (ADUCM_SYSCLK / 16)
#elif ADUCM_CDPCLK1_DIV == ADUCM_CDPCLK1_DIV32
#define ADUCM_PCLK1                 (ADUCM_SYSCLK / 32)
#elif ADUCM_CDPCLK1_DIV == ADUCM_CDPCLK1_DIV64
#define ADUCM_PCLK1                 (ADUCM_SYSCLK / 64)
#elif ADUCM_CDPCLK1_DIV == ADUCM_CDPCLK1_DIV128
#define ADUCM_PCLK1                 (ADUCM_SYSCLK / 128)
#else
#error "invalid ADUCM_CDPCLK1_DIV value specified"
#endif

/**
 * @brief   Analog Front End frequency.
 */
#if (ADUCM_CDANACLK_DIV == ADUCM_CDANACLK_DIV1) || defined(__DOXYGEN__)
#define ADUCM_AFE_CLK               (ADUCM_ANAROOT_CLK / 1)
#elif ADUCM_CDANACLK_DIV == ADUCM_CDANACLK_DIV2
#define ADUCM_AFE_CLK               (ADUCM_ANAROOT_CLK / 2)
#elif ADUCM_CDANACLK_DIV == ADUCM_CDANACLK_DIV4
#define ADUCM_AFE_CLK               (ADUCM_ANAROOT_CLK / 4)
#elif ADUCM_CDANACLK_DIV == ADUCM_CDANACLK_DIV8
#define ADUCM_AFE_CLK               (ADUCM_ANAROOT_CLK / 8)
#elif ADUCM_CDANACLK_DIV == ADUCM_CDANACLK_DIV16
#define ADUCM_AFE_CLK               (ADUCM_ANAROOT_CLK / 16)
#elif ADUCM_CDANACLK_DIV == ADUCM_CDANACLK_DIV32
#define ADUCM_AFE_CLK               (ADUCM_ANAROOT_CLK / 32)
#elif ADUCM_CDANACLK_DIV == ADUCM_CDANACLK_DIV64
#define ADUCM_AFE_CLK               (ADUCM_ANAROOT_CLK / 64)
#elif ADUCM_CDANACLK_DIV == ADUCM_CDANA_DIV128
#define ADUCM_AFE_CLK               (ADUCM_ANAROOT_CLK / 128)
#else
#error "invalid ADUCM_CDANA_DIV value specified"
#endif

/* 
 * Analog Front End clock check.
 */ 
#if (ADUCM_AFE_CLK < ADUCM_AFE_CLK_MIN) || (ADUCM_AFE_CLK > ADUCM_AFE_CLK_MAX)
#warning "The AFE is calibrated to work with a 32MHz clock."
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

/* Various helpers.*/
#include "nvic.h"
#include "cache.h"
#include "aducm_isr.h"

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
  void aducm_clock_init(void);
#ifdef __cplusplus
}
#endif

#endif /* HAL_LLD_H */

/** @} */

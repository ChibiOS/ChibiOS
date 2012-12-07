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
 * @file    templates/hal_lld.h
 * @brief   HAL subsystem low level driver header template.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "sam4l.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Defines the support for realtime counters in the HAL.
 */
#define HAL_IMPLEMENTS_COUNTERS TRUE

/**
 * @brief   Platform name
 */
#define PLATFORM_NAME   "SAM4L Series"

/**
 * @name    BUS IDs
 * @{
 */
#define SAM_CLK_GRP_CPU             0
#define SAM_CLK_GRP_HSB             1
#define SAM_CLK_GRP_PBA             2
#define SAM_CLK_GRP_PBB             3
#define SAM_CLK_GRP_PBC             4
#define SAM_CLK_GRP_PBD             5
/** @} */

/**
 * @name    Clocks derived from the HSB clock
 * @{
 */
#define SAM_HSB_PDCA_HSB            0
#define SAM_HSB_HFLASHC_DATA        1
#define SAM_HSB_HRAMC1_DATA         2
#define SAM_HSB_USBC_DATA           3
#define SAM_HSB_CRCCU_DATA          4
#define SAM_HSB_PBA_BRIDGE          5
#define SAM_HSB_PBB_BRIDGE          6
#define SAM_HSB_PBC_BRIDGE          7
#define SAM_HSB_PBD_BRIDGE          8
#define SAM_HSB_AESA_HSB            9
/** @} */

/**
 * @name    Clocks derived from the PBA clock
 * @{
 */
#define SAM_PBA_IISC                0
#define SAM_PBA_SPI                 1
#define SAM_PBA_TC0                 2
#define SAM_PBA_TC1                 3
#define SAM_PBA_TWIM0               4
#define SAM_PBA_TWIS0               5
#define SAM_PBA_TWIM1               6
#define SAM_PBA_TWIS1               7
#define SAM_PBA_USART0              8
#define SAM_PBA_USART1              9
#define SAM_PBA_USART2              10
#define SAM_PBA_USART3              11
#define SAM_PBA_ADCIFE              12
#define SAM_PBA_DACC                13
#define SAM_PBA_ACIFC               14
#define SAM_PBA_GLOC                15
#define SAM_PBA_ABDACB              16
#define SAM_PBA_TRNG                17
#define SAM_PBA_PARC                18
#define SAM_PBA_CATB                19
#define SAM_PBA_TWIM2               21
#define SAM_PBA_TWIM3               22
#define SAM_PBA_LCDCA               23
/** @} */

/**
 * @name    Clocks derived from the PBB clock
 * @{
 */
#define SAM_PBB_HFLASHC_REGS        0
#define SAM_PBB_HRAMC1_REGS         1
#define SAM_PBB_HMATRIX             2
#define SAM_PBB_PDCA_PB             3
#define SAM_PBB_CRCCU_REGS          4
#define SAM_PBB_USBC_REGS           5
#define SAM_PBB_PEVC                6
/** @} */

/**
 * @name    Clocks derived from the PBC clock
 * @{
 */
#define SAM_PBC_PM                  0
#define SAM_PBC_CHIPID              1
#define SAM_PBC_SCIF                2
#define SAM_PBC_FREQM               3
#define SAM_PBC_GPIO                4
/* @} */

/**
 * @name    Clocks derived from the PBD clock
 * @{
 */
#define SAM_PBD_BPM                 0
#define SAM_PBD_BSCIF               1
#define SAM_PBD_AST                 2
#define SAM_PBD_WDT                 3
#define SAM_PBD_EIC                 4
#define SAM_PBD_PICOUART            5
/** @} */

/**
 * @name    MCCTRL register bits definitions
 * @{
 */
#define SAM_MCSEL_MASK          (7 << 0)    /**< MCSEL bits mask.           */
#define SAM_MCSEL_RCSYS         (0 << 0)    /**< System RC oscillator.      */
#define SAM_MCSEL_OSC0          (1 << 0)    /**< Oscillator 0.              */
#define SAM_MCSEL_PLL           (2 << 0)    /**< PLL.                       */
#define SAM_MCSEL_DFLL          (3 << 0)    /**< DFLL.                      */
#define SAM_MCSEL_RC80M         (4 << 0)    /**< 80 MHz RC oscillator.      */
#define SAM_MCSEL_RCFAST        (5 << 0)    /**< 4/8/12 MHz RC oscillator.  */
#define SAM_MCSEL_RC1M          (6 << 0)    /**< 1 MHz RC oscillator.       */
/** @} */

/**
 * @name    CPUSEL register bits definitions
 * @{
 */
#define SAM_CPUSEL_MASK         (7 << 0)    /**< CPUSEL bits mask.          */
#define SAM_CPUSEL_DIV1         0
#define SAM_CPUSEL_DIV2         (SAM_CPUDIV | 0)
#define SAM_CPUSEL_DIV4         (SAM_CPUDIV | 1)
#define SAM_CPUSEL_DIV8         (SAM_CPUDIV | 2)
#define SAM_CPUSEL_DIV16        (SAM_CPUDIV | 3)
#define SAM_CPUSEL_DIV32        (SAM_CPUDIV | 4)
#define SAM_CPUSEL_DIV64        (SAM_CPUDIV | 5)
#define SAM_CPUSEL_DIV128       (SAM_CPUDIV | 6)
#define SAM_CPUSEL_DIV256       (SAM_CPUDIV | 7)
#define SAM_CPUDIV              (1 << 7)    /**< CPUDIV bit.                */

/**
 * @name    PBx registers bits definitions
 * @{
 */
#define SAM_PBSEL_MASK          (7 << 0)    /**< PBSEL bits mask.           */
#define SAM_PBSEL_DIV1          0
#define SAM_PBSEL_DIV2          (SAM_CPUDIV | 0)
#define SAM_PBSEL_DIV4          (SAM_CPUDIV | 1)
#define SAM_PBSEL_DIV8          (SAM_CPUDIV | 2)
#define SAM_PBSEL_DIV16         (SAM_CPUDIV | 3)
#define SAM_PBSEL_DIV32         (SAM_CPUDIV | 4)
#define SAM_PBSEL_DIV64         (SAM_CPUDIV | 5)
#define SAM_PBSEL_DIV128        (SAM_CPUDIV | 6)
#define SAM_PBSEL_DIV256        (SAM_CPUDIV | 7)
#define SAM_PBDIV               (1 << 7)    /**< PBDIV bit.                 */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Disables the clock initialization in the HAL.
 */
#if !defined(SAM_NO_INIT) || defined(__DOXYGEN__)
#define SAM_NO_INIT                         FALSE
#endif

/**
 * @brief   MCCTRL register settings.
 */
#if !defined(SAM_MCCTRL_MCSEL) || defined(__DOXYGEN__)
#define SAM_MCCTRL_MCSEL                    SAM_MCSEL_PLL
#endif

/**
 * @brief   CPUSEL register settings.
 */
#if !defined(SAM_CPUSEL) || defined(__DOXYGEN__)
#define SAM_CPUSEL                          SAM_CPUSEL_DIV1
#endif

/**
 * @brief   PBASEL register settings.
 */
#if !defined(SAM_PBASEL) || defined(__DOXYGEN__)
#define SAM_PBASEL                          SAM_PBSEL_DIV1
#endif

/**
 * @brief   PBBSEL register settings.
 */
#if !defined(SAM_PBBSEL) || defined(__DOXYGEN__)
#define SAM_PBBSEL                          SAM_PBSEL_DIV1
#endif

/**
 * @brief   PBCSEL register settings.
 */
#if !defined(SAM_PBCSEL) || defined(__DOXYGEN__)
#define SAM_PBCSEL                          SAM_PBSEL_DIV1
#endif

/**
 * @brief   PBDSEL register settings.
 */
#if !defined(SAM_PBDSEL) || defined(__DOXYGEN__)
#define SAM_PBDSEL                          SAM_PBSEL_DIV1
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type representing a system clock frequency.
 */
typedef uint32_t halclock_t;

/**
 * @brief   Type of the realtime free counter value.
 */
typedef uint32_t halrtcnt_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the current value of the system free running counter.
 * @note    This service is implemented by returning the content of the
 *          DWT_CYCCNT register.
 *
 * @return              The value of the system free running counter of
 *                      type halrtcnt_t.
 *
 * @notapi
 */
#define hal_lld_get_counter_value()         DWT_CYCCNT

/**
 * @brief   Realtime counter frequency.
 * @note    The DWT_CYCCNT register is incremented directly by the system
 *          clock so this function returns STM32_HCLK.
 *
 * @return              The realtime counter frequency of type halclock_t.
 *
 * @notapi
 */
#define hal_lld_get_counter_frequency()     STM32_HCLK

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void sam_enable_module(uint32_t bus_id, uint32_t module);
  void sam_disable_module(uint32_t bus_id, uint32_t module);
  void sam_clock_init(void);
  void hal_lld_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */

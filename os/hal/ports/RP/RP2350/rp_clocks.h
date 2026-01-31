/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    RP2350/rp_clocks.h
 * @brief   RP2350 clocks header.
 *
 * @addtogroup RP_CLOCKS
 * @{
 */

#ifndef RP_CLOCKS_H
#define RP_CLOCKS_H

#include "rp_xosc.h"
#include "rp_pll.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    PLL_SYS configuration
 * @note    Default configuration produces 150 MHz system clock
 * @{
 */
#if !defined(RP_PLL_SYS_REFDIV) || defined(__DOXYGEN__)
#define RP_PLL_SYS_REFDIV               1U
#endif
#if !defined(RP_PLL_SYS_VCO_FREQ) || defined(__DOXYGEN__)
#define RP_PLL_SYS_VCO_FREQ             1500000000U     /* 1500 MHz VCO */
#endif
#if !defined(RP_PLL_SYS_POSTDIV1) || defined(__DOXYGEN__)
#define RP_PLL_SYS_POSTDIV1             5U              /* 150 MHz output */
#endif
#if !defined(RP_PLL_SYS_POSTDIV2) || defined(__DOXYGEN__)
#define RP_PLL_SYS_POSTDIV2             2U
#endif
/** @} */

/**
 * @name    PLL_USB configuration
 * @{
 */
#if !defined(RP_PLL_USB_REFDIV) || defined(__DOXYGEN__)
#define RP_PLL_USB_REFDIV               1U
#endif
#if !defined(RP_PLL_USB_VCO_FREQ) || defined(__DOXYGEN__)
#define RP_PLL_USB_VCO_FREQ             1200000000U     /* 1200 MHz VCO */
#endif
#if !defined(RP_PLL_USB_POSTDIV1) || defined(__DOXYGEN__)
#define RP_PLL_USB_POSTDIV1             5U
#endif
#if !defined(RP_PLL_USB_POSTDIV2) || defined(__DOXYGEN__)
#define RP_PLL_USB_POSTDIV2             5U
#endif
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/**
 * @name    PLL output frequencies
 * @{
 */
/**
 * @brief   PLL_SYS output frequency in Hz.
 */
#define RP_PLL_SYS_CLK                      (RP_PLL_SYS_VCO_FREQ /           \
                                             (RP_PLL_SYS_POSTDIV1 *          \
                                              RP_PLL_SYS_POSTDIV2))

/**
 * @brief   PLL_USB output frequency in Hz (must be 48 MHz for USB).
 */
#define RP_PLL_USB_CLK                      (RP_PLL_USB_VCO_FREQ /           \
                                             (RP_PLL_USB_POSTDIV1 *          \
                                              RP_PLL_USB_POSTDIV2))
/** @} */

/**
 * @name    Compile-time clock frequencies
 * @note    These are compile-time constants derived from PLL configuration.
 *          For runtime clock queries, use the hal_lld_get_clock_point() API.
 * @{
 */
/**
 * @brief   System core clock frequency in Hz.
 */
#define RP_CLK_SYS_FREQ                     RP_PLL_SYS_CLK

/**
 * @brief   Reference clock frequency in Hz.
 */
#define RP_CLK_REF_FREQ                     RP_XOSCCLK

/**
 * @brief   Peripheral clock frequency in Hz.
 * @note    CLK_PERI runs at CLK_SYS frequency by default.
 */
#define RP_CLK_PERI_FREQ                    RP_PLL_SYS_CLK

/**
 * @brief   USB clock frequency in Hz (must be 48 MHz).
 */
#define RP_CLK_USB_FREQ                     RP_PLL_USB_CLK

/**
 * @brief   ADC clock frequency in Hz (must be 48 MHz).
 */
#define RP_CLK_ADC_FREQ                     RP_PLL_USB_CLK
/** @} */

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
  void rp_clock_init(void);
  uint32_t rp_clock_get_hz(uint32_t clk_index);
#ifdef __cplusplus
}
#endif

#endif /* RP_CLOCKS_H */

/** @} */

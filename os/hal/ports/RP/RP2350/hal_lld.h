/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

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
 * @file    RP2350/hal_lld.h
 * @brief   RP2350 HAL subsystem low level driver header.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef HAL_LLD_H
#define HAL_LLD_H

/*
 * Registry definitions.
 */
#include "rp_registry.h"
#include "rp_clocks.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Platform identification macros
 * @{
 */
#if defined(RP2350) || defined(__DOXYGEN__)
#define PLATFORM_NAME           "RP2350"

#else
#error "RP2350 device not specified"
#endif
/** @} */

/**
 * @name    Internal clock sources
 * @{
 */
#define RP_ROSCCLK              6500000     /**< 6.5MHz internal clock.     */
/** @} */

/**
 * @brief   Dynamic clock supported.
 */
#define HAL_LLD_USE_CLOCK_MANAGEMENT

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Disables the clocks initialization in the HAL.
 */
#if !defined(RP_NO_INIT) || defined(__DOXYGEN__)
#define RP_NO_INIT                          FALSE
#endif

/**
 * @brief   Starts core 1 after initialization.
 */
#if !defined(RP_CORE1_START) || defined(__DOXYGEN__)
#define RP_CORE1_START                      FALSE
#endif

/**
 * @brief   Symbol for core 1 vectors table.
 */
#if !defined(RP_CORE1_VECTORS_TABLE) || defined(__DOXYGEN__)
#define RP_CORE1_VECTORS_TABLE              _vectors
#endif

/**
 * @brief   Symbol for core 1 entry point.
 */
#if !defined(RP_CORE1_ENTRY_POINT) || defined(__DOXYGEN__)
#define RP_CORE1_ENTRY_POINT                _crt0_c1_entry
#endif

/**
 * @brief   Symbol for core 1 initial MSP position.
 */
#if !defined(RP_CORE1_STACK_END) || defined(__DOXYGEN__)
#define RP_CORE1_STACK_END                  __c1_main_stack_end__
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*
 * Configuration-related checks.
 */
#if !defined(RP2350_MCUCONF)
#error "Using a wrong mcuconf.h file, RP2350_MCUCONF not defined"
#endif

/*
 * Board files sanity checks.
 */
#if !defined(RP_XOSCCLK)
#error "RP_XOSCCLK not defined in board.h"
#endif

#if (RP_XOSCCLK < 1000000) || (RP_XOSCCLK > 15000000)
#error "RP_XOSCCLK out of valid range (1-15 MHz)"
#endif

/*
 * PLL_SYS configuration checks.
 */
#if (RP_PLL_SYS_REFDIV < 1) || (RP_PLL_SYS_REFDIV > 63)
#error "RP_PLL_SYS_REFDIV out of valid range (1-63)"
#endif

#if (RP_PLL_SYS_VCO_FREQ < RP_PLL_VCO_MIN_FREQ) ||                          \
    (RP_PLL_SYS_VCO_FREQ > RP_PLL_VCO_MAX_FREQ)
#error "RP_PLL_SYS_VCO_FREQ out of valid range (750-1600 MHz)"
#endif

#if (RP_PLL_SYS_POSTDIV1 < 1) || (RP_PLL_SYS_POSTDIV1 > 7)
#error "RP_PLL_SYS_POSTDIV1 out of valid range (1-7)"
#endif

#if (RP_PLL_SYS_POSTDIV2 < 1) || (RP_PLL_SYS_POSTDIV2 > 7)
#error "RP_PLL_SYS_POSTDIV2 out of valid range (1-7)"
#endif

#if RP_PLL_SYS_POSTDIV1 < RP_PLL_SYS_POSTDIV2
#error "RP_PLL_SYS_POSTDIV1 must be >= RP_PLL_SYS_POSTDIV2"
#endif

/*
 * PLL_USB configuration checks.
 */
#if (RP_PLL_USB_REFDIV < 1) || (RP_PLL_USB_REFDIV > 63)
#error "RP_PLL_USB_REFDIV out of valid range (1-63)"
#endif

#if (RP_PLL_USB_VCO_FREQ < RP_PLL_VCO_MIN_FREQ) ||                          \
    (RP_PLL_USB_VCO_FREQ > RP_PLL_VCO_MAX_FREQ)
#error "RP_PLL_USB_VCO_FREQ out of valid range (750-1600 MHz)"
#endif

#if (RP_PLL_USB_POSTDIV1 < 1) || (RP_PLL_USB_POSTDIV1 > 7)
#error "RP_PLL_USB_POSTDIV1 out of valid range (1-7)"
#endif

#if (RP_PLL_USB_POSTDIV2 < 1) || (RP_PLL_USB_POSTDIV2 > 7)
#error "RP_PLL_USB_POSTDIV2 out of valid range (1-7)"
#endif

#if RP_PLL_USB_POSTDIV1 < RP_PLL_USB_POSTDIV2
#error "RP_PLL_USB_POSTDIV1 must be >= RP_PLL_USB_POSTDIV2"
#endif

#if RP_PLL_USB_CLK != 48000000
#error "RP_PLL_USB_CLK must be 48 MHz for USB to work"
#endif

/**
 * @name    Various clock points.
 * @{
 */
#define RP_GPOUT0_CLK           hal_lld_get_clock_point(RP_CLK_GPOUT0)
#define RP_GPOUT1_CLK           hal_lld_get_clock_point(RP_CLK_GPOUT1)
#define RP_GPOUT2_CLK           hal_lld_get_clock_point(RP_CLK_GPOUT2)
#define RP_GPOUT3_CLK           hal_lld_get_clock_point(RP_CLK_GPOUT3)
#define RP_REF_CLK              hal_lld_get_clock_point(RP_CLK_REF)
#define RP_CORE_CLK             hal_lld_get_clock_point(RP_CLK_SYS)
#define RP_PERI_CLK             hal_lld_get_clock_point(RP_CLK_PERI)
#define RP_HSTX_CLK             hal_lld_get_clock_point(RP_CLK_HSTX)
#define RP_USB_CLK              hal_lld_get_clock_point(RP_CLK_USB)
#define RP_ADC_CLK              hal_lld_get_clock_point(RP_CLK_ADC)
/** @} */

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Type of a clock configuration structure.
 */
typedef struct {
  uint32_t          dummy;
} halclkcfg_t;
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Safety module counter support
 * @note    Uses TIMER0 peripheral which counts at 1 us resolution.
 *          During early clock init, accuracy depends on ROSC variance.
 *          After clock init completes, timing is precise.
 * @{
 */

/**
 * @brief   Counter type for safety timeouts.
 */
typedef uint32_t halcnt_t;

/**
 * @brief   Returns the counter frequency in Hz.
 * @note    Always returns 1 MHz (1 us ticks).
 */
#define HAL_LLD_GET_CNT_FREQUENCY()     1000000U

/**
 * @brief   Returns the current counter value.
 */
#define HAL_LLD_GET_CNT_VALUE()         ((halcnt_t)TIMER0->TIMERAWL)

/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Various helpers.*/
#include "nvic.h"
#include "cache.h"
#include "rp_isr.h"
#include "rp_fifo.h"
#include "rp_dma.h"

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Driver inline functions.                                                  */
/*===========================================================================*/

__STATIC_INLINE void hal_lld_peripheral_reset(uint32_t mask) {

  rp_set_bits(&RESETS->RESET, mask);
}

__STATIC_INLINE void hal_lld_peripheral_unreset(uint32_t mask) {

  rp_clear_bits(&RESETS->RESET, mask);
  while ((RESETS->RESET_DONE & mask) != mask) {
    /* Waiting for peripheral to come out of reset */
  }
}

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Switches to a different clock configuration
 *
 * @param[in] ccp       pointer to clock a @p halclkcfg_t structure
 * @return              The clock switch result.
 * @retval false        if the clock switch succeeded
 * @retval true         if the clock switch failed
 *
 * @notapi
 */
__STATIC_INLINE bool hal_lld_clock_switch_mode(const halclkcfg_t *ccp) {

  (void)ccp;

  return false;
}

/**
 * @brief   Returns the frequency of a clock point in Hz.
 *
 * @param[in] clkpt     clock point to be returned
 * @return              The clock point frequency in Hz or zero if the
 *                      frequency is unknown.
 *
 * @notapi
 */
__STATIC_INLINE halfreq_t hal_lld_get_clock_point(halclkpt_t clkpt) {

  osalDbgAssert(clkpt < RP_CLK_COUNT, "invalid clock point");

  return rp_clock_get_hz(clkpt);
}
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

#endif /* HAL_LLD_H */

/** @} */

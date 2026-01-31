/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    RP2350/rp_pll.h
 * @brief   RP2350 PLL header.
 *
 * @addtogroup RP_PLL
 * @{
 */

#ifndef RP_PLL_H
#define RP_PLL_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Minimum VCO frequency in Hz.
 */
#define RP_PLL_VCO_MIN_FREQ             750000000U

/**
 * @brief   Maximum VCO frequency in Hz.
 */
#define RP_PLL_VCO_MAX_FREQ             1600000000U

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

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
  void rp_pll_init(PLL_TypeDef *pll, uint32_t refdiv, uint32_t vco_freq,
                   uint32_t postdiv1, uint32_t postdiv2);
  void rp_pll_deinit(PLL_TypeDef *pll);
#ifdef __cplusplus
}
#endif

#endif /* RP_PLL_H */

/** @} */

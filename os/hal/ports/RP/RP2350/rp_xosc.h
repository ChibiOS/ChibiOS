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
 * @file    RP2350/rp_xosc.h
 * @brief   RP2350 XOSC header.
 *
 * @addtogroup RP_XOSC
 * @{
 */

#ifndef RP_XOSC_H
#define RP_XOSC_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   XOSC startup delay multiplier.
 * @note    Default value of 1 for 12MHz crystal.
 */
#if !defined(RP_XOSC_STARTUP_DELAY_MULTIPLIER)
#define RP_XOSC_STARTUP_DELAY_MULTIPLIER    1U
#endif

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/**
 * @brief   XOSC startup delay value.
 * @note    Delay is in multiples of 256 crystal periods
 *          For 12MHz, each count is ~21.3us. Default gives 1ms startup.
 */
#define RP_XOSC_STARTUP_DELAY               \
    ((((RP_XOSCCLK / 1000U) + 128U) / 256U) * RP_XOSC_STARTUP_DELAY_MULTIPLIER)

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
  void rp_xosc_init(void);
  void rp_xosc_disable(void);
#ifdef __cplusplus
}
#endif

#endif /* RP_XOSC_H */

/** @} */

/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
 * @file    STM32F1xx/hal_lld.h
 * @brief   STM32F1xx HAL subsystem low level driver header.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - STM32_LSECLK.
 *          - STM32_HSECLK.
 *          .
 *          One of the following macros must also be defined:
 *          - STM32F10X_LD_VL for Value Line Low Density devices.
 *          - STM32F10X_MD_VL for Value Line Medium Density devices.
 *          - STM32F10X_LD for Performance Low Density devices.
 *          - STM32F10X_MD for Performance Medium Density devices.
 *          - STM32F10X_HD for Performance High Density devices.
 *          - STM32F10X_XL for Performance eXtra Density devices.
 *          - STM32F10X_CL for Connectivity Line devices.
 *          .
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "stm32.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if defined(__DOXYGEN__)
/**
 * @name    Platform identification
 * @{
 */
#define PLATFORM_NAME           "STM32"
/** @} */

#elif defined(STM32F10X_LD_VL) || defined(STM32F10X_MD_VL) ||               \
      defined(STM32F10X_HD_VL) || defined(__DOXYGEN__)
#include "hal_lld_f100.h"

#elif defined(STM32F10X_LD) || defined(STM32F10X_MD) ||                     \
      defined(STM32F10X_HD) || defined(STM32F10X_XL) ||                     \
      defined(__DOXYGEN__)
#include "hal_lld_f103.h"

#elif defined(STM32F10X_CL) || defined(__DOXYGEN__)
#include "hal_lld_f105_f107.h"

#else
#error "unspecified, unsupported or invalid STM32 platform"
#endif

/* There are differences in vector names in the various sub-families,
   normalizing.*/
#if defined(STM32F10X_XL)
#define TIM1_BRK_IRQn       TIM1_BRK_TIM9_IRQn
#define TIM1_UP_IRQn        TIM1_UP_TIM10_IRQn
#define TIM1_TRG_COM_IRQn   TIM1_TRG_COM_TIM11_IRQn
#define TIM8_BRK_IRQn       TIM8_BRK_TIM12_IRQn
#define TIM8_UP_IRQn        TIM8_UP_TIM13_IRQn
#define TIM8_TRG_COM_IRQn   TIM8_TRG_COM_TIM14_IRQn

#elif defined(STM32F10X_LD_VL)|| defined(STM32F10X_MD_VL) ||                \
      defined(STM32F10X_HD_VL)
#define TIM1_BRK_IRQn       TIM1_BRK_TIM15_IRQn
#define TIM1_UP_IRQn        TIM1_UP_TIM16_IRQn
#define TIM1_TRG_COM_IRQn   TIM1_TRG_COM_TIM17_IRQn
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

/* STM32 DMA and RCC helpers.*/
#include "stm32_dma.h"
#include "stm32_rcc.h"

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
  void stm32_clock_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */

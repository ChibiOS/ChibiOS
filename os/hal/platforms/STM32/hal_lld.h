/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file STM32/hal_lld.h
 * @brief STM32 HAL subsystem low level driver header.
 * @addtogroup STM32_HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

/*
 * Tricks required to make the TRUE/FALSE declaration inside the library
 * compatible.
 */
#undef FALSE
#undef TRUE
#include "stm32f10x.h"
#define FALSE 0
#define TRUE (!FALSE)

#include "nvic.h"
#include "stm32_dma.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief System clock setting.
 * @note Only 48MHz and 72MHz are currently supported.
 */
#if !defined(STM32_SYSCLK) || defined(__DOXYGEN__)
#define STM32_SYSCLK                72
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*
 * NOTES: PLLPRE can be 1 or 2, PLLMUL can be 2..16.
 */
#define PLLPRE                      1
#if STM32_SYSCLK == 48
  #define PLLMUL                    6
#elif STM32_SYSCLK == 72
  #define PLLMUL                    9
#else
#error "unsupported STM32_SYSCLK setting"
#endif
#define PLLCLK                      ((HSECLK / PLLPRE) * PLLMUL)
#define SYSCLK                      PLLCLK
#define APB1CLK                     (SYSCLK / 2)
#define APB2CLK                     (SYSCLK / 2)
#define AHB1CLK                     (SYSCLK / 1)
#define TIMCLK2                     (APB2CLK * 2)
#define TIMCLK1                     (APB1CLK * 2)

/*
 * Values derived from the clock settings.
 */
#define PLLPREBITS                  ((PLLPRE - 1) << 17)
#define PLLMULBITS                  ((PLLMUL - 2) << 18)
#if STM32_SYSCLK == 48
  #define USBPREBITS                RCC_CFGR_USBPRE
  #define FLASHBITS                 0x00000011
#elif STM32_SYSCLK == 72
  #define USBPREBITS                0
  #define FLASHBITS                 0x00000012
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
  void stm32_clock_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */

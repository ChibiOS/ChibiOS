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
 * @file    STM32/stm32.h
 * @brief   STM32 common header.
 * @pre     One of the following macros must be defined before including
 *          this header, the macro selects the inclusion of the appropriate
 *          vendor header:
 *          - STM32F10X_LD_VL for Value Line Low Density devices.
 *          - STM32F10X_MD_VL for Value Line Medium Density devices.
 *          - STM32F10X_LD for Performance Low Density devices.
 *          - STM32F10X_MD for Performance Medium Density devices.
 *          - STM32F10X_HD for Performance High Density devices.
 *          - STM32F10X_XL for Performance eXtra Density devices.
 *          - STM32F10X_CL for Connectivity Line devices.
 *          - STM32F2XX for High-performance STM32 F-2 devices.
 *          - STM32F4XX for High-performance STM32 F-4 devices.
 *          - STM32L1XX_MD for Ultra Low Power Medium-density devices.
 *          .
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _STM32_H_
#define _STM32_H_

#if defined(STM32F10X_LD_VL) || defined(STM32F10X_MD_VL) ||                 \
    defined(STM32F10X_HD_VL) || defined(STM32F10X_LD)    ||                 \
    defined(STM32F10X_MD)    || defined(STM32F10X_HD)    ||                 \
    defined(STM32F10X_XL)    || defined(STM32F10X_CL)    ||                 \
    defined(__DOXYGEN__)
#include "stm32f10x.h"
#endif

#if defined(STM32F2XX) || defined(__DOXYGEN__)
#include "stm32f2xx.h"
#endif

#if defined(STM32F4XX) || defined(__DOXYGEN__)
#include "stm32f4xx.h"
#endif

#if defined(STM32L1XX_MD) || defined(__DOXYGEN__)
#include "stm32l1xx.h"
#endif

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   STM32 TIM registers block.
 * @note    Redefined from the ST headers because the non uniform
 *          declaration of the CCR registers among the various
 *          sub-families.
 */
typedef struct {
  volatile uint16_t     CR1;
  uint16_t              _resvd0;
  volatile uint16_t     CR2;
  uint16_t              _resvd1;
  volatile uint16_t     SMCR;
  uint16_t              _resvd2;
  volatile uint16_t     DIER;
  uint16_t              _resvd3;
  volatile uint16_t     SR;
  uint16_t              _resvd4;
  volatile uint16_t     EGR;
  uint16_t              _resvd5;
  volatile uint16_t     CCMR1;
  uint16_t              _resvd6;
  volatile uint16_t     CCMR2;
  uint16_t              _resvd7;
  volatile uint16_t     CCER;
  uint16_t              _resvd8;
  volatile uint32_t     CNT;
  volatile uint16_t     PSC;
  uint16_t              _resvd9;
  volatile uint32_t     ARR;
  volatile uint16_t     RCR;
  uint16_t              _resvd10;
  volatile uint32_t     CCR[4];
  volatile uint16_t     BDTR;
  uint16_t              _resvd11;
  volatile uint16_t     DCR;
  uint16_t              _resvd12;
  volatile uint16_t     DMAR;
  uint16_t              _resvd13;
  volatile uint16_t     OR;
  uint16_t              _resvd14;
} stm32_tim_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    TIM units references
 * @{
 */
#define STM32_TIM1      ((stm32_tim_t *)TIM1_BASE)
#define STM32_TIM2      ((stm32_tim_t *)TIM2_BASE)
#define STM32_TIM3      ((stm32_tim_t *)TIM3_BASE)
#define STM32_TIM4      ((stm32_tim_t *)TIM4_BASE)
#define STM32_TIM5      ((stm32_tim_t *)TIM5_BASE)
#define STM32_TIM6      ((stm32_tim_t *)TIM6_BASE)
#define STM32_TIM7      ((stm32_tim_t *)TIM7_BASE)
#define STM32_TIM8      ((stm32_tim_t *)TIM8_BASE)
#define STM32_TIM9      ((stm32_tim_t *)TIM9_BASE)
#define STM32_TIM10     ((stm32_tim_t *)TIM10_BASE)
#define STM32_TIM11     ((stm32_tim_t *)TIM11_BASE)
#define STM32_TIM12     ((stm32_tim_t *)TIM12_BASE)
#define STM32_TIM13     ((stm32_tim_t *)TIM13_BASE)
#define STM32_TIM14     ((stm32_tim_t *)TIM14_BASE)
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#endif /* _STM32_H_ */

/** @} */

/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    STM32/hal_lld.h
 * @brief   STM32 HAL subsystem low level driver header.
 *
 * @addtogroup STM32_HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

/* Tricks required to make the TRUE/FALSE declaration inside the library
   compatible.*/
#undef FALSE
#undef TRUE
#include "stm32f10x.h"
#define FALSE 0
#define TRUE (!FALSE)

#include "nvic.h"
#include "stm32_dma.h"

/**
 * @brief   Platform name.
 */
#if defined(STM32F10X_MD) || defined(__DOXYGEN__)
#define PLATFORM_NAME           "STM32 MD"
#include "hal_lld_f103.h"
#elif defined(STM32F10X_LD)
#define PLATFORM_NAME           "STM32 LD"
#include "hal_lld_f103.h"
#elif defined(STM32F10X_HD)
#define PLATFORM_NAME           "STM32 HD"
#include "hal_lld_f103.h"
#elif defined(STM32F10X_CL)
#define PLATFORM_NAME           "STM32 CL"
#include "hal_lld_f105_f107.h"
#else
#error "STM32 platform unknown or not specified"
#endif

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

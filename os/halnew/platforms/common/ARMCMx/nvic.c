/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
 * @file    common/ARMCMx/nvic.c
 * @brief   Cortex-Mx NVIC support code.
 *
 * @addtogroup COMMON_ARMCMx_NVIC
 * @{
 */

#include "hal.h"

/**
 * @brief   Sets the priority of an interrupt handler and enables it.
 * @note    The parameters are not tested for correctness.
 *
 * @param[in] n         the interrupt number
 * @param[in] prio      the interrupt priority mask
 */
void nvicEnableVector(uint32_t n, uint32_t prio) {

  NVIC->IP[n]        = prio << (8 - __NVIC_PRIO_BITS);
  NVIC->ICPR[n >> 5] = 1 << (n & 0x1F);
  NVIC->ISER[n >> 5] = 1 << (n & 0x1F);
}

/**
 * @brief   Disables an interrupt handler.
 * @note    The parameters are not tested for correctness.
 *
 * @param[in] n         the interrupt number
 */
void nvicDisableVector(uint32_t n) {

  NVIC->ICER[n >> 5] = 1 << (n & 0x1F);
  NVIC->IP[n]        = 0;
}

/** @} */

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
 * @file LPC214x/vic.c
 * @brief LPC214x VIC peripheral support code.
 * @addtogroup LPC214x_VIC
 * @{
 */

#include "ch.h"

/**
 * @brief VIC Initialization.
 * @note Better reset everything in the VIC, it is a HUGE source of trouble.
 */
void vic_init(void) {
  int i;

  VIC *vic = VICBase;
  vic->VIC_IntSelect = 0;               /* All sources assigned to IRQ. */
  vic->VIC_SoftIntClear = ALLINTMASK;   /* No interrupts enforced */
  vic->VIC_IntEnClear = ALLINTMASK;     /* All sources disabled. */
  for (i = 0; i < 16; i++) {
    vic->VIC_VectCntls[i] = 0;
    vic->VIC_VectAddrs[i] = 0;
    vic->VIC_VectAddr = 0;
  }
}

/**
 * @brief Initializes a VIC vector.
 * @details Set a vector for an interrupt source and enables it.
 * @param[in] handler the pointer to the IRQ service routine
 * @param[in] vector the vector number
 * @param[in] source the IRQ source to be associated to the vector
 */
void SetVICVector(void *handler, int vector, int source) {

  VIC *vicp = VICBase;
  vicp->VIC_VectAddrs[vector] = (IOREG32)handler;
  vicp->VIC_VectCntls[vector] = (IOREG32)(source | 0x20);
}

/** @} */

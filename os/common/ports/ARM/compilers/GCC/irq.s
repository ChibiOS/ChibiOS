/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,
              2011,2012,2013,2014 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    ARM/compilers/GCC/irq.s
 * @brief   IRQ handling for ARM devices.
 *
 * @defgroup ARM_IRQ ARM IRQ Handling
 * @{
 */

#define __FROM_ASM__
#include "armparams.h"

#if !defined(__DOXYGEN__)

                .section irq
                .code   32
                .balign 4

/*
 * IRQ entry point, note this code is always located at address 0x40, the
 * IRQ controller data register offset must be calculated accordingly.
 */
                .global IrqHandler
IrqHandler:
                stmfd   sp!, {r0-r3, r12, lr}
                ldr     r0, =ARM_IRQ_VECTOR_REG
                ldr     r0, [r0]
                ldr     lr, =_port_irq_common
                bx      r0

                .weak   _port_irq_common
_port_irq_common:
                // This exit sequence is usually replaced by the RTOS
                // specific one.
                ldmfd   sp!, {r0-r3, r12, lr}
                subs    pc, lr, #4

#endif

/** @} */

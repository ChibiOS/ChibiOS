/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

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

        MODULE  ?chcoreasm_v6m

        AAPCS INTERWORK, VFP_COMPATIBLE
        PRESERVE8

/*
 * Imports the Cortex-Mx parameters header and performs the same calculations
 * done in chcore.h.
 */
#include "cmparams.h"

#define CORTEX_PRIORITY_MASK(n) ((n) << (8 - CORTEX_PRIORITY_BITS))

EXTCTX_SIZE     SET 32
CONTEXT_OFFSET  SET 12

        SECTION .text:CODE:NOROOT(2)

        EXTERN  chThdExit
        EXTERN  chSchIsRescRequiredExI
        EXTERN  chSchDoRescheduleI
        EXTERN  _port_saved_pc
        EXTERN  _port_irq_nesting

        THUMB

/*
 * Performs a context switch between two threads.
 */
        PUBLIC _port_switch
_port_switch:
        push    {r4, r5, r6, r7, lr}
        mov     r4, r8
        mov     r5, r9
        mov     r6, r10
        mov     r7, r11
        push    {r4, r5, r6, r7}
        mov     r3, sp
        str     r3, [r1, #CONTEXT_OFFSET]
        ldr     r3, [r0, #CONTEXT_OFFSET]
        mov     sp, r3
        pop     {r4, r5, r6, r7}
        mov     r8, r4
        mov     r9, r5
        mov     r10, r6
        mov     r11, r7
        pop     {r4, r5, r6, r7, pc}

/*
 * Start a thread by invoking its work function.
 * If the work function returns @p chThdExit() is automatically invoked.
 */
        PUBLIC  _port_thread_start
_port_thread_start:
        cpsie   i
        mov     r0, r5
        blx     r4
        bl      chThdExit

/*
 * Post-IRQ switch code.
 * Exception handlers return here for context switching.
 */
        PUBLIC  _port_switch_from_isr
_port_switch_from_isr:
        /* Note, saves r4 to make space for the PC.*/
        push    {r0, r1, r2, r3, r4}
        mrs     r0, APSR
        mov     r1, r12
        push    {r0, r1, lr}
        ldr     r0, =_port_saved_pc
        ldr     r0, [r0]
        adds    r0, r0, #1
        str     r0, [sp, #28]
        bl      chSchDoRescheduleI
        pop     {r0, r1, r2}
        mov     r12, r1
        msr     APSR, r0
        mov     lr, r2
        cpsie   i
        pop     {r0, r1, r2, r3, pc}

/*
 * Reschedule verification and setup after an IRQ.
 */
        PUBLIC  _port_irq_epilogue
_port_irq_epilogue:
        push    {r4, lr}
        cpsid   i
        ldr     r2, =_port_irq_nesting
        ldr     r3, [r2]
        subs    r3, r3, #1
        str     r3, [r2]
        cmp     r3, #0
        beq     skipexit
notrequired
        cpsie   i
        pop     {r4, pc}
skipexit
        bl      chSchIsRescRequiredExI
        cmp     r0, #0
        beq     notrequired
        mrs     r1, PSP
        ldr     r2, =_port_saved_pc
        ldr     r3, [r1, #24]
        str     r3, [r2]
        ldr     r3, =_port_switch_from_isr
        str     r3, [r1, #24]
        pop     {r4, pc}

        END

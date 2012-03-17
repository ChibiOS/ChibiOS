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
 * Imports the Cortex-Mx configuration headers.
 */
#define _FROM_ASM_
#include "chconf.h"
#include "chcore.h"

EXTCTX_SIZE     SET     32
CONTEXT_OFFSET  SET     12
SCB_ICSR        SET     0xE000ED04

        SECTION .text:CODE:NOROOT(2)

        EXTERN  chThdExit
        EXTERN  chSchIsRescRequiredExI
        EXTERN  chSchDoRescheduleI

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
 * NMI vector.
 * The NMI vector is used for exception mode re-entering after a context
 * switch.
 */
#if !CORTEX_ALTERNATE_SWITCH
        PUBLIC  NMIVector
NMIVector:
        mrs     r3, PSP
        adds    r3, r3, #32
        msr     PSP, r3
        cpsie   i
        bx      lr
#endif

/*
 * PendSV vector.
 * The PendSV vector is used for exception mode re-entering after a context
 * switch.
 */
#if CORTEX_ALTERNATE_SWITCH
        PUBLIC  PendSVVector
PendSVVector:
        mrs     r3, PSP
        adds    r3, r3, #32
        msr     PSP, r3
        bx      lr
#endif

/*
 * Post-IRQ switch code.
 * Exception handlers return here for context switching.
 */
        PUBLIC  _port_switch_from_isr
_port_switch_from_isr:
        bl      chSchIsRescRequiredExI
        cmp     r0, #0
        beq     noresch
        bl      chSchDoRescheduleI
noresch:
        ldr     r2, =SCB_ICSR
        movs    r3, #128
#if CORTEX_ALTERNATE_SWITCH
        lsls    r3, r3, #21
        str     r3, [r2, #0]
        cpsie   i
#else
        lsls    r3, r3, #24
        str     r3, [r2, #0]
#endif
waithere:
        b       waithere

/*
 * Reschedule verification and setup after an IRQ.
 */
        PUBLIC  _port_irq_epilogue
_port_irq_epilogue:
        push    {lr}
        adds    r0, r0, #15
        beq     skipexit
        cpsid   i
        mrs     r3, PSP
        subs    r3, r3, #32
        msr     PSP, r3
        ldr     r2, =_port_switch_from_isr
        str     r2, [r3, #24]
        movs    r2, #128
        lsls    r2, r2, #17
        str     r2, [r3, #28]
skipexit:
        pop     {pc}

        END

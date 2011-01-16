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

        MODULE  ?chcoreasm_v7m

        AAPCS INTERWORK, VFP_COMPATIBLE
        PRESERVE8

/*
 * Imports the Cortex-Mx parameters header and performs the same calculations
 * done in chcore.h.
 */
#include "cmparams.h"

#define CORTEX_PRIORITY_MASK(n) ((n) << (8 - CORTEX_PRIORITY_BITS))

#ifndef CORTEX_PRIORITY_SVCALL
#define CORTEX_PRIORITY_SVCALL  1
#endif

#ifndef CORTEX_BASEPRI_KERNEL
#define CORTEX_BASEPRI_KERNEL   CORTEX_PRIORITY_MASK(CORTEX_PRIORITY_SVCALL+1)
#endif

#define CORTEX_BASEPRI_DISABLED 0

EXTCTX_SIZE     SET 32
CONTEXT_OFFSET  SET 12
SCB_ICSR        SET 0xE000ED04
ICSR_RETTOBASE  SET 0x00000800

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
        push    {r4, r5, r6, r7, r8, r9, r10, r11, lr}
        str     sp, [r1, #CONTEXT_OFFSET]                          
        ldr     sp, [r0, #CONTEXT_OFFSET]
        pop     {r4, r5, r6, r7, r8, r9, r10, r11, pc}

/*
 * Start a thread by invoking its work function.
 * If the work function returns @p chThdExit() is automatically invoked.
 */
        PUBLIC  _port_thread_start
_port_thread_start:
        movs    r3, #CORTEX_BASEPRI_DISABLED
        msr     BASEPRI, r3
        mov     r0, r5
        blx     r4
        bl      chThdExit

/*
 * Post-IRQ switch code.
 * Exception handlers return here for context switching.
 */
        PUBLIC  _port_switch_from_isr
_port_switch_from_isr:
        bl      chSchDoRescheduleI
        svc     #0

/*
 * Reschedule verification and setup after an IRQ.
 */
        PUBLIC  _port_irq_epilogue
_port_irq_epilogue:
        movs    r3, #CORTEX_BASEPRI_KERNEL
        msr     BASEPRI, r3
        mov     r3, #LWRD SCB_ICSR
        movt    r3, #HWRD SCB_ICSR
        ldr     r3, [r3, #0]
        tst     r3, #ICSR_RETTOBASE
        bne     .L7
        movs    r3, #CORTEX_BASEPRI_DISABLED
        msr     BASEPRI, r3
        bx      lr
.L7:
        push    {r3, lr}
        bl      chSchIsRescRequiredExI
        cmp     r0, #0
        beq     .L4
        mrs     r3, PSP
        subs    r3, r3, #EXTCTX_SIZE
        msr     PSP, r3
        ldr     r2, =_port_switch_from_isr
        str     r2, [r3, #24]
        mov     r2, #0x01000000
        str     r2, [r3, #28]
        pop     {r3, pc}
.L4:
        movs    r3, #CORTEX_BASEPRI_DISABLED
        msr     BASEPRI, r3
        pop     {r3, pc}

/*
 * SVC vector.
 * Discarding the current exception context and positioning the stack to
 * point to the real one.
 */
        PUBLIC  SVCallVector
SVCallVector:
        mrs     r3, PSP
        adds    r3, r3, #EXTCTX_SIZE
        msr     PSP, r3
        movs    r3, #CORTEX_BASEPRI_DISABLED
        msr     BASEPRI, r3
        bx      lr

        END

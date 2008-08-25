/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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

/*
 * Generic ARM startup file for ChibiOS/RT (Atmel variant).
 */

.set    MODE_USR, 0x10
.set    MODE_FIQ, 0x11
.set    MODE_IRQ, 0x12
.set    MODE_SVC, 0x13
.set    MODE_ABT, 0x17
.set    MODE_UND, 0x1B
.set    MODE_SYS, 0x1F

.equ    I_BIT, 0x80
.equ    F_BIT, 0x40

.section .startup
.code 32
.balign 4
/*
 * System entry points.
 */
_start:
        ldr     pc, _reset
        ldr     pc, _undefined
        ldr     pc, _swi
        ldr     pc, _prefetch
        ldr     pc, _abort
        nop
        ldr     pc, [pc,#-0xF20]        /* AIC - AIC_IVR */
        ldr     pc, [pc,#-0xF20]        /* AIC - AIC_FVR */

_reset:
        .word   ResetHandler
_undefined:
        .word   UndHandler
_swi:
        .word   SwiHandler
_prefetch:
        .word   PrefetchHandler
_abort:
        .word   AbortHandler
_fiq:
        .word   FiqHandler
        .word   0
        .word   0

/*
 * Reset handler.
 */
.text
ResetHandler:
        /*
         * Stack pointers initialization.
         */
        ldr     r0, =__ram_end__
        /* Undefined */
        msr     CPSR_c, #MODE_UND | I_BIT | F_BIT
        mov     sp, r0
        ldr     r1, =__und_stack_size__
        sub     r0, r0, r1
        /* Abort */
        msr     CPSR_c, #MODE_ABT | I_BIT | F_BIT
        mov     sp, r0
        ldr     r1, =__abt_stack_size__
        sub     r0, r0, r1
        /* FIQ */
        msr     CPSR_c, #MODE_FIQ | I_BIT | F_BIT
        mov     sp, r0
        ldr     r1, =__fiq_stack_size__
        sub     r0, r0, r1
        /* IRQ */
        msr     CPSR_c, #MODE_IRQ | I_BIT | F_BIT
        mov     sp, r0
        ldr     r1, =__irq_stack_size__
        sub     r0, r0, r1
        /* Supervisor */
        msr     CPSR_c, #MODE_SVC | I_BIT | F_BIT
        mov     sp, r0
        ldr     r1, =__svc_stack_size__
        sub     r0, r0, r1
        /* System */
        msr     CPSR_c, #MODE_SYS | I_BIT | F_BIT
        mov     sp, r0
//        ldr     r1, =__sys_stack_size__
//        sub     r0, r0, r1
        /*
         * Data initialization.
         * NOTE: It assumes that the DATA size is a multiple of 4.
         */
        ldr     r1, =_textdata
        ldr     r2, =_data
        ldr     r3, =_edata
dataloop:
        cmp     r2, r3
        ldrlo   r0, [r1], #4
        strlo   r0, [r2], #4
        blo     dataloop
        /*
         * BSS initialization.
         * NOTE: It assumes that the BSS size is a multiple of 4.
         */
        mov     r0, #0
        ldr     r1, =_bss_start
        ldr     r2, =_bss_end
bssloop:
        cmp     r1, r2
        strlo   r0, [r1], #4
        blo     bssloop
        /*
         * Application-provided HW initialization routine.
         */
#ifndef THUMB_NO_INTERWORKING
        bl      hwinit
        /*
         * main(0, NULL).
         */
        mov     r0, #0
        mov     r1, r0
        bl      main
        bl      chSysHalt
#else
        add     r0, pc, #1
        bx      r0
.code 16
        bl      hwinit
        mov     r0, #0
        mov     r1, r0
        bl      main
        bl      chSysHalt
.code 32
#endif

.weak UndHandler
.globl UndHandler
UndHandler:

.weak SwiHandler
.globl SwiHandler
SwiHandler:

.weak PrefetchHandler
.globl PrefetchHandler
PrefetchHandler:

.weak AbortHandler
.globl AbortHandler
AbortHandler:

.weak FiqHandler
.globl FiqHandler
FiqHandler:

.loop: b        .loop

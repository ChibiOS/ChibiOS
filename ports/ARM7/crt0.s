/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

/*
 * Generic ARM7 startup file for ChibiOS/RT.
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

.text
.code 32
.balign 4

/*
 * Reset handler.
 */
.global ResetHandler
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
         * Early initialization.
         */
#ifndef THUMB_NO_INTERWORKING
        bl      hwinit0
#else
        add     r0, pc, #1
        bx      r0
.code 16
        bl      hwinit0
        mov     r0, pc
        bx      r0
.code 32
#endif
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
         * Late initialization.
         */
#ifndef THUMB_NO_INTERWORKING
        bl      hwinit1
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
        bl      hwinit1
        mov     r0, #0
        mov     r1, r0
        bl      main
        bl      chSysHalt
.code 32
#endif

/*
 * Default exceptions handlers. The handlers are declared weak in order to be
 * replaced by the real handling code.
 */
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

#ifdef THUMB_NO_INTERWORKING
.code 16
#endif

/*
 * Default early initialization code. It is declared weak in order to be
 * replaced by the real initialization code.
 * Early initialization is performed just after reset before BSS and DATA
 * segments initialization.
 */
.global hwinit0
.weak hwinit0
.thumb_func
hwinit0:
        bx      lr

/*
 * Default late initialization code. It is declared weak in order to be
 * replaced by the real initialization code.
 * Late initialization is performed after BSS and DATA segments initialization
 * and before invoking the main() function.
 */
.global hwinit1
.weak hwinit1
.thumb_func
hwinit1:
        bx      lr

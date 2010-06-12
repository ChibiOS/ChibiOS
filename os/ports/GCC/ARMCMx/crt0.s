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
 * @file    ARMCMx/crt0.s
 * @brief   Generic ARM Cortex-Mx startup file for ChibiOS/RT.
 *
 * @addtogroup ARMCMx_CORE
 * @{
 */

#include "cmparams.h"

#if !defined(__DOXYGEN__)

.set    CONTROL_MODE_PRIVILEGED, 0
.set    CONTROL_MODE_UNPRIVILEGED, 1
.set    CONTROL_USE_MSP, 0
.set    CONTROL_USE_PSP, 2

.text
.balign 2
.syntax unified
.thumb

/*
 * Reset handler.
 */
.thumb_func
.global ResetHandler
.weak ResetHandler
ResetHandler:
        /*
         * Interrupts are globally masked initially.
         */
        cpsid   i
        /*
         * Stack pointers initialization.
         */
        ldr     r0, =__ram_end__
        ldr     r1, =__main_stack_size__
        subs    r0, r0, r1
        /*
         * Note that r0 is the main stack low boundary address and process
         * stack initial top address.
         */
        msr     PSP, r0
        /*
         * Early initialization phase, it is empty by default.
         */
        bl      hwinit0
        /*
         * Data initialization.
         * NOTE: It assumes that the DATA size is a multiple of 4.
         */
        ldr     r1, =_textdata
        ldr     r2, =_data
        ldr     r3, =_edata
dloop:
        cmp     r2, r3
#if (CORTEX_MODEL == CORTEX_M0) || (CORTEX_MODEL == CORTEX_M1)
        bge     enddloop
        ldr     r0, [r1]
        str     r0, [r2]
        adds    r1, r1, #4
        adds    r2, r2, #4
        b       dloop
enddloop:
#else
        ittt    lo
        ldrlo   r0, [r1], #4
        strlo   r0, [r2], #4
        blo     dloop
#endif
        /*
         * BSS initialization.
         * NOTE: It assumes that the BSS size is a multiple of 4.
         */
        movs    r0, #0
        ldr     r1, =_bss_start
        ldr     r2, =_bss_end
bloop:
        cmp     r1, r2
#if (CORTEX_MODEL == CORTEX_M0) || (CORTEX_MODEL == CORTEX_M1)
        bge     endbloop
        str     r0, [r1]
        adds    r1, r1, #4
        b       bloop
endbloop:
#else
        itt     lo
        strlo   r0, [r1], #4
        blo     bloop
#endif
        /*
         * Switches to the Process Stack and uses a barrier just to be safe.
         */
        movs    r0, #CONTROL_MODE_PRIVILEGED | CONTROL_USE_PSP
        msr     CONTROL, r0
        isb
        /*
         * Late initialization phase, it is empty by default.
         */
        bl      hwinit1
        movs    r0, #0
        mov     r1, r0
        bl      main
        b       MainExitHandler

/*
 * Default main exit code, just a loop.
 * It is a weak symbol, the application code can redefine the behavior.
 */
.thumb_func
.global MainExitHandler
.weak MainExitHandler
MainExitHandler:
.loop:  b       .loop

/*
 * Default early initialization code. It is declared weak in order to be
 * replaced by the real initialization code.
 * Early initialization is performed just after reset before BSS and DATA
 * segments initialization.
 */
.thumb_func
.global hwinit0
.weak hwinit0
hwinit0:
        bx      lr

/*
 * Default late initialization code. It is declared weak in order to be
 * replaced by the real initialization code.
 * Late initialization is performed after BSS and DATA segments initialization
 * and before invoking the main() function.
 */
.thumb_func
.global hwinit1
.weak hwinit1
hwinit1:
        bx      lr

#endif

/** @} */

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

#include "chconf.h"

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

#ifdef THUMB
.globl chSysLock
chSysLock:
        msr     CPSR_c, #MODE_SYS | I_BIT
        bx      lr

.globl chSysUnlock
chSysUnlock:
        msr     CPSR_c, #MODE_SYS
        bx      lr
#endif

.globl chSysSwitchI
chSysSwitchI:
#ifdef CH_CURRP_REGISTER_CACHE
        stmfd   sp!, {r4, r5, r6, r8, r9, r10, r11, lr}
        str     sp, [r0, #0]
        ldr     sp, [r1, #0]
#ifdef THUMB
        ldmfd   sp!, {r4, r5, r6, r8, r9, r10, r11, lr}
        bx      lr
#else
        ldmfd   sp!, {r4, r5, r6, r8, r9, r10, r11, pc}
#endif
#else
        stmfd   sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
        str     sp, [r0, #0]
        ldr     sp, [r1, #0]
#ifdef THUMB
        ldmfd   sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
        bx      lr
#else
        ldmfd   sp!, {r4, r5, r6, r7, r8, r9, r10, r11, pc}
#endif
#endif /* CH_CURRP_REGISTER_CACHE */

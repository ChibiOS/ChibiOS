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

#include <avr/io.h>

.global threadstart
threadstart:
        sei
        movw    r24, r4         // argument
        movw    r30, r2         // thread function
        icall
        call    chThdExit

.global chSysSwitchI
chSysSwitchI:
        push    r2
        push    r3
        push    r4
        push    r5
        push    r6
        push    r7
        push    r8
        push    r9
        push    r10
        push    r11
        push    r12
        push    r13
        push    r14
        push    r15
        push    r16
        push    r17
        push    r28
        push    r29
        movw    r30, r24        // Z <- oldp
        in      r0, _SFR_IO_ADDR(SPL)
        st      Z, r0
        in      r0, _SFR_IO_ADDR(SPH)
        std     Z+1, r0

        movw    r30, r22        // Z <- newp
        ld      r0, Z
        out     _SFR_IO_ADDR(SPL), r0
        ldd     r0, Z+1
        out     _SFR_IO_ADDR(SPH), r0
        pop     r29
        pop     r28
        pop     r17
        pop     r16
        pop     r15
        pop     r14
        pop     r13
        pop     r12
        pop     r11
        pop     r10
        pop     r9
        pop     r8
        pop     r7
        pop     r6
        pop     r5
        pop     r4
        pop     r3
        pop     r2
        ret

.global __vector_17
__vector_17:
        push    r0
        push    r1
        push    r18
        push    r19
        push    r20
        push    r21
        push    r22
        push    r23
        push    r24
        push    r25
        push    r26
        push    r27
        push    r30
        push    r31
        in      r0, _SFR_IO_ADDR(SREG)
        push    r0
        clr     r1
        call    chSchTimerHandlerI
intcommon:
        call    chSchRescRequiredI
        tst     r24
        breq    noschd
        call    chSchDoRescheduleI
noschd:
        pop     r0
        out     _SFR_IO_ADDR(SREG), r0
        pop     r31
        pop     r30
        pop     r27
        pop     r26
        pop     r25
        pop     r24
        pop     r23
        pop     r22
        pop     r21
        pop     r20
        pop     r19
        pop     r18
        pop     r1
        pop     r0
        reti

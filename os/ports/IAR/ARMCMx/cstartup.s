/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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

        MODULE  ?cstartup

CONTROL_MODE_PRIVILEGED SET 0
CONTROL_MODE_UNPRIVILEGED SET 1
CONTROL_USE_MSP SET 0
CONTROL_USE_PSP SET 2

        AAPCS INTERWORK, VFP_COMPATIBLE, ROPI
        PRESERVE8

        SECTION PSTACK:DATA:NOROOT(3)
        SECTION .intvec:CODE:NOROOT(3)

        PUBLIC  __iar_program_start
        EXTERN  __vector_table
        EXTWEAK __iar_init_core
        EXTWEAK __iar_init_vfp

        SECTION .text:CODE:REORDER(2)
        THUMB
__iar_program_start:
        cpsid   i
        ldr     r0, =sfe(PSTACK)
        msr     PSP, r0
        movs    r0, #CONTROL_MODE_PRIVILEGED | CONTROL_USE_PSP
        msr     CONTROL, r0
        isb
        bl      hwinit0
        bl      __iar_init_core
        bl      __iar_init_vfp

; Replicated cmain.s here in order to insert the call to hwinit1.
        EXTERN  main
        EXTERN  _exit
        EXTWEAK __low_level_init
        EXTWEAK __iar_data_init3
        bl      __low_level_init
        cmp     r0, #0
        beq.n   _skipinit
        bl      __iar_data_init3
_skipinit:
        bl      hwinit1
        bl      main
        bl      _exit

        PUBWEAK hwinit0
hwinit0:
        bx      lr

        PUBWEAK hwinit1
hwinit1:
        bx      lr

        REQUIRE __vector_table

        END

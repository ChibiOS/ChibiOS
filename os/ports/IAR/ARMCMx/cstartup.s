/**************************************************
 *
 * Part one of the system initialization code, contains low-level
 * initialization, plain thumb variant.
 *
 * Copyright 2008 IAR Systems. All rights reserved.
 *
 * $Revision: 34775 $
 *
 **************************************************/

;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

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
        beq.n   _call_main
        bl      __iar_data_init3
_call_main:
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

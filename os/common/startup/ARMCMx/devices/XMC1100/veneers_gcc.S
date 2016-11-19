/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    veneers_gcc.S
 * @brief   XMC1100 veneers for GCC.
 *
 * @addtogroup ARMCMx_XMC1100_VENEERS_GCC
 * @{
 */

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Code section.                                                             */
/*===========================================================================*/

#if !defined(__DOXYGEN__)

                .syntax     unified
                .cpu        cortex-m0

                .thumb

                .section    .vectors, "ax", %progbits
                .align      4
                .globl      _vectors
_vectors:
                .long       __main_stack_end__, __reset, NMI_Handler, HardFault_Handler
                .long       __clk_val1, __clk_val2, UsageFault_Handler, Vector1C
                .long       Vector20, Vector24, Vector28, SVC_Handler
                .long       DebugMon_Handler, Vector34, PendSV_Handler, SysTick_Handler
                .long       Vector40, Vector44, Vector48, Vector4C
                .long       Vector50, Vector54, Vector58, Vector5C
                .long       Vector60, Vector64, Vector68, Vector6C
                .long       Vector70, Vector74, Vector78, Vector7C
                .long       Vector80, Vector84, Vector88, Vector8C
                .long       Vector90, Vector94, Vector98, Vector9C
                .long       VectorA0, VectorA4, VectorA8, VectorAC
                .long       VectorB0, VectorB4, VectorB8, VectorBC

                .align  2
                .thumb_func
                .weak       HardFault_Handler, UsageFault_Handler, Vector1C
                .weak       Vector20, Vector24, Vector28, SVC_Handler
                .weak       DebugMon_Handler, Vector34, PendSV_Handler, SysTick_Handler
                .weak       Vector40, Vector44, Vector48, Vector4C
                .weak       Vector50, Vector54, Vector58, Vector5C
                .weak       Vector60, Vector64, Vector68, Vector6C
                .weak       Vector70, Vector74, Vector78, Vector7C
                .weak       Vector80, Vector84, Vector88, Vector8C
                .weak       Vector90, Vector94, Vector98, Vector9C
                .weak       VectorA0, VectorA4, VectorA8, VectorAC
                .weak       VectorB0, VectorB4, VectorB8, VectorBC
HardFault_Handler:
UsageFault_Handler:
Vector1C:
Vector20:
Vector24:
Vector28:
SVC_Handler:
DebugMon_Handler:
Vector34:
PendSV_Handler:
SysTick_Handler:
Vector40:
Vector44:
Vector48:
Vector4C:
Vector50:
Vector54:
Vector58:
Vector5C:
Vector60:
Vector64:
Vector68:
Vector6C:
Vector70:
Vector74:
Vector78:
Vector7C:
Vector80:
Vector84:
Vector88:
Vector8C:
Vector90:
Vector94:
Vector98:
Vector9C:
VectorA0:
VectorA4:
VectorA8:
VectorAC:
VectorB0:
VectorB4:
VectorB8:
VectorBC:

                b           _unhandled_exception

                .align  2
                .thumb_func
                .weak       _unhandled_exception
_unhandled_exception:
                b           _unhandled_exception

                .text
                .align  2
                .thumb_func
                .global __reset
__reset:        sub     sp, #32
                movs    r0, #0
                str     r0, [sp, #4]
                str     r0, [sp, #8]
                str     r0, [sp, #12]
                str     r0, [sp, #16]
                str     r0, [sp, #20]
                ldr     r0, =Reset_Handler
                str     r0, [sp, #24]
                ldr     r0, =0x01000000
                str     r0, [sp, #28]
                ldr     r0, =0xFFFFFFF9
                bx      r0

                .section    .ram0_init, "ax", %progbits
                .align      2
                .thumb_func
                .global     __veneers
__veneers:      movs        r0, #0
                b           .
                movs        r0, #1
                b           .
                movs        r0, #2
                b           .
                ldr         r0, =HardFault_Handler
                mov         pc, r0
                movs        r0, #4
                b           .
                movs        r0, #5
                b           .
                ldr         r0, =UsageFault_Handler
                mov         pc, r0
                movs        r0, #7
                b           .
                movs        r0, #8
                b           .
                movs        r0, #9
                b           .
                movs        r0, #10
                b           .
                ldr         r0, =SVC_Handler
                mov         pc, r0
                ldr         r0, =DebugMon_Handler
                mov         pc, r0
                movs        r0, #14
                b           .
                ldr         r0, =PendSV_Handler
                mov         pc, r0
                ldr         r0, =SysTick_Handler
                mov         pc, r0
                ldr         r0, =Vector40
                mov         pc, r0
                ldr         r0, =Vector44
                mov         pc, r0
                ldr         r0, =Vector48
                mov         pc, r0
                ldr         r0, =Vector4C
                mov         pc, r0
                ldr         r0, =Vector50
                mov         pc, r0
                ldr         r0, =Vector54
                mov         pc, r0
                ldr         r0, =Vector58
                mov         pc, r0
                ldr         r0, =Vector5C
                mov         pc, r0
                ldr         r0, =Vector60
                mov         pc, r0
                ldr         r0, =Vector64
                mov         pc, r0
                ldr         r0, =Vector68
                mov         pc, r0
                ldr         r0, =Vector6C
                mov         pc, r0
                ldr         r0, =Vector70
                mov         pc, r0
                ldr         r0, =Vector74
                mov         pc, r0
                ldr         r0, =Vector78
                mov         pc, r0
                ldr         r0, =Vector7C
                mov         pc, r0
                ldr         r0, =Vector80
                mov         pc, r0
                ldr         r0, =Vector84
                mov         pc, r0
                ldr         r0, =Vector88
                mov         pc, r0
                ldr         r0, =Vector8C
                mov         pc, r0
                ldr         r0, =Vector90
                mov         pc, r0
                ldr         r0, =Vector94
                mov         pc, r0
                ldr         r0, =Vector98
                mov         pc, r0
                ldr         r0, =Vector9C
                mov         pc, r0
                ldr         r0, =VectorA0
                mov         pc, r0
                ldr         r0, =VectorA4
                mov         pc, r0
                ldr         r0, =VectorA8
                mov         pc, r0
                ldr         r0, =VectorAC
                mov         pc, r0
                ldr         r0, =VectorB0
                mov         pc, r0
                ldr         r0, =VectorB4
                mov         pc, r0
                ldr         r0, =VectorB8
                mov         pc, r0
                ldr         r0, =VectorBC
                mov         pc, r0

#endif /* !defined(__DOXYGEN__) */

/** @} */

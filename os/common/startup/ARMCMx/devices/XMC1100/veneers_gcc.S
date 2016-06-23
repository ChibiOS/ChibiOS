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
 * @file    veneers_gcc.s
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

                .syntax unified
                .cpu    cortex-m0

                .thumb
                .section ram0_init

                .align  2
                .global __veneers
__veneers:
                .long       0
                .long       0
                ldr         r0, =NMI_Handler
                mov         pc, r0
                ldr         r0, =HardFault_Handler
                mov         pc, r0
                ldr         r0, =MemManage_Handler
                mov         pc, r0
                ldr         r0, =BusFault_Handler
                mov         pc, r0
                ldr         r0, =UsageFault_Handler
                mov         pc, r0
                .long       0
                .long       0
                .long       0
                .long       0
                ldr         r0, =SVC_Handler
                mov         pc, r0
                ldr         r0, =DebugMon_Handler
                mov         pc, r0
                .long       0
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

/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    ARMvx-M-SB/compilers/GCC/vectors.S
 * @brief   Interrupt vectors for Cortex-Mx devices.
 *
 * @defgroup ARMCMx_GCC_VECTORS Cortex-Mx Interrupt Vectors
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
        .cpu    cortex-m3
        .thumb

        .text

        .align      2
        .globl      __crt0_vrq
__crt0_vrq:
        ldr.w       r1, =__vectors
        ldr.w       r1, [r1, r0, lsl #2]
        blx         r1
        svc         #255

        .align      2
        .globl      _vectors
__vectors:
        .long       Vector00,   Vector04,   Vector08,   Vector0C
        .long       Vector10,   Vector14,   Vector18,   Vector1C
        .long       Vector20,   Vector24,   Vector28,   Vector2C
        .long       Vector30,   Vector34,   Vector38,   Vector3C
        .long       Vector40,   Vector44,   Vector48,   Vector4C
        .long       Vector50,   Vector54,   Vector58,   Vector5C
        .long       Vector60,   Vector64,   Vector68,   Vector6C
        .long       Vector70,   Vector74,   Vector78,   Vector7C

        .align      2
        .weak       Vector00,   Vector04,   Vector08,   Vector0C
        .weak       Vector10,   Vector14,   Vector18,   Vector1C
        .weak       Vector20,   Vector24,   Vector28,   Vector2C
        .weak       Vector30,   Vector34,   Vector38,   Vector3C
        .weak       Vector40,   Vector44,   Vector48,   Vector4C
        .weak       Vector50,   Vector54,   Vector58,   Vector5C
        .weak       Vector60,   Vector64,   Vector68,   Vector6C
        .weak       Vector70,   Vector74,   Vector78,   Vector7C

        .thumb_func
Vector00:
        .thumb_func
Vector04:
        .thumb_func
Vector08:
        .thumb_func
Vector0C:
        .thumb_func
Vector10:
        .thumb_func
Vector14:
        .thumb_func
Vector18:
        .thumb_func
Vector1C:
        .thumb_func
Vector20:
        .thumb_func
Vector24:
        .thumb_func
Vector28:
        .thumb_func
Vector2C:
        .thumb_func
Vector30:
        .thumb_func
Vector34:
        .thumb_func
Vector38:
        .thumb_func
Vector3C:
        .thumb_func
Vector40:
        .thumb_func
Vector44:
        .thumb_func
Vector48:
        .thumb_func
Vector4C:
        .thumb_func
Vector50:
        .thumb_func
Vector54:
        .thumb_func
Vector58:
        .thumb_func
Vector5C:
        .thumb_func
Vector60:
        .thumb_func
Vector64:
        .thumb_func
Vector68:
        .thumb_func
Vector6C:
        .thumb_func
Vector70:
        .thumb_func
Vector74:
        .thumb_func
Vector78:
        .thumb_func
Vector7C:
        bl          __unhandled_vrq

        .thumb_func
        .weak       __unhandled_vrq
__unhandled_vrq:
        svc         #255
.stay:  b           .stay

#endif /* !defined(__DOXYGEN__) */

/** @} */

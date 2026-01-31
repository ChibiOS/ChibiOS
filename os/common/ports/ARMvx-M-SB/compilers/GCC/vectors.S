/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    ARMvx-M-SB/compilers/GCC/vectors.S
 * @brief   Sandbox VRQ vectors.
 *
 * @defgroup ARMVXM_SB_GCC_VECTORS Sandbox VRQ vectors
 * @{
 */

#if !defined(FALSE) || defined(__DOXYGEN__)
#define FALSE   0
#endif

#if !defined(TRUE) || defined(__DOXYGEN__)
#define TRUE    1
#endif

#if !defined(SB_VECTORS_NORTOS)
#define _FROM_ASM_
#include "chlicense.h"
#include "chconf.h"
#endif

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


        .bss

        .align      2
        .globl      __sb_in_vrq
__sb_in_vrq:
        .ds.l       1

        .text

        .align      2
        .globl      __crt0_vrq
__crt0_vrq:
        ldr.w       r3, =__sb_in_vrq
        mov.w       r2, #1
        str.w       r2, [r3]

        ldr.w       r2, =__sb_vectors
        ldr.w       r2, [r2, r0, lsl #2]
        blx         r2

        ldr.w       r3, =__sb_in_vrq
        mov.w       r2, #0
        str.w       r2, [r3]

#if !defined(SB_VECTORS_NORTOS)
#if CH_DBG_STATISTICS
        bl          __stats_start_measure_crit_thd
#endif
#if CH_DBG_SYSTEM_STATE_CHECK
        bl          __dbg_check_lock
#endif
        bl          chSchPreemption
#if CH_DBG_SYSTEM_STATE_CHECK
        bl          __dbg_check_unlock
#endif
#if CH_DBG_STATISTICS
        bl          __stats_stop_measure_crit_thd
#endif
#endif

        svc         #127    /* SB_FASTC_VRQ_RETURN */

        .align      2
        .globl      __sb_vectors
__sb_vectors:
        .long       __sb_vector0,  __sb_vector1,  __sb_vector2,  __sb_vector3
        .long       __sb_vector4,  __sb_vector5,  __sb_vector6,  __sb_vector7
        .long       __sb_vector8,  __sb_vector9,  __sb_vector10, __sb_vector11
        .long       __sb_vector12, __sb_vector13, __sb_vector14, __sb_vector15
        .long       __sb_vector16, __sb_vector17, __sb_vector18, __sb_vector19
        .long       __sb_vector20, __sb_vector21, __sb_vector22, __sb_vector23
        .long       __sb_vector24, __sb_vector25, __sb_vector26, __sb_vector27
        .long       __sb_vector28, __sb_vector29, __sb_vector30, __sb_vector31

        .align      2
        .weak       __sb_vector0,  __sb_vector1,  __sb_vector2,  __sb_vector3
        .weak       __sb_vector4,  __sb_vector5,  __sb_vector6,  __sb_vector7
        .weak       __sb_vector8,  __sb_vector9,  __sb_vector10, __sb_vector11
        .weak       __sb_vector12, __sb_vector13, __sb_vector14, __sb_vector15
        .weak       __sb_vector16, __sb_vector17, __sb_vector18, __sb_vector19
        .weak       __sb_vector20, __sb_vector21, __sb_vector22, __sb_vector23
        .weak       __sb_vector24, __sb_vector25, __sb_vector26, __sb_vector27
        .weak       __sb_vector28, __sb_vector29, __sb_vector30, __sb_vector31

        .thumb_func
__sb_vector0:
        .thumb_func
__sb_vector1:
        .thumb_func
__sb_vector2:
        .thumb_func
__sb_vector3:
        .thumb_func
__sb_vector4:
        .thumb_func
__sb_vector5:
        .thumb_func
__sb_vector6:
        .thumb_func
__sb_vector7:
        .thumb_func
__sb_vector8:
        .thumb_func
__sb_vector9:
        .thumb_func
__sb_vector10:
        .thumb_func
__sb_vector11:
        .thumb_func
__sb_vector12:
        .thumb_func
__sb_vector13:
        .thumb_func
__sb_vector14:
        .thumb_func
__sb_vector15:
        .thumb_func
__sb_vector16:
        .thumb_func
__sb_vector17:
        .thumb_func
__sb_vector18:
        .thumb_func
__sb_vector19:
        .thumb_func
__sb_vector20:
        .thumb_func
__sb_vector21:
        .thumb_func
__sb_vector22:
        .thumb_func
__sb_vector23:
        .thumb_func
__sb_vector24:
        .thumb_func
__sb_vector25:
        .thumb_func
__sb_vector26:
        .thumb_func
__sb_vector27:
        .thumb_func
__sb_vector28:
        .thumb_func
__sb_vector29:
        .thumb_func
__sb_vector30:
        .thumb_func
__sb_vector31:
        bl          __unhandled_vrq

        .thumb_func
        .weak       __unhandled_vrq
__unhandled_vrq:
        /* TODO terminate SB???*/
        svc         #127    /* SB_FASTC_VRQ_RETURN */
.stay:  b           .stay

#endif /* !defined(__DOXYGEN__) */

/** @} */

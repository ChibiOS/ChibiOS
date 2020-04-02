/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    compilers/GCC/chcoreasm_v7m.S
 * @brief   ARMv7-M architecture port low level code.
 *
 * @addtogroup ARMCMx_GCC_CORE
 * @{
 */

#if !defined(FALSE) || defined(__DOXYGEN__)
#define FALSE   0
#endif

#if !defined(TRUE) || defined(__DOXYGEN__)
#define TRUE    1
#endif

#define _FROM_ASM_
#include "chlicense.h"
#include "chconf.h"
#include "chcore.h"

#if !defined(__DOXYGEN__)

/*
 * RTOS-specific context offset.
 */
#if defined(_CHIBIOS_RT_CONF_)
#if CH_CFG_USE_REGISTRY
#define CURRENT_OFFSET  20          /* ch.rlist.current */
#define CONTEXT_OFFSET  20
#else
#define CURRENT_OFFSET  12
#define CONTEXT_OFFSET  12
#endif

#elif defined(_CHIBIOS_NIL_CONF_)
#define CURRENT_OFFSET  0           /* nil.current */
#define CONTEXT_OFFSET  0

#else
#error "invalid chconf.h"
#endif

/* MPU-related constants.*/
#define MPU_RBAR        0xE000ED9C

/* Other constants.*/
#define SCB_ICSR        0xE000ED04
#define ICSR_PENDSVSET  0x10000000

                .syntax unified
                .cpu    cortex-m4
#if CORTEX_USE_FPU
                .fpu    fpv4-sp-d16
#else
                .fpu    softvfp
#endif

                .thumb
                .text

/*--------------------------------------------------------------------------*
 * Performs a context switch between two threads.
 *--------------------------------------------------------------------------*/
                .thumb_func
                .globl  SVC_Handler
SVC_Handler:
                /* Saving callee context of thread being swapped out.*/
                adds    r1, #CONTEXT_OFFSET
                mrs     r2, PSP
                mrs     r3, BASEPRI
//                movs    r3, #(2 << 5)
#if CH_DBG_ENABLE_STACK_CHECK
                mrs     r12, PSPLIM
                stmia   r1!, {r2-r12,lr}
#else
                stmia   r1!, {r2-r11,lr}
#endif
#if CORTEX_USE_FPU
                vstmia  r1, {s16-s31}
#endif

                /* Restoring calle context of thread being swapped in.*/
                adds    r0, #CONTEXT_OFFSET
#if CH_DBG_ENABLE_STACK_CHECK
                ldmia   r0!, {r2-r12, lr}
                msr     PSPLIM, r12
#else
                ldmia   r0!, {r2-r11, lr}
#endif
                msr     BASEPRI, r3
                msr     PSP, r2
#if CORTEX_USE_FPU
                vldmia  r0, {s16-s31}
#endif
                bx      lr

/*--------------------------------------------------------------------------*
 * Tail preemption check using PENDSV.
 *--------------------------------------------------------------------------*/
                .thumb_func
                .globl  PendSV_Handler
PendSV_Handler:
                /* Pointer to the current thread.*/
                ldr     r1, =ch
                ldr     r1, [r1, #CURRENT_OFFSET]

                /* Saving callee context of thread being swapped out.*/
                adds    r1, #CONTEXT_OFFSET
                mrs     r2, PSP
                mrs     r3, BASEPRI
#if CH_DBG_ENABLE_STACK_CHECK
                mrs     r12, PSPLIM
                stmia   r1!, {r2-r12,lr}
#else
                stmia   r1!, {r2-r11,lr}
#endif
#if CORTEX_USE_FPU
                vstmia  r1, {s16-s31}
#endif

                /* Selecting the thread to be swapped in.*/
                bl      port_schedule_next

                /* Restoring calle context of thread being swapped in.*/
                adds    r0, #CONTEXT_OFFSET
#if CH_DBG_ENABLE_STACK_CHECK
                ldmia   r0!, {r2-r12, lr}
                msr     PSPLIM, r12
#else
                ldmia   r0!, {r2-r11, lr}
#endif
                msr     BASEPRI, r3
                msr     PSP, r2
#if CORTEX_USE_FPU
                vldmia  r0, {s16-s31}
#endif
                bx      lr

/*--------------------------------------------------------------------------*
 * Start a thread by invoking its work function.
 *
 * Threads execution starts here, the code leaves the system critical zone
 * and then jumps into the thread function passed in register R4. The
 * register R5 contains the thread parameter. The function chThdExit() is
 * called on thread function return.
 *--------------------------------------------------------------------------*/
                .thumb_func
                .globl  __port_thread_start
__port_thread_start:
#if CH_DBG_ENABLE_STACK_CHECK && PORT_ENABLE_GUARD_PAGES
                bl      _port_set_region
#endif
#if CH_DBG_STATISTICS
                bl      _stats_stop_measure_crit_thd
#endif
                movs    r3, #0              /* CORTEX_BASEPRI_DISABLED */
                msr     BASEPRI, r3
                mov     r0, r5
                blx     r4
                movs    r0, #0              /* MSG_OK */
                bl      chThdExit
_zombies:       b       _zombies

#endif /* !defined(__DOXYGEN__) */

/** @} */

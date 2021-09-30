/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

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
 * @file    ARMv7-M-ALT/compilers/GCC/chcoreasm.S
 * @brief   ARMv7-M (alternate) architecture port low level code.
 *
 * @addtogroup ARMV7M_ALT_GCC_CORE
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
#define CURRENT_OFFSET  12
#define CONTEXT_OFFSET  12

#elif defined(_CHIBIOS_NIL_CONF_)
#define CURRENT_OFFSET  0           /* nil.current */
#define CONTEXT_OFFSET  0

#else
#error "invalid chconf.h"
#endif

/* MPU-related constants.*/
#define MPU_RNR         0xE000ED98
#define MPU_RBAR        0xE000ED9C

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
 * Context switch macros depending on various options.
 *--------------------------------------------------------------------------*/

                /* Store integer context through R1.*/
                .macro  PORT_STORE_INTEGER_CONTEXT
                mrs     r2, PSP
                mrs     r3, BASEPRI
                stmia   r1!, {r2-r11,lr}
                .endm

                /* Load integer context through R0.*/
                .macro  PORT_LOAD_INTEGER_CONTEXT
                ldmia   r0!, {r2-r11, lr}
                msr     PSP, r2
                msr     BASEPRI, r3
                .endm

#if CORTEX_USE_FPU
                /* Store float context through R1.*/
                .macro  PORT_STORE_FLOAT_CONTEXT
                vstmia  r1!, {s16-s31}
                .endm

                /* Load float context through R0.*/
                .macro  PORT_LOAD_FLOAT_CONTEXT
                vldmia  r0!, {s16-s31}
                .endm
#else
                .macro  PORT_STORE_FLOAT_CONTEXT
                .endm

                .macro  PORT_LOAD_FLOAT_CONTEXT
                .endm
#endif

                /* Store MPU context through R1.*/
                .macro  PORT_STORE_MPU_CONTEXT
#if PORT_SWITCHED_REGIONS_NUMBER == 1
                ldr     r2, =MPU_RBAR
                mov     r3, #0
                str     r3, [r2, #-4]       /* RNR */
                ldm     r2, {r4, r5}        /* RBAR0, RASR0 */
                stmia   r1!, {r4-r5}
#endif
#if PORT_SWITCHED_REGIONS_NUMBER == 2
                ldr     r2, =MPU_RBAR
                mov     r3, #0
                str     r3, [r2, #-4]       /* RNR */
                ldm     r2, {r4, r5}        /* RBAR0, RASR0 */
                add     r3, #1
                str     r3, [r2, #-4]       /* RNR */
                ldm     r2, {r6, r7}        /* RBAR1, RASR1 */
                stmia   r1!, {r4-r7}
#endif
#if PORT_SWITCHED_REGIONS_NUMBER == 3
                ldr     r2, =MPU_RBAR
                mov     r3, #0
                str     r3, [r2, #-4]       /* RNR */
                ldm     r2, {r4, r5}        /* RBAR0, RASR0 */
                add     r3, #1
                str     r3, [r2, #-4]       /* RNR */
                ldm     r2, {r6, r7}        /* RBAR1, RASR1 */
                add     r3, #1
                str     r3, [r2, #-4]       /* RNR */
                ldm     r2, {r8, r9}        /* RBAR2, RASR2 */
                stmia   r1!, {r4-r9}
#endif
#if PORT_SWITCHED_REGIONS_NUMBER == 4
                ldr     r2, =MPU_RBAR
                mov     r3, #0
                str     r3, [r2, #-4]       /* RNR */
                ldm     r2, {r4, r5}        /* RBAR0, RASR0 */
                add     r3, #1
                str     r3, [r2, #-4]       /* RNR */
                ldm     r2, {r6, r7}        /* RBAR1, RASR1 */
                add     r3, #1
                str     r3, [r2, #-4]       /* RNR */
                ldm     r2, {r8, r9}        /* RBAR2, RASR2 */
                add     r3, #1
                str     r3, [r2, #-4]       /* RNR */
                ldm     r2, {r10, r11}      /* RBAR3, RASR3 */
                stmia   r1!, {r4-r11}
#endif
                .endm

                /* Load MPU context through R0.*/
                .macro  PORT_LOAD_MPU_CONTEXT
#if PORT_SWITCHED_REGIONS_NUMBER == 1
                ldr     r2, =MPU_RNR
                mov     r3, #0
                ldmia   r0!, {r4-r5}
                stm     r2, {r3, r4-r5}     /* RNR, RBAR0, RASR0 */
#endif
#if PORT_SWITCHED_REGIONS_NUMBER == 2
                ldr     r2, =MPU_RNR
                mov     r3, #0
                ldmia   r0!, {r4-r7}
                stm     r2, {r3, r4-r5}     /* RNR, RBAR0, RASR0 */
                add     r3, #1
                stm     r2, {r3, r6-r7}     /* RNR, RBAR1, RASR1 */
#endif
#if PORT_SWITCHED_REGIONS_NUMBER == 3
                ldr     r2, =MPU_RNR
                mov     r3, #0
                ldmia   r0!, {r4-r9}
                stm     r2, {r3, r4-r5}     /* RNR, RBAR0, RASR0 */
                add     r3, #1
                stm     r2, {r3, r6-r7}     /* RNR, RBAR1, RASR1 */
                add     r3, #1
                stm     r2, {r3, r8-r9}     /* RNR, RBAR2, RASR2 */
#endif
#if PORT_SWITCHED_REGIONS_NUMBER == 4
                ldr     r2, =MPU_RNR
                mov     r3, #0
                ldmia   r0!, {r4-r11}
                stm     r2, {r3, r4-r5}     /* RNR, RBAR0, RASR0 */
                add     r3, #1
                stm     r2, {r3, r6-r7}     /* RNR, RBAR1, RASR1 */
                add     r3, #1
                stm     r2, {r3, r8-r9}     /* RNR, RBAR2, RASR2 */
                add     r3, #1
                stm     r2, {r3, r10-r11}   /* RNR, RBAR3, RASR3 */
#endif
                .endm

/*--------------------------------------------------------------------------*
 * Performs a context switch between two threads using SVC.
 *--------------------------------------------------------------------------*/
                .thumb_func
                .globl  SVC_Handler
SVC_Handler:
                /* Context store for old thread.*/
                adds    r1, #CONTEXT_OFFSET
                PORT_STORE_INTEGER_CONTEXT
                PORT_STORE_FLOAT_CONTEXT
                PORT_STORE_MPU_CONTEXT

                /* Context load for new thread.*/
                adds    r0, #CONTEXT_OFFSET
                PORT_LOAD_INTEGER_CONTEXT
                PORT_LOAD_FLOAT_CONTEXT
                PORT_LOAD_MPU_CONTEXT

                bx      lr

/*--------------------------------------------------------------------------*
 * Tail preemption check using PENDSV.
 *--------------------------------------------------------------------------*/
                .thumb_func
                .globl  PendSV_Handler
PendSV_Handler:
                push    {lr}
                bl      port_schedule_next
                cmp     r0, #0
                it      eq
                popeq   {pc}
                pop     {lr}

                /* Context store for old thread.*/
                adds    r1, #CONTEXT_OFFSET
                PORT_STORE_INTEGER_CONTEXT
                PORT_STORE_FLOAT_CONTEXT
                PORT_STORE_MPU_CONTEXT

                /* Context load for new thread.*/
                adds    r0, #CONTEXT_OFFSET
                PORT_LOAD_INTEGER_CONTEXT
                PORT_LOAD_FLOAT_CONTEXT
                PORT_LOAD_MPU_CONTEXT

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
#if CH_DBG_STATISTICS
                bl      __stats_stop_measure_crit_thd
#endif
                movs    r3, #CORTEX_BASEPRI_DISABLED
                msr     BASEPRI, r3
                mov     r0, r5
                blx     r4
                movs    r0, #0              /* MSG_OK */
                bl      chThdExit
.zombies:       b       .zombies

#endif /* !defined(__DOXYGEN__) */

/** @} */

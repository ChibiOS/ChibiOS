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
 * @file    ARMv8-M-ML-ALT/compilers/GCC/chcoreasm.S
 * @brief   ARMv8-M MainLine (alt) architecture port low level code.
 *
 * @addtogroup ARMV8M_ML_ALT_GCC_CORE
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
#define MPU_RLAR        0xE000EDA0

/* Calculating the MPU regions array offset within the port_context
   structure.*/
#if CORTEX_USE_FPU
  #if PORT_SAVE_PSPLIM && PORT_SAVE_CONTROL
    #define MPU_OFFSET  116
  #elif PORT_SAVE_PSPLIM || PORT_SAVE_CONTROL
    #define MPU_OFFSET  112
  #else
    #define MPU_OFFSET  108
  #endif
#else
  #if PORT_SAVE_PSPLIM && PORT_SAVE_CONTROL
    #define MPU_OFFSET  52
  #elif PORT_SAVE_PSPLIM || PORT_SAVE_CONTROL
    #define MPU_OFFSET  48
  #else
    #define MPU_OFFSET  44
  #endif
#endif

                .syntax unified
                .cpu    cortex-m33
#if CORTEX_USE_FPU
                .fpu    fpv5-sp-d16
#else
                .fpu    softvfp
#endif

                .thumb
                .text

/*--------------------------------------------------------------------------*
 * Context switch macros depending on various options.
 *--------------------------------------------------------------------------*/

                /* Store integer context through R1.
                   On entry R2=PSP, R3=BASEPRI.*/
                .macro  PORT_STORE_INTEGER_CONTEXT
                stmia   r1!, {r2-r11,lr}
                .endm

                /* Load integer context through R0.*/
                .macro  PORT_LOAD_INTEGER_CONTEXT
                ldmia   r0!, {r2-r11,lr}
                msr     PSP, r2
                msr     BASEPRI, r3
                .endm

                /* Store special registers context through R1.
                   On entry R12=CONTROL.*/
                .macro  PORT_STORE_SPECIAL_CONTEXT
#if PORT_SAVE_PSPLIM
                mrs     r2, PSPLIM
#if PORT_SAVE_CONTROL
                stmia   r1!, {r2, r12}
#else
                stmia   r1!, {r2}
#endif
#else /* !PORT_SAVE_PSPLIM */
#if PORT_SAVE_CONTROL
                stmia   r1!, {r12}
#else
                /* Nothing.*/
#endif
#endif /* !PORT_SAVE_PSPLIM */
                .endm

                /* Load special context through R0.*/
                .macro  PORT_LOAD_SPECIAL_CONTEXT
#if PORT_SAVE_PSPLIM
#if PORT_SAVE_CONTROL
                ldmia   r0!, {r2, r12}
                msr     PSPLIM, r2
                msr     CONTROL, r12
#else
                ldmia   r0!, {r2}
                msr     PSPLIM, r2
#endif
#else /* !PORT_SAVE_PSPLIM */
#if PORT_SAVE_CONTROL
                ldmia   r0!, {r12}
                msr     CONTROL, r12
#else
                /* Nothing.*/
#endif
#endif /* !PORT_SAVE_PSPLIM */
                .endm

                /* Store float context through R1.*/
                .macro  PORT_STORE_FLOAT_CONTEXT
#if CORTEX_USE_FPU
#if PORT_USE_FPU_FAST_SWITCHING > 2
                tst     lr, #16
                it      eq
                vstmiaeq r1!, {s16-s31}
#else
                vstmia  r1!, {s16-s31}
#endif
#endif
                .endm

                /* Load float context through R0.*/
                .macro  PORT_LOAD_FLOAT_CONTEXT
#if CORTEX_USE_FPU
#if PORT_USE_FPU_FAST_SWITCHING > 2
                tst     lr, #16
                it      eq
                vldmiaeq r0!, {s16-s31}
#else
                vldmia  r0!, {s16-s31}
#endif
#endif
                .endm

                /* Store MPU context through R1.*/
                .macro  PORT_STORE_MPU_CONTEXT
#if PORT_SWITCHED_REGIONS_NUMBER > 0
                ldr     r2, =MPU_RBAR
                mov     r3, #0
                str     r3, [r2, #-4]       /* RNR */
#if PORT_SWITCHED_REGIONS_NUMBER == 1
                ldm     r2, {r4-r5}         /* RBAR0, RLAR0 */
                stmia   r1!, {r4-r5}
#elif PORT_SWITCHED_REGIONS_NUMBER == 2
                ldm     r2, {r4-r7}         /* RBAR0, RLAR0 ... RBAR1, RLAR1 */
                stmia   r1!, {r4-r7}        /* RBAR0, RLAR0 ... RBAR1, RLAR1 */
#elif PORT_SWITCHED_REGIONS_NUMBER == 3
                ldm     r2, {r4-r9}         /* RBAR0, RLAR0 ... RBAR2, RLAR2 */
                stmia   r1!, {r4-r9}        /* RBAR0, RLAR0 ... RBAR2, RLAR2 */
#else
                ldm     r2, {r4-r11}        /* RBAR0, RLAR0 ... RBAR3, RLAR3 */
                stmia   r1!, {r4-r11}       /* RBAR0, RLAR0 ... RBAR3, RLAR3 */
#endif
#endif /* PORT_SWITCHED_REGIONS_NUMBER > 0 */
#if PORT_SWITCHED_REGIONS_NUMBER > 4
                mov     r3, #4
                str     r3, [r2, #-4]       /* RNR */
#if PORT_SWITCHED_REGIONS_NUMBER == 5
                ldm     r2, {r4-r5}         /* RBAR4, RLAR4 */
                stmia   r1!, {r4-r5}
#elif PORT_SWITCHED_REGIONS_NUMBER == 6
                ldm     r2, {r4-r7}         /* RBAR4, RLAR4 ... RBAR5, RLAR5 */
                stmia   r1!, {r4-r7}        /* RBAR4, RLAR4 ... RBAR5, RLAR5 */
#elif PORT_SWITCHED_REGIONS_NUMBER == 7
                ldm     r2, {r4-r9}         /* RBAR4, RLAR4 ... RBAR6, RLAR6 */
                stmia   r1!, {r4-r9}        /* RBAR4, RLAR4 ... RBAR6, RLAR6 */
#else
                ldm     r2, {r4-r11}        /* RBAR4, RLAR4 ... RBAR7, RLAR7 */
                stmia   r1!, {r4-r11}       /* RBAR4, RLAR4 ... RBAR7, RLAR7 */
#endif
#endif /* PORT_SWITCHED_REGIONS_NUMBER > 4 */
                .endm

                /* Load MPU context through R0.*/
                .macro  PORT_LOAD_MPU_CONTEXT
#if PORT_SWITCHED_REGIONS_NUMBER > 0
                add     r1, r0, #MPU_OFFSET
                ldr     r2, =MPU_RNR
                mov     r3, #0
#if PORT_SWITCHED_REGIONS_NUMBER == 1
                ldmia   r1!, {r4-r5}        /*      RBAR0, RLAR0 */
                stm     r2, {r3-r5}         /* RNR, RBAR0, RLAR0 */
#elif PORT_SWITCHED_REGIONS_NUMBER == 2
                ldmia   r1!, {r4-r7}        /*      RBAR0, RLAR0 ... RBAR1, RLAR1 */
                stm     r2, {r3-r7}         /* RNR, RBAR0, RLAR0 ... RBAR1, RLAR1 */
#elif PORT_SWITCHED_REGIONS_NUMBER == 3
                ldmia   r1!, {r4-r9}        /*      RBAR0, RLAR0 ... RBAR2, RLAR2 */
                stm     r2, {r3-r9}         /* RNR, RBAR0, RLAR0 ... RBAR2, RLAR2 */
#else
                ldmia   r1!, {r4-r11}       /*      RBAR0, RLAR0 ... RBAR3, RLAR3 */
                stm     r2, {r3-r11}        /* RNR, RBAR0, RLAR0 ... RBAR3, RLAR3 */
#endif
#endif /* PORT_SWITCHED_REGIONS_NUMBER > 0 */
#if PORT_SWITCHED_REGIONS_NUMBER > 4
                mov     r3, #4
#if PORT_SWITCHED_REGIONS_NUMBER == 5
                ldmia   r1!, {r4-r5}        /*      RBAR4, RLAR4 */
                stm     r2, {r3-r5}         /* RNR, RBAR4, RLAR4 */
#elif PORT_SWITCHED_REGIONS_NUMBER == 6
                ldmia   r1!, {r4-r7}        /*      RBAR4, RLAR4 ... RBAR5, RLAR5 */
                stm     r2, {r3-r7}         /* RNR, RBAR4, RLAR4 ... RBAR5, RLAR5 */
#elif PORT_SWITCHED_REGIONS_NUMBER == 7
                ldmia   r1!, {r4-r9}        /*      RBAR4, RLAR4 ... RBAR6, RLAR6 */
                stm     r2, {r3-r9}         /* RNR, RBAR4, RLAR4 ... RBAR6, RLAR6 */
#else
                ldmia   r1!, {r4-r11}       /*      RBAR4, RLAR4 ... RBAR7, RLAR7 */
                stm     r2, {r3-r11}        /* RNR, RBAR4, RLAR4 ... RBAR7, RLAR7 */
#endif
#endif /* PORT_SWITCHED_REGIONS_NUMBER > 4 */
                .endm

/*--------------------------------------------------------------------------*
 * Performs a context switch between two threads using SVC.
 *--------------------------------------------------------------------------*/
                .thumb_func
                .globl  SVC_Handler
SVC_Handler:
#if PORT_USE_SYSCALL
                mrs     r12, CONTROL
                tst     r12, #1
                beq     .Lfrompriv
                /* SVC called from non-privileged mode for a syscall.*/
                /* Note, LR already contains the return address.*/
                mrs     r1, PSP             /* Position of the exc.frame.*/
                ldr     r3, [r1, #24]       /* PC position.*/
                ldrb    r0, [r3, #-2]       /* SVC parameter.*/
                cmp     r0, #0x80
                blt     __port_do_fastcall_entry
                bic     r12, #1
                msr     CONTROL, r12        /* Switching to privileged.*/
                b       __port_do_syscall_entry

.Lfrompriv:
                /* SVC called from privileged mode for unprivileged return.*/
                mrs     r2, PSP             /* Position of the exc.frame.*/
                ldr     r3, [r2, #24]       /* PC position.*/
                ldrb    r3, [r3, #-2]       /* SVC immediate.*/
                cbz     r3, .Lctxswitch
                /* Called for non-privileged mode change.*/
                orr     r12, #1
                msr     CONTROL, r12        /* Switching to non-privileged. */
                b       __port_do_syscall_return

.Lctxswitch:
                /* SVC called from privilege mode for context switch.*/
#else
#if PORT_SAVE_CONTROL
                mrs     r12, CONTROL
#endif
                mrs     r2, PSP             /* Position of the exc.frame.*/
#endif
                /* Retrieving pointers to old and new context from the
                   saved stack frame.*/
                ldmia   r2, {r0, r1}        /* R0/R1 on SVC context (ntp/otp) */
                adds    r1, #CONTEXT_OFFSET
                adds    r0, #CONTEXT_OFFSET

                /* Context store for old thread through R1.*/
                mrs     r3, BASEPRI
                PORT_STORE_INTEGER_CONTEXT
                PORT_STORE_SPECIAL_CONTEXT
                PORT_STORE_FLOAT_CONTEXT
                PORT_STORE_MPU_CONTEXT

#if CH_DBG_SYSTEM_STATE_CHECK || CH_DBG_STATISTICS || CH_CFG_SMP_MODE
                ldr     r3, [r0, #4]        /* BASEPRI offset */
                cmp     r3, #0              /* CORTEX_BASEPRI_DISABLED */
                bne     1f
                mov     r4, r0
                /* Returning to a preempted thread, performing a logical
                   "unlock", handling statistics and releasing spinlock.*/
#if CH_DBG_SYSTEM_STATE_CHECK
                bl      __dbg_check_unlock
#endif
#if CH_DBG_STATISTICS
                bl      __stats_stop_measure_crit_thd
#endif
#if CH_CFG_SMP_MODE == TRUE
                bl      __port_spinlock_release
#endif
                mov     r0, r4
1:
#endif /* CH_DBG_SYSTEM_STATE_CHECK || CH_DBG_STATISTICS || CH_CFG_SMP_MODE */

                /* Context load for new thread through R0.*/
                PORT_LOAD_MPU_CONTEXT
                PORT_LOAD_INTEGER_CONTEXT
                PORT_LOAD_SPECIAL_CONTEXT
                PORT_LOAD_FLOAT_CONTEXT

                bx      lr

/*--------------------------------------------------------------------------*
 * Tail preemption check using PENDSV.
 *--------------------------------------------------------------------------*/
                .thumb_func
                .globl  PendSV_Handler
PendSV_Handler:
                push    {r3, lr}

                bl      __port_schedule_next
                cmp     r0, #0
                it      eq
                popeq   {r3, pc}
                pop     {r3, lr}

                /* Context store for old thread through R1.*/
                adds    r1, #CONTEXT_OFFSET

                mrs     r2, PSP
                mov     r3, #0              /* CORTEX_BASEPRI_DISABLED */
#if PORT_SAVE_CONTROL
                mrs     r12, CONTROL
#endif
                PORT_STORE_INTEGER_CONTEXT
                PORT_STORE_SPECIAL_CONTEXT
                PORT_STORE_FLOAT_CONTEXT
                PORT_STORE_MPU_CONTEXT

                /* Context load for new thread through R0.*/
                adds    r0, #CONTEXT_OFFSET

#if CH_DBG_SYSTEM_STATE_CHECK || CH_DBG_STATISTICS || CH_CFG_SMP_MODE
                ldr     r3, [r0, #4]        /* BASEPRI offset */
                cmp     r3, #0              /* CORTEX_BASEPRI_DISABLED */
                bne     1f
                mov     r4, r0
                /* Returning to a preempted thread, performing a logical
                   "unlock", handling statistics and releasing spinlock.*/
#if CH_DBG_SYSTEM_STATE_CHECK
                bl      __dbg_check_unlock
#endif
#if CH_DBG_STATISTICS
                bl      __stats_stop_measure_crit_thd
#endif
#if CH_CFG_SMP_MODE == TRUE
                bl      __port_spinlock_release
#endif
                mov     r0, r4
1:
#endif /* CH_DBG_SYSTEM_STATE_CHECK || CH_DBG_STATISTICS || CH_CFG_SMP_MODE */

                PORT_LOAD_MPU_CONTEXT
                PORT_LOAD_INTEGER_CONTEXT
                PORT_LOAD_SPECIAL_CONTEXT
                PORT_LOAD_FLOAT_CONTEXT

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
#if CORTEX_USE_FPU && (PORT_USE_FPU_FAST_SWITCHING >= 2)
                /* Clearing FPCA for the new thread, it is initially set
                   because a long context is used for starting it.*/
                mov     r0, #2
                msr     CONTROL, r0
#endif
#if CH_DBG_SYSTEM_STATE_CHECK
                bl      __dbg_check_unlock
#endif
#if CH_DBG_STATISTICS
                bl      __stats_stop_measure_crit_thd
#endif
#if CH_CFG_SMP_MODE == TRUE
                bl      __port_spinlock_release
#endif
                movs    r3, #0              /* CORTEX_BASEPRI_DISABLED */
                msr     BASEPRI, r3
                mov     r0, r5
                blx     r4
                movs    r0, #0              /* MSG_OK */
                bl      chThdExit
1:              b       1b

#endif /* !defined(__DOXYGEN__) */

/** @} */

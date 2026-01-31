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
 * @file    sb/sbexc.S
 * @brief   Exception handlers for sandbox.
 *
 * @defgroup ARMV7M_SANDBOX_EXCEPTIONS SandBox Exception Vectors
 * @{
 */

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

#define FPU     0xE000EF30

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
                .thumb_func
                .global     __sb_dispatch_syscall
__sb_dispatch_syscall:
                movw        r3, #:lower16:sb_syscalls
                movt        r3, #:upper16:sb_syscalls
                subs        r2, #128
                ldr.w       r3, [r3, r2, lsl #2]
                blx         r3
                svc         1
1:              b           1b

                .align      2
                .thumb_func
                .global     HardFault_Handler
HardFault_Handler:
                tst.w       lr, #8
                beq.n       1f
                mrs         r3, CONTROL
                tst.w       r3, #1
                bne.w       .Lexit_thd
1:              ldr         r3, =HardFault_Handler_SB
                bx          r3

                .align      2
                .thumb_func
                .global     MemManage_Handler
MemManage_Handler:
                tst.w       lr, #8
                beq.n       1f
                mrs         r3, CONTROL
                tst.w       r3, #1
                bne.w       .Lexit_thd
1:              ldr         r3, =MemManage_Handler_SB
                bx          r3

                .align      2
                .thumb_func
                .global     BusFault_Handler
BusFault_Handler:
                tst.w       lr, #8
                beq.n       1f
                mrs         r3, CONTROL
                tst.w       r3, #1
                bne.w       .Lexit_thd
1:              ldr         r3, =BusFault_Handler_SB
                bx          r3

                .align      2
                .thumb_func
                .global     UsageFault_Handler
UsageFault_Handler:
                tst.w       lr, #8
                beq.n       1f
                mrs         r3, CONTROL
                tst.w       r3, #1
                bne.w       .Lexit_thd
1:              ldr         r3, =UsageFault_Handler_SB
                bx          r3

/*
 * Common thread-exit handler on exception.
 * Makes the current exception return on chThdExit() with the PSR
 * value as exit message.
 */
.Lexit_thd:
                mov.w       r4, lr

                /* This thread is going to die so going back on
                   S-PSP and working safely from there.*/
                bl          port_get_s_psp
#if PORT_SAVE_PSPLIM
                msr         PSPLIM, r1
#endif
#if CORTEX_USE_FPU /* TODO */
                sub.w       r0, r0, #104
#else
                sub.w       r0, r0, #32
                msr         PSP, r0
#endif

                /* Forcing return on exit syscall code with PSR
                   value as exit message.*/
                mrs         r2, PSR
                str         r2, [r0, #0]
                ldr         r2, =.Ldo_exit
                str         r2, [r0, #24]
                ldr         r2, =0x01000000
                str         r2, [r0, #28]
#if CORTEX_USE_FPU
                ldr         r2, =FPU
                ldr         r2, [r2, #12]   /* FPDSCR*/
                str         r2, [r0, #96]   /* port_extctx.fpscr */
#endif

                /* Back to privileged mode.*/
                mrs         r3, CONTROL
                bic.w       r3, #1
                msr         CONTROL, r3

                /* Making sure there are no chained exceptions or interrupts
                   in the way, we need to exit this one atomically.*/
                bl          port_syslock_noinline
                bx          r4

                /* Exception exit point.*/
.Ldo_exit:
                bl          __sb_abort
1:              b           1b

                .align      2
                .thumb_func
                .weak       HardFault_Handler_SB
                .weak       MemManage_Handler_SB
                .weak       BusFault_Handler_SB
                .weak       UsageFault_Handler_SB
HardFault_Handler_SB:
MemManage_Handler_SB:
BusFault_Handler_SB:
UsageFault_Handler_SB:
                ldr         r3, =_unhandled_exception
                bx          r3

#endif /* !defined(__DOXYGEN__) */

/** @} */

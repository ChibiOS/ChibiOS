/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio.

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
 * @file    ARMCMAx-TZ/compilers/GCC/chcoreasm.S
 * @brief   ARMCMAx-TZ architecture port low level code.
 *
 * @addtogroup ARMCMAx-TZ_CORE
 * @{
 */

#define _FROM_ASM_
#include "chlicense.h"
#include "chconf.h"
#include "armparams.h"

#define FALSE 0
#define TRUE 1

#if !defined(__DOXYGEN__)

/*
 * RTOS-specific context offset.
 */
#if defined(_CHIBIOS_RT_CONF_)
#define CONTEXT_OFFSET  12
#elif defined(_CHIBIOS_NIL_CONF_)
#define CONTEXT_OFFSET  0
#else
#error "invalid chconf.h"
#endif

                .set    MODE_USR, 0x10
                .set    MODE_FIQ, 0x11
                .set    MODE_IRQ, 0x12
                .set    MODE_SVC, 0x13
                .set    MODE_MON, 0x16
                .set    MODE_ABT, 0x17
                .set    MODE_UND, 0x1B
                .set    MODE_SYS, 0x1F

                .equ    I_BIT,      0x80
                .equ    F_BIT,      0x40
                .equ    SCR_NS,     0x01
                .equ    SCR_IRQ,    0x02
                .equ    SCR_FIQ,    0x04
                .equ    SCR_EA,     0x08
                .equ    SCR_FW,     0x10
                .equ    SCR_AW,     0x20

                .equ    MON_S_SCR,  SCR_IRQ
                .equ    MON_NS_SCR, SCR_FIQ|SCR_NS

                .text

                .balign 16

                .code   32
                .global _port_switch_arm
_port_switch_arm:
                stmfd   sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
                str     sp, [r1, #12]
                ldr     sp, [r0, #12]
                ldmfd   sp!, {r4, r5, r6, r7, r8, r9, r10, r11, pc}

/*
 * Common IRQ code. It expects a macro ARM_IRQ_VECTOR_REG with the address
 * of a register holding the address of the ISR to be invoked, the ISR
 * then returns in the common epilogue code where the context switch will
 * be performed, if required.
 * System stack frame structure after a context switch in the
 * interrupt handler:
 *
 * High +------------+
 *      |   LR_USR   | -+
 *      |     r12    |  |
 *      |     r3     |  |
 *      |     r2     |  | External context: IRQ handler frame
 *      |     r1     |  |
 *      |     r0     |  |
 *      |   LR_IRQ   |  |   (user code return address)(could be in non-secure space)
 *      |   PSR_USR  | -+   (user code status)
 *      |    ....    | <- chSchDoReschedule() stack frame, optimize it for space
 *      |     LR     | -+   (system code return address)(always in secure space)
 *      |     r11    |  |
 *      |     r10    |  |
 *      |     r9     |  |
 *      |     r8     |  | Internal context: chSysSwitch() frame
 *      |     r7     |  |
 *      |     r6     |  |
 *      |     r5     |  |
 * SP-> |     r4     | -+
 * Low  +------------+
 *
 */

/*
 * We are facing an architecure with security extension exploited.
 * The following two monitor execution paths are taken by the execution units
 * running in secure state when an irq is fired (Mon_Irq_Handler), and in non-secure
 * state when a fiq interrupt is fired (Mon_Fiq_Handler).
 * They originate by the monitor irq/fiq vector and run in monitor mode,
 * ie in secure state.
 * It assumes the following, set at boot time, or wherever it needs:
 *   SCR.FW  == 0 and SCR.FIQ == 1 and SCR.IRQ == 0 in non-secure state,
 *      ie FIQs are taken to monitor mode, IRQs are taken locally
 *   SCR.FW  == 0 and SCR.FIQ == 0 and SCR.IRQ == 1 in secure state,
 *      ie FIQs are taken locally, IRQs are taken to monitor mode
 *   MVBAR holds the address of the monitor vectors base.
 *   The code and the stacks memory reside both in secure memory.
 */
                .balign 16
                .code   32
                .global Mon_Fiq_Handler
                .global Mon_Irq_Handler
                .global Fiq_Handler
Mon_Irq_Handler:
                // Here the IRQ is taken from secure state,
                // current mode is monitor (so current state is secure),
                // the previous mode and status is in mon.spsr and
                // the return address+4 is in mon.lr.
                //
                // This procedure is challenging, because the irq must be
                // executed in the non secure context and must return in this
                // secure context. The non secure IRQ handler has
                // the responsibility to return into secure state via a smc.
                //
                // The thread is interrupted in the same way that local FIQ.
                // The frame is created in the system stack of the current thread
                msr     CPSR_c, #MODE_SYS | I_BIT | F_BIT
                stmfd   sp!, {r0-r3, r12, lr}
                msr     CPSR_c, #MODE_MON | I_BIT | F_BIT
                mrs     r0, SPSR
                mov     r1, lr
                msr     CPSR_c, #MODE_SYS | I_BIT | F_BIT
                stmfd   sp!, {r0, r1}           // Push R0=SPSR, R1=LR_IRQ.

                /* Save the s_ctx e recover the ns_ctx */

                // Re-establish the original conditions
                ldmfd   sp!, {r0, r1}           // Pop R0=SPSR, R1=LR_IRQ.
                msr     CPSR_c, #MODE_MON | I_BIT | F_BIT
                msr     SPSR_fsxc, r0
                mov     lr, r1
                msr     CPSR_c, #MODE_SYS | I_BIT | F_BIT
                ldmfd   sp!, {r0-r3, r12, lr}
                msr     CPSR_c, #MODE_MON | I_BIT | F_BIT
                stmfd   sp!, {lr}               // save lr into monitor stack
                ldr     lr, =MON_NS_SCR         // set non-secure SCR before return
                mrc     p15, 0, lr, c1, c1, 0
                ldmfd   sp!, {lr}
                subs    pc, lr, #4              // return into non-secure world
                                                // and serve the IRQ
/*
 *
 */
Mon_Fiq_Handler:
                // Here the fiq is taken from non-secure state, via the FIQ vector
                // that is in the mon vector table.
                // Current mode is monitor (so current state is secure).
                // Note also that we never leave secure state while sec FIQ was disabled,
                // then it's always safe to process the FIQ here.
                stmfd   sp!, {lr}               // save lr into monitor stack
                ldr     lr, =MON_S_SCR          // set secure SCR before to switch to FIQ mode
                mrc     p15, 0, lr, c1, c1, 0
                cpsid   if, #MODE_FIQ           // secure FIQ mode
                stmfd   sp!, {r0-r3, r12}       // IRQ frame, save scratch registers
                ldr     r0, =ARM_IRQ_VECTOR_REG
                ldr     r0, [r0]
                ldr     lr, =_mon_fiq_ret_arm   // ISR return point.
                bx      r0                      // Calling the ISR.
_mon_fiq_ret_arm:
                cmp     r0, #0
                ldmfd   sp!, {r0-r3, r12}
                cpsid   if, #MODE_MON
                ldr     lr, =MON_NS_SCR         // set non-secure SCR before return
                mrceq   p15, 0, lr, c1, c1, 0   // only if it will return
                ldmfd   sp!, {lr}
                subeqs  pc, lr, #4              // No reschedule, returns.

                // Now the frame is created in the system stack
                // relative to non secure context,
                // the IRQ and monitor stacks are empty,
                // the state is secure.
                msr     CPSR_c, #MODE_SYS | I_BIT | F_BIT
                stmfd   sp!, {r0-r3, r12, lr}
                mov		r0, sp
                msr     CPSR_c, #MODE_MON | I_BIT | F_BIT
                stmfd	sp!, {r0}				// address of ns_ctx in mon stack
                mrs     r0, SPSR
                mov     r1, lr
                msr     CPSR_c, #MODE_SYS | I_BIT | F_BIT
                stmfd   sp!, {r0, r1}           // Push R0=SPSR, R1=LR_IRQ.
                mov		r0, sp

                // Context switch.
#if CH_DBG_SYSTEM_STATE_CHECK
                bl      _dbg_check_lock
#endif
                bl      chSchDoReschedule
#if CH_DBG_SYSTEM_STATE_CHECK
                bl      _dbg_check_unlock
#endif

                // Re-establish the IRQ conditions again.
                ldmfd   sp!, {r0, r1}           // Pop R0=SPSR, R1=LR_IRQ.
                msr     CPSR_c, #MODE_MON | I_BIT | F_BIT
                msr     SPSR_fsxc, r0
                mov     lr, r1
                ldmfd	sp!, {r0}				// ns_ctx no longer exists, cleanup mon stack
                msr     CPSR_c, #MODE_SYS | I_BIT | F_BIT
                ldmfd   sp!, {r0-r3, r12, lr}
                msr     CPSR_c, #MODE_MON | I_BIT | F_BIT
                stmfd   sp!, {lr}               // save lr into mon stack
                ldr     lr, =MON_NS_SCR         // set non-secure SCR before return
                mrc     p15, 0, lr, c1, c1, 0
                ldmfd   sp!, {lr}
                subs    pc, lr, #4              // return into non-secure world
/*
 *
 */
Fiq_Handler:
                // the fiq is taken locally from secure state
                // current mode is fiq
                stmfd   sp!, {r0-r3, r12, lr}   // save scratch registers and lr
                ldr     r0, =ARM_IRQ_VECTOR_REG
                ldr     r0, [r0]
                ldr     lr, =_fiq_ret_arm       // ISR return point.
                bx      r0                      // Calling the ISR.
_fiq_ret_arm:
                cmp     r0, #0
                ldmfd   sp!, {r0-r3, r12, lr}
                subeqs  pc, lr, #4              // No reschedule, returns.

                // Now the frame is created in the system stack of the current
                // thread, the IRQ stack is empty.
                msr     CPSR_c, #MODE_SYS | I_BIT | F_BIT
                stmfd   sp!, {r0-r3, r12, lr}
                msr     CPSR_c, #MODE_FIQ | I_BIT | F_BIT
                mrs     r0, SPSR
                mov     r1, lr
                msr     CPSR_c, #MODE_SYS | I_BIT | F_BIT
                stmfd   sp!, {r0, r1}           // Push R0=SPSR, R1=LR_IRQ.

                // Context switch.
#if CH_DBG_SYSTEM_STATE_CHECK
                bl      _dbg_check_lock
#endif
                bl      chSchDoReschedule
#if CH_DBG_SYSTEM_STATE_CHECK
                bl      _dbg_check_unlock
#endif

                // Re-establish the IRQ conditions again.
                ldmfd   sp!, {r0, r1}           // Pop R0=SPSR, R1=LR_IRQ.
                msr     CPSR_c, #MODE_FIQ | I_BIT | F_BIT
                msr     SPSR_fsxc, r0
                mov     lr, r1
                msr     CPSR_c, #MODE_SYS | I_BIT | F_BIT
                ldmfd   sp!, {r0-r3, r12, lr}
                msr     CPSR_c, #MODE_FIQ | I_BIT | F_BIT
                subs    pc, lr, #4

/*
 * Threads trampoline code.
 * NOTE: The threads always start in ARM mode.
 */
                .balign 16
                .code   32
                .globl  _port_thread_start
_port_thread_start:
#if CH_DBG_SYSTEM_STATE_CHECK
                bl      _dbg_check_unlock
#endif
				msr     CPSR_c, #MODE_SYS | I_BIT
                mov     r0, r5
                mov     lr, pc
                bx      r4
#if defined(_CHIBIOS_RT_CONF_)
                mov     r0, #0              /* MSG_OK */
                bl      chThdExit
_zombies:       b       _zombies
#endif
#if defined(_CHIBIOS_NIL_CONF_)
                mov     r0, #0
                bl      chSysHalt
#endif

#endif /* !defined(__DOXYGEN__) */

/** @} */

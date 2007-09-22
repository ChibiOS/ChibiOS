/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "chconf.h"

.set    MODE_USR, 0x10
.set    MODE_FIQ, 0x11
.set    MODE_IRQ, 0x12
.set    MODE_SVC, 0x13
.set    MODE_ABT, 0x17
.set    MODE_UND, 0x1B
.set    MODE_SYS, 0x1F

.equ    I_BIT, 0x80
.equ    F_BIT, 0x40

.text
.code 32
.balign 4

.globl threadstart
threadstart:
        msr     CPSR_c, #MODE_SYS
        mov     r0, r5
/*        blx     r4*/
        mov     lr, pc
        bx      r4
        bl      chThdExit

.globl SwiHandler
SwiHandler:
        b       SwiHandler

.globl DefIrqHandler
DefIrqHandler:
        b       DefIrqHandler

.globl FiqHandler
FiqHandler:
        b       FiqHandler

#ifdef THUMB_INTERWORK
.globl chSysLock
chSysLock:
    msr     CPSR_c, #0x9F
    bx      lr

.globl chSysUnlock
chSysUnlock:
    msr     CPSR_c, #0x1F
    bx      lr
#endif

.globl chSysSwitchI
chSysSwitchI:
#ifdef CH_CURRP_REGISTER_CACHE
    stmfd   sp!, {r4, r5, r6, r8, r9, r10, r11, lr}
    str     sp, [r0, #0]
    ldr     sp, [r1, #0]
#ifdef THUMB_INTERWORK
    ldmfd   sp!, {r4, r5, r6, r8, r9, r10, r11, lr}
    bx      lr
#else
    ldmfd   sp!, {r4, r5, r6, r8, r9, r10, r11, pc}
#endif
#else
    stmfd   sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    str     sp, [r0, #0]
    ldr     sp, [r1, #0]
#ifdef THUMB_INTERWORK
    ldmfd   sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx      lr
#else
    ldmfd   sp!, {r4, r5, r6, r7, r8, r9, r10, r11, pc}
#endif
#endif /* CH_CURRP_REGISTER_CACHE */

/*
 * System stack frame structure after a context switch in the
 * interrupt handler:
 *
 * High +------------+
 *      |     R12    | -+
 *      |     R3     |  |
 *      |     R2     |  |
 *      |     R1     |  | External context: IRQ handler frame
 *      |     R0     |  |
 *      |   LR_IRQ   |  |   (user code return address)
 *      |    SPSR    | -+   (user code status)
 *      |    ....    | <- mk_DoRescheduleI() stack frame, optimize it for space
 *      |     LR     | -+   (system code return address)
 *      |     R11    |  |
 *      |     R10    |  |
 *      |     R9     |  |
 *      |     R8     |  | Internal context: mk_SwitchI() frame
 *      |    (R7)    |  |   (optional, see MK_CURRP_REGISTER_CACHE)
 *      |     R6     |  |
 *      |     R5     |  |
 * SP-> |     R4     | -+
 * Low  +------------+
 */
.globl IrqHandler
IrqHandler:
        sub     lr, lr, #4
        stmfd   sp!, {r0-r3, r12, lr}
        mrs     r0, SPSR                        // Workaround for ARM7TDMI+VIC
        tst     r0, #I_BIT                      // spurious interrupts.
        ldmnefd sp!, {r0-r3, r12, pc}
        bl      NonVectoredIrq
        b       IrqCommon

.globl T0IrqHandler
T0IrqHandler:
        sub     lr, lr, #4
        stmfd   sp!, {r0-r3, r12, lr}
        mrs     r0, SPSR                        // Workaround for ARM7TDMI+VIC
        tst     r0, #I_BIT                      // spurious interrupts.
        ldmnefd sp!, {r0-r3, r12, pc}^
        bl      Timer0Irq
        b       IrqCommon

.globl UART0IrqHandler
UART0IrqHandler:
        sub     lr, lr, #4
        stmfd   sp!, {r0-r3, r12, lr}
        mrs     r0, SPSR                        // Workaround for ARM7TDMI+VIC
        tst     r0, #I_BIT                      // spurious interrupts.
        ldmnefd sp!, {r0-r3, r12, pc}^
        bl      UART0Irq
        b       IrqCommon

.globl UART1IrqHandler
UART1IrqHandler:
        sub     lr, lr, #4
        stmfd   sp!, {r0-r3, r12, lr}
        mrs     r0, SPSR                        // Workaround for ARM7TDMI+VIC
        tst     r0, #I_BIT                      // spurious interrupts.
        ldmnefd sp!, {r0-r3, r12, pc}^
        bl      UART1Irq
        b       IrqCommon

/*
 * Common exit point for all IRQ routines, it performs the rescheduling if
 * required.
 */
IrqCommon:
        bl      chSchRescRequiredI
        cmp     r0, #0                          // Simply returns if a
        ldmeqfd sp!, {r0-r3, r12, pc}^          // reschedule is not required.

        // Saves the IRQ mode registers in the system stack.
        ldmfd   sp!, {r0-r3, r12, lr}           // IRQ stack now empty.
        msr     CPSR_c, #MODE_SYS | I_BIT
        stmfd   sp!, {r0-r3, r12}               // Registers on System Stack.
        msr     CPSR_c, #MODE_IRQ | I_BIT
        mrs     r0, SPSR
        mov     r1, lr
        msr     CPSR_c, #MODE_SYS | I_BIT
        stmfd   sp!, {r0, r1}                   // Push R0=SPSR, R1=LR_IRQ.

        // Context switch.
        bl      chSchDoRescheduleI

        // Re-establish the IRQ conditions again.
        ldmfd   sp!, {r0, r1}                   // Pop R0=SPSR, R1=LR_IRQ.
        msr     CPSR_c, #MODE_IRQ | I_BIT
        msr     SPSR_fsxc, r0
        mov     lr, r1
        msr     CPSR_c, #MODE_SYS | I_BIT
        ldmfd   sp!, {r0-r3, r12}
        msr     CPSR_c, #MODE_IRQ | I_BIT
        subs    pc, lr, #0

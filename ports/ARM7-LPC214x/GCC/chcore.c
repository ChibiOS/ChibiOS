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

#include <ch.h>

#include "lpc214x.h"

/*
 * System idle thread loop.
 */
void _IdleThread(void *p) {

  while (TRUE) {
// Note, it is disabled because it causes trouble with the JTAG probe.
// Enable it in the final code only.
//    PCON = 1;
  }
}

/*
 * The following functions are present only if there is in the system any
 * code compiled as THUMB that may invoke them.
 * NOTE: The undefs are there in case this module is compiled in ARM mode but
 *       there are THUMB modules in the system.
 */
#ifdef THUMB_PRESENT
#undef chSysLock
void chSysLock(void) {

#ifdef THUMB
  asm(".p2align 2,,                                             \n\t" \
      "mov      r0, pc                                          \n\t" \
      "bx       r0                                              \n\t" \
      ".code 32                                                 \n\t");
#endif

  asm("msr      CPSR_c, #0x9F                                   \n\t" \
      "bx       lr                                              \n\t");
}

#undef chSysUnlock
void chSysUnlock(void) {

#ifdef THUMB
  asm(".p2align 2,,                                             \n\t" \
      "mov      r0, pc                                          \n\t" \
      "bx       r0                                              \n\t" \
      ".code 32                                                 \n\t");
#endif

  asm("msr      CPSR_c, #0x1F                                   \n\t" \
      "bx       lr                                              \n\t");
}
#endif

void chSysSwitchI(Thread *otp, Thread *ntp) {

#ifdef THUMB
  asm(".p2align 2,,                                             \n\t" \
      "mov      r2, pc                                          \n\t" \
      "bx       r2                                              \n\t" \
      ".code 32                                                 \n\t");
#endif

#ifdef CH_CURRP_REGISTER_CACHE
  asm("stmfd    sp!, {r4, r5, r6, r8, r9, r10, r11, lr}         \n\t" \
      "str      sp, [r0, #16]                                   \n\t" \
      "ldr      sp, [r1, #16]                                   \n\t");
#ifdef THUMB_PRESENT
  asm("ldmfd    sp!, {r4, r5, r6, r8, r9, r10, r11, lr}         \n\t" \
      "bx       lr                                              \n\t");
#else /* !THUMB_PRESENT */
  asm("ldmfd    sp!, {r4, r5, r6, r8, r9, r10, r11, pc}         \n\t");
#endif /* !THUMB_PRESENT */
#else /* !CH_CURRP_REGISTER_CACHE */
  asm("stmfd    sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}     \n\t" \
      "str      sp, [r0, #16]                                   \n\t" \
      "ldr      sp, [r1, #16]                                   \n\t");
#ifdef THUMB_PRESENT
  asm("ldmfd    sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}     \n\t" \
      "bx       lr                                              \n\t");
#else /* !THUMB_PRESENT */
  asm("ldmfd    sp!, {r4, r5, r6, r7, r8, r9, r10, r11, pc}     \n\t");
#endif /* !THUMB_PRESENT */
#endif /* !CH_CURRP_REGISTER_CACHE */
}

/*
 * System console message (not implemented).
 */
void chSysPuts(char *msg) {
}

/*
 * Common IRQ exit code, \p chSysIRQExitI() just jumps here.
 *
 * System stack frame structure after a context switch in the
 * interrupt handler:
 *
 * High +------------+
 *      |   LR_USR   | -+
 *      |     R12    |  |
 *      |     R3     |  |
 *      |     R2     |  | External context: IRQ handler frame
 *      |     R1     |  |
 *      |     R0     |  |
 *      |   LR_IRQ   |  |   (user code return address)
 *      |    SPSR    | -+   (user code status)
 *      |    ....    | <- chSchDoRescheduleI() stack frame, optimize it for space
 *      |     LR     | -+   (system code return address)
 *      |     R11    |  |
 *      |     R10    |  |
 *      |     R9     |  |
 *      |     R8     |  | Internal context: chSysSwitchI() frame
 *      |    (R7)    |  |   (optional, see CH_CURRP_REGISTER_CACHE)
 *      |     R6     |  |
 *      |     R5     |  |
 * SP-> |     R4     | -+
 * Low  +------------+
 */
__attribute__((naked, weak))
void IrqCommon(void) {
  register BOOL b asm("r0");

  VICVectAddr = 0;
  b = chSchRescRequiredI();
#ifdef THUMB
  asm(".p2align 2,,                                             \n\t" \
      "mov      lr, pc                                          \n\t" \
      "bx       lr                                              \n\t" \
      ".code 32                                                 \n\t");
#endif
  /*
   * If a reschedulation is not required then just returns from the IRQ.
   */
  asm("cmp      r0, #0                                          \n\t" \
      "ldmeqfd  sp!, {r0-r3, r12, lr}                           \n\t" \
      "subeqs   pc, lr, #4                                      \n\t");
  /*
   * Reschedulation required, saves the external context on the
   * system/user stack and empties the IRQ stack.
   */
  asm(".set     MODE_IRQ, 0x12                                  \n\t" \
      ".set     MODE_SYS, 0x1F                                  \n\t" \
      ".set     F_BIT, 0x40                                     \n\t" \
      ".set     I_BIT, 0x80                                     \n\t" \
      "ldmfd    sp!, {r0-r3, r12, lr}                           \n\t" \
      "msr      CPSR_c, #MODE_SYS | I_BIT                       \n\t" \
      "stmfd    sp!, {r0-r3, r12, lr}                           \n\t" \
      "msr      CPSR_c, #MODE_IRQ | I_BIT                       \n\t" \
      "mrs      r0, SPSR                                        \n\t" \
      "mov      r1, lr                                          \n\t" \
      "msr      CPSR_c, #MODE_SYS | I_BIT                       \n\t" \
      "stmfd    sp!, {r0, r1}                                   \n\t");

#ifdef THUMB_NO_INTERWORKING
  asm("add      r0, pc, #1                                      \n\t" \
      "bx       r0                                              \n\t" \
      ".code 16                                                 \n\t" \
      "bl       chSchDoRescheduleI                              \n\t" \
      ".p2align 2,,                                             \n\t" \
      "mov      lr, pc                                          \n\t" \
      "bx       lr                                              \n\t" \
      ".code 32                                                 \n\t");
#else
  asm("bl       chSchDoRescheduleI                              \n\t");
#endif

  /*
   * Restores the external context.
   */
  asm("ldmfd    sp!, {r0, r1}                                   \n\t" \
      "msr      CPSR_c, #MODE_IRQ | I_BIT                       \n\t" \
      "msr      SPSR_fsxc, r0                                   \n\t" \
      "mov      lr, r1                                          \n\t" \
      "msr      CPSR_c, #MODE_SYS | I_BIT                       \n\t" \
      "ldmfd    sp!, {r0-r3, r12, lr}                           \n\t" \
      "msr      CPSR_c, #MODE_IRQ | I_BIT                       \n\t" \
      "subs     pc, lr, #4                                      \n\t");

  /*
   * Threads entry/exit code. It is declared weak so you can easily replace it.
   * NOTE: It is always invoked in ARM mode, it does the mode switching.
   * NOTE: It is included into IrqCommon to make sure the symbol refers to
   *       32 bit code.
   */
  asm(".weak threadstart                                        \n\t" \
      ".globl threadstart                                       \n\t" \
      "threadstart:                                             \n\t" \
      "msr      CPSR_c, #MODE_SYS                               \n\t");
#ifndef THUMB_NO_INTERWORKING
  asm("mov      r0, r5                                          \n\t" \
      "mov      lr, pc                                          \n\t" \
      "bx       r4                                              \n\t" \
      "bl       chThdExit                                       \n\t");
#else
  asm("add      r0, pc, #1                                      \n\t" \
      "bx       r0                                              \n\t" \
      ".code 16                                                 \n\t" \
      "mov      r0, r5                                          \n\t" \
      "bl       jmpr4                                           \n\t" \
      "bl       chThdExit                                       \n\t" \
      "jmpr4:                                                   \n\t" \
      "bx       r4                                              \n\t");
#endif
}

/*
 * System halt.
 */
__attribute__((naked, weak))
void chSysHalt(void) {

#ifdef THUMB
  asm("ldr      r0, =_halt32                                    \n\t" \
      "bx       r0                                              \n\t");
#endif
  asm("b        _halt32                                         \n\t");
}

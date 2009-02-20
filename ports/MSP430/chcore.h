/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#ifndef _CHCORE_H_
#define _CHCORE_H_

#include <iomacros.h>
#include <msp430/common.h>

/*
 * Macro defining the MSP430 architecture.
 */
#define CH_ARCHITECTURE_MSP430

/*
 * 16 bit stack alignment.
 */
typedef uint16_t stkalign_t;

/*
 * Generic MSP430 register.
 */
typedef void *regmsp_t;

/*
 * Interrupt saved context.
 */
struct extctx {
  regmsp_t      r12;
  regmsp_t      r13;
  regmsp_t      r14;
  regmsp_t      r15;
  regmsp_t      sr;
  regmsp_t      pc;
};

/*
 * System saved context.
 */
struct intctx {
  regmsp_t      r4;
  regmsp_t      r5;
  regmsp_t      r6;
  regmsp_t      r7;
  regmsp_t      r8;
  regmsp_t      r9;
  regmsp_t      r10;
  regmsp_t      r11;
//  regmsp_t      sr;
  regmsp_t      pc;
};

typedef struct {
  struct intctx *sp;
} Context;

#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                      \
  tp->p_ctx.sp = (struct intctx *)((uint8_t *)workspace +               \
                                   wsize -                              \
                                   sizeof(struct intctx));              \
  tp->p_ctx.sp->r10 = pf;                                               \
  tp->p_ctx.sp->r11 = arg;                                              \
  tp->p_ctx.sp->pc = threadstart;                                       \
}

#define IDLE_THREAD_STACK_SIZE 0

#ifndef INT_REQUIRED_STACK
#define INT_REQUIRED_STACK 32
#endif

#define STACK_ALIGN(n) ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(Thread) +                     \
                                   sizeof(struct intctx) +              \
                                   sizeof(struct extctx) +              \
                                   (n) +                                \
                                   INT_REQUIRED_STACK)

#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)];

#define chSysLock() asm volatile ("dint")
#define chSysUnlock() asm volatile ("eint")
#define chSysEnable() asm volatile ("eint")

#define chSysIRQEnterI()
#define chSysIRQExitI() {                                               \
  if (chSchRescRequiredI())                                             \
    chSchDoRescheduleI();                                               \
}

#ifdef __cplusplus
extern "C" {
#endif
  void _idle(void *p) __attribute__((weak, noreturn));
  void chSysHalt(void);
  void chSysSwitchI(Thread *otp, Thread *ntp);
  void chSysPuts(char *msg);
  void threadstart(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

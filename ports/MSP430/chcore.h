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

#ifndef _CHCORE_H_
#define _CHCORE_H_

#include <iomacros.h>
#include <msp430/common.h>

#define CH_ARCHITECTURE_MSP430

typedef void *regmsp;

/*
 * Interrupt saved context.
 */
struct extctx {
  regmsp        r12;
  regmsp        r13;
  regmsp        r14;
  regmsp        r15;
  regmsp        sr;
  regmsp        pc;
};

/*
 * System saved context.
 */
struct intctx {
  regmsp        r4;
  regmsp        r5;
  regmsp        r6;
  regmsp        r7;
  regmsp        r8;
  regmsp        r9;
  regmsp        r10;
  regmsp        r11;
  regmsp        sr;
  regmsp        pc;
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
  tp->p_ctx.sp->sr = (regmsp)GIE;                                       \
  tp->p_ctx.sp->pc = threadstart;                                       \
}

#define IDLE_THREAD_STACK_SIZE 0

#define INT_REQUIRED_STACK 32
#define StackAlign(n) ((((n) - 1) | 1) + 1)
#define UserStackSize(n) StackAlign(sizeof(Thread) +                    \
                                    sizeof(struct intctx) +             \
                                    sizeof(struct extctx) +             \
                                    (n) + (INT_REQUIRED_STACK))
#define WorkingArea(s, n) uint16_t s[UserStackSize(n >> 1)];

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
  void _IdleThread(void *p);
  void chSysHalt(void);
  void chSysSwitchI(Thread *otp, Thread *ntp);
  void chSysPuts(char *msg);
  void threadstart(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

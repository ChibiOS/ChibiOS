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

/*
 * The following values are implementation dependent. You may change them in
 * order to match your HW.
 */
#define FOSC 12000000
#define CCLK 48000000
#define PCLK 12000000

typedef void *regarm;

/*
 * Interrupt saved context.
 */
struct extctx {
  regarm  spsr_irq;
  regarm  lr_irq;
  regarm  r0;
  regarm  r1;
  regarm  r2;
  regarm  r3;
  regarm  r12;
};

/*
 * System saved context.
 */
struct intctx {
  regarm  r4;
  regarm  r5;
  regarm  r6;
#ifndef CH_CURRP_REGISTER_CACHE
  regarm  r7;
#endif
  regarm  r8;
  regarm  r9;
  regarm  r10;
  regarm  r11;
  regarm  lr;
};

/*
 * Port dependent part of the Thread structure, you may add fields in
 * this structure.
 */
typedef struct {
  struct intctx *r13;
} Context;

/*
 * Platform dependent part of the \p chThdCreate() API.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                 \
  tp->p_ctx.r13 = (struct intctx *)((BYTE8 *)workspace +           \
                                     wsize -                       \
                                     sizeof(struct intctx));       \
  tp->p_ctx.r13->r4 = pf;                                          \
  tp->p_ctx.r13->r5 = arg;                                         \
  tp->p_ctx.r13->lr = threadstart;                                 \
}

#ifdef THUMB
extern void chSysLock(void);
extern void chSysUnlock(void);
#else /* !THUMB */
#define chSysLock() asm("msr     CPSR_c, #0x9F")
#define chSysUnlock() asm("msr     CPSR_c, #0x1F")
#endif /* THUMB */

#define chSysPuts(msg) {}

#ifdef THUMB
#define INT_REQUIRED_STACK 0x10
#else /* !THUMB */
#define INT_REQUIRED_STACK 0
#endif /* THUMB */
#define UserStackSize(n) (sizeof(Thread) +                         \
                          sizeof(struct intctx) +                  \
                          sizeof(struct extctx) +                  \
                          (INT_REQUIRED_STACK) +                   \
                          (n))

/* It requires zero bytes, but better be safe.*/
#define IDLE_THREAD_STACK_SIZE 8
void _IdleThread(void *p) __attribute__((noreturn));

void chSysHalt(void) __attribute__((noreturn));
void chSysSwitchI(Context *oldp, Context *newp);
void threadstart(void);
void DefFiqHandler(void);
void DefIrqHandler(void);
void SpuriousHandler(void);

#endif /* _CHCORE_H_ */

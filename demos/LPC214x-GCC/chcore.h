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

typedef void *regarm;

/*
 * Stack saved context.
 */
struct stackregs {
  regarm  r4;
  regarm  r5;
  regarm  r6;
#ifndef MK_CURRP_REGISTER_CACHE
  regarm  r7;
#endif
  regarm  r8;
  regarm  r9;
  regarm  r10;
  regarm  r11;
  regarm  lr;
};

typedef struct {
  struct stackregs *r13;
} Context;

#ifdef MK_CURRP_REGISTER_CACHE
#define SETUP_CONTEXT(workspace, wsize, pf, arg)                   \
{                                                                  \
  tp->p_ctx.r13 = (struct stackregs *)((BYTE8 *)workspace +        \
                                       wsize -                     \
                                       sizeof(struct stackregs));  \
  tp->p_ctx.r13->r4 = pf;                                          \
  tp->p_ctx.r13->r5 = arg;                                         \
  tp->p_ctx.r13->r6 = 0;                                           \
  tp->p_ctx.r13->r8 = 0;                                           \
  tp->p_ctx.r13->r9 = 0;                                           \
  tp->p_ctx.r13->r10 = 0;                                          \
  tp->p_ctx.r13->r11 = 0;                                          \
  tp->p_ctx.r13->lr = threadstart;                                 \
}
#else
#define SETUP_CONTEXT(workspace, wsize, pf, arg)                   \
{                                                                  \
  tp->p_ctx.r13 = (struct stackregs *)((BYTE8 *)workspace +        \
                                       wsize -                     \
                                       sizeof(struct stackregs));  \
  tp->p_ctx.r13->r4 = pf;                                          \
  tp->p_ctx.r13->r5 = arg;                                         \
  tp->p_ctx.r13->r6 = 0;                                           \
  tp->p_ctx.r13->r7 = 0;                                           \
  tp->p_ctx.r13->r8 = 0;                                           \
  tp->p_ctx.r13->r9 = 0;                                           \
  tp->p_ctx.r13->r10 = 0;                                          \
  tp->p_ctx.r13->r11 = 0;                                          \
  tp->p_ctx.r13->lr = threadstart;                                 \
}
#endif

#ifdef THUMB
extern void chSysLock(void);
extern void chSysUnlock(void);
#else /* !THUMB */
#define chSysLock() asm("msr     CPSR_c, #0x9F")
#define chSysUnlock() asm("msr     CPSR_c, #0x1F")
#endif /* THUMB */

#define INT_REQUIRED_STACK 0x40  // Must include registers and stack frames.

#define UserStackSize(n) (sizeof(Thread) + \
                          sizeof(struct stackregs) + (n) + (INT_REQUIRED_STACK))

void chSysHalt(void) __attribute__((noreturn));
void chSysPause(void);
void chSysSwitchI(Context *oldp, Context *newp);
void threadstart(void);
void DefFiqHandler(void);
void DefIrqHandler(void);
void SpuriousHandler(void);

void SetVICVector(void *handler, int vector, int source);

#endif /* _CHCORE_H_ */

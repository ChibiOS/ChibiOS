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

/**
 * @addtogroup Core
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

/*
 * Interrupt saved context.
 */
struct extctx {
  BYTE8         sr;
  BYTE8         r31;
  BYTE8         r30;
  BYTE8         r27;
  BYTE8         r26;
  BYTE8         r25;
  BYTE8         r24;
  BYTE8         r23;
  BYTE8         r22;
  BYTE8         r21;
  BYTE8         r20;
  BYTE8         r19;
  BYTE8         r18;
  BYTE8         r1;
  BYTE8         r0;
  UWORD16       pc;
};

/*
 * Stack saved context.
 */
struct intctx {
  BYTE8         r29;
  BYTE8         r28;
  BYTE8         r17;
  BYTE8         r16;
  BYTE8         r15;
  BYTE8         r14;
  BYTE8         r13;
  BYTE8         r12;
  BYTE8         r11;
  BYTE8         r10;
  BYTE8         r9;
  BYTE8         r8;
  BYTE8         r7;
  BYTE8         r6;
  BYTE8         r5;
  BYTE8         r4;
  BYTE8         r3;
  BYTE8         r2;
  UWORD16       pc;
};

typedef struct {
  struct intctx *sp;
} Context;

/**
 * Platform dependent part of the \p chThdCreate() API.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                 \
  tp->p_ctx.sp--;                                                  \
  tp->p_ctx.sp->r2 = (int)pf;                                      \
  tp->p_ctx.sp->r3 = (int)pf >> 8;                                 \
  tp->p_ctx.sp->r4 = (int)arg;                                     \
  tp->p_ctx.sp->r5 = (int)arg >> 8;                                \
  tp->p_ctx.sp->pc = (UWORD16)threadstart;                         \
}

/*
 * Interrupt stack usage except for saved registers.
 */
#define EXTRA_INT_STACK 0x10

#define UserStackSize(n) (sizeof(Thread) + \
                          sizeof(struct intctx) + \
                          sizeof(struct extctx) + \
                          EXTRA_INT_STACK + \
                          (n))

#define chSysLock() asm("cli")
#define chSysUnlock() asm("sei")

void chSysHalt(void) __attribute__((noreturn)) ;
void chSysPause(void) __attribute__((noreturn)) ;
void chSysSwitchI(Context *oldp, Context *newp);
void threadstart(void);

#endif /* _CHCORE_H_ */

/** @} */

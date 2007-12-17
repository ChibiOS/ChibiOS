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

/*
 * Core file for Visual Studio 7 demo project.
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

typedef void *regx86;

/*
 * Stack saved context.
 */
struct intctx {
  regx86  ebx;
  regx86  edi;
  regx86  esi;
  regx86  ebp;
  regx86  eip;
};

typedef struct {
  struct intctx *esp;
} Context;

#define APUSH(p, a) (p) -= sizeof(void *), *(void **)(p) = (void*)(a)

#define SETUP_CONTEXT(workspace, wsize, pf, arg)                        \
{                                                                       \
  BYTE8 *esp = (BYTE8 *)workspace + wsize;                              \
  APUSH(esp, arg);                                                      \
  APUSH(esp, threadexit);                                               \
  esp -= sizeof(struct intctx);                                         \
  ((struct intctx *)esp)->eip = pf;                                     \
  ((struct intctx *)esp)->ebx = 0;                                      \
  ((struct intctx *)esp)->edi = 0;                                      \
  ((struct intctx *)esp)->esi = 0;                                      \
  ((struct intctx *)esp)->ebp = 0;                                      \
  tp->p_ctx.esp = (struct intctx *)esp;                                 \
}

#define chSysLock()
#define chSysUnlock()
#define chSysPuts(msg) {}
#define chSysIRQEnterI()
#define chSysIRQExitI()

#define INT_REQUIRED_STACK 0
#define StackAlign(n) ((((n) - 1) | 3) + 1)
#define UserStackSize(n) StackAlign(sizeof(Thread) +                    \
                                    sizeof(void *) * 2 +                \
                                    sizeof(struct intctx) +             \
                                    (n) +                               \
                                    INT_REQUIRED_STACK)
#define WorkingArea(s, n) ULONG32 s[UserStackSize(n) >> 2];

#define IDLE_THREAD_STACK_SIZE 16384
t_msg _IdleThread(void *p);

void __fastcall chSysHalt(void);
void __fastcall chSysSwitchI(Context *oldp, Context *newp);
void __fastcall threadexit(void);

#endif /* _CHCORE_H_ */

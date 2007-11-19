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
 * Core file for MingGW32 demo project.
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

typedef void *regx86;

/*
 * Stack saved context.
 */
struct stackregs {
  regx86  ebx;
  regx86  edi;
  regx86  esi;
  regx86  ebp;
  regx86  eip;
};

typedef struct {
  struct stackregs *esp;
} Context;

#define APUSH(p, a) (p) -= sizeof(void *), *(void **)(p) = (void*)(a)

#define SETUP_CONTEXT(workspace, wsize, pf, arg) \
{ \
  BYTE8 *esp = (BYTE8 *)workspace + wsize; \
  APUSH(esp, arg); \
  APUSH(esp, threadstart); \
  esp -= sizeof(struct stackregs); \
  ((struct stackregs *)esp)->eip = pf; \
  ((struct stackregs *)esp)->ebx = 0; \
  ((struct stackregs *)esp)->edi = 0; \
  ((struct stackregs *)esp)->esi = 0; \
  ((struct stackregs *)esp)->ebp = 0; \
  tp->p_ctx.esp = (struct stackregs *)esp; \
}

#define chSysLock()
#define chSysUnlock()
#define chSysPuts(msg) {}

#define INT_REQUIRED_STACK 0x0
#define UserStackSize(n) (sizeof(Thread) + sizeof(void *) * 2 + \
                          sizeof(struct stackregs) + (n) + (INT_REQUIRED_STACK))

#define IDLE_THREAD_STACK_SIZE 16384
t_msg _IdleThread(void *p);

__attribute__((fastcall)) void chSysHalt(void);
__attribute__((fastcall)) void chSysSwitchI(Context *oldp, Context *newp);
__attribute__((fastcall)) void threadstart(void);

#endif /* _CHCORE_H_ */

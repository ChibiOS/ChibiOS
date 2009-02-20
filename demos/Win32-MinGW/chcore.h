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

/*
 * Core file for MingGW32 demo project.
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

/*
 * Unique macro for the implemented architecture.
 */
#define CH_ARCHITECTURE_WIN32SIM

/*
 * Base type for stack alignment.
 */
typedef uint32_t stkalign_t;

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
  uint8_t *esp = (uint8_t *)workspace + wsize;                          \
  APUSH(esp, arg);                                                      \
  APUSH(esp, threadstart);                                              \
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
#define chSysEnable()
#define chSysPuts(msg) {}
#define chSysIRQEnterI()
#define chSysIRQExitI()

#define INT_REQUIRED_STACK 0

#define STACK_ALIGN(n) ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(Thread) +                     \
                                   sizeof(void *) * 2 +                 \
                                   sizeof(struct intctx) +              \
                                   (n) +                                \
                                   INT_REQUIRED_STACK)

#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)];

/*
 * Stack size for the system idle thread.
 */
#define IDLE_THREAD_STACK_SIZE 16384

msg_t _idle(void *p);
__attribute__((fastcall)) void chSysHalt(void);
__attribute__((fastcall)) void chSysSwitchI(Thread *otp, Thread *ntp);
__attribute__((fastcall)) void threadstart(void);

#endif /* _CHCORE_H_ */

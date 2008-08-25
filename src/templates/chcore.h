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

/**
 * Unique macro for the implemented architecture.
 */
#define CH_ARCHITECTURE_XXX

/**
 * Interrupt saved context.
 */
struct extctx {
};

/**
 * System saved context.
 */
struct intctx {
};

typedef struct {
  struct intctx *sp;
} Context;

/**
 * Platform dependent part of the \p chThdCreate() API.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg)                        \
{                                                                       \
}

/**
 * Stack size for the system idle thread.
 */
#define IDLE_THREAD_STACK_SIZE 0

/**
 * Per-thread stack overhead for interrupts servicing.
 */
#define INT_REQUIRED_STACK 0

/**
 * Enforces a stack size alignment.
 */
#define StackAlign(n) (n)

/**
 * Macro to be used when allocating stack spaces, it adds the system-specific
 * overhead.
 */
#define UserStackSize(n) StackAlign(sizeof(Thread) +                    \
                                    sizeof(struct intctx) +             \
                                    sizeof(struct extctx) +             \
                                    (n) + (INT_REQUIRED_STACK))

/**
 * Macro used to allocate a thread working area aligned as both position and
 * size.
 */
#define WorkingArea(s, n) uint8_t s[UserStackSize(n)];

/**
 * Enters the ChibiOS/RT system mutual exclusion zone, the implementation is
 * architecture dependent, on single core systems usually this function
 * just disables the interrupts.
 * @note The code in the system mutual exclusion zone must be as light and
 *       fast as possible, the system performance is affected by this.
 * @note The use of system mutual exclusion zones are not recommended in
 *       the user code, it is a better idea to use the Semaphores instead.
 */
#define chSysLock()

/**
 * Leaves the ChibiOS/RT system mutual exclusion zone, the implementation is
 * architecture dependent, on single core systems usually this function
 * just enables the interrupts.
 * @note The code in the system mutual exclusion zone must be as light and
 *       fast as possible, the system performance is affected by this.
 * @note The use of system mutual exclusion zones are not recommended in
 *       the user code, it is a better idea to use the Semaphores instead.
 */
#define chSysUnlock()

/**
 * Enables the interrupts, it is only invoked once into \p chSysInit().
 */
#define chSysEnable()

/**
 * IRQ handler enter code.
 * @note Usually IRQ handlers function are also declared naked.
 * @note On some architectures this macro can be empty.
 */
#define chSysIRQEnterI()

/**
 * IRQ handler exit code.
 * @note Usually IRQ handlers function are also declared naked.
 * @note This macro must perform the final reschedulation by using
 *       \p chSchRescRequiredI() and \p chSchDoRescheduleI().
 */
#define chSysIRQExitI()

#ifdef __cplusplus
extern "C" {
#endif
  void _IdleThread(void *p);
  void chSysHalt(void);
  void chSysSwitchI(Thread *otp, Thread *ntp);
  void chSysPuts(char *msg);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */

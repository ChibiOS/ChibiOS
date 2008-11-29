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
 * Base type for stack alignment.
 */
typedef uint8_t stkalign_t;

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
 * Per-thread stack overhead for interrupts servicing, it is used in the
 * calculation of the correct working area size.
 */
#define INT_REQUIRED_STACK 0

/**
 * Enforces a correct alignment for a stack area size value.
 * @deprecated Use STACK_ALIGN() instead, this macro will be removed in
 *             version 1.0.0.
 */
#define StackAlign(n) STACK_ALIGN(n)

/**
 * Enforces a correct alignment for a stack area size value.
 */
#define STACK_ALIGN(n) ((((n) - 1) | sizeof(stkalign_t)) + 1)

/**
 * Computes the thread working area global size.
 * @deprecated Use THD_WA_SIZE() instead, this macro will be removed in
 *             version 1.0.0.
 */
#define UserStackSize(n) THD_WA_SIZE(n)

 /**
  * Computes the thread working area global size.
  */
#define THD_WA_SIZE(n) StackAlign(sizeof(Thread) +                      \
                                  sizeof(struct intctx) +               \
                                  sizeof(struct extctx) +               \
                                  (n) + (INT_REQUIRED_STACK))

/**
 * Macro used to allocate a thread working area aligned as both position and
 * size.
 * @deprecated Use WORKING_AREA() instead, this macro will be removed in
 *             version 1.0.0.
 */
#define WorkingArea(s, n) WORKING_AREA(s, n)

/**
 * Macro used to allocate a thread working area aligned as both position and
 * size.
 */
#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n)];

/**
 * IRQ handler enter code.
 * @note Usually IRQ handlers functions are also declared naked.
 * @note On some architectures this macro can be empty.
 */
#define chSysIRQEnterI()

/**
 * IRQ handler exit code.
 * @note Usually IRQ handlers function are also declared naked.
 * @note This macro usually performs the final reschedulation by using
 *       \p chSchRescRequiredI() and \p chSchDoRescheduleI().
 */
#define chSysIRQExitI()

#ifdef __cplusplus
extern "C" {
#endif
  void _idle(void *p);
  void chSysHalt(void);
  void chSysEnable(void);
  void chSysLock(void);
  void chSysUnlock(void);
  void chSysSwitchI(Thread *otp, Thread *ntp);
  void chSysPuts(char *msg);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */

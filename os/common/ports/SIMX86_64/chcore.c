/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    SIMX86_64/chcore.c
 * @brief   Simulator on x86-64 port code.
 *
 * @addtogroup SIMX86_64_GCC_CORE
 * @{
 */

#include <sys/time.h>

#include "ch.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*
 * RTOS-specific context offset.
 */
#if defined(_CHIBIOS_RT_CONF_) || defined(_CHIBIOS_NIL_CONF_)
#define CONTEXT_OFFSET      __CH_OFFSETOF(thread_t, ctx)
#else
#error "invalid chconf.h"
#endif

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

bool port_isr_context_flag;
syssts_t port_irq_sts;

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * Performs a context switch between two threads.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched in
 */
__attribute__((used))
static void __dummy(thread_t *ntp, thread_t *otp) {
  (void)ntp; (void)otp;

  asm volatile (
                ".globl port_switch                             \n\t"
                "port_switch:"
                "push    %%rsi                                  \n\t"
                "push    %%rdi                                  \n\t"
                "push    %%r15                                  \n\t"
                "push    %%r14                                  \n\t"
                "push    %%r13                                  \n\t"
                "push    %%r12                                  \n\t"
                "push    %%rbx                                  \n\t"
                "push    %%rbp                                  \n\t"
                "movq    %%rsp, %c[ctx](%%rsi)                  \n\t"
                "movq    %c[ctx](%%rdi), %%rsp                  \n\t"
                "pop     %%rbp                                  \n\t"
                "pop     %%rbx                                  \n\t"
                "pop     %%r12                                  \n\t"
                "pop     %%r13                                  \n\t"
                "pop     %%r14                                  \n\t"
                "pop     %%r15                                  \n\t"
                "pop     %%rdi                                  \n\t"
                "pop     %%rsi                                  \n\t"
                "ret"
                :
                : [ctx] "i" (CONTEXT_OFFSET));
}

/**
 * @brief   Start a thread by invoking its work function.
 * @details If the work function returns @p chThdExit() is automatically
 *          invoked.
 */
__attribute__((noreturn))
void _port_thread_start(msg_t (*pf)(void *), void *p) {

  chSysUnlock();
  pf(p);
  chThdExit(0);
  while(1);
}


/**
 * @brief   Returns the current value of the realtime counter.
 *
 * @return              The realtime counter value.
 */
rtcnt_t port_rt_get_counter_value(void) {
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return ((rtcnt_t)tv.tv_sec * (rtcnt_t)1000000) + (rtcnt_t)tv.tv_usec;
}

/** @} */

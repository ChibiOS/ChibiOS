/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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

/**
 * @file    cosmic/STM8/chcore.h
 * @brief   STM8 (Cosmic) architecture port macros and structures.
 *
 * @addtogroup STM8_COSMIC_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

#if CH_DBG_ENABLE_STACK_CHECK
#error "option CH_DBG_ENABLE_STACK_CHECK not supported by this port"
#endif

/*===========================================================================*/
/* Port configurable parameters.                                             */
/*===========================================================================*/

/**
 * @brief   Enables the use of the WFI instruction in the idle thread loop.
 */
#ifndef STM8_ENABLE_WFI_IDLE
#define STM8_ENABLE_WFI_IDLE            FALSE
#endif

/*===========================================================================*/
/* Port exported info.                                                       */
/*===========================================================================*/

/**
 * @brief   Unique macro for the implemented architecture.
 */
#define CH_ARCHITECTURE_STM8

/**
 * @brief   Name of the implemented architecture.
 */
#define CH_ARCHITECTURE_NAME            "STM8"

/**
 * @brief   Name of the compiler supported by this port.
 */
#define CH_COMPILER_NAME                "Cosmic"

/**
 * @brief   Port-specific information string.
 */
#define CH_PORT_INFO                    "None"

/*===========================================================================*/
/* Port implementation part.                                                 */
/*===========================================================================*/

/**
 * @brief   Base type for stack alignment.
 * @note    No alignment constraints so uint8_t.
 */
typedef uint8_t stkalign_t;

/**
 * @brief   Generic STM8 function pointer.
 * @note    It is used to allocate the proper size for return addresses in
 *          context-related structures.
 */
typedef void (*stm8func_t)(void);

/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 * @note    The structure requires one dummy field at its start because the
 *          stack is handled as preincremented/postdecremented.
 */
struct extctx {
  uint8_t       _next;
  uint8_t       c_lreg[4];
  uint8_t       c_y[3];
  uint8_t       c_x[3];
  uint8_t       cc;
  uint8_t       a;
  uint16_t      x;
  uint16_t      y;
  uint8_t       pce;
  uint8_t       pch;
  uint8_t       pcl;
};

/**
 * @brief   System saved context.
 * @details This structure represents the inner stack frame during a context
 *          switching..
 * @note    The structure requires one dummy field at its start because the
 *          stack is handled as preincremented/postdecremented.
 */
struct intctx {
  uint8_t       _next;
  stm8func_t    pc;             /* Function pointer sized return address.   */
};

/**
 * @brief   Platform dependent part of the @p Thread structure.
 * @details This structure usually contains just the saved stack pointer
 *          defined as a pointer to a @p intctx structure.
 */
struct context {
  struct intctx *sp;
};

/**
 * @brief   Start context.
 * @details This context is the stack organization for the trampoline code
 *          @p _port_thread_start().
 */
struct stm8_startctx {
  uint8_t       _next;
  stm8func_t    ts;             /* Trampoline address.                      */
  void          *arg;           /* Thread argument.                         */
  stm8func_t    pc;             /* Thread function address.                 */
  stm8func_t    ret;            /* chThdExit() address.                     */
};

/**
 * @brief   Platform dependent part of the @p chThdCreateI() API.
 * @details This code usually setup the context switching frame represented
 *          by an @p intctx structure.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                          \
  struct stm8_startctx *scp;                                                \
  scp = (struct stm8_startctx *)((uint8_t *)workspace + wsize -             \
                                 sizeof(struct stm8_startctx));             \
  scp->ts   = _port_thread_start;                                           \
  scp->arg  = arg;                                                          \
  scp->pc   = (stm8func_t)pf;                                               \
  scp->ret  = (stm8func_t)chThdExit;                                        \
  tp->p_ctx.sp = (struct intctx *)scp;                                      \
}

/**
 * @brief   Stack size for the system idle thread.
 * @details This size depends on the idle thread implementation, usually
 *          the idle thread should take no more space than those reserved
 *          by @p PORT_INT_REQUIRED_STACK.
 */
#ifndef PORT_IDLE_THREAD_STACK_SIZE
#define PORT_IDLE_THREAD_STACK_SIZE     0
#endif

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This is a safe value, you may trim it down after reading the
 *          right size in the map file.
 */
#ifndef PORT_INT_REQUIRED_STACK
#define PORT_INT_REQUIRED_STACK         48
#endif

/**
 * @brief   Enforces a correct alignment for a stack area size value.
 */
#define STACK_ALIGN(n) ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

/**
 * @brief   Computes the thread working area global size.
 */
#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(Thread) +                         \
                                   (sizeof(struct intctx) - 1) +            \
                                   (sizeof(struct extctx) - 1) +            \
                                   (n) + (PORT_INT_REQUIRED_STACK))

/**
 * @brief   Static working area allocation.
 * @details This macro is used to allocate a static thread working area
 *          aligned as both position and size.
 */
#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)]

/**
 * @brief   IRQ prologue code.
 * @details This macro must be inserted at the start of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_PROLOGUE()

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_EPILOGUE() {                                               \
  dbg_check_lock();                                                         \
  if (chSchIsPreemptionRequired())                                          \
    chSchDoReschedule();                                                    \
  dbg_check_unlock();                                                       \
}

/**
 * @brief   IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#define PORT_IRQ_HANDLER(id) @far @interrupt @svlreg void vector##id(void)

/**
 * @brief   Port-related initialization code.
 * @note    None in this port.
 */
#define port_init()

/**
 * @brief   Kernel-lock action.
 * @note    Implemented as global interrupts disable.
 */
#define port_lock() _asm("sim")

/**
 * @brief   Kernel-unlock action.
 * @note    Implemented as global interrupts enable.
 */
#define port_unlock() _asm("rim")

/**
 * @brief   Kernel-lock action from an interrupt handler.
 * @note    This function is empty in this port.
 */
#define port_lock_from_isr()

/**
 * @brief   Kernel-unlock action from an interrupt handler.
 * @note    This function is empty in this port.
 */
#define port_unlock_from_isr()

/**
 * @brief   Disables all the interrupt sources.
 * @note    Implemented as global interrupts disable.
 * @note    Of course non-maskable interrupt sources are not included.
 */
#define port_disable() _asm("sim")

/**
 * @brief   Disables the interrupt sources that are not supposed to preempt
 *          the kernel.
 * @note    Same as @p port_disable() in this port, there is no difference
 *          between the two states.
 */
#define port_suspend() _asm("sim")

/**
 * @brief   Enables all the interrupt sources.
 * @note    Implemented as global interrupt enable.
 */
#define port_enable() _asm("rim")

/**
 * @brief   Enters an architecture-dependent halt mode.
 * @note    Implemented with the specific "wfi" instruction.
 */
#if STM8_ENABLE_WFI_IDLE || defined(__DOXYGEN__)
#define port_wait_for_interrupt() _asm("wfi")
#else
#define port_wait_for_interrupt()
#endif

/**
 * @brief   Performs a context switch between two threads.
 * @details This is the most critical code in any port, this function
 *          is responsible for the context switch between 2 threads.
 * @note    Implemented as a call to a low level assembler routine.
 *
 * @param ntp   the thread to be switched in
 * @param otp   the thread to be switched out
 */
#define port_switch(ntp, otp) _port_switch(otp)

#ifdef __cplusplus
extern "C" {
#endif
  void _port_switch(Thread *otp);
  void _port_thread_start(void);
  void port_halt(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Scheduler captured code.                                                  */
/*===========================================================================*/

#define PORT_OPTIMIZED_RLIST_VAR
#define PORT_OPTIMIZED_RLIST_EXT
#define PORT_OPTIMIZED_READYLIST_STRUCT

typedef struct {
  ThreadsQueue          r_queue;
  tprio_t               r_prio;
  Thread                *r_current;
#if CH_USE_REGISTRY
  Thread                *r_newer;
  Thread                *r_older;
#endif
  /* End of the fields shared with the Thread structure.*/
#if CH_TIME_QUANTUM > 0
  cnt_t                 r_preempt;
#endif
} ReadyList;

@tiny extern ReadyList rlist;

#endif /* _CHCORE_H_ */

/** @} */

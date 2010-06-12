/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    chsys.h
 * @brief   System related macros and structures.
 *
 * @addtogroup system
 * @{
 */

#ifndef _CHSYS_H_
#define _CHSYS_H_

/**
 * @brief   Halts the system.
 * @details This function is invoked by the operating system when an
 *          unrecoverable error is detected, as example because a programming
 *          error in the application code that triggers an assertion while
 *          in debug mode.
 */
#define chSysHalt() port_halt()

/**
 * @brief   Performs a context switch.
 *
 * @param[in] ntp       the thread to be switched in
 * @param[in] otp       the thread to be switched out
 */
#define chSysSwitchI(ntp, otp) port_switch(ntp, otp)

/**
 * @brief   Raises the system interrupt priority mask to the maximum level.
 * @details All the maskable interrupt sources are disabled regardless their
 *          hardware priority.
 * @note    The implementation is architecture dependent, it may just disable
 *          the interrupts or be exactly equivalent to @p chSysDisable().
 * @note    Do not invoke this API from within a kernel lock.
 */
#define chSysDisable() port_disable()

/**
 * @brief   Raises the system interrupt priority mask to system level.
 * @details The interrupt sources that should not be able to preempt the kernel
 *          are disabled, interrupt sources with higher priority are still
 *          enabled.
 * @note    The implementation is architecture dependent, it may just disable
 *          the interrupts.
 * @note    Do not invoke this API from within a kernel lock.
 * @note    This API is no replacement for @p chSysLock(), the @p chSysLock()
 *          could do more than just disable the interrupts.
 */
#define chSysSuspend() port_suspend()

/**
 * @brief   Lowers the system interrupt priority mask to user level.
 * @details All the interrupt sources are enabled.
 * @note    The implementation is architecture dependent, it may just enable
 *          the interrupts.
 * @note    Do not invoke this API from within a kernel lock.
 * @note    This API is no replacement for @p chSysUnlock(), the
 *          @p chSysUnlock() could do more than just enable the interrupts.
 */
#define chSysEnable() port_enable()

/**
 * @brief   Enters the kernel lock mode.
 * @note    The use of kernel lock mode is not recommended in the user code,
 *          it is a better idea to use the semaphores or mutexes instead.
 * @see     CH_USE_NESTED_LOCKS
 */
#if CH_USE_NESTED_LOCKS || defined(__DOXYGEN__)
#if CH_OPTIMIZE_SPEED || defined(__DOXYGEN__)
#define chSysLock() {                                                   \
  if (currp->p_locks++ == 0)                                            \
    port_lock();                                                        \
}
#endif /* CH_OPTIMIZE_SPEED */
#else /* !CH_USE_NESTED_LOCKS */
#define chSysLock() port_lock()
#endif /* !CH_USE_NESTED_LOCKS */

/**
 * @brief   Leaves the kernel lock mode.
 * @note    The use of kernel lock mode is not recommended in the user code,
 *          it is a better idea to use the semaphores or mutexes instead.
 * @see     CH_USE_NESTED_LOCKS
 */
#if CH_USE_NESTED_LOCKS || defined(__DOXYGEN__)
#if CH_OPTIMIZE_SPEED || defined(__DOXYGEN__)
#define chSysUnlock() {                                                 \
  if (--currp->p_locks == 0)                                            \
    port_unlock();                                                      \
}
#endif /* CH_OPTIMIZE_SPEED */
#else /* !CH_USE_NESTED_LOCKS */
#define chSysUnlock() port_unlock()
#endif /* !CH_USE_NESTED_LOCKS */

/**
 * @brief   Enters the kernel lock mode from within an interrupt handler.
 * @note    This API may do nothing on some architectures, it is required
 *          because on ports that support preemptable interrupt handlers
 *          it is required to raise the interrupt mask to the same level of
 *          the system mutual exclusion zone.<br>
 *          It is good practice to invoke this API before invoking any I-class
 *          syscall from an interrupt handler.
 * @note    This API must be invoked exclusively from interrupt handlers.
 */
#define chSysLockFromIsr() port_lock_from_isr()

/**
 * @brief   Leaves the kernel lock mode from within an interrupt handler.
 *
 * @note    This API may do nothing on some architectures, it is required
 *          because on ports that support preemptable interrupt handlers
 *          it is required to raise the interrupt mask to the same level of
 *          the system mutual exclusion zone.<br>
 *          It is good practice to invoke this API after invoking any I-class
 *          syscall from an interrupt handler.
 * @note This API must be invoked exclusively from interrupt handlers.
 */
#define chSysUnlockFromIsr() port_unlock_from_isr()

/**
 * @brief   IRQ handler enter code.
 * @note    Usually IRQ handlers functions are also declared naked.
 * @note    On some architectures this macro can be empty.
 */
#define CH_IRQ_PROLOGUE() PORT_IRQ_PROLOGUE()

/**
 * @brief   IRQ handler exit code.
 * @note    Usually IRQ handlers function are also declared naked.
 * @note    This macro usually performs the final reschedule by using
 *          @p chSchRescRequiredI() and @p chSchDoRescheduleI().
 */
#define CH_IRQ_EPILOGUE() PORT_IRQ_EPILOGUE()

/**
 * @brief   Standard normal IRQ handler declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#define CH_IRQ_HANDLER(id) PORT_IRQ_HANDLER(id)

/**
 * @brief   Standard fast IRQ handler declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 * @note    Not all architectures support fast interrupts.
 */
#define CH_FAST_IRQ_HANDLER(id) PORT_FAST_IRQ_HANDLER(id)

#ifdef __cplusplus
extern "C" {
#endif
  void chSysInit(void);
  void chSysTimerHandlerI(void);
#if CH_USE_NESTED_LOCKS && !CH_OPTIMIZE_SPEED
  void chSysLock(void);
  void chSysUnlock(void);
#endif /* CH_USE_NESTED_LOCKS && !CH_OPTIMIZE_SPEED */
#ifdef __cplusplus
}
#endif

#endif /* _CHSYS_H_ */

/** @} */

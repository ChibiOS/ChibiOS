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
 * @addtogroup System
 * @{
 */

#ifndef _SYS_H_
#define _SYS_H_

/**
 * Prints a message on the system console (if any).
 * @param msg the message to be printed on the system console
 */
#define chSysPuts(msg) sys_puts(msg)

/**
 * Halts the system. This function is invoked by the operating system when an
 * unrecoverable error is detected (as example because a programming error in
 * the application code that triggers an assertion while in debug mode).
 */
#define chSysHalt() sys_halt()

/**
 * Performs a context switch.
 * This is the most critical code in any port, this function is responsible
 * for the context switch between 2 threads.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched in
 * @note The implementation of this code affects <b>directly</b> the context
 *       switch performance so optimize here as much as you can.
 */
#define chSysSwitchI(otp, ntp) sys_switch(otp, ntp)

/**
 * Lowers the system interrupt priority mask to user level.
 * @note The implementation is architecture dependent, it may just enable the
 *       interrupts.
 * @note This API is normally invoked only from within @p chSysInit().
 * @note The use of this API is <b>not</b> an alternative to @p chSysUnlock().
 */
#define chSysEnable() sys_enable()

/**
 * Raises the system interrupt priority mask to system level.
 * @note The implementation is architecture dependent, it may just disable the
 *       interrupts.
 * @note This API should only be invoked from the main thread in order to stop
 *       ChibiOS/RT, hardware de/re-initialization should follow. It would then
 *       be possible to re-initialize ChibiOS/RT using @p chSysInit().
 * @note The use of this API is <b>not</b> an alternative to @p chSysLock().
 */
#define chSysDisable() sys_disable()

/**
 * Raises the system interrupt priority mask to the maximum level thus disabling
 * any mask-able interrupt source..
 * @note The implementation is architecture dependent, it may just disable the
 *       interrupts or be exactly equivalent to @p chSysDisable().
 */
#define chSysDisableAll() sys_disable_all()

/**
 * Enters the ChibiOS/RT system mutual exclusion zone from within an interrupt
 * handler.
 * @note This API may do nothing on some architectures, it is required because
 *       on ports that support preemptable interrupt handlers it is required to
 *       raise the interrupt mask to the same level of the system mutual
 *       exclusion zone.<br>
 *       It is good practice to invoke this API before invoking any I-class
 *       syscall from an interrupt handler.
 * @note This API must be invoked exclusively from interrupt handlers.
 */
#define chSysLockI() sys_disable_from_isr()

/**
 * Leaves the ChibiOS/RT system mutual exclusion zone from within an interrupt
 * handler.
 * @note This API may do nothing on some architectures, it is required because
 *       on ports that support preemptable interrupt handlers it is required to
 *       raise the interrupt mask to the same level of the system mutual
 *       exclusion zone.<br>
 *       It is good practice to invoke this API after invoking any I-class
 *       syscall from an interrupt handler.
 * @note This API must be invoked exclusively from interrupt handlers.
 */
#define chSysUnlockI() sys_enable_from_isr()

#if defined(CH_USE_NESTED_LOCKS) || defined(_DOXYGEN_)
/**
 * Enters the ChibiOS/RT system mutual exclusion zone.
 * @note The use of system mutual exclusion zone is not recommended in
 *       the user code, it is a better idea to use the semaphores or mutexes
 *       instead.
 * @note The code of this API is always inlined regardless the
 *       @p CH_OPTIMIZE_SPEED setting. This function is meant to be used in
 *       places where the performance is always preferred.
 * @see CH_USE_NESTED_LOCKS
 */
#define chSysLockInline() {                                             \
  if (currp->p_locks == 0) {                                            \
    currp->p_locks++;                                                   \
    sys_disable();                                                      \
  }                                                                     \
}

/**
 * Leaves the ChibiOS/RT system mutual exclusion zone.
 * @note The use of system mutual exclusion zone is not recommended in
 *       the user code, it is a better idea to use the semaphores or mutexes
 *       instead.
 * @note The code of this API is always inlined regardless the
 *       @p CH_OPTIMIZE_SPEED setting. This function is meant to be used in
 *       places where the performance is always preferred.
 * @see CH_USE_NESTED_LOCKS
 */
#define chSysUnlockInline() {                                           \
  if (--currp->p_locks == 0)                                            \
    sys_enable();                                                       \
}

#else /* defined(CH_USE_NESTED_LOCKS) || defined(_DOXYGEN_) */

#define chSysLockInline() sys_disable()
#define chSysUnlockInline() sys_enable()

#endif /* !defined(CH_USE_NESTED_LOCKS) && !defined(_DOXYGEN_) */

/**
 * IRQ handler enter code.
 * @note Usually IRQ handlers functions are also declared naked.
 * @note On some architectures this macro can be empty.
 */
#define chSysIRQEnterI() SYS_IRQ_PROLOGUE()

/**
 * IRQ handler exit code.
 * @note Usually IRQ handlers function are also declared naked.
 * @note This macro usually performs the final reschedulation by using
 *       \p chSchRescRequiredI() and \p chSchDoRescheduleI().
 */
#define chSysIRQExitI() SYS_IRQ_EPILOGUE()

/**
 * Standard modifier for IRQ handler functions.
 */
#define CH_IRQ_HANDLER SYS_IRQ_HANDLER

/*
 * Inlined code when CH_OPTIMIZE_SPEED is defined.
 */
#if defined(CH_OPTIMIZE_SPEED)
#define chSysLock() chSysLockInline()
#define chSysUnlock() chSysUnlockInline()
#endif /* defined(CH_OPTIMIZE_SPEED) */

#ifdef __cplusplus
extern "C" {
#endif
  void chSysInit(void);
  void chSysTimerHandlerI(void);
#if !defined(CH_OPTIMIZE_SPEED)
  void chSysLock(void);
  void chSysUnlock(void);
#endif /* !defined(CH_OPTIMIZE_SPEED) */
#ifdef __cplusplus
}
#endif

#endif /* _SYS_H_ */

/** @} */

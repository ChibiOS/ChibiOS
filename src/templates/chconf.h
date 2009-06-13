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

/**
 * @file src/templates/chconf.h
 * @brief Configuration file template.
 * @addtogroup Config
 * @{
 */

#ifndef _CHCONF_H_
#define _CHCONF_H_

/*===========================================================================*/
/* Kernel parameters.                                                        */
/*===========================================================================*/

/**
 * Frequency of the system timer that drives the system ticks. This also
 * defines the system tick time unit.
 */
#if !defined(CH_FREQUENCY) || defined(__DOXYGEN__)
#define CH_FREQUENCY                    1000
#endif

/**
 * This constant is the number of system ticks allowed for the threads before
 * preemption occurs. This option is only meaningful if the option
 * @p CH_USE_ROUNDROBIN is also active.
 */
#if !defined(CH_TIME_QUANTUM) || defined(__DOXYGEN__)
#define CH_TIME_QUANTUM                 20
#endif

/**
 * If enabled then the use of nested @p chSysLock() / @p chSysUnlock()
 * operations is allowed.<br>
 * For performance and code size reasons the recommended setting is to leave
 * this option disabled.<br>
 * You can use this option if you need to merge ChibiOS/RT with external
 * libraries that require nested lock/unlock operations.
 * @note The default is @p FALSE.
 */
#if !defined(CH_USE_NESTED_LOCKS) || defined(__DOXYGEN__)
#define CH_USE_NESTED_LOCKS             FALSE
#endif

/**
 * If specified then the kernel performs the round robin scheduling algorithm
 * on threads of equal priority.
 * @note The default is @p TRUE.
 */
#if !defined(CH_USE_ROUNDROBIN) || defined(__DOXYGEN__)
#define CH_USE_ROUNDROBIN               TRUE
#endif

/**
 * Number of RAM bytes to use as system heap. If set to zero then the whole
 * available RAM is used as system heap.
 * @note In order to use the whole RAM as system heap the linker script must
 *       provide the @p __heap_base__ and @p __heap_end__ symbols.
 * @note Requires @p CH_USE_HEAP.
 */
#if !defined(CH_HEAP_SIZE) || defined(__DOXYGEN__)
#define CH_HEAP_SIZE                    0
#endif

/*===========================================================================*/
/* Performance options.                                                      */
/*===========================================================================*/

/**
 * If specified then time efficient rather than space efficient code is used
 * when two possible implementations exist.
 * @note This is not related to the compiler optimization options.
 * @note The default is @p TRUE.
 */
#if !defined(CH_OPTIMIZE_SPEED) || defined(__DOXYGEN__)
#define CH_OPTIMIZE_SPEED               TRUE
#endif

/**
 * If enabled defines a CPU register to be used as storage for the global
 * @p currp variable. Caching this variable in a register can greatly
 * improve both space and time efficiency of the generated code. Another side
 * effect is that one less register has to be saved during the context switch
 * resulting in lower RAM usage and faster code.
 * @note This option is only usable with the GCC compiler and is only useful
 *       on processors with many registers like ARM cores.
 * @note If this option is enabled then ALL the libraries linked to the
 *       ChibiOS/RT code <b>must</b> be recompiled with the GCC option @p
 *       -ffixed-@<reg@>.
 * @note This option must be enabled in the Makefile, it is listed here for
 *       documentation.
 */
#if defined(__DOXYGEN__)
#define CH_CURRP_REGISTER_CACHE         "reg"
#endif

/*===========================================================================*/
/* Subsystem options.                                                        */
/*===========================================================================*/

/**
 * If specified then the @p chThdWait() function is included in the kernel.
 * @note The default is @p TRUE.
 */
#if !defined(CH_USE_WAITEXIT) || defined(__DOXYGEN__)
#define CH_USE_WAITEXIT                 TRUE
#endif

/**
 * If specified then the Semaphores APIs are included in the kernel.
 * @note The default is @p TRUE.
 */
#if !defined(CH_USE_SEMAPHORES) || defined(__DOXYGEN__)
#define CH_USE_SEMAPHORES               TRUE
#endif

/**
 * If enabled then the threads are enqueued on semaphores by priority rather
 * than FIFO order.
 * @note The default is @p FALSE. Enable this if you have special requirements.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#if !defined(CH_USE_SEMAPHORES_PRIORITY) || defined(__DOXYGEN__)
#define CH_USE_SEMAPHORES_PRIORITY      FALSE
#endif

/**
 * If specified then the Semaphores the @p chSemWaitSignal() API is included
 * in the kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#if !defined(CH_USE_SEMSW) || defined(__DOXYGEN__)
#define CH_USE_SEMSW                    TRUE
#endif

/**
 * If specified then the Semaphores with timeout APIs are included in the
 * kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#if !defined(CH_USE_SEMAPHORES_TIMEOUT) || defined(__DOXYGEN__)
#define CH_USE_SEMAPHORES_TIMEOUT       TRUE
#endif

/**
 * If specified then the Mutexes APIs are included in the kernel.
 * @note The default is @p TRUE.
 */
#if !defined(CH_USE_MUTEXES) || defined(__DOXYGEN__)
#define CH_USE_MUTEXES                  TRUE
#endif

/**
 * If specified then the Conditional Variables APIs are included in the kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_MUTEXES.
 */
#if !defined(CH_USE_CONDVARS) || defined(__DOXYGEN__)
#define CH_USE_CONDVARS                 TRUE
#endif

/**
 * If specified then the Conditional Variables APIs are included in the kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_CONDVARS.
 */
#if !defined(CH_USE_CONDVARS_TIMEOUT) || defined(__DOXYGEN__)
#define CH_USE_CONDVARS_TIMEOUT         TRUE
#endif

/**
 * If specified then the Event flags APIs are included in the kernel.
 * @note The default is @p TRUE.
 */
#if !defined(CH_USE_EVENTS) || defined(__DOXYGEN__)
#define CH_USE_EVENTS                   TRUE
#endif

/**
 * If specified then the @p chEvtWaitXXXTimeout() functions are included in
 * the kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_EVENTS.
 */
#if !defined(CH_USE_EVENTS_TIMEOUT) || defined(__DOXYGEN__)
#define CH_USE_EVENTS_TIMEOUT           TRUE
#endif

/**
 * If specified then the Synchronous Messages APIs are included in the kernel.
 * @note The default is @p TRUE.
 */
#if !defined(CH_USE_MESSAGES) || defined(__DOXYGEN__)
#define CH_USE_MESSAGES                 TRUE
#endif

/**
 * If specified then the @p chMsgSendWithEvent() function is included in the
 * kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_MESSAGES and @p CH_USE_EVENTS.
 */
#if !defined(CH_USE_MESSAGES_EVENT) || defined(__DOXYGEN__)
#define CH_USE_MESSAGES_EVENT           TRUE
#endif

/**
 * If enabled then messages are served by priority rather than in FIFO order.
 * @note The default is @p FALSE. Enable this if you have special requirements.
 * @note Requires @p CH_USE_MESSAGES.
 */
#if !defined(CH_USE_MESSAGES_PRIORITY) || defined(__DOXYGEN__)
#define CH_USE_MESSAGES_PRIORITY        FALSE
#endif

/**
 * If specified then the Asynchronous Messages (Mailboxes) APIs are included
 * in the kernel.
 * @note The default is @p TRUE.
 */
#if !defined(CH_USE_MAILBOXES) || defined(__DOXYGEN__)
#define CH_USE_MAILBOXES                TRUE
#endif

/**
 * If specified then the I/O queues APIs are included in the kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#if !defined(CH_USE_QUEUES) || defined(__DOXYGEN__)
#define CH_USE_QUEUES                   TRUE
#endif

/**
 * If specified then the half duplex queues APIs are included in the kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#if !defined(CH_USE_QUEUES_HALFDUPLEX) || defined(__DOXYGEN__)
#define CH_USE_QUEUES_HALFDUPLEX        TRUE
#endif

/**
 * If specified then the I/O queues with timeout APIs are included in the
 * kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_QUEUES and @p CH_USE_SEMAPHORES_TIMEOUT.
 */
#if !defined(CH_USE_QUEUES_TIMEOUT) || defined(__DOXYGEN__)
#define CH_USE_QUEUES_TIMEOUT           TRUE
#endif

/**
 * If specified then the full duplex serial driver APIs are included in the
 * kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_QUEUES.
 */
#if !defined(CH_USE_SERIAL_FULLDUPLEX) || defined(__DOXYGEN__)
#define CH_USE_SERIAL_FULLDUPLEX        TRUE
#endif

/**
 * If specified then the half duplex serial driver APIs are included in the
 * kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_QUEUES_HALFDUPLEX.
 */
#if !defined(CH_USE_SERIAL_HALFDUPLEX) || defined(__DOXYGEN__)
#define CH_USE_SERIAL_HALFDUPLEX        TRUE
#endif

/**
 * If specified then the memory heap allocator APIs are included in the kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_MUTEXES or @p CH_USE_SEMAPHORES.
 * @note Mutexes are recommended.
 */
#if !defined(CH_USE_HEAP) || defined(__DOXYGEN__)
#define CH_USE_HEAP                     TRUE
#endif

/**
 * If enabled enforces the use of the C-runtime @p malloc() and @p free()
 * functions as backend for the system heap allocator.
 * @note The default is @p FALSE.
 * @note Requires @p CH_USE_HEAP.
 */
#if !defined(CH_USE_MALLOC_HEAP) || defined(__DOXYGEN__)
#define CH_USE_MALLOC_HEAP              FALSE
#endif

/**
 * If specified then the memory pools allocator APIs are included in the
 * kernel.
 * @note The default is @p TRUE.
 */
#if !defined(CH_USE_MEMPOOLS) || defined(__DOXYGEN__)
#define CH_USE_MEMPOOLS                 TRUE
#endif

/**
 * If specified then the dynamic threads creation APIs are included in the
 * kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_WAITEXIT.
 */
#if !defined(CH_USE_DYNAMIC) || defined(__DOXYGEN__)
#define CH_USE_DYNAMIC                  TRUE
#endif

/*===========================================================================*/
/* Debug options.                                                            */
/*===========================================================================*/

/**
 * Debug option, if enabled then the checks on the API functions input
 * parameters are activated.
 * @note The default is @p FALSE.
 */
#if !defined(CH_DBG_ENABLE_CHECKS) || defined(__DOXYGEN__)
#define CH_DBG_ENABLE_CHECKS            FALSE
#endif

/**
 * Debug option, if enabled then all the assertions in the kernel code are
 * activated. This includes consistency checks inside the kernel, runtime
 * anomalies and port-defined checks.
 * @note The default is @p FALSE.
 */
#if !defined(CH_DBG_ENABLE_ASSERTS) || defined(__DOXYGEN__)
#define CH_DBG_ENABLE_ASSERTS           FALSE
#endif

/**
 * Debug option, if enabled the context switch circular trace buffer is
 * activated.
 * @note The default is @p FALSE.
 */
#if !defined(CH_DBG_ENABLE_TRACE) || defined(__DOXYGEN__)
#define CH_DBG_ENABLE_TRACE             FALSE
#endif

/**
 * Debug option, if enabled a runtime stack check is performed.
 * @note The stack check is performed in a architecture/port dependent way. It
 *       may not be implemented at all.
 */
#if !defined(CH_DBG_ENABLE_STACK_CHECK) || defined(__DOXYGEN__)
#define CH_DBG_ENABLE_STACK_CHECK       FALSE
#endif

/**
 * Debug option, if enabled the threads working area is filled with a byte
 * pattern when a thread is created.
 */
#if !defined(CH_DBG_FILL_THREADS) || defined(__DOXYGEN__)
#define CH_DBG_FILL_THREADS             FALSE
#endif

/**
 * Debug option, if enabled a field is added to the @p Thread structure that
 * counts the system ticks occurred while executing the thread.
 */
#if !defined(CH_DBG_THREADS_PROFILING) || defined(__DOXYGEN__)
#define CH_DBG_THREADS_PROFILING        FALSE
#endif

/*===========================================================================*/
/* Kernel hooks.                                                             */
/*===========================================================================*/

/**
 * User fields added to the end of the @p Thread structure.
 */
#if !defined(THREAD_EXT_FIELDS) || defined(__DOXYGEN__)
#define THREAD_EXT_FIELDS                                               \
struct {                                                                \
  /* Add thread custom fields here.*/                                   \
};
#endif

/**
 * User initialization code added to the @p chThdInit() API.
 * @note It is invoked from within @p chThdInit().
 */
#if !defined(THREAD_EXT_INIT) || defined(__DOXYGEN__)
#define THREAD_EXT_INIT(tp) {                                           \
  /* Add thread initialization code here.*/                             \
}
#endif

/**
 * User finalization code added to the @p chThdExit() API.
 * @note It is inserted into lock zone.
 */
#if !defined(THREAD_EXT_EXIT) || defined(__DOXYGEN__)
#define THREAD_EXT_EXIT(tp) {                                           \
  /* Add thread finalization code here.*/                               \
}
#endif

/**
 * Code inserted inside the idle thread loop immediately after an interrupt
 * resumed execution.
 */
#if !defined(IDLE_LOOP_HOOK) || defined(__DOXYGEN__)
#define IDLE_LOOP_HOOK() {                                              \
  /* Idle loop code here.*/                                             \
}
#endif

#endif  /* _CHCONF_H_ */

/** @} */

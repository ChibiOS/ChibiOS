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
 * @file src/templates/chconf.h
 * @brief Configuration file template.
 * @addtogroup Config
 * @{
 */

#ifndef _CHCONF_H_
#define _CHCONF_H_

/**
 * If specified then time efficient rather than space efficient code is used
 * when two possible implementations exist.
 * @note This is not related to the compiler optimization options.
 * @note The default is @p TRUE.
 */
#ifndef CH_OPTIMIZE_SPEED
#define CH_OPTIMIZE_SPEED               TRUE
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
#ifndef CH_USE_NESTED_LOCKS
#define CH_USE_NESTED_LOCKS             FALSE
#endif

/**
 * If specified then the kernel performs the round robin scheduling algorithm
 * on threads of equal priority.
 * @note The default is @p TRUE.
 */
#ifndef CH_USE_ROUNDROBIN
#define CH_USE_ROUNDROBIN               TRUE
#endif

/**
 * If specified then the @p chThdWait() function is included in the kernel.
 * @note The default is @p TRUE.
 */
#ifndef CH_USE_WAITEXIT
#define CH_USE_WAITEXIT                 TRUE
#endif

/**
 * If specified then the Semaphores APIs are included in the kernel.
 * @note The default is @p TRUE.
 */
#ifndef CH_USE_SEMAPHORES
#define CH_USE_SEMAPHORES               TRUE
#endif

/**
 * If enabled then the threads are enqueued on semaphores by priority rather
 * than FIFO order.
 * @note The default is @p FALSE. Enable this if you have special requirements.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#ifndef CH_USE_SEMAPHORES_PRIORITY
#define CH_USE_SEMAPHORES_PRIORITY      FALSE
#endif

/**
 * If specified then the Semaphores the @p chSemWaitSignal() API is included
 * in the kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#ifndef CH_USE_SEMSW
#define CH_USE_SEMSW                    TRUE
#endif

/**
 * If specified then the Semaphores with timeout APIs are included in the
 * kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#ifndef CH_USE_SEMAPHORES_TIMEOUT
#define CH_USE_SEMAPHORES_TIMEOUT       TRUE
#endif

/**
 * If specified then the Mutexes APIs are included in the kernel.
 * @note The default is @p TRUE.
 */
#ifndef CH_USE_MUTEXES
#define CH_USE_MUTEXES                  TRUE
#endif

/**
 * If specified then the Conditional Variables APIs are included in the kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_MUTEXES.
 */
#ifndef CH_USE_CONDVARS
#define CH_USE_CONDVARS                 TRUE
#endif

/**
 * If specified then the Conditional Variables APIs are included in the kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_CONDVARS.
 */
#ifndef CH_USE_CONDVARS_TIMEOUT
#define CH_USE_CONDVARS_TIMEOUT         TRUE
#endif

/**
 * If specified then the Event flags APIs are included in the kernel.
 * @note The default is @p TRUE.
 */
#ifndef CH_USE_EVENTS
#define CH_USE_EVENTS                   TRUE
#endif

/**
 * If specified then the @p chEvtWaitXXXTimeout() functions are included in
 * the kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_EVENTS.
 */
#ifndef CH_USE_EVENTS_TIMEOUT
#define CH_USE_EVENTS_TIMEOUT           TRUE
#endif

/**
 * If specified then the Synchronous Messages APIs are included in the kernel.
 * @note The default is @p TRUE.
 */
#ifndef CH_USE_MESSAGES
#define CH_USE_MESSAGES                 TRUE
#endif

/**
 * If specified then the @p chMsgSendWithEvent() function is included in the
 * kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_MESSAGES and @p CH_USE_EVENTS.
 */
#ifndef CH_USE_MESSAGES_EVENT
#define CH_USE_MESSAGES_EVENT           TRUE
#endif

/**
 * If enabled then messages are served by priority rather than in FIFO order.
 * @note The default is @p FALSE. Enable this if you have special requirements.
 * @note Requires @p CH_USE_MESSAGES.
 */
#ifndef CH_USE_MESSAGES_PRIORITY
#define CH_USE_MESSAGES_PRIORITY        FALSE
#endif

/**
 * If specified then the I/O queues APIs are included in the kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#ifndef CH_USE_QUEUES
#define CH_USE_QUEUES                   TRUE
#endif

/**
 * If specified then the half duplex queues APIs are included in the kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#ifndef CH_USE_QUEUES_HALFDUPLEX
#define CH_USE_QUEUES_HALFDUPLEX        TRUE
#endif

/**
 * If specified then the I/O queues with timeout APIs are included in the
 * kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_QUEUES and @p CH_USE_SEMAPHORES_TIMEOUT.
 */
#ifndef CH_USE_QUEUES_TIMEOUT
#define CH_USE_QUEUES_TIMEOUT           TRUE
#endif

/**
 * If specified then the full duplex serial driver APIs are included in the
 * kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_QUEUES.
 */
#ifndef CH_USE_SERIAL_FULLDUPLEX
#define CH_USE_SERIAL_FULLDUPLEX        TRUE
#endif

/**
 * If specified then the half duplex serial driver APIs are included in the
 * kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_QUEUES_HALFDUPLEX.
 */
#ifndef CH_USE_SERIAL_HALFDUPLEX
#define CH_USE_SERIAL_HALFDUPLEX        TRUE
#endif

/**
 * If specified then the memory heap allocator APIs are included in the kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_MUTEXES or @p CH_USE_SEMAPHORES.
 * @note Mutexes are recommended.
 */
#ifndef CH_USE_HEAP
#define CH_USE_HEAP                     TRUE
#endif

/**
 * Number of RAM bytes to use as system heap. If set to zero then the whole
 * available RAM is used as system heap.
 * @note In order to use the whole RAM as system heap the linker script must
 *       provide the @p __heap_base__ and @p __heap_end__ symbols.
 * @note Requires @p CH_USE_HEAP.
 */
#ifndef CH_HEAP_SIZE
#define CH_HEAP_SIZE                    0
#endif

/**
 * If enabled enforces the use of the C-runtime @p malloc() and @p free()
 * functions as backend for the system heap allocator.
 * @note The default is @p FALSE.
 * @note Requires @p CH_USE_HEAP.
 */
#ifndef CH_USE_MALLOC_HEAP
#define CH_USE_MALLOC_HEAP              FALSE
#endif

/**
 * If specified then the memory pools allocator APIs are included in the
 * kernel.
 * @note The default is @p TRUE.
 */
#ifndef CH_USE_MEMPOOLS
#define CH_USE_MEMPOOLS                 TRUE
#endif

/**
 * If specified then the dynamic threads creation APIs are included in the
 * kernel.
 * @note The default is @p TRUE.
 * @note Requires @p CH_USE_WAITEXIT.
 */
#ifndef CH_USE_DYNAMIC
#define CH_USE_DYNAMIC                  TRUE
#endif

/**
 * Frequency of the system timer that drives the system ticks. This also
 * defines the system tick time unit.
 */
#ifndef CH_FREQUENCY
#define CH_FREQUENCY                    1000
#endif

/**
 * This constant is the number of system ticks allowed for the threads before
 * preemption occurs. This option is only meaningful if the option
 * @p CH_USE_ROUNDROBIN is also active.
 */
#ifndef CH_TIME_QUANTUM
#define CH_TIME_QUANTUM                 20
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

/**
 * Debug option, if enabled all the assertions in the kernel code are
 * activated. This includes function parameters checks and consistency
 * checks inside the kernel.
 * @note The default is @p FALSE.
 */
#ifndef CH_DBG_ENABLE_ASSERTS
#define CH_DBG_ENABLE_ASSERTS           FALSE
#endif

/**
 * Debug option, if enabled the context switch circular trace buffer is
 * activated.
 * @note The default is @p FALSE.
 */
#ifndef CH_DBG_ENABLE_TRACE
#define CH_DBG_ENABLE_TRACE             FALSE
#endif

/**
 * Debug option, if enabled the threads working area is filled with a byte
 * pattern when a thread is created.
 */
#ifndef CH_DBG_FILL_THREADS
#define CH_DBG_FILL_THREADS             FALSE
#endif

/**
 * User fields added to the end of the @p Thread structure.
 */
#ifndef THREAD_EXT_FIELDS
#define THREAD_EXT_FIELDS                                               \
struct {                                                                \
  /* Add thread custom fields here.*/                                   \
};
#endif

/**
 * User initialization code added to the @p chThdInit() API.
 * @note It is invoked from within @p chThdInit().
 */
#ifndef THREAD_EXT_INIT
#define THREAD_EXT_INIT(tp) {                                           \
  /* Add thread initialization code here.*/                             \
}
#endif

/**
 * User finalization code added to the @p chThdExit() API.
 * @note It is inserted into lock zone.
 */
#ifndef THREAD_EXT_EXIT
#define THREAD_EXT_EXIT(tp) {                                           \
  /* Add thread finalization code here.*/                               \
}
#endif

/**
 * Code inserted inside the idle thread loop immediately after an interrupt
 * resumed execution.
 */
#ifndef IDLE_LOOP_HOOK
#define IDLE_LOOP_HOOK() {                                              \
  /* Idle loop code here.*/                                             \
}
#endif

#endif  /* _CHCONF_H_ */

/** @} */

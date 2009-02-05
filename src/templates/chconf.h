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

/*
 * NOTE: this is just documentation for doxigen, the real configuration file
 * is the one into the project directories.
 */

/**
 * If specified then time efficient rather than space efficient code is used
 * when two possible implementations exist.
 * @note This is not related to the compiler optimization options.
 */
#define CH_OPTIMIZE_SPEED

/**
 * If enabled then the use of nested @p chSysLock() / @p chSysUnlock()
 * operations is allowed.<br>
 * For performance and code size reasons the recommended setting is to leave
 * this option disabled.<br>
 * You can use this option if you need to merge ChibiOS/RT with external
 * libraries that require nested lock/unlock operations.
 */
#define CH_USE_NESTED_LOCKS

/**
 * If specified then the kernel performs the round robin scheduling algorithm
 * on threads of equal priority.
 * @note The default is ON.
 */
#define CH_USE_ROUNDROBIN

/**
 * If specified then the @p chThdWait() function is included in the kernel.
 */
#define CH_USE_WAITEXIT

/**
 * If specified then the Semaphores APIs are included in the kernel.
 */
#define CH_USE_SEMAPHORES

/**
 * If enabled then the threads are enqueued on semaphores by priority rather
 * than FIFO order.
 * @note The recommended default is OFF unless you have some specific
 *       requirements.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#define CH_USE_SEMAPHORES_PRIORITY

/**
 * If specified then the Semaphores the @p chSemWaitSignal() API is included
 * in the kernel.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#define CH_USE_SEMSW

/**
 * If specified then the Semaphores with timeout APIs are included in the
 * kernel.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#define CH_USE_SEMAPHORES_TIMEOUT

/**
 * If specified then the Mutexes APIs are included in the kernel.
 */
#define CH_USE_MUTEXES

/**
 * If specified then the Conditional Variables APIs are included in the kernel.
 * @note Requires @p CH_USE_MUTEXES.
 */
#define CH_USE_CONDVARS

/**
 * If specified then the Conditional Variables APIs are included in the kernel.
 * @note Requires @p CH_USE_CONDVARS.
 */
#define CH_USE_CONDVARS_TIMEOUT

/**
 * If specified then the Event flags APIs are included in the kernel.
 */
#define CH_USE_EVENTS

/**
 * If specified then the @p chEvtWaitXXXTimeout() functions are included in
 * the kernel.
 * @note Requires @p CH_USE_EVENTS.
 */
#define CH_USE_EVENTS_TIMEOUT

/**
 * If specified then the Synchronous Messages APIs are included in the kernel.
 */
#define CH_USE_MESSAGES

/**
 * If specified then the @p chMsgSendWithEvent() function is included in the
 * kernel.
 * @note Requires @p CH_USE_MESSAGES and @p CH_USE_EVENTS.
 */
#define CH_USE_MESSAGES_EVENT

/**
 * If enabled then messages are served by priority rather than in FIFO order.
 * @note Requires @p CH_USE_MESSAGES.
 */
#define CH_USE_MESSAGES_PRIORITY

/**
 * If specified then the I/O queues APIs are included in the kernel.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#define CH_USE_QUEUES

/**
 * If specified then the half duplex queues APIs are included in the kernel.
 * @note Requires @p CH_USE_SEMAPHORES.
 */
#define CH_USE_QUEUES_HALFDUPLEX

/**
 * If specified then the I/O queues with timeout APIs are included in the
 * kernel.
 * @note Requires @p CH_USE_QUEUES and @p CH_USE_SEMAPHORES_TIMEOUT.
 */
#define CH_USE_QUEUES_TIMEOUT

/**
 * If specified then the full duplex serial driver APIs are included in the
 * kernel.
 * @note Requires @p CH_USE_QUEUES.
 */
#define CH_USE_SERIAL_FULLDUPLEX

/**
 * If specified then the half duplex serial driver APIs are included in the
 * kernel.
 * @note Requires @p CH_USE_QUEUES_HALFDUPLEX.
 */
#define CH_USE_SERIAL_HALFDUPLEX

/**
 * If specified then the memory heap allocator APIs are included in the kernel.
 * @note Requires @p CH_USE_MUTEXES or @p CH_USE_SEMAPHORES.
 * @note Mutexes are recommended.
 */
#define CH_USE_HEAP

/**
 * Number of RAM bytes to use as system heap. If set to zero then the whole
 * available RAM is used as system heap.
 * @note In order to use the whole RAM as system heap the linker script must
 *       provide the @p __heap_base__ and @p __heap_end__ symbols.
 * @note Requires @p CH_USE_HEAP.
 */
#define CH_HEAP_SIZE 0

/**
 * If enabled enforces the use of the C-runtime @p malloc() and @p free()
 * functions as backend for the system heap allocator.
 * @note Requires @p CH_USE_HEAP.
 * @note The recommended setting is OFF.
 */
#define CH_USE_MALLOC_HEAP

/**
 * If specified then the memory pools allocator APIs are included in the
 * kernel.
 */
#define CH_USE_MEMPOOLS

/**
 * If specified then the dynamic threads creation APIs are included in the
 * kernel.
 * @note Requires @p CH_USE_WAITEXIT.
 */
#define CH_USE_DYNAMIC

/**
 * Frequency of the system timer that drives the system ticks. This also
 * defines the system tick time unit.
 */
#define CH_FREQUENCY 1000

/**
 * This constant is the number of system ticks allowed for the threads before
 * preemption occurs. This option is only meaningful if the option
 * @p CH_USE_ROUNDROBIN is also active.
 */
#define CH_TIME_QUANTUM 20

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
 */
#define CH_CURRP_REGISTER_CACHE "reg"

/**
 * Debug option, if enableed includes basic debug support to the kernel.
 * @note The debug support is port-dependent, it may be not present on some
 *       targets. In that case stub functions will be included.
 */
#define CH_USE_DEBUG

/**
 * Debug option, includes the threads context switch tracing feature.
 */
#define CH_USE_TRACE

/**
 * User fields added to the end of the @p Thread structure.
 */
#define THREAD_EXT_FIELDS                                               \
struct {                                                                \
  /* Add thread custom fields here.*/                                   \
};

/**
 * User initialization code added to the @p chThdInit() API.
 * @note It is invoked from within @p chThdInit().
 */
#define THREAD_EXT_INIT(tp) {                                           \
  /* Add thread initialization code here.*/                             \
}

/**
 * User finalization code added to the @p chThdExit() API.
 * @note It is inserted into lock zone.
 */
#define THREAD_EXT_EXIT(tp) {                                           \
  /* Add thread finalization code here.*/                               \
}

#endif  /* _CHCONF_H_ */

/** @} */

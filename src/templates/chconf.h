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
 * @addtogroup Config
 * @{
 */

#ifndef _CHCONF_H_
#define _CHCONF_H_

/*
 * NOTE: this is just documentation for doxigen, the real configuration file
 * is the one into the project directories.
 */

/** Configuration option: if specified then time efficient rather than space
 *  efficient code is used when two possible implementations exist, note
 *  that this is not related to the compiler optimization options.*/
#define CH_OPTIMIZE_SPEED

/** Configuration option: if specified then the Virtual Timers subsystem is
 *  included in the kernel.*/
#define CH_USE_VIRTUAL_TIMERS

/** Configuration option: if specified then the kernel performs the round
 *  robin scheduling algorithm on threads of equal priority.*/
#define CH_USE_ROUNDROBIN

/** Configuration option: if specified then the System Timer subsystem is
 *  included in the kernel.*/
#define CH_USE_SYSTEMTIME

/** Configuration option: if specified then the \p chThdWait() function
 *  is included in the kernel.*/
#define CH_USE_WAITEXIT

/** Configuration option: if specified then the Semaphores APIs are included
 *  in the kernel.*/
#define CH_USE_SEMAPHORES

/** Configuration option: if specified then the Semaphores atomic Signal+Wait
 *  APIs are included in the kernel.*/
#define CH_USE_SEMSW

/** Configuration option: if specified then the Semaphores with timeout APIs
 *  are included in the kernel.
 * @note requires \p CH_USE_SEMAPHORES.
 * @note requires \p CH_USE_VIRTUAL_TIMERS.*/
#define CH_USE_SEMAPHORES_TIMEOUT

/** Configuration option: if specified then the Mutexes APIs are included in
 * the kernel.*/
#define CH_USE_MUTEXES

/** Configuration option: if specified then the Events APIs are included in
 *  the kernel.*/
#define CH_USE_EVENTS

/** Configuration option: if specified then the \p chEvtWaitTimeout()
 *  function is included in the kernel.
 * @note requires \p CH_USE_EVENTS.
 * @note requires \p CH_USE_VIRTUAL_TIMERS.*/
#define CH_USE_EVENTS_TIMEOUT

/** Configuration option: if specified then the Synchronous Messages APIs are
 *  included in the kernel.*/
#define CH_USE_MESSAGES

/** Configuration option: if specified then the \p chMsgSendWithEvent()
 *  function is included in the kernel.
 * @note requires \p CH_USE_MESSAGES.
 * @note requires \p CH_USE_VIRTUAL_TIMERS.*/
#define CH_USE_MESSAGES_EVENT

/** Configuration option: If enabled then the threads have an option to serve
 *  messages by priority instead of FIFO order.
 * @note requires \p CH_USE_MESSAGES.*/
#define CH_USE_MESSAGES_PRIORITY

/** Configuration option: if specified then the
 *  \p chThdGetExitEventSource() function is included in the kernel.
 * @note requires \p CH_USE_EVENTS.
 * @deprecated \p THREAD_EXT_EXIT should be used, this functionality will be
 *             removed in version 1.0.0.
 */
#define CH_USE_EXIT_EVENT

/** Configuration option: if specified then the I/O queues APIs are included
 *  in the kernel.*/
#define CH_USE_QUEUES

/** Configuration option: if specified then the halfduplex queue APIs are
 *  included in the kernel.*/
#define CH_USE_QUEUES_HALFDUPLEX

/** Configuration option: if specified then the I/O queues with timeout
 *  APIs are included in the kernel.
 * @note requires \p CH_USE_SEMAPHORES_TIMEOUT.*/
#define CH_USE_QUEUES_TIMEOUT

/** Configuration option: if specified then the full duplex serial driver APIs
 *  are included in the kernel.*/
#define CH_USE_SERIAL_FULLDUPLEX

/** Configuration option: if specified then the half duplex serial driver APIs
 *  are included in the kernel.*/
#define CH_USE_SERIAL_HALFDUPLEX

/** Configuration option: if specified then the memory heap allocator APIs
 *  are included in the kernel.*/
#define CH_USE_HEAP

/** Configuration option: Number of RAM bytes to use as system heap. If set to
 *  zero then the whole available RAM is used as system heap.
 * @note In order to use the whole RAM as system heap the linker script must
 *       provide the \p __heap_base__ and \p __heap_end__ symbols.
 * @note requires \p CH_USE_HEAP.
 */
#define CH_HEAP_SIZE 0

/** Configuration option: enforces the use of the C-runtime \p malloc() and
 *  \p free() functions as backend for the system heap allocator.*/
#define CH_USE_MALLOC_HEAP

/** Configuration option: if specified then the memory pools allocator APIs
 *  are included in the kernel.*/
#define CH_USE_MEMPOOLS

/** Configuration option: if specified then the dynamic objects creation APIs
 *  are included in the kernel.
 * @note requires \p CH_USE_WAITEXIT.
 */
#define CH_USE_DYNAMIC

/** Configuration option: Frequency of the system timer that drives the system
 *  ticks. This also defines the system time unit.*/
#define CH_FREQUENCY 1000

/** Configuration option: This constant is the number of ticks allowed for the
 *  threads before preemption occurs. This option is only meaningful if the
 *  option \p CH_USE_ROUNDROBIN is also active.*/
#define CH_TIME_QUANTUM 20

/** Configuration option: Defines a CPU register to be used as storage for the
 *  global \p currp variable. Caching this variable in a register can greatly
 *  improve both space and time efficiency of the generated code. Another side
 *  effect is that one less register has to be saved during the context switch
 *  resulting in lower RAM usage and faster code.
 *  @note This option is only useable with the GCC compiler and is only useful
 *        on processors with many registers like ARM cores.
 *  @note If this option is enabled then ALL the libraries linked to the
 *        ChibiOS/RT code <b>must</b> be recompiled with the GCC option \p
 *        -ffixed-\<reg\>.
 */
//#define CH_CURRP_REGISTER_CACHE "reg"

/** Debug option: Includes basic debug support to the kernel.
 *  @note the debug support is port-dependent, it may be not present on some
 *       targets. In that case stub functions will be included.
 */
#define CH_USE_DEBUG

/** Debug option: Includes the threads context switch tracing feature.
 */
#define CH_USE_TRACE

/** User fields added to the end of the \p Thread structure. */
#define THREAD_EXT_FIELDS                                               \
struct {                                                                \
  /* Add thread custom fields here.*/                                   \
};

/** User initialization code added to the \p chThdCreate() API.
 *  @note It is invoked from within \p chThdInit(). */
#define THREAD_EXT_INIT(tp) {                                           \
  /* Add thread initialization code here.*/                             \
}

/** User finalization code added to the \p chThdExit() API.
 *  @note It is inserted into lock zone. */
#define THREAD_EXT_EXIT(tp) {                                           \
  /* Add thread finalization code here.*/                               \
}

#endif  /* _CHCONF_H_ */

/** @} */

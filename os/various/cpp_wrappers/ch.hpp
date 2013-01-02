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
*/

/**
 * @file    ch.hpp
 * @brief   C++ wrapper classes and definitions.
 *
 * @addtogroup cpp_library
 * @{
 */

#include <ch.h>

#ifndef _CH_HPP_
#define _CH_HPP_

namespace chibios_rt {

  /*------------------------------------------------------------------------*
   * chibios_rt::System                                                     *
   *------------------------------------------------------------------------*/
  /**
   * @brief Class encapsulating the base system functionalities.
   */
  class System {
  public:
    /**
     * @brief   ChibiOS/RT initialization.
     * @details After executing this function the current instructions stream
     *          becomes the main thread.
     * @pre     Interrupts must be still disabled when @p chSysInit() is invoked
     *          and are internally enabled.
     * @post    The main thread is created with priority @p NORMALPRIO.
     * @note    This function has special, architecture-dependent, requirements,
     *          see the notes into the various port reference manuals.
     *
     * @special
     */
    static void init(void);

    /**
     * @brief   Enters the kernel lock mode.
     *
     * @special
     */
    static void lock(void);

    /**
     * @brief   Leaves the kernel lock mode.
     *
     * @special
     */
    static void unlock(void);

    /**
     * @brief   Returns the system time as system ticks.
     * @note    The system tick time interval is implementation dependent.
     *
     * @return          The system time.
     *
     * @api
     */
    static systime_t getTime(void);

    /**
     * @brief   Checks if the current system time is within the specified time
     *          window.
     * @note    When start==end then the function returns always true because the
     *          whole time range is specified.
     *
     * @param[in] start     the start of the time window (inclusive)
     * @param[in] end       the end of the time window (non inclusive)
     * @retval true         current time within the specified time window.
     * @retval false        current time not within the specified time window.
     *
     * @api
     */
    static bool isTimeWithin(systime_t start, systime_t end);
  };

  /*------------------------------------------------------------------------*
   * chibios_rt::Timer                                                      *
   *------------------------------------------------------------------------*/
  /**
   * @brief   Timer class.
   */
  class Timer {
  public:
    /**
     * @brief   Embedded @p VirtualTimer structure.
     */
    struct ::VirtualTimer timer_ref;

    /**
     * @brief   Enables a virtual timer.
     * @note    The associated function is invoked from interrupt context.
     *
     * @param[in] time      the number of ticks before the operation timeouts, the
     *                      special values are handled as follow:
     *                      - @a TIME_INFINITE is allowed but interpreted as a
     *                        normal time specification.
     *                      - @a TIME_IMMEDIATE this value is not allowed.
     *                      .
     * @param[in] vtfunc    the timer callback function. After invoking the
     *                      callback the timer is disabled and the structure can
     *                      be disposed or reused.
     * @param[in] par       a parameter that will be passed to the callback
     *                      function
     *
     * @iclass
     */
    void setI(systime_t time, vtfunc_t vtfunc, void *par);

    /**
     * @brief   Resets the timer, if armed.
     *
     * @iclass
     */
    void resetI();

    /**
     * @brief   Returns the timer status.
     *
     * @retval TRUE         The timer is armed.
     * @retval FALSE        The timer already fired its callback.
     *
     * @iclass
     */
    bool isArmedI(void);
  };

  /*------------------------------------------------------------------------*
   * chibios_rt::ThreadReference                                            *
   *------------------------------------------------------------------------*/
  /**
   * @brief     Thread reference class.
   * @details   This class encapsulates a reference to a system thread. All
   *            operations involving another thread are performed through
   *            an object of this type.
   */
  class ThreadReference {
  public:
    /**
     * @brief   Pointer to the system thread.
     */
    ::Thread *thread_ref;

    /**
     * @brief   Thread reference constructor.
     *
     * @param[in] tp            the target thread. This parameter can be
     *                          @p NULL if the thread is not known at
     *                          creation time.
     *
     * @api
     */
    ThreadReference(Thread * tp) : thread_ref(tp) {

    };

    /**
     * @brief   Suspends the current thread on the reference.
     * @details The suspended thread becomes the referenced thread. It is
     *          possible to use this method only if the thread reference
     *          was set to @p NULL.
     *
     * @return                  The incoming message.
     *
     * @api
     */
    msg_t suspend(void);

    /**
     * @brief   Suspends the current thread on the reference.
     * @details The suspended thread becomes the referenced thread. It is
     *          possible to use this method only if the thread reference
     *          was set to @p NULL.
     *
     * @return                  The incoming message.
     *
     * @sclass
     */
    msg_t suspendS(void);

    /**
     * @brief   Resumes the currently referenced thread, if any.
     *
     * @api
     */
    void resume(msg_t msg);

    /**
     * @brief   Resumes the currently referenced thread, if any.
     *
     * @iclass
     */
    void resumeI(msg_t msg);

    /**
     * @brief   Requests a thread termination.
     * @pre     The target thread must be written to invoke periodically
     *          @p chThdShouldTerminate() and terminate cleanly if it returns
     *          @p TRUE.
     * @post    The specified thread will terminate after detecting the termination
     *          condition.
     *
     * @api
     */
    void requestTerminate(void);

#if CH_USE_WAITEXIT || defined(__DOXYGEN__)
    /**
     * @brief   Blocks the execution of the invoking thread until the specified
     *          thread terminates then the exit code is returned.
     * @details This function waits for the specified thread to terminate then
     *          decrements its reference counter, if the counter reaches zero then
     *          the thread working area is returned to the proper allocator.<br>
     *          The memory used by the exited thread is handled in different ways
     *          depending on the API that spawned the thread:
     *          - If the thread was spawned by @p chThdCreateStatic() or by
     *            @p chThdCreateI() then nothing happens and the thread working
     *            area is not released or modified in any way. This is the
     *            default, totally static, behavior.
     *          - If the thread was spawned by @p chThdCreateFromHeap() then
     *            the working area is returned to the system heap.
     *          - If the thread was spawned by @p chThdCreateFromMemoryPool()
     *            then the working area is returned to the owning memory pool.
     *          .
     * @pre     The configuration option @p CH_USE_WAITEXIT must be enabled in
     *          order to use this function.
     * @post    Enabling @p chThdWait() requires 2-4 (depending on the
     *          architecture) extra bytes in the @p Thread structure.
     * @post    After invoking @p chThdWait() the thread pointer becomes invalid
     *          and must not be used as parameter for further system calls.
     * @note    If @p CH_USE_DYNAMIC is not specified this function just waits for
     *          the thread termination, no memory allocators are involved.
     *
     * @param[in] tp        pointer to the thread
     * @return              The exit code from the terminated thread.
     *
     * @api
     */
    msg_t wait(void);
#endif /* CH_USE_WAITEXIT */

#if CH_USE_MESSAGES || defined(__DOXYGEN__)
    /**
     * @brief   Sends a message to the thread and returns the answer.
     *
     * @param[in] msg           the sent message
     * @return                  The returned message.
     *
     * @api
     */
    msg_t sendMessage(msg_t msg);

    /**
     * @brief   Returns true if there is at least one message in queue.
     *
     * @retval true             A message is waiting in queue.
     * @retval false            A message is not waiting in queue.
     *
     * @api
     */
    bool isPendingMessage(void);
#endif /* CH_USE_MESSAGES */

#if CH_USE_EVENTS || defined(__DOXYGEN__)
    /**
     * @brief   Adds a set of event flags directly to specified @p Thread.
     *
     * @param[in] mask      the event flags set to be ORed
     *
     * @api
     */
    void signalEvents(eventmask_t mask);

    /**
     * @brief   Adds a set of event flags directly to specified @p Thread.
     *
     * @param[in] mask      the event flags set to be ORed
     *
     * @iclass
     */
    void signalEventsI(eventmask_t mask);
#endif /* CH_USE_EVENTS */

#if CH_USE_DYNAMIC || defined(__DOXYGEN__)
#endif /* CH_USE_DYNAMIC */
  };

  /*------------------------------------------------------------------------*
   * chibios_rt::BaseThread                                             *
   *------------------------------------------------------------------------*/
  /**
   * @brief   Abstract base class for a ChibiOS/RT thread.
   * @details The thread body is the virtual function @p Main().
   */
  class BaseThread : public ThreadReference {
  public:

    /**
     * @brief   BaseThread constructor.
     *
     * @api
     */
    BaseThread(void);

    /**
     * @brief   Thread body function.
     *
     * @return                  The exit message.
     *
     * @api
     */
    virtual msg_t Main(void) {
      return 0;
    };

    /**
     * @brief   Creates and starts a system thread.
     *
     * @param[in] prio          thread priority
     * @return                  A reference to the created thread with
     *                          reference counter set to one.
     *
     * @api
     */
    virtual ThreadReference start(tprio_t prio) {

      (void)prio;

      return *this;
    };

    /**
     * @brief   Sets the current thread name.
     * @pre     This function only stores the pointer to the name if the option
     *          @p CH_USE_REGISTRY is enabled else no action is performed.
     *
     * @param[in] p         thread name as a zero terminated string
     *
     * @api
     */
    static void setName(const char *tname);

    /**
     * @brief   Changes the running thread priority level then reschedules if
     *          necessary.
     * @note    The function returns the real thread priority regardless of the
     *          current priority that could be higher than the real priority
     *          because the priority inheritance mechanism.
     *
     * @param[in] newprio   the new priority level of the running thread
     * @return              The old priority level.
     *
     * @api
     */
    static tprio_t setPriority(tprio_t newprio);

    /**
     * @brief   Terminates the current thread.
     * @details The thread goes in the @p THD_STATE_FINAL state holding the
     *          specified exit status code, other threads can retrieve the
     *          exit status code by invoking the function @p chThdWait().
     * @post    Eventual code after this function will never be executed,
     *          this function never returns. The compiler has no way to
     *          know this so do not assume that the compiler would remove
     *          the dead code.
     *
     * @param[in] msg       thread exit code
     *
     * @api
     */
    static void exit(msg_t msg);

    /**
     * @brief   Terminates the current thread.
     * @details The thread goes in the @p THD_STATE_FINAL state holding the
     *          specified exit status code, other threads can retrieve the
     *          exit status code by invoking the function @p chThdWait().
     * @post    Eventual code after this function will never be executed,
     *          this function never returns. The compiler has no way to
     *          know this so do not assume that the compiler would remove
     *          the dead code.
     *
     * @param[in] msg       thread exit code
     *
     * @sclass
     */
    static void exitS(msg_t msg);

    /**
     * @brief   Verifies if the current thread has a termination request pending.
     * @note    Can be invoked in any context.
     *
     * @retval TRUE         termination request pending.
     * @retval FALSE        termination request not pending.
     *
     * @special
     */
    static bool shouldTerminate(void);

    /**
     * @brief   Suspends the invoking thread for the specified time.
     *
     * @param[in] time      the delay in system ticks, the special values are
     *                      handled as follow:
     *                      - @a TIME_INFINITE the thread enters an infinite sleep
     *                        state.
     *                      - @a TIME_IMMEDIATE this value is not allowed.
     *                      .
     *
     * @api
     */
    static void sleep(systime_t interval);

    /**
     * @brief   Suspends the invoking thread until the system time arrives to the
     *          specified value.
     *
     * @param[in] time      absolute system time
     *
     * @api
     */
    static void sleepUntil(systime_t time);

    /**
     * @brief   Yields the time slot.
     * @details Yields the CPU control to the next thread in the ready list with
     *          equal priority, if any.
     *
     * @api
     */
    static void yield(void);

#if CH_USE_MESSAGES || defined(__DOXYGEN__)
    /**
     * @brief   Waits for a message.
     *
     * @return                  The sender thread.
     *
     * @api
     */
    static ThreadReference waitMessage(void);

    /**
     * @brief   Returns an enqueued message or @p NULL.
     *
     * @param[in] trp           the sender thread reference
     * @return                  The incoming message.
     *
     * @api
     */
    static msg_t getMessage(ThreadReference* trp);

    /**
     * @brief   Releases the next message in queue with a reply.
     *
     * @param[in] trp           the sender thread reference
     * @param[in] msg           the answer message
     *
     * @api
     */
    static void releaseMessage(ThreadReference* trp, msg_t msg);
#endif /* CH_USE_MESSAGES */

#if CH_USE_EVENTS || defined(__DOXYGEN__)
    /**
     * @brief   Clears the pending events specified in the mask.
     *
     * @param[in] mask      the events to be cleared
     * @return              The pending events that were cleared.
     *
     * @api
     */
    static eventmask_t getAndClearEvents(eventmask_t mask);

    /**
     * @brief   Adds (OR) a set of event flags on the current thread, this is
     *          @b much faster than using @p chEvtBroadcast() or @p chEvtSignal().
     *
     * @param[in] mask      the event flags to be added
     * @return              The current pending events mask.
     *
     * @api
     */
    static eventmask_t addEvents(eventmask_t mask);

    /**
     * @brief   Waits for a single event.
     * @details A pending event among those specified in @p ewmask is selected,
     *          cleared and its mask returned.
     * @note    One and only one event is served in the function, the one with
     *          the lowest event id. The function is meant to be invoked into
     *          a loop in order to serve all the pending events.<br>
     *          This means that Event Listeners with a lower event identifier
     *          have an higher priority.
     *
     * @param[in] ewmask        mask of the events that the function should
     *                          wait for, @p ALL_EVENTS enables all the events
     * @return                  The mask of the lowest id served and cleared
     *                          event.
     *
     * @api
     */
    static eventmask_t waitOneEvent(eventmask_t ewmask);

    /**
     * @brief   Waits for any of the specified events.
     * @details The function waits for any event among those specified in
     *          @p ewmask to become pending then the events are cleared and
     *          returned.
     *
     * @param[in] ewmask        mask of the events that the function should
     *                          wait for, @p ALL_EVENTS enables all the events
     * @return                  The mask of the served and cleared events.
     *
     * @api
     */
    static eventmask_t waitAnyEvent(eventmask_t ewmask);

    /**
     * @brief   Waits for all the specified event flags then clears them.
     * @details The function waits for all the events specified in @p ewmask
     *          to become pending then the events are cleared and returned.
     *
     * @param[in] ewmask        mask of the event ids that the function should
     *                          wait for
     * @return                  The mask of the served and cleared events.
     *
     * @api
     */
    static eventmask_t waitAllEvents(eventmask_t ewmask);

#if CH_USE_EVENTS_TIMEOUT || defined(__DOXYGEN__)
    /**
     * @brief   Waits for a single event.
     * @details A pending event among those specified in @p ewmask is selected,
     *          cleared and its mask returned.
     * @note    One and only one event is served in the function, the one with
     *          the lowest event id. The function is meant to be invoked into
     *          a loop in order to serve all the pending events.<br>
     *          This means that Event Listeners with a lower event identifier
     *          have an higher priority.
     *
     * @param[in] ewmask        mask of the events that the function should
     *                          wait for, @p ALL_EVENTS enables all the events
     *
     * @param[in] time          the number of ticks before the operation timouts
     * @return                  The mask of the lowest id served and cleared
     *                          event.
     * @retval 0                if the specified timeout expired.
     *
     * @api
     */
    static eventmask_t waitOneEventTimeout(eventmask_t ewmask,
                                           systime_t time);

    /**
     * @brief   Waits for any of the specified events.
     * @details The function waits for any event among those specified in
     *          @p ewmask to become pending then the events are cleared and
     *          returned.
     *
     * @param[in] ewmask        mask of the events that the function should
     *                          wait for, @p ALL_EVENTS enables all the events
     * @param[in] time          the number of ticks before the operation
     *                          timouts
     * @return                  The mask of the served and cleared events.
     * @retval 0                if the specified timeout expired.
     *
     * @api
     */
    static eventmask_t waitAnyEventTimeout(eventmask_t ewmask,
                                           systime_t time);

    /**
     * @brief   Waits for all the specified event flags then clears them.
     * @details The function waits for all the events specified in @p ewmask
     *          to become pending then the events are cleared and returned.
     *
     * @param[in] ewmask        mask of the event ids that the function should
     *                          wait for
     * @param[in] time          the number of ticks before the operation
     *                          timouts
     * @return                  The mask of the served and cleared events.
     * @retval 0                if the specified timeout expired.
     *
     * @api
     */
    static eventmask_t waitAllEventsTimeout(eventmask_t ewmask,
                                            systime_t time);
#endif /* CH_USE_EVENTS_TIMEOUT */

    /**
     * @brief   Invokes the event handlers associated to an event flags mask.
     *
     * @param[in] mask      mask of the event flags to be dispatched
     * @param[in] handlers  an array of @p evhandler_t. The array must have size
     *                      equal to the number of bits in eventmask_t.
     *
     * @api
     */
    static void dispatchEvents(const evhandler_t handlers[],
                               eventmask_t mask);
#endif /* CH_USE_EVENTS */

#if CH_USE_MUTEXES || defined(__DOXYGEN__)
    /**
     * @brief   Unlocks the next owned mutex in reverse lock order.
     * @pre     The invoking thread <b>must</b> have at least one owned mutex.
     * @post    The mutex is unlocked and removed from the per-thread stack of
     *          owned mutexes.
     *
     * @return              A pointer to the unlocked mutex.
     *
     * @api
     */
    static void unlockMutex(void);

    /**
     * @brief   Unlocks the next owned mutex in reverse lock order.
     * @pre     The invoking thread <b>must</b> have at least one owned mutex.
     * @post    The mutex is unlocked and removed from the per-thread stack of
     *          owned mutexes.
     * @post    This function does not reschedule so a call to a rescheduling
     *          function must be performed before unlocking the kernel.
     *
     * @return              A pointer to the unlocked mutex.
     *
     * @sclass
     */
    static void unlockMutexS(void);

    /**
     * @brief   Unlocks all the mutexes owned by the invoking thread.
     * @post    The stack of owned mutexes is emptied and all the found
     *          mutexes are unlocked.
     * @note    This function is <b>MUCH MORE</b> efficient than releasing the
     *          mutexes one by one and not just because the call overhead,
     *          this function does not have any overhead related to the priority
     *          inheritance mechanism.
     *
     * @api
     */
    static void unlockAllMutexes(void);
#endif /* CH_USE_MUTEXES */
  };

  /*------------------------------------------------------------------------*
   * chibios_rt::BaseStaticThread                                           *
   *------------------------------------------------------------------------*/
  /**
   * @brief   Static threads template class.
   * @details This class introduces static working area allocation.
   *
   * @param N               the working area size for the thread class
   */
  template <int N>
  class BaseStaticThread : public BaseThread {
  protected:
    WORKING_AREA(wa, N);                        // Thread working area.

  public:
    /**
     * @brief   Thread constructor.
     * @details The thread object is initialized but the thread is not
     *          started here.
     *
     * @api
     */
    BaseStaticThread(void) : BaseThread() {

    }

    /**
     * @brief   Creates and starts a system thread.
     *
     * @param[in] prio          thread priority
     * @return                  A reference to the created thread with
     *                          reference counter set to one.
     *
     * @api
     */
    ThreadReference start(tprio_t prio) {
      msg_t _thd_start(void *arg);

      thread_ref = chThdCreateStatic(wa, sizeof(wa), prio, _thd_start, this);
      return *this;
    }
  };

#if CH_USE_SEMAPHORES || defined(__DOXYGEN__)
  /*------------------------------------------------------------------------*
   * chibios_rt::Semaphore                                                  *
   *------------------------------------------------------------------------*/
  /**
   * @brief   Class encapsulating a semaphore.
   */
  class Semaphore {
  public:
    /**
     * @brief   Embedded @p ::Semaphore structure.
     */
    struct ::Semaphore sem;

    /**
     * @brief   Semaphore constructor.
     * @details The embedded @p ::Semaphore structure is initialized.
     *
     * @param[in] n             the semaphore counter value, must be greater
     *                          or equal to zero
     *
     * @api
     */
    Semaphore(cnt_t n);

    /**
     * @brief   Performs a reset operation on the semaphore.
     * @post    After invoking this function all the threads waiting on the
     *          semaphore, if any, are released and the semaphore counter is set
     *          to the specified, non negative, value.
     * @note    The released threads can recognize they were waked up by a reset
     *          rather than a signal because the @p chSemWait() will return
     *          @p RDY_RESET instead of @p RDY_OK.
     *
     * @param[in] n         the new value of the semaphore counter. The value must
     *                      be non-negative.
     *
     * @api
     */
    void reset(cnt_t n);

    /**
     * @brief   Performs a reset operation on the semaphore.
     * @post    After invoking this function all the threads waiting on the
     *          semaphore, if any, are released and the semaphore counter is set
     *          to the specified, non negative, value.
     * @post    This function does not reschedule so a call to a rescheduling
     *          function must be performed before unlocking the kernel. Note that
     *          interrupt handlers always reschedule on exit so an explicit
     *          reschedule must not be performed in ISRs.
     * @note    The released threads can recognize they were waked up by a reset
     *          rather than a signal because the @p chSemWait() will return
     *          @p RDY_RESET instead of @p RDY_OK.
     *
     * @param[in] n         the new value of the semaphore counter. The value must
     *                      be non-negative.
     *
     * @iclass
     */
    void resetI(cnt_t n);

    /**
     * @brief   Performs a wait operation on a semaphore.
     *
     * @return              A message specifying how the invoking thread has been
     *                      released from the semaphore.
     * @retval RDY_OK       if the thread has not stopped on the semaphore or the
     *                      semaphore has been signaled.
     * @retval RDY_RESET    if the semaphore has been reset using @p chSemReset().
     *
     * @api
     */
    msg_t wait(void);

    /**
     * @brief   Performs a wait operation on a semaphore.
     *
     * @return              A message specifying how the invoking thread has been
     *                      released from the semaphore.
     * @retval RDY_OK       if the thread has not stopped on the semaphore or the
     *                      semaphore has been signaled.
     * @retval RDY_RESET    if the semaphore has been reset using @p chSemReset().
     *
     * @sclass
     */
    msg_t waitS(void);

    /**
     * @brief   Performs a wait operation on a semaphore with timeout specification.
     *
     * @param[in] time      the number of ticks before the operation timeouts,
     *                      the following special values are allowed:
     *                      - @a TIME_IMMEDIATE immediate timeout.
     *                      - @a TIME_INFINITE no timeout.
     *                      .
     * @return              A message specifying how the invoking thread has been
     *                      released from the semaphore.
     * @retval RDY_OK       if the thread has not stopped on the semaphore or the
     *                      semaphore has been signaled.
     * @retval RDY_RESET    if the semaphore has been reset using @p chSemReset().
     * @retval RDY_TIMEOUT  if the semaphore has not been signaled or reset within
     *                      the specified timeout.
     *
     * @api
     */
    msg_t waitTimeout(systime_t time);

    /**
     * @brief   Performs a wait operation on a semaphore with timeout specification.
     *
     * @param[in] time      the number of ticks before the operation timeouts,
     *                      the following special values are allowed:
     *                      - @a TIME_IMMEDIATE immediate timeout.
     *                      - @a TIME_INFINITE no timeout.
     *                      .
     * @return              A message specifying how the invoking thread has been
     *                      released from the semaphore.
     * @retval RDY_OK       if the thread has not stopped on the semaphore or the
     *                      semaphore has been signaled.
     * @retval RDY_RESET    if the semaphore has been reset using @p chSemReset().
     * @retval RDY_TIMEOUT  if the semaphore has not been signaled or reset within
     *                      the specified timeout.
     *
     * @sclass
     */
    msg_t waitTimeoutS(systime_t time);

    /**
     * @brief   Performs a signal operation on a semaphore.
     *
     * @api
     */
    void signal(void);

    /**
     * @brief   Performs a signal operation on a semaphore.
     * @post    This function does not reschedule so a call to a rescheduling
     *          function must be performed before unlocking the kernel. Note that
     *          interrupt handlers always reschedule on exit so an explicit
     *          reschedule must not be performed in ISRs.
     *
     * @iclass
     */
    void signalI(void);

    /**
     * @brief   Adds the specified value to the semaphore counter.
     * @post    This function does not reschedule so a call to a rescheduling
     *          function must be performed before unlocking the kernel. Note that
     *          interrupt handlers always reschedule on exit so an explicit
     *          reschedule must not be performed in ISRs.
     *
     * @param[in] n         value to be added to the semaphore counter. The value
     *                      must be positive.
     *
     * @iclass
     */
    void addCounterI(cnt_t n);

    /**
     * @brief   Returns the semaphore counter value.
     *
     * @iclass
     */
    cnt_t getCounterI(void);

#if CH_USE_SEMSW || defined(__DOXYGEN__)
    /**
     * @brief   Atomic signal and wait operations.
     *
     * @param[in] ssem          @p Semaphore object to be signaled
     * @param[in] wsem          @p Semaphore object to wait on
     * @return                  A message specifying how the invoking thread has been
     *                          released from the semaphore.
     * @retval RDY_OK           if the thread has not stopped on the semaphore or the
     *                          semaphore has been signaled.
      * @retval RDY_RESET       if the semaphore has been reset using @p chSemReset().
     *
     * @api
     */
    static msg_t signalWait(chibios_rt::Semaphore *ssem,
                            chibios_rt::Semaphore *wsem);
#endif /* CH_USE_SEMSW */
  };
#endif /* CH_USE_SEMAPHORES */

#if CH_USE_MUTEXES || defined(__DOXYGEN__)
  /*------------------------------------------------------------------------*
   * chibios_rt::Mutex                                                      *
   *------------------------------------------------------------------------*/
  /**
   * @brief   Class encapsulating a mutex.
   */
  class Mutex {
  public:
    /**
     * @brief   Embedded @p ::Mutex structure.
     */
    struct ::Mutex mutex;

    /**
     * @brief   Mutex object constructor.
     * @details The embedded @p ::Mutex structure is initialized.
     *
     * @api
     */
    Mutex(void);

    /**
     * @brief   Tries to lock a mutex.
     * @details This function attempts to lock a mutex, if the mutex is already
     *          locked by another thread then the function exits without waiting.
     * @post    The mutex is locked and inserted in the per-thread stack of owned
     *          mutexes.
     * @note    This function does not have any overhead related to the
     *          priority inheritance mechanism because it does not try to
     *          enter a sleep state.
     *
     * @return              The operation status.
     * @retval TRUE         if the mutex has been successfully acquired
     * @retval FALSE        if the lock attempt failed.
     *
     * @api
     */
    bool tryLock(void);

    /**
     * @brief   Tries to lock a mutex.
     * @details This function attempts to lock a mutex, if the mutex is already
     *          taken by another thread then the function exits without waiting.
     * @post    The mutex is locked and inserted in the per-thread stack of owned
     *          mutexes.
     * @note    This function does not have any overhead related to the
     *          priority inheritance mechanism because it does not try to
     *          enter a sleep state.
     *
     * @return              The operation status.
     * @retval TRUE         if the mutex has been successfully acquired
     * @retval FALSE        if the lock attempt failed.
     *
     * @sclass
     */
    bool tryLockS(void);

    /**
     * @brief   Locks the specified mutex.
     * @post    The mutex is locked and inserted in the per-thread stack of owned
     *          mutexes.
     *
     * @api
     */
    void lock(void);

    /**
     * @brief   Locks the specified mutex.
     * @post    The mutex is locked and inserted in the per-thread stack of owned
     *          mutexes.
     *
     * @sclass
     */
    void lockS(void);
  };

#if CH_USE_CONDVARS || defined(__DOXYGEN__)
  /*------------------------------------------------------------------------*
   * chibios_rt::CondVar                                                    *
   *------------------------------------------------------------------------*/
  /**
   * @brief   Class encapsulating a conditional variable.
   */
  class CondVar {
  public:
    /**
     * @brief   Embedded @p ::CondVar structure.
     */
    struct ::CondVar condvar;

    /**
     * @brief   CondVar object constructor.
     * @details The embedded @p ::CondVar structure is initialized.
     *
     * @api
     */
    CondVar(void);

    /**
     * @brief   Signals one thread that is waiting on the condition variable.
     *
     * @api
     */
    void signal(void);

    /**
     * @brief   Signals one thread that is waiting on the condition variable.
     * @post    This function does not reschedule so a call to a rescheduling
     *          function must be performed before unlocking the kernel. Note that
     *          interrupt handlers always reschedule on exit so an explicit
     *          reschedule must not be performed in ISRs.
     *
     * @iclass
     */
    void signalI(void);

    /**
     * @brief   Signals all threads that are waiting on the condition variable.
     *
     * @api
     */
    void broadcast(void);

    /**
     * @brief   Signals all threads that are waiting on the condition variable.
     * @post    This function does not reschedule so a call to a rescheduling
     *          function must be performed before unlocking the kernel. Note that
     *          interrupt handlers always reschedule on exit so an explicit
     *          reschedule must not be performed in ISRs.
     *
     * @iclass
     */
    void broadcastI(void);

    /**
     * @brief   Waits on the condition variable releasing the mutex lock.
     * @details Releases the currently owned mutex, waits on the condition
     *          variable, and finally acquires the mutex again. All the sequence
     *          is performed atomically.
     * @pre     The invoking thread <b>must</b> have at least one owned mutex.
     *
     * @return              A message specifying how the invoking thread has been
     *                      released from the condition variable.
     * @retval RDY_OK       if the condvar has been signaled using
     *                      @p chCondSignal().
     * @retval RDY_RESET    if the condvar has been signaled using
     *                      @p chCondBroadcast().
     *
     * @api
     */
    msg_t wait(void);

    /**
     * @brief   Waits on the condition variable releasing the mutex lock.
     * @details Releases the currently owned mutex, waits on the condition
     *          variable, and finally acquires the mutex again. All the sequence
     *          is performed atomically.
     * @pre     The invoking thread <b>must</b> have at least one owned mutex.
     *
     * @return              A message specifying how the invoking thread has been
     *                      released from the condition variable.
     * @retval RDY_OK       if the condvar has been signaled using
     *                      @p chCondSignal().
     * @retval RDY_RESET    if the condvar has been signaled using
     *                      @p chCondBroadcast().
     *
     * @sclass
     */
    msg_t waitS(void);

#if CH_USE_CONDVARS_TIMEOUT || defined(__DOXYGEN__)
    /**
     * @brief   Waits on the CondVar while releasing the controlling mutex.
     *
     * @param[in] time          the number of ticks before the operation fails
     * @return                  The wakep mode.
     * @retval RDY_OK if        the condvar was signaled using
     *                          @p chCondSignal().
     * @retval RDY_RESET        if the condvar was signaled using
     *                          @p chCondBroadcast().
     * @retval RDY_TIMEOUT      if the condvar was not signaled within the
     *                          specified timeout.
     *
     * @api
     */
    msg_t waitTimeout(systime_t time);
#endif /* CH_USE_CONDVARS_TIMEOUT */
  };
#endif /* CH_USE_CONDVARS */
#endif /* CH_USE_MUTEXES */

#if CH_USE_EVENTS || defined(__DOXYGEN__)
  /*------------------------------------------------------------------------*
   * chibios_rt::EvtListener                                              *
   *------------------------------------------------------------------------*/
  /**
   * @brief   Class encapsulating an event listener.
   */
  class EvtListener {
  public:
    /**
     * @brief   Embedded @p ::EvtListener structure.
     */
    struct ::EventListener ev_listener;

    /**
     * @brief   Returns the pending flags from the listener and clears them.
     *
     * @param[in] flags         the events to be cleared
     * @return                  The flags added to the listener by the
     *                          associated event source.
     *
     * @api
     */
    flagsmask_t getAndClearFlags(void);
  };

  /*------------------------------------------------------------------------*
   * chibios_rt::EvtSource                                                *
   *------------------------------------------------------------------------*/
  /**
   * @brief   Class encapsulating an event source.
   */
  class EvtSource {
  public:
    /**
     * @brief   Embedded @p ::EvtSource structure.
     */
    struct ::EventSource ev_source;

    /**
     * @brief   EvtSource object constructor.
     * @details The embedded @p ::EvtSource structure is initialized.
     *
     * @api
     */
    EvtSource(void);

    /**
     * @brief   Registers a listener on the event source.
     *
     * @param[in] elp           pointer to the @p EvtListener object
     * @param[in] eid           numeric identifier assigned to the Event
     *                          Listener
     *
     * @api
     */
    void registerOne(chibios_rt::EvtListener *elp, eventid_t eid);

    /**
     * @brief   Registers an Event Listener on an Event Source.
     * @note    Multiple Event Listeners can specify the same bits to be added.
     *
     * @param[in] elp           pointer to the @p EvtListener object
     * @param[in] emask         the mask of event flags to be pended to the
     *                          thread when the event source is broadcasted
     *
     * @api
     */
    void registerMask(chibios_rt::EvtListener *elp, eventmask_t emask);

    /**
     * @brief   Unregisters a listener.
     * @details The specified listeners is no more signaled by the event
     *          source.
     *
     * @param[in] elp           the listener to be unregistered
     *
     * @api
     */
    void unregister(chibios_rt::EvtListener *elp);

    /**
     * @brief   Broadcasts on an event source.
     * @details All the listeners registered on the event source are signaled
     *          and the flags are added to the listener's flags mask.
     *
     * @param[in] flags         the flags set to be added to the listener
     *                          flags mask
     *
     * @api
     */
    void broadcastFlags(flagsmask_t flags);

    /**
     * @brief   Broadcasts on an event source.
     * @details All the listeners registered on the event source are signaled
     *          and the flags are added to the listener's flags mask.
     *
     * @param[in] flags         the flags set to be added to the listener
     *                          flags mask
     *
     * @iclass
     */
    void broadcastFlagsI(flagsmask_t flags);
  };
#endif /* CH_USE_EVENTS */
}

#endif /* _CH_HPP_ */

/** @} */

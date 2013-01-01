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
     * @details The system is initialized, the idle thread is spawned and the
     *          current instruction flow becomes the main thread with priority
     *          @p NORMALPRIO.
     *
     * @api
     */
    static void init(void);

    /**
     * @brief   Kernel lock.
     * @note    On some ports it is faster to invoke chSysLock() directly
     *          because inlining.
     *
     * @special
     */
    static void lock(void);

    /**
     * @brief   Kernel unlock.
     * @note    On some ports it is faster to invoke chSysUnlock() directly
     *          because inlining.
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
     * @brief   Starts the timer.
     * @note    It must be called with the interrupts disabled.
     * @note    The associated function is invoked by an interrupt handler.
     *
     * @param[in] time      the time in system ticks
     * @param[in] vtfunc    the timer callback function
     * @param[in] par       the parameter for the callback function
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
   * @details   This class encapsulates a reference to a system thread.
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
    ThreadReference(Thread * tp);

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
     * @brief   Requests thread termination.
     * @details A termination flag is added to the thread, it is thread
     *          responsibility to detect it and exit.
     */
    void requestTerminate(void);

#if CH_USE_WAITEXIT || defined(__DOXYGEN__)
    /**
     * @brief   Synchronization on Thread exit.
     *
     * @return                  The exit message from the thread.
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

#if CH_USE_DYNAMIC
#endif /* CH_USE_DYNAMIC */
  };

  /*------------------------------------------------------------------------*
   * chibios_rt::BaseThread                                             *
   *------------------------------------------------------------------------*/
  /**
   * @brief   Abstract base class for a ChibiOS/RT thread.
   * @details The thread body is the virtual function @p Main().
   */
  class BaseThread : ThreadReference{
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
    virtual msg_t Main(void);

    /**
     * @brief   Creates and starts a system thread.
     *
     * @param[in] tname         the name to be assigned to the thread
     * @param[in] prio          thread priority
     * @return                  Error flag.
     * @retval false            if the operation failed.
     * @retval true             if the operation succeeded.
     *
     * @api
     */
    virtual bool start(const char *tname, tprio_t prio);

    /**
     * @brief   Thread exit.
     *
     * @param[in] msg           the exit message
     *
     * @api
     */
    static void exit(msg_t msg);

#if CH_USE_WAITEXIT || defined(__DOXYGEN__)
    /**
     * @brief   Synchronization on Thread exit.
     *
     * @return                  The exit message from the thread.
     *
     * @api
     */
    msg_t wait(void);
#endif /* CH_USE_WAITEXIT */

    /**
     * @brief   Changes the current thread priority.
     *
     * @param[in] newprio       The new priority level
     * @return                  The old priority level.
     *
     * @api
     */
    static tprio_t setPriority(tprio_t newprio);

    /**
     * @brief   Requests thread termination.
     * @details A termination flag is added to the thread, it is thread
     *          responsibility to detect it and exit.
     *
     * @api
     */
    void requestTerminate(void);

    /**
     * @brief   Determines if there is a pending termination request.
     *
     * @return                  The termination status.
     * @retval false            if there is no termination request pending.
     * @retval true             if there is a termination request pending.
     *
     * @api
     */
    bool shouldTerminate(void);

    /**
     * @brief   Suspends the thread execution for the specified number of
     *          system ticks.
     *
     * @param[in] interval      the number of system ticks
     *
     * @api
     */
    static void sleep(systime_t interval);

    /**
     * @brief   Suspends the thread execution until the specified time arrives.
     *
     * @param[in]               time the system time
     *
     * @api
     */
    static void sleepUntil(systime_t time);

#if CH_USE_MESSAGES
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
     * @param[in] tname         the name to be assigned to the thread
     * @param[in] prio          thread priority
     * @return                  Error flag.
     * @retval false            if the operation succeeded.
     * @retval true             if the operation failed.
     *
     * @api
     */
    bool start(const char *tname, tprio_t prio) {
      msg_t _thd_start(void *arg);

      thread_ref = chThdCreateStatic(wa, sizeof(wa), prio, _thd_start, this);
      return false;
    }
  };

#if CH_USE_SEMAPHORES
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
     * @brief   Resets a semaphore.
     *
     * @param[in] n             the new semaphore counter value, must be
     *                          greater or equal to zero
     *
     * @api
     */
    void reset(cnt_t n);

    /**
     * @brief   Wait operation on the semaphore.
     *
     * @retval RDY_OK           if the semaphore was signaled or not taken.
     * @retval RDY_RESET        if the semaphore was reset.
     *
     * @api
     */
    msg_t wait(void);

    /**
     * @brief   Wait operation on the semaphore with timeout.
     *
     * @param[in] time          the number of ticks before the operation fails
     * @retval RDY_OK           if the semaphore was signaled or not taken.
     * @retval RDY_RESET        if the semaphore was reset.
     * @retval RDY_TIMEOUT      if the semaphore was not signaled or reset
     *                          within the specified timeout.
     *
     * @api
     */
    msg_t waitTimeout(systime_t time);

    /**
     * @brief   Signal operation on the semaphore.
     * @details The semaphore is signaled, the next thread in queue, if any,
     *          is awakened.
     *
     * @api
     */
    void signal(void);

#if CH_USE_SEMSW
    /**
     * @brief   Atomic signal and wait operations.
     *
     * @param[in] ssem          pointer to a @p Semaphore to be signaled
     * @param[in] wsem          pointer to a @p Semaphore to be wait on
     * @retval RDY_OK           if the semaphore was signaled or not taken.
     * @retval RDY_RESET        if the semaphore was reset.
     *
     * @api
     */
    static msg_t signalWait(Semaphore *ssem, Semaphore *wsem);
#endif /* CH_USE_SEMSW */
  };
#endif /* CH_USE_SEMAPHORES */

#if CH_USE_MUTEXES
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
     * @brief   Mutex constructor.
     * @details The embedded @p ::Mutex structure is initialized.
     *
     * @api
     */
    Mutex(void);

    /**
     * @brief   Tries a lock operation on the mutex.
     *
     * @retval TRUE             if the mutex was successfully acquired
     * @retval FALSE            if the lock attempt failed.
     *
     * @api
     */
    bool tryLock(void);

    /**
     * @brief   Locks the mutex.
     * @details Performs a lock operation on the mutex, if the mutex is
     *          already locked then the thread enters the mutex priority
     *          queue and waits.
     *
     * @api
     */
    void lock(void);

    /**
     * @brief   Unlocks the mutex.
     * @details Performs an unlock operation on the mutex, the next waiting
     *          thread, if any, is resumed and locks the mutex.
     *
     * @api
     */
    static void unlock(void);

    /**
     * @brief   Unlocks all the mutexes owned by the invoking thread.
     * @details This operation is <b>MUCH MORE</b> efficient than releasing
     *          the mutexes one by one and not just because the call overhead,
     *          this function does not have any overhead related to the
     *          priority inheritance mechanism.
     *
     * @api
     */
    static void unlockAll(void);
  };

#if CH_USE_CONDVARS
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
     * @brief   CondVar constructor.
     * @details The embedded @p ::CondVar structure is initialized.
     *
     * @api
     */
    CondVar(void);

    /**
     * @brief   Signals the CondVar.
     * @details The next thread waiting on the @p CondVar, if any, is awakened.
     *
     * @api
     */
    void signal(void);

    /**
     * @brief   Broadcasts the CondVar.
     * @details All the threads waiting on the @p CondVar, if any, are awakened.
     *
     * @api
     */
    void broadcast(void);

    /**
     * @brief   Waits on the CondVar while releasing the controlling mutex.
     *
     * @return                  The wakep mode.
     * @retval RDY_OK           if the condvar was signaled using
     *                          @p chCondSignal().
     * @retval RDY_RESET        if the condvar was signaled using
     *                          @p chCondBroadcast().
     *
     * @api
     */
    msg_t wait(void);

#if CH_USE_CONDVARS_TIMEOUT
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

#if CH_USE_EVENTS
  /**
   * @brief   Class encapsulating an event source.
   */
  class Event {
  public:
    /**
     * @brief   Embedded @p ::EventSource structure.
     */
    struct ::EventSource event;

    /**
     * @brief   Event constructor.
     * @details The embedded @p ::EventSource structure is initialized.
     *
     * @api
     */
    Event(void);

    /**
     * @brief   Registers a listener on the event source.
     *
     * @param[in] elp           pointer to the @p EventListener structure
     * @param[in] eid           numeric identifier assigned to the Event
     *                          Listener
     *
     * @api
     */
    void registerOne(EventListener *elp, eventid_t eid);

    /**
     * @brief   Registers an Event Listener on an Event Source.
     * @note    Multiple Event Listeners can specify the same bits to be added.
     *
     * @param[in] elp           pointer to the @p EventListener structure
     * @param[in] emask         the mask of event flags to be pended to the
     *                          thread when the event source is broadcasted
     *
     * @api
     */
    void registerMask(EventListener *elp, eventmask_t emask);

    /**
     * @brief   Unregisters a listener.
     * @details The specified listeners is no more signaled by the event
     *          source.
     *
     * @param[in] elp           the listener to be unregistered
     *
     * @api
     */
    void unregister(EventListener *elp);

    /**
     * @brief   Broadcasts an event.
     * @details All the listeners registered on the event source are signaled.
     *
     * @param[in] flags         the flags set to be added to the listener
     *                          flags mask
     *
     * @api
     */
    void broadcastFlags(flagsmask_t flags);

    /**
     * @brief   Clears specified events from the pending events mask.
     *
     * @param[in] elp           pointer to the @p EventListener structure
     * @param[in] flags         the events to be cleared
     * @return                  The flags added to the listener by the
     *                          associated event source.
     *
     * @api
     */
    static flagsmask_t getAndClearFlags(EventListener *elp);

    /**
     * @brief   Clears specified events from the pending events mask.
     *
     * @param[in] mask          the events to be cleared
     * @return                  The pending events that were cleared.
     *
     * @api
     */
    static eventmask_t getAndClearEvents(eventmask_t mask);

    /**
     * @brief   Makes an events mask pending in the current thread.
     * @details This functon is @b much faster than using @p Broadcast().
     *
     * @param[in] mask          the events to be pended
     * @return                  The current pending events mask.
     *
     * @api
     */
    static eventmask_t addEvents(eventmask_t mask);

    /**
     * @brief   Invokes the event handlers associated with a mask.
     *
     * @param[in] mask          mask of the events to be dispatched
     * @param[in] handlers      an array of @p evhandler_t. The array must be
     *                          have indexes from zero up the higher registered
     *                          event identifier.
     *
     * @api
     */
    static void dispatch(const evhandler_t handlers[], eventmask_t mask);

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
    static eventmask_t waitOne(eventmask_t ewmask);

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
    static eventmask_t waitAny(eventmask_t ewmask);

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
    static eventmask_t waitAll(eventmask_t ewmask);

#if CH_USE_EVENTS_TIMEOUT
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
    static eventmask_t waitOneTimeout(eventmask_t ewmask, systime_t time);

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
    static eventmask_t waitAnyTimeout(eventmask_t ewmask, systime_t time);

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
    static eventmask_t waitAllTimeout(eventmask_t ewmask, systime_t time);
#endif /* CH_USE_EVENTS_TIMEOUT */
  };
#endif /* CH_USE_EVENTS */
}

#endif /* _CH_HPP_ */

/** @} */

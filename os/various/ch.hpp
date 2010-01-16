/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

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
 * @file ch.hpp
 * @brief C++ wrapper classes and definitions.
 * @addtogroup cpp_library
 * @{
 */

#include <ch.h>

#ifndef _CH_HPP_
#define _CH_HPP_

namespace chibios_rt {

  /**
   * @brief Class encapsulating the base system functionalities.
   */
  class System {
  public:
    /**
     * @brief ChibiOS/RT initialization.
     * @details The system is initialized, the idle thread is spawned and the
     * current instruction flow becomes the main thread with priority
     * @p NORMALPRIO.
     */
    static void Init(void);

    /**
     * @brief Kernel lock.
     *
     * @note On some ports it is faster to invoke chSysLock() directly because
     *       inlining.
     */
    static void Lock(void);

    /**
     * @brief Kernel unlock.
     *
     * @note On some ports it is faster to invoke chSysUnlock() directly
     *       because inlining.
     */
    static void Unlock(void);

    /**
     * @brief Returns the system time as system ticks.
     *
     * @note the system tick time interval is implementation dependent.
     */
    static systime_t GetTime(void);
  };

  /**
   * @brief Timer class.
   */
  class Timer {
  public:
    /**
     * @brief Embedded @p VirtualTimer structure.
     */
    struct ::VirtualTimer timer;

    /**
     * @brief Starts the timer.
     *
     * @param time the time in system ticks
     * @param vtfunc the timer callback function
     * @param par the parameter for the callback function
     * @note It must be called with the interrupts disabled.
     * @note The associated function is invoked by an interrupt handler.
     */
    void Set(systime_t time, vtfunc_t vtfunc, void *par);

    /**
     * @brief Resets the timer.
     *
     * @note It must be called with the interrupts disabled.
     * @note The timer MUST be active when this function is invoked.
     */
    void Reset();

    /**
     * @brief Returns the timer status.
     *
     * @retval TRUE The timer is armed.
     * @retval FALSE The timer already fired its callback.
     */
    bool IsArmed(void);
  };

  /**
   * @brief Base class for a ChibiOS/RT thread.
   * @details The thread body is the virtual function @p Main().
   */
  class BaseThread {
  public:
    /**
     * @brief Pointer to the system thread.
     */
    ::Thread *thread_ref;

    /**
     * @brief Thread constructor.
     * @details The thread object is initialized and a system thread is
     * started.
     *
     * @param workspace pointer to the workspace area
     * @param wsize size of the workspace area
     * @param prio thread priority
     */
    BaseThread(void *workspace, size_t wsize, tprio_t prio);

    /**
     * @brief Thread exit.
     *
     * @param msg the exit message
     */
    static void Exit(msg_t msg);

#if CH_USE_WAITEXIT
    /**
     * @brief Synchronization on Thread exit.
     *
     * @return the exit message from the thread
     */
    msg_t Wait(void);
#endif /* CH_USE_WAITEXIT */

    /**
     * @brief Resumes the thread.
     * @details The thread encapsulated into the object is resumed.
     */
    void Resume(void);

    /**
     * @brief Changes the thread priority.
     *
     * @param newprio the new priority level
     */
    static void SetPriority(tprio_t newprio);

    /**
     * @brief Requests thread termination.
     * @details A termination flag is pended on the thread, it is thread
     * responsibility to detect it and exit.
     */
    void Terminate(void);

    /**
     * @brief Suspends the thread execution for the specified number of
     * system ticks.
     *
     * @param n the number of system ticks
     */
    static void Sleep(systime_t n);

    /**
     * @brief Suspends the thread execution until the specified time arrives.
     *
     * @param time the system time
     */
    static void SleepUntil(systime_t time);

#if CH_USE_MESSAGES
    /**
     * @brief Sends a message to the thread and returns the answer.
     *
     * @param tp the target thread
     * @param msg the sent message
     * @return The returned message.
     */
    static msg_t SendMessage(::Thread *tp, msg_t msg);

    /**
     * @brief Sends a message to the thread and returns the answer.
     *
     * @param msg the sent message
     * @return The returned message.
     */
    msg_t SendMessage(msg_t msg);

    /**
     * @brief Waits for a message and returns it.
     *
     * @return The incoming message.
     */
    static msg_t WaitMessage(void);

    /**
     * @brief Returns an enqueued message or @p NULL.
     *
     * @return The incoming message.
     * @retval NULL No incoming message.
     */
    static msg_t GetMessage(void);

    /**
     * @brief Releases the next message in queue with a reply.
     *
     * @param msg the answer message
     */
    static void ReleaseMessage(msg_t msg);

    /**
     * @brief Returns true if there is at least one message in queue.
     *
     * @retval TRUE A message is waiting in queue.
     * @retval FALSE A message is not waiting in queue.
     */
    static bool IsPendingMessage(void);
#endif /* CH_USE_MESSAGES */

    /**
     * @brief Thread body function.
     *
     * @return The exit message.
     */
    virtual msg_t Main(void);
  };

  /**
   * @brief Enhanced threads template class.
   * @details This class introduces thread names and static working area
   * allocation.
   *
   * @param N the working area size for the thread class
   */
  template <int N>
  class EnhancedThread : public BaseThread {
  protected:
    WORKING_AREA(wa, N);                        // Thread working area.

  public:
    /**
     * @brief The thread name.
     */
    const char *name;

    /**
     * @brief Full constructor.
     * @details This constructor allows to set a priority level for the new
     * thread.
     * @param tname the name to be assigned to the thread
     * @param prio the priority to be assigned to the thread
     */
    EnhancedThread(const char *tname, tprio_t prio) :
          BaseThread(wa, sizeof wa, prio) {

      name = tname;
    }

    /**
     * @brief Simplified constructor.
     * @details This constructor allows to create a thread by simply
     * specifying a name. In is assumed @p NORMALPRIO as initial priority.
     *
     * @param tname the name to be assigned to the thread
     */
    EnhancedThread(const char *tname) :
          BaseThread(wa, sizeof wa, NORMALPRIO) {

      name = tname;
    }
  };

#if CH_USE_SEMAPHORES
  /**
   * @brief Class encapsulating a semaphore.
   */
  class Semaphore {
  public:
    /**
     * @brief Embedded @p ::Semaphore structure.
     */
    struct ::Semaphore sem;

    /**
     * @brief Semaphore constructor.
     * @details The embedded @p ::Semaphore structure is initialized.
     *
     * @param n the semaphore counter value, must be greater or equal to zero
     */
    Semaphore(cnt_t n);

    /**
     * @brief Resets a semaphore.
     *
     * @param n the new semaphore counter value, must be greater or equal to zero
     */
    void Reset(cnt_t n);

    /**
     * @brief Wait operation on the semaphore.
     *
     * @retval RDY_OK if the semaphore was signaled or not taken.
     * @retval RDY_RESET if the semaphore was reset.
     */
    msg_t Wait(void);

    /**
     * @brief Wait operation on the semaphore with timeout.
     *
     * @param time the number of ticks before the operation fails
     * @retval RDY_OK if the semaphore was signaled or not taken.
     * @retval RDY_RESET if the semaphore was reset.
     * @retval RDY_TIMEOUT if the semaphore was not signaled or reset within the
     *         specified timeout.
     */
    msg_t WaitTimeout(systime_t time);

    /**
     * @brief Signal operation on the semaphore.
     * @details The semaphore is signaled, the next thread in queue, if any,
     * is awakened.
     */
    void Signal(void);

#if CH_USE_SEMSW
    /**
     * @brief Atomic signal and wait operations.
     *
     * @param ssem pointer to a @p Semaphore to be signaled
     * @param wsem pointer to a @p Semaphore to be wait on
     * @retval RDY_OK if the semaphore was signaled or not taken.
     * @retval RDY_RESET if the semaphore was reset.
     */
    static msg_t SignalWait(Semaphore *ssem, Semaphore *wsem);
#endif /* CH_USE_SEMSW */
  };
#endif /* CH_USE_SEMAPHORES */

#if CH_USE_MUTEXES
  /**
   * @brief Class encapsulating a mutex.
   */
  class Mutex {
  public:
    /**
     * @brief Embedded @p ::Mutex structure.
     */
    struct ::Mutex mutex;

    /**
     * @brief Mutex constructor.
     * @details The embedded @p ::Mutex structure is initialized.
     */
    Mutex(void);

    /**
     * @brief Tries a lock operation on the mutex.
     * @retval TRUE if the mutex was successfully acquired
     * @retval FALSE if the lock attempt failed.
     */
    bool TryLock(void);

    /**
     * @brief Locks the mutex.
     * @details Performs a lock operation on the mutex, if the mutex is
     * already locked then the thread enters the mutex priority queue and
     * waits.
     */
    void Lock(void);

    /**
     * @brief Unlocks the mutex.
     * @details Performs an unlock operation on the mutex, the next waiting
     * thread, if any, is resumed and locks the mutex.
     */
    static void Unlock(void);

    /**
     * @brief Unlocks all the mutexes owned by the invoking thread.
     * @details This operation is <b>MUCH MORE</b> efficient than releasing
     * the mutexes one by one and not just because the call overhead, this
     * function does not have any overhead related to the priority inheritance
     * mechanism.
     */
    static void UnlockAll(void);
  };

#if CH_USE_CONDVARS
  /**
   * @brief Class encapsulating a conditional variable.
   */
  class CondVar {
  public:
    /**
     * @brief Embedded @p ::CondVar structure.
     */
    struct ::CondVar condvar;

    /**
     * @brief CondVar constructor.
     * @details The embedded @p ::CondVar structure is initialized.
     */
    CondVar(void);

    /**
     * @brief Signals the CondVar.
     * @details The next thread waiting on the @p CondVar, if any, is awakened.
     */
    void Signal(void);

    /**
     * @brief Broadcasts the CondVar.
     * @details All the threads waiting on the @p CondVar, if any, are awakened.
     */
    void Broadcast(void);

    /**
     * @brief Waits on the CondVar while releasing the controlling mutex.
     *
     * @return The wakep mode.
     * @retval RDY_OK if the condvar was signaled using chCondSignal().
     * @retval RDY_RESET if the condvar was signaled using chCondBroadcast().
     */
    msg_t Wait(void);

#if CH_USE_CONDVARS_TIMEOUT
    /**
     * @brief Waits on the CondVar while releasing the controlling mutex.
     *
     * @param time the number of ticks before the operation fails
     * @return The wakep mode.
     * @retval RDY_OK if the condvar was signaled using chCondSignal().
     * @retval RDY_RESET if the condvar was signaled using chCondBroadcast().
     * @retval RDY_TIMEOUT if the condvar was not signaled within the specified
     *         timeout.
     */
    msg_t WaitTimeout(systime_t time);
#endif /* CH_USE_CONDVARS_TIMEOUT */
  };
#endif /* CH_USE_CONDVARS */
#endif /* CH_USE_MUTEXES */

#if CH_USE_EVENTS
  /**
   * @brief Class encapsulating an event source.
   */
  class Event {
  public:
    /**
     * @brief Embedded @p ::EventSource structure.
     */
    struct ::EventSource event;

    /**
     * @brief Event constructor.
     * @details The embedded @p ::EventSource structure is initialized.
     */
    Event(void);

    /**
     * @brief Registers a listener on the event source.
     *
     * @param elp pointer to the @p EventListener structure
     * @param eid numeric identifier assigned to the Event Listener
     */
    void Register(EventListener *elp, eventid_t eid);

    /**
     * @brief Registers an Event Listener on an Event Source.
     *
     * @param elp pointer to the @p EventListener structure
     * @param emask the mask of event flags to be pended to the thread when the
     *              event source is broadcasted
     * @note Multiple Event Listeners can specify the same bits to be pended.
     */
    void RegisterMask(EventListener *elp, eventmask_t emask);

    /**
     * @brief Unregisters a listener.
     * @details The specified listeners is no more signaled by the event
     * source.
     *
     * @param elp the listener to be unregistered
     */
    void Unregister(EventListener *elp);

    /**
     * @brief Broadcasts an event.
     * @details All the listeners registered on the event source are signaled.
     */
    void Broadcast(void);

    /**
     * @brief Clears specified events from the pending events mask.
     *
     * @param mask the events to be cleared
     * @return The pending events that were cleared.
     */
    static eventmask_t Clear(eventmask_t mask);

    /**
     * @brief Makes an events mask pending in the current thread.
     * @details This functon is @b much faster than using @p Broadcast().
     *
     * @param mask the events to be pended
     * @return The current pending events mask.
     */
    static eventmask_t Pend(eventmask_t mask);

    /**
     * @brief Invokes the event handlers associated with a mask.
     *
     * @param mask mask of the events to be dispatched
     * @param handlers an array of @p evhandler_t. The array must be
     *                 have indexes from zero up the higher registered event
     *                 identifier.
     */
    static void Dispatch(const evhandler_t handlers[], eventmask_t mask);

    /**
     * @brief Waits for a single event.
     * @details A pending event among those specified in @p ewmask is selected,
     * cleared and its mask returned.
     *
     * @param ewmask mask of the events that the function should wait for,
     *               @p ALL_EVENTS enables all the events
     * @return The mask of the lowest id served and cleared event.
     * @note One and only one event is served in the function, the one with the
     *       lowest event id. The function is meant to be invoked into a loop in
     *       order to serve all the pending events.<br>
     *       This means that Event Listeners with a lower event identifier have
     *       an higher priority.
     */
    static eventmask_t WaitOne(eventmask_t ewmask);

    /**
     * @brief Waits for any of the specified events.
     * @details The function waits for any event among those specified in
     * @p ewmask to become pending then the events are cleared and returned.
     *
     * @param ewmask mask of the events that the function should wait for,
     *               @p ALL_EVENTS enables all the events
     * @return The mask of the served and cleared events.
     */
    static eventmask_t WaitAny(eventmask_t ewmask);

    /**
     * @brief Waits for all the specified event flags then clears them.
     * @details The function waits for all the events specified in @p ewmask
     * to become pending then the events are cleared and returned.
     *
     * @param ewmask mask of the event ids that the function should wait for
     * @return The mask of the served and cleared events.
     */
    static eventmask_t WaitAll(eventmask_t ewmask);

#if CH_USE_EVENTS_TIMEOUT
    /**
     * @brief Waits for a single event.
     * @details A pending event among those specified in @p ewmask is selected,
     * cleared and its mask returned.
     * @param ewmask mask of the events that the function should wait for,
     *               @p ALL_EVENTS enables all the events
     * @param time the number of ticks before the operation timouts
     * @return The mask of the lowest id served and cleared event.
     * @retval 0 if the specified timeout expired.
     * @note One and only one event is served in the function, the one with the
     *       lowest event id. The function is meant to be invoked into a loop in
     *       order to serve all the pending events.<br>
     *       This means that Event Listeners with a lower event identifier have
     *       an higher priority.
     */
    static eventmask_t WaitOneTimeout(eventmask_t ewmask, systime_t time);

    /**
     * @brief Waits for any of the specified events.
     * @details The function waits for any event among those specified in
     * @p ewmask to become pending then the events are cleared and returned.
     *
     * @param ewmask mask of the events that the function should wait for,
     *               @p ALL_EVENTS enables all the events
     * @param time the number of ticks before the operation timouts
     * @return The mask of the served and cleared events.
     * @retval 0 if the specified timeout expired.
     */
    static eventmask_t WaitAnyTimeout(eventmask_t ewmask, systime_t time);

    /**
     * @brief Waits for all the specified event flags then clears them.
     * @details The function waits for all the events specified in @p ewmask
     * to become pending then the events are cleared and returned.
     *
     * @param ewmask mask of the event ids that the function should wait for
     * @param time the number of ticks before the operation timouts
     * @return The mask of the served and cleared events.
     * @retval 0 if the specified timeout expired.
     */
    static eventmask_t WaitAllTimeout(eventmask_t ewmask, systime_t time);

#endif /* CH_USE_EVENTS_TIMEOUT */
  };
#endif /* CH_USE_EVENTS */
}

#endif /* _CH_HPP_ */

/** @} */

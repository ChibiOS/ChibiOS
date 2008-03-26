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
 * @addtogroup CPlusPlusLibrary
 * @{
 */

#include <ch.h>

#ifndef _CH_HPP_
#define _CH_HPP_

namespace chibios_rt {

  /**
   * Class encapsulating the base system functionalities.
   */
  class System {
  public:
    /**
     * ChibiOS/RT initialization.
     */
    static void Init(void);

    /**
     * Disables interrupts.
     * @note On some ports it is faster to invoke chSysLock() directly.
     */
    static void Lock(void);
    /**
     * Enables interrupts.
     * @note On some ports it is faster to invoke chSysUnlock() directly.
     */
    static void Unlock(void);

#ifdef CH_USE_SYSTEMTIME
    /**
     * Returns the system time as system ticks.
     */
    static systime_t GetTime(void);
#endif /* CH_USE_SYSTEMTIME */
  };

#ifdef CH_USE_VIRTUAL_TIMERS
  /**
   * Timer class.
   */
  class Timer {
  public:
    ::VirtualTimer timer;

    /**
     * Starts the timer.
     * @note It must be called with the interrupts disabled.
     * @note The associated function is invoked by an interrupt handler.
     */
    void Set(systime_t time, vtfunc_t vtfunc, void *par);

    /**
     * Resets the timer.
     * @note It must be called with the interrupts disabled.
     * @note The timer MUST be active when this function is invoked.
     */
    void Reset();

    /**
     * Returns true if the timer is armed.
     */
    bool IsArmed(void);
  };
#endif /* CH_USE_VIRTUAL_TIMER */

  /**
   * Base class for a ChibiOS/RT thread, the thread body is the virtual
   * function /p Main().
   */
  class BaseThread {
  protected:
    /**
     * Thread exit.
     */
//    __attribute__((noreturn))
    void Exit(msg_t msg);

    /**
     * Change thread priority.
     */
    void SetPriority(tprio_t newprio);

#ifdef CH_USE_MESSAGES
    /**
     * Waits for a message and returns it.
     */
    msg_t WaitMessage(void);

    /**
     * Returns an enqueued message or /p NULL.
     */
    msg_t GetMessage(void);

    /**
     * Releases the next message in queue with a reply.
     */
    void ReleaseMessage(msg_t msg);

    /**
     * Returns true if there is at least one message in queue.
     */
    bool IsPendingMessage(void);
#endif /* CH_USE_MESSAGES */

  public:
    ::Thread *thread_ref;

    /**
     * Thread constructor.
     */
    BaseThread(tprio_t prio, tmode_t mode, void *workspace, size_t wsize);

#ifdef CH_USE_WAITEXIT
    /**
     * Synchronization on Thread exit.
     */
    msg_t Wait(void);
#endif /* CH_USE_WAITEXIT */

#ifdef CH_USE_RESUME
    /**
     * Resumes thread.
     */
    void Resume(void);
#endif /* CH_USE_RESUME */

#ifdef CH_USE_TERMINATE
    /**
     * Requests thread termination.
     */
    void Terminate(void);
#endif /* CH_USE_TERMINATE */

#ifdef CH_USE_SLEEP
    /**
     * Suspends the thread execution for the specified number of system ticks.
     */
    void Sleep(systime_t n);

#ifdef CH_USE_SYSTEMTIME
    /**
     * Suspends the thread execution until the specified time arrives.
     */
    void SleepUntil(systime_t time);
#endif /* CH_USE_SYSTEMTIME */
#endif /* CH_USE_SLEEP */

#ifdef CH_USE_MESSAGES
    /**
     * Sends a message to the thread and returns the answer.
     */
    msg_t SendMessage(msg_t msg);
#endif /* CH_USE_MESSAGES */

    /**
     * Thread body function.
     */
//    __attribute__((naked))
    virtual msg_t Main(void);
  };

#ifdef CH_USE_SEMAPHORES
  /**
   * Class encapsulating a /p Semaphore.
   */
  class Semaphore {
  public:
    ::Semaphore sem;

    /**
     * Semaphore constructor.
     */
    Semaphore(cnt_t n);

    /**
     * Resets a semaphore to a given positive value.
     */
    void Reset(cnt_t n);

    /**
     * Wait operation on the semaphore.
     */
    msg_t Wait(void);

#ifdef CH_USE_SEMAPHORES_TIMEOUT
    /**
     * Wait operation on the semaphore with timeout.
     */
    msg_t WaitTimeout(systime_t time);
#endif /* CH_USE_SEMAPHORES_TIMEOUT */

    /**
     * Signal operation on the semaphore.
     */
    void Signal(void);
  };
#endif /* CH_USE_SEMAPHORES */

#ifdef CH_USE_MUTEXES
  /**
   * Class encapsulating a /p Mutex.
   */
  class Mutex {
  public:
    ::Mutex mutex;

    /**
     * Mutex constructor.
     */
    Mutex(void);

    /**
     * Tries a lock operation on the mutex.
     */
    bool TryLock(void);

    /**
     * Lock operation on the mutex.
     */
    void Lock(void);

    /**
     * Unlock operation on the most recently locked mutex.
     */
    static void Unlock(void);

    /**
     * Unlocks all the mutexes owned by the invoking thread.
     */
    static void UnlockAll(void);
  };
#endif /* CH_USE_MUTEXES */

#ifdef CH_USE_EVENTS
  /**
   * Class encapsulating an /p EventSource.
   */
  class Event {
  public:
    EventSource event;

    /**
     * Event constructor.
     */
    Event(void);

    /**
     * Registers a listener on the event source.
     */
    void Register(EventListener *elp, eventid_t eid);

    /**
     * Unregisters a listener from the event source.
     */
    void Unregister(EventListener *elp);

    /**
     * Sends an event.
     */
    void Send(void);

    /**
     * Clears specified events from the pending events mask.
     */
    static void Clear(eventmask_t mask);

    /**
     * Waits and dispatchs events.
     */
    static eventid_t Wait(eventmask_t ewmask, const evhandler_t handlers[]);

#ifdef CH_USE_EVENTS_TIMEOUT
    /**
     * Waits and dispatchs events with timeout specification.
     */
    static eventid_t WaitTimeout(eventmask_t ewmask,
                                 const evhandler_t handlers[],
                                 systime_t time);
#endif /* CH_USE_EVENTS_TIMEOUT */
  };
#endif /* CH_USE_EVENTS */
}

#endif /* _CH_HPP_ */

/** @} */

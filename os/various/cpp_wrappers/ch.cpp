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
 * @file    ch.cpp
 * @brief   C++ wrapper code.
 *
 * @addtogroup cpp_library
 * @{
 */

#include "ch.hpp"

namespace chibios_rt {

  /*------------------------------------------------------------------------*
   * chibios_rt::System                                                     *
   *------------------------------------------------------------------------*/
  void System::init(void) {

    chSysInit();
  }

  void System::lock(void) {

    chSysLock();
  }

  void System::unlock(void) {

    chSysUnlock();
  }

  systime_t System::getTime(void) {

    return chTimeNow();
  }

  /*------------------------------------------------------------------------*
   * chibios_rt::Timer                                                      *
   *------------------------------------------------------------------------*/
  void Timer::setI(systime_t time, vtfunc_t vtfunc, void *par) {

    chVTSetI(&timer_ref, time, vtfunc, par);
  }

  void Timer::resetI() {

    if (chVTIsArmedI(&timer_ref))
      chVTResetI(&timer_ref);
  }

  bool Timer::isArmedI(void) {

    return (bool)chVTIsArmedI(&timer_ref);
  }

  /*------------------------------------------------------------------------*
   * chibios_rt::ThreadReference                                            *
   *------------------------------------------------------------------------*/
  msg_t ThreadReference::suspend(void) {
    msg_t msg;

    chSysLock();

    chDbgAssert(thread_ref != NULL,
                "ThreadReference, #1",
                "already referenced");

    thread_ref = chThdSelf();
    chSchGoSleepS(THD_STATE_SUSPENDED);
    msg = thread_ref->p_u.rdymsg;

    chSysUnlock();
    return msg;
  }

  msg_t ThreadReference::suspendS(void) {

    chDbgAssert(thread_ref == NULL,
                "ThreadReference, #2",
                "already referenced");

    thread_ref = chThdSelf();
    chSchGoSleepS(THD_STATE_SUSPENDED);
    return thread_ref->p_u.rdymsg;
  }

  void ThreadReference::resume(msg_t msg) {

    chSysLock()

    chDbgAssert(thread_ref != NULL,
                "ThreadReference, #3",
                "not referenced");

    if (thread_ref) {
      Thread *tp = thread_ref;
      thread_ref = NULL;
      chSchWakeupS(tp, msg);
    }

    chSysUnlock();
  }

  void ThreadReference::resumeI(msg_t msg) {

    chDbgAssert(thread_ref != NULL,
                "ThreadReference, #4",
                "not referenced");

    if (thread_ref) {
      Thread *tp = thread_ref;
      thread_ref = NULL;
      tp->p_msg = msg;
      chSchReadyI(tp);
    }
  }

  void ThreadReference::requestTerminate(void) {

    chThdTerminate(thread_ref);
  }

#if CH_USE_WAITEXIT || defined(__DOXYGEN__)
    msg_t ThreadReference::wait(void) {

      chDbgAssert(thread_ref != NULL,
                  "ThreadReference, #5",
                  "not referenced");

      msg_t msg = chThdWait(thread_ref);
      thread_ref = NULL;
      return msg;
    }
#endif /* CH_USE_WAITEXIT */

#if CH_USE_MESSAGES || defined(__DOXYGEN__)
  msg_t ThreadReference::sendMessage(msg_t msg) {

    chDbgAssert(thread_ref != NULL,
                "ThreadReference, #6",
                "not referenced");

    return chMsgSend(thread_ref, msg);
  }

  bool ThreadReference::isPendingMessage(void) {

    return (bool)chMsgIsPendingI(thread_ref);
  }
#endif /* CH_USE_MESSAGES */

#if CH_USE_EVENTS
    void ThreadReference::signalEvents(eventmask_t mask) {

      chEvtSignal(thread_ref, mask);
    }

    void ThreadReference::signalEventsI(eventmask_t mask) {

      chEvtSignalI(thread_ref, mask);
    }
#endif /* CH_USE_EVENTS */

#if CH_USE_DYNAMIC
#endif /* CH_USE_DYNAMIC */

  /*------------------------------------------------------------------------*
   * chibios_rt::BaseThread                                             *
   *------------------------------------------------------------------------*/
  BaseThread::BaseThread() : ThreadReference(NULL) {

  }

  msg_t _thd_start(void *arg) {

    return ((BaseThread *)arg)->Main();
  }

  void BaseThread::setName(const char *tname) {

    chRegSetThreadName(tname);
  }

  tprio_t BaseThread::setPriority(tprio_t newprio) {

    return chThdSetPriority(newprio);
  }

  void BaseThread::exit(msg_t msg) {

    chThdExit(msg);
  }

  bool BaseThread::shouldTerminate(void) {

    return (bool)chThdShouldTerminate();
  }

  void BaseThread::sleep(systime_t interval){

    chThdSleep(interval);
  }

  void BaseThread::sleepUntil(systime_t time) {

    chThdSleepUntil(time);
  }

#if CH_USE_MESSAGES
  msg_t BaseThread::getMessage(ThreadReference* trp) {

    return chMsgGet(trp->thread_ref);
  }

  void BaseThread::releaseMessage(ThreadReference* trp, msg_t msg) {

    chMsgRelease(trp->thread_ref, msg);
  }
#endif /* CH_USE_MESSAGES */

#if CH_USE_EVENTS
  eventmask_t BaseThread::getAndClearEvents(eventmask_t mask) {

    return chEvtGetAndClearEvents(mask);
  }

  eventmask_t BaseThread::addEvents(eventmask_t mask) {

    return chEvtAddEvents(mask);
  }

  eventmask_t BaseThread::waitOneEvent(eventmask_t ewmask) {

    return chEvtWaitOne(ewmask);
  }

  eventmask_t BaseThread::waitAnyEvent(eventmask_t ewmask) {

    return chEvtWaitAny(ewmask);
  }

  eventmask_t BaseThread::waitAllEvents(eventmask_t ewmask) {

    return chEvtWaitAll(ewmask);
  }

#if CH_USE_EVENTS_TIMEOUT
  eventmask_t BaseThread::waitOneEventTimeout(eventmask_t ewmask,
                                              systime_t time) {

    return chEvtWaitOneTimeout(ewmask, time);
  }

  eventmask_t BaseThread::waitAnyEventTimeout(eventmask_t ewmask,
                                              systime_t time) {

    return chEvtWaitAnyTimeout(ewmask, time);
  }

  eventmask_t BaseThread::waitAllEventsTimeout(eventmask_t ewmask,
                                               systime_t time) {

    return chEvtWaitAllTimeout(ewmask, time);
  }
#endif /* CH_USE_EVENTS_TIMEOUT */

  void BaseThread::dispatchEvents(const evhandler_t handlers[],
                                  eventmask_t mask) {

    chEvtDispatch(handlers, mask);
  }
#endif /* CH_USE_EVENTS */

#if CH_USE_SEMAPHORES
  /*------------------------------------------------------------------------*
   * chibios_rt::Semaphore                                                  *
   *------------------------------------------------------------------------*/
  Semaphore::Semaphore(cnt_t n) {

    chSemInit(&sem, n);
  }

  void Semaphore::reset(cnt_t n) {

    chSemReset(&sem, n);
  }

  msg_t Semaphore::wait(void) {

    return chSemWait(&sem);
  }

  msg_t Semaphore::waitTimeout(systime_t time) {

    return chSemWaitTimeout(&sem, time);
  }

  void Semaphore::signal(void) {

    chSemSignal(&sem);
  }

#if CH_USE_SEMSW
  msg_t Semaphore::signalWait(Semaphore *ssem, Semaphore *wsem) {

    return chSemSignalWait(&ssem->sem, &wsem->sem);
  }
#endif /* CH_USE_SEMSW */
#endif /* CH_USE_SEMAPHORES */

#if CH_USE_MUTEXES
  /*------------------------------------------------------------------------*
   * chibios_rt::Mutex                                                      *
   *------------------------------------------------------------------------*/
  Mutex::Mutex(void) {

    chMtxInit(&mutex);
  }

  bool Mutex::tryLock(void) {

    return chMtxTryLock(&mutex);
  }

  void Mutex::lock(void) {

    chMtxLock(&mutex);
  }

  void Mutex::unlock(void) {

    chMtxUnlock();
  }

  void Mutex::unlockAll(void) {

    chMtxUnlockAll();
  }

#if CH_USE_CONDVARS
  /*------------------------------------------------------------------------*
   * chibios_rt::CondVar                                                    *
   *------------------------------------------------------------------------*/
  CondVar::CondVar(void) {

    chCondInit(&condvar);
  }

  void CondVar::signal(void) {

    chCondSignal(&condvar);
  }

  void CondVar::broadcast(void) {

    chCondBroadcast(&condvar);
  }

  msg_t CondVar::wait(void) {

    return chCondWait(&condvar);
  }

#if CH_USE_CONDVARS_TIMEOUT
  msg_t CondVar::waitTimeout(systime_t time) {

    return chCondWaitTimeout(&condvar, time);
  }
#endif /* CH_USE_CONDVARS_TIMEOUT */
#endif /* CH_USE_CONDVARS */
#endif /* CH_USE_MUTEXES */

#if CH_USE_EVENTS
  /*------------------------------------------------------------------------*
   * chibios_rt::EvtListener                                              *
   *------------------------------------------------------------------------*/
  flagsmask_t EvtListener::getAndClearFlags(void) {

    return chEvtGetAndClearFlags(&ev_listener);
  }

  /*------------------------------------------------------------------------*
   * chibios_rt::EvtSource                                                *
   *------------------------------------------------------------------------*/
  EvtSource::EvtSource(void) {

    chEvtInit(&ev_source);
  }

  void EvtSource::registerOne(chibios_rt::EvtListener *elp,
                                eventid_t eid) {

    chEvtRegister(&ev_source, &elp->ev_listener, eid);
  }

  void EvtSource::registerMask(chibios_rt::EvtListener *elp,
                                 eventmask_t emask) {

    chEvtRegisterMask(&ev_source, &elp->ev_listener, emask);
  }

  void EvtSource::unregister(chibios_rt::EvtListener *elp) {

    chEvtUnregister(&ev_source, &elp->ev_listener);
  }

  void EvtSource::broadcastFlags(flagsmask_t flags) {

    chEvtBroadcastFlags(&ev_source, flags);
  }

  void EvtSource::broadcastFlagsI(flagsmask_t flags) {

    chEvtBroadcastFlagsI(&ev_source, flags);
  }
#endif /* CH_USE_EVENTS */
}

/** @} */

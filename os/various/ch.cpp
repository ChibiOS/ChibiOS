/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
  void System::Init(void) {

    chSysInit();
  }

  void System::Lock(void) {

    chSysLock();
  }

  void System::Unlock(void) {

    chSysUnlock();
  }

  systime_t System::GetTime(void) {

    return chTimeNow();
  }

  /*------------------------------------------------------------------------*
   * chibios_rt::Timer                                                      *
   *------------------------------------------------------------------------*/
  void Timer::Set(systime_t time, vtfunc_t vtfunc, void *par) {

    chVTSetI(&timer, time, vtfunc, par);
  }

  void Timer::Reset() {

    chVTResetI(&timer);
  }

  bool Timer::IsArmed(void) {

    return chVTIsArmedI(&timer);
  }

  /*------------------------------------------------------------------------*
   * chibios_rt::BaseThread                                                 *
   *------------------------------------------------------------------------*/
  static msg_t thdstart(void *arg) {

    return ((BaseThread *)arg)->Main();
  }

  BaseThread::BaseThread(void *workspace, size_t wsize, tprio_t prio) {

    thread_ref = chThdCreateStatic(workspace, wsize, prio, thdstart, this);
  }

  void BaseThread::Exit(msg_t msg) {

    chThdExit(msg);
  }

#if CH_USE_WAITEXIT
  msg_t BaseThread::Wait(void) {

    return chThdWait(thread_ref);
  }
#endif /* CH_USE_WAITEXIT */

  void BaseThread::SetPriority(tprio_t newprio) {

    chThdSetPriority(newprio);
  }

  void BaseThread::Resume(void) {

    chThdResume(thread_ref);
  }

  void BaseThread::Terminate(void) {

    chThdTerminate(thread_ref);
  }

  void BaseThread::Sleep(systime_t n) {

    chThdSleep(n);
  }

  void BaseThread::SleepUntil(systime_t time) {

    chThdSleepUntil(time);
  }

#if CH_USE_MESSAGES
  msg_t BaseThread::SendMessage(::Thread* tp, msg_t msg) {

    return chMsgSend(tp, msg);
  }

  msg_t BaseThread::SendMessage(msg_t msg) {

    return chMsgSend(thread_ref, msg);
  }

  msg_t BaseThread::WaitMessage(void) {

    return chMsgWait();
  }

  msg_t BaseThread::GetMessage(void) {

    return chMsgGet();
  }

  void BaseThread::ReleaseMessage(msg_t msg) {

    chMsgRelease(msg);
  }

  bool BaseThread::IsPendingMessage(void) {

    return chMsgIsPendingI(currp);
  }
#endif /* CH_USE_MESSAGES */

  msg_t BaseThread::Main(void) {

    return 0;
  }

#if CH_USE_SEMAPHORES
  /*------------------------------------------------------------------------*
   * chibios_rt::Semaphore                                                  *
   *------------------------------------------------------------------------*/
  Semaphore::Semaphore(cnt_t n) {

    chSemInit(&sem, n);
  }

  void Semaphore::Reset(cnt_t n) {

    chSemReset(&sem, n);
  }

  msg_t Semaphore::Wait(void) {

    return chSemWait(&sem);
  }

  msg_t Semaphore::WaitTimeout(systime_t time) {

    return chSemWaitTimeout(&sem, time);
  }

  void Semaphore::Signal(void) {

    chSemSignal(&sem);
  }

#if CH_USE_SEMSW
  msg_t Semaphore::SignalWait(Semaphore *ssem, Semaphore *wsem) {

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

  bool Mutex::TryLock(void) {

    return chMtxTryLock(&mutex);
  }

  void Mutex::Lock(void) {

    chMtxLock(&mutex);
  }

  void Mutex::Unlock(void) {

    chMtxUnlock();
  }

  void UnlockAll(void) {

    chMtxUnlockAll();
  }

#if CH_USE_CONDVARS
  /*------------------------------------------------------------------------*
   * chibios_rt::CondVar                                                    *
   *------------------------------------------------------------------------*/
  CondVar::CondVar(void) {

    chCondInit(&condvar);
  }

  void CondVar::Signal(void) {

    chCondSignal(&condvar);
  }

  void CondVar::Broadcast(void) {

    chCondBroadcast(&condvar);
  }

  msg_t CondVar::Wait(void) {

    return chCondWait(&condvar);
  }

#if CH_USE_CONDVARS_TIMEOUT
  msg_t CondVar::WaitTimeout(systime_t time) {

    return chCondWaitTimeout(&condvar, time);
  }
#endif /* CH_USE_CONDVARS_TIMEOUT */
#endif /* CH_USE_CONDVARS */
#endif /* CH_USE_MUTEXES */

#if CH_USE_EVENTS
  /*------------------------------------------------------------------------*
   * chibios_rt::Event                                                      *
   *------------------------------------------------------------------------*/
  Event::Event(void) {

    chEvtInit(&event);
  }

  void Event::Register(EventListener *elp, eventid_t eid) {

    chEvtRegister(&event,elp, eid);
  }

  void Event::RegisterMask(EventListener *elp, eventmask_t emask) {

    chEvtRegisterMask(&event,elp, emask);
  }

  void Event::Unregister(EventListener *elp) {

    chEvtUnregister(&event, elp);
  }

  void Event::Broadcast(void) {

    chEvtBroadcast(&event);
  }

  eventmask_t Event::ClearFlags(eventmask_t mask) {

    return chEvtClearFlags(mask);
  }

  eventmask_t Event::AddFlags(eventmask_t mask) {

    return chEvtAddFlags(mask);
  }

  void Event::Dispatch(const evhandler_t handlers[], eventmask_t mask) {

    chEvtDispatch(handlers, mask);
  }

  eventmask_t Event::WaitOne(eventmask_t ewmask) {

    return chEvtWaitOne(ewmask);
  }

  eventmask_t Event::WaitAny(eventmask_t ewmask) {

    return chEvtWaitAny(ewmask);
  }

  eventmask_t Event::WaitAll(eventmask_t ewmask) {

    return chEvtWaitAll(ewmask);
  }

#if CH_USE_EVENTS_TIMEOUT
  eventmask_t Event::WaitOneTimeout(eventmask_t ewmask, systime_t time) {

    return chEvtWaitOneTimeout(ewmask, time);
  }

  eventmask_t Event::WaitAnyTimeout(eventmask_t ewmask, systime_t time) {

    return chEvtWaitAnyTimeout(ewmask, time);
  }

  eventmask_t Event::WaitAllTimeout(eventmask_t ewmask, systime_t time) {

    return chEvtWaitAllTimeout(ewmask, time);
  }
#endif /* CH_USE_EVENTS_TIMEOUT */
#endif /* CH_USE_EVENTS */
}

/** @} */

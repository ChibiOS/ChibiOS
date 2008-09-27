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

#include <ch.hpp>

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

#ifdef CH_USE_SYSTEMTIME
  systime_t System::GetTime(void) {

    return chSysGetTime();
  }
#endif /* CH_USE_SYSTEMTIME */

#ifdef CH_USE_VIRTUAL_TIMERS
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
#endif /* CH_USE_VIRTUAL_TIMER */

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

#ifdef CH_USE_WAITEXIT
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

#ifdef CH_USE_TERMINATE
  void BaseThread::Terminate(void) {

    chThdTerminate(thread_ref);
  }
#endif /* CH_USE_TERMINATE */

  void BaseThread::Sleep(systime_t n) {

    chThdSleep(n);
  }

#ifdef CH_USE_SYSTEMTIME
  void BaseThread::SleepUntil(systime_t time) {

    chThdSleepUntil(time);
  }
#endif /* CH_USE_SYSTEMTIME */

#ifdef CH_USE_MESSAGES
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

#ifdef CH_USE_SEMAPHORES
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

#ifdef CH_USE_SEMAPHORES_TIMEOUT
  msg_t Semaphore::WaitTimeout(systime_t time) {

    return chSemWaitTimeout(&sem, time);
  }
#endif /* CH_USE_SEMAPHORES_TIMEOUT */

  void Semaphore::Signal(void) {

    chSemSignal(&sem);
  }

#ifdef CH_USE_SEMSW
  msg_t Semaphore::SignalWait(Semaphore *ssem, Semaphore *wsem) {

    return chSemSignalWait(&ssem->sem, &wsem->sem);
  }
#endif /* CH_USE_SEMSW */
#endif /* CH_USE_SEMAPHORES */

#ifdef CH_USE_MUTEXES
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
#endif /* CH_USE_MUTEXES */

#ifdef CH_USE_EVENTS
  /*------------------------------------------------------------------------*
   * chibios_rt::Event                                                      *
   *------------------------------------------------------------------------*/
  Event::Event(void) {

    chEvtInit(&event);
  }

  void Event::Register(EventListener *elp, eventid_t eid) {

    chEvtRegister(&event,elp, eid);
  }

  void Event::Unregister(EventListener *elp) {

    chEvtUnregister(&event, elp);
  }

  void Event::Send(void) {

    chEvtSend(&event);
  }

  void Event::Clear(eventmask_t mask) {

    chEvtClear(mask);
  }

  eventid_t Event::Wait(eventmask_t ewmask, const evhandler_t handlers[]) {

    return chEvtWait(ewmask, handlers);
  }

#ifdef CH_USE_EVENTS_TIMEOUT
  eventid_t Event::WaitTimeout(eventmask_t ewmask,
                               const evhandler_t handlers[],
                               systime_t time) {

    return chEvtWaitTimeout(ewmask, handlers, time);
  }
#endif /* CH_USE_EVENTS_TIMEOUT */
#endif /* CH_USE_EVENTS */
}

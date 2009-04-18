/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file chevents.c
 * @brief Events code.
 * @addtogroup Events
 * @{
 */
#include <ch.h>

#if CH_USE_EVENTS
/**
 * @brief Registers an Event Listener on an Event Source.
 *
 * @param[in] esp pointer to the  @p EventSource structure
 * @param[in] elp pointer to the @p EventListener structure
 * @param[in] emask the mask of event flags to be pended to the thread when the
 *                  event source is broadcasted
 * @note Multiple Event Listeners can specify the same bits to be pended.
 */
void chEvtRegisterMask(EventSource *esp, EventListener *elp, eventmask_t emask) {

  chDbgCheck((esp != NULL) && (elp != NULL), "chEvtRegisterMask");

  chSysLock();
  elp->el_next = esp->es_next;
  esp->es_next = elp;
  elp->el_listener = currp;
  elp->el_mask = emask;
  chSysUnlock();
}

/**
 * @brief Unregisters an Event Listener from its Event Source.
 *
 * @param[in] esp pointer to the  @p EventSource structure
 * @param[in] elp pointer to the @p EventListener structure
 * @note If the event listener is not registered on the specified event source
 *       then the function does nothing.
 * @note For optimal performance it is better to perform the unregister
 *       operations in inverse order of the register operations (elements are
 *       found on top of the list).
 */
void chEvtUnregister(EventSource *esp, EventListener *elp) {
  EventListener *p;

  chDbgCheck((esp != NULL) && (elp != NULL), "chEvtUnregister");

  p = (EventListener *)esp;
  chSysLock();
  while (p->el_next != (EventListener *)esp) {
    if (p->el_next == elp) {
      p->el_next = elp->el_next;
      break;
    }
    p = p->el_next;
  }
  chSysUnlock();
}

/**
 * @brief Clears the pending events specified in the mask.
 *
 * @param[in] mask the events to be cleared
 * @return The pending events that were cleared.
 */
eventmask_t chEvtClear(eventmask_t mask) {
  eventmask_t m;

  chSysLock();

  m = currp->p_epending & mask;
  currp->p_epending &= ~mask;

  chSysUnlock();
  return m;
}

/**
 * @brief Pends a set of event flags on the current thread, this is @b much
 *        faster than using @p chEvtBroadcast() or @p chEvtSignal().
 *
 * @param[in] mask the events to be pended
 * @return The current pending events mask.
 */
eventmask_t chEvtPend(eventmask_t mask) {

  chSysLock();

  mask = (currp->p_epending |= mask);

  chSysUnlock();
  return mask;
}

/**
 * @brief Pends a set of event flags on the specified @p Thread.
 *
 * @param[in] tp the thread to be signaled
 * @param[in] mask the event flags set to be pended
 */
void chEvtSignal(Thread *tp, eventmask_t mask) {

  chDbgCheck(tp != NULL, "chEvtSignal");

  chSysLock();
  chEvtSignalI(tp, mask);
  chSysUnlock();
}

/**
 * @brief Pends a set of event flags on the specified @p Thread.
 *
 * @param[in] tp the thread to be signaled
 * @param[in] mask the event flags set to be pended
 */
void chEvtSignalI(Thread *tp, eventmask_t mask) {

  chDbgCheck(tp != NULL, "chEvtSignalI");

  tp->p_epending |= mask;
  /* Test on the AND/OR conditions wait states.*/
  if (((tp->p_state == PRWTOREVT) && ((tp->p_epending & tp->p_ewmask) != 0)) ||
      ((tp->p_state == PRWTANDEVT) && ((tp->p_epending & tp->p_ewmask) == tp->p_ewmask)))
    chSchReadyI(tp)->p_rdymsg = RDY_OK;
}

/**
 * @brief Signals all the Event Listeners registered on the specified Event
 *        Source.
 *
 * @param[in] esp pointer to the @p EventSource structure
 */
void chEvtBroadcast(EventSource *esp) {

  chSysLock();
  chEvtBroadcastI(esp);
  chSchRescheduleS();
  chSysUnlock();
}

/**
 * @brief Signals all the Event Listeners registered on the specified Event
 *        Source.
 *
 * @param[in] esp pointer to the @p EventSource structure
 */
void chEvtBroadcastI(EventSource *esp) {
  EventListener *elp;

  chDbgCheck(esp != NULL, "chEvtBroadcastI");

  elp = esp->es_next;
  while (elp != (EventListener *)esp) {
    chEvtSignalI(elp->el_listener, elp->el_mask);
    elp = elp->el_next;
  }
}

/**
 * @brief Invokes the event handlers associated with a mask.
 *
 * @param[in] mask mask of the events to be dispatched
 * @param[in] handlers an array of @p evhandler_t. The array must be
 *                     have indexes from zero up the higher registered event
 *                     identifier.
 */
void chEvtDispatch(const evhandler_t handlers[], eventmask_t mask) {
  eventid_t eid;

  chDbgCheck(handlers != NULL, "chEvtDispatch");

  eid = 0;
  while (mask) {
    if (mask & EVENT_MASK(eid)) {
      chDbgAssert(handlers[eid] != NULL,
                  "chEvtDispatch(), #1",
                  "null handler");
      mask &= ~EVENT_MASK(eid);
      handlers[eid](eid);
    }
    eid++;
  }
}

#if CH_OPTIMIZE_SPEED || !CH_USE_EVENTS_TIMEOUT || defined(__DOXYGEN__)
/**
 * @brief Waits for exactly one of the specified events.
 * @details The function waits for one event among those specified in
 *          @p ewmask to become pending then the event is cleared and returned.
 *
 * @param[in] ewmask mask of the events that the function should wait for,
 *                   @p ALL_EVENTS enables all the events
 * @return The mask of the lowest id served and cleared event.
 * @note One and only one event is served in the function, the one with the
 *       lowest event id. The function is meant to be invoked into a loop in
 *       order to serve all the pending events.<br>
 *       This means that Event Listeners with a lower event identifier have
 *       an higher priority.
 */
eventmask_t chEvtWaitOne(eventmask_t ewmask) {
  eventmask_t m;

  chSysLock();

  if ((m = (currp->p_epending & ewmask)) == 0) {
    currp->p_ewmask = ewmask;
    chSchGoSleepS(PRWTOREVT);
    m = currp->p_epending & ewmask;
  }
  m &= -m;
  currp->p_epending &= ~m;

  chSysUnlock();
  return m;
}

/**
 * @brief Waits for any of the specified events.
 * @details The function waits for any event among those specified in
 *          @p ewmask to become pending then the events are cleared and returned.
 *
 * @param[in] ewmask mask of the events that the function should wait for,
 *                   @p ALL_EVENTS enables all the events
 * @return The mask of the served and cleared events.
 */
eventmask_t chEvtWaitAny(eventmask_t ewmask) {
  eventmask_t m;

  chSysLock();

  if ((m = (currp->p_epending & ewmask)) == 0) {
    currp->p_ewmask = ewmask;
    chSchGoSleepS(PRWTOREVT);
    m = currp->p_epending & ewmask;
  }
  currp->p_epending &= ~m;

  chSysUnlock();
  return m;
}

/**
 * @brief Waits for all the specified events.
 * @details The function waits for all the events specified in @p ewmask to
 *          become pending then the events are cleared and returned.
 *
 * @param[in] ewmask mask of the event ids that the function should wait for
 * @return The mask of the served and cleared events.
 */
eventmask_t chEvtWaitAll(eventmask_t ewmask) {

  chSysLock();

  if ((currp->p_epending & ewmask) != ewmask) {
    currp->p_ewmask = ewmask;
    chSchGoSleepS(PRWTANDEVT);
  }
  currp->p_epending &= ~ewmask;

  chSysUnlock();
  return ewmask;
}
#endif /* CH_OPTIMIZE_SPEED || !CH_USE_EVENTS_TIMEOUT */

#if CH_USE_EVENTS_TIMEOUT
/**
 * @brief Waits for exactly one of the specified events.
 * @details The function waits for one event among those specified in
 *          @p ewmask to become pending then the event is cleared and returned.
 *
 * @param[in] ewmask mask of the events that the function should wait for,
 *                   @p ALL_EVENTS enables all the events
 * @param[in] time the number of ticks before the operation timeouts,
 *                 the following special values are allowed:
 *                 - @a TIME_IMMEDIATE immediate timeout.
 *                 - @a TIME_INFINITE no timeout.
 *                 .
 * @return The mask of the lowest id served and cleared event.
 * @retval 0 if the specified timeout expired.
 * @note One and only one event is served in the function, the one with the
 *       lowest event id. The function is meant to be invoked into a loop in
 *       order to serve all the pending events.<br>
 *       This means that Event Listeners with a lower event identifier have
 *       an higher priority.
 */
eventmask_t chEvtWaitOneTimeout(eventmask_t ewmask, systime_t time) {
  eventmask_t m;

  chSysLock();

  if ((m = (currp->p_epending & ewmask)) == 0) {
    if (TIME_IMMEDIATE == time)
      return (eventmask_t)0;
    currp->p_ewmask = ewmask;
    if (chSchGoSleepTimeoutS(PRWTOREVT, time) < RDY_OK)
      return (eventmask_t)0;
    m = currp->p_epending & ewmask;
  }
  m &= -m;
  currp->p_epending &= ~m;

  chSysUnlock();
  return m;
}

/**
 * @brief Waits for any of the specified events.
 * @details The function waits for any event among those specified in
 *          @p ewmask to become pending then the events are cleared and
 *          returned.
 *
 * @param[in] ewmask mask of the events that the function should wait for,
 *                   @p ALL_EVENTS enables all the events
 * @param[in] time the number of ticks before the operation timeouts,
 *                 the following special values are allowed:
 *                 - @a TIME_IMMEDIATE immediate timeout.
 *                 - @a TIME_INFINITE no timeout.
 *                 .
 * @return The mask of the served and cleared events.
 * @retval 0 if the specified timeout expired.
 */
eventmask_t chEvtWaitAnyTimeout(eventmask_t ewmask, systime_t time) {
  eventmask_t m;

  chSysLock();

  if ((m = (currp->p_epending & ewmask)) == 0) {
    if (TIME_IMMEDIATE == time)
      return (eventmask_t)0;
    currp->p_ewmask = ewmask;
    if (chSchGoSleepTimeoutS(PRWTOREVT, time) < RDY_OK)
      return (eventmask_t)0;
    m = currp->p_epending & ewmask;
  }
  currp->p_epending &= ~m;

  chSysUnlock();
  return m;
}

/**
 * @brief Waits for all the specified events.
 * @details The function waits for all the events specified in @p ewmask to
 *          become pending then the events are cleared and returned.
 *
 * @param[in] ewmask mask of the event ids that the function should wait for
 * @param[in] time the number of ticks before the operation timeouts,
 *                 the following special values are allowed:
 *                 - @a TIME_IMMEDIATE immediate timeout.
 *                 - @a TIME_INFINITE no timeout.
 *                 .
 * @return The mask of the served and cleared events.
 * @retval 0 if the specified timeout expired.
 */
eventmask_t chEvtWaitAllTimeout(eventmask_t ewmask, systime_t time) {

  chSysLock();

  if ((currp->p_epending & ewmask) != ewmask) {
    if (TIME_IMMEDIATE == time)
      return (eventmask_t)0;
    currp->p_ewmask = ewmask;
    if (chSchGoSleepTimeoutS(PRWTANDEVT, time) < RDY_OK)
      return (eventmask_t)0;
  }
  currp->p_epending &= ~ewmask;

  chSysUnlock();
  return ewmask;
}
#endif /* CH_USE_EVENTS_TIMEOUT */

#endif /* CH_USE_EVENTS */

/** @} */

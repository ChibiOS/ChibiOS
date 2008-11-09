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
 * @addtogroup Events
 * @{
 */
#include <ch.h>

#ifdef CH_USE_EVENTS
/**
 * Registers an Event Listener on an Event Source.
 * @param esp pointer to the  \p EventSource structure
 * @param elp pointer to the \p EventListener structure
 * @param emask the mask of event flags to be pended to the thread when the
 *              event source is broadcasted
 * @note Multiple Event Listeners can specify the same bits to be pended.
 */
void chEvtRegisterMask(EventSource *esp, EventListener *elp, eventmask_t emask) {

  chSysLock();

  elp->el_next = esp->es_next;
  esp->es_next = elp;
  elp->el_listener = currp;
  elp->el_mask = emask;

  chSysUnlock();
}

/**
 * Unregisters an Event Listener from its Event Source.
 * @param esp pointer to the  \p EventSource structure
 * @param elp pointer to the \p EventListener structure
 * @note If the event listener is not registered on the specified event source
 *       then the function does nothing.
 * @note For optimal performance it is better to perform the unregister
 *       operations in inverse order of the register operations (elements are
 *       found on top of the list).
 */
void chEvtUnregister(EventSource *esp, EventListener *elp) {
  EventListener *p = (EventListener *)esp;

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
 * Clears the pending events specified in the mask.
 * @param mask the events to be cleared
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
 * Makes an events mask pending in the current thread, this is \b much faster than
 * using \p chEvtBreadcast().
 * @param mask the events to be pended
 * @return The current pending events mask.
 */
eventmask_t chEvtPend(eventmask_t mask) {

  chSysLock();

  mask = (currp->p_epending |= mask);

  chSysUnlock();
  return mask;
}

/**
 * Signals all the Event Listeners registered on the specified Event Source.
 * @param esp pointer to the \p EventSource structure
 */
void chEvtBroadcast(EventSource *esp) {

  chSysLock();

  chEvtBroadcastI(esp);
  chSchRescheduleS();

  chSysUnlock();
}

/**
 * Signals all the Event Listeners registered on the specified Event Source.
 * @param esp pointer to the \p EventSource structure
 * @note This function does not reschedule.
 */
void chEvtBroadcastI(EventSource *esp) {
  EventListener *elp;

  elp = esp->es_next;
  while (elp != (EventListener *)esp) {
    Thread *tp = elp->el_listener;

    tp->p_epending |= elp->el_mask;

    /* Test on the AND/OR conditions wait states.*/
    if (((tp->p_state == PRWTOREVT) && ((tp->p_epending & tp->p_ewmask) != 0)) ||
        ((tp->p_state == PRWTANDEVT) && ((tp->p_epending & tp->p_ewmask) == tp->p_ewmask)))
      chSchReadyI(tp)->p_rdymsg = RDY_OK;

    elp = elp->el_next;
  }
}

/**
 * Invokes the event handlers associated with a mask.
 * @param mask mask of the events that should be invoked by the function,
 *               \p ALL_EVENTS enables all the sources
 * @param handlers an array of \p evhandler_t. The array must be
 *                 have indexes from zero up the higher registered event
 *                 identifier. The array can be \p NULL or contain \p NULL
 *                 elements (no callbacks specified).
 */
void chEvtDispatch(const evhandler_t handlers[], eventmask_t mask) {
  eventid_t i;
  eventmask_t m;

  i = 0, m = 1;
  while ((mask & m) == 0) {
    i += 1, m <<= 1;
    mask &= ~m;
    handlers[i](i);
  }
}

#if defined(CH_OPTIMIZE_SPEED) || !defined(CH_USE_EVENT_TIMEOUT)
/**
 * A pending event among those specified in \p ewmask is selected, cleared and
 * its mask returned.
 * @param ewmask mask of the events that the function should wait for,
 *               \p ALL_EVENTS enables all the events
 * @return The mask of the lowest id served and cleared event.
 * @note Only a single event is served in the function, the one with the
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
 * Waits for any of the specified events.
 * The function waits for any event among those specified in \p ewmask to
 * become pending then the events are cleared and returned.
 * @param ewmask mask of the events that the function should wait for,
 *               \p ALL_EVENTS enables all the events
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
 * Waits for all the specified event flags then clears them.
 * The function waits for all the events specified in \p ewmask to become
 * pending then the events are cleared and returned.
 * @param ewmask mask of the event ids that the function should wait for
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

/**
 * The function waits for an event and returns the event identifier, if an
 * event handler is specified then the handler is executed before returning.
 * @param ewmask mask of the events that should be served by the function,
 *               \p ALL_EVENTS enables all the sources
 * @param handlers an array of \p evhandler_t. The array must be
 *                 have indexes from zero up the higher registered event
 *                 identifier. The array can be \p NULL or contain \p NULL
 *                 elements (no callbacks specified).
 * @return The event identifier.
 * @note Only a single event is served in the function, the one with the
 *       lowest event id. The function is meant to be invoked into a loop so
 *       that all events are received and served.<br>
 *       This means that Event Listeners with a lower event identifier have
 *       an higher priority.
 * @deprecated Please use \p chEvtWaitOne() and \p chEvtDispatch() instead,
 *             this function will be removed in version 1.0.0.
 */
eventid_t chEvtWait(eventmask_t ewmask,
                    const evhandler_t handlers[]) {
  eventid_t i;
  eventmask_t m;

  chSysLock();

  if ((currp->p_epending & ewmask) == 0) {
    currp->p_ewmask = ewmask;
    chSchGoSleepS(PRWTOREVT);
  }
  i = 0, m = 1;
  while ((currp->p_epending & ewmask & m) == 0)
    i += 1, m <<= 1;
  currp->p_epending &= ~m;

  chSysUnlock();

  if (handlers && handlers[i])
    handlers[i](i);

  return i;
}
#endif /* defined(CH_OPTIMIZE_SPEED) || !defined(CH_USE_EVENT_TIMEOUT) */

#ifdef CH_USE_EVENT_TIMEOUT
/**
 * Waits for a single event.
 * A pending event among those specified in \p ewmask is selected, cleared and
 * its mask returned.
 * @param ewmask mask of the events that the function should wait for,
 *               \p ALL_EVENTS enables all the events
 * @param time the number of ticks before the operation timouts
 * @return The mask of the lowest id served and cleared event.
 * @retval 0 if the specified timeout expired
 * @note Only a single event is served in the function, the one with the
 *       lowest event id. The function is meant to be invoked into a loop in
 *       order to serve all the pending events.<br>
 *       This means that Event Listeners with a lower event identifier have
 *       an higher priority.
 */
eventmask_t chEvtWaitOneTimeout(eventmask_t ewmask, systime_t time) {
  eventmask_t m;

  chSysLock();

  if ((m = (currp->p_epending & ewmask)) == 0) {
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
 * Waits for any of the specified events.
 * The function waits for any event among those specified in \p ewmask to
 * become pending then the events are cleared and returned.
 * @param ewmask mask of the events that the function should wait for,
 *               \p ALL_EVENTS enables all the events
 * @param time the number of ticks before the operation timouts
 * @return The mask of the served and cleared events.
 * @retval 0 if the specified timeout expired
 */
eventmask_t chEvtWaitAnyTimeout(eventmask_t ewmask, systime_t time) {
  eventmask_t m;

  chSysLock();

  if ((m = (currp->p_epending & ewmask)) == 0) {
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
 * Waits for all the specified event flags then clears them.
 * The function waits for all the events specified in \p ewmask to become
 * pending then the events are cleared and returned.
 * @param ewmask mask of the event ids that the function should wait for
 * @param time the number of ticks before the operation timouts
 * @return The mask of the served and cleared events.
 * @retval 0 if the specified timeout expired
 */
eventmask_t chEvtWaitAllTimeout(eventmask_t ewmask, systime_t time) {

  chSysLock();

  if ((currp->p_epending & ewmask) != ewmask) {
    currp->p_ewmask = ewmask;
    if (chSchGoSleepTimeoutS(PRWTANDEVT, time) < RDY_OK)
      return (eventmask_t)0;
  }
  currp->p_epending &= ~ewmask;

  chSysUnlock();
  return ewmask;
}

/**
 * The function waits for an event or the specified timeout then returns the
 * event identifier, if an event handler is specified then the handler is
 * executed before returning.
 * @param ewmask mask of the events that should be served by the function,
 *               \p ALL_EVENTS enables all the sources
 * @param handlers an array of \p evhandler_t. The array must be
 *                 have indexes from zero up the higher registered event
 *                 identifier. The array can be NULL or contain NULL elements
 *                 (no callback specified).
 * @param time the number of ticks before the operation timouts
 * @return The event identifier.
 * @retval RDY_TIMEOUT if the specified timeout expired.
 * @note Only a single event is served in the function, the one with the
 *       lowest event id. The function is meant to be invoked into a loop so
 *       that all events are received and served.<br>
 *       This means that Event Listeners with a lower event identifier have
 *       an higher priority.
 * @deprecated Please use \p chEvtWaitOneTimeout() and \p chEvtDispatch()
 *             instead, this function will be removed in version 1.0.0.
 */
eventid_t chEvtWaitTimeout(eventmask_t ewmask,
                           const evhandler_t handlers[],
                           systime_t time) {
  eventid_t i;
  eventmask_t m;

  chSysLock();

  if ((currp->p_epending & ewmask) == 0) {
    currp->p_ewmask = ewmask;
    if (chSchGoSleepTimeoutS(PRWTOREVT, time) < RDY_OK)
      return RDY_TIMEOUT;
  }
  i = 0, m = 1;
  while ((currp->p_epending & ewmask & m) == 0)
    i += 1, m <<= 1;
  currp->p_epending &= ~m;

  chSysUnlock();

  if (handlers && handlers[i])
    handlers[i](i);

  return i;
}
#endif /* CH_USE_EVENT_TIMEOUT */

#endif /* CH_USE_EVENTS */

/** @} */

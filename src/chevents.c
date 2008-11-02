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
 * @param eid numeric identifier assigned to the Event Listener. The identifier
 *            is used as index for the event callback function.
 *            The value must range between zero and the size, in bit, of the
 *            \p eventid_t type minus one.
 * @note Multiple Event Listeners can use the same event identifier, the
 *       listener will share the callback function.
 */
void chEvtRegister(EventSource *esp, EventListener *elp, eventid_t eid) {

  chSysLock();

  elp->el_next = esp->es_next;
  esp->es_next = elp;
  elp->el_listener = currp;
  elp->el_id = eid;

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
 */
void chEvtClear(eventmask_t mask) {

  chSysLock();

  currp->p_epending &= ~mask;

  chSysUnlock();
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

    tp->p_epending |= EventMask(elp->el_id);
    if ((tp->p_state == PRWTEVENT) && (tp->p_epending & tp->p_ewmask))
      chSchReadyI(tp)->p_rdymsg = RDY_OK;
    elp = elp->el_next;
  }
}

#ifdef CH_USE_EVENTS_TIMEOUT
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
 */
eventid_t chEvtWait(eventmask_t ewmask,
                    const evhandler_t handlers[]) {

  return chEvtWaitTimeout(ewmask, handlers, TIME_INFINITE);
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
 * @note The function is available only if the \p CH_USE_EVENTS_TIMEOUT
 *       option is enabled in \p chconf.h.
 */
eventid_t chEvtWaitTimeout(eventmask_t ewmask,
                           const evhandler_t handlers[],
                           systime_t time) {
  eventid_t i;
  eventmask_t m;

  chSysLock();

  if ((currp->p_epending & ewmask) == 0) {
    currp->p_ewmask = ewmask;
    if (chSchGoSleepTimeoutS(PRWTEVENT, time) < RDY_OK)
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

#else /* !CH_USE_EVENTS_TIMEOUT */
eventid_t chEvtWait(eventmask_t ewmask,
                    const evhandler_t handlers[]) {
  eventid_t i;
  eventmask_t m;

  chSysLock();

  if ((currp->p_epending & ewmask) == 0) {
    currp->p_ewmask = ewmask;
    chSchGoSleepS(PRWTEVENT);
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

#endif /* CH_USE_EVENTS_TIMEOUT */

#endif /* CH_USE_EVENTS */

/** @} */

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
 * using \p chEvtBroadcast().
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
 * @param mask mask of the events to be dispatched
 * @param handlers an array of \p evhandler_t. The array must be
 *                 have indexes from zero up the higher registered event
 *                 identifier.
 */
void chEvtDispatch(const evhandler_t handlers[], eventmask_t mask) {
  eventid_t eid;

  eid = 0;
  while (mask) {
    if (mask & EVENT_MASK(eid)) {
      mask &= ~EVENT_MASK(eid);
      handlers[eid](eid);
    }
    eid++;
  }
}

#if defined(CH_OPTIMIZE_SPEED) || !defined(CH_USE_EVENTS_TIMEOUT) || \
    defined(__DOXIGEN__)
/**
 * A pending event among those specified in \p ewmask is selected, cleared and
 * its mask returned.
 * @param ewmask mask of the events that the function should wait for,
 *               \p ALL_EVENTS enables all the events
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
#endif /* defined(CH_OPTIMIZE_SPEED) || !defined(CH_USE_EVENTS_TIMEOUT) */

#ifdef CH_USE_EVENTS_TIMEOUT
/**
 * Waits for a single event.
 * A pending event among those specified in \p ewmask is selected, cleared and
 * its mask returned.
 * @param ewmask mask of the events that the function should wait for,
 *               \p ALL_EVENTS enables all the events
 * @param time the number of ticks before the operation timouts
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
 * @retval 0 if the specified timeout expired.
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
 * @retval 0 if the specified timeout expired.
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
#endif /* CH_USE_EVENTS_TIMEOUT */

#endif /* CH_USE_EVENTS */

/** @} */

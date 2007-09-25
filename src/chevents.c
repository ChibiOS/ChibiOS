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
 *            \p t_eventid type minus one.
 * @note Multiple Event Listeners can use the same event identifier, the
 *       listener will share the callback function.
 */
void chEvtRegister(EventSource *esp, EventListener *elp, t_eventid eid) {

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
 * @note For optimal performance perform the unregister operations in inverse
 *       order of the register operations (elements are found on top of the
 *       list).
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
void chEvtClear(t_eventmask mask) {

  chSysLock();

  currp->p_epending &= ~mask;

  chSysUnlock();
}

/**
 * Signals all the Event Listeners registered on the specified Event Source.
 * @param esp pointer to the \p EventSource structure
 */
void chEvtSend(EventSource *esp) {
  EventListener *elp;
  BOOL flag = FALSE;

  chSysLock();

  elp = esp->es_next;
  while (elp != (EventListener *)esp) {
    Thread *tp = elp->el_listener;

    tp->p_epending |= EventMask(elp->el_id);
    if ((tp->p_state == PRWTEVENT) && (tp->p_epending & tp->p_ewmask))
      chSchReadyI(tp), flag = TRUE;
    elp = elp->el_next;
  }
  if (flag)
    chSchRescheduleI();

  chSysUnlock();
}

/**
 * Signals all the Event Listeners registered on the specified Event Source.
 * @param esp pointer to the \p EventSource structure
 * @note This function must be called with interrupts disabled.
 */
void chEvtSendI(EventSource *esp) {
  EventListener *elp;

  elp = esp->es_next;
  while (elp != (EventListener *)esp) {
    Thread *tp = elp->el_listener;

    tp->p_epending |= EventMask(elp->el_id);
    if ((tp->p_state == PRWTEVENT) && (tp->p_epending & tp->p_ewmask))
      chSchReadyI(tp);
    elp = elp->el_next;
  }
}

/**
 * The function waits for an event and returns the event identifier, if an
 * event handler is specified then the handler is executed before returning.
 * @param ewmask mask of the events that should be served by the function,
 *               \p ALL_EVENTS enables all the sources
 * @param handlers an array of \p t_evhandler. The array must be
 *                 have indexes from zero up the higher registered event
 *                 identifier. The array can be \p NULL or contain \p NULL
 *                 elements (no callbacks specified).
 * @return the event identifier
 * @note Only a single event is served in the function, the one with the
 *       lowest event id. The function is meant to be invoked into a loop so
 *        that all events are received and served.<br>
 *       This means that Event Listeners with a lower event identifier have
 *       an higher priority.
 */
t_eventid chEvtWait(t_eventmask ewmask, t_evhandler handlers[]) {
  t_eventid i;
  t_eventmask m;

  chSysLock();

  if ((currp->p_epending & ewmask) == 0) {
    currp->p_ewmask = ewmask;
    chSchGoSleepI(PRWTEVENT);
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

#ifdef CH_USE_EVENTS_TIMEOUT
static void unwait(void *p) {

// Test removed, it should never happen.
//  if (((Thread *)p)->p_state == PRWTEVENT)
  chSchReadyI(dequeue(p))->p_rdymsg = RDY_TIMEOUT;
}

/**
 * The function waits for an event or the specified timeout then returns the
 * event identifier, if an event handler is specified then the handler is
 * executed before returning.
 * @param ewmask mask of the events that should be served by the function,
 *               \p ALL_EVENTS enables all the sources
 * @param handlers an array of \p t_evhandler. The array must be
 *                 have indexes from zero up the higher registered event
 *                 identifier. The array can be NULL or contain NULL elements
 *                 (no callback specified).
 * @param time the number of ticks before the operation timouts, if set to
 *             zero then the function exits immediatly with \p RDY_TIMEOUT if
 *             there are not serviceable events pending
 * @return the event identifier or \p RDY_TIMEOUT the specified time expired or
 *         if the timeout was set to zero and no serviceable pending events
 *         were present
 * @note Only a single event is served in the function, the one with the
 *       lowest event id. The function is meant to be invoked into a loop so
 *        that all events are received and served.<br>
 *       This means that Event Listeners with a lower event identifier have
 *       an higher priority.
 * @note The function is available only if the \p CH_USE_EVENTS_TIMEOUT
 *       option is enabled in \p chconf.h.
 */
t_eventid chEvtWaitTimeout(t_eventmask ewmask,
                           t_evhandler handlers[],
                           t_time time) {
  t_eventid i;
  t_eventmask m;

  chSysLock();

  if ((currp->p_epending & ewmask) == 0) {
    VirtualTimer vt;

    if (time == 0) {

      chSysUnlock();
      return RDY_TIMEOUT;
    }

    chVTSetI(&vt, time, unwait, currp);
    currp->p_ewmask = ewmask;
    chSchGoSleepI(PRWTEVENT);
    if (!vt.vt_func) {

      chSysUnlock();
      return RDY_TIMEOUT; // No need to read t_rdymsg value, it is a timeout.
    }
    chVTResetI(&vt);
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
#endif /*CH_USE_EVENTS_TIMEOUT */

#endif /* CH_USE_EVENTS */

/** @} */

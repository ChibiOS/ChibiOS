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

#ifndef _EVENTS_H_
#define _EVENTS_H_

#ifdef CH_USE_EVENTS

/** All events allowed mask.*/
#define ALL_EVENTS -1

typedef struct EventListener EventListener;

/**
 * Event Listener structure.
 */
struct EventListener {
  /** Next Event Listener registered on the Event Source.*/
  EventListener     *el_next;
  /** Thread interested in the Event Source.*/
  Thread            *el_listener;
  /** Event identifier associated by the thread to the Event Source.*/
  eventid_t         el_id;
};

/**
 * Event Source structure.
 */
typedef struct EventSource {
  /** First Event Listener registered on the Event Source.*/
  EventListener     *es_next;
} EventSource;

/** Returns the event mask from the event identifier.*/
#define EventMask(eid) (1 << (eid))

/**
 * Initializes an Event Source.
 * @param esp pointer to the \p EventSource structure
 * @note Can be called with interrupts disabled or enabled.
 */
#define chEvtInit(esp) \
        ((esp)->es_next = (EventListener *)(void *)(esp))

/**
 * Verifies if there is at least one \p EventListener registered on the
 * \p EventSource.
 * @param esp pointer to the \p EventSource structure
 * @note Can be called with interrupts disabled or enabled.
 */
#define chEvtIsListening(esp) \
                ((esp) != (EventListener *)(void *)(esp)->es_next)


/** Event Handler callback function.*/
typedef void (*evhandler_t)(eventid_t);

#ifdef __cplusplus
extern "C" {
#endif
  void chEvtRegister(EventSource *esp, EventListener *elp, eventid_t eid);
  void chEvtUnregister(EventSource *esp, EventListener *elp);
  eventmask_t chEvtClear(eventmask_t mask);
  eventmask_t chEvtPend(eventmask_t mask);
  eventmask_t chEvtWaitOneTimeout(eventmask_t ewmask, systime_t time);
  eventmask_t chEvtWaitAnyTimeout(eventmask_t ewmask, systime_t time);
  eventmask_t chEvtWaitAllTimeout(eventmask_t ewmask, systime_t time);
  void chEvtBroadcast(EventSource *esp);
  void chEvtBroadcastI(EventSource *esp);
  eventid_t chEvtWait(eventmask_t ewmask,
                      const evhandler_t handlers[]);
  eventid_t chEvtWaitTimeout(eventmask_t ewmask,
                             const evhandler_t handlers[],
                             systime_t time);
#ifdef __cplusplus
}
#endif

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
#define chEvtWaitOne(ewmask) chEvtWaitOneTimeout(ewmask, TIME_INFINITE)

/**
 * Waits for any of the specified events.
 * The function waits for any event among those specified in \p ewmask to
 * become pending then the events are cleared and returned.
 * @param ewmask mask of the events that the function should wait for,
 *               \p ALL_EVENTS enables all the events
 * @return The mask of the served and cleared events.
 */
#define chEvtWaitAny(ewmask) chEvtWaitAnyTimeout(ewmask, TIME_INFINITE)

/**
 * Waits for all the specified event flags then clears them.
 * The function waits for all the events specified in \p ewmask to become
 * pending then the events are cleared and returned.
 * @param ewmask mask of the event ids that the function should wait for
 * @return The mask of the served and cleared events.
 */
#define chEvtWaitAll(ewmask) chEvtWaitAllTimeout(ewmask, TIME_INFINITE)

/*
 * Old function names, deprecated, will be removed in some next release.
 */
#define chEvtSend chEvtBroadcast
#define chEvtSendI chEvtBroadcastI

#endif /* CH_USE_EVENTS */

#endif /* _EVENTS_H_ */

/** @} */

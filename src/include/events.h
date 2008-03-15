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
        ((esp)->es_next = (void *)(esp))

/**
 * Verifies if there is at least one \p EventListener registered on the
 * \p EventSource.
 * @param esp pointer to the \p EventSource structure
 * @note Can be called with interrupts disabled or enabled.
 */
#define chEvtIsListening(esp) \
                ((esp) != (void *)(esp)->es_next)


/** Event Handler callback function.*/
typedef void (*evhandler_t)(eventid_t);

#ifdef __cplusplus
extern "C" {
#endif
  void chEvtRegister(EventSource *esp, EventListener *elp, eventid_t eid);
  void chEvtUnregister(EventSource *esp, EventListener *elp);
  void chEvtClear(eventmask_t mask);
  void chEvtSend(EventSource *esp);
  void chEvtSendI(EventSource *esp);
  eventid_t chEvtWait(eventmask_t ewmask,
                      const evhandler_t handlers[]);
#ifdef CH_USE_EVENTS_TIMEOUT
  eventid_t chEvtWaitTimeout(eventmask_t ewmask,
                             const evhandler_t handlers[],
                             systime_t time);
#endif
#ifdef __cplusplus
}
#endif

#endif /* CH_USE_EVENTS */

#endif /* _EVENTS_H_ */

/** @} */

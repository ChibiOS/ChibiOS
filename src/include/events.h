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
  EventListener         *el_next;
  /** Thread interested in the Event Source.*/
  Thread                *el_listener;
  /** Event flags mask associated by the thread to the Event Source.*/
  eventmask_t           el_mask;
};

/**
 * Event Source structure.
 */
typedef struct EventSource {
  /** First Event Listener registered on the Event Source.*/
  EventListener         *es_next;
} EventSource;

/** Returns the event mask from the event identifier.*/
#define EVENT_MASK(eid) (1 << (eid))

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
                ((void *)(esp) != (void *)(esp)->es_next)

/** Event Handler callback function.*/
typedef void (*evhandler_t)(eventid_t);

#ifdef __cplusplus
extern "C" {
#endif
  void chEvtRegisterMask(EventSource *esp, EventListener *elp, eventmask_t emask);
  void chEvtUnregister(EventSource *esp, EventListener *elp);
  eventmask_t chEvtClear(eventmask_t mask);
  eventmask_t chEvtPend(eventmask_t mask);
  void chEvtBroadcast(EventSource *esp);
  void chEvtBroadcastI(EventSource *esp);
  void chEvtDispatch(const evhandler_t handlers[], eventmask_t mask);
#if defined(CH_OPTIMIZE_SPEED) || !defined(CH_USE_EVENTS_TIMEOUT)
  eventmask_t chEvtWaitOne(eventmask_t ewmask);
  eventmask_t chEvtWaitAny(eventmask_t ewmask);
  eventmask_t chEvtWaitAll(eventmask_t ewmask);
#endif
#ifdef CH_USE_EVENTS_TIMEOUT
  eventmask_t chEvtWaitOneTimeout(eventmask_t ewmask, systime_t time);
  eventmask_t chEvtWaitAnyTimeout(eventmask_t ewmask, systime_t time);
  eventmask_t chEvtWaitAllTimeout(eventmask_t ewmask, systime_t time);
#endif
#ifdef __cplusplus
}
#endif

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
#define chEvtRegister(esp, elp, eid) chEvtRegisterMask(esp, elp, EVENT_MASK(eid))

#if !defined(CH_OPTIMIZE_SPEED) && defined(CH_USE_EVENTS_TIMEOUT)
#define chEvtWaitOne(ewmask) chEvtWaitOneTimeout(ewmask, TIME_INFINITE)
#define chEvtWaitAny(ewmask) chEvtWaitAnyTimeout(ewmask, TIME_INFINITE)
#define chEvtWaitAll(ewmask) chEvtWaitAllTimeout(ewmask, TIME_INFINITE)
#endif

#endif /* CH_USE_EVENTS */

#endif /* _EVENTS_H_ */

/** @} */

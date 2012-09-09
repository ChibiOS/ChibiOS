/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    evtimer.h
 * @brief   Events Generator Timer structures and macros.
 *
 * @addtogroup event_timer
 * @{
 */

#ifndef _EVTIMER_H_
#define _EVTIMER_H_


/*
 * Module dependencies check.
 */
#if !CH_USE_EVENTS
#error "Event Timers require CH_USE_EVENTS"
#endif

/**
 * @brief Event timer structure.
 */
typedef struct {
  VirtualTimer  et_vt;
  EventSource   et_es;
  systime_t     et_interval;
} EvTimer;

#ifdef __cplusplus
extern "C" {
#endif
  void evtStart(EvTimer *etp);
  void evtStop(EvTimer *etp);
#ifdef __cplusplus
}
#endif

/**
 * @brief Initializes an @p EvTimer structure.
 *
 * @param etp the EvTimer structure to be initialized
 * @param time the interval in system ticks
 */
#define evtInit(etp, time) {                                            \
  chEvtInit(&(etp)->et_es);                                             \
  (etp)->et_vt.vt_func = NULL;                                          \
  (etp)->et_interval = (time);                                          \
}

#endif /* _EVTIMER_H_ */

/** @} */

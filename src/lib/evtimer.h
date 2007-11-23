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
 * @file evtimer.h
 * @{
 * Event Timer definitions.
 * @see evtimer.c
 */

#ifndef _EVTIMER_H_
#define _EVTIMER_H_


typedef struct {
  VirtualTimer  et_vt;
  EventSource   et_es;
  t_time        et_interval;
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
 * Initializes an \p EvTimer structure.
 */
#define evtInit(etp, i) (chEvtInit(&(etp)->et_es), \
                        (etp)->et_vt.vt_func = NULL, \
                        (etp)->et_interval = (i))

#endif /* _EVTIMER_H_ */

/** @} */

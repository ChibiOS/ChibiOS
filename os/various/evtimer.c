/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file evtimer.c
 * @brief Events Generator Timer code.
 * @addtogroup event_timer
 * @{
 */

#include "ch.h"
#include "evtimer.h"

static void tmrcb(void *p) {
  EvTimer *etp = p;

  chEvtBroadcastI(&etp->et_es);
  chVTSetI(&etp->et_vt, etp->et_interval, tmrcb, etp);
}

/**
 * @brief Starts the timer
 * @details If the timer was already running then the function has no effect.
 *
 * @param etp pointer to an initialized @p EvTimer structure.
 */
void evtStart(EvTimer *etp) {

  chSysLock();

  if (!chVTIsArmedI(&etp->et_vt))
    chVTSetI(&etp->et_vt, etp->et_interval, tmrcb, etp);

  chSysUnlock();
}

/**
 * @brief Stops the timer.
 * @details If the timer was already stopped then the function has no effect.
 *
 * @param etp pointer to an initialized @p EvTimer structure.
 */
void evtStop(EvTimer *etp) {

  chSysLock();

  if (chVTIsArmedI(&etp->et_vt))
    chVTResetI(&etp->et_vt);

  chSysUnlock();
}

/** @} */

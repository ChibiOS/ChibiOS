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

#include <ch.h>
#include <evtimer.h>
#include <sam7x_emac.h>

#include <uip.h>
#include <uip_arp.h>
#include <httpd.h>
#include <timer.h>
#include <clock-arch.h>

static EvTimer evt;
struct EventListener el0, el1, el2;

void clock_init(void) {}

clock_time_t clock_time( void )
{
  return chSysGetTime();
}

/*
 * Executed as event handler at 1000mS intervals.
 */
static void TimerHandler(eventid_t id) {

  (void)EMACGetLinkStatus();
}

msg_t WebThread(void *p) {
  static const evhandler_t evhndl[] = {
    TimerHandler,
    NULL,
    NULL
  };

  evtInit(&evt, 1000);                  /* Initializes an event timer object.   */
  evtStart(&evt);                       /* Starts the event timer.              */
  chEvtRegister(&evt.et_es, &el0, 0);   /* Registers on the timer event source. */
  chEvtRegister(&EMACFrameTransmitted, &el1, 1);
  chEvtRegister(&EMACFrameReceived, &el2, 2);
  
  while (TRUE) {
    chEvtWait(ALL_EVENTS, evhndl);
  }
  return 0;
}

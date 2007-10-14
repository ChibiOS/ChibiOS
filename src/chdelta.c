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
 * @addtogroup VirtualTimers
 * @{
 */

#include <ch.h>

#ifdef CH_USE_VIRTUAL_TIMERS
DeltaList dlist;

/**
 * Virtual Timers initialization. Internal use only.
 */
void chVTInit(void) {

  dlist.dl_next = dlist.dl_prev = (VirtualTimer *)&dlist;
  dlist.dl_dtime = (t_time)-1;
}

/**
 * Enables a virtual timer.
 * @param vtp the \p VirtualTimer structure pointer
 * @param time the number of time ticks, the value zero is allowed with
 *             meaning "infinite".
 * @param vtfunc the timer callback function. After invoking the callback
 *               the timer is disabled and the structure can be disposed or
 *               reused.
 * @param par a parameter that will be passed to the callback function
 * @note Must be called with the interrupts disabled.
 * @note The associated function is invoked by an interrupt handler.
 */
void chVTSetI(VirtualTimer *vtp, t_time time, t_vtfunc vtfunc, void *par) {

  vtp->vt_par = par;
  if (time) {
    vtp->vt_func = vtfunc;
    VirtualTimer *p = dlist.dl_next;
    while (p->vt_dtime < time) {
      time -= p->vt_dtime;
      p = p->vt_next;
    }

    vtp->vt_prev = (vtp->vt_next = p)->vt_prev;
    vtp->vt_prev->vt_next = p->vt_prev = vtp;
    vtp->vt_dtime = time;
    if (p != (VirtualTimer *)&dlist)
      p->vt_dtime -= time;
  }
  else
    vtp->vt_func = NULL;
}

/**
 * Disables a Virtual Timer.
 * @param vtp the \p VirtualTimer structure pointer
 * @note It must be called with the interrupts disabled.
 * @note The timer MUST be active when this function is invoked.
 */
void chVTResetI(VirtualTimer *vtp) {

  if (vtp->vt_next != (VirtualTimer *)&dlist)
    vtp->vt_next->vt_dtime += vtp->vt_dtime;
  vtp->vt_prev->vt_next = vtp->vt_next;
  vtp->vt_next->vt_prev = vtp->vt_prev;
  vtp->vt_func = 0;
}
#endif /* CH_USE_VIRTUAL_TIMER */

/** @} */

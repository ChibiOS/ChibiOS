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

#ifndef _DELTA_H_
#define _DELTA_H_

#ifdef CH_USE_VIRTUAL_TIMERS

/** Virtual Timer callback function.*/
typedef void (*vtfunc_t)(void *);

typedef struct VirtualTimer VirtualTimer;

/**
 * Virtual Timer descriptor structure.
 */
struct VirtualTimer {
    /** Next timer in the delta list.*/
    VirtualTimer    *vt_next;
    /** Previous timer in the delta list.*/
    VirtualTimer    *vt_prev;
    /** Time delta before timeout.*/
    systime_t       vt_dtime;
    /** Timer callback function pointer. The pointer is reset to zero after
        the callback is invoked.*/
    vtfunc_t        vt_func;
    /** Timer callback function parameter.*/
    void            *vt_par;
};

/**
 * Delta List header.
 * @note The delta list is implemented as a double link bidirectional list in
 *       order to make the unlink time constant, the reset of a virtual timer
 *       is often used in the code. An slower implementation using a single
 *       link list is possible and might be added later with a
 *       \p CH_OPTIMIZE_SPACE option.
 */
typedef struct {
    /** Next timer in the list (the one that will be triggered next).*/
    VirtualTimer    *dl_next;
    /** Last timer in the list.*/
    VirtualTimer    *dl_prev;
    /** Not used but it must be set to /p MAXDELTA.*/
    systime_t       dl_dtime;
} DeltaList;

extern DeltaList dlist;

#define chVTDoTickI() \
  if (&dlist != (DeltaList *)dlist.dl_next) {                           \
    VirtualTimer *vtp;                                                  \
                                                                        \
    --dlist.dl_next->vt_dtime;                                          \
    while (!(vtp = dlist.dl_next)->vt_dtime) {                          \
      vtfunc_t fn = vtp->vt_func;                                       \
      vtp->vt_func = 0;                                                 \
      (vtp->vt_next->vt_prev = (VirtualTimer *)&dlist)->vt_next = vtp->vt_next; \
      fn(vtp->vt_par);                                                  \
    }                                                                   \
  }

/** Infinite time specification.*/
#define TIME_INFINITE 0

/*
 * Virtual Timers APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif
  void chVTInit(void);
  void chVTSetI(VirtualTimer *vtp, systime_t time, vtfunc_t vtfunc, void *par);
  void chVTResetI(VirtualTimer *vtp);
#ifdef __cplusplus
}
#endif

/** Returns TRUE if the speciified timer is armed.*/
#define chVTIsArmedI(vtp) ((vtp)->vt_func != NULL)

#endif /* CH_USE_VIRTUAL_TIMER */

#endif /* _DELTA_H_ */

/** @} */

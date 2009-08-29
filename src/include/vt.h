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
 * @file vt.h
 * @brief Time macros and structures.
 * @addtogroup Time
 * @{
 */

#ifndef _VT_H_
#define _VT_H_

/**
 * Time conversion utility. Converts from seconds to system ticks number.
 */
#define S2ST(sec)   ((systime_t)((sec) * CH_FREQUENCY))

/**
 * Time conversion utility. Converts from milliseconds to system ticks number.
 * @note The result is rounded upward to the next tick boundary.
 */
#define MS2ST(msec) ((systime_t)(((((msec) - 1L) * CH_FREQUENCY) / 1000L) + 1L))

/**
 * Time conversion utility. Converts from microseconds to system ticks number.
 * @note The result is rounded upward to the next tick boundary.
 */
#define US2ST(usec) ((systime_t)(((((usec) - 1L) * CH_FREQUENCY) / 1000000L) + 1L))

/** Virtual Timer callback function.*/
typedef void (*vtfunc_t)(void *);

typedef struct VirtualTimer VirtualTimer;

/**
 * @brief Virtual Timer descriptor structure.
 * @extends DeltaList
 */
struct VirtualTimer {
  VirtualTimer          *vt_next;       /**< Next timer in the delta list.*/
  VirtualTimer          *vt_prev;       /**< Previous timer in the delta list.*/
  systime_t             vt_time;        /**< Time delta before timeout.*/
  vtfunc_t              vt_func;        /**< Timer callback function pointer.
                                             The pointer is reset to zero after
                                             the callback is invoked.*/
  void                  *vt_par;        /**< Timer callback function
                                             parameter.*/
};

/**
 * @brief Virtual timers list header.
 * @note The delta list is implemented as a double link bidirectional list in
 *       order to make the unlink time constant, the reset of a virtual timer
 *       is often used in the code.
 */
typedef struct {
  VirtualTimer          *vt_next;       /**< Next timer in the delta list (the
                                             one that will be triggered next).*/
  VirtualTimer          *vt_prev;       /**< Last timer in the delta list.*/
  systime_t             vt_time;        /**< Must be initialized to -1.*/
  volatile systime_t    vt_systime;     /**< System Time counter.*/
} VTList;

extern VTList vtlist;

#define chVTDoTickI() {                                                 \
  vtlist.vt_systime++;                                                  \
  if (&vtlist != (VTList *)vtlist.vt_next) {                            \
    VirtualTimer *vtp;                                                  \
                                                                        \
    --vtlist.vt_next->vt_time;                                          \
    while (!(vtp = vtlist.vt_next)->vt_time) {                          \
      vtfunc_t fn = vtp->vt_func;                                       \
      vtp->vt_func = NULL;                                              \
      vtp->vt_next->vt_prev = (void *)&vtlist;                          \
      (&vtlist)->vt_next = vtp->vt_next;                                \
      fn(vtp->vt_par);                                                  \
    }                                                                   \
  }                                                                     \
}

/*
 * Virtual Timers APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif
  void vt_init(void);
  void chVTSetI(VirtualTimer *vtp, systime_t time, vtfunc_t vtfunc, void *par);
  void chVTResetI(VirtualTimer *vtp);
  bool_t chTimeIsWithin(systime_t start, systime_t end);
#ifdef __cplusplus
}
#endif

/** Returns TRUE if the speciified timer is armed.*/
#define chVTIsArmedI(vtp) ((vtp)->vt_func != NULL)

/**
 * Returns the number of system ticks since the @p chSysInit() invocation.
 * @return the system ticks number
 * @note The counter can reach its maximum and then returns to zero.
 * @note This function is designed to work with the @p chThdSleepUntil().
 */
#define chTimeNow() (vtlist.vt_systime)

#endif /* _VT_H_ */

/** @} */

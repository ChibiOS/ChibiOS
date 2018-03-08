/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    tsclient.c
 * @brief   TSSI client module code.
 *
 * @addtogroup TSSI
 * @{
 */

#include "ch.h"
#include "tsclient.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/*
 * @brief   Yields all the idle time to secure world.
 * @note    see CH_CFG_IDLE_LOOP_HOOK in chconf.h
 *
 * @notapi
 */
void tsIdle(void) {
  (void)tsInvoke1(TS_HND_IDLE, 0, 0, TS_GRANTED_TIMESLICE * 10);
}

/**
 * @brief   Call a service via smc instruction.
 *
 * @param[in] handle        The handle of the service to invoke.
 *                          The handle is obtained by an invoke to discovery
 *                          service.
 * @param[in,out] svc_data  Service request data, often a reference to a more
 *                          complex structure.
 * @param[in] svc_datalen   Size of the svc_data memory area.
 * @param[in] svc_nsec_time The time slice that will be yielded to the lower
 *                          prio NSEC threads, whenever the service call is
 *                          interrupted, in microseconds.
 *                          This avoids the starvation of lower NSEC thread due
 *                          to continue polling of the called service status.
 *                          0 means no time slice is yielded.
 *
 * @return                  The service status. The value depends on the service.
 *
 * @retval SMC_SVC_OK       generic success value.
 * @retval SMC_SVC_BUSY     the service has a pending request.
 * @retval SMC_SVC_INVALID  bad parameters.
 * @retval SMC_SVC_NOENT    no such service.
 * @retval SMC_SVC_BADH     bad handle.
 *
 * @api
 */
msg_t tsInvokeService(ts_service_t handle, ts_params_area_t data,
                       size_t size, sysinterval_t svc_nsec_time)
{
  int64_t result;

  result = tsInvoke1(handle, data, size, TS_GRANTED_TIMESLICE);
  while ((msg_t)result == SMC_SVC_INTR) {
    if (svc_nsec_time != 0)
      chThdSleepMicroseconds(svc_nsec_time);
    result = tsInvoke1(TS_HND_STQRY, handle, 0, TS_GRANTED_TIMESLICE);
  }
  return (msg_t)result;
}

/** @} */

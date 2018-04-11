/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

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
 * @file    tsclient.h
 * @brief   TSSI client module macros and structures.
 */

#ifndef TSCLIENT_H
#define TSCLIENT_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/* Service registry errors as returned by smc.*/
#define SMC_SVC_OK            MSG_OK        /* No error.*/
#define SMC_SVC_INTR          (msg_t)-1     /* Service interrupted.*/
#define SMC_SVC_NOENT         (msg_t)-2     /* No existent service.*/
#define SMC_SVC_INVALID       (msg_t)-3     /* Invalid service parameter(s).*/
#define SMC_SVC_BADH          (msg_t)-4     /* Invalid service handle.*/
#define SMC_SVC_EXIST         (msg_t)-5     /* Service already exists.*/
#define SMC_SVC_NHND          (msg_t)-6     /* No more services.*/
#define SMC_SVC_BUSY          (msg_t)-7     /* Service busy.*/

/* Special trusted service handles.*/
#define TS_HND_DISCOVERY      ((ts_service_t *)1)  /* Discovery service handle.*/
#define TS_HND_STQRY          ((ts_service_t *)2)  /* Query status service handle.*/
#define TS_HND_IDLE           ((ts_service_t *)3)  /* Idle service handle.*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

#define TS_GRANTED_TIMESLICE  1000                 /* Microseconds.*/

#if !defined(TS_CHECK_EVENT_HOOK)
#define TS_CHECK_EVENT_HOOK(f) {                                             \
  extern event_source_t stubsEventSource;                                    \
  if (f)                                                                     \
    chEvtBroadcastFlags(&stubsEventSource, f);                               \
}
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

typedef uint8_t * ts_params_area_t;
typedef void * ts_service_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Call a service via smc instruction.
 * @details call a given service via smc.
 *
 * @param[in] handle        The handle of the service to invoke.
 *                          The handle is obtained by an invoke to discovery
 *                          service.
 * @param[in,out] svc_data  Service request data, often a reference to a more
 *                          complex structure.
 * @param[in] svc_datalen   Size of the svc_data memory area.
 * @param[in] yieldtime     The time yield to SEC service to run, in microsec.
 *
 * @return                  A 64bit value. It is composed by the 32bit service
 *                          status in the lo-word with the 32bit event mask in
 *                          the hi-word.
 *                          The retval values are returned in the lower word
 *                          as 32bit int.
 * @retval SMC_SVC_OK       generic success value.
 * @retval SMC_SVC_INTR     call interrupted.
 * @retval SMC_SVC_BUSY     the service has a pending request.
 * @retval SMC_SVC_INVALID  bad parameters.
 * @retval SMC_SVC_NOENT    no such service.
 * @retval SMC_SVC_BADH     bad handle.
 *
 * @notapi
 */
static inline int64_t tsInvoke0(ts_service_t handle, ts_params_area_t data,
                       size_t size, sysinterval_t yieldtime) {
  register int64_t result asm("r0");

  register int32_t r0 asm("r0") = (int32_t) handle;
  register int32_t r1 asm("r1") = (int32_t) data;
  register int32_t r2 asm("r2") = (int32_t) size;
  register int32_t r3 asm("r3") = (int32_t) yieldtime;

  __asm volatile ("smc #0" : "=r" (result) : "r" (r0), "r" (r1), "r" (r2),
      "r" (r3) : "memory");
  return result;
}

/**
 * @brief   Call a service via smc instruction.
 * @details call a given service via smc and evaluate the eventflags mask
 *          returned by the secure world. The flags mask is checked by a
 *          macro, TS_CHECK_EVENT_HOOK, supplied by the user.
 *
 * @param[in] handle        The handle of the service to invoke.
 *                          The handle is obtained by an invoke to discovery
 *                          service.
 * @param[in,out] svc_data  Service request data, often a reference to a more
 *                          complex structure.
 * @param[in] svc_datalen   Size of the svc_data memory area.
 * @param[in] yieldtime     The time yield to SEC service to run, in microsec.
 *
 * @return                  The service status. The value depends on the service.
 *
 * @retval SMC_SVC_OK       generic success value.
 * @retval SMC_SVC_INTR     call interrupted.
 * @retval SMC_SVC_BUSY     the service has a pending request.
 * @retval SMC_SVC_INVALID  bad parameters.
 * @retval SMC_SVC_NOENT    no such service.
 * @retval SMC_SVC_BADH     bad handle.
 *
 * @api
 */
static inline msg_t tsInvoke1(ts_service_t handle, ts_params_area_t data,
    size_t size, sysinterval_t yieldtime) {
  int64_t result;
  eventflags_t f;

  result = tsInvoke0(handle, data, size, yieldtime);
  f = (eventflags_t)(result >> 32);
  TS_CHECK_EVENT_HOOK(f);
  return (msg_t)result;
}

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  msg_t tsInvokeService(ts_service_t handle, ts_params_area_t data,
                          size_t size);
  msg_t tsInvokeServiceNoYield(ts_service_t handle, ts_params_area_t data,
                                 size_t size);
  extern event_source_t stubsEventSource;
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* TSCLIENT_H */

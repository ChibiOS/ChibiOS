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
 * @file    chsmc.c
 * @brief   smc call module code.
 *
 * @addtogroup SMC
 * @{
 */
#include <string.h>

#include "ch.h"
#include "chsmc.h"

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
static memory_pool_t svcs_pool;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   XXX Module initialization.
 * @note    This function is implicitly invoked on system initialization,
 *          there is no need to explicitly initialize the module.
 *
 * @notapi
 */
void _smc_init(void) {
  chPoolObjectInit(&svcs_pool, sizeof (smc_service_t),
                   chCoreAllocAlignedI);
}

/**
 * @brief   The trusted service call entry point.
 * @post    A request is passed to the thread registered for the service.
 * @post    The service thread is resumed, if necessary.
 *
 * @param[in] svc_handle  the handle of the service to be invoked
 * @param[in] svc_data    service request data, often a reference to a more complex structure
 *
 * @return              a value defined by the service.
 * @retval MSG_OK       a success value.
 * @retval MSG_RESET    if the service is unavailable.
 *
 * @notapi
 */
msg_t smcEntry(registered_object_t svc_handle, void *svc_data) {
  msg_t r;

  asm("bkpt #0\n\t");
  chDbgAssert(_ns_thread == NULL, "main thread consistency");

  chSysLock();
  r = chThdSuspendS(&_ns_thread);
  _ns_thread = NULL;
  chSysUnlock();
  return r;
}

/**
 * @brief   Register the calling thread as a manager of the given service @p svc_name.
 * @post    The current thread is registered as manager for @p svc_name service.
 *          Only a thread will be the manager of the service @p svc_name.
 *
 * @param[in] svc_name    the name of the service.
 *
 * @return                A new allocated service object.
 * @retval NULL           if @p svc_name failed to be registered.
 *
 * @notapi
 */
smc_service_t *smcRegisterMeAsService(const char *svc_name)
{
  registered_object_t *rop;

  smc_service_t *svcp = chPoolAlloc(&svcs_pool);
  svcp->svct = chThdGetSelfX();
  memset(svcp->params, 0, sizeof svcp->params);
  rop = chFactoryRegisterObject(svc_name, svcp);
  if (rop == NULL) {
    chPoolFree(&svcs_pool, svcp);
    return NULL;
  }
  return svcp;
}

/**
 * @brief   The calling thread is a service and wait the arrival of a request.
 * @post
 *
 * @param[in] svc_name    the name of the service
 * @param[in] svc_number  the number of the service
 *
 * @return                  the
 * @retval SMC_SVC_OK       a success value.
 * @retval SMC_SVC_MANAGED  if the service @p scv_name already exists.
 *
 * @notapi
 */
msg_t smcServiceWaitRequest(smc_service_t *svcp)
{
  msg_t r;

  chDbgCheck(svcp != NULL);

  if (svcp->svct != chThdGetSelfX())
    return MSG_TIMEOUT;
  chSysLock();
  r = chThdSuspendTimeoutS(&svcp->svct, TIME_INFINITE);
  chSysUnlock();
  return r;
}

/** @} */

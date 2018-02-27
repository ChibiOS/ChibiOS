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
thread_reference_t _ns_thread = NULL;

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/
static thread_reference_t main_t;

static smc_service_t svcs_table[SMC_SVC_MAX_N];
static mutex_t svcs_table_mtx;

static uint32_t n_registered_services = 0;
static smc_service_t *discovery_entry = NULL;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static bool isAddrSpaceValid(uint8_t *addr, size_t size)
{
  return (bool)((addr - NSEC_IMAGE_START_ADDR) <
                (NSEC_MEMORY_END - NSEC_IMAGE_START_ADDR)) &&
         (bool)((addr + size - NSEC_IMAGE_START_ADDR) <
                (NSEC_MEMORY_END - NSEC_IMAGE_START_ADDR));
}

static bool isHndlValid(smc_service_t *handle)
{
  if ((handle < &svcs_table[0]) || (handle >= &svcs_table[SMC_SVC_MAX_N]))
    return FALSE;
  if (((char *)handle - (char *)&svcs_table[0]) % sizeof svcs_table[0])
    return FALSE;
  return TRUE;
}

static smc_service_t *getFreeSvcsEntry(void)
{
  int i;
  for (i = 0; i < SMC_SVC_MAX_N; ++i) {
    if (svcs_table[i].svct == NULL)
      return &svcs_table[i];
  }
  return NULL;
}

static smc_service_t *findSvcsEntry(const char *name)
{
  int i;
  for (i = 0; i < SMC_SVC_MAX_N; ++i) {
    if (!strncmp(svcs_table[i].svc_name, name, SMC_SVC_MAX_NAME_LEN))
      return &svcs_table[i];
  }
  return NULL;
}

/*
 * Internal discovery service.
 */
static THD_WORKING_AREA(waDiscoveryTrustService, 512);
static THD_FUNCTION(DiscoveryTrustService, arg) {
  (void) arg;
  msg_t m;
  smc_service_t *svcp;

  discovery_entry = smcRegisterMeAsService("_discovery");
  if ((msg_t)discovery_entry < 0)
    chSysHalt("no entry available for discovery service");
  m = smcServiceWaitRequest(discovery_entry, MSG_OK);
  while (true) {
    chDbgAssert(m == MSG_OK, "");
    if (discovery_entry->svc_datalen) {
      *((char *)discovery_entry->svc_data + discovery_entry->svc_datalen - 1) = '\0';
      chMtxLock(&svcs_table_mtx);
      svcp = findSvcsEntry((char *)discovery_entry->svc_data);
      chMtxUnlock(&svcs_table_mtx);
      m = smcServiceWaitRequest(discovery_entry, (msg_t)svcp);
    }
  }
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   SMC Module initialization.
 *
 * @notapi
 */
void smcInit(void) {
  int i;

  main_t = chThdGetSelfX();
  for (i = 0; i < SMC_SVC_MAX_N; ++i) {
    svcs_table[i].svct = NULL;
    svcs_table[i].register_order = 0;
  }
  chMtxObjectInit(&svcs_table_mtx);
  /*
   * Creates the discovery service thread.
   */
  chThdCreateStatic(waDiscoveryTrustService, sizeof(waDiscoveryTrustService), NORMALPRIO-63,
      DiscoveryTrustService, NULL);
}

/**
 * @brief   The trusted service call entry point.
 * @pre     The foreign interrupts are disabled.
 * @post    A request is passed to the thread registered for the service.
 * @post    The service thread is resumed.
 *
 * @param[in]    svc_handle  the handle of the service to be invoked
 * @param[inout] svc_data    service request data, often a reference to a more complex structure
 * @param[in]    svc_datalen size of the svc_data memory area
 *
 * @return              a value defined by the service.
 * @retval > 0          the handle of requested service.
 * @retval MSG_OK       default success value.
 * @retval MSG_RESET    if the service is unavailable.
 * @retval MSG_TIMEOUT  call interrupted.
 *
 * @api
 */
msg_t smcEntry(smc_service_t *svc_handle, smc_params_area_t svc_data, size_t svc_datalen) {
  smc_service_t *svcp = NULL;
  msg_t r;

  if (svc_handle != SMC_HND_REENTER) {
    if (!isAddrSpaceValid(svc_data, svc_datalen))
      return SMC_SVC_INVALID;
    if (svc_handle == SMC_HND_DISCOVERY) {
      svcp = discovery_entry;
      if (svcp == NULL)
        return SMC_SVC_NOENT;
    } else {
      if (!isHndlValid(svc_handle))
        return SMC_SVC_BADH;
      svcp = svc_handle;
    }
    svcp->svc_data = svc_data;
    svcp->svc_datalen = svc_datalen;
  }

#if (CH_DBG_SYSTEM_STATE_CHECK == TRUE)
  _dbg_check_lock();
#endif

  if (svcp)
    chThdResumeS(&svcp->svct, MSG_OK);
  r = chThdSuspendS(&_ns_thread);

#if (CH_DBG_SYSTEM_STATE_CHECK == TRUE)
  _dbg_check_unlock();
#endif
  return r;
}

/**
 * @brief   Register the calling thread as a manager of the given service @p svc_name.
 * @post    The current thread is registered as manager for @p svc_name service.
 *          Only a thread will be the manager of the service @p svc_name.
 *
 * @param[in] svc_name    the name of the service.
 *
 * @return                a registered smc service object.
 * @retval NULL           if @p svc_name failed to be registered.
 *
 * @api
 */
smc_service_t *smcRegisterMeAsService(const char *svc_name)
{
  smc_service_t *svcp;

  if (n_registered_services == SMC_SVC_MAX_N)
    return (smc_service_t *)SMC_SVC_NHND;
  chMtxLock(&svcs_table_mtx);
  if (findSvcsEntry(svc_name) != NULL) {
    chMtxUnlock(&svcs_table_mtx);
    return (smc_service_t *)SMC_SVC_EXIST;
  }
  svcp = getFreeSvcsEntry();
  svcp->register_order = n_registered_services;
  ++n_registered_services;
  strncpy(svcp->svc_name, svc_name, SMC_SVC_MAX_NAME_LEN);
  chMtxUnlock(&svcs_table_mtx);
  return svcp;
}

/**
 * @brief   The calling thread is a service and wait the arrival of a request.
 * @post    the service object is filled with the parameters of the requestor.
 *
 * @param[in] svcp          the service object reference.
 *
 * @return                  the reason of the awakening
 * @retval MSG_OK           a success value.
 *
 * @api
 */
msg_t smcServiceWaitRequest(smc_service_t *svcp, msg_t msg)
{
  msg_t r;

  chDbgCheck(svcp != NULL);

  chSysLock();
  if (_ns_thread) {
    /* Ack the previous service invocation. Not schedule. */
    chThdResumeI(&_ns_thread, msg);
  }
  chEvtSignalI(main_t, (1 << svcp->register_order));
  r = chThdSuspendTimeoutS(&svcp->svct, TIME_INFINITE);
  chSysUnlock();
  return r;
}

void smcWaitServicesStarted(uint32_t n_services)
{
  eventmask_t mask;

  chDbgAssert(chThdGetSelfX() == main_t, "Only main thread is allowed to call this");
  mask = (1 << (n_services + 1)) - 1;
  chEvtWaitAll(mask);
}

/** @} */

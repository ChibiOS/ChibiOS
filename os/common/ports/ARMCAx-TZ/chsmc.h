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
 * @file    chsmc.h
 * @brief   smc module macros and structures.
 *
 * @addtogroup SMC
 * @{
 */

#ifndef CHSMC_H
#define CHSMC_H

#include "ch.h"
#include "ccportab.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/
/*
 * Service registry errors
 */
#define SMC_SVC_OK                MSG_OK        /* No error */
#define SMC_SVC_INTR              (msg_t)-1     /* Service interrupted */
#define SMC_SVC_NOENT             (msg_t)-2     /* No existent service */
#define SMC_SVC_INVALID           (msg_t)-3     /* Invalid service parameter(s) */
#define SMC_SVC_BADH              (msg_t)-4     /* Invalid service handle */
#define SMC_SVC_EXIST             (msg_t)-5     /* Service already exists */
#define SMC_SVC_NHND              (msg_t)-6     /* No more services */
/*
 * Special service handles
 */
#define SMC_HND_TRAMP             ((smc_service_t *)0)  /* Trampoline service handle */
#define SMC_HND_DISCOVERY         ((smc_service_t *)1)  /* Discovery service handle */
#define SMC_HND_REENTER           ((smc_service_t *)2)  /* Reentering a service service handle */

/*
 * Non secure memory address space
 * (to be redefined in an other place. Makefile?)
 */
#define NSEC_MEMORY_START         ((uint8_t *)0x20000000)
#define NSEC_MEMORY_END           ((uint8_t *)0x20100000)

/*
 * Services table cardinality
 */
#define SMC_SVC_MAX_N             32
#define SMC_SVC_MAX_NAME_LEN      16

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/
typedef uint8_t * smc_params_area_t;

typedef struct ch_smc_service {
  thread_reference_t  svct;
  uint32_t  register_order;
  smc_params_area_t svc_data;
  uint32_t  svc_datalen;
  char      svc_name[SMC_SVC_MAX_NAME_LEN];
} smc_service_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
extern thread_reference_t _ns_thread;
void _ns_trampoline(uint8_t *addr);
void smcInit(void);
void smcWaitServicesStarted(uint32_t n_services);
smc_service_t *smcRegisterMeAsService(const char *svc_name);
msg_t smcServiceWaitRequest(smc_service_t *svcp, msg_t msg);
msg_t smcServiceWaitRequestS(smc_service_t *svcp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* CHSMC_H */

/** @} */

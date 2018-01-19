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
 * @file    chsmc.h
 * @brief   smc Module macros and structures.
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
#define SMC_SVC_OK                MSG_RESET /* No error */
#define SMC_SVC_MANAGED           0x01      /* The service is already managed by a thread */
#define SMC_SVC_THREAD_EXIST      0x02      /* The thread already manages a service */
#define SMC_SVC_THREAD_NOENT      0x03      /* The thread is not in the registry */

/*
 * Special service handles
 */
#define SMC_HND_TRAMP             0
#define SMC_HND_FIND              1

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
  smc_params_area_t svc_data;
  uint32_t  svc_datalen;
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
CC_NO_RETURN void _ns_trampoline(uint32_t addr);
void smcInit(void);
msg_t smc_entry(msg_t svc_number, void *svc_data);
registered_object_t * smcRegisterMeAsService(const char *svc_name);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* CHSMC_H */

/** @} */

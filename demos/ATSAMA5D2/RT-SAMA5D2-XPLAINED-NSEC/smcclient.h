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
 * @file    smcclient.h
 * @brief   smc Module macros and structures.
 *
 * @addtogroup SMC
 * @{
 */

#ifndef SMCCLIENT_H
#define SMCCLIENT_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/
/*
 * Service registry errors
 */
#define SMC_SVC_OK                MSG_OK /* No error */

/*
 * Special service handles
 */
#define SMC_HND_TRAMP             ((smc_service_t)0)
#define SMC_HND_GET               ((smc_service_t)1)

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
typedef void * smc_service_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
msg_t smcInvokeService(smc_service_t handle, smc_params_area_t data,
                       size_t size);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* SMCCLIENT_H */

/** @} */

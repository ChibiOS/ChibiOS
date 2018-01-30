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
 * @file    smcclient.c
 * @brief   SMC client module code.
 *
 * @addtogroup SMC
 * @{
 */

#include "ch.h"
#include "smcclient.h"

msg_t smcInvoke(smc_service_t handle, smc_params_area_t data,
                       size_t size);
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
msg_t smcInvokeService(smc_service_t handle, smc_params_area_t data,
                       size_t size)
{
  msg_t result = MSG_OK;

  result = smcInvoke(handle, data, size);
  while (result == SMC_SVC_INTR)
    result = smcInvoke(SMC_HND_REENTER, 0, 0);
  return result;
}

/** @} */

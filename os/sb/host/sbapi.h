/*
    ChibiOS - Copyright (C) 2006..2019 Giovanni Di Sirio.

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
 * @file    sb/host/sbapi.h
 * @brief   ARMv7-M sandbox host API macros and structures.
 *
 * @addtogroup ARMV7M_SANDBOX_HOSTAPI
 * @{
 */

#ifndef SBAPI_H
#define SBAPI_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if CH_CFG_ST_RESOLUTION != 32
#error "SandBox requires CH_CFG_ST_RESOLUTION == 32"
#endif

#if CH_CFG_INTERVALS_SIZE != 32
#error "SandBox requires CH_CFG_INTERVALS_SIZE == 32"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a syscall handler.
 */
typedef void (*port_syscall_t)(struct port_extctx *ectx);

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void sb_api_exit(struct port_extctx *ctxp);
  void sb_api_get_systime(struct port_extctx *ctxp);
  void sb_api_get_frequency(struct port_extctx *ctxp);
  void sb_api_sleep(struct port_extctx *ctxp);
  void sb_api_sleep_until_windowed(struct port_extctx *ctxp);
  void sb_api_wait_message(struct port_extctx *ctxp);
  void sb_api_reply_message(struct port_extctx *ctxp);
  void sb_api_wait_one_timeout(struct port_extctx *ctxp);
  void sb_api_wait_any_timeout(struct port_extctx *ctxp);
  void sb_api_wait_all_timeout(struct port_extctx *ctxp);
  void sb_api_broadcast_flags(struct port_extctx *ctxp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* SBAPI_H */

/** @} */

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

#define SB_SVC1_HANDLER         sb_api_exit
#define SB_SVC2_HANDLER         sb_api_get_systime
#define SB_SVC3_HANDLER         sb_api_get_frequency
#define SB_SVC4_HANDLER         sb_api_sleep
#define SB_SVC5_HANDLER         sb_api_sleep_until_windowed
#define SB_SVC6_HANDLER         sb_api_wait_message
#define SB_SVC7_HANDLER         sb_api_reply_message
#define SB_SVC8_HANDLER         sb_api_wait_one_timeout
#define SB_SVC9_HANDLER         sb_api_wait_any_timeout
#define SB_SVC10_HANDLER        sb_api_wait_all_timeout
#define SB_SVC11_HANDLER        sb_api_broadcast_flags

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

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  uint32_t sb_api_exit(struct port_extctx *ctxp);
  uint32_t sb_api_get_systime(struct port_extctx *ctxp);
  uint32_t sb_api_get_frequency(struct port_extctx *ctxp);
  uint32_t sb_api_sleep(struct port_extctx *ctxp);
  uint32_t sb_api_sleep_until_windowed(struct port_extctx *ctxp);
  uint32_t sb_api_wait_message(struct port_extctx *ctxp);
  uint32_t sb_api_reply_message(struct port_extctx *ctxp);
  uint32_t sb_api_wait_one_timeout(struct port_extctx *ctxp);
  uint32_t sb_api_wait_any_timeout(struct port_extctx *ctxp);
  uint32_t sb_api_wait_all_timeout(struct port_extctx *ctxp);
  uint32_t sb_api_broadcast_flags(struct port_extctx *ctxp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* SBAPI_H */

/** @} */

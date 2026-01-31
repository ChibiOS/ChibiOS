/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    sb/host/sbapi.h
 * @brief   ARM SandBox host API macros and structures.
 *
 * @addtogroup ARM_SANDBOX_HOSTAPI
 * @{
 */

#ifndef SBAPI_H
#define SBAPI_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Standard API handlers
 * @{
 */
#define SB_SVC1_HANDLER         sb_fastc_get_systime
#define SB_SVC2_HANDLER         sb_fastc_get_frequency

#define SB_SVC129_HANDLER       sb_sysc_exit
#define SB_SVC130_HANDLER       sb_sysc_sleep
#define SB_SVC131_HANDLER       sb_sysc_sleep_until_windowed
#define SB_SVC132_HANDLER       sb_sysc_wait_message
#define SB_SVC133_HANDLER       sb_sysc_reply_message
#define SB_SVC134_HANDLER       sb_sysc_wait_one_timeout
#define SB_SVC135_HANDLER       sb_sysc_wait_any_timeout
#define SB_SVC136_HANDLER       sb_sysc_wait_all_timeout
#define SB_SVC137_HANDLER       sb_sysc_broadcast_flags
#define SB_SVC138_HANDLER       sb_sysc_loadelf
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing a VRQ handling block.
 */
typedef struct sb_apiblock sb_apiblock_t;

/**
 * @brief   Structure representing a VRQ handling block.
 */
struct sb_apiblock {
#if (CH_CFG_USE_MESSAGES == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Thread sending a message to the sandbox.
   */
  thread_t                      *msg_tp;
#endif
#if (CH_CFG_USE_EVENTS == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Sandbox events source.
   */
  event_source_t                es;
#endif
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void sb_fastc_get_systime(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_fastc_get_frequency(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_sysc_exit(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_sysc_sleep(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_sysc_sleep_until_windowed(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_sysc_wait_message(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_sysc_reply_message(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_sysc_wait_one_timeout(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_sysc_wait_any_timeout(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_sysc_wait_all_timeout(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_sysc_broadcast_flags(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_sysc_loadelf(sb_class_t *sbp, struct port_extctx *ectxp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* SBAPI_H */

/** @} */

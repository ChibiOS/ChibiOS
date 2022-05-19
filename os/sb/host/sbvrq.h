/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

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
 * @file    sb/host/sbvrq.h
 * @brief   ARM SandBox host Virtual IRQs macros and structures.
 *
 * @addtogroup ARM_SANDBOX_HOSTAPI
 * @{
 */

#ifndef SBVRQ_H
#define SBVRQ_H

#if (SB_CFG_ENABLE_VRQ == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Virtual ISR register bit definitions
 * @{
 */
#define SB_VRQ_ISR_DISABLED     1U
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

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void sbVRQTriggerS(sb_class_t *sbp, sb_vrqmask_t vmask);
  void sbVRQTriggerFromISR(sb_class_t *sbp, sb_vrqmask_t vmask);
  void sb_api_vrq_wait(struct port_extctx *ectxp);
  void sb_api_vrq_setwt(struct port_extctx *ectxp);
  void sb_api_vrq_clrwt(struct port_extctx *ectxp);
  void sb_api_vrq_seten(struct port_extctx *ectxp);
  void sb_api_vrq_clren(struct port_extctx *ectxp);
  void sb_api_vrq_disable(struct port_extctx *ectxp);
  void sb_api_vrq_enable(struct port_extctx *ectxp);
  void sb_api_vrq_getisr(struct port_extctx *ectxp);
  void sb_api_vrq_return(struct port_extctx *ectxp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* SB_CFG_ENABLE_VRQ == TRUE */

#endif /* SBVRQ_H */

/** @} */

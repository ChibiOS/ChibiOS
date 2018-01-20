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
 * @file    chpreempt.h
 * @brief   Preemption enforcement structures and macros.
 *
 * @addtogroup preemption_enforcement
 * @{
 */

#ifndef CHPREEMPT_H
#define CHPREEMPT_H

/* This header is included from chconf.h and that can be included by asm
   modules.*/
#if !defined(_FROM_ASM_)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Hooks function @p chSchIsPreemptionRequired().
 */
#define CH_SCH_IS_PREEMPTION_REQUIRED_HOOKED

/**
 * @brief   Hooks function @p chSchDoReschedule().
 */
#define CH_SCH_DO_RESCHEDULE_HOOKED

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(CH_CFG_ROUND_ROBIN_QUANTUM)
#error "CH_CFG_ROUND_ROBIN_QUANTUM not defined in chconf.h"
#endif

#if CH_CFG_ROUND_ROBIN_QUANTUM <= 0
#error "CH_CFG_ROUND_ROBIN_QUANTUM must be greater than zero"
#endif

#if CH_CFG_TIME_QUANTUM > 0
#error "CH_CFG_ROUND_ROBIN_QUANTUM is not compatible with CH_CFG_TIME_QUANTUM > 0"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Fields to be added to the @p ch_system_t structure.
 *
 * @notapi
 */
#define CH_PREEMPT_SYSTEM_FIELDS                                            \
  virtual_timer_t       preempt_vt;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void ch_preempt_system_init(void);
  void ch_preempt_thread_switch(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* !defined(_FROM_ASM_) */

#endif /* CHPREEMPT_H */

/** @} */

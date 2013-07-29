/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    chstats.h
 * @brief   Statistics module macros and structures.
 *
 * @addtogroup statistics
 * @{
 */

#ifndef _CHSTATS_H_
#define _CHSTATS_H_

#if CH_DBG_STATISTICS || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

#if !CH_CFG_USE_TM
#error "CH_DBG_STATISTICS requires CH_CFG_USE_TM"
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a kernel statistics structure.
 */
typedef struct {
  ucnt_t                nirq;       /**< @brief IRQ number.
                                         @note  Fast Interrupts are not
                                                accounted for.              */
  ucnt_t                nctxswc;    /**< @brief Context switch number.      */
  time_measurement_t    critical;   /**< @brief Critical zones measurement. */
  time_measurement_t    isr;        /**< @brief ISR measurement.            */
} kernel_stats_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Increases the IRQ counter.
 */
#define _stats_increase_irq() kernel_stats.nirq++

/**
 * @brief   Increases the context switch counter.
 */
#define _stats_increase_ctxswc() kernel_stats.nctxswc++

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern kernel_stats_t kernel_stats;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void _stats_init(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#else /* !CH_DBG_STATISTICS */

/* Stub functions for when the statistics module is disabled. */
#define _stats_increase_irq()
#define _stats_increase_ctxswc()

#endif /* !CH_DBG_STATISTICS */

#endif /* _CHSTATS_H_ */

/** @} */

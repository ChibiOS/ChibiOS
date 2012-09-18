/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    SPC560Pxx/hal_lld.c
 * @brief   SPC560Pxx HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {

  /* The system is switched to the RUN0 mode, the default for normal
     operations.*/
  if (halSPC560PSetRunMode(SPC560P_RUNMODE_RUN0) == CH_FAILED)
    chSysHalt();
}

/**
 * @brief   SPC560Pxx clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h and
 *          @p hal_lld.h
 * @note    This function must be invoked only after the system reset.
 *
 * @special
 */
void spc560p_clock_init(void) {

  /* Waiting for IRC stabilization before attempting anything else.*/
  while (!ME.GS.B.S_RC)
    ;

#if !SPC560P_NO_INIT

#if defined(SPC560P_OSC_BYPASS)
  /* If the board is equipped with an oscillator instead of a xtal then the
     bypass must be activated.*/
  CGM.OSC_CTL.B.OSCBYP = TRUE;
#endif /* SPC560P_ENABLE_XOSC */

  /* Initialization of the FMPLLs settings.*/
  CGM.FMPLL[0].CR.R = SPC560P_FMPLL0_ODF |
                      (SPC560P_FMPLL0_IDF_VALUE << 26) |
                      (SPC560P_FMPLL0_NDIV_VALUE << 16);
  CGM.FMPLL[0].MR.R = 0;                        /* TODO: Add a setting.     */
  CGM.FMPLL[1].CR.R = SPC560P_FMPLL1_ODF |
                      (SPC560P_FMPLL1_IDF_VALUE << 26) |
                      (SPC560P_FMPLL1_NDIV_VALUE << 16);
  CGM.FMPLL[1].MR.R = 0;                        /* TODO: Add a setting.     */

  /* Run modes initialization.*/
  ME.MER.R          = SPC560P_ME_ME_BITS;       /* Enabled run modes.       */
  ME.TEST.R         = SPC560P_ME_TEST_MC_BITS;  /* TEST run mode.           */
  ME.SAFE.R         = SPC560P_ME_SAFE_MC_BITS;  /* SAFE run mode.           */
  ME.DRUN.R         = SPC560P_ME_DRUN_MC_BITS;  /* DRUN run mode.           */
  ME.RUN[0].R       = SPC560P_ME_RUN0_MC_BITS;  /* RUN0 run mode.           */
  ME.RUN[1].R       = SPC560P_ME_RUN1_MC_BITS;  /* RUN0 run mode.           */
  ME.RUN[2].R       = SPC560P_ME_RUN2_MC_BITS;  /* RUN0 run mode.           */
  ME.RUN[3].R       = SPC560P_ME_RUN3_MC_BITS;  /* RUN0 run mode.           */
  ME.HALT0.R        = SPC560P_ME_HALT0_MC_BITS; /* HALT0 run mode.          */
  ME.STOP0.R        = SPC560P_ME_STOP0_MC_BITS; /* STOP0 run mode.          */

  /* Switches again to DRUN mode (current mode) in order to update the
     settings.*/
  if (halSPC560PSetRunMode(SPC560P_RUNMODE_DRUN) == CH_FAILED)
    chSysHalt();

#endif /* !SPC560P_NO_INIT */
}

/**
 * @brief   Switches the system to the specified run mode.
 */
bool_t  halSPC560PSetRunMode(spc560prunmode_t mode) {

  /* Starts a transition process.*/
  ME.MCTL.R = SPC560P_ME_MCTL_MODE(mode) | SPC560P_ME_MCTL_KEY;
  ME.MCTL.R = SPC560P_ME_MCTL_MODE(mode) | SPC560P_ME_MCTL_KEY_INV;

  /* Waits the transition process to start.*/
  while (!ME.GS.B.S_MTRANS)
    ;

  /* Waits the transition process to end.*/
  while (ME.GS.B.S_MTRANS)
    ;

  /* Verifies that the mode has been effectively switched.*/
  if (ME.GS.B.S_CURRENTMODE != mode)
    return TRUE;

  return FALSE;
}

/** @} */

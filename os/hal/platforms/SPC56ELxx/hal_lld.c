/*
 * Licensed under ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file    SPC56ELxx/hal_lld.c
 * @brief   SPC56ELxx HAL subsystem low level driver source.
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
  extern void _vectors(void);

  /* The system is switched to the RUN0 mode, the default for normal
     operations.*/
  if (halSPCSetRunMode(SPC5_RUNMODE_RUN0) == CH_FAILED)
    chSysHalt();

  /* INTC initialization, software vector mode, 4 bytes vectors, starting
     at priority 0.*/
  INTC.MCR.R        = 0;
  INTC.CPR.R        = 0;
  INTC.IACKR.R      = (uint32_t)_vectors;
}

/**
 * @brief   SPC56ELxx early initialization.
 * @note    All the involved constants come from the file @p board.h and
 *          @p hal_lld.h
 * @note    This function must be invoked only after the system reset.
 *
 * @special
 */
void spc_early_init(void) {

  /* Waiting for IRC stabilization before attempting anything else.*/
  while (!ME.GS.B.S_IRCOSC)
    ;

#if !SPC5_NO_INIT

  /* SSCM initialization. Setting up the most restrictive handling of
     invalid accesses to peripherals.*/
  SSCM.ERROR.R = 3;                             /* PAE and RAE bits.        */

  /* Enabling peripheral bridges to allow any operation.*/
  AIPS.MPROT.R      = 0x77777777;
  AIPS.PACR0_7.R    = 0;
  AIPS.PACR8_15.R   = 0;
  AIPS.PACR16_23.R  = 0;
  AIPS.PACR24_31.R  = 0;
  AIPS.OPACR0_7.R   = 0;
  AIPS.OPACR8_15.R  = 0;
  AIPS.OPACR16_23.R = 0;
  AIPS.OPACR24_31.R = 0;
  AIPS.OPACR32_39.R = 0;
  AIPS.OPACR40_47.R = 0;
  AIPS.OPACR48_55.R = 0;
  AIPS.OPACR56_63.R = 0;
  AIPS.OPACR64_71.R = 0;
  AIPS.OPACR72_79.R = 0;
  AIPS.OPACR80_87.R = 0;
  AIPS.OPACR88_95.R = 0;

#if defined(SPC5_OSC_BYPASS)
  /* If the board is equipped with an oscillator instead of a xtal then the
     bypass must be activated.*/
  CGM.OSC_CTL.B.OSCBYP = TRUE;
#endif /* SPC5_OSC_BYPASS */

  /* Initialization of the FMPLLs settings.*/
  CGM.FMPLL[0].CR.R = SPC5_FMPLL0_ODF |
                      ((SPC5_FMPLL0_IDF_VALUE - 1) << 26) |
                      (SPC5_FMPLL0_NDIV_VALUE << 16);
  CGM.FMPLL[0].MR.R = 0;                        /* TODO: Add a setting.     */
  CGM.FMPLL[1].CR.R = SPC5_FMPLL1_ODF |
                      (SPC5_FMPLL1_IDF_VALUE << 26) |
                      (SPC5_FMPLL1_NDIV_VALUE << 16);
  CGM.FMPLL[1].MR.R = 0;                        /* TODO: Add a setting.     */

  /* Run modes initialization.*/
  ME.MER.R          = SPC5_ME_ME_BITS;          /* Enabled run modes.       */
  ME.SAFE.R         = SPC5_ME_SAFE_MC_BITS;     /* SAFE run mode.           */
  ME.DRUN.R         = SPC5_ME_DRUN_MC_BITS;     /* DRUN run mode.           */
  ME.RUN[0].R       = SPC5_ME_RUN0_MC_BITS;     /* RUN0 run mode.           */
  ME.RUN[1].R       = SPC5_ME_RUN1_MC_BITS;     /* RUN1 run mode.           */
  ME.RUN[2].R       = SPC5_ME_RUN2_MC_BITS;     /* RUN2 run mode.           */
  ME.RUN[3].R       = SPC5_ME_RUN3_MC_BITS;     /* RUN0 run mode.           */
  ME.HALT0.R        = SPC5_ME_HALT0_MC_BITS;    /* HALT0 run mode.          */
  ME.STOP0.R        = SPC5_ME_STOP0_MC_BITS;    /* STOP0 run mode.          */

  /* Peripherals run and low power modes initialization.*/
  ME.RUNPC[0].R     = SPC5_ME_RUN_PC0_BITS;
  ME.RUNPC[1].R     = SPC5_ME_RUN_PC1_BITS;
  ME.RUNPC[2].R     = SPC5_ME_RUN_PC2_BITS;
  ME.RUNPC[3].R     = SPC5_ME_RUN_PC3_BITS;
  ME.RUNPC[4].R     = SPC5_ME_RUN_PC4_BITS;
  ME.RUNPC[5].R     = SPC5_ME_RUN_PC5_BITS;
  ME.RUNPC[6].R     = SPC5_ME_RUN_PC6_BITS;
  ME.RUNPC[7].R     = SPC5_ME_RUN_PC7_BITS;
  ME.LPPC[0].R      = SPC5_ME_LP_PC0_BITS;
  ME.LPPC[1].R      = SPC5_ME_LP_PC1_BITS;
  ME.LPPC[2].R      = SPC5_ME_LP_PC2_BITS;
  ME.LPPC[3].R      = SPC5_ME_LP_PC3_BITS;
  ME.LPPC[4].R      = SPC5_ME_LP_PC4_BITS;
  ME.LPPC[5].R      = SPC5_ME_LP_PC5_BITS;
  ME.LPPC[6].R      = SPC5_ME_LP_PC6_BITS;
  ME.LPPC[7].R      = SPC5_ME_LP_PC7_BITS;

  /* Switches again to DRUN mode (current mode) in order to update the
     settings.*/
  if (halSPCSetRunMode(SPC5_RUNMODE_DRUN) == CH_FAILED)
    chSysHalt();

  /* CFLASH settings calculated for a maximum clock of 64MHz.*/
/*  CFLASH.PFCR0.B.BK0_APC  = 2;
  CFLASH.PFCR0.B.BK0_RWSC = 2;
  CFLASH.PFCR1.B.BK1_APC  = 2;
  CFLASH.PFCR1.B.BK1_RWSC = 2;*/

#endif /* !SPC5_NO_INIT */
}

/**
 * @brief   Switches the system to the specified run mode.
 *
 * @param[in] mode      one of the possible run modes
 *
 * @return              The operation status.
 * @retval CH_SUCCESS   if the switch operation has been completed.
 * @retval CH_FAILED    if the switch operation failed.
 */
bool_t halSPCSetRunMode(spc5_runmode_t mode) {

  /* Starts a transition process.*/
  ME.MCTL.R = SPC5_ME_MCTL_MODE(mode) | SPC5_ME_MCTL_KEY;
  ME.MCTL.R = SPC5_ME_MCTL_MODE(mode) | SPC5_ME_MCTL_KEY_INV;

  /* Waits the transition process to start.*/
  while (!ME.GS.B.S_MTRANS)
    ;

  /* Waits the transition process to end.*/
  while (ME.GS.B.S_MTRANS)
    ;

  /* Verifies that the mode has been effectively switched.*/
  if (ME.GS.B.S_CURRENT_MODE != mode)
    return CH_FAILED;

  return CH_SUCCESS;
}

/**
 * @brief   Changes the clock mode of a peripheral.
 *
 * @param[in] n         index of the @p PCTL register
 * @param[in] pctl      new value for the @p PCTL register
 *
 * @notapi
 */
void halSPCSetPeripheralClockMode(uint32_t n, uint32_t pctl) {
  uint32_t mode;

  ME.PCTL[n].R = pctl;
  mode = ME.MCTL.B.TARGET_MODE;
  ME.MCTL.R = SPC5_ME_MCTL_MODE(mode) | SPC5_ME_MCTL_KEY;
  ME.MCTL.R = SPC5_ME_MCTL_MODE(mode) | SPC5_ME_MCTL_KEY_INV;
}

#if !SPC5_NO_INIT || defined(__DOXYGEN__)
/**
 * @brief   Returns the system clock under the current run mode.
 *
 * @return              The system clock in Hertz.
 */
uint32_t halSPCGetSystemClock(void) {
  uint32_t sysclk;

  sysclk = ME.GS.B.S_SYSCLK;
  switch (sysclk) {
  case SPC5_ME_GS_SYSCLK_IRC:
    return SPC5_IRC_CLK;
  case SPC5_ME_GS_SYSCLK_XOSC:
    return SPC5_XOSC_CLK;
  case SPC5_ME_GS_SYSCLK_FMPLL0:
    return SPC5_FMPLL0_CLK;
  default:
    return 0;
  }
}
#endif /* !SPC5_NO_INIT */

/** @} */

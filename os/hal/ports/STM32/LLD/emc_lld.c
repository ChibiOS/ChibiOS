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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    emc_lld.c
 * @brief   EMC Driver subsystem low level driver source template.
 *
 * @addtogroup EMC
 * @{
 */

#include "hal.h"

#if HAL_USE_EMC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   EMC1 driver identifier.
 */
#if STM32_EMC_USE_FSMC1 || defined(__DOXYGEN__)
EMCDriver EMCD1;
#endif

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
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
 * @brief   Low level EMC driver initialization.
 *
 * @notapi
 */
void emc_lld_init(void) {

#if STM32_EMC_USE_FSMC1
  emcObjectInit(&EMCD1);

#if STM32_EMCNAND_USE_EMCNAND1
  EMCD1.nand1 = (FSMC_NAND_TypeDef *)FSMC_Bank2_R_BASE;
#endif

#if STM32_EMCNAND_USE_EMCNAND2
  EMCD1.nand2 = (FSMC_NAND_TypeDef *)FSMC_Bank3_R_BASE;
#endif

#if STM32_USE_EMC_PCCARD
  EMCD1.pccard = (FSMC_PCCARD_TypeDef *)FSMC_Bank4_R_BASE;
#endif

#endif /* STM32_EMC_USE_EMC1 */
}

/**
 * @brief   Configures and activates the EMC peripheral.
 *
 * @param[in] emcp      pointer to the @p EMCDriver object
 *
 * @notapi
 */
void emc_lld_start(EMCDriver *emcp) {

  if (emcp->state == EMC_STOP) {
    /* Enables the peripheral.*/
#if STM32_EMC_USE_FSMC1
    if (&EMCD1 == emcp) {
      rccResetFSMC();
      rccEnableFSMC(FALSE);
  #if STM32_EMC_USE_INT
      nvicEnableVector(FSMC_IRQn,
          CORTEX_PRIORITY_MASK(STM32_EMC_FSMC1_IRQ_PRIORITY));
  #endif /* STM32_EMC_USE_INT */
    }
#endif /* PLATFORM_STM32_USE_EMC1 */

    emcp->state = EMC_READY;
  }
}

/**
 * @brief   Deactivates the EMC peripheral.
 *
 * @param[in] emcp      pointer to the @p EMCDriver object
 *
 * @notapi
 */
void emc_lld_stop(EMCDriver *emcp) {

  if (emcp->state == EMC_READY) {
    /* Resets the peripheral.*/
    rccResetFSMC();

    /* Disables the peripheral.*/
#if STM32_EMC_USE_FSMC1
    if (&EMCD1 == emcp) {
      #if STM32_EMC_USE_INT
      nvicDisableVector(FSMC_IRQn);
      #endif
      rccDisableFSMC(FALSE);
    }
#endif /* PLATFORM_STM32_USE_EMC1 */

    emcp->state = EMC_STOP;
  }
}

#if STM32_EMC_USE_INT
/**
 * @brief   Serve common interrupt.
 *
 * @notapi
 */
void emc_lld_serve_interrupt(void) {
#warning "This functionality untested"

  chDbgPanic("Unrealized");
}

/**
 * @brief   FSMC shared interrupt handler.
 *
 * @notapi
 */
CH_IRQ_HANDLER(FSMC_IRQHandler) {
#warning "This functionality untested"

  CH_IRQ_PROLOGUE();

  if (EMCD1.nand1->SR & FSMC_SR_ISR_MASK){
    EMCNANDD1.isr_handler(&EMCNANDD1, EMCD1.nand1->SR);
  }
  if (EMCD1.nand2->SR & FSMC_SR_ISR_MASK){
    EMCNANDD2.isr_handler(&EMCNANDD2, EMCD1.nand2->SR);
  }

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_EMC_USE_INT */

#endif /* HAL_USE_EMC */

/** @} */

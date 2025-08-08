/*
    ChibiOS/HAL - Copyright (C) 2014 Uladzimir Pylinsky aka barthess

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
/*
   SDRAM routines added by Nick Klimov aka progfin.
 */

/**
 * @file    hal_fsmc_sdram.c
 * @brief   SDRAM Driver subsystem low level driver source.
 *
 * @addtogroup SDRAM
 * @{
 */

#include "hal.h"

#if (defined(STM32F427xx) || defined(STM32F437xx) || \
     defined(STM32F429xx) || defined(STM32F439xx) || \
     defined(STM32F745xx) || defined(STM32F746xx) || \
     defined(STM32F756xx) || defined(STM32F767xx) || \
     defined(STM32F769xx) || defined(STM32F777xx) || \
     defined(STM32F779xx))

#if (HAL_USE_SDRAM == TRUE) || defined(__DOXYGEN__)

#include "hal_sdram_lld.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * FMC_Command_Mode
 */
#define FMCCM_NORMAL              ((uint32_t)0x00000000)
#define FMCCM_CLK_ENABLED         ((uint32_t)0x00000001)
#define FMCCM_PALL                ((uint32_t)0x00000002)
#define FMCCM_AUTO_REFRESH        ((uint32_t)0x00000003)
#define FMCCM_LOAD_MODE           ((uint32_t)0x00000004)
#define FMCCM_SELFREFRESH         ((uint32_t)0x00000005)
#define FMCCM_POWER_DOWN          ((uint32_t)0x00000006)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/
/**
 * @brief   SDRAM driver identifier.
 */
SDRAMDriver SDRAMD1;

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Wait until the SDRAM controller is ready.
 *
 * @notapi
 */
static void sdram_lld_wait_ready(void) {
  /* Wait until the SDRAM controller is ready */
  while (SDRAMD1.sdram->SDSR & FMC_SDSR_BUSY);
}

/**
 * @brief   Executes the SDRAM memory initialization sequence.
 *
 * @param[in] cfgp         pointer to the @p SDRAMConfig object
 *
 * @notapi
 */
static void sdram_lld_init_sequence(const SDRAMConfig *cfgp) {

  uint32_t command_target = 0;

#if STM32_SDRAM_USE_SDRAM1
  command_target |= FMC_SDCMR_CTB1;
#endif
#if STM32_SDRAM_USE_SDRAM2
  command_target |= FMC_SDCMR_CTB2;
#endif

  /* Step 3: Configure a clock configuration enable command.*/
  sdram_lld_wait_ready();
  SDRAMD1.sdram->SDCMR = FMCCM_CLK_ENABLED | command_target;

  /* Step 4: Insert delay (tipically 100uS).*/
  osalSysPolledDelayX(OSAL_US2RTC(STM32_HCLK, 100));

  /* Step 5: Configure a PALL (precharge all) command.*/
  sdram_lld_wait_ready();
  SDRAMD1.sdram->SDCMR = FMCCM_PALL | command_target;

  /* Step 6.1: Configure a Auto-Refresh command: send the first command.*/
  sdram_lld_wait_ready();
  SDRAMD1.sdram->SDCMR = FMCCM_AUTO_REFRESH | command_target |
      (cfgp->sdcmr & FMC_SDCMR_NRFS);

  /* Step 6.2: Send the second command.*/
  sdram_lld_wait_ready();
  SDRAMD1.sdram->SDCMR = FMCCM_AUTO_REFRESH | command_target |
      (cfgp->sdcmr & FMC_SDCMR_NRFS);

  /* Step 7: Program the external memory mode register.*/
  sdram_lld_wait_ready();
  SDRAMD1.sdram->SDCMR = FMCCM_LOAD_MODE | command_target |
      (cfgp->sdcmr & FMC_SDCMR_MRD);

  /* Step 8: Set clock.*/
  sdram_lld_wait_ready();
  SDRAMD1.sdram->SDRTR = cfgp->sdrtr & FMC_SDRTR_COUNT;

  sdram_lld_wait_ready();
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void sdram_lld_start(SDRAMDriver *sdramp, const SDRAMConfig *cfgp)
{
  sdramp->sdram->SDCR1 = cfgp->sdcr;
  sdramp->sdram->SDTR1 = cfgp->sdtr;
  sdramp->sdram->SDCR2 = cfgp->sdcr;
  sdramp->sdram->SDTR2 = cfgp->sdtr;

  sdram_lld_init_sequence(cfgp);
}

void sdram_lld_stop(SDRAMDriver *sdramp) {
  uint32_t command_target = 0;

#if STM32_SDRAM_USE_SDRAM1
  command_target |= FMC_SDCMR_CTB1;
#endif
#if STM32_SDRAM_USE_SDRAM2
  command_target |= FMC_SDCMR_CTB2;
#endif

  sdramp->sdram->SDCMR = FMCCM_POWER_DOWN | command_target;
}
#endif /* STM32_USE_FSMC_SDRAM */

#endif /* STM32F427xx / STM32F429xx / STM32F437xx / STM32F439xx */

/** @} */


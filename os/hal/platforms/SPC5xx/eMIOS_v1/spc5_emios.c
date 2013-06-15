/*
    SPC5 HAL - Copyright (C) 2013 STMicroelectronics

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

/**
 * @file    eMIOS_v1/spc5_emios.c
 * @brief   SPC5xx low level ICU and PWM drivers common code.
 *
 * @addtogroup ICU - PWM
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_ICU || HAL_USE_PWM || defined(__DOXYGEN__)

#include "spc5_emios.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Number of active eMIOSx Channels.
 */
static uint32_t emios0_active_channels;
static uint32_t emios1_active_channels;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

void reset_emios0_active_channels() {
  emios0_active_channels = 0;
}

void reset_emios1_active_channels() {
  emios1_active_channels = 0;
}

uint32_t get_emios0_active_channels() {
  return emios0_active_channels;
}

uint32_t get_emios1_active_channels() {
  return emios1_active_channels;
}

void increase_emios0_active_channels() {
  emios0_active_channels++;
}

void decrease_emios0_active_channels() {
  emios0_active_channels--;
}

void increase_emios1_active_channels() {
  emios1_active_channels++;
}

void decrease_emios1_active_channels() {
  emios1_active_channels--;
}

void active_emios0_clock(ICUDriver *icup, PWMDriver *pwmp) {
  /* If this is the first Channel activated then the eMIOS0 is enabled.*/
  if (emios0_active_channels == 1) {
    halSPCSetPeripheralClockMode(SPC5_EMIOS0_PCTL,
                                 SPC5_EMIOS0_START_PCTL);

    /* Disable all unified channels.*/
    if (icup != NULL) {
      icup->emiosp->MCR.B.GPREN = 0;
      icup->emiosp->MCR.R = EMIOSMCR_GPRE(SPC5_EMIOS0_GLOBAL_PRESCALER);
      icup->emiosp->MCR.R |= EMIOSMCR_GPREN;

      icup->emiosp->MCR.B.GTBE = 1U;

      icup->emiosp->UCDIS.R = 0xFFFFFFFF;

    } else if (pwmp != NULL) {
      pwmp->emiosp->MCR.B.GPREN = 0;
      pwmp->emiosp->MCR.R = EMIOSMCR_GPRE(SPC5_EMIOS0_GLOBAL_PRESCALER);
      pwmp->emiosp->MCR.R |= EMIOSMCR_GPREN;

      pwmp->emiosp->MCR.B.GTBE = 1U;

      pwmp->emiosp->UCDIS.R = 0xFFFFFFFF;

    }

  }
}

void active_emios1_clock(ICUDriver *icup, PWMDriver *pwmp) {
  /* If this is the first Channel activated then the eMIOS1 is enabled.*/
  if (emios1_active_channels == 1) {
    halSPCSetPeripheralClockMode(SPC5_EMIOS1_PCTL,
                                 SPC5_EMIOS1_START_PCTL);

    /* Disable all unified channels.*/
    if (icup != NULL) {
      icup->emiosp->MCR.B.GPREN = 0;
      icup->emiosp->MCR.R = EMIOSMCR_GPRE(SPC5_EMIOS1_GLOBAL_PRESCALER);
      icup->emiosp->MCR.R |= EMIOSMCR_GPREN;

      icup->emiosp->MCR.B.GTBE = 1U;

      icup->emiosp->UCDIS.R = 0xFFFFFFFF;

    } else if (pwmp != NULL) {
      pwmp->emiosp->MCR.B.GPREN = 0;
      pwmp->emiosp->MCR.R = EMIOSMCR_GPRE(SPC5_EMIOS1_GLOBAL_PRESCALER);
      pwmp->emiosp->MCR.R |= EMIOSMCR_GPREN;

      pwmp->emiosp->MCR.B.GTBE = 1U;

      pwmp->emiosp->UCDIS.R = 0xFFFFFFFF;

    }

  }
}

void deactive_emios0_clock(ICUDriver *icup, PWMDriver *pwmp) {
  /* If it is the last active channels then the eMIOS0 is disabled.*/
  if (emios0_active_channels == 0) {
    if (icup != NULL) {
      if (icup->emiosp->UCDIS.R == 0) {
        //icup->emiosp->MCR.B.MDIS = 0;
        halSPCSetPeripheralClockMode(SPC5_EMIOS0_PCTL,
                                     SPC5_EMIOS0_STOP_PCTL);
      }
    } else if (pwmp != NULL) {
      if (pwmp->emiosp->UCDIS.R == 0) {
        //pwmp->emiosp->MCR.B.MDIS = 0;
        halSPCSetPeripheralClockMode(SPC5_EMIOS0_PCTL,
                                     SPC5_EMIOS0_STOP_PCTL);
      }
    }
  }
}

void deactive_emios1_clock(ICUDriver *icup, PWMDriver *pwmp) {
  /* If it is the last active channels then the eMIOS1 is disabled.*/
  if (emios1_active_channels == 0) {
    if (icup != NULL) {
      if (icup->emiosp->UCDIS.R == 0) {
        //icup->emiosp->MCR.B.MDIS = 0;
        halSPCSetPeripheralClockMode(SPC5_EMIOS1_PCTL,
                                     SPC5_EMIOS1_STOP_PCTL);
      }
    } else if (pwmp != NULL) {
      if (pwmp->emiosp->UCDIS.R == 0) {
        //pwmp->emiosp->MCR.B.MDIS = 0;
        halSPCSetPeripheralClockMode(SPC5_EMIOS1_PCTL,
                                     SPC5_EMIOS1_STOP_PCTL);
      }
    }
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/


#endif /* HAL_USE_ICU || HAL_USE_PWM */

/** @} */

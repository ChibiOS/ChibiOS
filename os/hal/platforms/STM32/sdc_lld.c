/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
 * @file    STM32/sdc_lld.c
 * @brief   STM32 SDC subsystem low level driver source.
 *
 * @addtogroup SDC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SDC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief SDCD1 driver identifier.*/
SDCDriver SDCD1;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   SDIO IRQ handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SDIO_IRQHandler) {

  CH_IRQ_PROLOGUE();


  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SDC driver initialization.
 *
 * @notapi
 */
void sdc_lld_init(void) {

  sdcObjectInit(&SDCD1);
}

/**
 * @brief   Configures and activates the SDC peripheral.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_start(SDCDriver *sdcp) {

  if (sdcp->state == SDC_STOP) {
    /* Note, the DMA must be enabled before the IRQs.*/
    dmaAllocate(STM32_DMA2_ID, STM32_DMA_CHANNEL_4, NULL, NULL);
    NVICEnableVector(SDIO_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_SDC_SDIO_IRQ_PRIORITY));
    RCC->AHBENR |= RCC_AHBENR_SDIOEN;
  }
  /* Configuration, card clock is initially stopped.*/
  SDIO->POWER = 0;
  SDIO->CLKCR = 0;
  SDIO->DCTRL = 0;
}

/**
 * @brief   Deactivates the SDC peripheral.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_stop(SDCDriver *sdcp) {

  if ((sdcp->state == SDC_READY) || (sdcp->state == SDC_ACTIVE)) {
    SDIO->POWER = 0;
    SDIO->CLKCR = 0;
    SDIO->DCTRL = 0;

    /* Clock deactivation.*/
    NVICDisableVector(SDIO_IRQn);
    dmaRelease(STM32_DMA2_ID, STM32_DMA_CHANNEL_4);
  }
}

/**
 * @brief   Sends an SDIO command with no response expected.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_send_cmd_none(SDCDriver *sdcp, uint8_t cmd, uint32_t arg) {

}

/**
 * @brief   Sends an SDIO command with a short response expected.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @return              The operation status.
 * @retval FALSE        the operation succeeded.
 * @retval TRUE         the operation failed because timeout, CRC check or
 *                      other errors.
 *
 * @notapi
 */
bool_t sdc_lld_send_cmd_short(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                              uint32_t *resp) {

}

/**
 * @brief   Sends an SDIO command with a long response expected.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @return              The operation status.
 * @retval FALSE        the operation succeeded.
 * @retval TRUE         the operation failed because timeout, CRC check or
 *                      other errors.
 *
 * @notapi
 */
bool_t sdc_lld_send_cmd_long(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                             uint32_t *resp) {

}

#endif /* HAL_USE_SDC */

/** @} */

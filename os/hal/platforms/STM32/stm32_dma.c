/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file stm32_dma.c
 * @brief STM32 DMA helper driver code.
 * @addtogroup STM32_DMA
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

static cnt_t dmacnt1;
#if defined(STM32F10X_HD) || defined (STM32F10X_CL)
static cnt_t dmacnt2;
#endif

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
 * @brief STM32 DMA helper initialization.
 */
void dmaInit(void) {

  dmacnt1 = 0;
#if defined(STM32F10X_HD) || defined (STM32F10X_CL)
  dmacnt2 = 0;
#endif
}

/**
 * @brief Enables the specified DMA controller clock.
 *
 * @param[in] dma the DMA controller id
 */
void dmaEnable(uint32_t dma) {

  switch (dma) {
  case DMA1_ID:
    if (dmacnt1++ == 0) {
      RCC->AHBENR |= RCC_AHBENR_DMA1EN;
      DMA1->IFCR = 0x0FFFFFFF;
    }
    break;
#if defined(STM32F10X_HD) || defined (STM32F10X_CL)
  case DMA2_ID:
    if (dmacnt2++ == 0) {
      RCC->AHBENR |= RCC_AHBENR_DMA2EN;
      DMA2->IFCR = 0x0FFFFFFF;
    }
    break;
#endif
  }
}

/**
 * @brief Disables the specified DMA controller clock.
 *
 * @param[in] dma the DMA controller id
 */
void dmaDisable(uint32_t dma) {

  switch (dma) {
  case DMA1_ID:
    if (--dmacnt1 == 0)
      RCC->AHBENR &= ~RCC_AHBENR_DMA1EN;
    break;
#if defined(STM32F10X_HD) || defined (STM32F10X_CL)
  case DMA2_ID:
    if (--dmacnt2 == 0)
      RCC->AHBENR &= ~RCC_AHBENR_DMA2EN;
    break;
#endif
  }
}

/** @} */

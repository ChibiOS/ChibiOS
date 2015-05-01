/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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
 * @file    STM32/DACv1/dac_lld.c
 * @brief   STM32 DAC subsystem low level driver source.
 *
 * @addtogroup DAC
 * @{
 */

#include "hal.h"

#if HAL_USE_DAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* Because ST headers naming inconsistencies.*/
#if !defined(DAC1)
#define DAC1 DAC
#endif

#define DAC1_CH1_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_DAC1_CH1_DMA_STREAM,                            \
                       STM32_DAC1_CH1_DMA_CHN)

#define DAC1_CH2_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_DAC1_CH2_DMA_STREAM,                            \
                       STM32_DAC1_CH2_DMA_CHN)

#define DAC2_CH1_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_DAC2_CH1_DMA_STREAM,                            \
                       STM32_DAC2_CH1_DMA_CHN)

#define DAC2_CH2_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_DAC2_CH2_DMA_STREAM,                            \
                       STM32_DAC2_CH2_DMA_CHN)

#define CHANNEL_DATA_OFFSET 12

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief DAC1 CH1 driver identifier.*/
#if STM32_DAC_USE_DAC1_CH1 || defined(__DOXYGEN__)
DACDriver DACD1;
#endif

/** @brief DAC1 CH2 driver identifier.*/
#if (STM32_DAC_USE_DAC1_CH2 && !STM32_DAC_DUAL_MODE) || defined(__DOXYGEN__)
DACDriver DACD2;
#endif

/** @brief DAC2 CH1 driver identifier.*/
#if STM32_DAC_USE_DAC2_CH1 || defined(__DOXYGEN__)
DACDriver DACD3;
#endif

/** @brief DAC2 CH2 driver identifier.*/
#if (STM32_DAC_USE_DAC2_CH2 && !STM32_DAC_DUAL_MODE) || defined(__DOXYGEN__)
DACDriver DACD4;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared end/half-of-tx service routine.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void dac_lld_serve_tx_interrupt(DACDriver *dacp, uint32_t flags) {

  if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0) {
    /* DMA errors handling.*/
    _dac_isr_error_code(dacp, DAC_ERR_DMAFAILURE);
  }
  else {
    if ((flags & STM32_DMA_ISR_HTIF) != 0) {
      /* Half transfer processing.*/
      _dac_isr_half_code(dacp);
    }
    if ((flags & STM32_DMA_ISR_TCIF) != 0) {
      /* Transfer complete processing.*/
      _dac_isr_full_code(dacp);
    }
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level DAC driver initialization.
 *
 * @notapi
 */
void dac_lld_init(void) {

#if STM32_DAC_USE_DAC1_CH1
  dacObjectInit(&DACD1);
  DACD1.dac     = DAC1;
  DACD1.dma     = STM32_DMA_STREAM(STM32_DAC1_CH1_DMA_STREAM);
  DACD1.dmamode = STM32_DMA_CR_CHSEL(DAC1_CH1_DMA_CHANNEL) |
                  STM32_DMA_CR_PL(STM32_DAC1_CH1_DMA_PRIORITY) |
                  STM32_DMA_CR_MINC |
                  STM32_DMA_CR_CIRC |
                  STM32_DMA_CR_DIR_M2P |
                  STM32_DMA_CR_DMEIE | STM32_DMA_CR_TEIE |
                  STM32_DMA_CR_HTIE | STM32_DMA_CR_TCIE;
#endif

#if STM32_DAC_USE_DAC1_CH2
  dacObjectInit(&DACD2);
  DACD2.dac     = DAC1;
  DACD2.dma     = STM32_DMA_STREAM(STM32_DAC1_CH2_DMA_STREAM);
  DACD2.dmamode = STM32_DMA_CR_CHSEL(DAC1_CH2_DMA_CHANNEL) |
                  STM32_DMA_CR_PL(STM32_DAC1_CH2_DMA_PRIORITY) |
                  STM32_DMA_CR_MINC |
                  STM32_DMA_CR_CIRC |
                  STM32_DMA_CR_DIR_M2P |
                  STM32_DMA_CR_DMEIE | STM32_DMA_CR_TEIE |
                  STM32_DMA_CR_HTIE | STM32_DMA_CR_TCIE;
#endif

#if STM32_DAC_USE_DAC2_CH1
  dacObjectInit(&DACD3);
  DACD3.dac     = DAC2;
  DACD3.dma     = STM32_DMA_STREAM(STM32_DAC2_CH1_DMA_STREAM);
  DACD3.dmamode = STM32_DMA_CR_CHSEL(DAC2_CH1_DMA_CHANNEL) |
                  STM32_DMA_CR_PL(STM32_DAC2_CH1_DMA_PRIORITY) |
                  STM32_DMA_CR_MINC |
                  STM32_DMA_CR_CIRC |
                  STM32_DMA_CR_DIR_M2P |
                  STM32_DMA_CR_DMEIE | STM32_DMA_CR_TEIE |
                  STM32_DMA_CR_HTIE | STM32_DMA_CR_TCIE;
#endif

#if STM32_DAC_USE_DAC2_CH2
  dacObjectInit(&DACD4);
  DACD4.dac     = DAC2;
  DACD4.dma     = STM32_DMA_STREAM(STM32_DAC2_CH2_DMA_STREAM);
  DACD4.dmamode = STM32_DMA_CR_CHSEL(DAC2_CH2_DMA_CHANNEL) |
                  STM32_DMA_CR_PL(STM32_DAC2_CH2_DMA_PRIORITY) |
                  STM32_DMA_CR_MINC |
                  STM32_DMA_CR_CIRC |
                  STM32_DMA_CR_DIR_M2P |
                  STM32_DMA_CR_DMEIE | STM32_DMA_CR_TEIE |
                  STM32_DMA_CR_HTIE | STM32_DMA_CR_TCIE;
#endif
}

/**
 * @brief   Configures and activates the DAC peripheral.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
void dac_lld_start(DACDriver *dacp) {
  uint32_t cr, regshift, dataoffset;
  bool b;

#if STM32_DAC_USE_DAC1_CH1
    if (&DACD1 == dacp) {
      if (dacp->state == DAC_STOP) {
        b = dmaStreamAllocate(dacp->dma, STM32_DAC1_CH1_IRQ_PRIORITY,
                              (stm32_dmaisr_t)dac_lld_serve_tx_interrupt,
                              (void *)dacp);
        osalDbgAssert(!b, "stream already allocated");

        rccEnableDAC1(false);
      }

      /* Channel-specific parameters.*/
      dataoffset = 0;
      regshift = 0;
   }
#endif

#if STM32_DAC_USE_DAC1_CH2
    if (&DACD2 == dacp) {
      if (dacp->state == DAC_STOP) {
        b = dmaStreamAllocate(dacp->dma, STM32_DAC1_CH2_IRQ_PRIORITY,
                              (stm32_dmaisr_t)dac_lld_serve_tx_interrupt,
                              (void *)dacp);
        osalDbgAssert(!b, "stream already allocated");
        rccEnableDAC1(false);
      }

      /* Channel-specific parameters.*/
      dataoffset = CHANNEL_DATA_OFFSET;
      regshift = 16;
    }
#endif

#if STM32_DAC_USE_DAC2_CH1
    if (&DACD3 == dacp) {
      if (dacp->state == DAC_STOP) {
        b = dmaStreamAllocate(dacp->dma, STM32_DAC2_CH1_IRQ_PRIORITY,
                              (stm32_dmaisr_t)dac_lld_serve_tx_interrupt,
                              (void *)dacp);
        osalDbgAssert(!b, "stream already allocated");
        rccEnableDAC2(false);
      }

      /* Channel-specific parameters.*/
      dataoffset = 0;
      regshift = 0;
    }
#endif

#if STM32_DAC_USE_DAC2_CH2
    if (&DACD3 == dacp) {
      if (dacp->state == DAC_STOP) {
        b = dmaStreamAllocate(dacp->dma, STM32_DAC2_CH2_IRQ_PRIORITY,
                              (stm32_dmaisr_t)dac_lld_serve_tx_interrupt,
                              (void *)dacp);
        osalDbgAssert(!b, "stream already allocated");
        rccEnableDAC2(false);
      }

      /* Channel-specific parameters.*/
      dataoffset = CHANNEL_DATA_OFFSET;
      regshift = 16;
    }
#endif

  /* DAC configuration.*/
#if STM32_DAC_DUAL_MODE == FALSE
  cr = DAC_CR_DMAEN1 | (dacp->config->cr_tsel << 3) |
       DAC_CR_TEN1 | DAC_CR_EN1;
  dacp->dac->CR = (dacp->dac->CR & ~(0x0000FFFF << regshift)) |
                  (cr << regshift);
#else
  /* TODO: Dual.*/
#endif

#if STM32_DAC_DUAL_MODE == FALSE
  switch (dacp->config->dhrm) {
    /* Sets the DAC data register */
    case DAC_DHRM_12BIT_RIGHT:
      dmaStreamSetPeripheral(dacp->dma, &dacp->dac->DHR12R1 + dataoffset);
      dacp->dmamode = (dacp->dmamode & ~STM32_DMA_CR_SIZE_MASK)|
                      STM32_DMA_CR_PSIZE_HWORD | STM32_DMA_CR_MSIZE_HWORD;
      *(&dacp->dac->DHR12R1 + dataoffset) = (uint32_t)dacp->config->sample;
      break;
    case DAC_DHRM_12BIT_LEFT:
      dmaStreamSetPeripheral(dacp->dma, &dacp->dac->DHR12L1 + dataoffset);
      dacp->dmamode = (dacp->dmamode & ~STM32_DMA_CR_SIZE_MASK)|
                      STM32_DMA_CR_PSIZE_HWORD | STM32_DMA_CR_MSIZE_HWORD;
      *(&dacp->dac->DHR12L1 + dataoffset) = (uint32_t)dacp->config->sample;
      break;
    case DAC_DHRM_8BIT_RIGHT:
      dmaStreamSetPeripheral(dacp->dma, &dacp->dac->DHR8R1 + dataoffset);
      dacp->dmamode = (dacp->dmamode & ~STM32_DMA_CR_SIZE_MASK)|
                      STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MSIZE_BYTE;
      *(&dacp->dac->DHR8R1 + dataoffset) = (uint32_t)dacp->config->sample;
      break;
    default:
      chDbgAssert(false, "unexpected DAC mode");
      break;
  }
#else
#if defined(STM32_HAS_DAC_CHN2) && STM32_HAS_DAC_CHN2
      case DAC_DHRM_12BIT_RIGHT_DUAL:
        dmaStreamSetPeripheral(dacp->dma, &dacp->dac->DHR12RD);
        dacp->dmamode = (dacp->dmamode & ~STM32_DMA_CR_SIZE_MASK) |
              STM32_DMA_CR_PSIZE_HWORD | STM32_DMA_CR_MSIZE_HWORD;
        break;
      case DAC_DHRM_12BIT_LEFT_DUAL:
        dmaStreamSetPeripheral(dacp->dma, &dacp->dac->DHR12LD);
        dacp->dmamode = (dacp->dmamode & ~STM32_DMA_CR_SIZE_MASK) |
              STM32_DMA_CR_PSIZE_HWORD | STM32_DMA_CR_MSIZE_HWORD;
        break;
      case DAC_DHRM_8BIT_RIGHT_DUAL:
        dmaStreamSetPeripheral(dacp->dma, &dacp->dac->DHR8RD);
        dacp->dmamode = (dacp->dmamode & ~STM32_DMA_CR_SIZE_MASK) |
             STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MSIZE_BYTE;
        break;
#endif
#endif
}

/**
 * @brief   Deactivates the DAC peripheral.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
void dac_lld_stop(DACDriver *dacp) {

  /* If in ready state then disables the DAC clock.*/
  if (dacp->state == DAC_READY) {

    /* DMA channel released.*/
    dmaStreamRelease(dacp->dma);

#if STM32_DAC_USE_DAC1_CH1
    if (&DACD1 == dacp) {
      dacp->dac->CR &= ~DAC_CR_EN1;

      if ((dacp->dac->CR & DAC_CR_EN2) == 0U) {
        rccDisableDAC1(false);
      }
    }
#endif

#if STM32_DAC_USE_DAC1_CH2
    if (&DACD2 == dacp) {
      dacp->dac->CR &= ~DAC_CR_EN2;

      if ((dacp->dac->CR & DAC_CR_EN1) == 0U) {
        rccDisableDAC1(false);
      }
    }
#endif
  }
}

/**
 * @brief   Starts a DAC conversion.
 * @details Starts an asynchronous conversion operation.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
void dac_lld_start_conversion(DACDriver *dacp) {

  dmaStreamSetMemory0(dacp->dma, dacp->samples);
  dmaStreamSetTransactionSize(dacp->dma, dacp->depth);
  dmaStreamSetMode(dacp->dma, dacp->dmamode |
                              STM32_DMA_CR_DMEIE | STM32_DMA_CR_TEIE |
                              STM32_DMA_CR_HTIE  | STM32_DMA_CR_TCIE);
  dmaStreamEnable(dacp->dma);
}


/**
 * @brief   Stops an ongoing conversion.
 * @details This function stops the currently ongoing conversion and returns
 *          the driver in the @p DAC_READY state. If there was no conversion
 *          being processed then the function does nothing.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @iclass
 */
void dac_lld_stop_conversion(DACDriver *dacp) {

  dmaStreamDisable(dacp->dma);
}

#endif /* HAL_USE_DAC */

/** @} */

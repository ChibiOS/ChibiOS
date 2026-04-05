/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio

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
 * @file    ADCv1/hal_adc_lld.c
 * @brief   RP2040/RP2350 ADC subsystem low level driver source.
 *
 * @addtogroup ADC
 * @{
 */

#include "hal.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @name    PAD control register bits
 * @{
 */
#define PADS_GPIO_IE                        (1U << 6)
#define PADS_GPIO_OD                        (1U << 7)
#define PADS_GPIO_PUE                       (1U << 3)
#define PADS_GPIO_PDE                       (1U << 2)
/** @} */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief ADC1 driver identifier.*/
#if RP_ADC_USE_ADC1 || defined(__DOXYGEN__)
ADCDriver ADCD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Drains the ADC FIFO.
 * @note    Waits for any in-progress conversion to complete before draining.
 *
 * @param[in] adc       pointer to the ADC registers block
 */
static void adc_lld_drain_fifo(ADC_TypeDef *adc) {

  /* Wait for any in-progress conversion to complete.*/
  while ((adc->CS & ADC_CS_READY) == 0U) {
  }

  /* Drain all samples from FIFO.*/
  while ((adc->FCS & ADC_FCS_EMPTY) == 0U) {
    (void)adc->FIFO;
  }
}

/**
 * @brief   ADC DMA service routine.
 *
 * @param[in] p         parameter for the registered function
 * @param[in] ct        content of the CTRL_TRIG register
 */
static void adc_lld_serve_dma_interrupt(void *p, uint32_t ct) {
  ADCDriver *adcp = (ADCDriver *)p;

  /* DMA errors handling.*/
  if ((ct & (DMA_CTRL_TRIG_READ_ERROR | DMA_CTRL_TRIG_WRITE_ERROR)) != 0U) {
    _adc_isr_error_code(adcp, ADC_ERR_DMAFAILURE);
  }
  else {
    /* Conversion group may have been reset by error handler.*/
    if (adcp->grpp != NULL) {
      /* Checking for DMA transfer complete.*/
      if ((ct & DMA_CTRL_TRIG_BUSY) == 0U) {
        /* Check for ADC-level errors during transfer.*/
        adcerror_t emask = 0U;
        if ((adcp->adc->FCS & ADC_FCS_OVER) != 0U) {
          emask |= ADC_ERR_OVERFLOW;
        }
        if ((adcp->adc->CS & ADC_CS_ERR_STICKY) != 0U) {
          emask |= ADC_ERR_CONVERSION;
        }
        if (emask != 0U) {
          _adc_isr_error_code(adcp, emask);
        }
        else if (adcp->grpp->circular) {
          /* Circular mode: re-arm DMA before callback.*/
          const ADCConversionGroup *grpp = adcp->grpp;
          size_t total = (size_t)grpp->num_channels * adcp->depth;
          adcsample_t *dest;
          size_t count;
          bool notify_half = false;

          if (adcp->depth > 1U) {
            /* Double-buffer mode with half-transfer notifications.*/
            size_t half_count = total / 2U;
            if (!adcp->half) {
              /* First half complete, re-arm DMA for second half.*/
              adcp->half = true;
              dest = adcp->samples + half_count;
              count = total - half_count;
              notify_half = true;
            }
            else {
              /* Second half complete, re-arm DMA for first half.*/
              adcp->half = false;
              dest = adcp->samples;
              count = half_count;
            }
          }
          else {
            /* depth == 1: no half-buffer, just re-arm full buffer.*/
            dest = adcp->samples;
            count = total;
          }

          dmaChannelSetDestinationX(adcp->dma, (uint32_t)dest);
          dmaChannelSetCounterX(adcp->dma, (uint32_t)count);
          dmaChannelSetModeX(adcp->dma, adcp->dmamode);
          dmaChannelEnableX(adcp->dma);

          if (notify_half) {
            _adc_isr_half_code(adcp);
          }
          else {
            _adc_isr_full_code(adcp);
          }
        }
        else {
          /* Linear mode: transfer complete.*/
          _adc_isr_full_code(adcp);
        }
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

/**
 * @brief   Low level ADC driver initialization.
 *
 * @notapi
 */
void adc_lld_init(void) {

#if RP_ADC_USE_ADC1
  /* Driver initialization.*/
  adcObjectInit(&ADCD1);
  ADCD1.adc     = ADC;
  ADCD1.dma     = NULL;
  ADCD1.dmamode = DMA_CTRL_TRIG_DATA_SIZE_HWORD |
                  DMA_CTRL_TRIG_INCR_WRITE      |
                  DMA_CTRL_TRIG_TREQ_ADC        |
#if RP_ADC_ADC1_DMA_PRIORITY > 0
                  DMA_CTRL_TRIG_HIGH_PRIORITY   |
#endif
                  0U;
  ADCD1.half    = false;
#endif
}

/**
 * @brief   Configures and activates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_start(ADCDriver *adcp) {

  /* If in stopped state then enables the ADC and DMA clocks.*/
  if (adcp->state == ADC_STOP) {
#if RP_ADC_USE_ADC1
    if (&ADCD1 == adcp) {
      /* Allocate DMA channel.*/
      adcp->dma = dmaChannelAllocI(RP_DMA_CHANNEL_ID_ANY,
                                  RP_ADC_ADC1_DMA_IRQ_PRIORITY,
                                  adc_lld_serve_dma_interrupt,
                                  (void *)adcp);
      osalDbgAssert(adcp->dma != NULL, "unable to allocate DMA channel");

      if (adcp->dma == NULL) {
        return;
      }

      /* Reset ADC peripheral.*/
      rp_peripheral_reset(RESETS_ALLREG_ADC);
      rp_peripheral_unreset(RESETS_ALLREG_ADC);

      /* Enable ADC.*/
      adcp->adc->CS = ADC_CS_EN;

      /* Wait for ADC ready.*/
      while ((adcp->adc->CS & ADC_CS_READY) == 0U) {
      }

      /* Clear any pending FIFO errors.*/
      adcp->adc->FCS = ADC_FCS_UNDER | ADC_FCS_OVER;

      /* Drain any samples in FIFO.*/
      adc_lld_drain_fifo(adcp->adc);

      /* Set DMA source to ADC FIFO (constant across conversions).*/
      dmaChannelSetSourceX(adcp->dma, (uint32_t)&adcp->adc->FIFO);
    }
#endif /* RP_ADC_USE_ADC1 */
  }
}

/**
 * @brief   Deactivates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop(ADCDriver *adcp) {

  /* If in ready state then disables the ADC peripheral and clock.*/
  if (adcp->state == ADC_READY) {
#if RP_ADC_USE_ADC1
    if (&ADCD1 == adcp) {
      /* Stop conversions. NOTE: CLR alias may also clear W1C ERR_STICKY
         as a side-effect; acceptable during shutdown.*/
      adcp->adc->CLR.CS = ADC_CS_START_MANY | ADC_CS_TS_EN;

      /* Disable FIFO and DMA.*/
      adcp->adc->FCS = 0U;

      /* Drain FIFO.*/
      adc_lld_drain_fifo(adcp->adc);

      /* Disable ADC.*/
      adcp->adc->CS = 0U;

      /* Release DMA channel.*/
      dmaChannelFreeI(adcp->dma);
      adcp->dma = NULL;

      /* Hold ADC in reset to gate the peripheral clock.*/
      rp_peripheral_reset(RESETS_ALLREG_ADC);
    }
#endif
  }
}

/**
 * @brief   Starts an ADC conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_start_conversion(ADCDriver *adcp) {
  uint32_t cs_val;
  uint32_t fcs_val;
  uint32_t mode;
  uint32_t dma_count;
  const ADCConversionGroup *grpp = adcp->grpp;

  /* Clear any previous errors (SET alias writes 1 to W1C bits).*/
  adcp->adc->SET.CS = ADC_CS_ERR_STICKY;
  adcp->adc->FCS = ADC_FCS_UNDER | ADC_FCS_OVER;

  /* Drain FIFO.*/
  adc_lld_drain_fifo(adcp->adc);

  /* Build CS register value.*/
  cs_val = ADC_CS_EN |
           ((grpp->channel << ADC_CS_AINSEL_POS) & ADC_CS_AINSEL_MASK);

  /* Enable temperature sensor if needed.*/
  if (grpp->ts_enabled) {
    cs_val |= ADC_CS_TS_EN;
  }

  /* Configure round-robin if multiple channels.*/
  if (grpp->rrobin != 0U) {
    cs_val |= (grpp->rrobin << ADC_CS_RROBIN_POS) & RP_ADC_RROBIN_MASK;
  }

  /* Write CS without START_MANY first. AINSEL must be stable when
     START_MANY transitions 0->1 to set the round-robin starting channel.*/
  adcp->adc->CS = cs_val;

  /* Configure clock divisor.*/
  adcp->adc->DIV = grpp->div;

  /* Configure FIFO: enable, DMA request, threshold = 1.*/
  fcs_val = ADC_FCS_EN | ADC_FCS_DREQ_EN | (1U << ADC_FCS_THRESH_POS);
  adcp->adc->FCS = fcs_val;

  /* DMA setup.*/
  mode = adcp->dmamode;
  adcp->half = false;

  /* Circular with depth > 1: DMA first half, ISR re-arms second half.
     Otherwise: DMA full buffer.*/
  if (grpp->circular && (adcp->depth > 1U)) {
    dma_count = ((uint32_t)grpp->num_channels * (uint32_t)adcp->depth) / 2U;
  }
  else {
    dma_count = (uint32_t)grpp->num_channels * (uint32_t)adcp->depth;
  }

  /* Configure DMA transfer (source address set once in adc_lld_start).*/
  dmaChannelSetDestinationX(adcp->dma, (uint32_t)adcp->samples);
  dmaChannelSetCounterX(adcp->dma, dma_count);
  dmaChannelSetModeX(adcp->dma, mode);

  /* Enable DMA channel interrupt.*/
  dmaChannelEnableInterruptX(adcp->dma);

  /* Enable DMA channel.*/
  dmaChannelEnableX(adcp->dma);

  /* Wait for ADC ready before starting conversion.*/
  while ((adcp->adc->CS & ADC_CS_READY) == 0U) {
  }

  /* Start continuous conversion (second step: add START_MANY).*/
  adcp->adc->CS = cs_val | ADC_CS_START_MANY;
}

/**
 * @brief   Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop_conversion(ADCDriver *adcp) {

  /* Stop conversions. Using CLR alias for atomic bit clear.*/
  adcp->adc->CLR.CS = ADC_CS_START_MANY;

  /* Disable DMA channel.*/
  dmaChannelDisableX(adcp->dma);
  dmaChannelDisableInterruptX(adcp->dma);

  /* Disable FIFO.*/
  adcp->adc->FCS = 0U;

  /* Drain FIFO.*/
  adc_lld_drain_fifo(adcp->adc);
}

/**
 * @brief   Enables the temperature sensor.
 * @note    Must be called after @p adcStart().
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adcRPEnableTS(ADCDriver *adcp) {

  adcp->adc->SET.CS = ADC_CS_TS_EN;
}

/**
 * @brief   Disables the temperature sensor.
 * @note    Must be called after @p adcStart().
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adcRPDisableTS(ADCDriver *adcp) {

  adcp->adc->CLR.CS = ADC_CS_TS_EN;
}

/**
 * @brief   Configures a GPIO pin for ADC input.
 * @note    Disables digital I/O and pulls for proper analog operation.
 *
 * @param[in] gpio      GPIO pin number (26-29 RP2040, 26-29/40-47 RP2350)
 *
 * @api
 */
void adcRPGpioInit(uint32_t gpio) {
  uint32_t padbits;

  /* Validate GPIO range. NUM_CHANNELS includes the temperature sensor
     which has no GPIO pin, hence the -1.*/
  osalDbgCheck(gpio >= RP_ADC_BASE_PIN);
  osalDbgCheck(gpio < (RP_ADC_BASE_PIN + RP_ADC_NUM_CHANNELS - 1U));

  /* FUNCSEL = NULL (31): disconnect digital output driver.*/
  IO_BANK0->GPIO[gpio].CTRL = 31U;

  /* Disable pulls and digital input, enable output disable.*/
  padbits = PADS_BANK0->GPIO[gpio];
  padbits &= ~(PADS_GPIO_PUE | PADS_GPIO_PDE | PADS_GPIO_IE);
  padbits |= PADS_GPIO_OD;
  PADS_BANK0->GPIO[gpio] = padbits;
}

#endif /* HAL_USE_ADC */

/** @} */

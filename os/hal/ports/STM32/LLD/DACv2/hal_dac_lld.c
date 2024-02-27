/*
    ChibiOS - Copyright (C) 2006..2024 Giovanni Di Sirio

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
 * @file    DACv2/hal_dac_lld.c
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

#define CHANNEL_DATA_OFFSET           (12U / 4U)
#define CHANNEL_REGISTER_SHIFT        16U
#define CHANNEL_REGISTER_MASK1        0xFFFF0000U
#define CHANNEL_REGISTER_MASK2        0x0000FFFFU

#define BYTE_SINGLE_SAMPLE_MULTIPLIER 1U
#define HALF_SINGLE_SAMPLE_MULTIPLIER 2U
#define HALF_DUAL_SAMPLE_MULTIPLIER   2U
#define WORD_DUAL_SAMPLE_MULTIPLIER   4U

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/* Fix ST headers naming inconsistencies.*/
#if !defined(DAC1)
#define DAC1 DAC
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief DAC1 CH1 driver identifier.*/
#if STM32_DAC_USE_DAC1_CH1 || defined(__DOXYGEN__)
DACDriver DACD1;
#endif

/** @brief DAC1 CH2 driver identifier.*/
#if STM32_DAC_USE_DAC1_CH2 || defined(__DOXYGEN__)
#if !STM32_DAC_DUAL_MODE
DACDriver DACD2;
#else
#error "DAC1 CH2 not independently available in dual mode"
#endif
#endif

/** @brief DAC2 CH1 driver identifier.*/
#if STM32_DAC_USE_DAC2_CH1 || defined(__DOXYGEN__)
DACDriver DACD3;
#endif

/** @brief DAC2 CH2 driver identifier.*/
#if STM32_DAC_USE_DAC2_CH2 || defined(__DOXYGEN__)
#if !STM32_DAC_DUAL_MODE
DACDriver DACD4;
#else
#error "DAC2 CH2 not independently available in dual mode"
#endif
#endif

/** @brief DAC3 CH1 driver identifier.*/
#if STM32_DAC_USE_DAC3_CH1 || defined(__DOXYGEN__)
DACDriver DACD5;
#endif

/** @brief DAC3 CH2 driver identifier.*/
#if STM32_DAC_USE_DAC3_CH2 || defined(__DOXYGEN__)
#if !STM32_DAC_DUAL_MODE
DACDriver DACD6;
#else
#error "DAC3 CH2 not independently available in dual mode"
#endif
#endif

/** @brief DAC4 CH1 driver identifier.*/
#if STM32_DAC_USE_DAC4_CH1 || defined(__DOXYGEN__)
DACDriver DACD7;
#endif

/** @brief DAC4 CH2 driver identifier.*/
#if STM32_DAC_USE_DAC4_CH2 || defined(__DOXYGEN__)
#if !STM32_DAC_DUAL_MODE
DACDriver DACD8;
#else
#error "DAC4 CH2 not independently available in dual mode"
#endif
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

#if STM32_DAC_USE_DAC1_CH1 == TRUE
static const dacparams_t dac1_ch1_params = {
  .dac          = DAC1,
  .dataoffset   = 0U,
  .regshift     = 0U,
  .regmask      = CHANNEL_REGISTER_MASK1,
  .dmach        = STM32_DAC_DAC1_CH1_GPDMA_CHANNEL,
  .dmaprio      = STM32_DAC_DAC1_CH1_DMA_PRIORITY,
  .dmareq       = STM32_GPDMA_REQ_DAC1_CH1,
  .dmairqprio   = STM32_DAC_DAC1_CH1_DMA_IRQ_PRIORITY
};
#endif

#if STM32_DAC_USE_DAC1_CH2 == TRUE
static const dacparams_t dac1_ch2_params = {
  .dac          = DAC1,
  .dataoffset   = CHANNEL_DATA_OFFSET,
  .regshift     = CHANNEL_REGISTER_SHIFT,
  .regmask      = CHANNEL_REGISTER_MASK2,
  .dmach        = STM32_DAC_DAC1_CH2_GPDMA_CHANNEL,
  .dmaprio      = STM32_DAC_DAC1_CH2_DMA_PRIORITY,
  .dmareq       = STM32_GPDMA_REQ_DAC1_CH2,
  .dmairqprio   = STM32_DAC_DAC1_CH2_DMA_IRQ_PRIORITY
};
#endif

#if STM32_DAC_USE_DAC2_CH1 == TRUE
static const dacparams_t dac2_ch1_params = {
  .dac          = DAC2,
  .dataoffset   = 0U,
  .regshift     = 0U,
  .regmask      = CHANNEL_REGISTER_MASK1,
  .dmach        = STM32_DAC_DAC2_CH1_GPDMA_CHANNEL,
  .dmaprio      = STM32_DAC_DAC2_CH1_DMA_PRIORITY,
  .dmareq       = STM32_GPDMA_REQ_DAC2_CH1,
  .dmairqprio   = STM32_DAC_DAC2_CH1_DMA_IRQ_PRIORITY
};
#endif

#if STM32_DAC_USE_DAC2_CH2 == TRUE
static const dacparams_t dac2_ch2_params = {
  .dac          = DAC2,
  .dataoffset   = CHANNEL_DATA_OFFSET,
  .regshift     = CHANNEL_REGISTER_SHIFT,
  .regmask      = CHANNEL_REGISTER_MASK2,
  .dmach        = STM32_DAC_DAC2_CH2_GPDMA_CHANNEL,
  .dmaprio      = STM32_DAC_DAC2_CH2_DMA_PRIORITY,
  .dmareq       = STM32_GPDMA_REQ_DAC2_CH2,
  .dmairqprio   = STM32_DAC_DAC2_CH2_DMA_IRQ_PRIORITY
};
#endif

#if STM32_DAC_USE_DAC3_CH1 == TRUE
static const dacparams_t dac3_ch1_params = {
  .dac          = DAC3,
  .dataoffset   = 0U,
  .regshift     = 0U,
  .regmask      = CHANNEL_REGISTER_MASK1,
  .dmach        = STM32_DAC_DAC3_CH1_GPDMA_CHANNEL,
  .dmaprio      = STM32_DAC_DAC3_CH1_DMA_PRIORITY,
  .dmareq       = STM32_GPDMA_REQ_DAC3_CH1,
  .dmairqprio   = STM32_DAC_DAC3_CH1_DMA_IRQ_PRIORITY
};
#endif

#if STM32_DAC_USE_DAC3_CH2 == TRUE
static const dacparams_t dac3_ch2_params = {
  .dac          = DAC3,
  .dataoffset   = CHANNEL_DATA_OFFSET,
  .regshift     = CHANNEL_REGISTER_SHIFT,
  .regmask      = CHANNEL_REGISTER_MASK2,
  .dmach        = STM32_DAC_DAC3_CH2_GPDMA_CHANNEL,
  .dmaprio      = STM32_DAC_DAC3_CH2_DMA_PRIORITY,
  .dmareq       = STM32_GPDMA_REQ_DAC3_CH2,
  .dmairqprio   = STM32_DAC_DAC3_CH2_DMA_IRQ_PRIORITY
};
#endif

#if STM32_DAC_USE_DAC4_CH1 == TRUE
static const dacparams_t dac4_ch1_params = {
  .dac          = DAC4,
  .dataoffset   = 0U,
  .regshift     = 0U,
  .regmask      = CHANNEL_REGISTER_MASK1,
  .dmach        = STM32_DAC_DAC4_CH1_GPDMA_CHANNEL,
  .dmaprio      = STM32_DAC_DAC4_CH1_DMA_PRIORITY,
  .dmareq       = STM32_GPDMA_REQ_DAC4_CH1,
  .dmairqprio   = STM32_DAC_DAC4_CH1_DMA_IRQ_PRIORITY
};
#endif

#if STM32_DAC_USE_DAC4_CH2 == TRUE
static const dacparams_t dac4_ch2_params = {
  .dac          = DAC4,
  .dataoffset   = CHANNEL_DATA_OFFSET,
  .regshift     = CHANNEL_REGISTER_SHIFT,
  .regmask      = CHANNEL_REGISTER_MASK2,
  .dmach        = STM32_DAC_DAC4_CH2_GPDMA_CHANNEL,
  .dmaprio      = STM32_DAC_DAC4_CH2_DMA_PRIORITY,
  .dmareq       = STM32_GPDMA_REQ_DAC4_CH2,
  .dmairqprio   = STM32_DAC_DAC4_CH2_DMA_IRQ_PRIORITY
};
#endif

/* DMA circular link control.*/

#if STM32_DAC_USE_DAC1_CH1 || defined(__DOXYGEN__)
static dac_dmabuf_t __gpdma_dac1_ch1;
#endif

#if STM32_DAC_USE_DAC1_CH2 || defined(__DOXYGEN__)
static dac_dmabuf_t __gpdma_dac1_ch2;
#endif

#if STM32_DAC_USE_DAC2_CH1 || defined(__DOXYGEN__)
static dac_dmabuf_t __gpdma_dac2_ch1;
#endif

#if STM32_DAC_USE_DAC2_CH2 || defined(__DOXYGEN__)
static dac_dmabuf_t __gpdma_dac2_ch2;
#endif

#if STM32_DAC_USE_DAC3_CH1 || defined(__DOXYGEN__)
static dac_dmabuf_t __gpdma_dac3_ch1;
#endif

#if STM32_DAC_USE_DAC3_CH2 || defined(__DOXYGEN__)
static dac_dmabuf_t __gpdma_dac3_ch2;
#endif

#if STM32_DAC_USE_DAC4_CH1 || defined(__DOXYGEN__)
static dac_dmabuf_t __gpdma_dac4_ch1;
#endif

#if STM32_DAC_USE_DAC4_CH2 || defined(__DOXYGEN__)
static dac_dmabuf_t __gpdma_dac4_ch2;
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared DAC GPDMA service routine.
 *
 * @param[in] p         parameter for the registered function
 * @param[in] flags     content of the CxSR register
 */
static void dac_lld_serve_dma_interrupt(void *p, uint32_t flags) {
  DACDriver *dacp = (DACDriver *)p;

  if ((flags & STM32_GPDMA_CSR_ERRORS) != 0) {
    /* DMA errors handling.*/
    _dac_isr_error_code(dacp, DAC_ERR_DMAFAILURE);
  }
  else {
    /* It is possible that the conversion group has already been reset by a
       DAC error handler. In this case this interrupt is spurious.*/
    if (dacp->grpp != NULL) {
      if ((flags & STM32_GPDMA_CSR_HTF) != 0) {
        /* Half transfer processing.*/
        _dac_isr_half_code(dacp);
      }
      if ((flags & STM32_GPDMA_CSR_TCF) != 0) {
        /* Transfer complete processing.*/
        _dac_isr_full_code(dacp);
      }
    }
  }
}

/**
 * @brief   DAC IRQ service routine.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] isr       content of the ISR register
 */
static void dac_lld_serve_interrupt(DACDriver *dacp, uint32_t isr) {

    /* Check for DMA underrun.*/
    if ((isr & (DAC_SR_DMAUDR1 | DAC_SR_DMAUDR2)) &&
                                (dacp->state == DAC_ACTIVE)) {
      /* DAC DMA underrun condition. This can happen only if the DMA is
         unable to read data fast enough.*/
      dacp->params->dac->SR = (DAC_SR_DMAUDR1 << dacp->params->regshift);
      _dac_isr_error_code(dacp, DAC_ERR_UNDERFLOW);
    }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_DAC_USE_DAC1_CH1 || STM32_DAC_USE_DAC1_CH2 || defined(__DOXYGEN__)
/**
 * @brief   DAC1 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DAC1_HANDLER) {
  uint32_t isr;

  OSAL_IRQ_PROLOGUE();

  isr  = DAC1->SR;
  DAC1->SR = isr;
#if defined(STM32_DAC_DAC1_IRQ_HOOK)
  STM32_DAC_DAC1_IRQ_HOOK
#endif
  if ((isr & DAC_SR_DMAUDR1) != 0) {
    dac_lld_serve_interrupt(&DACD1, DAC_SR_DMAUDR1);
  }
#if !STM32_DAC_DUAL_MODE && STM32_DAC_USE_DAC1_CH2
  if ((isr & DAC_SR_DMAUDR2) != 0) {
    dac_lld_serve_interrupt(&DACD2, DAC_SR_DMAUDR2);
  }
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif /* STM32_DAC_USE_DAC1_CH1 || STM32_DAC_USE_DAC1_CH2 */

#if STM32_DAC_USE_DAC2_CH1 || STM32_DAC_USE_DAC2_CH2 || defined(__DOXYGEN__)
/**
 * @brief   DAC2 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DAC2_HANDLER) {
  uint32_t isr;

  OSAL_IRQ_PROLOGUE();

  isr  = DAC2->SR;
  DAC2->SR = isr;
#if defined(STM32_DAC_DAC2_IRQ_HOOK)
  STM32_DAC_DAC2_IRQ_HOOK
#endif
  if ((isr & DAC_SR_DMAUDR1) != 0) {
    dac_lld_serve_interrupt(&DACD3, DAC_SR_DMAUDR1);
  }
#if !STM32_DAC_DUAL_MODE && STM32_DAC_USE_DAC2_CH2
  if ((isr & DAC_SR_DMAUDR2) != 0) {
    dac_lld_serve_interrupt(&DACD4, DAC_SR_DMAUDR2);
  }
#endif
  OSAL_IRQ_EPILOGUE();
}
#endif /* STM32_DAC_USE_DAC2_CH1 || STM32_DAC_USE_DAC2_CH2  */

#if STM32_DAC_USE_DAC3_CH1 || STM32_DAC_USE_DAC3_CH2 || defined(__DOXYGEN__)
/**
 * @brief   DAC3 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DAC3_HANDLER) {
  uint32_t isr;

  OSAL_IRQ_PROLOGUE();

  isr  = DAC3->SR;
  DAC3->SR = isr;
#if defined(STM32_DAC_DAC3_IRQ_HOOK)
  STM32_DAC_DAC3_IRQ_HOOK
#endif
  if ((isr & DAC_SR_DMAUDR1) != 0) {
    dac_lld_serve_interrupt(&DACD6, DAC_SR_DMAUDR1);
  }
#if !STM32_DAC_DUAL_MODE && STM32_DAC_USE_DAC3_CH2
  if ((isr & DAC_SR_DMAUDR2) != 0) {
    dac_lld_serve_interrupt(&DACD7, DAC_SR_DMAUDR2);
  }
#endif
  OSAL_IRQ_EPILOGUE();
}
#endif /* STM32_DAC_USE_DAC3_CH1 || STM32_DAC_USE_DAC3_CH2  */

#if STM32_DAC_USE_DAC4_CH1 || STM32_DAC_USE_DAC4_CH2 || defined(__DOXYGEN__)
/**
 * @brief   DAC4 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DAC4_HANDLER) {
  uint32_t isr;

  OSAL_IRQ_PROLOGUE();

  isr  = DAC4->SR;
  DAC4->SR = isr;
#if defined(STM32_DAC_DAC4_IRQ_HOOK)
  STM32_DAC_DAC4_IRQ_HOOK
#endif
  if ((isr & DAC_SR_DMAUDR1) != 0) {
    dac_lld_serve_interrupt(&DACD8, DAC_SR_DMAUDR1);
  }
#if !STM32_DAC_DUAL_MODE && STM32_DAC_USE_DAC4_CH2
  if ((isr & DAC_SR_DMAUDR2) != 0) {
    dac_lld_serve_interrupt(&DACD9, DAC_SR_DMAUDR2);
  }
#endif
  OSAL_IRQ_EPILOGUE();
}
#endif /* STM32_DAC_USE_DAC4_CH1 || STM32_DAC_USE_DAC4_CH2  */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level DAC driver initialization.
 *
 * @notapi
 */
void dac_lld_init(void) {

  /* Initialise driver fields.*/
#if STM32_DAC_USE_DAC1_CH1
  dacObjectInit(&DACD1);
  DACD1.params  = &dac1_ch1_params;
  DACD1.dmachp = NULL;
  DACD1.dbuf    = &__gpdma_dac1_ch1;
#endif

#if STM32_DAC_USE_DAC1_CH2
  dacObjectInit(&DACD2);
  DACD2.params  = &dac1_ch2_params;
  DACD2.dmachp = NULL;
  DACD2.dbuf    = &__gpdma_dac1_ch2;
#endif

#if STM32_DAC_USE_DAC2_CH1
  dacObjectInit(&DACD3);
  DACD3.params  = &dac2_ch1_params;
  DACD3.dmachp = NULL;
  DACD2.dbuf    = &__gpdma_dac2_ch1;
#endif

#if STM32_DAC_USE_DAC2_CH2
  dacObjectInit(&DACD4);
  DACD4.params  = &dac2_ch2_params;
  DACD4.dmachp = NULL;
  DACD2.dbuf    = &__gpdma_dac2_ch2;
#endif

#if STM32_DAC_USE_DAC3_CH1
  dacObjectInit(&DACD5);
  DACD5.params  = &dac3_ch1_params;
  DACD5.dmachp = NULL;
  DACD2.dbuf    = &__gpdma_dac3_ch1;
#endif

#if STM32_DAC_USE_DAC3_CH2
  dacObjectInit(&DACD6);
  DACD6.params  = &dac3_ch2_params;
  DACD6.dmachp = NULL;
  DACD2.dbuf    = &__gpdma_dac3_ch2;
#endif

#if STM32_DAC_USE_DAC4_CH1
  dacObjectInit(&DACD7);
  DACD7.params  = &dac4_ch1_params;
  DACD7.dmachp = NULL;
  DACD2.dbuf    = &__gpdma_dac4_ch1;
#endif

#if STM32_DAC_USE_DAC4_CH2
  dacObjectInit(&DACD8);
  DACD8.params  = &dac4_ch2_params;
  DACD8.dmachp = NULL;
  DACD2.dbuf    = &__gpdma_dac4_ch2;
#endif


  /* Reset DAC H/W then setup IRQs. The IRQs are used to capture DMA underrun
     errors only.*/
#if STM32_DAC_USE_DAC1_CH1 || STM32_DAC_USE_DAC1_CH2
  rccResetDAC1();
  rccEnableDAC1(true);
  rccDisableDAC1();
  nvicEnableVector(STM32_DAC1_NUMBER, STM32_DAC_DAC1_IRQ_PRIORITY);
#endif

#if STM32_DAC_USE_DAC2_CH1 || STM32_DAC_USE_DAC2_CH2
  rccResetDAC2();
  rccEnableDAC2(true);
  rccDisableDAC2();
  nvicEnableVector(STM32_DAC2_NUMBER, STM32_DAC_DAC2_IRQ_PRIORITY);
#endif

#if STM32_DAC_USE_DAC3_CH1 || STM32_DAC_USE_DAC3_CH2
  rccResetDAC3();
  rccEnableDAC3(true);
  rccDisableDAC3();
  nvicEnableVector(STM32_DAC3_NUMBER, STM32_DAC_DAC3_IRQ_PRIORITY);
#endif

#if STM32_DAC_USE_DAC4_CH1 || STM32_DAC_USE_DAC4_CH2
  rccResetDAC4();
  rccEnableDAC4(true);
  rccDisableDAC4();
  nvicEnableVector(STM32_DAC4_NUMBER, STM32_DAC_DAC4_IRQ_PRIORITY);
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

  /* If the driver is in DAC_STOP state then a full initialization is
     required.*/
  if (dacp->state == DAC_STOP) {
    dacchannel_t channel = 0;

    /* Enable DAC clock. DMA channel allocation is deferred to conversion
       start and only allocated if a group conversion is used.*/

    if (false) {
    }
#if STM32_DAC_USE_DAC1_CH1
    else if (&DACD1 == dacp) {
      rccEnableDAC1(true);
    }
#endif

#if STM32_DAC_USE_DAC1_CH2
    else if (&DACD2 == dacp) {
      rccEnableDAC1(true);
      channel = 1;
    }
#endif

#if STM32_DAC_USE_DAC2_CH1
    else if (&DACD3 == dacp) {
      rccEnableDAC2(true);
    }
#endif

#if STM32_DAC_USE_DAC2_CH2
    else if (&DACD4 == dacp) {
      rccEnableDAC2(true);
      channel = 1;
    }
#endif

#if STM32_DAC_USE_DAC3_CH1
    else if (&DACD5 == dacp) {
      rccEnableDAC3(true);
    }
#endif

#if STM32_DAC_USE_DAC3_CH2
    else if (&DACD6 == dacp) {
      rccEnableDAC3(true);
      channel = 1;
    }
#endif

#if STM32_DAC_USE_DAC4_CH1
    else if (&DACD7 == dacp) {
      rccEnableDAC4(true);
    }
#endif

#if STM32_DAC_USE_DAC4_CH2
    else if (&DACD8 == dacp) {
      rccEnableDAC4(true);
      channel = 1;
    }
#endif

    else {
      osalDbgAssert(false, "invalid DAC instance");
    }

    /* Enabling DAC in SW triggering mode initially, initializing data to
       configuration default.*/
    uint32_t reg;

#if STM32_DAC_DUAL_MODE == FALSE
    /* Operating in SINGLE mode. Setup registers for specified channel from
       configuration. Lower half word of configuration specifies configuration
       for either channel 1 or 2.*/
#if STM32_DAC_HAS_MCR == TRUE
    reg = dacp->params->dac->MCR & dacp->params->regmask;
#if defined(STM32H5XX)
    /* Handle HFSEL setting based on DAC clock.*/
    if (STM32_ADCDACCLK > 160000000) {
      reg |= DAC_MCR_HFSEL_1;
    }
    else if (STM32_ADCDACCLK > 80000000) {
      reg |= DAC_MCR_HFSEL_0;
    }
#endif
    dacp->params->dac->MCR = reg |
      ((dacp->config->mcr & ~dacp->params->regmask) << dacp->params->regshift);
#endif
    /* Enable and initialise the channel.*/
    reg = dacp->params->dac->CR;
    reg &= dacp->params->regmask;
    reg |= (DAC_CR_EN1 | dacp->config->cr) << dacp->params->regshift;
    dacp->params->dac->CR = reg;
    dac_lld_put_channel(dacp, channel, (dacsample_t)dacp->config->init);
#else /* STM32_DAC_DUAL_MODE != FALSE */
    /* Operating in DUAL mode with two channels to setup. Set registers for
       both channels from configuration. Lower and upper half words specify
       configuration for channels 1 & 2 respectively.*/
    (void)channel;
#if STM32_DAC_HAS_MCR == TRUE
#if defined(STM32H5XX)
    /* Handle HFSEL setting based on DAC clock.*/
    reg = dacp->params->dac->MCR;
    if (STM32_ADCDACCLK > 80000000) {
      reg |= DAC_MCR_HFSEL_0;
    }
    else if (STM32_ADCDACCLK > 160000000) {
      reg |= DAC_MCR_HFSEL_1;
    }
    dacp->params->dac->MCR = reg;
#endif
#endif
    /* Enable and initialise both channels 1 and 2. Mask out DMA enable.*/
    reg = dacp->config->cr;
    reg &= ~(DAC_CR_DMAEN1 | DAC_CR_DMAEN2);
    dacp->params->dac->CR = DAC_CR_EN2 | DAC_CR_EN1 | reg;
    dac_lld_put_channel(dacp, 0U, (dacsample_t)dacp->config->init);
    dac_lld_put_channel(dacp, 1U, (dacsample_t)(dacp->config->init >>
                                               (sizeof(dacsample_t) * 8)));
#endif /* STM32_DAC_DUAL_MODE == FALSE */
  }
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

    /* Disabling DAC conditionally.*/
    dacp->params->dac->CR &= dacp->params->regmask;

#if STM32_DAC_USE_DAC1_CH1
    if (&DACD1 == dacp) {
#if defined(DAC_CR_EN2)
      if ((dacp->params->dac->CR & DAC_CR_EN2) != 0U) {
        return;
      }
      rccDisableDAC1();
#endif
      return;
    }
#endif

#if STM32_DAC_USE_DAC1_CH2
    if (&DACD2 == dacp) {
      if ((dacp->params->dac->CR & DAC_CR_EN1) == 0U) {
        rccDisableDAC1();
      }
      return;
    }
#endif

#if STM32_DAC_USE_DAC2_CH1
    if (&DACD3 == dacp) {
#if defined(DAC_CR_EN2)
      if ((dacp->params->dac->CR & DAC_CR_EN2) != 0U) {
        return;
      }
      rccDisableDAC2();
#endif
      return;
    }
#endif

#if STM32_DAC_USE_DAC2_CH2
    if (&DACD4 == dacp) {
      if ((dacp->params->dac->CR & DAC_CR_EN1) == 0U) {
        rccDisableDAC2();
      }
      return;
    }
#endif

#if STM32_DAC_USE_DAC3_CH1
    if (&DACD5 == dacp) {
#if defined(DAC_CR_EN2)
      if ((dacp->params->dac->CR & DAC_CR_EN2) != 0U) {
        return;
      }
      rccDisableDAC3();
#endif
      return;
    }
#endif

#if STM32_DAC_USE_DAC3_CH2
    if (&DACD6 == dacp) {
      if ((dacp->params->dac->CR & DAC_CR_EN1) == 0U) {
        rccDisableDAC3();
      }
      return;
    }
#endif

#if STM32_DAC_USE_DAC4_CH1
    if (&DACD7 == dacp) {
#if defined(DAC_CR_EN2)
      if ((dacp->params->dac->CR & DAC_CR_EN2) != 0U) {
        return;
      }
      rccDisableDAC4();
#endif
      return;
    }
#endif

#if STM32_DAC_USE_DAC4_CH2
    if (&DACD8 == dacp) {
      if ((dacp->params->dac->CR & DAC_CR_EN1) == 0U) {
        rccDisableDAC4();
      }
      return;
    }
#endif
  }
}

/**
 * @brief   Outputs a value directly on a DAC channel.
 * @note    While a group is active in DUAL mode on CH1 only then CH2
 *          is available for normal output (put) operations.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] channel   DAC channel number
 * @param[in] sample    value to be output
 *
 * @api
 */
void dac_lld_put_channel(DACDriver *dacp,
                         dacchannel_t channel,
                         dacsample_t sample) {

#if STM32_DAC_DUAL_MODE
  if (dacp->grpp != NULL) {
    osalDbgAssert(dacp->grpp->num_channels == 1 && channel == 1,
                                          "channel busy");
    return;
  }
#endif /* STM32_DAC_DUAL_MODE */

#if defined(DAC_SR_DAC1RDY)
  /* Wait for DAC ready.*/
  while ((dacp->params->dac->SR & (DAC_SR_DAC1RDY << dacp->params->regshift)) == 0);
#endif
  switch (dacp->config->datamode) {
  case DAC_DHRM_12BIT_RIGHT:
#if STM32_DAC_DUAL_MODE
  case DAC_DHRM_12BIT_RIGHT_DUAL:
#endif
    if (channel == 0U) {
#if STM32_DAC_DUAL_MODE
      dacp->params->dac->DHR12R1 = sample;
#else
      *(&dacp->params->dac->DHR12R1 + dacp->params->dataoffset) = sample;
#endif
    }
#if (STM32_HAS_DAC1_CH2 || STM32_HAS_DAC2_CH2 ||                            \
     STM32_HAS_DAC3_CH2 || STM32_HAS_DAC4_CH2)
    else {
      dacp->params->dac->DHR12R2 = sample;
    }
#endif
    break;
  case DAC_DHRM_12BIT_LEFT:
#if STM32_DAC_DUAL_MODE
  case DAC_DHRM_12BIT_LEFT_DUAL:
#endif
    if (channel == 0U) {
#if STM32_DAC_DUAL_MODE
      dacp->params->dac->DHR12L1 = sample;
#else
      *(&dacp->params->dac->DHR12L1 + dacp->params->dataoffset) = sample;
#endif
    }
#if (STM32_HAS_DAC1_CH2 || STM32_HAS_DAC2_CH2 ||                            \
     STM32_HAS_DAC3_CH2 || STM32_HAS_DAC4_CH2)
    else {
      dacp->params->dac->DHR12L2 = sample;
    }
#endif
    break;
  case DAC_DHRM_8BIT_RIGHT:
#if STM32_DAC_DUAL_MODE
  case DAC_DHRM_8BIT_RIGHT_DUAL:
#endif
    if (channel == 0U) {
#if STM32_DAC_DUAL_MODE
      dacp->params->dac->DHR8R1 = sample;
#else
      *(&dacp->params->dac->DHR8R1 + dacp->params->dataoffset) = (uint8_t)sample;
#endif
    }
#if (STM32_HAS_DAC1_CH2 || STM32_HAS_DAC2_CH2 ||                            \
     STM32_HAS_DAC3_CH2 || STM32_HAS_DAC4_CH2)
    else {
      dacp->params->dac->DHR8R2 = (uint8_t)sample;
    }
#endif
    break;
  default:
    osalDbgAssert(false, "unexpected DAC mode");
    break;
  }
}

/**
 * @brief   Starts a DAC conversion.
 * @details Starts an asynchronous conversion operation.
 * @note    In @p DAC_DHRM_8BIT_RIGHT mode two samples are packed in a single
 *          dacsample_t element. DMA does byte to byte transfer.
 * @note    In @p DAC_DHRM_8BIT_RIGHT_DUAL mode two samples are treated
 *          as a single 16 bits sample and packed into a single dacsample_t
 *          element. The num_channels must be set to one in the group
 *          conversion configuration structure. DMA does half to half transfer.
 *          translation.
 * @note    If using DUAL mode with a single channel conversion then channel 2
 *          is enabled for manual (put_channel) for non DMA triggered use. The
 *          the data format for put operations is specified in the upper half
 *          word of the 'datamode' field. The CR setting is in the upper half
 *          word of the 'cr' field of the configuration.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
void dac_lld_start_conversion(DACDriver *dacp) {
  uint32_t n, ni, cr, dmamode, dmaccr, dmallr;
  uint8_t *si;

  /* Determine channel for setting initial value.*/
  dacchannel_t channel = dacp->params->regshift == 0 ? 0U : 1U;

  /* Allocate DMA channel.*/
  dacp->dmachp = gpdmaChannelAllocI(dacp->params->dmach,
                                    dacp->params->dmaprio,
                                    dac_lld_serve_dma_interrupt,
                                    (void *)dacp);
  osalDbgAssert(dacp->dmachp != NULL, "unable to allocate DMA channel");

  /* DMA settings depend on the chosen DAC mode. If not dual mode then each
     channel of a DAC operates independently. The DAC DMA update request of DHR
     happens after the DAC does DHR to DOR transfer. Thus the first sample must
     be loaded into DHR prior to conversion start. The initial DMA count and
     source address must be adjusted accordingly for first DMA cycle and then
     restored from then on using DMA LLR.*/
  switch (dacp->config->datamode) {

  /* Sets the DAC data register */
  case DAC_DHRM_12BIT_RIGHT:
    osalDbgAssert(dacp->grpp->num_channels == 1, "invalid number of channels");

    gpdmaChannelSetDestination(dacp->dmachp, &dacp->params->dac->DHR12R1 +
                                      dacp->params->dataoffset);
    dmamode = (STM32_GPDMA_CTR1_DDW_HALF | STM32_GPDMA_CTR1_SDW_HALF);

    /* Set initial value of channel.*/
    dac_lld_put_channel(dacp, channel, *dacp->samples);

    /* One channel where data is a 16 bit (dacsample_t). GPDMA count is 2 bytes
       per transfer. Adjust count and source address for first cycle.*/
    n = dacp->depth * HALF_SINGLE_SAMPLE_MULTIPLIER;

    /* A depth of one just repeats one sample.*/
    if (dacp->depth == 1) {
      ni = n;
      si = (uint8_t *)dacp->samples;
    }
    else {
      ni = n - HALF_SINGLE_SAMPLE_MULTIPLIER;
      si = (uint8_t *)dacp->samples + HALF_SINGLE_SAMPLE_MULTIPLIER;
    }
    break;

  case DAC_DHRM_12BIT_LEFT:
    osalDbgAssert(dacp->grpp->num_channels == 1, "invalid number of channels");

    gpdmaChannelSetDestination(dacp->dmachp, &dacp->params->dac->DHR12L1 +
                                      dacp->params->dataoffset);
    dmamode = (STM32_GPDMA_CTR1_DDW_HALF | STM32_GPDMA_CTR1_SDW_HALF);

    /* Set initial value of channel.*/
    dac_lld_put_channel(dacp, channel, *dacp->samples);

    /* One channel where data is a 16 bit (dacsample_t). GPDMA count is 2 bytes
       per transfer. Adjust count and source address for first cycle.*/
    n = dacp->depth * HALF_SINGLE_SAMPLE_MULTIPLIER;

    /* A depth of one just repeats one sample.*/
    if (dacp->depth == 1) {
      ni = n;
      si = (uint8_t *)dacp->samples;
    }
    else {
      ni = n - HALF_SINGLE_SAMPLE_MULTIPLIER;
      si = (uint8_t *)dacp->samples + HALF_SINGLE_SAMPLE_MULTIPLIER;
    }
    break;

  case DAC_DHRM_8BIT_RIGHT:
    osalDbgAssert(dacp->grpp->num_channels == 1, "invalid number of channels");

    gpdmaChannelSetDestination(dacp->dmachp, &dacp->params->dac->DHR8R1 +
                                      dacp->params->dataoffset);
    dmamode = (STM32_GPDMA_CTR1_DDW_BYTE | STM32_GPDMA_CTR1_SDW_BYTE);

    /* Set initial value of channel.*/
    dac_lld_put_channel(dacp, channel, *dacp->samples &0xFF);

    /* One channel where data is in bytes. GPDMA count is 1 byte per transfer.
       Adjust count and source address for first cycle.*/
    n = dacp->depth * BYTE_SINGLE_SAMPLE_MULTIPLIER;

    /* A depth of one just repeats one sample.*/
    if (dacp->depth == 1) {
      ni = n;
      si = (uint8_t *)dacp->samples;
    }
    else {
      ni = n - BYTE_SINGLE_SAMPLE_MULTIPLIER;
      si = (uint8_t *)dacp->samples + BYTE_SINGLE_SAMPLE_MULTIPLIER;
    }
    break;

#if STM32_DAC_DUAL_MODE == TRUE
  case DAC_DHRM_12BIT_RIGHT_DUAL:
    osalDbgAssert(dacp->grpp->num_channels == 2, "invalid number of channels");

    gpdmaChannelSetDestination(dacp->dmachp, &dacp->params->dac->DHR12RD);
    dmamode = (STM32_GPDMA_CTR1_DDW_WORD | STM32_GPDMA_CTR1_SDW_WORD);

    dac_lld_put_channel(dacp, 0U, *dacp->samples);
    dac_lld_put_channel(dacp, 1U, *(dacp->samples + 2);
    /* Two channels as 2 x dacsample_t in a word. GPDMA count is 4 bytes per
       transfer.*/
    n = dacp->depth * WORD_DUAL_SAMPLE_MULTIPLIER;

    /* TODO: Set ni and si.*/
    break;

  case DAC_DHRM_12BIT_LEFT_DUAL:
    osalDbgAssert(dacp->grpp->num_channels == 2, "invalid number of channels");

    gpdmaChannelSetDestination(dacp->dmachp, &dacp->params->dac->DHR12LD);
    dmamode = (STM32_GPDMA_CTR1_DDW_WORD | STM32_GPDMA_CTR1_SDW_WORD);

    dac_lld_put_channel(dacp, 0U, *dacp->samples);
    dac_lld_put_channel(dacp, 1U, *(dacp->samples + 2);
    /* Two channels as 2 x dacsample_t per word. GPDMA count is 4 bytes per
       transfer.*/
    n = dacp->depth * WORD_DUAL_SAMPLE_MULTIPLIER;

    /* TODO: Set ni and si.*/
    break;

  case DAC_DHRM_8BIT_RIGHT_DUAL:
    osalDbgAssert(dacp->grpp->num_channels == 1, "invalid number of channels");

    gpdmaChannelSetDestination(dacp->dmachp, &dacp->params->dac->DHR8RD);
    dmamode = (STM32_GPDMA_CTR1_DDW_HALF | STM32_GPDMA_CTR1_SDW_HALF);

    dac_lld_put_channel(dacp, 0U, *dacp->samples) & 0xFF;
    dac_lld_put_channel(dacp, 1U, *(dacp->samples) >> 8;
    /* Two channels packed as two bytes in a single dacsample_t. GPDMA count
       is 2 bytes per transfer.*/
    n = dacp->depth * HALF_DUAL_SAMPLE_MULTIPLIER;

    /* TODO: Set ni and si.*/
    break;

#endif
  default:
    osalDbgAssert(false, "unexpected DAC mode");
    return;
  }

  osalDbgAssert(n <= STM32_GPDMA_MAX_TRANSFER,
                                "unsupported GPDMA transfer size");

  /* Setup control registers values.*/
  dmaccr = STM32_GPDMA_CCR_PRIO((uint32_t)dacp->params->dmaprio) |
           STM32_GPDMA_CCR_LAP_MEM                               |
           STM32_GPDMA_CCR_TOIE                                  |
           STM32_GPDMA_CCR_USEIE                                 |
           STM32_GPDMA_CCR_ULEIE                                 |
           STM32_GPDMA_CCR_DTEIE                                 |
           STM32_GPDMA_CCR_TCIE;

  /* DAC uses a circular operation. Use the GPDMA linking mechanism to reload
     source pointer and count for subsequent cycles.*/
  dmallr = STM32_GPDMA_CLLR_USA | STM32_GPDMA_CLLR_UB1 |
              (((uint32_t)&dacp->dbuf->csar) & 0xFFFFU);
  dacp->dbuf->cb1r = n;
  dacp->dbuf->csar = (uint32_t)dacp->samples;


  if (n > 1U) {
    /* If circular buffer depth > 1, then the half transfer interrupt
       is enabled in order to allow streaming processing.*/
    dmaccr |= STM32_GPDMA_CCR_HTIE;
  }

  /* Configure and enable DMA controller with initial transfer settings.*/
  gpdmaChannelSetSource(dacp->dmachp, si);
  gpdmaChannelSetTransactionSize(dacp->dmachp, ni);
  gpdmaChannelSetMode(dacp->dmachp,
                      dmaccr,
                      (dmamode                                        |
                       STM32_GPDMA_CTR1_SAP_MEM                       |
                       STM32_GPDMA_CTR1_SINC                          |
                       STM32_GPDMA_CTR1_DAP_PER),
                      (STM32_GPDMA_CTR2_REQSEL(dacp->params->dmareq)  |
                       STM32_GPDMA_CTR2_DREQ),
                       dmallr);

  gpdmaChannelEnable(dacp->dmachp);

  /* DAC configuration.*/
  cr = dacp->params->dac->CR;
#if STM32_DAC_DUAL_MODE == FALSE
  cr &= dacp->params->regmask;

  /* Enable DMA and trigger on the specified channel. Clear underrun status.*/
  dacp->params->dac->SR = (DAC_SR_DMAUDR1 << dacp->params->regshift);
  cr |= (DAC_CR_DMAEN1 | (dacp->grpp->trigger << DAC_CR_TSEL1_Pos) |
         DAC_CR_TEN1 | DAC_CR_EN1 | dacp->config->cr) << dacp->params->regshift;
#else
  /* Enable DMA to trigger on CH1. Clear underrun status.*/
  dacp->params->dac->SR = DAC_SR_DMAUDR1;
  cr = DAC_CR_DMAEN1 | (dacp->grpp->trigger << DAC_CR_TSEL1_Pos) |
       DAC_CR_TEN1 | DAC_CR_EN1 | dacp->config->cr;
#endif

  /* Start continuous conversion.*/
  dacp->params->dac->CR = cr;

}

/**
 * @brief   Stops an ongoing conversion.
 * @details This function stops the currently ongoing conversion. The
 *          configuration is restored to start condition. The DOR values
 *          are not updated.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @iclass
 */
void dac_lld_stop_conversion(DACDriver *dacp) {
  uint32_t cr;

  /* DMA channel disabled and released.*/
  gpdmaChannelDisable(dacp->dmachp);
  gpdmaChannelFreeI(dacp->dmachp);
  dacp->dmachp = NULL;

  /* Restore start configuration but leave DORx at current values.*/
  cr = dacp->params->dac->CR;
#if STM32_DAC_DUAL_MODE == FALSE
#if STM32_DAC_HAS_MCR == TRUE
  uint32_t mcr;
  mcr = dacp->params->dac->MCR & dacp->params->regmask;
  dacp->params->dac->MCR = mcr |
    ((dacp->config->mcr & dacp->params->regmask) << dacp->params->regshift);
#endif
  cr &= dacp->params->regmask;
  cr |= (DAC_CR_EN1 | (dacp->config->cr & ~dacp->params->regmask)) <<
                                      dacp->params->regshift;
#else
#if STM32_DAC_HAS_MCR == TRUE
  dacp->params->dac->MCR = dacp->config->mcr;
#endif
  cr = dacp->config->cr | DAC_CR_EN1 | DAC_CR_EN2;
#endif

  dacp->params->dac->CR = cr;
}

#endif /* HAL_USE_DAC */

/** @} */

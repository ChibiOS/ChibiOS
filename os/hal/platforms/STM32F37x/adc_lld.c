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

/**
 * @file    STM32F4xx/adc_lld.c
 * @brief   STM32F4xx/STM32F2xx ADC subsystem low level driver source.
 *
 * @addtogroup ADC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

int debugzero = 0;

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief ADC1 driver identifier.*/
#if STM32_ADC_USE_ADC1 || defined(__DOXYGEN__)
ADCDriver ADCD1;
#endif

/** @brief ADC2 driver identifier.*/
#if STM32_ADC_USE_ADC2 || defined(__DOXYGEN__)
ADCDriver ADCD2;
#endif

/** @brief ADC3 driver identifier.*/
#if STM32_ADC_USE_ADC3 || defined(__DOXYGEN__)
ADCDriver ADCD3;
#endif

/** @brief SDADC1 driver identifier.*/
#if STM32_ADC_USE_SDADC1 || defined(__DOXYGEN__)
ADCDriver SDADCD1;
#endif

/** @brief SDADC2 driver identifier.*/
#if STM32_ADC_USE_SDADC2 || defined(__DOXYGEN__)
ADCDriver SDADCD2;
#endif

/** @brief SDADC3 driver identifier.*/
#if STM32_ADC_USE_SDADC3 || defined(__DOXYGEN__)
ADCDriver SDADCD3;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   ADC DMA ISR service routine.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 *
 * @notapi
 */
static void adc_lld_serve_rx_interrupt(ADCDriver *adcp, uint32_t flags) {
  /* DMA errors handling.*/
  if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0) {
    /* DMA, this could help only if the DMA tries to access an unmapped
       address space or violates alignment rules.*/
    _adc_isr_error_code(adcp, ADC_ERR_DMAFAILURE);
  }
  else {
    /* It is possible that the conversion group has already be reset by the
       ADC error handler, in this case this interrupt is spurious.*/
    if (adcp->grpp != NULL) {
      if ((flags & STM32_DMA_ISR_HTIF) != 0) {
        /* Half transfer processing.*/
        _adc_isr_half_code(adcp);
      }
      if ((flags & STM32_DMA_ISR_TCIF) != 0) {
        /* Transfer complete processing.*/
        _adc_isr_full_code(adcp);
      }
    }
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_ADC_USE_ADC1 || defined(__DOXYGEN__)
/**
 * @brief   ADC interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(Vector88) {
  CH_IRQ_PROLOGUE();

#if STM32_ADC_USE_ADC1
  /* TODO: Add here analog watchdog handling.*/
#endif /* STM32_ADC_USE_ADC1 */

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ADC driver initialization.
 *
 * @notapi
 */
void adc_lld_init(void) {

#if STM32_ADC_USE_ADC1
  /* Driver initialization.*/
  adcObjectInit(&ADCD1);
  ADCD1.adc     = ADC1;
#if STM32_ADC_USE_SDADC
  ADCD1.sdadc   = NULL;
#endif
  ADCD1.dmastp  = STM32_DMA1_STREAM1;
  ADCD1.dmamode = STM32_DMA_CR_CHSEL(ADC1_DMA_CHANNEL) |
                  STM32_DMA_CR_PL(STM32_ADC_ADC1_DMA_PRIORITY) |
                  STM32_DMA_CR_DIR_P2M |
                  STM32_DMA_CR_MSIZE_HWORD | STM32_DMA_CR_PSIZE_HWORD |
                  STM32_DMA_CR_MINC        | STM32_DMA_CR_TCIE        |
                  STM32_DMA_CR_DMEIE       | STM32_DMA_CR_TEIE;
  nvicEnableVector(ADC1_IRQn, CORTEX_PRIORITY_MASK(STM32_ADC_IRQ_PRIORITY));
#endif

#if STM32_ADC_USE_SDADC1
  /* Driver initialization.*/
  adcObjectInit(&SDADCD1);
#if STM32_ADC_USE_ADC
  SDADCD1.adc     = NULL;
#endif
  SDADCD1.sdadc   = SDADC1;
  SDADCD1.dmastp  = STM32_DMA2_STREAM3;
  SDADCD1.dmamode = STM32_DMA_CR_CHSEL(SDADC1_DMA_CHANNEL) |
                  STM32_DMA_CR_PL(STM32_ADC_SDADC1_DMA_PRIORITY) |
                  STM32_DMA_CR_DIR_P2M |
                  STM32_DMA_CR_MSIZE_HWORD | STM32_DMA_CR_PSIZE_HWORD |
                  STM32_DMA_CR_MINC        | STM32_DMA_CR_TCIE        |
                  STM32_DMA_CR_DMEIE       | STM32_DMA_CR_TEIE;
  nvicEnableVector(SDADC1_IRQn,
                   CORTEX_PRIORITY_MASK(STM32_ADC_SDADC1_IRQ_PRIORITY));
#endif

#if STM32_ADC_USE_SDADC2
  /* Driver initialization.*/
  adcObjectInit(&SDADCD2);
#if STM32_ADC_USE_ADC
  SDADCD2.adc     = NULL;
#endif
  SDADCD2.sdadc   = SDADC2;
  SDADCD2.dmastp  = STM32_DMA2_STREAM4;
  SDADCD2.dmamode = STM32_DMA_CR_CHSEL(SDADC2_DMA_CHANNEL) |
                  STM32_DMA_CR_PL(STM32_ADC_SDADC2_DMA_PRIORITY) |
                  STM32_DMA_CR_DIR_P2M |
                  STM32_DMA_CR_MSIZE_HWORD | STM32_DMA_CR_PSIZE_HWORD |
                  STM32_DMA_CR_MINC        | STM32_DMA_CR_TCIE        |
                  STM32_DMA_CR_DMEIE       | STM32_DMA_CR_TEIE;
  nvicEnableVector(SDADC2_IRQn,
                   CORTEX_PRIORITY_MASK(STM32_ADC_SDADC2_IRQ_PRIORITY));
#endif

#if STM32_ADC_USE_SDADC3
  /* Driver initialization.*/
  adcObjectInit(&SDADCD3);
#if STM32_ADC_USE_ADC
  SDADCD3.adc     = NULL;
#endif
  SDADCD3.sdadc   = SDADC3;
  SDADCD3.dmastp  = STM32_DMA2_STREAM5;
  SDADCD3.dmamode = STM32_DMA_CR_CHSEL(SDADC3_DMA_CHANNEL) |
                  STM32_DMA_CR_PL(STM32_ADC_SDADC3_DMA_PRIORITY) |
                  STM32_DMA_CR_DIR_P2M |
                  STM32_DMA_CR_MSIZE_HWORD | STM32_DMA_CR_PSIZE_HWORD |
                  STM32_DMA_CR_MINC        | STM32_DMA_CR_TCIE        |
                  STM32_DMA_CR_DMEIE       | STM32_DMA_CR_TEIE;
  nvicEnableVector(SDADC3_IRQn,
                   CORTEX_PRIORITY_MASK(STM32_ADC_SDADC3_IRQ_PRIORITY));
#endif
}

#if 0
/**
 * @brief   Initial config for SDADC peripheral.
 *
 * @param[in] adcdp            pointer to the @p ADCDriver object
 * @param[in] dmaPriority     priority for the dma channel 0..3
 * @param[in] rxIsrFunc       isr handler for dma,
 * @param[in] dmaSrcLoc       pointer to the @p SDADC data
 * @param[in] periphEnableBit SDADC bit in rcc APB2 Enable register
 *
 * @notapi
 */
void sdadc_lld_start_cr_init_helper(ADCDriver*     adcdp, 
				    uint32_t       dmaPriority, 
				    stm32_dmaisr_t rxIsrFunc, 
				    volatile void* dmaSrcLoc, 
				    uint32_t       periphEnableBit) {
      bool_t b;
      b = dmaStreamAllocate(adcdp->dmastp,
                            dmaPriority,
                            rxIsrFunc,
                            adcdp);
      chDbgAssert(!b, "adc_lld_start(), #1", "stream already allocated");
      dmaStreamSetPeripheral(adcdp->dmastp, dmaSrcLoc);
      rccEnableAPB2(periphEnableBit, FALSE);
      rccResetAPB2(periphEnableBit);

      /* SDADC initial setup, starting the analog part here in order to reduce
	 the latency when starting a conversion.*/

      /*
	====== SDADC CR1 settings breakdown =====
	Initialization mode request               : disabled
	DMA Enabled to read data for reg ch. grp  : disabled
	DMA Enabled to read data for inj ch. grp  : disabled
	Launch reg conv sync w SDADC1             : Do not
	Launch injected conv sync w SDADC1        : Do not
	Enter power down mode when idle           : False
	Enter standby mode when idle              : False
	Slow clock mode                           : fast mode
	Reference voltage selection               : external Vref
	reg data overrun interrupt                : disabled
	reg data end of conversion interrupt      : disabled
	injected data overrun interrupt           : disabled
	injected data end of conversion interrupt : disabled
	end of calibration interrupt              : disabled
       */
      adcdp->sdadc->CR1 = 0;

      /*
	====== SDADC CR1 settings breakdown =====
	SDADC enable                                    : X
	Number of calibration sequences to be performed : 0 
	Start calibration                               : NO
	Continuous mode selection for injected conv     : once
	Delay start of injected conversions             : asap
	Trig sig sel for launching inj conv             : TIM13_CH1,TIM17_CH1, TIM16_CH1
	Trig en and trig edge sel for injected conv     : disabled
	Start a conv of the inj group of ch             : 0
	Regular channel sel (0-8)                       : 0 
	Continuous mode sel for regular conv            : once
	Software start of a conversion on the regular ch: 0
	Fast conv mode sel                              : disabled
      */
      adcdp->sdadc->CR2 = 0;
      adcdp->sdadc->CR2 = SDADC_CR2_ADON;
}
#endif

/**
 * @brief   Configures and activates the ADC peripheral.
 *
 * @param[in] adcdp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_start(ADCDriver *adcdp) {

  /* If in stopped state then enables the ADC and DMA clocks.*/
  if (adcdp->state == ADC_STOP) {
#if STM32_ADC_USE_ADC1
    if (&ADCD1 == adcdp) {
      bool_t b;
      b = dmaStreamAllocate(adcdp->dmastp,
                            STM32_ADC_ADC1_DMA_IRQ_PRIORITY,
                            (stm32_dmaisr_t)adc_lld_serve_rx_interrupt,
                            (void *)adcdp);
      chDbgAssert(!b, "adc_lld_start(), #1", "stream already allocated");
      dmaStreamSetPeripheral(adcdp->dmastp, &ADC1->DR);
      rccEnableADC1(FALSE);

      /* ADC initial setup, starting the analog part here in order to reduce
	     the latency when starting a conversion.*/
      adcdp->adc->CR1 = 0;
      adcdp->adc->CR2 = 0;
      adcdp->adc->CR2 = ADC_CR2_ADON;
    }
#endif /* STM32_ADC_USE_ADC1 */

#if STM32_ADC_USE_SDADC1
    if (&SDADCD1 == adcdp) {
      sdadc_lld_start_cr_init_helper(adcdp,
				     STM32_ADC_SDADC1_DMA_IRQ_PRIORITY,
				     (stm32_dmaisr_t) adc_lld_serve_rx_interrupt,
				     &SDADC1->RDATAR,
				     RCC_APB2ENR_SDADC1EN);
      PWR->CR |= PWR_CR_SDADC1EN;
    }
#endif /* STM32_ADC_USE_SDADC1 */

#if STM32_ADC_USE_SDADC2
    if (&SDADCD2 == adcdp) {
      sdadc_lld_start_cr_init_helper(adcdp,
				     STM32_ADC_SDADC2_DMA_IRQ_PRIORITY,
				     (stm32_dmaisr_t) adc_lld_serve_rx_interrupt,
				     &SDADC2->RDATAR,
				     RCC_APB2ENR_SDADC2EN);
      PWR->CR |= PWR_CR_SDADC2EN;
    }
#endif /* STM32_ADC_USE_SDADC2 */

#if STM32_ADC_USE_SDADC3
    if (&SDADCD3 == adcdp) {
      sdadc_lld_start_cr_init_helper(adcdp,
				     STM32_ADC_SDADC3_DMA_IRQ_PRIORITY,
				     (stm32_dmaisr_t) adc_lld_serve_rx_interrupt,
				     &SDADC3->RDATAR,
				     RCC_APB2ENR_SDADC3EN);
      PWR->CR |= PWR_CR_SDADC3EN;
    }
#endif /* STM32_ADC_USE_SDADC3 */
  }
}

/**
 * @brief   Deactivates the ADC peripheral.
 *
 * @param[in] adcdp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop(ADCDriver *adcdp) {

  /* If in ready state then disables the ADC clock.*/
  if (adcdp->state == ADC_READY) {
    dmaStreamRelease(adcdp->dmastp);

#if STM32_ADC_USE_ADC1
    if (&ADCD1 == adcdp) {
      adcdp->adc->CR1 = 0;
      adcdp->adc->CR2 = 0;
      rccDisableADC1(FALSE);
    }
#endif

#if STM32_ADC_USE_SDADC1
    if (&SDADCD1 == adcdp) {
      adcdp->sdadc-CR1 = 0;
      adcdp->sdadc-CR2 = 0;
      rccDisableSDADC1(FALSE);
      PWR->CR &= ~PWR_CR_SDADC1EN;
    }
#endif

#if STM32_ADC_USE_SDADC2
    if (&SDADCD2 == adcdp) {
      adcdp->sdadc-CR1 = 0;
      adcdp->sdadc-CR2 = 0;
      rccDisableSDADC2(FALSE);
      PWR->CR &= ~PWR_CR_SDADC2EN;
    }
#endif

#if STM32_ADC_USE_SDADC3
    if (&SDADCD3 == adcdp) {
      adcdp->sdadc-CR1 = 0;
      adcdp->sdadc-CR2 = 0;
      rccDisableSDADC3(FALSE);
      PWR->CR &= ~PWR_CR_SDADC3EN;
    }
#endif
  }
}

/**
 * @brief   Starts an ADC conversion.
 *
 * @param[in] adcdp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_start_conversion(ADCDriver *adcdp) {
  uint32_t mode;
  const ADCConversionGroup* grpp = adcdp->grpp;

  /* DMA setup.*/
  mode = adcdp->dmamode;
  if (grpp->circular) {
    mode |= STM32_DMA_CR_CIRC;
  }
  if (adcdp->depth > 1) {
    /* If the buffer depth is greater than one then the half transfer interrupt
       interrupt is enabled in order to allows streaming processing.*/
    mode |= STM32_DMA_CR_HTIE;
  }
  dmaStreamSetMemory0(adcdp->dmastp, adcdp->samples);
  dmaStreamSetTransactionSize(adcdp->dmastp, 
			      (uint32_t)grpp->num_channels * 
			      (uint32_t)adcdp->depth);
  dmaStreamSetMode(adcdp->dmastp, mode);
  dmaStreamEnable(adcdp->dmastp);

#if STM32_ADC_USE_ADC && STM32_ADC_USE_SDADC
  if (adcdp->adc != NULL) {
#endif /* STM32_ADC_USE_ADC && STM32_ADC_USE_SDADC */
#if STM32_ADC_USE_ADC
    /* ADC setup.*/
    adcdp->adc->SR    = 0;
    adcdp->adc->SMPR1 = grpp->u.adc.smpr[0];
    adcdp->adc->SMPR2 = grpp->u.adc.smpr[1];
    adcdp->adc->SQR1  = grpp->u.adc.sqr[0] |
                        ADC_SQR1_NUM_CH(grpp->num_channels);
    adcdp->adc->SQR2  = grpp->u.adc.sqr[1];
    adcdp->adc->SQR3  = grpp->u.adc.sqr[2];

    /* ADC configuration and start, the start is performed using the method
       specified in the CR2 configuration, usually ADC_CR2_SWSTART.*/
    adcdp->adc->CR1   = grpp->u.adc.cr1 | ADC_CR1_SCAN;
    if ((grpp->u.adc.cr2 & ADC_CR2_SWSTART) != 0)
      adcdp->adc->CR2 = grpp->u.adc.cr2 | ADC_CR2_CONT
	                                    | ADC_CR2_DMA | ADC_CR2_ADON;
    else
      adcdp->adc->CR2 = grpp->u.adc.cr2 | ADC_CR2_DMA | ADC_CR2_ADON;
#endif /* STM32_ADC_USE_ADC */
#if STM32_ADC_USE_ADC && STM32_ADC_USE_SDADC
  }
  else if (adcdp->sdadc != NULL) {
#endif /* STM32_ADC_USE_ADC && STM32_ADC_USE_SDADC */
#if STM32_ADC_USE_SDADC
    /* SDADC setup.*/
    sdadcSTM32SetInitializationMode(adcdp, true);

    /* SDADC setup.*/
    adcdp->sdadc->CONF0R   = grpp->ll.sdadc.conf0r;
    adcdp->sdadc->CONF1R   = grpp->ll.sdadc.conf1r;
    adcdp->sdadc->CONF2R   = grpp->ll.sdadc.conf2r;
    adcdp->sdadc->CONFCHR1 = grpp->ll.sdadc.confchr1;
    adcdp->sdadc->CONFCHR2 = grpp->ll.sdadc.confchr2;

    sdadcSTM32SetInitializationMode(adcdp, false);

    /* SDADC configuration and start, the start is performed using the method
       specified in the CR2 configuration, usually ADC_CR2_SWSTART.*/
    adcdp->sdadc->CR1 = grpp->ll.sdadc.cr1 | SDADC_CR1_RDMAEN;
    adcdp->sdadc->CR2 = grpp->ll.sdadc.cr2 | SDADC_CR2_ADON;
#endif /* STM32_ADC_USE_SDADC */
#if STM32_ADC_USE_ADC && STM32_ADC_USE_SDADC
  }
  else {
    chDbgAssert(FALSE, "adc_lld_start_conversion(), #1", "invalid state");
  }
#endif /* STM32_ADC_USE_ADC && STM32_ADC_USE_SDADC */
}

/**
 * @brief   Stops an ongoing conversion.
 *
 * @param[in] adcdp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop_conversion(ADCDriver *adcdp) {

  /* Disabling the associated DMA stream.*/
  dmaStreamDisable(adcdp->dmastp);

#if STM32_ADC_USE_ADC && STM32_ADC_USE_SDADC
  if (adcdp->adc != NULL) {
#endif /* STM32_ADC_USE_ADC && STM32_ADC_USE_SDADC */
#if STM32_ADC_USE_ADC
    adcdp->adc->CR1 = 0;
    adcdp->adc->CR2 = 0;
    adcdp->adc->CR2 = ADC_CR2_ADON;
#endif /* STM32_ADC_USE_ADC */
#if STM32_ADC_USE_ADC && STM32_ADC_USE_SDADC
  }
  else if (adcdp->sdadc != NULL) {
#endif /* STM32_ADC_USE_ADC && STM32_ADC_USE_SDADC */
#if STM32_ADC_USE_SDADC
    adcdp->sdadc->CR1 = 0;
    adcdp->sdadc->CR2 = 0;
    adcdp->sdadc->CR2 = ADC_CR2_ADON;
#endif /* STM32_ADC_USE_SDADC */
#if STM32_ADC_USE_ADC && STM32_ADC_USE_SDADC
  }
  else {
    chDbgAssert(FALSE, "adc_lld_stop_conversion(), #1", "invalid state");
  }
#endif /* STM32_ADC_USE_ADC && STM32_ADC_USE_SDADC */
}

#if STM32_ADC_USE_SDADC || defined(__DOXYGEN__)
/**
 * @brief   Sets the VREF for the 3 Sigma-Delta ADC Converters
 * @details VREF can be changed only when all SDADCs are disabled. Disables all SDADCs, sets the value and then sleeps 5 ms waiting for the change to occur.
 * @note    This is an STM32-only functionality.
 * @param[in] adcdp      pointer to the @p ADCDriver object
 * @param[in] enable    true means init mode, false means exit init mode
 *
 */
void sdadcSTM32VREFSelect(SDADC_VREF_SEL svs)
{
  uint32_t tmpcr1, sdadc1_adon, sdadc2_adon, sdadc3_adon;

  sdadc1_adon = SDADC1->CR2 & SDADC_CR2_ADON;
  sdadc2_adon = SDADC2->CR2 & SDADC_CR2_ADON;
  sdadc3_adon = SDADC3->CR2 & SDADC_CR2_ADON;

  SDADC1->CR2 &= ~SDADC_CR2_ADON;
  SDADC2->CR2 &= ~SDADC_CR2_ADON;
  SDADC3->CR2 &= ~SDADC_CR2_ADON;

  /* Get SDADC1_CR1 register value */
  tmpcr1 = SDADC1->CR1;

  /* Clear the SDADC1_CR1_REFV bits */
  tmpcr1 &= (uint32_t) (~SDADC_CR1_REFV);

  /* Select the external reference voltage */
  tmpcr1 |= svs;

  /* Write in SDADC_CR1 */
  SDADC1->CR1 = tmpcr1;

  /* Insert delay equal to ~10 ms (4 ms required) */
  chThdSleepMilliseconds(5);

  SDADC1->CR2 |= sdadc1_adon;
  SDADC2->CR2 |= sdadc2_adon;
  SDADC3->CR2 |= sdadc3_adon;
}

/**
 * @brief   Sets the Sigma-Delta ADC Converter into initialization mode
 * @details The sdadc is either put into init mode or exits init mode.
 * @note    This is an STM32-only functionality.
 * @note    This function is meant to be called after @p adcStart().
 * @param[in] adcdp      pointer to the @p ADCDriver object
 * @param[in] enable    true means init mode, false means exit init mode
 *
 */
void sdadcSTM32SetInitializationMode(ADCDriver* adcdp, bool_t enterInitMode)
{
  uint32_t SDADCTimeout = 300000;

  if ((adcdp == &SDADCD1) ||
      (adcdp == &SDADCD2) || 
      (adcdp == &SDADCD3)) {

    if (enterInitMode) {
      adcdp->sdadc->CR1 |= SDADC_CR1_INIT;

      /* wait for INITRDY flag to be set */
      while (((adcdp->sdadc->ISR & SDADC_ISR_INITRDY) == 0) && 
	     (--SDADCTimeout != 0));

      if (SDADCTimeout == 0)
      {
	/* INITRDY flag can not set */
	port_halt();
      }
    }
    else {
      adcdp->sdadc->CR1 &= ~SDADC_CR1_INIT;
    }
  }
}

/**
  * @brief  Configures the calibration sequence.
  * @note   TODO - UPDATE
  * @param  ADCDriver*  one of &SDADCD1, &SDADCD2, &SDADCD3
  * @param  SDADC_CalibrationSequence: Number of calibration sequence to be performed.
  *          This parameter can be one of the following values:
  *            @arg SDADC_CalibrationSequence_1: One calibration sequence will be performed
  *                                      to calculate OFFSET0[11:0] (offset that corresponds to conf0)
  *            @arg SDADC_CalibrationSequence_2: Two calibration sequences will be performed
  *                                      to calculate OFFSET0[11:0] and OFFSET1[11:0]
  *                                      (offsets that correspond to conf0 and conf1)
  *            @arg SDADC_CalibrationSequence_3: Three calibration sequences will be performed
  *                                      to calculate OFFSET0[11:0], OFFSET1[11:0], 
  *                                      and OFFSET2[11:0] (offsets that correspond to conf0, conf1 and conf2)
  * @retval None
  */
void sdadcSTM32Calibrate(ADCDriver* adcdp, 
			 SDADC_NUM_CALIB_SEQ numCalibSequences,
			 ADCConversionGroup* grpp)
{
  uint32_t SDADCTimeout = 0;
  uint32_t tmpcr2 = 0;

  if (!(adcdp == &SDADCD1 ||
	adcdp == &SDADCD2 || 
	adcdp == &SDADCD3))
      return;

  sdadcSTM32SetInitializationMode(adcdp, true);

  /* SDADC setup.*/
  adcdp->sdadc->CR2      = grpp->ll.sdadc.cr2;
  adcdp->sdadc->CONF0R   = grpp->ll.sdadc.conf0r;
  adcdp->sdadc->CONF1R   = grpp->ll.sdadc.conf1r;
  adcdp->sdadc->CONF2R   = grpp->ll.sdadc.conf2r;
  adcdp->sdadc->CONFCHR1 = grpp->ll.sdadc.confchr1;
  adcdp->sdadc->CONFCHR2 = grpp->ll.sdadc.confchr2;

  sdadcSTM32SetInitializationMode(adcdp, false);

  /* configure calibration to be performed on conf0 */
  /* Get SDADC_CR2 register value */
  tmpcr2 = adcdp->sdadc->CR2;

  /* Clear the SDADC_CR2_CALIBCNT bits */
  tmpcr2 &= (uint32_t) (~SDADC_CR2_CALIBCNT);
  /* Set the calibration sequence */
  tmpcr2 |= numCalibSequences;

  /* 
     Write in SDADC_CR2 and
     start calibration 
  */
  adcdp->sdadc->CR2 = tmpcr2 | SDADC_CR2_STARTCALIB;
 
  /* Set calibration timeout: 5.12 ms at 6 MHz in a single calibration sequence */
  SDADCTimeout = SDADC_CAL_TIMEOUT;

  /* wait for SDADC Calibration process to end */
  while (((adcdp->sdadc->ISR & SDADC_ISR_EOCALF) == 0) && (--SDADCTimeout != 0));

  if(SDADCTimeout == 0)
  {
    /* Calib timeout */
    port_halt();
    return;
  }

  /* cleanup by clearing EOCALF flag */
  adcdp->sdadc->CLRISR |= SDADC_ISR_CLREOCALF;
}
#endif /* STM32_ADC_USE_SDADC */

#endif /* HAL_USE_ADC */

/** @} */

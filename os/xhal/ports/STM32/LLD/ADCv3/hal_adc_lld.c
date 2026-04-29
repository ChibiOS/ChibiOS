/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    ADCv3/hal_adc_lld.c
 * @brief   STM32 ADC subsystem low level driver source.
 *
 * @addtogroup ADC
 * @{
 */

#include "hal.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#if STM32_ADC_DUAL_MODE
#if STM32_ADC_COMPACT_SAMPLES
/* Compact type dual mode.*/
#define ADC_DMA_SIZE    (STM32_DMA_CR_MSIZE_HWORD | STM32_DMA_CR_PSIZE_HWORD)
#define ADC_DMA_MDMA    ADC_CCR_MDMA_HWORD

#else /* !STM32_ADC_COMPACT_SAMPLES */
/* Large type dual mode.*/
#define ADC_DMA_SIZE    (STM32_DMA_CR_MSIZE_WORD | STM32_DMA_CR_PSIZE_WORD)
#define ADC_DMA_MDMA    ADC_CCR_MDMA_WORD
#endif /* !STM32_ADC_COMPACT_SAMPLES */

#else /* !STM32_ADC_DUAL_MODE */
#if STM32_ADC_COMPACT_SAMPLES
/* Compact type single mode.*/
#define ADC_DMA_SIZE    (STM32_DMA_CR_MSIZE_BYTE | STM32_DMA_CR_PSIZE_BYTE)
#define ADC_DMA_MDMA    ADC_CCR_MDMA_DISABLED

#else /* !STM32_ADC_COMPACT_SAMPLES */
/* Large type single mode.*/
#define ADC_DMA_SIZE    (STM32_DMA_CR_MSIZE_HWORD | STM32_DMA_CR_PSIZE_HWORD)
#define ADC_DMA_MDMA    ADC_CCR_MDMA_DISABLED
#endif /* !STM32_ADC_COMPACT_SAMPLES */
#endif /* !STM32_ADC_DUAL_MODE */

/* Addressing header differences.*/
#if !defined(ADC_IER_OVRIE)
#define ADC_IER_OVRIE           ADC_IER_OVR
#endif

#if !defined(ADC_IER_AWD1IE)
#define ADC_IER_AWD1IE          ADC_IER_AWD1
#endif

#if !defined(ADC_ISR_ADRDY)
#define ADC_ISR_ADRDY           ADC_ISR_ADRD
#endif

/* The following bits are too different in the various headers, just
   redefining those here. Values can be overridden by placing definitions
   in hal_lld.h.*/
#if !defined(STM32_ADC_CR_ADVREGEN)
#define STM32_ADC_CR_ADVREGEN   (1U << 28)
#endif

#if !defined(STM32_ADC_CR_DEEPPWD)
#define STM32_ADC_CR_DEEPPWD    (1U << 29)
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief ADC1 driver identifier.*/
#if STM32_ADC_USE_ADC1 || defined(__DOXYGEN__)
hal_adc_driver_c ADCD1;
#endif

/** @brief ADC2 driver identifier.*/
#if STM32_ADC_USE_ADC2 || defined(__DOXYGEN__)
hal_adc_driver_c ADCD2;
#endif

/** @brief ADC3 driver identifier.*/
#if STM32_ADC_USE_ADC3 || defined(__DOXYGEN__)
hal_adc_driver_c ADCD3;
#endif

/** @brief ADC4 driver identifier.*/
#if STM32_ADC_USE_ADC4 || defined(__DOXYGEN__)
hal_adc_driver_c ADCD4;
#endif

/** @brief ADC5 driver identifier.*/
#if STM32_ADC_USE_ADC5 || defined(__DOXYGEN__)
hal_adc_driver_c ADCD5;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if ADC_USE_CONFIGURATIONS != TRUE
static const hal_adc_config_t default_config = {
  .grps   = NULL,
  .difsel = 0
};
#endif

static uint32_t clkmask;

#define ADC1_CLKMASK                        (1U << 0)
#define ADC2_CLKMASK                        (1U << 1)
#define ADC3_CLKMASK                        (1U << 2)
#define ADC4_CLKMASK                        (1U << 3)
#define ADC5_CLKMASK                        (1U << 4)
#define ADC12_CLKMASK                       (ADC1_CLKMASK | ADC2_CLKMASK)
#define ADC34_CLKMASK                       (ADC3_CLKMASK | ADC4_CLKMASK)
#define ADC123_CLKMASK                      (ADC1_CLKMASK | ADC2_CLKMASK | ADC3_CLKMASK)
#define ADC345_CLKMASK                      (ADC3_CLKMASK | ADC4_CLKMASK | ADC5_CLKMASK)

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Enables the ADC voltage regulator.
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 */
static void adc_lld_vreg_on(hal_adc_driver_c *adcp) {

  adcp->adcm->CR = 0;   /* See RM.*/
  adcp->adcm->CR = STM32_ADC_CR_ADVREGEN;
#if STM32_ADC_DUAL_MODE
  adcp->adcs->CR = STM32_ADC_CR_ADVREGEN;
#endif
  osalSysPolledDelayX(OSAL_US2RTC(STM32_HCLK, 20));
}

/**
 * @brief   Disables the ADC voltage regulator.
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 */
static void adc_lld_vreg_off(hal_adc_driver_c *adcp) {

  adcp->adcm->CR = 0;   /* See RM.*/
  adcp->adcm->CR = STM32_ADC_CR_DEEPPWD;
#if STM32_ADC_DUAL_MODE
  adcp->adcs->CR = 0;
  adcp->adcs->CR = STM32_ADC_CR_DEEPPWD;
#endif
}

/**
 * @brief   Calibrates an ADC unit.
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 */
static void adc_lld_calibrate(hal_adc_driver_c *adcp) {

  osalDbgAssert(adcp->adcm->CR == STM32_ADC_CR_ADVREGEN,
                "invalid register state");

  /* Differential calibration for master ADC.*/
  adcp->adcm->CR = STM32_ADC_CR_ADVREGEN | ADC_CR_ADCALDIF;
  adcp->adcm->CR = STM32_ADC_CR_ADVREGEN | ADC_CR_ADCALDIF | ADC_CR_ADCAL;
  while ((adcp->adcm->CR & ADC_CR_ADCAL) != 0)
    ;

  osalSysPolledDelayX(OSAL_US2RTC(STM32_HCLK, 20));

  /* Single-ended calibration for master ADC.*/
  adcp->adcm->CR = STM32_ADC_CR_ADVREGEN;
  adcp->adcm->CR = STM32_ADC_CR_ADVREGEN | ADC_CR_ADCAL;
  while ((adcp->adcm->CR & ADC_CR_ADCAL) != 0)
    ;

  osalSysPolledDelayX(OSAL_US2RTC(STM32_HCLK, 20));

#if STM32_ADC_DUAL_MODE
  osalDbgAssert(adcp->adcs->CR == STM32_ADC_CR_ADVREGEN,
                "invalid register state");

  /* Differential calibration for slave ADC.*/
  adcp->adcs->CR = STM32_ADC_CR_ADVREGEN | ADC_CR_ADCALDIF;
  adcp->adcs->CR = STM32_ADC_CR_ADVREGEN | ADC_CR_ADCALDIF | ADC_CR_ADCAL;
  while ((adcp->adcs->CR & ADC_CR_ADCAL) != 0)
    ;

  osalSysPolledDelayX(OSAL_US2RTC(STM32_HCLK, 20));

  /* Single-ended calibration for slave ADC.*/
  adcp->adcs->CR = STM32_ADC_CR_ADVREGEN;
  adcp->adcs->CR = STM32_ADC_CR_ADVREGEN | ADC_CR_ADCAL;
  while ((adcp->adcs->CR & ADC_CR_ADCAL) != 0)
    ;

  osalSysPolledDelayX(OSAL_US2RTC(STM32_HCLK, 20));
#endif
}

/**
 * @brief   Enables the ADC analog circuit.
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 */
static void adc_lld_analog_on(hal_adc_driver_c *adcp) {

  adcp->adcm->CR |= ADC_CR_ADEN;
  while ((adcp->adcm->ISR & ADC_ISR_ADRDY) == 0)
    ;
#if STM32_ADC_DUAL_MODE
  adcp->adcs->CR |= ADC_CR_ADEN;
  while ((adcp->adcs->ISR & ADC_ISR_ADRDY) == 0)
    ;
#endif
}

/**
 * @brief   Disables the ADC  analog circuit.
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 */
static void adc_lld_analog_off(hal_adc_driver_c *adcp) {

  adcp->adcm->CR |= ADC_CR_ADDIS;
  while ((adcp->adcm->CR & ADC_CR_ADDIS) != 0)
    ;
#if STM32_ADC_DUAL_MODE
  adcp->adcs->CR |= ADC_CR_ADDIS;
  while ((adcp->adcs->CR & ADC_CR_ADDIS) != 0)
    ;
#endif
}

/**
 * @brief   Stops an ongoing conversion, if any.
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 */
static void adc_lld_stop_adc(hal_adc_driver_c *adcp) {

  if (adcp->adcm->CR & ADC_CR_ADSTART) {
    adcp->adcm->CR |= ADC_CR_ADSTP;
    while (adcp->adcm->CR & ADC_CR_ADSTP)
      ;
    adcp->adcm->IER = 0;
  }
}

/**
 * @brief   ADC DMA service routine.
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void adc_lld_serve_dma_interrupt(hal_adc_driver_c *adcp, uint32_t flags) {

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
      if ((flags & STM32_DMA_ISR_TCIF) != 0) {
        /* Transfer complete processing.*/
        _adc_isr_full_code(adcp);
      }
      else if ((flags & STM32_DMA_ISR_HTIF) != 0) {
        /* Half transfer processing.*/
        _adc_isr_half_code(adcp);
      }
    }
  }
}

/**
 * @brief   ADC IRQ service routine.
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 */
void adc_lld_serve_interrupt(hal_adc_driver_c *adcp) {
  uint32_t isr;

  isr = adcp->adcm->ISR;
  adcp->adcm->ISR = isr;
#if STM32_ADC_DUAL_MODE
  if (adcp->adcs != NULL) {
    uint32_t sisr;

    sisr = adcp->adcs->ISR;
    adcp->adcs->ISR = sisr;
    isr |= sisr;
  }
#endif

  /* It could be a spurious interrupt caused by overflows after DMA disabling,
     just ignore it in this case.*/
  if (adcp->grpp != NULL) {
    adcerror_t emask = 0U;

    /* Note, an overflow may occur after the conversion ended before the driver
       is able to stop the ADC, this is why the state is checked too.*/
    if ((isr & ADC_ISR_OVR) && (adcp->state == HAL_DRV_STATE_ACTIVE)) {
      /* ADC overflow condition, this could happen only if the DMA is unable
         to read data fast enough.*/
      emask |= ADC_ERR_OVERFLOW;
    }
    if (isr & ADC_ISR_AWD1) {
      /* Analog watchdog 1 error.*/
      emask |= ADC_ERR_AWD1;
    }
    if (isr & ADC_ISR_AWD2) {
      /* Analog watchdog 2 error.*/
      emask |= ADC_ERR_AWD2;
    }
    if (isr & ADC_ISR_AWD3) {
      /* Analog watchdog 3 error.*/
      emask |= ADC_ERR_AWD3;
    }
    if (emask != 0U) {
      _adc_isr_error_code(adcp, emask);
    }
  }
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ADC driver initialization.
 *
 * @notapi
 */
void adc_lld_init(void) {

  clkmask = 0;

#if STM32_ADC_USE_ADC1
  /* Driver initialization.*/
  adcObjectInit(&ADCD1);
#if defined(ADC1_2_COMMON)
  ADCD1.adcc = ADC1_2_COMMON;
#elif defined(ADC12_COMMON)
  ADCD1.adcc = ADC12_COMMON;
#elif defined(ADC123_COMMON)
  ADCD1.adcc = ADC123_COMMON;
#else
  ADCD1.adcc = ADC1_COMMON;
#endif
  ADCD1.adcm    = ADC1;
#if STM32_ADC_DUAL_MODE
  ADCD1.adcs    = ADC2;
#endif
  ADCD1.dmastp  = NULL;
  ADCD1.dmamode = ADC_DMA_SIZE |
                  STM32_DMA_CR_PL(STM32_ADC_ADC1_DMA_PRIORITY) |
                  STM32_DMA_CR_DIR_P2M |
                  STM32_DMA_CR_MINC        | STM32_DMA_CR_TCIE        |
                  STM32_DMA_CR_DMEIE       | STM32_DMA_CR_TEIE;
#endif /* STM32_ADC_USE_ADC1 */

#if STM32_ADC_USE_ADC2
  /* Driver initialization.*/
  adcObjectInit(&ADCD2);
#if defined(ADC1_2_COMMON)
  ADCD2.adcc = ADC1_2_COMMON;
#elif defined(ADC12_COMMON)
  ADCD2.adcc = ADC12_COMMON;
#elif defined(ADC123_COMMON)
  ADCD2.adcc = ADC123_COMMON;
#endif
  ADCD2.adcm    = ADC2;
  ADCD2.dmastp  = NULL;
  ADCD2.dmamode = ADC_DMA_SIZE |
                  STM32_DMA_CR_PL(STM32_ADC_ADC2_DMA_PRIORITY) |
                  STM32_DMA_CR_DIR_P2M |
                  STM32_DMA_CR_MINC        | STM32_DMA_CR_TCIE        |
                  STM32_DMA_CR_DMEIE       | STM32_DMA_CR_TEIE;
#endif /* STM32_ADC_USE_ADC2 */

#if STM32_ADC_USE_ADC3
  /* Driver initialization.*/
  adcObjectInit(&ADCD3);
#if defined(ADC3_4_COMMON)
  ADCD3.adcc = ADC3_4_COMMON;
#elif defined(ADC345_COMMON)
  ADCD3.adcc = ADC345_COMMON;
#elif defined(ADC123_COMMON)
  ADCD3.adcc = ADC123_COMMON;
#else
  ADCD3.adcc = ADC3_COMMON;
#endif
  ADCD3.adcm    = ADC3;
#if STM32_ADC_DUAL_MODE
  ADCD3.adcs    = ADC4;
#endif
  ADCD3.dmastp  = NULL;
  ADCD3.dmamode = ADC_DMA_SIZE |
                  STM32_DMA_CR_PL(STM32_ADC_ADC3_DMA_PRIORITY) |
                  STM32_DMA_CR_DIR_P2M |
                  STM32_DMA_CR_MINC        | STM32_DMA_CR_TCIE        |
                  STM32_DMA_CR_DMEIE       | STM32_DMA_CR_TEIE;
#endif /* STM32_ADC_USE_ADC3 */

#if STM32_ADC_USE_ADC4
  /* Driver initialization.*/
  adcObjectInit(&ADCD4);
#if defined(ADC3_4_COMMON)
  ADCD4.adcc = ADC3_4_COMMON;
#elif defined(ADC345_COMMON)
  ADCD4.adcc = ADC345_COMMON;
#endif
  ADCD4.adcm    = ADC4;
  ADCD4.dmastp  = NULL;
  ADCD4.dmamode = ADC_DMA_SIZE |
                  STM32_DMA_CR_PL(STM32_ADC_ADC4_DMA_PRIORITY) |
                  STM32_DMA_CR_DIR_P2M |
                  STM32_DMA_CR_MINC        | STM32_DMA_CR_TCIE        |
                  STM32_DMA_CR_DMEIE       | STM32_DMA_CR_TEIE;
#endif /* STM32_ADC_USE_ADC4 */

#if STM32_ADC_USE_ADC5
  /* Driver initialization.*/
  adcObjectInit(&ADCD5);
#if defined(ADC345_COMMON)
  ADCD5.adcc = ADC345_COMMON;
#endif
  ADCD5.adcm    = ADC5;
  ADCD5.dmastp  = NULL;
  ADCD5.dmamode = ADC_DMA_SIZE |
                  STM32_DMA_CR_PL(STM32_ADC_ADC5_DMA_PRIORITY) |
                  STM32_DMA_CR_DIR_P2M |
                  STM32_DMA_CR_MINC        | STM32_DMA_CR_TCIE        |
                  STM32_DMA_CR_DMEIE       | STM32_DMA_CR_TEIE;
#endif /* STM32_ADC_USE_ADC5 */

  /* ADC units pre-initializations.*/
#if defined(STM32F3XX)
#if STM32_HAS_ADC1 && STM32_HAS_ADC2
#if STM32_ADC_USE_ADC1 || STM32_ADC_USE_ADC2
  rccResetADC12();
  rccEnableADC12(true);
  ADC1_2_COMMON->CCR = STM32_ADC_ADC12_CLOCK_MODE | ADC_DMA_MDMA;
  rccDisableADC12();
#endif
#else
#if STM32_ADC_USE_ADC1
  rccResetADC12();
  rccEnableADC12(true);
  ADC1_COMMON->CCR = STM32_ADC_ADC12_CLOCK_MODE | ADC_DMA_MDMA;
  rccDisableADC12();
#endif
#endif
#if STM32_ADC_USE_ADC3 || STM32_ADC_USE_ADC4
  rccResetADC34();
  rccEnableADC34(true);
  ADC3_4_COMMON->CCR = STM32_ADC_ADC34_CLOCK_MODE | ADC_DMA_MDMA;
  rccDisableADC34();
#endif
#endif

#if defined(STM32L4XX) || defined(STM32L4XXP)
  rccResetADC123();
  rccEnableADC123(true);
#if defined(ADC1_2_COMMON)
  ADC1_2_COMMON->CCR = STM32_ADC_ADC123_PRESC | STM32_ADC_ADC123_CLOCK_MODE | ADC_DMA_MDMA;
#elif defined(ADC12_COMMON)
  ADC12_COMMON->CCR = STM32_ADC_ADC123_PRESC | STM32_ADC_ADC123_CLOCK_MODE | ADC_DMA_MDMA;
#elif defined(ADC123_COMMON)
  ADC123_COMMON->CCR = STM32_ADC_ADC123_PRESC | STM32_ADC_ADC123_CLOCK_MODE | ADC_DMA_MDMA;
#else
  ADC1_COMMON->CCR   = STM32_ADC_ADC123_PRESC | STM32_ADC_ADC123_CLOCK_MODE | ADC_DMA_MDMA;
#endif
  rccDisableADC123();
#endif

#if defined(STM32G4XX)
#if STM32_ADC_USE_ADC1 || STM32_ADC_USE_ADC2
  rccResetADC12();
  rccEnableADC12(true);
  ADC12_COMMON->CCR = STM32_ADC_ADC12_PRESC | STM32_ADC_ADC12_CLOCK_MODE | ADC_DMA_MDMA;
  rccDisableADC12();
#endif
#if STM32_ADC_USE_ADC3 || STM32_ADC_USE_ADC4 || STM32_ADC_USE_ADC5
  rccResetADC345();
  rccEnableADC345(true);
  ADC345_COMMON->CCR = STM32_ADC_ADC345_PRESC | STM32_ADC_ADC345_CLOCK_MODE | ADC_DMA_MDMA;
  rccDisableADC345();
#endif
#endif

#if defined(STM32WBXX)
#if STM32_ADC_USE_ADC1
  rccResetADC1();
  rccEnableADC1(true);
  ADC1_COMMON->CCR = STM32_ADC_ADC1_PRESC | STM32_ADC_ADC1_CLOCK_MODE;
  rccDisableADC1();
#endif
#endif
}

/**
 * @brief   Configures and activates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 *
 * @notapi
 */
msg_t adc_lld_start(hal_adc_driver_c *adcp) {
  const hal_adc_config_t *cfg;

  cfg = (const hal_adc_config_t *)adcp->config;
  if (cfg == NULL) {
    cfg = adc_lld_selcfg(adcp, 0U);
  }
  if (cfg == NULL) {
    return HAL_RET_CONFIG_ERROR;
  }

  /* Enabling the ADC and DMA clocks.*/
  {
#if STM32_ADC_USE_ADC1
    if (&ADCD1 == adcp) {

      osalDbgAssert(STM32_ADC1_CLOCK <= STM32_ADCCLK_MAX,
                    "invalid clock frequency");

      adcp->dmastp = dmaStreamAlloc(STM32_ADC_ADC1_DMA_STREAM,
                                    STM32_ADCV3_ADC1_IRQ_PRIORITY,
                                    (stm32_dmaisr_t)adc_lld_serve_dma_interrupt,
                                    (void *)adcp);
      osalDbgAssert(adcp->dmastp != NULL, "unable to allocate stream");

      clkmask |= ADC1_CLKMASK;
#if defined(STM32F3XX) || defined(STM32G4XX)
      rccEnableADC12(true);
#endif
#if defined(STM32L4XX) || defined(STM32L4XXP)
      rccEnableADC123(true);
#endif
#if defined(STM32WBXX)
      rccEnableADC1(true);
#endif
#if STM32_DMA_SUPPORTS_DMAMUX
      dmaSetRequestSource(adcp->dmastp, STM32_DMAMUX1_ADC1);
#endif
    }
#endif /* STM32_ADC_USE_ADC1 */

#if STM32_ADC_USE_ADC2
    if (&ADCD2 == adcp) {

      osalDbgAssert(STM32_ADC2_CLOCK <= STM32_ADCCLK_MAX,
                    "invalid clock frequency");

      adcp->dmastp = dmaStreamAlloc(STM32_ADC_ADC2_DMA_STREAM,
                                    STM32_ADCV3_ADC2_IRQ_PRIORITY,
                                    (stm32_dmaisr_t)adc_lld_serve_dma_interrupt,
                                    (void *)adcp);
      osalDbgAssert(adcp->dmastp != NULL, "unable to allocate stream");

      clkmask |= ADC2_CLKMASK;
#if defined(STM32F3XX) || defined(STM32G4XX)
      rccEnableADC12(true);
#endif
#if defined(STM32L4XX) || defined(STM32L4XXP)
      rccEnableADC123(true);
#endif
#if STM32_DMA_SUPPORTS_DMAMUX
      dmaSetRequestSource(adcp->dmastp, STM32_DMAMUX1_ADC2);
#endif
    }
#endif /* STM32_ADC_USE_ADC2 */

#if STM32_ADC_USE_ADC3
    if (&ADCD3 == adcp) {

      osalDbgAssert(STM32_ADC3_CLOCK <= STM32_ADCCLK_MAX,
                    "invalid clock frequency");

      adcp->dmastp = dmaStreamAlloc(STM32_ADC_ADC3_DMA_STREAM,
                                    STM32_ADCV3_ADC3_IRQ_PRIORITY,
                                    (stm32_dmaisr_t)adc_lld_serve_dma_interrupt,
                                    (void *)adcp);
      osalDbgAssert(adcp->dmastp != NULL, "unable to allocate stream");

      clkmask |= ADC3_CLKMASK;
#if defined(STM32F3XX)
      rccEnableADC34(true);
#endif
#if defined(STM32L4XX) || defined(STM32L4XXP)
      rccEnableADC123(true);
#endif
#if defined(STM32G4XX)
      rccEnableADC345(true);
#endif
#if STM32_DMA_SUPPORTS_DMAMUX
      dmaSetRequestSource(adcp->dmastp, STM32_DMAMUX1_ADC3);
#endif
    }
#endif /* STM32_ADC_USE_ADC3 */

#if STM32_ADC_USE_ADC4
    if (&ADCD4 == adcp) {

      osalDbgAssert(STM32_ADC4_CLOCK <= STM32_ADCCLK_MAX,
                    "invalid clock frequency");

      adcp->dmastp = dmaStreamAlloc(STM32_ADC_ADC4_DMA_STREAM,
                                    STM32_ADCV3_ADC4_IRQ_PRIORITY,
                                    (stm32_dmaisr_t)adc_lld_serve_dma_interrupt,
                                    (void *)adcp);
      osalDbgAssert(adcp->dmastp != NULL, "unable to allocate stream");

      clkmask |= ADC4_CLKMASK;
#if defined(STM32F3XX)
      rccEnableADC34(true);
#endif
#if defined(STM32L4XX) || defined(STM32L4XXP)
      rccEnableADC123(true);
#endif
#if defined(STM32G4XX)
      rccEnableADC345(true);
#endif
#if STM32_DMA_SUPPORTS_DMAMUX
      dmaSetRequestSource(adcp->dmastp, STM32_DMAMUX1_ADC4);
#endif
    }
#endif /* STM32_ADC_USE_ADC4 */

#if STM32_ADC_USE_ADC5
    if (&ADCD5 == adcp) {
      osalDbgAssert(STM32_ADC5_CLOCK <= STM32_ADCCLK_MAX,
                    "invalid clock frequency");

      adcp->dmastp = dmaStreamAlloc(STM32_ADC_ADC5_DMA_STREAM,
                                    STM32_ADCV3_ADC5_IRQ_PRIORITY,
                                    (stm32_dmaisr_t)adc_lld_serve_dma_interrupt,
                                    (void *)adcp);
      osalDbgAssert(adcp->dmastp != NULL, "unable to allocate stream");

      clkmask |= ADC5_CLKMASK;
#if defined(STM32G4XX)
      rccEnableADC345(true);
#endif
#if STM32_DMA_SUPPORTS_DMAMUX
      dmaSetRequestSource(adcp->dmastp, STM32_DMAMUX1_ADC5);
#endif
    }
#endif /* STM32_ADC_USE_ADC5 */

    /* Setting DMA peripheral-side pointer.*/
#if STM32_ADC_DUAL_MODE
    dmaStreamSetPeripheral(adcp->dmastp, &adcp->adcc->CDR);
#else
    dmaStreamSetPeripheral(adcp->dmastp, &adcp->adcm->DR);
#endif

    /* Differential channels setting.*/
#if STM32_ADC_DUAL_MODE
    adcp->adcm->DIFSEL = cfg->difsel;
    adcp->adcs->DIFSEL = cfg->difsel;
#else
    adcp->adcm->DIFSEL = cfg->difsel;
#endif

    /* Master ADC calibration.*/
    adc_lld_vreg_on(adcp);
    adc_lld_calibrate(adcp);

    /* Master ADC enabled here in order to reduce conversions latencies.*/
    adc_lld_analog_on(adcp);
  }

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Deactivates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 *
 * @notapi
 */
void adc_lld_stop(hal_adc_driver_c *adcp) {

  /* If stopping then disables the ADC clock and analog part.*/
  if (adcp->state == HAL_DRV_STATE_STOPPING) {

    /* Releasing the associated DMA channel.*/
    dmaStreamFree(adcp->dmastp);
    adcp->dmastp = NULL;

    /* Stopping the ongoing conversion, if any.*/
    adc_lld_stop_adc(adcp);

    /* Disabling ADC analog circuit and regulator.*/
    adc_lld_analog_off(adcp);
    adc_lld_vreg_off(adcp);

#if STM32_ADC_USE_ADC1
    if (&ADCD1 == adcp) {
      /* Resetting CCR options except default ones.*/
      clkmask &= ~ADC1_CLKMASK;
    }
#endif

#if STM32_ADC_USE_ADC2
    if (&ADCD2 == adcp) {
      clkmask &= ~ADC2_CLKMASK;
    }
#endif

#if STM32_ADC_USE_ADC3
    if (&ADCD3 == adcp) {
      clkmask &= ~ADC3_CLKMASK;
    }
#endif

#if STM32_ADC_USE_ADC4
    if (&ADCD4 == adcp) {
      clkmask &= ~ADC4_CLKMASK;
    }
#endif

#if STM32_ADC_USE_ADC5
    if (&ADCD5 == adcp) {
      clkmask &= ~ADC5_CLKMASK;
    }
#endif

#if defined(STM32F3XX)
#if STM32_HAS_ADC1 || STM32_HAS_ADC2
    if ((clkmask & ADC12_CLKMASK) == 0U) {
      rccDisableADC12();
    }
#endif

#if STM32_HAS_ADC3 || STM32_HAS_ADC4
    if ((clkmask & ADC34_CLKMASK) == 0U) {
      rccDisableADC34();
    }
#endif
#endif

#if defined(STM32L4XX) || defined(STM32L4XXP)
    if ((clkmask & ADC123_CLKMASK) == 0U) {
      rccDisableADC123();
    }
#endif

#if defined(STM32WBXX)
    if ((clkmask & ADC1_CLKMASK) == 0U) {
      rccDisableADC1();
    }
#endif

#if defined(STM32G4XX)
#if STM32_HAS_ADC1 || STM32_HAS_ADC2
    if ((clkmask & ADC12_CLKMASK) == 0U) {
      rccDisableADC12();
    }
#endif

#if STM32_HAS_ADC3 || STM32_HAS_ADC4 || STM32_HAS_ADC5
    if ((clkmask & ADC345_CLKMASK) == 0U) {
      rccDisableADC345();
    }
#endif
#endif
  }
}

/**
 * @brief   Starts an ADC conversion.
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 *
 * @notapi
 */
const hal_adc_config_t *adc_lld_setcfg(hal_adc_driver_c *adcp,
                                       const hal_adc_config_t *config) {
  (void)adcp;

  if (config == NULL) {
    return adc_lld_selcfg(adcp, 0U);
  }

  return config;
}

const hal_adc_config_t *adc_lld_selcfg(hal_adc_driver_c *adcp,
                                       unsigned cfgnum) {
#if ADC_USE_CONFIGURATIONS == TRUE
  extern const adc_configurations_t adc_configurations;

  if (cfgnum >= adc_configurations.cfgsnum) {
    return NULL;
  }

  return adc_lld_setcfg(adcp, &adc_configurations.cfgs[cfgnum]);
#else

  (void)adcp;

  if (cfgnum > 0U) {
    return NULL;
  }

  return &default_config;
#endif
}

void adc_lld_set_callback(hal_adc_driver_c *adcp, drv_cb_t cb) {
  (void)adcp;
  (void)cb;
}

msg_t adc_lld_start_conversion(hal_adc_driver_c *adcp, unsigned grpnum,
                               adcsample_t *samples,
                               size_t depth) {
  const hal_adc_config_t *config = (const hal_adc_config_t *)adcp->config;
  const adc_conversion_group_t *grpp;
  uint32_t dmamode, cfgr;
  bool circular;
#if STM32_ADC_DUAL_MODE
  uint32_t ccr;
#endif

  if ((config == NULL) || (config->grps == NULL) ||
      (grpnum >= config->grps->grpsnum)) {
    return HAL_RET_CONFIG_ERROR;
  }

  grpp = &config->grps->grps[grpnum];
  adcp->grpp = grpp;
  circular = (adcp->state == ADC_ACTIVE_CIRCULAR);
#if STM32_ADC_DUAL_MODE
  ccr = grpp->ccr & ~(ADC_CCR_CKMODE_MASK | ADC_CCR_MDMA_MASK);
#endif

  osalDbgAssert(!STM32_ADC_DUAL_MODE || ((grpp->num_channels & 1) == 0),
                "odd number of channels in dual mode");

  /* Calculating control registers values.*/
  dmamode = adcp->dmamode;
  cfgr    = grpp->cfgr | ADC_CFGR_DMAEN;
  if (circular) {
    dmamode |= STM32_DMA_CR_CIRC;
#if STM32_ADC_DUAL_MODE
    ccr  |= ADC_CCR_DMACFG_CIRCULAR;
#else
    cfgr |= ADC_CFGR_DMACFG_CIRCULAR;
#endif
    if (depth > 1U) {
      /* If circular buffer depth > 1, then the half transfer interrupt
         is enabled in order to allow streaming processing.*/
      dmamode |= STM32_DMA_CR_HTIE;
    }
  }

  /* DMA setup.*/
  dmaStreamSetMemory0(adcp->dmastp, samples);
#if STM32_ADC_DUAL_MODE
  dmaStreamSetTransactionSize(adcp->dmastp, ((uint32_t)grpp->num_channels/2) *
                                            (uint32_t)depth);
#else
  dmaStreamSetTransactionSize(adcp->dmastp, (uint32_t)grpp->num_channels *
                                            (uint32_t)depth);
#endif
  dmaStreamSetMode(adcp->dmastp, dmamode);
  dmaStreamEnable(adcp->dmastp);

  adcp->adcm->ISR   = adcp->adcm->ISR;
  adcp->adcm->IER    = ADC_IER_OVRIE | ADC_IER_AWD1IE
                                     | ADC_IER_AWD2IE
                                     | ADC_IER_AWD3IE;
  adcp->adcm->TR1    = grpp->tr1;
  adcp->adcm->TR2    = grpp->tr2;
  adcp->adcm->TR3    = grpp->tr3;
  adcp->adcm->AWD2CR = grpp->awd2cr;
  adcp->adcm->AWD3CR = grpp->awd3cr;

#if STM32_ADC_DUAL_MODE

  /* Configuring the CCR register with the user-specified settings
     in the conversion group configuration structure, static settings are
     preserved.*/
  adcp->adcc->CCR   = (adcp->adcc->CCR &
                       (ADC_CCR_CKMODE_MASK | ADC_CCR_MDMA_MASK)) | ccr;

  adcp->adcm->SMPR1 = grpp->smpr[0];
  adcp->adcm->SMPR2 = grpp->smpr[1];
  adcp->adcm->SQR1  = grpp->sqr[0] | ADC_SQR1_NUM_CH(grpp->num_channels / 2);
  adcp->adcm->SQR2  = grpp->sqr[1];
  adcp->adcm->SQR3  = grpp->sqr[2];
  adcp->adcm->SQR4  = grpp->sqr[3];
  adcp->adcs->SMPR1 = grpp->ssmpr[0];
  adcp->adcs->SMPR2 = grpp->ssmpr[1];
  adcp->adcs->SQR1  = grpp->ssqr[0] | ADC_SQR1_NUM_CH(grpp->num_channels / 2);
  adcp->adcs->SQR2  = grpp->ssqr[1];
  adcp->adcs->SQR3  = grpp->ssqr[2];
  adcp->adcs->SQR4  = grpp->ssqr[3];

#else /* !STM32_ADC_DUAL_MODE */
  adcp->adcm->SMPR1 = grpp->smpr[0];
  adcp->adcm->SMPR2 = grpp->smpr[1];
  adcp->adcm->SQR1  = grpp->sqr[0] | ADC_SQR1_NUM_CH(grpp->num_channels);
  adcp->adcm->SQR2  = grpp->sqr[1];
  adcp->adcm->SQR3  = grpp->sqr[2];
  adcp->adcm->SQR4  = grpp->sqr[3];
#endif /* !STM32_ADC_DUAL_MODE */

  /* ADC configuration.*/
  adcp->adcm->CFGR  = cfgr;
#if (STM32_ADCV3_OVERSAMPLING == TRUE) || defined(__DOXYGEN__)
  adcp->adcm->CFGR2 = grpp->cfgr2;
#endif

  /* Starting conversion.*/
  adcp->adcm->CR   |= ADC_CR_ADSTART;

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 *
 * @notapi
 */
void adc_lld_stop_conversion(hal_adc_driver_c *adcp) {

  dmaStreamDisable(adcp->dmastp);
  adc_lld_stop_adc(adcp);
}

/**
 * @brief   Enables the VREFEN bit.
 * @details The VREFEN bit is required in order to sample the VREF channel.
 * @note    This is an STM32-only functionality.
 * @note    This function is meant to be called after @p drvStart().
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 *
 * @notapi
 */
void adcSTM32EnableVREF(hal_adc_driver_c *adcp) {

  adcp->adcc->CCR |= ADC_CCR_VREFEN;
}

/**
 * @brief   Disables the VREFEN bit.
 * @details The VREFEN bit is required in order to sample the VREF channel.
 * @note    This is an STM32-only functionality.
 * @note    This function is meant to be called after @p drvStart().
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 *
 * @notapi
 */
void adcSTM32DisableVREF(hal_adc_driver_c *adcp) {

  adcp->adcc->CCR &= ~ADC_CCR_VREFEN;
}

/**
 * @brief   Enables the TSEN bit.
 * @details The TSEN bit is required in order to sample the internal
 *          temperature sensor and internal reference voltage.
 * @note    This is an STM32-only functionality.
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 *
 * @notapi
 */
void adcSTM32EnableTS(hal_adc_driver_c *adcp) {

  adcp->adcc->CCR |= ADC_CCR_TSEN;
}

/**
 * @brief   Disables the TSEN bit.
 * @details The TSEN bit is required in order to sample the internal
 *          temperature sensor and internal reference voltage.
 * @note    This is an STM32-only functionality.
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 *
 * @notapi
 */
void adcSTM32DisableTS(hal_adc_driver_c *adcp) {

  adcp->adcc->CCR &= ~ADC_CCR_TSEN;
}

/**
 * @brief   Enables the VBATEN bit.
 * @details The VBATEN bit is required in order to sample the VBAT channel.
 * @note    This is an STM32-only functionality.
 * @note    This function is meant to be called after @p drvStart().
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 *
 * @notapi
 */
void adcSTM32EnableVBAT(hal_adc_driver_c *adcp) {

  adcp->adcc->CCR |= ADC_CCR_VBATEN;
}

/**
 * @brief   Disables the VBATEN bit.
 * @details The VBATEN bit is required in order to sample the VBAT channel.
 * @note    This is an STM32-only functionality.
 * @note    This function is meant to be called after @p drvStart().
 *
 * @param[in] adcp      pointer to the @p hal_adc_driver_c object
 *
 * @notapi
 */
void adcSTM32DisableVBAT(hal_adc_driver_c *adcp) {

  adcp->adcc->CCR &= ~ADC_CCR_VBATEN;
}

#endif /* HAL_USE_ADC */

/** @} */

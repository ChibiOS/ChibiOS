/*
 * Licensed under ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file    SPC5xx/EQADC_v1/adc_lld.c
 * @brief   SPC5xx low level ADC driver code.
 *
 * @addtogroup ADC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

/* Some forward declarations.*/
static void adc_serve_rfifo_irq(edma_channel_t channel, void *p);
static void adc_serve_rfifo_error_irq(edma_channel_t channel, void *p);
static void adc_serve_cfifo_error_irq(edma_channel_t channel, void *p);

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief Calibration constant.
 * @details Ideal conversion result for 75%(VRH - VRL) minus 2.
 */
#define ADC_IDEAL_RES75_2       12286

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   ADCD1 driver identifier.
 */
#if SPC5_ADC_USE_ADC0_Q0 || defined(__DOXYGEN__)
ADCDriver ADCD1;
#endif

/**
 * @brief   ADCD2 driver identifier.
 */
#if SPC5_ADC_USE_ADC0_Q1 || defined(__DOXYGEN__)
ADCDriver ADCD2;
#endif

/**
 * @brief   ADCD3 driver identifier.
 */
#if SPC5_ADC_USE_ADC0_Q2 || defined(__DOXYGEN__)
ADCDriver ADCD3;
#endif

/**
 * @brief   ADCD4 driver identifier.
 */
#if SPC5_ADC_USE_ADC1_Q3 || defined(__DOXYGEN__)
ADCDriver ADCD4;
#endif

/**
 * @brief   ADCD5 driver identifier.
 */
#if SPC5_ADC_USE_ADC1_Q4 || defined(__DOXYGEN__)
ADCDriver ADCD5;
#endif

/**
 * @brief   ADCD6 driver identifier.
 */
#if SPC5_ADC_USE_ADC1_Q5 || defined(__DOXYGEN__)
ADCDriver ADCD6;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const uint16_t pudcrs[8] = SPC5_ADC_PUDCR;

#if SPC5_ADC_USE_ADC0_Q0 || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for EQADC CFIFO0.
 */
static const edma_channel_config_t adc_cfifo0_dma_config = {
  0, SPC5_ADC_FIFO0_DMA_PRIO, SPC5_ADC0_FIFO0_DMA_IRQ_PRIO,
  NULL, adc_serve_cfifo_error_irq, NULL
};

/**
 * @brief   DMA configuration for EQADC RFIFO0.
 */
static const edma_channel_config_t adc_rfifo0_dma_config = {
  1, SPC5_ADC_FIFO0_DMA_PRIO, SPC5_ADC0_FIFO0_DMA_IRQ_PRIO,
  adc_serve_rfifo_irq, adc_serve_rfifo_error_irq, NULL
};
#endif /* SPC5_ADC_USE_ADC0_Q3 */

#if SPC5_ADC_USE_ADC1_Q3 || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for EQADC CFIFO3.
 */
static const edma_channel_config_t adc_cfifo3_dma_config = {
  0, SPC5_ADC_FIFO3_DMA_PRIO, SPC5_ADC0_FIFO3_DMA_IRQ_PRIO,
  NULL, adc_serve_cfifo_error_irq, NULL
};

/**
 * @brief   DMA configuration for EQADC RFIFO3.
 */
static const edma_channel_config_t adc_rfifo3_dma_config = {
  1, SPC5_ADC_FIFO3_DMA_PRIO, SPC5_ADC0_FIFO3_DMA_IRQ_PRIO,
  adc_serve_rfifo_irq, adc_serve_rfifo_error_irq, NULL
};
#endif /* SPC5_ADC_USE_ADC1_Q3 */

/*===========================================================================*/
/* Driver local functions and macros.                                        */
/*===========================================================================*/

/**
 * @brief   Enables a CFIFO.
 *
 * @param[in] fifo      the FIFO identifier
 * @param[in] cfcr      CFCR register value
 * @param[in] idcr      IDCR register value
 *
 * @notapi
 */
static void cfifo_enable(adcfifo_t fifo, uint16_t cfcr, uint16_t idcr) {

  EQADC.CFCR[fifo].R = cfcr;
  EQADC.IDCR[fifo].R = idcr;
}


/**
 * @brief   Disables a CFIFO and the associated resources.
 *
 * @param[in] fifo      the FIFO identifier
 *
 * @notapi
 */
static void cfifo_disable(adcfifo_t fifo) {

  /* Disables the CFIFO.*/
  EQADC.CFCR[fifo].R = EQADC_CFCR_MODE_DISABLED;

  /* Disables Interrupts and DMAs of the CFIFO.*/
  EQADC.IDCR[fifo].R = 0;

  /* Waits for the CFIFO to become idle.*/
  while ((EQADC.CFSR.R & (0xC0000000 >> (fifo * 2))) != 0)
    ;

  /* Invalidates the CFIFO.*/
  EQADC.CFCR[fifo].R = EQADC_CFCR_CFINV | EQADC_CFCR_MODE_DISABLED;

  /* Clears all Interrupts and eDMA flags for the CFIFO.*/
  EQADC.FISR[fifo].R = EQADC_FISR_CLEAR_MASK;

  /* Clears the Tx Count Registers for the CFIFO.*/
  EQADC.CFTCR[fifo].R = 0;
}

/**
 * @brief   Pushes a command into the CFIFO0.
 *
 * @param[in] cmd       the command
 *
 * @notapi
 */
static void cfifo0_push_command(adccommand_t cmd) {

  while (EQADC.FISR[0].B.CFCTR >= 4)
    ;
  EQADC.CFPR[0].R = cmd;
}

/**
 * @brief   Waits until the RFIFO0 contains the specified number of entries.
 *
 * @param[in] n     number of entries
 *
 * @notapi
 */
static void cfifo0_wait_rfifo(uint32_t n) {

  while (EQADC.FISR[0].B.RFCTR < n)
    ;
  EQADC.FISR[0].R = EQADC_FISR_CLEAR_MASK;
}

/**
 * @brief   Reads a sample from the RFIFO0.
 *
 * @notapi
 */
#define rfifo0_get_value() (EQADC.RFPR[0].R)

/**
 * @brief   Writes an internal ADC register.
 *
 * @param[in] adc       the ADC unit
 * @param[in] reg       the register index
 * @param[in] value     value to be written into the register
 *
 * @notapi
 */
#define adc_write_register(adc, reg, value)                                 \
  cfifo0_push_command(EQADC_RW_WRITE | (adc) | EQADC_RW_REG_ADDR(reg) |     \
                      EQADC_RW_VALUE(value))


/**
 * @brief   Enables both ADCs.
 *
 * @notapi
 */
static void adc_enable(void) {

  /* Both ADCs must be enabled because this sentence in the reference manual:
    "Both ADC0 and ADC1 of an eQADC module pair must be enabled before
     calibrating or using either ADC0 or ADC1 of the pair. Failure to
     enable both ADC0 and ADC1 of the pair can result in inaccurate
     conversions.".*/
  adc_write_register(EQADC_RW_BN_ADC0, ADC_REG_CR,
                     SPC5_ADC_CR_CLK_PS | ADC_CR_EN);
  adc_write_register(EQADC_RW_BN_ADC1, ADC_REG_CR,
                     SPC5_ADC_CR_CLK_PS | ADC_CR_EN);
}

/**
 * @brief   Disables both ADCs.
 *
 * @notapi
 */
static void adc_disable(void) {

  adc_write_register(EQADC_RW_BN_ADC0, ADC_REG_CR,
                     SPC5_ADC_CR_CLK_PS);
  adc_write_register(EQADC_RW_BN_ADC1, ADC_REG_CR,
                     SPC5_ADC_CR_CLK_PS);
}

/**
 * @brief   Calibrates an ADC unit.
 *
 * @param[in] adc       the ADC unit
 *
 * @notapi
 */
static void adc_calibrate(uint32_t adc) {
  uint16_t res25, res75;
  uint32_t gcc, occ;

  /* Starts the calibration, write command messages to sample 25% and
     75% VREF.*/
  cfifo0_push_command(0x00002C00 | adc);      /* Vref 25%.*/
  cfifo0_push_command(0x00002B00 | adc);      /* Vref 75%.*/
  cfifo0_wait_rfifo(2);

  /* Reads the results and compute calibration register values.*/
  res25 = rfifo0_get_value();
  res75 = rfifo0_get_value();

  gcc = 0x08000000UL / ((uint32_t)res75 - (uint32_t)res25);
  occ = (uint32_t)ADC_IDEAL_RES75_2 - ((gcc * (uint32_t)res75) >> 14);

  /* Loads the gain and offset values (default configuration, 12 bits).*/
  adc_write_register(adc, ADC_REG_GCCR, gcc);
  adc_write_register(adc, ADC_REG_OCCR, occ & 0xFFFF);

  /* Loads gain and offset values (alternate configuration 1, 10 bits).*/
  adc_write_register(adc, ADC_REG_AC1GCCR, gcc);
  adc_write_register(adc, ADC_REG_AC1OCCR, occ & 0xFFFF);

  /* Loads gain and offset values (alternate configuration 1, 8 bits).*/
  adc_write_register(adc, ADC_REG_AC2GCCR, gcc);
  adc_write_register(adc, ADC_REG_AC2OCCR, occ & 0xFFFF);
}

/**
 * @brief   Calibrates an ADC unit.
 *
 * @param[in] adc       the ADC unit
 *
 * @notapi
 */
static void adc_setup_resistors(uint32_t adc) {
  unsigned i;

  for (i = 0; i < 8; i++)
    adc_write_register(adc, ADC_REG_PUDCR(i), pudcrs[i]);
}

/**
 * @brief   Shared ISR for RFIFO DMA events.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 *
 * @notapi
 */
static void adc_serve_rfifo_irq(edma_channel_t channel, void *p) {

  (void)channel;
  (void)p;
}

/**
 * @brief   Shared ISR for RFIFO DMA error events.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 *
 * @notapi
 */
static void adc_serve_rfifo_error_irq(edma_channel_t channel, void *p) {

  (void)channel;
  (void)p;
}

/**
 * @brief   Shared ISR for CFIFO DMA error events.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 *
 * @notapi
 */
static void adc_serve_cfifo_error_irq(edma_channel_t channel, void *p) {

  (void)channel;
  (void)p;
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

#if SPC5_ADC_USE_ADC0_Q0
  /* Driver initialization.*/
  adcObjectInit(&ADCD1);
  ADCD1.cfifo_channel = EDMA_ERROR;
  ADCD1.rfifo_channel = EDMA_ERROR;
#endif /* SPC5_ADC_USE_EQADC_Q0 */

#if SPC5_ADC_USE_ADC1_Q3
  /* Driver initialization.*/
  adcObjectInit(&ADCD1);
  ADCD1.cfifo_channel = EDMA_ERROR;
  ADCD1.rfifo_channel = EDMA_ERROR;
#endif /* SPC5_ADC_USE_ADC1_Q3 */

  /* Temporarily enables CFIFO0 for calibration and initialization.*/
  cfifo_enable(ADC_FIFO_0, EQADC_CFCR_SSE | EQADC_CFCR_MODE_SWCS, 0);
  adc_enable();

  /* Calibration of both ADC units, programming alternate configs
     one and two for 10 and 8 bits operations, setting up pull up/down
     resistors.*/
#if SPC5_ADC_USE_ADC0
  adc_calibrate(EQADC_RW_BN_ADC0);
  adc_write_register(EQADC_RW_BN_ADC0, ADC_REG_AC1CR, ADC_ACR_RESSEL_10BITS);
  adc_write_register(EQADC_RW_BN_ADC0, ADC_REG_AC2CR, ADC_ACR_RESSEL_8BITS);
  adc_setup_resistors(EQADC_RW_BN_ADC0);
#endif
#if SPC5_ADC_USE_ADC1
  adc_calibrate(EQADC_RW_BN_ADC1);
  adc_write_register(EQADC_RW_BN_ADC1, ADC_REG_AC1CR, ADC_ACR_RESSEL_10BITS);
  adc_write_register(EQADC_RW_BN_ADC1, ADC_REG_AC2CR, ADC_ACR_RESSEL_8BITS);
  adc_setup_resistors(EQADC_RW_BN_ADC1);
#endif

  /* ADCs disabled until the driver is started by the application.*/
  adc_disable();
  cfifo_disable(ADC_FIFO_0);
}

/**
 * @brief   Configures and activates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_start(ADCDriver *adcp) {

  if (adcp->state == ADC_STOP) {
    /* Enables the peripheral.*/
#if SPC5_ADC_USE_ADC0_Q0
    if (&ADCD1 == adcp) {
      adcp->cfifo_channel = edmaChannelAllocate(&adc_cfifo0_dma_config);
      adcp->rfifo_channel = edmaChannelAllocate(&adc_rfifo0_dma_config);
    }
#endif /* SPC5_ADC_USE_EQADC_Q0 */
#if SPC5_ADC_USE_ADC1_Q3
    if (&ADCD4 == adcp) {
      adcp->cfifo_channel = edmaChannelAllocate(&adc_cfifo3_dma_config);
      adcp->rfifo_channel = edmaChannelAllocate(&adc_rfifo3_dma_config);
    }
#endif /* SPC5_ADC_USE_ADC1_Q3 */
  }

  chDbgAssert((adcp->cfifo_channel != EDMA_ERROR) &&
              (adcp->rfifo_channel != EDMA_ERROR),
              "adc_lld_start(), #1", "channel cannot be allocated");

  /* Configures the peripheral.*/

}

/**
 * @brief   Deactivates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop(ADCDriver *adcp) {

  if (adcp->state == ADC_READY) {
    /* Resets the peripheral.*/

    /* Releases the allocated EDMA channels.*/
    edmaChannelRelease(adcp->cfifo_channel);
    edmaChannelRelease(adcp->rfifo_channel);

    /* Disables the peripheral.*/
#if SPC5_ADC_USE_ADC0_Q0
    if (&ADCD1 == adcp) {
    }
#endif /* SPC5_ADC_USE_ADC0_Q0 */
#if SPC5_ADC_USE_ADC1_Q3
    if (&ADCD1 == adcp) {
    }
#endif /* SPC5_ADC_USE_ADC1_Q3 */
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

  (void)adcp;
}

/**
 * @brief   Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop_conversion(ADCDriver *adcp) {

  (void)adcp;
}

#endif /* HAL_USE_ADC */

/** @} */

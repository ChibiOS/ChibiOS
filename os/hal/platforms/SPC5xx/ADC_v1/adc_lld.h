/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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
 * @file    SPC5xx/ADC_v1/adc_lld.h
 * @brief   ADC Driver subsystem low level driver header.
 *
 * @addtogroup ADC
 * @{
 */

#ifndef _ADC_LLD_H_
#define _ADC_LLD_H_

#if HAL_USE_ADC || defined(__DOXYGEN__)

#include "spc5_adc.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Analog channel identifiers
 * @{
 */
#if SPC5_HAS_ADC0 || defined(__DOXYGEN__)
#define ADC0_CHN_AN0                    0U
#define ADC0_CHN_AN1                    1U
#define ADC0_CHN_AN2                    2U
#define ADC0_CHN_AN3                    3U
#define ADC0_CHN_AN4                    4U
#define ADC0_CHN_AN5                    5U
#define ADC0_CHN_AN6                    6U
#define ADC0_CHN_AN7                    7U
#define ADC0_CHN_AN8                    8U
#define ADC0_CHN_AN9                    9U
#define ADC0_CHN_AN10                   10U
#define ADC0_CHN_AN11                   11U
#define ADC0_CHN_AN12                   12U
#define ADC0_CHN_AN13                   13U
#define ADC0_CHN_AN14                   14U
#define ADC0_CHN_AN15                   15U
#endif

#if SPC5_HAS_ADC1 || defined(__DOXYGEN__)
#define ADC1_CHN_AN0                    0U
#define ADC1_CHN_AN1                    1U
#define ADC1_CHN_AN2                    2U
#define ADC1_CHN_AN3                    3U
#define ADC1_CHN_AN4                    4U
#define ADC1_CHN_AN5                    5U
#define ADC1_CHN_AN6                    6U
#define ADC1_CHN_AN7                    7U
#define ADC1_CHN_AN8                    8U
#define ADC1_CHN_AN9                    9U
#define ADC1_CHN_AN10                   10U
#define ADC1_CHN_AN11                   11U
#define ADC1_CHN_AN12                   12U
#define ADC1_CHN_AN13                   13U
#define ADC1_CHN_AN14                   14U
#define ADC1_CHN_AN15                   15U
#endif
/** @} */

/**
 * @name    ADC MCR register definitions
 * @{
 */
#define ADC_MCR_OWREN                   (1U << 31)
#define ADC_MCR_WLSIDE                  (1U << 30)
#define ADC_MCR_MODE                    (1U << 29)
#define ADC_MCR_NSTART                  (1U << 24)
#define ADC_MCR_JTRGEN                  (1U << 22)
#define ADC_MCR_JEDGE                   (1U << 21)
#define ADC_MCR_JSTART                  (1U << 20)
#define ADC_MCR_CTUEN                   (1U << 17)
#define ADC_MCR_ADCLKSEL                (1U << 8)
#define ADC_MCR_ABORTCHAIN              (1U << 7)
#define ADC_MCR_ABORT                   (1U << 6)
#define ADC_MCR_ACKO                    (1U << 5)
#define ADC_MCR_PWDN                    (1U << 0)
/** @} */

/**
 * @name    ADC MSR register definitions
 * @{
 */
#define ADC_MSR_NSTART                  (1U << 24)
#define ADC_MSR_JABORT                  (1U << 23)
#define ADC_MSR_JSTART                  (1U << 20)
#define ADC_MSR_CTUSTART                (1U << 16)
#define ADC_MSR_CHADDR                  (1U << 9)
#define ADC_MSR_ACKO                    (1U << 5)
#define ADC_MSR_ADCSTATUS               (1U << 0)
/** @} */

/**
 * @name    ADC ISR register definitions
 * @{
 */
#define ADC_ISR_EOCTU                   (1U << 4)
#define ADC_ISR_JEOC                    (1U << 3)
#define ADC_ISR_JECH                    (1U << 2)
#define ADC_ISR_EOC                     (1U << 1)
#define ADC_ISR_ECH                     (1U << 0)
/** @} */

/**
 * @name    ADC IMR register definitions
 * @{
 */
#define ADC_IMR_MSKEOCTU                (1U << 4)
#define ADC_IMR_MSKJEOC                 (1U << 3)
#define ADC_IMR_MSKJECH                 (1U << 2)
#define ADC_IMR_MSKEOC                  (1U << 1)
#define ADC_IMR_MSKECH                  (1U << 0)
/** @} */

/**
 * @name    ADC DMAE register definitions
 * @{
 */
#define ADC_DMAE_DCLR                   (1U << 1)
#define ADC_DMAE_DMAEN                  (1U << 0)
/** @} */

/**
 * @name    ADC CDR register definitions
 * @{
 */
#define ADC_CDR_VALID                   (1U << 19)
#define ADC_CDR_OVERW                   (1U << 18)
#define ADC_CDR_RESULT                  (1U << 16)
#define ADC_CDR_CDATA_LEFT              (1U << 6)
#define ADC_CDR_CDATA_RIGHT             (1U << 0)
/** @} */

/**
 * @name    ADC Wathdog ISR definitions
 * @{
 */
#define ADC_ISR_AWD3_HT                 (1U << 7)
#define ADC_ISR_AWD2_HT                 (1U << 6)
#define ADC_ISR_AWD1_HT                 (1U << 5)
#define ADC_ISR_AWD0_HT                 (1U << 4)
#define ADC_ISR_AWD3_LT                 (1U << 3)
#define ADC_ISR_AWD2_LT                 (1U << 2)
#define ADC_ISR_AWD1_LT                 (1U << 1)
#define ADC_ISR_AWD0_LT                 (1U << 0)
/** @} */


/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
#if SPC5_HAS_ADC0 || defined(__DOXYGEN__)
/**
 * @brief   ADCD1 driver enable switch.
 * @details If set to @p TRUE the support for ADC0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ADC_USE_ADC0) || defined(__DOXYGEN__)
#define SPC5_ADC_USE_ADC0                   FALSE
#endif

/**
 * @brief   ADC0 EOC interrupt priority level setting.
 */
#if !defined(SPC5_ADC_ADC0_EOC_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ADC_ADC0_EOC_PRIORITY          12
#endif

/**
 * @brief   ADC0 ER interrupt priority level setting.
 */
#if !defined(SPC5_ADC_ADC0_ER_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ADC_ADC0_ER_PRIORITY           12
#endif

/**
 * @brief   ADC0 WD interrupt priority level setting.
 */
#if !defined(SPC5_ADC_ADC0_WD_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ADC_ADC0_WD_PRIORITY           12
#endif

/**
 * @brief   ADC0 DMA IRQ priority.
 */
#if !defined(SPC5_ADC_ADC0_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_ADC_ADC0_DMA_IRQ_PRIO          12
#endif

/**
 * @brief   ADC0 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_ADC_ADC0_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_ADC_ADC0_START_PCTL            (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   ADC0 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_ADC_ADC0_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_ADC_ADC0_STOP_PCTL             (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif
#endif

#if SPC5_HAS_ADC1 || defined(__DOXYGEN__)
/**
 * @brief   ADCD2 driver enable switch.
 * @details If set to @p TRUE the support for ADC1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ADC_USE_ADC1) || defined(__DOXYGEN__)
#define SPC5_ADC_USE_ADC1                   FALSE
#endif

/**
 * @brief   ADC1 EOC interrupt priority level setting.
 */
#if !defined(SPC5_ADC_ADC1_EOC_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ADC_ADC1_EOC_PRIORITY          12
#endif

/**
 * @brief   ADC1 ER interrupt priority level setting.
 */
#if !defined(SPC5_ADC_ADC1_ER_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ADC_ADC1_ER_PRIORITY           12
#endif

/**
 * @brief   ADC1 WD interrupt priority level setting.
 */
#if !defined(SPC5_ADC_ADC1_WD_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ADC_ADC1_WD_PRIORITY           12
#endif

/**
 * @brief   ADC1 DMA IRQ priority.
 */
#if !defined(SPC5_ADC_ADC1_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_ADC_ADC1_DMA_IRQ_PRIO          12
#endif

/**
 * @brief   ADC1 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_ADC_ADC1_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_ADC_ADC1_START_PCTL            (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   ADC1 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_ADC_ADC1_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_ADC_ADC1_STOP_PCTL             (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !SPC5_HAS_ADC0
#error "ADC0 not present in the selected device"
#endif

#if !SPC5_HAS_ADC1
#error "ADC1 not present in the selected device"
#endif

#if !SPC5_ADC_USE_ADC0 && !SPC5_ADC_USE_ADC1
#error "ADC driver activated but no ADC peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   ADC clock frequency.
 */
typedef enum {
  HALF_PERIPHERAL_SET_CLOCK_FREQUENCY = 0,  /**< ADC clock frequency is half Peripheral Set Clock frequency.    */
  PERIPHERAL_SET_CLOCK_FREQUENCY = 1        /**< ADC clock frequency is equal to Peripheral Set Clock frequency.    */
} adc_clock;

/**
 * @brief   ADC sample data type.
 */
typedef uint16_t adcsample_t;

/**
 * @brief   Channels number in a conversion group.
 */
typedef uint16_t adc_channels_num_t;

/**
 * @brief   Possible ADC failure causes.
 * @note    Error codes are architecture dependent and should not relied
 *          upon.
 */
typedef enum {
  ADC_ERR_DMAFAILURE = 0,                   /**< DMA operations failure.                    */
  ADC_ERR_AWD0_HT = 1,                      /**< Watchdog 0 triggered Higher Threshold.     */
  ADC_ERR_AWD0_LT = 2,                      /**< Watchdog 0 triggered Lower Threshold.      */
  ADC_ERR_AWD1_HT = 3,                      /**< Watchdog 1 triggered Higher Threshold.     */
  ADC_ERR_AWD1_LT = 4,                      /**< Watchdog 1 triggered Lower Threshold.      */
  ADC_ERR_AWD2_HT = 5,                      /**< Watchdog 2 triggered Higher Threshold.     */
  ADC_ERR_AWD2_LT = 6,                      /**< Watchdog 2 triggered Lower Threshold.      */
  ADC_ERR_AWD3_HT = 7,                      /**< Watchdog 3 triggered Higher Threshold.     */
  ADC_ERR_AWD3_LT = 8,                      /**< Watchdog 3 triggered Lower Threshold.      */
} adcerror_t;

/**
 * @brief   Type of a structure representing an ADC driver.
 */
typedef struct ADCDriver ADCDriver;

/**
 * @brief   ADC notification callback type.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object triggering the
 *                      callback
 * @param[in] buffer    pointer to the most recent samples data
 * @param[in] n         number of buffer rows available starting from @p buffer
 */
typedef void (*adccallback_t)(ADCDriver *adcp, adcsample_t *buffer, size_t n);

/**
 * @brief   ADC error callback type.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object triggering the
 *                      callback
 * @param[in] err       ADC error code
 */
typedef void (*adcerrorcallback_t)(ADCDriver *adcp, adcerror_t err);

/**
 * @brief   Conversion group configuration structure.
 * @details This implementation-dependent structure describes a conversion
 *          operation.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
typedef struct {
  /**
   * @brief   Enables the circular buffer mode for the group.
   */
  bool_t                    circular;
  /**
   * @brief   Number of the analog channels belonging to the conversion group.
   */
  adc_channels_num_t        num_channels;
  /**
   * @brief   Callback function associated to the group or @p NULL.
   */
  adccallback_t             end_cb;
  /**
   * @brief   Error callback or @p NULL.
   */
  adcerrorcallback_t        error_cb;
  /* End of the mandatory fields.*/
  /**
   * @brief   ADC WTIMR register initialization data.
   */
  uint32_t                  wtimr;
  /**
   * @brief   ADC TRCx register initialization data.
   */
  uint32_t                  trcr[4];
  /**
   * @brief   ADC THRHLRx register initialization data.
   */
  uint32_t                  thrhlr[4];
  /**
   * @brief   ADC CTR0 register initialization data.
   */
  uint32_t                  ctr;
  /**
   * @brief   Only the conversion of contiguous channels is implemented.
   *          Specify initial and final conversion channels.
   */
  /**
   * @brief   ADC Initial conversion channel.
   */
  uint32_t                  init_channel;
  /**
   * @brief   ADC Final conversion channel.
   */
  uint32_t                  final_channel;
} ADCConversionGroup;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief   Analog clock frequency.
   */
  adc_clock                 clock;
} ADCConfig;

/**
 * @brief   Structure representing an ADC driver.
 */
struct ADCDriver {
  /**
   * @brief   Driver state.
   */
  adcstate_t                state;
  /**
   * @brief   Current configuration data.
   */
  const ADCConfig           *config;
  /**
   * @brief   Current samples buffer pointer or @p NULL.
   */
  adcsample_t               *samples;
  /**
   * @brief   Current samples buffer depth or @p 0.
   */
  size_t                    depth;
  /**
   * @brief   Current conversion group pointer or @p NULL.
   */
  const ADCConversionGroup  *grpp;
#if ADC_USE_WAIT || defined(__DOXYGEN__)
  /**
   * @brief   Waiting thread.
   */
  thread_reference_t        thread;
#endif
#if ADC_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
  /**
   * @brief   Mutex protecting the peripheral.
   */
  mutex_t                   mutex;
#endif /* ADC_USE_MUTUAL_EXCLUSION */
#if defined(ADC_DRIVER_EXT_FIELDS)
  ADC_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief   EDMA channel used for the ADC.
   */
  edma_channel_t            adc_dma_channel;
  /**
   * @brief Pointer to the ADCx registers block.
   */
  volatile struct spc5_adc  *adc_tagp;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if SPC5_ADC_USE_ADC0 && !defined(__DOXYGEN__)
extern ADCDriver ADCD1;
#endif

#if SPC5_ADC_USE_ADC1 && !defined(__DOXYGEN__)
extern ADCDriver ADCD2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void adc_lld_init(void);
  void adc_lld_start(ADCDriver *adcp);
  void adc_lld_stop(ADCDriver *adcp);
  void adc_lld_start_conversion(ADCDriver *adcp);
  void adc_lld_stop_conversion(ADCDriver *adcp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_ADC */

#endif /* _ADC_LLD_H_ */

/** @} */

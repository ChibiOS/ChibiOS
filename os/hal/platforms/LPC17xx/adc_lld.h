/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC17xx ADC driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC17xx/adc_lld.h
 * @brief   LPC17xx ADC subsystem low level driver header.
 * @note    Values in samples buffer are from DR register.
 *          To get ADC values make conversion (DR >> 6) & 0x03FF.
 * @addtogroup ADC
 * @{
 */

#ifndef _ADC_LLD_H_
#define _ADC_LLD_H_

#if HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define ADC0STAT_DONE_MASK         0x000000FF
#define ADC0STAT_OVERRUN_MASK      0x0000FF00

#define AD0CR_PDN                  (1UL << 21)
/**
 * @name    Absolute Maximum Ratings
 * @{
 */

/**
 * @brief   Maximum ADC clock frequency.
 */
#define LPC17xx_ADCCLK_MAX          13000000

/**
 * @brief   Available number of ADC channels.
 */
#define ADC_MAX_CHANNELS            8
/** @} */

/**
 * @name    ADC  settings
 * @{
 */

/**
 * @name    Available analog channels
 * @note    In software-controlled mode only
 *          one channel can be selected.
 * @{
 */
#define AD0CR_CHANNEL0              (1UL << 0)
#define AD0CR_CHANNEL1              (1UL << 1)
#define AD0CR_CHANNEL2              (1UL << 2)
#define AD0CR_CHANNEL3              (1UL << 3)
#define AD0CR_CHANNEL4              (1UL << 4)
#define AD0CR_CHANNEL5              (1UL << 5)
#define AD0CR_CHANNEL6              (1UL << 6)
#define AD0CR_CHANNEL7              (1UL << 7)
/** @} */

/**
 * @name    ADC mode types
 * @note    In software-controlled mode only one conversion
 *          is make
 * @{
 */
#define AD0CR_MODE_SOFT             (0UL  << 16)
#define AD0CR_MODE_BURST            (1UL  << 16)
/** @} */

/**
 * @name    Triggers selection
 * @note    Only use in software-controlled mode
 * @{
 */
#define AD0CR_START_NO              (0UL << 24)
#define AD0CR_START_NOW             (1UL << 24)
#define AD0CR_START_CT16B0_CAP0     (2UL << 24)
#define AD0CR_START_CT32B0_CAP0     (3UL << 24)
#define AD0CR_START_CT32B0_MAT0     (4UL << 24)
#define AD0CR_START_CT32B0_MAT1     (5UL << 24)
#define AD0CR_START_CT16B0_MAT0     (6UL << 24)
#define AD0CR_START_CT16B0_MAT1     (7UL << 24)
#define AD0CR_START_MASK            (7UL << 24)
/** @} */

/**
 * @name    Trigger edge type selection
 * @note    Only use in software-controlled mode.
 * @{
 */
#define AD0CR_EDGE_RISSING           (0UL << 27)
#define AD0CR_EDGE_FALLING           (1UL << 27)
/** @} */
/** @} */
/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   ADC common clock divider.
 */
#if !defined(LPC17xx_ADC_CLKDIV) || defined(__DOXYGEN__)
#define LPC17xx_ADC_CLKDIV                  12
#endif

#if LPC17xx_PCLK/LPC17xx_ADC_CLKDIV > LPC17xx_ADCCLK_MAX
#error "ADC clock frequency out of the acceptable range (13MHz max)"
#endif

/**
 * @brief   ADC interrupt priority level setting.
 */
#if !defined(LPC17xx_ADC_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC17xx_ADC_IRQ_PRIORITY            3
#endif

/**
 * @brief   ADC DMA enable switch.
 * @details If set to @p TRUE the support for ADC DMA is included.
 * @note    ADC DMA only support one selected channel. The default is @p FALSE.
 */
#if !defined(LPC17xx_ADC_USE_DMA) || defined(__DOXYGEN__)
#define LPC17xx_ADC_USE_DMA                  FALSE
#endif

#if LPC17xx_ADC_USE_DMA || defined(__DOXYGEN__)
#if !defined(LPC17xx_DMA_REQUIRED)
#define LPC17xx_DMA_REQUIRED
#endif
#endif

/**
 * @brief   ADC DMA channel.
 */
#if !defined(LPC17xx_ADC_DMA_CHANNEL) || defined(__DOXYGEN__)
#define LPC17xx_ADC_DMA_CHANNEL                 DMA_CHANNEL6
#endif
/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   ADC sample data type.
 */
typedef uint32_t adcsample_t;

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
  ADC_ERR_OVERRUN = 1                      /**< ADC overrun condition.    */
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
 * @note    The use of this configuration structure requires knowledge of
 *          LPC17xx ADC cell registers interface, please refer to the LPC17xx
 *          reference manual for details.
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
   * @brief   ADC CR0 register initialization data.
   * @note    All the required bits must be defined into this field.
   */
  uint32_t                  cr0;
  /**
   * @brief   ADC INTENT register initialization data.
   * @note    In interrupt burst mode only define interrupt for
   *          last enabled channel.
   */
  uint32_t                  inten;
} ADCConversionGroup;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  uint32_t                  dummy;
} ADCConfig;

/**
 * @brief   Structure representing an ADC driver.
 */
struct ADCDriver {
  /**
   * @brief Driver state.
   */
  adcstate_t                state;
  /**
   * @brief Current configuration data.
   */
  const ADCConfig           *config;
  /**
   * @brief Current samples buffer pointer or @p NULL.
   */
  adcsample_t               *samples;
  /**
   * @brief Current samples buffer depth or @p 0.
   */
  size_t                    depth;
  /**
   * @brief Current conversion group pointer or @p NULL.
   */
  const ADCConversionGroup  *grpp;
#if ADC_USE_WAIT || defined(__DOXYGEN__)
  /**
   * @brief Waiting thread.
   */
  Thread                    *thread;
#endif
#if ADC_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
#if CH_USE_MUTEXES || defined(__DOXYGEN__)
  /**
   * @brief Mutex protecting the peripheral.
   */
  Mutex                     mutex;
#elif CH_USE_SEMAPHORES
  Semaphore                 semaphore;
#endif
#endif /* ADC_USE_MUTUAL_EXCLUSION */
#if defined(ADC_DRIVER_EXT_FIELDS)
  ADC_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the ADCx registers block.
   */
  LPC_ADC_TypeDef           *adc;
  /**
   * @brief Number of all samples in buffer.
   */
  uint32_t                  nsamples;
  /**
   * @brief Samples buffer counter.
   */
  uint32_t                  num;
#if LPC17xx_ADC_USE_DMA
  /**
   * @brief ADC DMA configuration structure.
   */
  lpc17xx_dma_channel_config_t  adc_dma_cfg;
#endif
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/


/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern ADCDriver ADCD1;
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

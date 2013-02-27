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
 * @brief   SPC5xx low level ADC driver header.
 *
 * @addtogroup ADC
 * @{
 */

#ifndef _ADC_LLD_H_
#define _ADC_LLD_H_

#if HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    CFIFO commands
 * @{
 */
#define ADC_CFIFO_CHANNEL_MASK  (0xFFU << 8)/**< @brief Channel number mask.*/
#define ADC_CFIFO_CHANNEL(n)    ((n) << 8)  /**< @brief Channel number.     */
#define ADC_CFIFO_FMT_RJU       (0U << 16)  /**< @brief Unsigned samples.   */
#define ADC_CFIFO_FMT_RJS       (1U << 16)  /**< @brief Signed samples.     */
#define ADC_CFIFO_TSR           (1U << 17)  /**< @brief Time stamp request. */
#define ADC_CFIFO_LST_MASK      (3U << 18)  /**< @brief Sample time.        */
#define ADC_CFIFO_LST_2         (0U << 18)  /**< @brief 2 clock cycles.     */
#define ADC_CFIFO_LST_8         (1U << 18)  /**< @brief 8 clock cycles.     */
#define ADC_CFIFO_LST_64        (2U << 18)  /**< @brief 64 clock cycles.    */
#define ADC_CFIFO_LST_128       (3U << 18)  /**< @brief 128 clock cycles.   */
#define ADC_CFIFO_MSG_MASK      (15U << 20) /**< @brief Message mask.       */
#define ADC_CFIFO_MSG_RFIFO(n)  ((n) << 20) /**< @brief Result in RFIFO0..5.*/
#define ADC_CFIFO_MSG_NULL      (6U << 20)  /**< @brief Null message.       */
#define ADC_CFIFO_CAL           (1U << 24)  /**< @brief Calibrated result.  */
#define ADC_CFIFO_BN_MASK       (1U << 25)  /**< @brief Buffer number mask. */
#define ADC_CFIFO_BN(n)         ((n) << 25) /**< @brief Buffer number.      */
#define ADC_CFIFO_REP           (1U << 29)  /**< @brief Repeat loop flag.   */
#define ADC_CFIFO_PAUSE         (1U << 30)  /**< @brief Pause flag.         */
#define ADC_CFIFO_EOQ           (1U << 31)  /**< @brief End of queue flag.  */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   ADCD10 driver enable switch.
 * @details If set to @p TRUE the support for EQADC1 queue 0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ADC_USE_EQADC1_Q0) || defined(__DOXYGEN__)
#define SPC5_ADC_USE_EQADC1_Q0                 FALSE
#endif

/**
 * @brief   ADCD20 driver enable switch.
 * @details If set to @p TRUE the support for EQADC2 queue 0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ADC_USE_EQADC2_Q0) || defined(__DOXYGEN__)
#define SPC5_ADC_USE_EQADC2_Q0                 FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   ADC command data type.
 */
typedef uint32_t adccommand_t;

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
  ADC_ERR_DMAFAILURE = 0,                   /**< DMA operations failure.    */
  ADC_ERR_OVERFLOW = 1                      /**< ADC overflow condition.    */
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
   * @brief   Number of command iterations stored in @p commands.
   * @note    The total number of array elements must be @p num_channels *
   *          @p num_iterations.
   * @note    This fields is the upper limit of the parameter @p n that can
   *          be passed to the functions @p adcConvert() and
   *          @p adcStartConversion().
   */
  uint32_t                  num_iterations;
  /**
   * @brief   Pointer to an array of low level EQADC commands to be pushed
   *          into the CFIFO during a conversion.
   */
  const adccommand_t        *commands;
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
  Thread                    *thread;
#endif
#if ADC_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
#if CH_USE_MUTEXES || defined(__DOXYGEN__)
  /**
   * @brief   Mutex protecting the peripheral.
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
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if SPC5_ADC_USE_EQADC1_Q0 && !defined(__DOXYGEN__)
extern ADCDriver ADCD10;
#endif

#if SPC5_ADC_USE_EQADC2_Q0 && !defined(__DOXYGEN__)
extern ADCDriver ADCD20;
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

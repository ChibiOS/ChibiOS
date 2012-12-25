/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    STM32F3xx/adc_lld.h
 * @brief   STM32F3xx ADC subsystem low level driver header.
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
 * @name    Sampling rates
 * @{
 */
#define ADC_SMPR_SMP_1P5        0   /**< @brief 14 cycles conversion time   */
#define ADC_SMPR_SMP_2P5        1   /**< @brief 15 cycles conversion time.  */
#define ADC_SMPR_SMP_4P5        2   /**< @brief 17 cycles conversion time.  */
#define ADC_SMPR_SMP_7P5        3   /**< @brief 20 cycles conversion time.  */
#define ADC_SMPR_SMP_19P5       4   /**< @brief 32 cycles conversion time.  */
#define ADC_SMPR_SMP_61P5       5   /**< @brief 74 cycles conversion time.  */
#define ADC_SMPR_SMP_181P5      6   /**< @brief 194 cycles conversion time. */
#define ADC_SMPR_SMP_601P5      7   /**< @brief 614 cycles conversion time. */
/** @} */

/**
 * @name    Resolution
 * @{
 */
#define ADC_CFGR1_RES_12BIT     (0 << 3)
#define ADC_CFGR1_RES_10BIT     (1 << 3)
#define ADC_CFGR1_RES_8BIT      (2 << 3)
#define ADC_CFGR1_RES_6BIT      (3 << 3)
/** @} */

/**
 * @name    Threashold register initializer
 * @{
 */
#define ADC_TR(low, high)       (((uint32_t)(high) << 16) | (uint32_t)(low))
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   ADC1 driver enable switch.
 * @details If set to @p TRUE the support for ADC1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_ADC_USE_ADC1) || defined(__DOXYGEN__)
#define STM32_ADC_USE_ADC1                  FALSE
#endif
/**
 * @brief   ADC1+ADC2 driver enable switch.
 * @details If set to @p TRUE the support for ADC1+ADC2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_ADC_USE_ADC12) || defined(__DOXYGEN__)
#define STM32_ADC_USE_ADC12                 FALSE
#endif
/**
 * @brief   ADC3 driver enable switch.
 * @details If set to @p TRUE the support for ADC3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_ADC_USE_ADC3) || defined(__DOXYGEN__)
#define STM32_ADC_USE_ADC3                  FALSE
#endif
/**
 * @brief   ADC3+ADC4 driver enable switch.
 * @details If set to @p TRUE the support for ADC3+ADC4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_ADC_USE_ADC34) || defined(__DOXYGEN__)
#define STM32_ADC_USE_ADC34                 FALSE
#endif

/**
 * @brief   ADC1/ADC2 DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_ADC_ADC12_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ADC_ADC12_DMA_PRIORITY        2
#endif

/**
 * @brief   ADC3/ADC4 DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_ADC_ADC34_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ADC_ADC34_DMA_PRIORITY        2
#endif

/**
 * @brief   ADC1/ADC2 interrupt priority level setting.
 */
#if !defined(STM32_ADC_ADC12_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ADC_ADC12_IRQ_PRIORITY        2
#endif

/**
 * @brief   ADC3/ADC4 interrupt priority level setting.
 */
#if !defined(STM32_ADC34_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ADC_ADC34_IRQ_PRIORITY        2
#endif

/**
 * @brief   ADC1/ADC2 DMA interrupt priority level setting.
 */
#if !defined(STM32_ADC_ADC12_DMA_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ADC_ADC12_DMA_IRQ_PRIORITY    2
#endif

/**
 * @brief   ADC3/ADC4 DMA interrupt priority level setting.
 */
#if !defined(STM32_ADC_ADC34_DMA_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ADC_ADC34_DMA_IRQ_PRIORITY    2
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if STM32_ADC_USE_ADC1 && !STM32_HAS_ADC1
#error "ADC1 not present in the selected device"
#endif

#if STM32_ADC_USE_ADC12 && (!STM32_HAS_ADC1 || !STM32_HAS_ADC2)
#error "ADC12 not present in the selected device"
#endif

#if STM32_ADC_USE_ADC3 && !STM32_HAS_ADC3
#error "ADC3 not present in the selected device"
#endif

#if STM32_ADC_USE_ADC34 && (!STM32_HAS_ADC3 || !STM32_HAS_ADC4)
#error "ADC34 not present in the selected device"
#endif

#if !STM32_ADC_USE_ADC1 || !STM32_ADC_USE_ADC12 ||                          \
    !STM32_ADC_USE_ADC3 || !STM32_ADC_USE_ADC34
#error "ADC driver activated but no ADC peripheral assigned"
#endif

#if STM32_ADC_USE_ADC1 &&                                                   \
    !CORTEX_IS_VALID_KERNEL_PRIORITY(STM32_ADC_ADC12_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to ADC1"
#endif

#if STM32_ADC_USE_ADC1 &&                                                   \
    !CORTEX_IS_VALID_KERNEL_PRIORITY(STM32_ADC_ADC12_DMA_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to ADC1 DMA"
#endif

#if STM32_ADC_USE_ADC1 &&                                                   \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_ADC_ADC12_DMA_PRIORITY)
#error "Invalid DMA priority assigned to ADC1"
#endif

#if STM32_ADC_USE_ADC12 &&                                                  \
    !CORTEX_IS_VALID_KERNEL_PRIORITY(STM32_ADC_ADC12_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to ADC12"
#endif

#if STM32_ADC_USE_ADC12 &&                                                  \
    !CORTEX_IS_VALID_KERNEL_PRIORITY(STM32_ADC_ADC12_DMA_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to ADC12 DMA"
#endif

#if STM32_ADC_USE_ADC12 &&                                                  \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_ADC_ADC12_DMA_PRIORITY)
#error "Invalid DMA priority assigned to ADC12"
#endif

#if STM32_ADC_USE_ADC3 &&                                                   \
    !CORTEX_IS_VALID_KERNEL_PRIORITY(STM32_ADC_ADC34_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to ADC3"
#endif

#if STM32_ADC_USE_ADC3 &&                                                   \
    !CORTEX_IS_VALID_KERNEL_PRIORITY(STM32_ADC_ADC34_DMA_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to ADC3 DMA"
#endif

#if STM32_ADC_USE_ADC3 &&                                                   \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_ADC_ADC34_DMA_PRIORITY)
#error "Invalid DMA priority assigned to ADC3"
#endif

#if STM32_ADC_USE_ADC34 &&                                                  \
    !CORTEX_IS_VALID_KERNEL_PRIORITY(STM32_ADC_ADC34_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to ADC34"
#endif

#if STM32_ADC_USE_ADC34 &&                                                  \
    !CORTEX_IS_VALID_KERNEL_PRIORITY(STM32_ADC_ADC34_DMA_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to ADC34 DMA"
#endif

#if STM32_ADC_USE_ADC34 &&                                                  \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_ADC_ADC34_DMA_PRIORITY)
#error "Invalid DMA priority assigned to ADC34"
#endif

#if !defined(STM32_DMA_REQUIRED)
#define STM32_DMA_REQUIRED
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

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
  ADC_ERR_OVERFLOW = 1,                     /**< ADC overflow condition.    */
  ADC_ERR_AWD1 = 2                          /**< Watchdog 1 triggered.      */
  ADC_ERR_AWD2 = 3                          /**< Watchdog 2 triggered.      */
  ADC_ERR_AWD3 = 4                          /**< Watchdog 3 triggered.      */
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
 *          STM32 ADC cell registers interface, please refer to the STM32
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
   * @brief   ADC CFGR1 register initialization data.
   */
  uint32_t                  cfgr1;
  /**
   * @brief   ADC TR register initialization data.
   */
  uint32_t                  tr;
  /**
   * @brief   ADC SMPR register initialization data.
   */
  uint32_t                  smpr;
  /**
   * @brief   ADC CHSELR register initialization data.
   * @details The number of bits at logic level one in this register must
   *          be equal to the number in the @p num_channels field.
   */
  uint32_t                  chselr;
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
  ADC_TypeDef               *adc;
  /**
   * @brief Pointer to associated SMA channel.
   */
  const stm32_dma_stream_t  *dmastp;
  /**
   * @brief DMA mode bit mask.
   */
  uint32_t                  dmamode;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Changes the value of the ADC CCR registers.
 * @details Use this function in order to enable or disable the internal
 *          analog sources. See the documentation in the STM32F3xx Reference
 *          Manual.
 */
#define adcSTM32SetCCR(adc, ccr) ((adc)->CCR = (ccr))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_ADC_USE_ADC1 && !defined(__DOXYGEN__)
extern ADCDriver ADCD1;
#endif

#if STM32_ADC_USE_ADC12 && !defined(__DOXYGEN__)
extern ADCDriver ADCD12;
#endif

#if STM32_ADC_USE_ADC3 && !defined(__DOXYGEN__)
extern ADCDriver ADCD3;
#endif

#if STM32_ADC_USE_ADC34 && !defined(__DOXYGEN__)
extern ADCDriver ADCD34;
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

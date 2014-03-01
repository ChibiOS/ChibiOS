/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC17xx DAC driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC17xx/dac_lld.h
 * @brief   LPC17xx DAC subsystem low level driver header.
 *
 * @addtogroup DAC
 * @{
 */

#ifndef _DAC_LLD_H_
#define _DAC_LLD_H_

#if HAL_USE_DAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define DACR_BIAS                   (1UL << 16)

#define DACCTRL_INT_DMA_REQ         (1UL << 0)
#define DACCTRL_DBLBUF_ENA          (1UL << 1)
#define DACCTRL_CNT_ENA             (1UL << 2)
#define DACCTRL_DMA_ENA             (1UL << 3)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */

/**
 * @brief   DMA stream used for DAC CHN1 TX operations.
 * @note    This option is only available on platforms with enhanced DMA.
 */
#if !defined(LPC17xx_DAC_DMA_CHANNEL) || defined(__DOXYGEN__)
#define LPC17xx_DAC_DMA_CHANNEL             DMA_CHANNEL5
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(LPC17xx_DMA_REQUIRED)
#define LPC17xx_DMA_REQUIRED
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an DAC driver.
 */
typedef struct DACDriver DACDriver;

/**
 * @brief   Type representing a DAC sample.
 */
typedef uint32_t dacsample_t;

/**
 * @brief   DAC notification callback type.
 *
 * @param[in] dacp      pointer to the @p DACDriver object triggering the
 *                      callback
 */
typedef void (*dacendcallback_t)(DACDriver *dacp, const dacsample_t * samples, size_t pos);

/**
 * @brief   DAC notification callback type.
 *
 * @param[in] dacp      pointer to the @p DACDriver object triggering the
 *                      callback
 */
typedef void (*dacerrcallback_t)(DACDriver *dacp, uint32_t flags);

/**
 * @brief   DAC Conversion group structure.
 */
typedef struct {
  /**
   * @brief Number of DAC channels.
   */
  uint16_t              num_channels;
  /**
   * @brief Operation complete callback or @p NULL.
   */
  dacendcallback_t     end_cb;
  /**
   * @brief Error handling callback or @p NULL.
   */
  dacerrcallback_t      error_cb;
  /**
   * @brief Error handling callback or @p NULL.
   */
  bool      circular;

} DACConversionGroup;

/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  /**
   * @brief   Timer frequency in Hz.
   */
  uint32_t                  frequency;
  /* End of the mandatory fields.*/
} DACConfig;

/**
 * @brief   Structure representing a DAC driver.
 */
struct DACDriver {
  /**
   * @brief Driver state.
   */
  dacstate_t                state;
  /**
   * @brief Conversion group.
   */
  const DACConversionGroup *grpp;
  /**
   * @brief Samples buffer pointer.
   */
  const dacsample_t *samples;
  /**
   * @brief Samples buffer size.
   */
  uint16_t depth;
  /**
   * @brief Current configuration data.
   */
  const DACConfig           *config;
#if DAC_USE_WAIT || defined(__DOXYGEN__)
  /**
   * @brief Waiting thread.
   */
  Thread                    *thread;
#endif /* DAC_USE_WAIT */
#if DAC_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
#if CH_USE_MUTEXES || defined(__DOXYGEN__)
  /**
   * @brief Mutex protecting the bus.
   */
  Mutex                     mutex;
#elif CH_USE_SEMAPHORES
  Semaphore                 semaphore;
#endif
#endif /* DAC_USE_MUTUAL_EXCLUSION */
#if defined(DAC_DRIVER_EXT_FIELDS)
  DAC_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief Half buffer indicator.
   */
  bool_t                    half_buffer;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern DACDriver DACD1;

#ifdef __cplusplus
extern "C" {
#endif
  void dac_lld_init(void);
  void dac_lld_start(DACDriver *dacp);
  void dac_lld_stop(DACDriver *dacp);
  void dac_lld_start_conversion(DACDriver *dacp);
  void dac_lld_stop_conversion(DACDriver *dacp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_DAC */

#endif /* _DAC_LLD_H_ */

/** @} */

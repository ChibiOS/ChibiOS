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
 * @file    STM32F37x/dac_lld.h
 * @brief   STM32F37x DAC subsystem low level driver header.
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

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   DAC1 driver enable switch.
 * @details If set to @p TRUE the support for DAC1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(PLATFORM_DAC_USE_DAC1) || defined(__DOXYGEN__)
#define PLATFORM_DAC_USE_DAC1               FALSE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   DAC sample data type.
 */
typedef uint16_t dacsample_t;

/**
 * @brief   Channels number in a conversion group.
 */
typedef uint16_t dac_channels_num_t;

/**
 * @brief   Possible DAC failure causes.
 * @note    Error codes are architecture dependent and should not relied
 *          upon.
 */
typedef enum {
  DAC_ERR_DMAFAILURE = 0,                   /**< DMA operations failure.    */
  DAC_ERR_UNDERRUN = 1                      /**< ADC overflow condition.    */
} dacerror_t;

/**
 * @brief   Type of a structure representing an DAC driver.
 */
typedef struct DACDriver DACDriver;

/**
 * @brief   DAC notification callback type.
 *
 * @param[in] dacp      pointer to the @p DACDriver object triggering the
 *                      callback
 * @param[in] buffer    pointer to the most recent samples data
 * @param[in] n         number of buffer rows available starting from @p buffer
 */
typedef void (*daccallback_t)(DACDriver *dacp, dacsample_t *buffer, size_t n);

/**
 * @brief   DAC error callback type.
 *
 * @param[in] dacp      pointer to the @p DACDriver object triggering the
 *                      callback
 * @param[in] err       DAC error code
 */
typedef void (*dacerrorcallback_t)(DACDriver *dacp, dacerror_t err);

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
  dac_channels_num_t        num_channels;
  /**
   * @brief   Callback function associated to the group or @p NULL.
   */
  daccallback_t             end_cb;
  /**
   * @brief   Error callback or @p NULL.
   */
  dacerrorcallback_t        error_cb;
  /* End of the mandatory fields.*/
} DACConversionGroup;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  uint32_t                  dummy;
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
   * @brief Current configuration data.
   */
  const DACConfig           *config;
  /**
   * @brief   Current samples buffer pointer or @p NULL.
   */
  const dacsample_t         *samples;
  /**
   * @brief   Current samples buffer depth or @p 0.
   */
  size_t                    depth;
  /**
   * @brief   Current conversion group pointer or @p NULL.
   */
  const DACConversionGroup  *grpp;
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
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if PLATFORM_DAC_USE_DAC1 && !defined(__DOXYGEN__)
extern DACDriver DACD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void dac_lld_init(void);
  void dac_lld_start(DACDriver *dacp);
  void dac_lld_stop(DACDriver *dacp);
  void dac_lld_send(DACDriver *dacp);
  void dac_lld_start_conversion(DACDriver *dacp);
  void dac_lld_stop_conversion(DACDriver *dacp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_DAC */

#endif /* _DAC_LLD_H_ */

/** @} */

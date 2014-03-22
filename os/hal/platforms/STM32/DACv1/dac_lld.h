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

#define STM32_DAC_CR_EN DAC_CR_EN1
#define STM32_DAC_CR_DMAEN DAC_CR_DMAEN1
#define STM32_DAC_CR_TEN DAC_CR_TEN1

#define STM32_DAC_CR_MASK (uint32_t)0x00000FFE

#define STM32_DAC_CR_BOFF_ENABLE    (uint32_t)0x00000000
#define STM32_DAC_CR_BOFF_DISABLE   DAC_CR_BOFF1

#define STM32_DAC_CR_TSEL_NONE     (uint32_t)0x00000000
#define STM32_DAC_CR_TSEL_TIM2     DAC_CR_TEN1 | DAC_CR_TSEL1_2
#define STM32_DAC_CR_TSEL_TIM4     DAC_CR_TEN1 | DAC_CR_TEN0 | DAC_CR_TSEL1_2
#define STM32_DAC_CR_TSEL_TIM5     DAC_CR_TEN1 | DAC_CR_TEN0 | DAC_CR_TSEL1_1
#define STM32_DAC_CR_TSEL_TIM6     DAC_CR_TEN1
#define STM32_DAC_CR_TSEL_TIM7     DAC_CR_TEN1 | DAC_CR_TSEL1_1
#define STM32_DAC_CR_TSEL_TIM3     DAC_CR_TEN1 | DAC_CR_TSEL1_0
#define STM32_DAC_CR_TSEL_TIM18     DAC_CR_TEN1 | DAC_CR_TSEL1_0 | DAC_CR_TSEL1_1
#define STM32_DAC_CR_TSEL_EXT_IT9  DAC_CR_TEN1 | DAC_CR_TEN1 | DAC_CR_TSEL1_2
#define STM32_DAC_CR_TSEL_SOFT     DAC_CR_TEN1 | DAC_CR_TEN0 | DAC_CR_TSEL1_1 | DAC_CR_TSEL1_2

#define STM32_DAC_CR_WAVE_NONE      (uint32_t)0x00000000
#define STM32_DAC_CR_WAVE_NOISE     DAC_CR_WAVE1_0
#define STM32_DAC_CR_WAVE_TRIANGLE  DAC_CR_WAVE1_1

#define STM32_DAC_MAMP_1 (uint32_t)0x00000000
#define STM32_DAC_MAMP_3    DAC_CR_MAMP1_0
#define STM32_DAC_MAMP_7    DAC_CR_MAMP1_1
#define STM32_DAC_MAMP_15   DAC_CR_MAMP1_0 | DAC_CR_MAMP1_1
#define STM32_DAC_MAMP_31   DAC_CR_MAMP1_2
#define STM32_DAC_MAMP_63   DAC_CR_MAMP1_0 | DAC_CR_MAMP1_2
#define STM32_DAC_MAMP_127  DAC_CR_MAMP1_1 | DAC_CR_MAMP1_2
#define STM32_DAC_MAMP_255  DAC_CR_MAMP1_0 | DAC_CR_MAMP1_1 | DAC_CR_MAMP1_2
#define STM32_DAC_MAMP_511  DAC_CR_MAMP1_3
#define STM32_DAC_MAMP_1023 DAC_CR_MAMP1_0 | DAC_CR_MAMP1_3
#define STM32_DAC_MAMP_2047 DAC_CR_MAMP1_1 | DAC_CR_MAMP1_3
#define STM32_DAC_MAMP_4095 DAC_CR_MAMP1_0 | DAC_CR_MAMP1_1 | DAC_CR_MAMP1_2

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
 * @note    The default is @p TRUE.
 */
#if !defined(STM32_DAC_USE_DAC1) || defined(__DOXYGEN__)
#define STM32_DAC_USE_DAC1                  FALSE
#endif

/**
 * @brief   DAC2 driver enable switch.
 * @details If set to @p TRUE the support for DAC2 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(STM32_DAC_USE_DAC2) || defined(__DOXYGEN__)
#define STM32_DAC_USE_DAC2                  FALSE
#endif

/**
 * @brief   DAC1 DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_DAC_DAC1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_DAC_DAC1_DMA_PRIORITY         2
#endif

/**
 * @brief   DAC2 DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_DAC_DAC2_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_DAC_DAC2_DMA_PRIORITY         2
#endif

/**
 * @brief   DAC1 interrupt priority level setting.
 */
#if !defined(STM32_DAC_DAC1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_DAC_DAC1_IRQ_PRIORITY         10
#endif

/**
 * @brief   DAC2 interrupt priority level setting.
 */
#if !defined(STM32_DAC_DAC2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_DAC_DAC2_IRQ_PRIORITY         10
#endif

/**
 * @brief   DAC1 interrupt priority level setting.uhm
 */
#if !defined(STM32_DAC_DAC1_DMA_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_DAC_DAC1_DMA_IRQ_PRIORITY     10
#endif

/**
 * @brief   DAC2 interrupt priority level setting.
 */
#if !defined(STM32_DAC_DAC2_DMA_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_DAC_DAC2_DMA_IRQ_PRIORITY     10
#endif



/**
 * @brief   DMA stream used for DAC CHN1 TX operations.
 * @note    This option is only available on platforms with enhanced DMA.
 */
#if !defined(STM32_DAC_CHN1_DMA_STREAM) || defined(__DOXYGEN__)
#define STM32_DAC_CHN1_DMA_STREAM        STM32_DMA_STREAM_ID(1, 3)
#endif

/**
 * @brief   DMA stream used for DAC CHN2 TX operations.
 * @note    This option is only available on platforms with enhanced DMA.
 */
#if !defined(STM32_DAC_CHN2_DMA_STREAM) || defined(__DOXYGEN__)
#define STM32_DAC_CHN2_DMA_STREAM        STM32_DMA_STREAM_ID(1, 4)
#endif

/**
 * @brief   DMA stream used for DAC CHN3 TX operations.
 * @note    This option is only available on platforms with enhanced DMA.
 */
#if !defined(STM32_DAC_CHN3_DMA_STREAM) || defined(__DOXYGEN__)
#define STM32_DAC_CHN3_DMA_STREAM        STM32_DMA_STREAM_ID(1, 5)
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if STM32_DAC_USE_CHN1 && !STM32_HAS_DAC_CHN1
#error "DAC CHN1 not present in the selected device"
#endif

#if STM32_DAC_USE_CHN2 && !STM32_HAS_DAC_CHN2
#error "DAC CHN2 not present in the selected device"
#endif

#if STM32_DAC_USE_CHN3 && !STM32_HAS_DAC_CHN3
#error "DAC CHN3 not present in the selected device"
#endif

#if !STM32_DAC_USE_CHN1  && !STM32_DAC_USE_CHN2 && !STM32_DAC_USE_CHN3
#error "DAC driver activated but no DAC peripheral assigned"
#endif

#if STM32_DAC_USE_CHN1 &&                                                   \
    !STM32_DMA_IS_VALID_ID(STM32_DAC_CHN1_DMA_STREAM, STM32_DAC_CHN1_DMA_MSK)
#error "invalid DMA stream associated to DAC CHN1"
#endif

#if STM32_DAC_USE_CHN2 &&                                                   \
    !STM32_DMA_IS_VALID_ID(STM32_DAC_CHN2_DMA_STREAM, STM32_DAC_CHN2_DMA_MSK)
#error "invalid DMA stream associated to DAC CHN2"
#endif

#if STM32_DAC_USE_CHN3 &&                                                   \
    !STM32_DMA_IS_VALID_ID(STM32_DAC_CHN3_DMA_STREAM, STM32_DAC_CHN3_DMA_MSK)
#error "invalid DMA stream associated to DAC CHN3"
#endif

#if !defined(STM32_DMA_REQUIRED)
#define STM32_DMA_REQUIRED
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an DAC driver.
 */
typedef struct DACDriver DACDriver;

/**
 * @brief   DAC notification callback type.
 *
 * @param[in] dacp      pointer to the @p DACDriver object triggering the
 *                      callback
 */
typedef void (*daccallback_t)(DACDriver *dacp);

typedef enum { 
  DAC_DHRM_12BIT_RIGHT = 0,
  DAC_DHRM_12BIT_LEFT = 1,
  DAC_DHRM_8BIT_RIGHT = 2,
#if defined(STM32_HAS_DAC_CHN2) && STM32_HAS_DAC_CHN2 && !defined(__DOXYGEN__)
  DAC_DHRM_12BIT_RIGHT_DUAL = 3,
  DAC_DHRM_12BIT_LEFT_DUAL = 4,
  DAC_DHRM_8BIT_RIGHT_DUAL = 5
#endif
} dacdhrmode_t;


/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  /**
   * @brief   DAC mode selection.
   */
  dacmode_t                 mode;
  /**
   * @brief   Timer frequency in Hz.
   */
  uint32_t                  frequency;
  /**
   * @brief   Transmission buffer 1 pointer.
   */
  const void                *buffer1;
  /**
   * @brief   Transmission buffer 2 pointer.
   */
  const void                *buffer2;
  /**
   * @brief   Transmission buffers size in number of samples.
   */
  size_t                    buffers_size;
  /**
   * @brief Operation complete callback or @p NULL.
   */
  daccallback_t             callback;
  /**
   * @brief Error handling callback or @p NULL.
   */
  daccallback_t             errcallback;
  /* End of the mandatory fields.*/
  /**
   * @brief   DAC data holding register mode.
   */
  dacdhrmode_t               dhrm;
  /**
   * @brief DAC initialization data.
   */
  uint32_t                  cr_flags;
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
   * @brief Pointer to the DAC registers block.
   */
  DAC_TypeDef               *dac;
  /**
   * @brief Pointer to the TIMx registers block.
   */
  stm32_tim_t               *tim;
  /**
   * @brief The Timer IRQ priority.
   */
  uint32_t                  irqprio;
  /**
   * @brief Transmit DMA stream.
   */
  const stm32_dma_stream_t  *dma;
  /**
   * @brief TX DMA mode bit mask.
   */
  uint32_t                  dmamode;
  /**
   * @brief Timer base clock.
   */
  uint32_t                  clock;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_DAC_USE_CHN1 && !defined(__DOXYGEN__)
extern DACDriver DACD1;
#endif

#if STM32_DAC_USE_CHN2 && !defined(__DOXYGEN__)
extern DACDriver DACD2;
#endif

#if STM32_DAC_USE_CHN3 && !defined(__DOXYGEN__)
extern DACDriver DACD3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void dac_lld_init(void);
  void dac_lld_start(DACDriver *dacp);
  void dac_lld_stop(DACDriver *dacp);
  void dac_lld_send(DACDriver *dacp);
  void dac_lld_send_continuous(DACDriver *dacp);
  void dac_lld_send_doublebuffer(DACDriver *dacp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_DAC */

#endif /* _DAC_LLD_H_ */

/** @} */

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    STM32/ext_lld.h
 * @brief   STM32 EXT subsystem low level driver header.
 *
 * @addtogroup EXT
 * @{
 */

#ifndef _EXT_LLD_H_
#define _EXT_LLD_H_

#if HAL_USE_EXT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Available number of EXT channels.
 */
#define EXT_MAX_CHANNELS    STM32_EXTI_NUM_CHANNELS

/**
 * @brief   Mask of the available channels.
 */
#define EXT_CHANNELS_MASK   ((1 << EXT_MAX_CHANNELS) - 1)

/**
 * @name    EXTI configuration helpers
 * @{
 */
/**
 * @brief   EXTI-GPIO association macro.
 * @details Helper macro to associate a GPIO to each of the Mx EXTI inputs.
 */
#define EXT_MODE_EXTI(m0, m1, m2, m3, m4, m5, m6, m7,                       \
                      m8, m9, m10, m11, m12, m13, m14, m15)                 \
  {                                                                         \
    ((m0) << 0)  | ((m1) << 4)  | ((m2) << 8)  | ((m3) << 12),              \
    ((m4) << 0)  | ((m5) << 4)  | ((m6) << 8)  | ((m7) << 12),              \
    ((m8) << 0)  | ((m9) << 4)  | ((m10) << 8) | ((m11) << 12),             \
    ((m12) << 0) | ((m13) << 4) | ((m14) << 8) | ((m15) << 12)              \
  }

#define EXT_MODE_GPIOA      0           /**< @brief GPIOA identifier.       */
#define EXT_MODE_GPIOB      1           /**< @brief GPIOB identifier.       */
#define EXT_MODE_GPIOC      2           /**< @brief GPIOC identifier.       */
#define EXT_MODE_GPIOD      3           /**< @brief GPIOD identifier.       */
#define EXT_MODE_GPIOE      4           /**< @brief GPIOE identifier.       */
#define EXT_MODE_GPIOF      5           /**< @brief GPIOF identifier.       */
#define EXT_MODE_GPIOG      6           /**< @brief GPIOG identifier.       */
#define EXT_MODE_GPIOH      7           /**< @brief GPIOH identifier.       */
#define EXT_MODE_GPIOI      8           /**< @brief GPIOI identifier.       */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   EXTI0 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI0_IRQ_PRIORITY        6
#endif

/**
 * @brief   EXTI1 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI1_IRQ_PRIORITY        6
#endif

/**
 * @brief   EXTI2 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI2_IRQ_PRIORITY        6
#endif

/**
 * @brief   EXTI3 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI3_IRQ_PRIORITY        6
#endif

/**
 * @brief   EXTI4 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI4_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI4_IRQ_PRIORITY        6
#endif

/**
 * @brief   EXTI9..5 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI5_9_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI5_9_IRQ_PRIORITY      6
#endif

/**
 * @brief   EXTI15..10 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI10_15_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI10_15_IRQ_PRIORITY    6
#endif

/**
 * @brief   EXTI16 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI16_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI16_IRQ_PRIORITY       6
#endif

/**
 * @brief   EXTI17 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI17_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI17_IRQ_PRIORITY       6
#endif

/**
 * @brief   EXTI18 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI18_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI18_IRQ_PRIORITY       6
#endif

/**
 * @brief   EXTI19 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI19_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI19_IRQ_PRIORITY       6
#endif

/**
 * @brief   EXTI20 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI20_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI20_IRQ_PRIORITY       6
#endif

/**
 * @brief   EXTI21 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI21_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI21_IRQ_PRIORITY       6
#endif

/**
 * @brief   EXTI22 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI22_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI22_IRQ_PRIORITY       6
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   EXT channel identifier.
 */
typedef uint32_t expchannel_t;

/**
 * @brief   Type of an EXT generic notification callback.
 *
 * @param[in] extp      pointer to the @p EXPDriver object triggering the
 *                      callback
 */
typedef void (*extcallback_t)(EXTDriver *extp, expchannel_t channel);

/**
 * @brief   Channel configuration structure.
 */
typedef struct {
  /**
   * @brief Channel mode.
   */
  uint32_t              mode;
  /**
   * @brief Channel callback.
   * @details  In the STM32 implementation a @p NULL callback pointer is
   *           valid and configures the channel as an event sources instead
   *           of an interrupt source.
   */
  extcallback_t         cb;
} EXTChannelConfig;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief Channel configurations.
   */
  EXTChannelConfig      channels[EXT_MAX_CHANNELS];
  /* End of the mandatory fields.*/
  /**
   * @brief Initialization values for EXTICRx registers.
   */
  uint16_t              exti[4];
} EXTConfig;

/**
 * @brief   Structure representing an EXT driver.
 */
struct EXTDriver {
  /**
   * @brief Driver state.
   */
  extstate_t                state;
  /**
   * @brief Current configuration data.
   */
  const EXTConfig           *config;
  /* End of the mandatory fields.*/
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern EXTDriver EXTD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void ext_lld_init(void);
  void ext_lld_start(EXTDriver *extp);
  void ext_lld_stop(EXTDriver *extp);
  void ext_lld_channel_enable(EXTDriver *extp, expchannel_t channel);
  void ext_lld_channel_disable(EXTDriver *extp, expchannel_t channel);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_EXT */

#endif /* _EXT_LLD_H_ */

/** @} */

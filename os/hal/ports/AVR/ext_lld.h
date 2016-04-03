/*
    EXT Low Level Driver for ChibiOS
    Copyright (C) 2015 Igor Stoppa <igor.stoppa@gmail.com>

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
 * @file    ext_lld.h
 * @brief   AVR EXT subsystem low level driver header.
 *
 * @addtogroup EXT
 * @{
 */

#ifndef _EXT_LLD_H_
#define _EXT_LLD_H_

#if (HAL_USE_EXT == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Indexes of EXT channels.
 */
#define EXT_INT_MIN_CHANNEL 0
#if defined(INT0_vect)
  #define EXT_INT0_PRESENT 1
  #define EXT_INT0_CHANNEL   EXT_INT_MIN_CHANNEL
#else
  #define EXT_INT0_PRESENT 0
  #define EXT_INT0_CHANNEL   (EXT_INT_MIN_CHANNEL - 1)
#endif

#if defined(INT1_vect)
  #define EXT_INT1_PRESENT 1
  #define EXT_INT1_CHANNEL   (EXT_INT0_CHANNEL + 1)
#else
  #define EXT_INT1_PRESENT 0
  #define EXT_INT1_CHANNEL   EXT_INT0_CHANNEL
#endif

#if defined(INT2_vect)
  #define EXT_INT2_PRESENT 1
  #define EXT_INT2_CHANNEL   (EXT_INT1_CHANNEL + 1)
#else
  #define EXT_INT2_PRESENT 0
  #define EXT_INT2_CHANNEL   EXT_INT1_CHANNEL
#endif

#if defined(INT3_vect)
  #define EXT_INT3_PRESENT 1
  #define EXT_INT3_CHANNEL   (EXT_INT2_CHANNEL + 1)
#else
  #define EXT_INT3_PRESENT 0
  #define EXT_INT3_CHANNEL   EXT_INT2_CHANNEL
#endif

#define EXT_INT_NUM_CHANNELS \
    (EXT_INT0_PRESENT + EXT_INT1_PRESENT + \
     EXT_INT2_PRESENT + EXT_INT3_PRESENT)

#define EXT_INT_MAX_CHANNEL \
  (EXT_INT_MIN_CHANNEL + EXT_INT_NUM_CHANNELS - 1)

/**
 * @brief   Indexes of EXT channels.
 */
#define EXT_PC_MIN_PORT     EXT_INT_NUM_CHANNELS
#if defined(PORTA)
  #define PORTA_PRESENT 1
  #define PORTA_INDEX   EXT_PC_MIN_PORT
#else
  #define PORTA_PRESENT 0
  #define PORTA_INDEX   (EXT_PC_MIN_PORT - 1)
#endif

#if defined(PORTB)
  #define PORTB_PRESENT 1
  #define PORTB_INDEX   (PORTA_INDEX + 1)
#else
  #define PORTB_PRESENT 0
  #define PORTB_INDEX   PORTA_INDEX
#endif

#if defined(PORTC)
  #define PORTC_PRESENT 1
  #define PORTC_INDEX   (PORTB_INDEX + 1)
#else
  #define PORTC_PRESENT 0
  #define PORTC_INDEX   PORTB_INDEX
#endif

#if defined(PORTD)
  #define PORTD_PRESENT 1
  #define PORTD_INDEX   (PORTC_INDEX + 1)
#else
  #define PORTD_PRESENT 0
  #define PORTD_INDEX   PORTC_INDEX
#endif

#if defined(PORTE)
  #define PORTE_PRESENT 1
  #define PORTE_INDEX   (PORTD_INDEX + 1)
#else
  #define PORTE_PRESENT 0
  #define PORTE_INDEX   PORTD_INDEX
#endif

#if defined(PORTF)
  #define PORTF_PRESENT 1
  #define PORTF_INDEX   (PORTE_INDEX + 1)
#else
  #define PORTF_PRESENT 0
  #define PORTF_INDEX   PORTE_INDEX
#endif

#if defined(PORTG)
  #define PORTG_PRESENT 1
  #define PORTG_INDEX   (PORTF_INDEX + 1)
#else
  #define PORTG_PRESENT 0
  #define PORTG_INDEX   PORTF_INDEX
#endif

#if defined(PORTH)
  #define PORTH_PRESENT 1
  #define PORTH_INDEX   (PORTG_INDEX + 1)
#else
  #define PORTH_PRESENT 0
  #define PORTH_INDEX   PORTG_INDEX
#endif

#if defined(PORTI)
  #define PORTI_PRESENT 1
  #define PORTI_INDEX   (PORTH_INDEX + 1)
#else
  #define PORTI_PRESENT 0
  #define PORTI_INDEX   PORTH_INDEX
#endif

#if defined(PORTJ)
  #define PORTJ_PRESENT 1
  #define PORTJ_INDEX   (PORTI_INDEX + 1)
#else
  #define PORTJ_PRESENT 0
  #define PORTJ_INDEX   PORTI_INDEX
#endif

#if defined(PORTK)
  #define PORTK_PRESENT 1
  #define PORTK_INDEX   (PORTJ_INDEX + 1)
#else
  #define PORTK_PRESENT 0
  #define PORTK_INDEX   PORTJ_INDEX
#endif

/**
 * @brief   Available number of PC ports.
 */

#define EXT_PC_NUM_PORTS  \
    (PORTA_PRESENT + PORTB_PRESENT + PORTC_PRESENT + PORTD_PRESENT +\
     PORTE_PRESENT + PORTF_PRESENT + PORTG_PRESENT + PORTH_PRESENT +\
     PORTI_PRESENT + PORTJ_PRESENT + PORTK_PRESENT)

#define EXT_PC_MAX_PORT (EXT_PC_MIN_PORT + EXT_PC_NUM_PORTS - 1)

#define EXT_TOTAL_CHANNELS (EXT_INT_NUM_CHANNELS + EXT_PC_NUM_PORTS)
#define EXT_MAX_CHANNELS EXT_TOTAL_CHANNELS
#define EXT_PC_MIN_CHANNEL EXT_INT_NUM_CHANNELS
#define EXT_PC_MAX_CHANNEL (EXT_PC_MIN_CHANNEL + EXT_PC_NUM_PORTS * 8 - 1)
/**
 * @brief  Level-driven irq generation.
 */
#define EXT_CH_MODE_LEVELS_MASK       8U  /**< @brief Mask of levels field. */
#define EXT_CH_MODE_LOW_LEVEL         8U  /**< @brief Trigger on Low level. */
#define EXT_CH_MODE_INTERNAL_PULLUP  16U  /**< @brief Use internal pullup.  */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    AVR configuration options
 * @{
 */
/**
 * @brief   EXT driver enable switch.
 * @details If set to @p TRUE the support for EXT1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_EXT1) || defined(__DOXYGEN__)
#define AVR_EXT_USE_EXT1               FALSE
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
typedef uint8_t expchannel_t;

/**
 * @brief   Type of an EXT generic notification callback.
 *
 * @param[in] extp      pointer to the @p EXPDriver object triggering the
 *                      callback
 * @param[in] channel   channel being triggered.
 */
typedef void (*extcallback_t)(EXTDriver *extp, expchannel_t channel);

/**
 * @brief   Channel configuration structure.
 */
typedef struct {
  /**
   * @brief Channel mode from HAL.
   */
  uint8_t              mode;
  /**
   * @brief Channel callback.
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
  EXTChannelConfig      channels[EXT_TOTAL_CHANNELS];
  /* End of the mandatory fields.*/
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
  uint8_t pc_current_values[EXT_PC_NUM_PORTS];
  uint8_t pc_old_values[EXT_PC_NUM_PORTS];
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define ext_port_to_channel(port, bit) \
  ((PORT##port##_INDEX - EXT_PC_MIN_PORT) * 8 + bit)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (AVR_EXT_USE_EXT1 == TRUE) && !defined(__DOXYGEN__)
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

#endif /* HAL_USE_EXT == TRUE */

#endif /* _EXT_LLD_H_ */

/** @} */

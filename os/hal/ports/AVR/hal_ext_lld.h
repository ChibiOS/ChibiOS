/*
  EXT Low Level Driver for ChibiOS
  Copyright (C) 2015 Igor Stoppa <igor.stoppa@gmail.com>
  Copyright (C) 2016 Theodore Ateba

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
 * @file  AVR/hal_ext_lld.h
 * @brief EXT Driver subsystem low level driver source.
 *
 * @addtogroup EXT
 * @{
 */

#ifndef _HAL_EXT_LLD_H_
#define _HAL_EXT_LLD_H_

#if HAL_USE_EXT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief  Level-driven irq generation.
 */
#define EXT_CH_MODE_LEVELS_MASK       8U  /**< @brief Mask of levels field. */
#undef EXT_CH_MODE_LOW_LEVEL          
#define EXT_CH_MODE_LOW_LEVEL         8U  /**< @brief Trigger on Low level. */
#define EXT_CH_MODE_INTERNAL_PULLUP   16U /**< @brief Use internal pullup.  */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    AVR configuration options
 * @{
 */
/**
 * @brief   INT0 support enable switch.
 * @details If set to @p TRUE the support for INT0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_INT0) || defined(__DOXYGEN__)
#define AVR_EXT_USE_INT0                    FALSE
#endif

/**
 * @brief   INT1 support enable switch.
 * @details If set to @p TRUE the support for INT1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_INT1) || defined(__DOXYGEN__)
#define AVR_EXT_USE_INT1                    FALSE
#endif

/**
 * @brief   INT2 support enable switch.
 * @details If set to @p TRUE the support for INT2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_INT2) || defined(__DOXYGEN__)
#define AVR_EXT_USE_INT2                    FALSE
#endif

/**
 * @brief   INT3 support enable switch.
 * @details If set to @p TRUE the support for INT3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_INT3) || defined(__DOXYGEN__)
#define AVR_EXT_USE_INT3                    FALSE
#endif

/**
 * @brief   INT4 support enable switch.
 * @details If set to @p TRUE the support for INT4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_INT4) || defined(__DOXYGEN__)
#define AVR_EXT_USE_INT4                    FALSE
#endif

/**
 * @brief   INT5 support enable switch.
 * @details If set to @p TRUE the support for INT5 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_INT5) || defined(__DOXYGEN__)
#define AVR_EXT_USE_INT5                    FALSE
#endif

/**
 * @brief   PCINT0 support enable switch.
 * @details If set to @p TRUE the support for PCINT0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_PCINT0) || defined(__DOXYGEN__)
#define AVR_EXT_USE_PCINT0                  FALSE
#endif

/**
 * @brief   PCINT1 support enable switch.
 * @details If set to @p TRUE the support for PCINT1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_PCINT1) || defined(__DOXYGEN__)
#define AVR_EXT_USE_PCINT1                  FALSE
#endif

/**
 * @brief   PCINT2 support enable switch.
 * @details If set to @p TRUE the support for PCINT2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_PCINT2) || defined(__DOXYGEN__)
#define AVR_EXT_USE_PCINT2                  FALSE
#endif

/**
 * @brief   PCINT3 support enable switch.
 * @details If set to @p TRUE the support for PCINT3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_PCINT3) || defined(__DOXYGEN__)
#define AVR_EXT_USE_PCINT3                  FALSE
#endif

/**
 * @brief   PCINT4 support enable switch.
 * @details If set to @p TRUE the support for PCINT4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_PCINT4) || defined(__DOXYGEN__)
#define AVR_EXT_USE_PCINT4                  FALSE
#endif

/**
 * @brief   PCINT5 support enable switch.
 * @details If set to @p TRUE the support for PCINT5 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_PCINT5) || defined(__DOXYGEN__)
#define AVR_EXT_USE_PCINT5                  FALSE
#endif

/**
 * @brief   PCINT6 support enable switch.
 * @details If set to @p TRUE the support for PCINT6 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_PCINT6) || defined(__DOXYGEN__)
#define AVR_EXT_USE_PCINT6                  FALSE
#endif

/**
 * @brief   PCINT7 support enable switch.
 * @details If set to @p TRUE the support for PCINT7 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_PCINT7) || defined(__DOXYGEN__)
#define AVR_EXT_USE_PCINT7                  FALSE
#endif

/**
 * @brief   PCINT8 support enable switch.
 * @details If set to @p TRUE the support for PCINT8 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_PCINT8) || defined(__DOXYGEN__)
#define AVR_EXT_USE_PCINT8                  FALSE
#endif

/**
 * @brief   PCINT9 support enable switch.
 * @details If set to @p TRUE the support for PCINT9 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_PCINT9) || defined(__DOXYGEN__)
#define AVR_EXT_USE_PCINT9                  FALSE
#endif

/**
 * @brief   PCINT10 support enable switch.
 * @details If set to @p TRUE the support for PCINT10 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_EXT_USE_PCINT10) || defined(__DOXYGEN__)
#define AVR_EXT_USE_PCINT10                  FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(INT0_vect) && AVR_EXT_USE_INT0
#error "INT0 is not present in the selected device"
#endif

#if !defined(INT1_vect) && AVR_EXT_USE_INT1
#error "INT1 is not present in the selected device"
#endif

#if !defined(INT2_vect) && AVR_EXT_USE_INT2
#error "INT2 is not present in the selected device"
#endif

#if !defined(INT3_vect) && AVR_EXT_USE_INT3
#error "INT3 is not present in the selected device"
#endif

#if !defined(INT4_vect) && AVR_EXT_USE_INT4
#error "INT4 is not present in the selected device"
#endif

#if !defined(INT5_vect) && AVR_EXT_USE_INT5
#error "INT5 is not present in the selected device"
#endif

#if !defined(PCINT0_PIN) && AVR_EXT_USE_PCINT0
#error "PCINT0 is not present in the selected device"
#endif

#if !defined(PCINT1_PIN) && AVR_EXT_USE_PCINT1
#error "PCINT1 is not present in the selected device"
#endif

#if !defined(PCINT2_PIN) && AVR_EXT_USE_PCINT2
#error "PCINT2 is not present in the selected device"
#endif

#if !defined(PCINT3_PIN) && AVR_EXT_USE_PCINT3
#error "PCINT3 is not present in the selected device"
#endif

#if !defined(PCINT4_PIN) && AVR_EXT_USE_PCINT4
#error "PCINT4 is not present in the selected device"
#endif

#if !defined(PCINT5_PIN) && AVR_EXT_USE_PCINT5
#error "PCINT5 is not present in the selected device"
#endif

#if !defined(PCINT6_PIN) && AVR_EXT_USE_PCINT6
#error "PCINT6 is not present in the selected device"
#endif

#if !defined(PCINT7_PIN) && AVR_EXT_USE_PCINT7
#error "PCINT7 is not present in the selected device"
#endif

#if !defined(PCINT8_PIN) && AVR_EXT_USE_PCINT8
#error "PCINT8 is not present in the selected device"
#endif

#if !defined(PCINT9_PIN) && AVR_EXT_USE_PCINT9
#error "PCINT9 is not present in the selected device"
#endif

#if !defined(PCINT10_PIN) && AVR_EXT_USE_PCINT10
#error "PCINT10 is not present in the selected device"
#endif

/**
 * @brief   Indexes of INT channels.
 */
#define EXT_INT_MIN_CHANNEL     0

#if AVR_EXT_USE_INT0
#define EXT_INT0_PRESENT        1
#define EXT_INT0_CHANNEL        EXT_INT_MIN_CHANNEL
#else
#define EXT_INT0_PRESENT        0
#define EXT_INT0_CHANNEL        (EXT_INT_MIN_CHANNEL - 1)
#endif

#if AVR_EXT_USE_INT1
#define EXT_INT1_PRESENT        1
#define EXT_INT1_CHANNEL        (EXT_INT0_CHANNEL + 1)
#else
#define EXT_INT1_PRESENT        0
#define EXT_INT1_CHANNEL        EXT_INT0_CHANNEL
#endif

#if AVR_EXT_USE_INT2
#define EXT_INT2_PRESENT        1
#define EXT_INT2_CHANNEL        (EXT_INT1_CHANNEL + 1)
#else
#define EXT_INT2_PRESENT        0
#define EXT_INT2_CHANNEL        EXT_INT1_CHANNEL
#endif

#if AVR_EXT_USE_INT3
#define EXT_INT3_PRESENT        1
#define EXT_INT3_CHANNEL        (EXT_INT2_CHANNEL + 1)
#else
#define EXT_INT3_PRESENT        0
#define EXT_INT3_CHANNEL        EXT_INT2_CHANNEL
#endif

#if AVR_EXT_USE_INT4
#define EXT_INT4_PRESENT        1
#define EXT_INT4_CHANNEL        (EXT_INT3_CHANNEL + 1)
#else
#define EXT_INT4_PRESENT        0
#define EXT_INT4_CHANNEL        EXT_INT3_CHANNEL
#endif

#if AVR_EXT_USE_INT5
#define EXT_INT5_PRESENT        1
#define EXT_INT5_CHANNEL        (EXT_INT4_CHANNEL + 1)
#else
#define EXT_INT5_PRESENT        0
#define EXT_INT5_CHANNEL        EXT_INT4_CHANNEL
#endif

#define EXT_INT_NUM_CHANNELS                                                \
  (EXT_INT0_PRESENT + EXT_INT1_PRESENT + EXT_INT2_PRESENT +                 \
   EXT_INT3_PRESENT + EXT_INT4_PRESENT + EXT_INT5_PRESENT)

#if EXT_INT_NUM_CHANNELS > 0
#define EXT_INT_MAX_CHANNEL     (EXT_INT_MIN_CHANNEL + EXT_INT_NUM_CHANNELS - 1)
#else
#define EXT_INT_MAX_CHANNEL     0
#endif

/**
 * @brief   Indexes of PC channels.
 */
#define EXT_PC_MIN_PORT         EXT_INT_NUM_CHANNELS

#if AVR_EXT_USE_PCINT0
#define PORTA_PRESENT           1
#define PORTA_INDEX             EXT_PC_MIN_PORT
#else
#define PORTA_PRESENT           0
#define PORTA_INDEX             (EXT_PC_MIN_PORT - 1)
#endif

#if AVR_EXT_USE_PCINT1
#define PORTB_PRESENT           1
#define PORTB_INDEX             (PORTA_INDEX + 1)
#else
#define PORTB_PRESENT           0
#define PORTB_INDEX             PORTA_INDEX
#endif

#if AVR_EXT_USE_PCINT2
#define PORTC_PRESENT           1
#define PORTC_INDEX             (PORTB_INDEX + 1)
#else
#define PORTC_PRESENT           0
#define PORTC_INDEX             PORTB_INDEX
#endif

#if AVR_EXT_USE_PCINT3
#define PORTD_PRESENT           1
#define PORTD_INDEX             (PORTC_INDEX + 1)
#else
#define PORTD_PRESENT           0
#define PORTD_INDEX             PORTC_INDEX
#endif

#if AVR_EXT_USE_PCINT4
#define PORTE_PRESENT           1
#define PORTE_INDEX             (PORTD_INDEX + 1)
#else
#define PORTE_PRESENT           0
#define PORTE_INDEX             PORTD_INDEX
#endif

#if AVR_EXT_USE_PCINT5
#define PORTF_PRESENT           1
#define PORTF_INDEX             (PORTE_INDEX + 1)
#else
#define PORTF_PRESENT           0
#define PORTF_INDEX             PORTE_INDEX
#endif

#if AVR_EXT_USE_PCINT6
#define PORTG_PRESENT           1
#define PORTG_INDEX             (PORTF_INDEX + 1)
#else
#define PORTG_PRESENT           0
#define PORTG_INDEX             PORTF_INDEX
#endif

#if AVR_EXT_USE_PCINT7
#define PORTH_PRESENT           1
#define PORTH_INDEX             (PORTG_INDEX + 1)
#else
#define PORTH_PRESENT           0
#define PORTH_INDEX             PORTG_INDEX
#endif

#if AVR_EXT_USE_PCINT8
#define PORTI_PRESENT           1
#define PORTI_INDEX             (PORTH_INDEX + 1)
#else
#define PORTI_PRESENT           0
#define PORTI_INDEX             PORTH_INDEX
#endif

#if AVR_EXT_USE_PCINT9
#define PORTJ_PRESENT           1
#define PORTJ_INDEX             (PORTI_INDEX + 1)
#else
#define PORTJ_PRESENT           0
#define PORTJ_INDEX             PORTI_INDEX
#endif

#if AVR_EXT_USE_PCINT10
#define PORTK_PRESENT           1
#define PORTK_INDEX             (PORTJ_INDEX + 1)
#else
#define PORTK_PRESENT           0
#define PORTK_INDEX             PORTJ_INDEX
#endif

/**
 * @brief   Available number of PC ports.
 */

#define EXT_PC_NUM_PORTS                                                    \
  (PORTA_PRESENT + PORTB_PRESENT + PORTC_PRESENT + PORTD_PRESENT +          \
   PORTE_PRESENT + PORTF_PRESENT + PORTG_PRESENT + PORTH_PRESENT +          \
   PORTI_PRESENT + PORTJ_PRESENT + PORTK_PRESENT)

#if EXT_PC_NUM_PORTS > 0
#define EXT_PC_MAX_PORT         (EXT_PC_MIN_PORT + EXT_PC_NUM_PORTS - 1)
#else
#define EXT_PC_MAX_PORT         0
#endif

#define EXT_PC_NUM_CHANNELS     (EXT_PC_NUM_PORTS * 8)

#define EXT_TOTAL_CHANNELS      (EXT_INT_NUM_CHANNELS + EXT_PC_NUM_CHANNELS)
#define EXT_MAX_CHANNELS        EXT_TOTAL_CHANNELS
#define EXT_PC_MIN_CHANNEL      EXT_INT_NUM_CHANNELS
#define EXT_PC_MAX_CHANNEL      (EXT_PC_MIN_CHANNEL + EXT_PC_NUM_CHANNELS - 1)

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief EXT channel identifier.
 */
typedef uint16_t expchannel_t;

/**
 * @brief Type of an EXT generic notification callback.
 *
 * @param[in] extp  pointer to the @p EXPDriver object triggering the
 *                  callback
 * @param[in] channel   channel being triggered.
 */
typedef void (*extcallback_t)(EXTDriver *extp, expchannel_t channel);

/**
 * @brief Channel configuration structure.
 */
typedef struct {
  /**
   * @brief Channel mode from HAL.
   */
  uint8_t      mode;
  /**
   * @brief Channel callback.
   */
  extcallback_t cb;
} EXTChannelConfig;

/**
 * @brief Driver configuration structure.
 * @note  It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief Channel configurations.
   */
  EXTChannelConfig  channels[EXT_TOTAL_CHANNELS];
  /* End of the mandatory fields.*/
} EXTConfig;

/**
 * @brief Structure representing an EXT driver.
 */
struct EXTDriver {
  /**
   * @brief Driver state.
   */
  extstate_t      state;

  /**
   * @brief Current configuration data.
   */
  const EXTConfig *config;
  /* End of the mandatory fields.*/
#if EXT_PC_NUM_PORTS > 0
  /**
   * @brief Current pin values. Only valid for PCINT.
   */
  uint8_t pc_current_values[EXT_PC_NUM_PORTS];
  /**
   * @brief Previous pin states. Only valid for PCINT.
   */
  uint8_t pc_old_values[EXT_PC_NUM_PORTS];
#endif
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define ext_port_to_channel(port, bit) \
  ((PORT##port##_INDEX - EXT_PC_MIN_PORT) * 8 + bit)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern EXTDriver EXTD1;

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

#endif /* _HAL_EXT_LLD_H_ */

/** @} */

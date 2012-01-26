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
 * @file    SPC56x/serial_lld.h
 * @brief   SPC563 low level serial driver header.
 *
 * @addtogroup SERIAL
 * @{
 */

#ifndef _SERIAL_LLD_H_
#define _SERIAL_LLD_H_

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define SD_MODE_PARITY          0x03        /**< @brief Parity field mask.  */
#define SD_MODE_PARITY_NONE     0x00        /**< @brief No parity.          */
#define SD_MODE_PARITY_EVEN     0x01        /**< @brief Even parity.        */
#define SD_MODE_PARITY_ODD      0x02        /**< @brief Odd parity.         */

#define SD_MODE_NORMAL          0x00        /**< @brief Normal operations.  */
#define SD_MODE_LOOPBACK        0x80        /**< @brief Internal loopback.  */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   eSCI-A driver enable switch.
 * @details If set to @p TRUE the support for eSCI-A is included.
 * @note    The default is @p TRUE.
 */
#if !defined(USE_SPC563_ESCIA) || defined(__DOXYGEN__)
#define USE_SPC563_ESCIA            TRUE
#endif

/**
 * @brief   eSCI-B driver enable switch.
 * @details If set to @p TRUE the support for eSCI-B is included.
 * @note    The default is @p TRUE.
 */
#if !defined(USE_SPC563_ESCIB) || defined(__DOXYGEN__)
#define USE_SPC563_ESCIB            TRUE
#endif

/**
 * @brief   eSCI-A interrupt priority level setting.
 */
#if !defined(SPC563_ESCIA_PRIORITY) || defined(__DOXYGEN__)
#define SPC563_ESCIA_PRIORITY       8
#endif

/**
 * @brief   eSCI-B interrupt priority level setting.
 */
#if !defined(SPC563_ESCIB_PRIORITY) || defined(__DOXYGEN__)
#define SPC563_ESCIB_PRIORITY       8
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Generic Serial Driver configuration structure.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 * @note    This structure content is architecture dependent, each driver
 *          implementation defines its own version and the custom static
 *          initializers.
 */
typedef struct {
  /**
   * @brief Bit rate.
   */
  uint32_t                  sc_speed;
  /**
   * @brief Mode flags.
   */
  uint8_t                   sc_mode;
} SerialConfig;

/**
 * @brief   @p SerialDriver specific data.
 */
#define _serial_driver_data                                                 \
  _base_asynchronous_channel_data                                           \
  /* Driver state.*/                                                        \
  sdstate_t                 state;                                          \
  /* Input queue.*/                                                         \
  InputQueue                iqueue;                                         \
  /* Output queue.*/                                                        \
  OutputQueue               oqueue;                                         \
  /* Input circular buffer.*/                                               \
  uint8_t                   ib[SERIAL_BUFFERS_SIZE];                        \
  /* Output circular buffer.*/                                              \
  uint8_t                   ob[SERIAL_BUFFERS_SIZE];                        \
  /* End of the mandatory fields.*/                                         \
  /* Pointer to the volatile eSCI registers block.*/                        \
  volatile struct ESCI_tag  *escip;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if USE_SPC563_ESCIA && !defined(__DOXYGEN__)
extern SerialDriver SD1;
#endif
#if USE_SPC563_ESCIB && !defined(__DOXYGEN__)
extern SerialDriver SD2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sd_lld_init(void);
  void sd_lld_start(SerialDriver *sdp, const SerialConfig *config);
  void sd_lld_stop(SerialDriver *sdp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SERIAL */

#endif /* _SERIAL_LLD_H_ */

/** @} */

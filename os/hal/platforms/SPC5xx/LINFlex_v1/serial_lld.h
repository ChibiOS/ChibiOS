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
 * @file    SPC5xx/serial_lld.h
 * @brief   SPC5xx low level serial driver header.
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

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   LINFlex-0 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-0 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX0) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX0            TRUE
#endif

/**
 * @brief   LINFlex-1 driver enable switch.
 * @details If set to @p TRUE the support for LINFlex-1 is included.
 */
#if !defined(SPC5_SERIAL_USE_LINFLEX1) || defined(__DOXYGEN__)
#define SPC5_SERIAL_USE_LINFLEX1            TRUE
#endif

/**
 * @brief   LINFlex-0 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX0_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX0_PRIORITY       8
#endif

/**
 * @brief   LINFlex-1 interrupt priority level setting.
 */
#if !defined(SPC5_SERIAL_LINFLEX1_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_SERIAL_LINFLEX1_PRIORITY       8
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if SPC5_SERIAL_USE_LINFLEX0 && !SPC5_HAS_LINFLEX0
#error "LINFlex-0 not present in the selected device"
#endif

#if SPC5_SERIAL_USE_LINFLEX1 && !SPC5_HAS_LINFLEX1
#error "LINFlex-1 not present in the selected device"
#endif

#if SPC5_SERIAL_USE_LINFLEX2 && !SPC5_HAS_LINFLEX2
#error "LINFlex-2 not present in the selected device"
#endif

#if SPC5_SERIAL_USE_LINFLEX3 && !SPC5_HAS_LINFLEX3
#error "LINFlex-3 not present in the selected device"
#endif

#if !SPC5_SERIAL_USE_LINFLEX0 && !SPC5_SERIAL_USE_LINFLEX1 &&               \
    !SPC5_SERIAL_USE_LINFLEX2 && !SPC5_SERIAL_USE_LINFLEX3
#error "SERIAL driver activated but no LINFlex peripheral assigned"
#endif

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
  uint32_t                  speed;
  /**
   * @brief Mode flags.
   */
  uint8_t                   mode;
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
  /* Pointer to the volatile LINFlex registers block.*/                     \
  volatile struct LINFLEX_tag *linflexp;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if SPC5_SERIAL_USE_LINFLEX0 && !defined(__DOXYGEN__)
extern SerialDriver SD1;
#endif
#if SPC5_SERIAL_USE_LINFLEX1 && !defined(__DOXYGEN__)
extern SerialDriver SD2;
#endif
#if SPC5_SERIAL_USE_LINFLEX2 && !defined(__DOXYGEN__)
extern SerialDriver SD3;
#endif
#if SPC5_SERIAL_USE_LINFLEX3 && !defined(__DOXYGEN__)
extern SerialDriver SD4;
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

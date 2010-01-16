/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file templates/serial_lld.h
 * @brief Serial Driver subsystem low level driver header template.
 * @addtogroup SERIAL_LLD
 * @{
 */

#ifndef _SERIAL_LLD_H_
#define _SERIAL_LLD_H_

#if CH_HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * Serial Driver condition flags type.
 */
typedef uint8_t sdflags_t;

/**
 * @brief Generic Serial Driver configuration structure.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 *
 * @note This structure content is architecture dependent, each driver
 *       implementation defines its own version and the custom static
 *       initializers.
 */
typedef struct {

} SerialConfig;

/**
 * @brief @p SerialDriver specific data.
 */
struct _serial_driver_data {
  /**
   * @brief Driver state.
   */
  sdstate_t                 state;
  /**
   * @brief Current configuration data.
   */
  const SerialConfig        *config;
  /**
   * @brief Input queue, incoming data can be read from this input queue by
   *        using the queues APIs.
   */
  InputQueue                iqueue;
  /**
   * @brief Output queue, outgoing data can be written to this output queue by
   *        using the queues APIs.
   */
  OutputQueue               oqueue;
  /**
   * @brief Status Change @p EventSource. This event is generated when one or
   *        more condition flags change.
   */
  EventSource               sevent;
  /**
   * @brief I/O driver status flags.
   */
  sdflags_t                 flags;
  /**
   * @brief Input circular buffer.
   */
  uint8_t                   ib[SERIAL_BUFFERS_SIZE];
  /**
   * @brief Output circular buffer.
   */
  uint8_t                   ob[SERIAL_BUFFERS_SIZE];
  /* End of the mandatory fields.*/
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void sd_lld_init(void);
  void sd_lld_start(SerialDriver *sdp);
  void sd_lld_stop(SerialDriver *sdp);
#ifdef __cplusplus
}
#endif

#endif /* CH_HAL_USE_SERIAL */

#endif /* _SERIAL_LLD_H_ */

/** @} */

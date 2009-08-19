/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @file os/io/templates/serial_lld.h
 * @brief Serial Driver subsystem low level driver header template
 * @addtogroup SERIAL_LLD
 * @{
 */

#ifndef _SERIAL_LLD_H_
#define _SERIAL_LLD_H_

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Unsupported event flags and custom events.                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * Serial Driver condition flags type.
 */
typedef uint8_t sdflags_t;

/**
 * @brief @p SerialDriver specific data.
 */
struct _serial_driver_data {
  /**
   * Input queue, incoming data can be read from this input queue by
   * using the queues APIs.
   */
  InputQueue            iqueue;
  /**
   * Output queue, outgoing data can be written to this output queue by
   * using the queues APIs.
   */
  OutputQueue           oqueue;
  /**
   * Status Change @p EventSource. This event is generated when one or more
   * condition flags change.
   */
  EventSource           sevent;
  /**
   * I/O driver status flags.
   */
  sdflags_t             flags;
  /**
   * Input circular buffer.
   */
  uint8_t               ib[SERIAL_BUFFERS_SIZE];
  /**
   * Output circular buffer.
   */
  uint8_t               ob[SERIAL_BUFFERS_SIZE];
};

/**
 * @brief Generic Serial Driver static initializer.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 *
 * @note This structure content is architecture dependent, each driver
 *       implementation defines its own version and the custom static
 *       initializers.
 */
typedef struct {

} SerialDriverConfig;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
  void sd_lld_init(void);
  void sd_lld_start(SerialDriver *sdp, const SerialDriverConfig *config);
  void sd_lld_stop(SerialDriver *sdp);
#endif
#ifdef __cplusplus
}
#endif

#endif /* _SERIAL_LLD_H_ */

/** @} */

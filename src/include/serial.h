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
 * @file serial.h
 * @brief Serial Drivers macros and structures.
 * @addtogroup Serial
 * @{
 */

#ifndef _SERIAL_H_
#define _SERIAL_H_

#if CH_USE_SERIAL_FULLDUPLEX

/** No pending conditions.*/
#define SD_NO_ERROR             0
/** Connection happened.*/
#define SD_CONNECTED            1
/** Disconnection happened.*/
#define SD_DISCONNECTED         2
/** Parity error happened.*/
#define SD_PARITY_ERROR         4
/** Framing error happened.*/
#define SD_FRAMING_ERROR        8
/** Overflow happened.*/
#define SD_OVERRUN_ERROR        16
/** Break detected.*/
#define SD_BREAK_DETECTED       32

/** Serial Driver condition flags type.*/
typedef uint8_t dflags_t;

/**
 * @brief @p GenericSerialDriver specific methods.
 */
struct _generic_serial_driver_methods {
};

/**
 * @brief @p GenericSerialDriver specific data.
 */
struct _generic_serial_driver_data {
  /**
   * Status Change @p EventSource. This event is generated when one or more
   * condition flags change.
   */
  EventSource           sevent;
  /**
   * I/O driver status flags. This field should not be read directly but
   * the @p () function should be used instead.
   */
  dflags_t              flags;
};

/**
 * @brief @p GenericSerialDriver virtual methods table.
 */
struct _generic_serial_driver_vmt {
  /**
   * @p BaseChannel class inherited methods.
   */
  struct _base_channel_methods m0;
  /**
   * @p BaseAsynchronousChannel class inherited methods.
   */
  struct _base_asynchronous_channel_methods m1;
  /**
   * @p GenericSerialDriver specific methods.
   */
  struct _generic_serial_driver_methods m2;
};

/**
 * @extends BaseAsynchronousChannel
 *
 * @brief Generic serial driver class.
 * @details This class extends @p BaseAsynchronousChannel by adding handling for
 *          serial error events.
 */
typedef struct {
  /**
   * Virtual Methods Table.
   */
  struct _generic_serial_driver_vmt *vmt;
  /**
   * @p BaseChannel class inherited data.
   */
  struct _base_channel_data d0;
  /**
   * @p BaseAsynchronousChannel class inherited data.
   */
  struct _base_asynchronous_channel_data d1;
  /**
   * @p GenericSerialDriver specific data.
   */
  struct _generic_serial_driver_data d2;
} GenericSerialDriver;


/**
 * @brief @p FullDuplexDriver specific methods.
 */
struct _full_duplex_driver_methods {
};

/**
 * @brief @p FullDuplexDriver specific data.
 */
struct _full_duplex_driver_data {
  /**
   * Input queue, incoming data can be read from this input queue by
   * using the queues APIs.
   */
  InputQueue            sd_iqueue;
  /**
   * Output queue, outgoing data can be written to this output queue by
   * using the queues APIs.
   */
  OutputQueue           sd_oqueue;
};

/**
 * @brief @p FullDuplexDriver virtual methods table.
 */
struct _full_duplex_driver_vmt {
  /**
   * @p BaseChannel class inherited methods.
   */
  struct _base_channel_methods m0;
  /**
   * @p BaseAsynchronousChannel class inherited methods.
   */
  struct _base_asynchronous_channel_methods m1;
  /**
   * @p GenericSerialDriver specific methods.
   */
  struct _generic_serial_driver_methods m2;
  /**
   * @p FullDuplexDriver specific methods.
   */
  struct _full_duplex_driver_methods m3;
};

/**
 * @extends GenericSerialDriver
 *
 * @brief Full duplex serial driver class.
 * @details This class extends @p GenericSerialDriver by adding physical I/O
 *          queues.
 */
typedef struct {
  /**
   * Virtual Methods Table.
   */
  struct _generic_serial_driver_vmt *vmt;
  /**
   * @p BaseChannel class inherited data.
   */
  struct _base_channel_data d0;
  /**
   * @p BaseAsynchronousChannel class inherited data.
   */
  struct _base_asynchronous_channel_data d1;
  /**
   * @p GenericSerialDriver specific data.
   */
  struct _generic_serial_driver_data d2;
  /**
   * @p FullDuplexDriver specific data.
   */
  struct _full_duplex_driver_data d3;
} FullDuplexDriver_;

/**
 * @brief Full Duplex Serial Driver main structure.
 */
typedef struct {

  /** Input queue. Incoming data can be read from this queue by using the
   *  queues APIs.*/
  InputQueue            sd_iqueue;
  /** Data Available @p EventSource. This event is generated when some incoming
   *  data is inserted in the Input @p Queue.*/
  EventSource           sd_ievent;

  /** Output queue. Outgoing data can be written to this Output @p Queue by
   *   using the queues APIs.*/
  OutputQueue           sd_oqueue;
  /** Data Transmitted @p EventSource. This event is generated when the
   *  Output @p Queue is empty.*/
  EventSource           sd_oevent;

  /** I/O driver status flags. This field should not be read directly but
   *  the @p chFDDGetAndClearFlags() funtion should be used instead.*/
  dflags_t              sd_flags;
  /** Status Change @p EventSource. This event is generated when a
   *  condition flag was changed.*/
  EventSource           sd_sevent;
} FullDuplexDriver;

#ifdef __cplusplus
extern "C" {
#endif
  void chFDDInit(FullDuplexDriver *sd,
                 uint8_t *ib, size_t isize, qnotify_t inotify,
                 uint8_t *ob, size_t osize, qnotify_t onotify);
  void chFDDIncomingDataI(FullDuplexDriver *sd, uint8_t b);
  msg_t chFDDRequestDataI(FullDuplexDriver *sd);
  void chFDDAddFlagsI(FullDuplexDriver *sd, dflags_t mask);
  dflags_t chFDDGetAndClearFlags(FullDuplexDriver *sd);
#ifdef __cplusplus
}
#endif

/** @see chIQRead()*/
#define chFDDRead(sd, b, n) \
        chIQRead(&(sd)->sd_iqueue, b, n)

/** @see chOQWrite()*/
#define chFDDWrite(sd, b, n) \
        chOQWrite(&(sd)->sd_oqueue, b, n)

/** @see chIQGet()*/
#define chFDDGet(sd) \
        chIQGet(&(sd)->sd_iqueue)

/** @see chIQGetTimeout()*/
#define chFDDGetTimeout(sd, t) \
        chIQGetTimeout(&(sd)->sd_iqueue, t)

/** @see chOQPut()*/
#define chFDDPut(sd, b) \
        chOQPut(&(sd)->sd_oqueue, b)

#endif /* CH_USE_SERIAL_FULLDUPLEX */

#endif /* _SERIAL_H_ */

/** @} */

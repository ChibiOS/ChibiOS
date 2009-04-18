/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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
 * @file serial.h
 * @brief Serial Drivers macros and structures.
 * @addtogroup Serial
 * @{
 */

#ifndef _SERIAL_H_
#define _SERIAL_H_

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
typedef uint16_t dflags_t;

#if CH_USE_SERIAL_FULLDUPLEX

/**
 * @brief Full Duplex Serial Driver main structure.
 */
typedef struct {

  /** Input queue. Incoming data can be read from this queue by using the
   *  queues APIs.*/
  Queue                 sd_iqueue;
  /** Data Available @p EventSource. This event is generated when some incoming
   *  data is inserted in the Input @p Queue.*/
  EventSource           sd_ievent;

  /** Output queue. Outgoing data can be written to this Output @p Queue by
   *   using the queues APIs.*/
  Queue                 sd_oqueue;
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

#if CH_USE_SERIAL_HALFDUPLEX

/**
 * @brief Full Duplex Serial Driver main structure.
 */
typedef struct {

  /** Data queue. Transmit/receive @p HalfDuplexQueue.*/
  HalfDuplexQueue       sd_queue;
  /** Data Available @p EventSource. This event is generated when some
   *  incoming data is inserted in the receive queue.*/
  EventSource           sd_ievent;
  /** Data Transmitted @p EventSource. This event is generated when the
   *  transmission queue is empty and the driver can either transmit more
   *  data or enter receive mode.*/
  EventSource           sd_oevent;
  /** I/O driver status flags. This field should not be read directly but
   *  the @p chHDDGetAndClearFlags() funtion should be used
   *  instead.*/
  dflags_t              sd_flags;
  /** Status Change Event Source. This event is generated when a condition
   *  flag was changed.*/
  EventSource           sd_sevent;
} HalfDuplexDriver;

#ifdef __cplusplus
extern "C" {
#endif
  void chHDDInit(HalfDuplexDriver *sd, uint8_t *b, size_t size,
                qnotify_t inotify, qnotify_t onotify);
  void chHDDIncomingDataI(HalfDuplexDriver *sd, uint8_t b);
  msg_t chHDDRequestDataI(HalfDuplexDriver *sd);
  void chHDDAddFlagsI(HalfDuplexDriver *sd, dflags_t mask);
  dflags_t chHDDGetAndClearFlags(HalfDuplexDriver *sd);
#ifdef __cplusplus
}
#endif

/** @see chHDQGetReceive()*/
#define chHDDGetReceive(sd) \
        chHDQGetReceive(&(sd)->sd_queue)

/** @see chHDQGetReceiveTimeout()*/
#define chHDDGetReceiveTimeout(sd, t) \
        chHDQGetReceiveTimeout(&(sd)->sd_queue, t)

/** @see chHDQPutTransmit()*/
#define chHDDPutTransmit(sd, b) \
        chHDQPutTransmit(&(sd)->sd_queue, b)

#endif /* CH_USE_SERIAL_HALFDUPLEX */

#endif /* _SERIAL_H_ */

/** @} */

/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

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
 * @file        hal_frame_io.h
 * @brief       Generated Frames I/O header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_FRAME_IO
 * @brief       Frames-exchange devices definitions and interfaces.
 * @{
 */

#ifndef HAL_FRAME_IO_H
#define HAL_FRAME_IO_H

#include <string.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @interface   frame_io_i
 * @extends     base_interface_i.
 *
 * @brief       Frames-exchange device interface.
 * @details     This header defines an abstract interface useful to access
 *              generic frames-based block devices in a standardized way.
 *
 * @name        Interface @p frame_io_i structures
 * @{
 */

/**
 * @brief       Type of a frame device interface.
 */
typedef struct frame_io frame_io_i;

/**
 * @brief       Interface @p frame_io_i virtual methods table.
 */
struct frame_io_vmt {
  /* Memory offset between this interface structure and begin of
     the implementing class structure.*/
  size_t instance_offset;
  /* From base_interface_i.*/
  /* From frame_io_i.*/
  msg_t (*get_rxh)(void *ip);
  msg_t (*get_txh)(void *ip);
  void (*release_rxh)(void *ip, unsigned rxh);
  void (*release_txh)(void *ip, unsigned txh);
  ssize_t (*read_rxh)(void *ip, unsigned rxh, uint8_t *bp, size_t n);
  ssize_t (*write_txh)(void *ip, unsigned txh, const uint8_t *bp, size_t n);
  const uint8_t * (*get_rxh_buffer)(void *ip, unsigned rxh, size_t *sizep);
  uint8_t * (*get_txh_buffer)(void *ip, unsigned txh, size_t *sizep);
};

/**
 * @brief       Structure representing a frame device interface.
 */
struct frame_io {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct frame_io_vmt *vmt;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Virtual methods of frame_io_i
 * @{
 */
/**
 * @memberof    frame_io_i
 * @public
 *
 * @brief       Queries for a received frame handle.
 *
 * @param[in,out] ip            Pointer to a @p frame_io_i instance.
 * @return                      The receive handle.
 * @retval MSG_TIMEOUT          If a received frame is not available.
 */
CC_FORCE_INLINE
static inline msg_t frmGetReceiveHandle(void *ip) {
  frame_io_i *self = (frame_io_i *)ip;

  return self->vmt->get_rxh(ip);
}

/**
 * @memberof    frame_io_i
 * @public
 *
 * @brief       Queries for a transmit frame handle.
 *
 * @param[in,out] ip            Pointer to a @p frame_io_i instance.
 * @return                      The transmit handle.
 * @retval MSG_TIMEOUT          If an empty transmit frame is not available.
 */
CC_FORCE_INLINE
static inline msg_t frmGetTransmitHandle(void *ip) {
  frame_io_i *self = (frame_io_i *)ip;

  return self->vmt->get_txh(ip);
}

/**
 * @memberof    frame_io_i
 * @public
 *
 * @brief       Releases a received frame.
 *
 * @param[in,out] ip            Pointer to a @p frame_io_i instance.
 * @param[in]     rxh           Receive handle.
 */
CC_FORCE_INLINE
static inline void frmReleaseReceiveHandle(void *ip, unsigned rxh) {
  frame_io_i *self = (frame_io_i *)ip;

  self->vmt->release_rxh(ip, rxh);
}

/**
 * @memberof    frame_io_i
 * @public
 *
 * @brief       Releases and transmits a frame.
 *
 * @param[in,out] ip            Pointer to a @p frame_io_i instance.
 * @param[in]     txh           Transmit handle.
 */
CC_FORCE_INLINE
static inline void frmReleaseTransmitHandle(void *ip, unsigned txh) {
  frame_io_i *self = (frame_io_i *)ip;

  self->vmt->release_txh(ip, txh);
}

/**
 * @memberof    frame_io_i
 * @public
 *
 * @brief       Reads data sequentially from a received frame.
 *
 * @param[in,out] ip            Pointer to a @p frame_io_i instance.
 * @param[in]     rxh           Receive handle.
 * @param[out]    bp            Received data buffer pointer.
 * @param[in]     n             Number of bytes to read.
 * @return                      The number of bytes read from the handle
 *                              buffer, this value can be less than the amount
 *                              specified in the parameter @p size if there are
 *                              no more bytes to read.
 */
CC_FORCE_INLINE
static inline ssize_t frmReadReceiveHandle(void *ip, unsigned rxh, uint8_t *bp,
                                           size_t n) {
  frame_io_i *self = (frame_io_i *)ip;

  return self->vmt->read_rxh(ip, rxh, bp, n);
}

/**
 * @memberof    frame_io_i
 * @public
 *
 * @brief       Reads data sequentially from a received frame.
 *
 * @param[in,out] ip            Pointer to a @p frame_io_i instance.
 * @param[in]     txh           Transmit handle.
 * @param[in]     bp            Transmit data buffer pointer.
 * @param[in]     n             Number of bytes to write.
 * @return                      The number of bytes written into the handle
 *                              buffer this value can be less than the amount
 *                              specified in the parameter @p size if the
 *                              maximum frame size is reached.
 */
CC_FORCE_INLINE
static inline ssize_t frmWriteTransmitHandle(void *ip, unsigned txh,
                                             const uint8_t *bp, size_t n) {
  frame_io_i *self = (frame_io_i *)ip;

  return self->vmt->write_txh(ip, txh, bp, n);
}

/**
 * @memberof    frame_io_i
 * @public
 *
 * @brief       Direct access to the receive handle buffer.
 *
 * @param[in,out] ip            Pointer to a @p frame_io_i instance.
 * @param[in]     rxh           Receive handle.
 * @param[out]    sizep         Size of the received frame.
 * @return                      Pointer to the received frame buffer or @p NULL
 *                              if the driver does not support memory-mapped
 *                              direct access.
 */
CC_FORCE_INLINE
static inline const uint8_t *frmGetReceiveBuffer(void *ip, unsigned rxh,
                                                 size_t *sizep) {
  frame_io_i *self = (frame_io_i *)ip;

  return self->vmt->get_rxh_buffer(ip, rxh, sizep);
}

/**
 * @memberof    frame_io_i
 * @public
 *
 * @brief       Direct access to the transmit handle buffer.
 *
 * @param[in,out] ip            Pointer to a @p frame_io_i instance.
 * @param[in]     txh           Transmit handle.
 * @param[out]    sizep         Maximum size of the transmit buffer.
 * @return                      Pointer to the transmit frame buffer or @p NULL
 *                              if the driver does not support memory-mapped
 *                              direct access.
 */
CC_FORCE_INLINE
static inline uint8_t *frmGetTransmitBuffer(void *ip, unsigned txh,
                                            size_t *sizep) {
  frame_io_i *self = (frame_io_i *)ip;

  return self->vmt->get_txh_buffer(ip, txh, sizep);
}
/** @} */

#endif /* HAL_FRAME_IO_H */

/** @} */

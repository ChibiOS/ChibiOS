/*
    ChibiOS - Copyright (C) 2006..2023 Giovanni Di Sirio

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
 * @file        oop_sequential_stream.h
 * @brief       Generated Sequential Stream Interface header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  OOP_SEQUENTIAL_STREAM
 * @brief       Sequential data streams interface.
 * @{
 */

#ifndef OOP_SEQUENTIAL_STREAM_H
#define OOP_SEQUENTIAL_STREAM_H

#include "oop_base_interface.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Streams return codes
 * @{
 */
#define STM_OK                              MSG_OK
#define STM_TIMEOUT                         MSG_TIMEOUT
#define STM_RESET                           MSG_RESET
/** @} */

#if (defined(OOP_USE_LEGACY)) || defined (__DOXYGEN__)
/**
 * @name    Legacy interface method names
 * @{
 */
#define streamWrite                         stmWrite
#define streamRead                          stmRead
#define streamPut                           stmPut
#define streamGet                           stmGet
/** @} */
#endif /* defined(OOP_USE_LEGACY) */

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
 * @interface   sequential_stream_i
 * @extends     base_interface_i.
 *
 * @brief       Sequential data streams interface.
 * @details     This module define an interface for generic sequential data
 *              streams.
 *              This interface is meant to be implemented in classes requiring
 *              streaming capability.
 * @note        This interface is meant to be compatible with legacy HAL @p
 *              BaseSequentialStream interface.
 *
 * @name        Interface @p sequential_stream_i structures
 * @{
 */

/**
 * @brief       Type of a sequential stream interface.
 */
typedef struct sequential_stream sequential_stream_i;

/**
 * @brief       Interface @p sequential_stream_i virtual methods table.
 */
struct sequential_stream_vmt {
  /* Memory offset between this interface structure and begin of
     the implementing class structure.*/
  size_t instance_offset;
  /* From base_interface_i.*/
  /* From sequential_stream_i.*/
  size_t (*write)(void *ip, const uint8_t *bp, size_t n);
  size_t (*read)(void *ip, uint8_t *bp, size_t n);
  msg_t (*put)(void *ip, uint8_t b);
  msg_t (*get)(void *ip);
};

/**
 * @brief       Structure representing a sequential stream interface.
 */
struct sequential_stream {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct sequential_stream_vmt *vmt;
};
/** @} */

#if (defined(OOP_USE_LEGACY)) || defined (__DOXYGEN__)
/**
 * @brief       For compatibility with legacy @p BaseSequentialStream.
 */
typedef sequential_stream_i BaseSequentialStream;
#endif /* defined(OOP_USE_LEGACY) */

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
 * @name        Virtual methods of sequential_stream_i
 * @{
 */
/**
 * @memberof    sequential_stream_i
 * @public
 *
 * @brief       Sequential Stream write.
 * @details     This function writes data from a buffer to a stream.
 *
 * @param[in,out] ip            Pointer to a @p sequential_stream_i instance.
 * @param[in]     bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @return                      The number of bytes transferred. The returned
 *                              value can be less than the specified number of
 *                              bytes if an end-of-file condition has been met.
 */
CC_FORCE_INLINE
static inline size_t stmWrite(void *ip, const uint8_t *bp, size_t n) {
  sequential_stream_i *self = (sequential_stream_i *)ip;

  return self->vmt->write(ip, bp, n);
}

/**
 * @memberof    sequential_stream_i
 * @public
 *
 * @brief       Sequential Stream read.
 * @details     This function reads data from a stream into a buffer.
 *
 * @param[in,out] ip            Pointer to a @p sequential_stream_i instance.
 * @param[out]    bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @return                      The number of bytes transferred. The returned
 *                              value can be less than the specified number of
 *                              bytes if an end-of-file condition has been met.
 */
CC_FORCE_INLINE
static inline size_t stmRead(void *ip, uint8_t *bp, size_t n) {
  sequential_stream_i *self = (sequential_stream_i *)ip;

  return self->vmt->read(ip, bp, n);
}

/**
 * @memberof    sequential_stream_i
 * @public
 *
 * @brief       Sequential Stream blocking byte write.
 * @details     This function writes a byte value to a stream. If the stream is
 *              not ready to accept data then the calling thread is suspended.
 *
 * @param[in,out] ip            Pointer to a @p sequential_stream_i instance.
 * @param[in]     b             The byte value to be written to the stream.
 * @return                      The operation status.
 */
CC_FORCE_INLINE
static inline msg_t stmPut(void *ip, uint8_t b) {
  sequential_stream_i *self = (sequential_stream_i *)ip;

  return self->vmt->put(ip, b);
}

/**
 * @memberof    sequential_stream_i
 * @public
 *
 * @brief       Sequential Stream blocking byte read.
 * @details     This function reads a byte value from a stream. If the data is
 *              not available then the calling thread is suspended.
 *
 * @param[in,out] ip            Pointer to a @p sequential_stream_i instance.
 * @return                      A byte value from the stream.
 */
CC_FORCE_INLINE
static inline msg_t stmGet(void *ip) {
  sequential_stream_i *self = (sequential_stream_i *)ip;

  return self->vmt->get(ip);
}
/** @} */

#endif /* OOP_SEQUENTIAL_STREAM_H */

/** @} */
